#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sysexits.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <errno.h>
#include "roboctl.h"

/* Keep <= PIC_ERASE_BLOCK_SIZE to ensure that all flash is erased before
   writing. */
#define VEX_WRITE_CLUSTER_SIZE    64

/****************************************************************************
 * Description: 
 *  Upload a .hex file to the VEX controller.
 *
 *  Line format of hex file is ": ll aaaa 00 data -checksum"
 *      ll = len (1 byte)
 *      aaaa = address (2 bytes)
 *      Note that the checksum is negated (2's comp)
 *      Ex. :02080600743349
 * Author: 
 ***************************************************************************/

rct_status_t    vex_upload_program(rct_pic_t *pic, char *hexfile_name)

{
    FILE            *fp;
    char            buff[PIC_HEX_LINE_MAX+1],
		    /* VEX User program memory = 0x0800 to 0x7ffd.
		       0x7ffe and 0x7fff are read-only. This gives
		       0x77fe bytes to work with, which we round
		       to 0x7800 to make an integral # of 64-byte
		       blocks. */
		    code[0x7800];
    unsigned long   c,
		    address,
		    start_address,
		    end_address,
		    cluster_offset,
		    len,
		    erase_blocks,
		    write_blocks,
		    blocks_per_write;
    int             val,
		    checksum = 0;
    extern int      Debug; 
    struct timeval  tp_start,tp_stop;   /* Time the upload and report speed */
    double          write_time;

    fp = fopen(hexfile_name, "r");
    if ( fp == NULL )
    {
	fprintf(stderr, "vex_upload_program(): Cannot open %s.\n", hexfile_name);
	return -1;
    }

    /* 
     * Get starting and ending addresses. HEX file addresses need not be
     * in order with this read/rewind/read method.
     */
    start_address = 0x7fff;     /* Prime to highest possible address */
    end_address = 0;            /* Prime to lowest possible address */
    while ( fgets(buff, PIC_HEX_LINE_MAX, fp) != NULL )
    {
	address = hex_val(buff+3, 4);
	if ( address != 0 )
	{
	    len = hex_val(buff+1, 2);
	    if ( address < start_address )
		start_address = address;
	    if ( address+len > end_address )
		end_address = address+len;
	}
    }
    debug_printf("vex_upload_program(): Start address: %06lX  End address: %06lX\n", 
	    start_address, end_address);

    /* If the end address is within range, the end of the last block
       will be as well, as long as VEX_WRITE_CLUSTER_SIZE <=
       PIC_ERASE_BLOCK_SIZE. */
    if ( !vex_valid_program_range(start_address, end_address,
	    "vex_upload_program") )
	return -1;

    write_blocks = (end_address - start_address) / VEX_WRITE_CLUSTER_SIZE + 1;
    blocks_per_write = VEX_WRITE_CLUSTER_SIZE / PIC_WRITE_BLOCK_SIZE;

    /* Initialize code array so unused bytes contain 0xff. This will
       prevent them from being altered by write, and save a little 
       wear on the flash RAM. The erase command sets all bits to 1,
       and write only resets the bits that are 0 in the buffer. */
    memset(code, 0xff, write_blocks * VEX_WRITE_CLUSTER_SIZE);

    /* Load HEX file to buffer */
    rewind(fp);
    while ( fgets(buff, PIC_HEX_LINE_MAX, fp) != NULL )
    {
	/* Parse line.  See function comment for format. */
	address = hex_val(buff+3, 4);
	if ( address != 0 )
	{
	    len = hex_val(buff+1, 2);
	    debug_printf("%02X %04lX: ", len, address);
	    for (c=0; c < len; ++c)
	    {
		val = hex_val(buff+9+c*2, 2);
		checksum -= val;
		code[address - start_address + c] = val;
	    }
	    debug_hex_dump(code + address - start_address, len);
	}
    }
    fclose(fp);

    vex_set_program_mode(pic);
    
    /* Erase code area */
    erase_blocks = (end_address - start_address) / PIC_ERASE_BLOCK_SIZE + 1;
    printf("Program size is %lu bytes.\n", end_address - start_address);
    printf("Erasing %lu blocks (64 bytes/block, %lu bytes total)...\n",
	    erase_blocks, erase_blocks * PIC_ERASE_BLOCK_SIZE);
    pic_erase_program_mem(pic, start_address, end_address);
    //pic_read_program_mem(pic, start_address, 32);
    
    /* Write code to PIC. */
    printf("Writing %lu clusters (8 bytes/block, 8 blocks/cluster, %lu bytes total)\n",
	    write_blocks, write_blocks * VEX_WRITE_CLUSTER_SIZE);
    gettimeofday(&tp_start,NULL);
    for (c=0; c < write_blocks; ++c)
    {
	cluster_offset = c * VEX_WRITE_CLUSTER_SIZE;
	address = start_address + cluster_offset;
	
	if ( Debug )
	    putchar('\n');
	pic_write_program_mem(pic, address, blocks_per_write, code + cluster_offset);

	printf("Bytes: %4lu\r",cluster_offset + VEX_WRITE_CLUSTER_SIZE);
	fflush(stdout);
    }
    gettimeofday(&tp_stop,NULL);
    write_time = (tp_stop.tv_sec - tp_start.tv_sec) + 
		(tp_stop.tv_usec - tp_start.tv_usec) / 1000000.0;
    cluster_offset = c * VEX_WRITE_CLUSTER_SIZE;
    printf("\n%lu bytes written in %f seconds (%0.2f bytes/sec)\n",
	    c * VEX_WRITE_CLUSTER_SIZE, write_time,
	    c * VEX_WRITE_CLUSTER_SIZE / write_time);
    putchar('\n');

    pic_return_to_user_code(pic);
    return RCT_OK;
}


rct_status_t    vex_status(rct_pic_t *pic)

{
    rct_status_t    status;
    
    vex_set_program_mode(pic);
    status = pic_print_bootloader_version(pic);
    pic_return_to_user_code(pic);
    
    return status;
}


int     vex_valid_program_range(unsigned long start_address,
				unsigned long end_address,
				char *caller)

{
    return pic_valid_program_range(start_address, end_address, 0x0800, 0x7ffd, caller);
}


void    rs232_high(int fd,int pin_mask)

{
    int     state;
    
    /* RS232 uses negative logic: Set state bit to 0 to set the line to 1. */
    ioctl(fd, TIOCMGET, &state);
    state &= ~pin_mask;
    ioctl(fd, TIOCMSET, &state);
}


void    rs232_low(int fd,int pin_mask)

{
    int     state;
    
    /* RS232 uses negative logic: Set state bit to 1 to set the line to 0. */
    ioctl(fd, TIOCMGET, &state);
    state |= pin_mask;
    ioctl(fd, TIOCMSET, &state);
}


time_t  difftimeofday(
	struct timeval *t1, /* Newer time value */
	struct timeval *t2  /* Older time value */
       )
	    
{
    return 1000000 * (t1->tv_sec - t2->tv_sec) + (t1->tv_usec - t2->tv_usec);
}


void    vex_set_program_mode(rct_pic_t *pic)

{
#if 1
    puts("Make sure the VEX controller is turned on.");
    puts("Press the button on the programming module until the PGRM STATUS button flashes.");
    puts("Then press return...");
    getchar();
#else
    struct timeval  assert1,
		    deassert1,
		    assert2,
		    deassert2,
		    assert3,
		    deassert3;
    char    buff[2] = "\377";

    /*
     *  Using usleep() like this runs a risk of drifting from the
     *  desired times, but it's probably close enough.  The alternative
     *  is to watch the timer using getitimer() or gettimeofday().
     *
     *  Note: RS232 uses negative logic: assert -> low, deassert -> high
     */

    /*
     *  New approach
     */

    //puts("Dropping RTS and CTS...");
    /* Everything low except TD before beginning the prog init sequence */
    //rs232_low(pic->fd, TIOCM_LE);
    //rs232_low(pic->fd, TIOCM_DTR);
    //rs232_low(pic->fd, TIOCM_DSR);
    rs232_low(pic->fd, TIOCM_RTS|TIOCM_CTS);
    //rs232_high(pic->fd, TIOCM_ST);    /* Is this the same as TD? */
    //rs232_low(pic->fd, TIOCM_SR);      /* Is this the same as RD? */
    //rs232_low(pic->fd, TIOCM_DCD);
    //rs232_low(pic->fd, TIOCM_RI);
    //getchar();
    
    
    usleep(250000);
    
    /* Raise TD? */
    // Causes auto-trigger to work the second time after a reboot of
    // the controller.  The first try just hangs
    // write(pic->fd, buff, 1);
    
    usleep(250000);
    
    //puts("Raising RTS and CTS...");
    /* Begin sequence by raising RTS and CTS at the same time */
    rs232_high(pic->fd, TIOCM_RTS|TIOCM_CTS);
    //rs232_high(pic->fd, TIOCM_CTS);
    //getchar();

    /* Read ack from controller? */
    
    /* Drop RTS */
    usleep(25000);
    rs232_low(pic->fd, TIOCM_RTS);
    
    /* Drop CTS */
    usleep(25000);
    rs232_low(pic->fd, TIOCM_CTS);
    
    /* Drop TD */
    // rs232_low(pic->fd, TIOCM_ST);  /* Is this the same as TD? */
    
    usleep(250000);
#endif

#if 0
    printf("%u %lu %lu %lu %lu %lu\n",
	0,
       (unsigned long)difftimeofday(&deassert1,&assert1),
       (unsigned long)difftimeofday(&assert2,&assert1),
       (unsigned long)difftimeofday(&deassert2,&assert1),
       (unsigned long)difftimeofday(&assert3,&assert1),
       (unsigned long)difftimeofday(&deassert3,&assert1));
#endif
}


rct_status_t    vex_open_controller(rct_pic_t *pic, char *device)

{
    int     status;

    if ( (status = pic_open_controller(pic,device)) != RCT_OK )
	return status;

    /* close() on Mac and Linux (but not FreeBSD) sends the VEX
       into programming mode, as if the dongle button had been pressed.
       Causes FreeBSD 8.0 to fail unless --debug is used.  OK on
       earlier versions.
     */

#ifndef __FreeBSD__
    //rs232_high(pic->fd,TIOCM_CTS);
#endif
    
    /*
     *  Discard leftover characters buffered by the serial driver or
     *  hardware from previous vex output.  This is particularly
     *  a problem with OS X /dev/cu.usbserial.
     */
    
    /* Delay required on FreeBSD after opening port */
    usleep(100000);
    
    /*
     *  Might be input in the buffer from a previous upload or monitor
     *  that was unceremoniously disconnected.
     */
    serial_eat_leftovers(pic->fd);
    
    return RCT_OK;
}


void    vex_close_controller(rct_pic_t *pic)

{
    /* 
     *  Empty serial input, or it might get picked up by the next
     *  vexctl command.  Should do this at startup as well.
     */
    serial_eat_leftovers(pic->fd);
    
    /* Make sure RTS and CTS are down? */
    //rs232_high(pic->fd, TIOCM_CTS);
    //rs232_low(pic->fd, TIOCM_RTS);
    
    // There might be something else we need to do here.  Stay tuned...
    pic_close_controller(pic);
}


void    serial_eat_leftovers(int fd)

{
    int     original_flags,
	    nonblock_flags;
    char    ch;
    
    /* Set non-blocking mode */
    original_flags = fcntl(fd, F_GETFL, 0);
    nonblock_flags = original_flags | O_NONBLOCK;
    fcntl(fd, F_SETFL, nonblock_flags);
    
    /*
     *  Read characters until a time-out occurs waiting for input
     *  sleep value determined by trial-and-error.  300 was the
     *  lowest increment of 100 that seemed to work reliably.
     */
    
    //printf("Leftovers:\n");
    while ( read(fd, &ch, 1) != -1 )
    {
	usleep(1000);
	// putchar(ch);
    }

    /* Restore fd flags */
    fcntl(fd, F_SETFL, original_flags);
}

