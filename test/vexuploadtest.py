import unittest
import vexupload
import serial
from vexupload import CHAR_ESC, CHAR_ETX, CHAR_STX, Command

packet_header = (CHAR_STX, CHAR_STX)

class PacketTest(unittest.TestCase):

    def setUp(self):
        pass

    def test_read_program_mem(self):
        arguments = (5, 0x06, 0x0F, 0x03)
        data = (0xFF, 0xEF, 0x45, 0x65, 0x34)
        
        checksum = -(sum(arguments) + sum(data) + Command.read_program_mem) & 0xff # @UndefinedVariable
        
        packet = vexupload.Packet.from_response(packet_header + (Command.read_program_mem,) + arguments + data + (checksum,) + (CHAR_ETX,))  # @UndefinedVariable
        
        assert packet.command == Command.read_program_mem
        assert packet.arguments == arguments
        assert packet.data == data

class SerialTest(unittest.TestCase):
    
    def setUp(self):
        # Create loopback serial connection
        self.serial_conn = serial.serial_for_url("loop://")
        
    def test_read_response(self):        
        self.serial_conn.write(packet_header + (CHAR_ETX,))
        
        expected_response = bytearray(packet_header + (CHAR_ETX,))
        response = vexupload.read_response(self.serial_conn)
        assert expected_response == response, "Did not read correct response, recieved: %s, expected: %s" % (response, expected_response)

    def test_read_response_escape(self):
        self.serial_conn.write(packet_header + (CHAR_ESC, CHAR_ESC, 0x45, CHAR_ESC, CHAR_ETX, CHAR_ETX))
        
        expected_response = bytearray(packet_header + (CHAR_ESC, 0x45, CHAR_ETX, CHAR_ETX))
        response = vexupload.read_response(self.serial_conn)
        assert expected_response == response, "Did not read correct response, recieved: %s, expected: %s" % (response, expected_response)
        
    def test_erase_program_mem(self):
        vexupload.erase_program_mem(self.serial_conn, 0x800, 256)
        

if __name__ == "__main__":
    unittest.main()
