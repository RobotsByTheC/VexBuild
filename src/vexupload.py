#!/usr/bin/env python3
from enum import IntEnum, Enum
import enum
from pathlib import Path
import sys
import serial.tools.list_ports
import binascii
import textwrap

MAX_PACKET_LENGTH = 255

MIN_PROGRAM_ADDRESS = 0x0800
MAX_PROGRAM_ADDRESS = 0x7ffd

WRITE_CLUSTER_SIZE = 64
WRITE_BLOCK_SIZE = 8

# A very conservative value, that assumes every single byte has to be escaped (and then some)
MAX_READ_LENGTH = 100

ERASE_ROW_SIZE = 64
MAX_ERASE_ROWS = 128

CHAR_STX = 0x0F
CHAR_ETX = 0x04
CHAR_ESC = 0x05

# Commands
@enum.unique
class Command(IntEnum):
    erase_program_mem = 0x09
    write_program_mem = 0x02
    read_program_mem = 0x01
    return_to_user_code = 0x08
    
@enum.unique
class DebugLevel(Enum):
    none = 0
    verbose = 1
    debug = 2
    insane = 3
    
debug_level = DebugLevel.none

class HexException(Exception):
    """Exception raised for problems with the hex file.

    Attributes:
        hex_file -- the file that caused the error
        message -- explanation of the error
    """

    def __init__(self, hex_file, message):
        self.hex_file = hex_file
        self.message = message
        
class Packet(object):
    def __init__(self, command, arguments, data):
        self.__checksum = 0
        self.__command = None
        self.__arguments = None
        self.__data = None
        self.command = command
        self.arguments = arguments
        self.data = data
        
    @property
    def command(self):
        return self.__command
    
    @command.setter
    def command(self, command):
        if self.__command:
            self.__checksum += self.__command.value
        self.__command = command
        self.__checksum -= command.value
        
    @property
    def arguments(self):
        return self.__arguments
    
    @arguments.setter
    def arguments(self, arguments):
        if self.__arguments:
            self.__checksum += sum(self.__arguments)
        self.__arguments = arguments
        if arguments:
            self.__checksum -= sum(arguments)
        
    @property
    def data(self):
        return self.__data
    
    @data.setter
    def data(self, data):
        if self.__data:
            self.__checksum += sum(self.__data)
        self.__data = data
        if data:
            self.__checksum -= sum(data)
    
    @property
    def checksum(self):
        return self.__checksum & 0xff
    
    @staticmethod
    def from_response(response):
        # Perform stringent integrity checks on the packet
        response_len = len(response)
        if response_len < 9:
            raise IOError("Packet too short to be valid. Received %i bytes, expected at least 9 bytes" % response_len)
    
        if not (response[0] == CHAR_STX and response[1] == CHAR_STX):
            raise IOError("Packet did not begin with two STX bytes, instead it had %s" % response[:2])
    
        if response[-1] != CHAR_ETX:
            raise IOError("Packet did not end with an ETX byte, instead it had %#06x" % response[-1])
    
        checksum = response[-2]
        command = Command(response[2])
    
        if command == Command.read_program_mem:
            arguments = response[3:7]
            data_len = response[3]
            if (data_len + 9) < response_len:
                raise IOError("Packet does not contain all requested program memory data: expected %i, got %i" % (data_len, response_len - 9))
            data = response[7:7 + data_len]
    
        packet = Packet(command, arguments, data)
        
        if packet.checksum != checksum:
            raise IOError("Checksum does not match data: calculated %#02x, received %#02x" % (packet.checksum, checksum))

        return packet

def upload(hex_file, serial_port=None):
    debug("upload(): hex_file=%s, serial_port=%s" % (hex_file, serial_port))
    # Vex uses 115200 bits/sec, no parity, 8 data bits, 1 stop bit
    if serial_port == None:
        ports = serial.tools.list_ports.grep("2303")
        try:
            serial_port = next(ports)[0]
        except:
            serial_port = 0

    debug("upload(): Using serial port: %s" % serial_port)
    serial_conn = serial.Serial(serial_port, baudrate=115200, timeout=3)
    # Loop device for testing
#     serial_conn = serial.serial_for_url("loop://")
    serial_conn.flushInput()
     
    # Load hex file   
    with Path(hex_file).open("r") as fd:
        start_address = MAX_PROGRAM_ADDRESS;  # Initialize to highest possible address
        end_address = 0;  # Initialize to lowest possible address 
            
        for line in fd:
            line = line.strip()
            
            address = int(line[3:7], 16);
            if address != 0:
                line_len = int(line[1:3], 16);
                if address < start_address:
                    start_address = address
                if address + line_len > end_address:
                    end_address = address + line_len
        
        debug("upload(): Start address: %#06x, End address: %#06x" % (start_address, end_address), DebugLevel.debug);
        
        # Make sure hex file addresses are within the correct range
        check_program_range(hex_file, start_address, end_address)
        
        program_length = end_address - start_address

        
        code = bytearray((0xff,) * program_length)
        
        # Return to beginning of file
        fd.seek(0)
        
        for line in fd:
            line = line.strip()
            
            address = int(line[3:7], 16);
            if address != 0:
                line_len = int(line[1:3], 16);
                code_offset = address - start_address
                
                for c in range(line_len):
                    pos = 9 + c * 2
                    val = int(line[pos:pos + 2], 16)
                    code[code_offset + c] = val
                # Calculate checksum based on data read from file
                computed_checksum = -(sum(code[code_offset:code_offset + line_len])+line_len + (address & 0xff) + ((address >> 8) & 0xff)) & 0xff
                # Read checksum from line
                line_checksum = int(line[-2:], 16)
                if computed_checksum != line_checksum:
                    raise HexException(hex_file, "Hex file checksum verification failed: computed=%#04x, expected=%#04x" % (computed_checksum,line_checksum))

    set_program_mode()
    
    erase_rows = program_length // ERASE_ROW_SIZE
    if program_length % ERASE_ROW_SIZE != 0: erase_rows += 1
    info("\nProgram size is %i bytes." % program_length)
    info("Erasing %i rows (%i bytes/row, %i bytes total)...\n" % 
        (erase_rows, ERASE_ROW_SIZE, program_length))
        
    erase_program_mem(serial_conn, start_address, erase_rows * ERASE_ROW_SIZE)
    
    write_blocks = program_length // WRITE_CLUSTER_SIZE
    if program_length % WRITE_CLUSTER_SIZE != 0: write_blocks += 1
    info("Writing %i clusters (8 bytes/block, 8 blocks/cluster, %i bytes total)" % 
        (write_blocks, write_blocks * WRITE_CLUSTER_SIZE))
    
    write_program_mem(serial_conn, start_address, code)
        
    return_to_user_code(serial_conn)
        
def check_program_range(hex_file, start_address, end_address):
    if end_address < start_address:
        raise HexException(hex_file, "End address (%#06x) is less than start address (%#06x)" % (end_address, start_address))
    if (start_address < MIN_PROGRAM_ADDRESS) or (end_address > MAX_PROGRAM_ADDRESS):
        raise HexException(hex_file, """Valid program addresses are %#08x to %#08x.
Start and end addresses received are %#08x to %#08x.""" % (MIN_PROGRAM_ADDRESS, MAX_PROGRAM_ADDRESS, start_address, end_address))

def is_valid_address(address):
    return address <= MAX_PROGRAM_ADDRESS and address >= MIN_PROGRAM_ADDRESS
 
def set_program_mode():
    info("Make sure the VEX controller is turned on.")
    info("Press the button on the programming module until the PGRM STATUS button flashes.")
    input("Then press return...")

def erase_program_mem(serial_conn, address, length):
    debug("erase_program_mem(): address=%#08x, length=%i" % (address, length), DebugLevel.debug)
    # Make sure the caller is only trying to erase to the boundary of a block
    if (length % ERASE_ROW_SIZE) != 0:
        raise IOError("Erase length must be a multiple of %i" % ERASE_ROW_SIZE)

    total_rows = length // ERASE_ROW_SIZE

    # If low_len is 0 (e.g. len = 256) the controller just goes to the IFI> prompt.
    # Work around this by erasing erase_rows less than 256 bytes, so that low_len
    # is always > 0 and high_len is always 0.
    curr_addr = address
    while total_rows > 0:
        erase_rows = min(MAX_ERASE_ROWS, total_rows) 
        total_rows -= erase_rows
        erase_length = erase_rows * ERASE_ROW_SIZE
        debug("erase_program_mem(): Erasing %i rows at %#08x" % (erase_rows, curr_addr))
        
        assert is_valid_address(curr_addr)
        assert is_valid_address(curr_addr + erase_length)
        
        send_command(serial_conn, Packet(Command.erase_program_mem,
                    (erase_rows & 0xff,
                    curr_addr & 0xff,
                    (curr_addr >> 8) & 0xff,
                    (curr_addr >> 16) & 0xff,
                    0),
                    None))
        
        curr_addr += erase_length

def read_program_mem(serial_conn, address, length):
    if length > MAX_READ_LENGTH:
        raise ValueError("Can only read a maximum of %i bytes at a time, attempted to read %i" % (MAX_READ_LENGTH, length))
    
    packet = Packet.from_response(send_command(serial_conn, Packet(Command.read_program_mem, (length,
                    address & 0xff,
                    (address >> 8) & 0xff,
                    (address >> 16) & 0xff,
                    None))))
    
    return packet.data

def write_program_mem(serial_conn, address, code):
    debug("write_program_mem(): address=%#08x, length=%i" % (address, len(code)))
    debug("code:\n%s" % textwrap.fill(hex_dump(code), 100), DebugLevel.insane)
    
    # Align the code to the nearest block
    address_block_offset = address % WRITE_BLOCK_SIZE
    code[0:0] = (0xff,) * address_block_offset
    address -= address_block_offset
    assert address % WRITE_BLOCK_SIZE == 0, "Code start not aligned to block"
    
    code_len = len(code)
    total_blocks = code_len // WRITE_BLOCK_SIZE
    # If the code is not aligned to a block, increment the block count
    if (code_len % WRITE_BLOCK_SIZE) != 0:
        total_blocks += 1
    
    # Pad the code with 0xff so it is aligned with a block. The extra 0xff will
    # not actually modify the flash, since 1 can only be written with erase.
    code.extend((0xff,) * ((total_blocks * WRITE_BLOCK_SIZE) - code_len))
    
    code_len = len(code)
    assert code_len % WRITE_BLOCK_SIZE == 0, "Code end not aligned to block"
    
    curr_addr = address
    while total_blocks > 0:
        write_blocks = min(WRITE_CLUSTER_SIZE // WRITE_BLOCK_SIZE, total_blocks) 
        total_blocks -= write_blocks
        debug("write_program_mem(): Writing %i blocks at %#06x" % (write_blocks, curr_addr))
        
        assert is_valid_address(curr_addr)
        assert is_valid_address(curr_addr + WRITE_CLUSTER_SIZE)
        
        code_offset = curr_addr - address
        
        send_command(serial_conn, Packet(Command.write_program_mem,
                    (write_blocks,
                    curr_addr & 0xff,
                    (curr_addr >> 8) & 0xff,
                    (curr_addr >> 16) & 0xff),
                    code[code_offset:code_offset + (write_blocks * WRITE_BLOCK_SIZE)]))
        curr_addr += WRITE_CLUSTER_SIZE
    
def return_to_user_code(serial_conn):
    return send_command(serial_conn, Packet(Command.return_to_user_code,
                    (0x40,),
                    None), 0x40)

def send_command(serial_conn, packet, response_etx=CHAR_ETX):
    debug("send_command(): command=%s, arguments=%s, data=%s" % (packet.command, hex_dump(packet.arguments), hex_dump(packet.data) if packet.data else None),
          DebugLevel.debug)
    
    payload = bytearray()
    checksum = 0
    
    # Build payload    
    checksum -= packet.command.value
    payload.append(packet.command.value)
    checksum -= sum(packet.arguments)
        
    payload.extend(packet.arguments)
    
    if packet.data:
        checksum -= sum(packet.data)
        payload.extend(packet.data)
    payload.append(packet.checksum)
    
    # Escape special characters in payload
    escape_payload(payload)
    
    # Add the STX and EXT bytes, which shouldn't be escaped
    payload.insert(0, CHAR_STX)
    payload.insert(0, CHAR_STX)
    payload.append(CHAR_ETX)
    
    # Packets can have a maximum of 255 bytes
    if len(payload) > MAX_PACKET_LENGTH:
        raise IOError("Tried to sent a %i byte packet. The maximum length is %i." % (len(payload), MAX_PACKET_LENGTH))
    
    sent = serial_conn.write(payload)
    serial_conn.flush()

    if sent != len(payload):
        raise IOError("Error sending command. %i bytes to write, sent %i." % (len(payload), sent))
    
    return read_response(serial_conn, response_etx)

def escape_payload(payload):
    i = 0
    while i < len(payload):
        char = payload[i]
        if char == CHAR_STX or char == CHAR_ETX or char == CHAR_ESC:
            payload.insert(i, CHAR_ESC)
            i += 1
        i += 1

def read_response(serial_conn, etx=CHAR_ETX):
    debug("read_response(): etx=%#04x" % etx, DebugLevel.debug)
    response = bytearray()
    esc = False
    
    while True:
        data = serial_conn.read()
        if len(data) == 1:
            data_val = data[0]
            if not esc:
                if data_val == CHAR_ESC:
                    esc = True
                else:
                    response.append(data_val)
                    if data_val == etx:
                        break
            else:
                esc = False
                response.append(data_val)
        else:
            raise IOError("Timeout while reading from Vex controller, response received so far: %s" % hex_dump(response))
    
    debug("read_response(): response=%s" % hex_dump(response), DebugLevel.debug)
    
    return response
   
def info(msg):
    print(msg, flush=True)

def debug(msg, level=DebugLevel.verbose):
    if debug_level.value >= level.value:
        info(msg)
        
def hex_dump(data):
    return str(binascii.hexlify(bytes(data)), sys.getdefaultencoding())
    
def parse_args():
    import argparse
    parser = argparse.ArgumentParser()
    
    parser.add_argument("--debug", help="debug level", default="none")
    parser.add_argument("--dev", help="Use serial port dev instead of the default", default=None)
    parser.add_argument("hex_file", help="Hex file to upload")
        
    return parser.parse_args()

if __name__ == "__main__":
    args = parse_args()
    
    debug_level = DebugLevel[args.debug]
    
    upload(args.hex_file, args.dev)
