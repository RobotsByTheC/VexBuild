#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <sysexits.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include "roboctl.h"

/**
 *  
 */

rct_status_t    pic_send_command(int fd,const char *raw_cmd,int raw_len,
			const char *data,int dlen,char *response,
			int eot)

{
    char        packet[PIC_CMD_MAX+1],
		payload[PIC_CMD_MAX+1],
		checksum = 0;
    int         payload_len,
		packet_len,
		c,
		sent;
    
    debug_printf("pic_send_command(): raw_cmd: ");
    debug_hex_dump(raw_cmd,raw_len);
    
    /* Build payload */
    for (payload_len=0; payload_len < raw_len; ++payload_len)
    {
	checksum -= raw_cmd[payload_len];
	payload[payload_len] = raw_cmd[payload_len];
    }
    
    /* If write command, add data */
    if ( raw_cmd[0] == PIC_WRITE_PROGRAM_MEM )
    {
	if ( dlen > 31 )
	{
	    fprintf(stderr,"pic_send_data(): Cannot send more than 31 blocks.\n");
	    return -1;
	}
	
	dlen *= PIC_WRITE_BLOCK_SIZE;  /* 8 bytes in a write block */
	c = 0;
	while ( dlen-- > 0 )
	{
	    checksum -= data[c];
	    payload[payload_len++] = data[c++];
	}
    }
    
    /* Add checksum */
    payload[payload_len++] = checksum;
    
    debug_printf("Raw payload: ");
    debug_hex_dump(payload,payload_len);
    
    /* PIC protocol begins with 2 SI chars */
    packet[0] = packet[1] = CHAR_SI;

    /* Add payload (with special chars escaped) to packet */
    packet_len = 2 + memcpy_esc(packet+2,payload,payload_len);
    
    /* Add EOT to finish packet */
    packet[packet_len++] = CHAR_EOT;
    
    debug_printf("Command packet:  ");
    debug_hex_dump(packet,packet_len);
    
    /* Transmit to controller */
    if ( (sent = write(fd,packet,packet_len)) != packet_len )
    {
	fprintf(stderr,"Error sending command.  %d bytes to write, sent %d\n",
		packet_len,sent);
	return -1;
    }
    
    pic_read_response(fd,response,eot);
    
    return RCT_OK;
}


rct_status_t    pic_read_response(int fd,char *response,int eot)

{
    int     esc;
    char    *dest;
    
    /* Read response */
    dest = response;
    esc = 0;
    do
    {
	read(fd,dest,1);
	if ( *dest == CHAR_ESC )
	{
	    esc = 1;
	    read(fd,dest,1);
	}
	else
	{
	    esc = 0;
	}
	//debug_printf("%c ", *dest);
    }   while ( (dest-response < PIC_RESPONSE_MAX) && 
		((*dest++ != eot) || esc) );
    
    debug_printf("Response packet: ");
    debug_hex_dump(response,dest-response);
    return RCT_OK;
}


/**
 *  Dump a character string of length 'len' in HEX format.
 */

void    debug_hex_dump(const char *str,int len)

{
    const char    *p;
    extern int      Debug;
    
    if ( Debug )
    {
	for (p=str; p<str+len; ++p)
	    printf("0x%02X ",(unsigned char)*p);
	putchar('\n');
	fflush(stdout);
    }
}


int     memcpy_esc(char *dest,const char *src,int slen)

{
    char    *p = dest;
    
    while ( slen-- > 0 )
    {
	/* SI, EOT, and ESC in payload must be escaped */
	switch(*src)
	{
	    case    CHAR_SI:
	    case    CHAR_EOT:
	    case    CHAR_ESC:
		*p++ = CHAR_ESC;
		break;
	    default:
		break;
	}
	*p++ = *src++;
    }
    return p - dest;
}


rct_status_t    pic_erase_program_mem(rct_pic_t *pic,
			    unsigned long start_address,
			    unsigned long end_address)

{
    unsigned long   erase_blocks, addr, blocks;
    char            cmd[7];

    erase_blocks = (end_address - start_address) / PIC_ERASE_BLOCK_SIZE + 1;
    debug_printf("Program length: %lu  Erase blocks: %lu\n",
	    end_address - start_address,erase_blocks);
    debug_printf("Erasing %lu blocks at %06lX.\n",erase_blocks,start_address);
    
    /*
     *  If low_len is 0 (e.g. len = 256) the controller just goes to the IFI> prompt.
     *  Work around this by erasing blocks less than 256 bytes, so that low_len
     *  is always > 0 and high_len is always 0.
     */
    for (addr = start_address; erase_blocks > 0;
	addr += PIC_MAX_ERASE_BLOCKS * PIC_ERASE_BLOCK_SIZE)
    {
	blocks = MIN(PIC_MAX_ERASE_BLOCKS, erase_blocks);
	erase_blocks -= blocks;
	debug_printf("\nErasing %u blocks at 0x%3x\n", blocks, addr);
	snprintf(cmd,7,"%c%c%c%c%c%c",
		PIC_ERASE_PROGRAM_MEM,
		(int)blocks,                /* low_len */
		(int)(addr & 0xff),
		(int)((addr >> 8) & 0xff),
		(int)((addr >> 16) & 0xff),
		(int)0);                    /* high_len */
	debug_hex_dump(cmd,6);
	pic_send_command(pic->fd,cmd,6,NULL,0,pic->response,CHAR_EOT);
    }
    debug_printf("\n*** pic_erase_program_mem() ***\n");
    //getchar(); 
    return RCT_OK;
}


rct_status_t    pic_write_program_mem(rct_pic_t *pic,unsigned long address,
			    unsigned int blocks,char *code)

{
    char            cmd[6];
    
    if ( blocks > 255 )
    {
	fprintf(stderr,"pic_write_program_mem(): Tried to write %d blocks.  Maximum is 255.\n",blocks);
	return -1;
    }
    snprintf(cmd,5,"%c%c%c%c%c",
	    PIC_WRITE_PROGRAM_MEM,
	    blocks,
	    (int)(address & 0xff),
	    (int)((address >> 8) & 0xff),
	    (int)((address >> 16) & 0xff));
    debug_printf("\n*** pic_write_program_mem() ***\n");
    return pic_send_command(pic->fd,cmd,5,code,blocks,pic->response,CHAR_EOT);
}


rct_status_t    pic_get_bootloader_version(rct_pic_t *pic)

{
    char            cmd[2];
    rct_status_t    status;
    
    cmd[0] = PIC_GET_BOOTLOADER_VERSION;
    cmd[1] = 2;
    debug_printf("\n*** pic_get_bootloader_version() ***\n");
    status = pic_send_command(pic->fd,cmd,2,NULL,0,pic->response,CHAR_EOT);
    if ( status == RCT_OK )
    {
	pic->bootloader_major = pic->response[4];
	pic->bootloader_minor = pic->response[5];
    }
    return status;
}


rct_status_t pic_print_bootloader_version(rct_pic_t *pic)

{
    rct_status_t    status;
    
    if ( (status=pic_get_bootloader_version(pic)) == RCT_OK )
    {
	printf("Bootloader: %d.%d\n",pic->bootloader_major,
		pic->bootloader_minor);
    }
    return RCT_OK;
}


rct_status_t    pic_read_program_mem(rct_pic_t *pic,unsigned long address,
	    unsigned int bytes)

{
    char    cmd[5];
    
    snprintf(cmd,5,"%c%c%c%c%c",
	    PIC_READ_PROGRAM_MEM,
	    bytes,
	    (int)(address & 0xff),
	    (int)((address >> 8) & 0xff),
	    (int)((address >> 16) & 0xff));
    debug_printf("\n*** pic_read_program_mem() ***\n");
    return pic_send_command(pic->fd,cmd,5,NULL,0,pic->response,CHAR_EOT);
}


rct_status_t    pic_return_to_user_code(rct_pic_t *pic)

{
    char    cmd[2];
    rct_status_t    status;
    
    cmd[0] = PIC_RETURN_TO_USER_CODE;
    cmd[1] = 0x40;
    debug_printf("\n*** pic_return_to_user_code() ***\n");
    status = pic_send_command(pic->fd,cmd,2,NULL,0,pic->response,'\x40');
    
    /* Read extra trailing response bytes? */
    /*
    for (c=0; 1; ++c)
    {
	read(pic->fd, cmd, 1);
	fprintf(stderr, "Extra byte %d: %c\n", c, cmd[0]);
    }
    */
    return status;
}


unsigned long   hex_val(char *start,int digits)

{
    unsigned long   val;
    char    temp,
	    *end;
    
    temp = start[digits];
    start[digits] = '\0';
    val = strtol(start,&end,16);
    if ( end != start + digits )
    {
	fprintf(stderr,"Error converting hex value: %s\n",start);
	val = -1;
    }
    start[digits] = temp;
    return val;
}


rct_status_t    pic_reset(int fd,char *response)

{
    debug_printf("\n*** pic_reset() ***\n");
    return pic_send_command(fd,"\x00\x00",2,NULL,0,response,CHAR_EOT);
}


#if defined(__CYGWIN__) || defined(sun)
void    cfmakeraw(struct termios *t)
{
    t->c_iflag |= IGNBRK;           /* Ignore break condition */
    t->c_iflag &= ~(INLCR|ICRNL);   /* Disable CR/NL mappings */
    t->c_oflag &= ~OPOST;           /* Disable all output processing */
    t->c_cflag |= CREAD;
    t->c_cflag &= ~CRTSCTS;         /* Disable flow control */
}
#endif


rct_status_t    pic_open_controller(rct_pic_t *pic, char *device)

{
    pic->fd = open(device, O_RDWR|O_NOCTTY);
    if ( pic->fd == -1 )
    {
	fprintf(stderr, "Cannot open %s for read/write.\n", device);
	return EX_UNAVAILABLE;
    }

    /* Get tty attributes */
    if ( tcgetattr(pic->fd, &pic->original_port_settings) != 0 )
    {
	fprintf(stderr, "%s: tcgetattr() failed: %s\n",
	    __func__, strerror(errno));
	exit(EX_OSERR);
    }
    
    /* Put tty into raw mode (no buffering or char translations
       intended for terminals) */
    pic->current_port_settings = pic->original_port_settings;
    cfmakeraw(&pic->current_port_settings);

    /*
     *  Vex uses 115200 bits/sec, no parity, 8 data bits, 1 stop bit
     *  Assume same for other PIC devices for now.  Maybe add a tty
     *  settings argument to this function later if necessary.
     */
    cfsetispeed(&pic->current_port_settings, PIC_BAUD_RATE);
    cfsetospeed(&pic->current_port_settings, PIC_BAUD_RATE);
    pic->current_port_settings.c_cflag |= CS8|CLOCAL|CREAD;
    pic->current_port_settings.c_cflag &= ~CRTSCTS;
    //pic->current_port_settings.c_cflag &= ~(CCTS_OFLOW|CRTSCTS|CRTS_IFLOW|HUPCL);
    pic->current_port_settings.c_iflag |= IGNPAR;
    if ( tcsetattr(pic->fd, TCSANOW, &pic->current_port_settings) != 0 )
    {
	fprintf(stderr, "%s: tcsetattr() failed: %s\n",
	    __func__, strerror(errno));
	exit(EX_OSERR);
    }
    
    return RCT_OK;
}


rct_status_t    pic_close_controller(rct_pic_t *pic)

{
    /* Restore original port settings. */
    /* TCSAFLUSH makes the change after output buffers are flushed and
       discards unread input. */
    if ( tcsetattr(pic->fd,TCSAFLUSH,&pic->original_port_settings) != 0 )
    {
	fprintf(stderr, "%s: tcsetattr() failed: %s\n",
	    __func__, strerror(errno));
	exit(EX_OSERR);
    }

    close(pic->fd);
    return RCT_OK;
}


void    pic_init_struct(rct_pic_t *pic)

{
    pic->fd = -1;
    pic->device = NULL;
}


int     pic_valid_program_range(unsigned long start_address,
				unsigned long end_address,
				unsigned long valid_start,
				unsigned long valid_end,
				const char *caller)

{
    if ( end_address < start_address )
    {
	fprintf(stderr,"%s(): end_address %06lX"
			"is less than start_address: %06lX.\n",
			caller,end_address,start_address);
	return -1;
    }
    if ( (start_address >= valid_start) && (end_address <= valid_end) )
    {
	return 1;
    }
    else
    {
	fprintf(stderr,"%s(): Valid program address is %04lx to %04lx.\n",
		caller,valid_start,valid_end);
	fprintf(stderr,"Start and end addresses received are %04lx to %04lx.\n",
		start_address,end_address);
	return 0;
    }
}

