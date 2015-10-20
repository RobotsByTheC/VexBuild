
/****************************************************************************
 *  This file contains NXT-specific functions, which should generally
 *  not be called directly from application programs.  Using the rct_
 *  brick-independent API will result in more portable, lower-maintenance
 *  code.
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <usb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sysexits.h>
#include <stdarg.h>
#include <inttypes.h>
#include <errno.h>
#include "roboctl.h"

extern int  Debug;


/****************************************************************************
 * Description: 
 *  Open a connection to an NXT brick.  This must be done before issuing
 *  any direct or system commands.  If a USB connection is sensed, then
 *  USB will be used for the connection.  If not, an attempt is made to
 *  open a Bluetooth connection.
 * Author:  Jason W. Bacon
 ***************************************************************************/

rct_status_t nxt_open_brick(rct_nxt_t *nxt)

{
    if ( nxt->usb_dev != NULL )
	return nxt_open_brick_usb(nxt);
    else
	return nxt_open_brick_bluetooth(nxt);
}


/****************************************************************************
 * Description: 
 *  Open a USB connection to an NXT brick specified by nxt.
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 * Author: Jason W. Bacon
 ***************************************************************************/

rct_status_t nxt_open_brick_usb(rct_nxt_t *nxt)

{
    debug_printf("Opening USB connection: nxt=%p\n",nxt);
    /* No error return code listed in libusb docs.  Is this call
       supposed to always succeed? */
    nxt->usb_handle = usb_open(nxt->usb_dev);
    debug_printf("usb_open returned handle %p...\n", nxt->usb_handle);

    /*
     * ret = usb_set_configuration(brick->nxt.usb_handle, 1); if (ret < 0) {
     * usb_close(brick->nxt.usb_handle); return RCT_NXT_CONFIGURATION_ERROR; }
     */
    if ( usb_claim_interface(nxt->usb_handle, NXT_USB_INTERFACE) < 0 )
    {
	fprintf(stderr, "Error: %s(): Could not claim interface.\n", __func__);
	usb_close(nxt->usb_handle);
	return RCT_CANNOT_CLAIM_INTERFACE;
    }

    return RCT_OK;
}


/****************************************************************************
 * Description: 
 *  Open a Bluetooth connection to an NXT brick specified by nxt.
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 * Author: Jason W. Bacon
 ***************************************************************************/

rct_status_t nxt_open_brick_bluetooth(rct_nxt_t *nxt)

{
#if defined(__FreeBSD__) || defined(__linux__)
    /* FreeBSD and Linux bluetooth APIs are very similar.  The main
       differences are in the naming of some constants and structure
       members.  Even the identifiers that differ are very similar.
       (e.g. FreeBSD rfcomm_family vs. Linux rc_family)
       The FreeBSD identifiers are more descriptive, so they are used here
       to maximize readability.  The Linux identifiers are aliased to
       these in machdep.h. */
    int     fd;
    rct_status_t    status;
    struct sockaddr_rfcomm raddr,laddr;
    bdaddr_t bdaddr;
    char    device[18]; /* "00:16:53:01:11:b9" */
    
    /* Format bluetooth address for error and debugging outoput */
    snprintf(device,18,"%02X:%02X:%02X:%02X:%02X:%02X",
	    nxt->bluetooth_address[0],
	    nxt->bluetooth_address[1],
	    nxt->bluetooth_address[2],
	    nxt->bluetooth_address[3],
	    nxt->bluetooth_address[4],
	    nxt->bluetooth_address[5]);
    
    debug_printf("Opening bluetooth address %s...\n",device);
    debug_printf("Creating socket...\n");
    
    /* Create socket */
    fd = socket(PF_BLUETOOTH, SOCK_STREAM, BLUETOOTH_PROTO_RFCOMM);
    if ( fd == -1 )
    {
	perror("nxt_open_brick_bluetooth(): Cannot create socket");
	return RCT_CANNOT_CREATE_SOCKET;
    }
    
    /* Bind socket */
    bacpy(&laddr.rfcomm_bdaddr, NG_HCI_BDADDR_ANY);
    laddr.rfcomm_family = AF_BLUETOOTH;
    laddr.rfcomm_channel = 0;
    
    debug_printf("Binding...\n");
    if (bind(fd, (struct sockaddr *)&laddr, sizeof(laddr)) < 0)
    {
	perror("nxt_open_brick_bluetooth(): Cannot bind");
	return RCT_CANNOT_BIND_SOCKET;
    }

    /* Connection to NXT */
    str2ba(device, &bdaddr);
    bacpy(&raddr.rfcomm_bdaddr,&bdaddr);
    raddr.rfcomm_family = AF_BLUETOOTH;
    raddr.rfcomm_channel = 1;
    
    debug_printf("Connecting...\n");
    status = connect(fd, (struct sockaddr *)&raddr, sizeof(raddr));
    
    if (status != 0)
    {
	fprintf(stderr, "Error: %s(): Connect failed: %s\n",
		__func__, strerror(errno));
	fputs("Try rebooting your NXT brick.\n",stderr);
	return RCT_CANNOT_CONNECT_SOCKET;
    }
    nxt->bluetooth_fd = fd;
    
#elif defined(Darwin)
    /* Use IOBluetooth framework */
#endif
    return RCT_OK;
}


/****************************************************************************
 * Description: 
 *  Read and print the battery level from an NXT brick.
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 * Author: Jason W. Bacon
 ***************************************************************************/

rct_status_t nxt_print_battery_level(rct_nxt_t *nxt)

{
    rct_status_t    status;
    
    if ( (status=nxt_get_battery_level(nxt)) == RCT_OK )
    {
	printf("Battery level = %dmV.\n", nxt->battery_level);
	puts("Maximum expected is 9000mV for alkaline batteries.  Other");
	puts("battery types may operate at lower voltages, such as 7200mV.");
    }
    debug_printf("Getting battery level...\n");

    return status;
}


/****************************************************************************
 * Description: 
 *  Read and print device info.
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 * Author: Jason W. Bacon
 ***************************************************************************/

rct_status_t nxt_print_device_info(rct_nxt_t *nxt)

{
    rct_status_t status;
    
    if ( (status = nxt_get_device_info(nxt)) == RCT_OK )
    {
	printf("Device name: %s\n",nxt->name);
	printf("Bluetooth address: %02x:%02x:%02x:%02x:%02x:%02x\n",
		nxt->bluetooth_address[0], nxt->bluetooth_address[1],
		nxt->bluetooth_address[2], nxt->bluetooth_address[3],
		nxt->bluetooth_address[4], nxt->bluetooth_address[5]);
	printf("Bluetooth signal strength: %lu\n",nxt->bluetooth_signal_strength);
	printf("Free flash RAM: %luk\n",nxt->free_flash/1024);
    }
    return status;
}


/****************************************************************************
 * Description: 
 *  Read and print firmware version.
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 * Author: Jason W. Bacon
 ***************************************************************************/

rct_status_t nxt_print_firmware_version(rct_nxt_t *nxt)

{
    rct_status_t    status;
    
    if ( (status=nxt_get_firmware_version(nxt)) == RCT_OK )
    {
	printf("Firmware: %d.%02d\n",nxt->firmware_major,nxt->firmware_minor);
	printf("Protocol: %d.%d\n",nxt->protocol_major,nxt->protocol_minor);
    }
    return RCT_OK;
}


/****************************************************************************
 * Description: 
 *  Send a complex command (one with arguments) to an NXT brick
 *  and report back the brick's response.
 *
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 *
 *  This function is not part of the API, and should not be called
 *  from client programs.  The interface uses a variable number of
 *  arguments, and is prone to coding mistakes.  It's sole purpose is
 *  to eliminate redundant code among the many fixed NXT commands which
 *  all have a similar binary format, and hence require much of the
 *  same code to implement.  They are, however, different enough to
 *  require significant flexibility in the arguments needed to construct
 *  the protocol for each command.
 *
 * Author: Jason W. Bacon
 ***************************************************************************/

int nxt_send_simple_cmd(rct_nxt_t * nxt, int cmd_type, int cmd, 
		    char *response,int response_max)

{
    int     bytes = 0;
    char    cmd_buff[2];
    
    if ( !NXT_IS_OPEN(nxt) )
    {
	fputs("nxt_send_cmd(): Error: NXT is not currently open.\n",stderr);
    }
    else
    {
	cmd_buff[0] = cmd_type;
	cmd_buff[1] = cmd;
	if (nxt_send_buf(nxt, cmd_buff, 2) == 2)
	{
	    bytes = nxt_recv_buf(nxt, response, response_max);
	}
	else
	{
	    fprintf(stderr, 
		    "nxt_direct_cmd(): Error: Failed to send command %d.\n",
		    cmd);
	}
    }
    return bytes;
}


/****************************************************************************
 * Description: 
 *  Send a simple command (one without arguments) to an NXT brick
 *  and report back the brick's response.
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 * Author: Jason W. Bacon
 ***************************************************************************/

int nxt_send_cmd(rct_nxt_t * nxt, int cmd_type, int cmd, char *response,
	    int response_max, char *format, ...)

{
    int     bytes,
	    len;
    unsigned char    cmd_buff[NXT_BUFF_LEN+1],
	    *outp,
	    *string;
    uint16_t  short_word;
    uint32_t  long_word;
    va_list ap;
    
    if ( !NXT_IS_OPEN(nxt) )
    {
	fputs("nxt_send_cmd(): Error: NXT is not currently open.\n",stderr);
	bytes = 0;
    }
    else
    {
	cmd_buff[0] = cmd_type | nxt->response_mask;
	cmd_buff[1] = cmd;
	
	/* Process variable arguments depending on format */
	va_start(ap,format);
	for (outp = cmd_buff+2; *format != '\0'; ++format)
	{
	    switch(*format)
	    {
		case    '%':
		    break;
		case    'c':
		    *outp++ = va_arg(ap,unsigned int);
		    break;
		case    'w':
		    short_word = va_arg(ap,unsigned int);
		    short2buf(outp,short_word);
		    outp += 2;
		    break;
		case    'l':
		    long_word = va_arg(ap,unsigned long);
		    long2buf(outp,long_word);
		    outp += 4;
		    break;
		case    's':
		    string = (unsigned char *)va_arg(ap,char *);
		    memset(outp,0,20);
		    strlcpy((char *)outp,(char *)string,20);
		    outp += 20;
		    break;
		default:
		    *outp++ = *format;
		    break;
	    }
	}
	
	len = outp - cmd_buff;
	
	if (nxt_send_buf(nxt, (char *)cmd_buff, len) == len)
	{
	    if ( nxt->response_mask != NXT_NO_RESPONSE )
		bytes = nxt_recv_buf(nxt, response, response_max);
	    else
		bytes = 0;
	}
	else
	{
	    fprintf(stderr, 
		    "nxt_send_cmd(): Error: Failed to send command %d:%d.\n",
		    cmd_type,cmd);
	    bytes = 0;
	}
    }
    return bytes;
}


/****************************************************************************
 * Description: 
 *  Send the contents of buffer to an NXT brick via the currently
 *  open interface.
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 * Author: Jason W. Bacon
 ***************************************************************************/

int     nxt_send_buf(rct_nxt_t * nxt, char *buf, int len)

{
    int     bytes;
    unsigned short  msg_len = len;
    char    bt_buf[NXT_BUFF_LEN+1];

    switch(nxt_connection_type(nxt))
    {
	case    NXT_USB:
	    debug_nxt_dump_cmd(buf,len,"");
	    bytes = usb_bulk_write(nxt->usb_handle, NXT_USB_OUT_ENDPOINT, buf, len,
			   USB_TIMEOUT);
	    break;
	case    NXT_BLUETOOTH:
	    if ( len > NXT_BUFF_LEN - 2 )
	    {
		fprintf(stderr,
		    "nxt_send_buf(): Internal error: Message length %d exceeds maximum of %d.\n",
		    len,NXT_BUFF_LEN);
		exit(EX_SOFTWARE);
	    }
	    /* Bluetooth adds a 2-byte message length to the beginning
	       of each packet.  Otherwise, the protocol is identical to USB */
	    short2buf((unsigned char *)bt_buf,msg_len);
	    
	    memcpy(bt_buf+2,buf,len);
	    debug_printf("nxt_send_buf() sending %d bytes...\n",len+2);
	    debug_nxt_dump_cmd(bt_buf,len+2,"");
	    /* Subtract msg_len 2 bytes from total returned */
	    bytes = write(nxt->bluetooth_fd,bt_buf,len+2) - 2;
	    break;
	case    NXT_NO_CONNECTION:
	    fputs("nxt_send_buf(): Internal error: No connection.\n",stderr);
	    exit(EX_SOFTWARE);
	    break;
	default:
	    fprintf(stderr,
		"nxt_send_buf(): Internal error: Unknown connection type: %d.\n",
		nxt_connection_type(nxt));
	    exit(EX_SOFTWARE);
	    break;
    }
    return bytes;
}


/****************************************************************************
 * Description: 
 *  Send a null-terminated string to the NXT.
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 * Author: Jason W. Bacon
 ***************************************************************************/

rct_status_t nxt_send_str(rct_nxt_t * nxt, char *str)

{
    return nxt_send_buf(nxt, str, strlen(str));
}


/****************************************************************************
 * Description: 
 *  Read a response from an NXT via the currently open connection.
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 * Author: Jason W. Bacon
 ***************************************************************************/

rct_status_t nxt_recv_buf(rct_nxt_t * nxt, char *buf, int maxlen)

{
    int     bytes;
    char    bt_buf[NXT_BUFF_LEN+1];
    
    switch(nxt_connection_type(nxt))
    {
	case    NXT_USB:
	    bytes = usb_bulk_read(nxt->usb_handle, NXT_USB_IN_ENDPOINT, buf, maxlen, USB_TIMEOUT);
	    break;
	case    NXT_BLUETOOTH:
	    /* Bluetooth prepends an extra 2 bytes for message length */
	    bytes = read(nxt->bluetooth_fd,bt_buf,NXT_BUFF_LEN) - 2;
	    if ( bytes > maxlen )
	    {
		fprintf(stderr,"nxt_recv_buf(): Internal error: %d byte message is greater than maxlen of %d\n",
			bytes,maxlen);
		exit(EX_SOFTWARE);
	    }
	    memcpy(buf,bt_buf+2,bytes);
	    break;
	case    NXT_NO_CONNECTION:
	    fputs("nxt_recv_buf(): Internal error: No connection.\n",stderr);
	    exit(EX_SOFTWARE);
	    break;
	default:
	    fprintf(stderr,
		"nxt_recv_buf(): Internal error: Unknown connection type: %d.\n",
		nxt_connection_type(nxt));
	    exit(EX_SOFTWARE);
	    break;
    }
    return bytes;
}


/****************************************************************************
 * Description: 
 *  Close the currently open connection to an NXT brick.
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 * Author: Jason W. Bacon
 ***************************************************************************/

rct_status_t nxt_close_brick(rct_nxt_t *nxt)

{
    switch(nxt_connection_type(nxt))
    {
	case    NXT_USB:
	    return nxt_close_brick_usb(nxt);
	case    NXT_BLUETOOTH:
	    return nxt_close_brick_bluetooth(nxt);
	case    NXT_NO_CONNECTION:
	    fputs("nxt_close_connection(): Warning: Nothing to close.\n",stderr);
	    return RCT_NOT_CONNECTED;
    }
    return RCT_OK;
}


/****************************************************************************
 * Description: 
 *  Close a USB connection to an NXT brick.
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 * Author: Jason W. Bacon
 ***************************************************************************/

rct_status_t nxt_close_brick_usb(rct_nxt_t *nxt)

{
    if ( nxt->usb_handle != NULL )
    {
	debug_printf("Closing USB connection: nxt=%p, usb_handle=%p...\n",
		nxt,nxt->usb_handle);
	usb_release_interface(nxt->usb_handle, NXT_USB_INTERFACE);
	usb_close(nxt->usb_handle);
	nxt->usb_handle = NULL;
	return RCT_OK;
    }
    else
    {
	fputs("nxt_close_brick_usb(): USB connection is not currently open.\n",stderr);
	return RCT_NOT_CONNECTED;
    }
}


/****************************************************************************
 * Description: 
 *  Close a Bluetooth connection to an NXT brick.
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 * Author: Jason W. Bacon
 ***************************************************************************/

rct_status_t nxt_close_brick_bluetooth(rct_nxt_t *nxt)

{
    if ( nxt->bluetooth_fd != -1 )
    {
	debug_printf("Closing Bluetooth connection...\n");
	close(nxt->bluetooth_fd);
	nxt->bluetooth_fd = -1;
	/* This sleep() is a hack until I figure out why connect() fails with
	    "connection reset by peer" when running roboctl twice in
	    quick succession.  This is a problem for ape, which runs
	    
	    roboctl upload program
	    roboctl start program
	    
	    with no delay in between.
	*/
	sleep(1);
    }
    else
    {
	fputs("nxt_close_brick_bluetooth(): Bluetooth connection is not currently open.\n",stderr);
	return RCT_NOT_CONNECTED;
    }
    return RCT_OK;
}


/****************************************************************************
 * Description: 
 *  Verify that a filename conforms to the limitations imposed by the
 *  NXT brick.
 *  If correct_ext is not NULL:
 *      If the filename has an correct_ext, make sure they match.
 *      If the filename does not have an correct_ext, append correct_ext.
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 * Author: Jason W. Bacon
 ***************************************************************************/

rct_status_t nxt_validate_filename(char *filename, char *correct_ext, const char *caller)

{
    char    *ext;
    
    if ( (ext=strchr(filename,'.')) != NULL )
    {
	/* Only 1 '.' allowed */
	if ( strchr(ext+1,'.') != NULL )
	{
	    fprintf(stderr, "Error: %s() from %s(): Filename %s has more than 1 '.'.\n",
		__func__, caller, filename);
	}
	
	/* Stem is limited to 15 chars */
	if ( ext-filename > 15 )
	{
	    fprintf(stderr, "Error: %s() from %s(): Filename %s has more than 15 chars in stem.\n",
		__func__, caller, filename);
	    return RCT_INVALID_FILENAME;
	}
	
	/* Extension is limited to 3 chars, not including '.' */
	if ( strlen(ext) > 4 )
	{
	    fprintf(stderr, "nxt_validate_filename(): Error: Filename %s has more than 3 chars in extension.\n",
		filename);
	    return RCT_INVALID_FILENAME;
	}
	
	if ( (correct_ext != NULL) && (strcmp(ext,correct_ext) != 0) )
	{
	    fprintf(stderr, "nxt_validate_filename(): Error: Filename %s should end in %s.\n",
		filename,correct_ext);
	    return RCT_INVALID_FILENAME;
	}
    }
    else
    {
	if ( strlen(filename) > 15 )
	{
	    fprintf(stderr, "nxt_validate_filename(): Error: Filename %s has more than 15 chars in stem.\n",
		filename);
	    return RCT_INVALID_FILENAME;
	}
	else if ( correct_ext != NULL )   /* Append correct extension */
	    strlcat(filename,correct_ext,NXT_FILENAME_MAX);
    }
    return RCT_OK;
}


/****************************************************************************
 * Description:
 *  Upload a file to an NXT brick.
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 * Author: Jason W. Bacon
 ***************************************************************************/

rct_status_t nxt_upload_file(rct_nxt_t *nxt,char *filename_on_pc,rct_flag_t flags)

{
    int             file_handle;
    rct_status_t    status;
    char            *filename_on_brick;
    struct stat     st;
    
    /* Upload file */
    debug_printf("Uploading %s\n",filename_on_pc);

    filename_on_brick = nxt_pc_to_brick_filename(filename_on_pc);
    fprintf(stderr, "filename_on_brick = %s\n", filename_on_brick);

    if ( stat(filename_on_pc,&st) != 0 )
    {
	fprintf(stderr,"nxt_open_file_write(): Cannot stat %s.\n",filename_on_pc);
	return -1;
    }
    // Is open_write_data needed for some file types?
    // file_handle = nxt_open_file_write_data(nxt,filename);
    file_handle = nxt_open_file_write_linear(nxt,filename_on_brick, st.st_size);
    if ( file_handle != -1 )
    {
	status = nxt_write_file(nxt,filename_on_pc,file_handle);
	nxt_close_file(nxt,file_handle);
    }
    else
    {
	fprintf(stderr,"Error: %s(): Unable to open %s in write mode.\n",
	    __func__, filename_on_brick);
	status = RCT_OPEN_FAILED;
    }
    
    if ( flags & RCT_UPLOAD_PLAY_SOUND )
    {
	if ( status == RCT_OK )
	    nxt_play_sound_file(nxt,RCT_NO_FLAGS,"! Attention");
	else
	    nxt_play_sound_file(nxt,RCT_NO_FLAGS,"Woops");
    }
    return status;
}


/****************************************************************************
 * Description: 
 *  Initialize an rct_nxt_t structure.  This must be done before calling
 *  any other nxt_ functions.
 * Author: Jason W. Bacon
 ***************************************************************************/

void    nxt_init_struct(rct_nxt_t *nxt)

{
    nxt->usb_handle = NULL;
    nxt->usb_dev = NULL;
    nxt->bluetooth_fd = -1;
    nxt->is_in_reset_mode = 0;
    nxt_response_on(nxt);
}


/****************************************************************************
 * Description: 
 *  Convert a 2 byte little-endian integer response to a short on the local
 *  host in an endian-independent way.
 *  This produces an unaltered bit string on little endian systems
 *  in a slightly inefficient way, but produces valid results on big
 *  and mixed endian systems without performing an endian check
 *  which would have comparable cost.
 * Author: Jason W. Bacon
 ***************************************************************************/

short   buf2short(unsigned char *buf)

{
    return buf[0] + (buf[1] << 8);
}


/****************************************************************************
 * Description: 
 *  Convert a short on the local host to a little-endian integer
 *  in an endian-independent way.
 *  This produces an unaltered bit string on little endian systems
 *  in a slightly inefficient way, but produces valid results on big
 *  and mixed endian systems without performing an endian check
 *  which would have comparable cost.
 * Author: Jason W. Bacon
 ***************************************************************************/

void    short2buf(unsigned char *buf,long val)

{
    buf[0] = (val & 0x00ff);
    buf[1] = (val & 0xff00) >> 8;
}


/****************************************************************************
 * Description: 
 *  Convert a 4 byte little-endian integer response to a long on the local
 *  host in an endian-independent way.
 *  This produces an unaltered bit string on little endian systems
 *  in a slightly inefficient way, but produces valid results on big
 *  and mixed endian systems without performing an endian check
 *  which would have comparable cost.
 * Author: Jason W. Bacon
 ***************************************************************************/

long    buf2long(unsigned char *buf)

{
    return buf[0] + (buf[1] << 8) + (buf[2] << 16) + (buf[3] << 24);
}


/****************************************************************************
 * Description: 
 *  Convert a long on the local host to a little-endian 4 byte integer
 *  in an endian-independent way.
 *  This produces an unaltered bit string on little endian systems
 *  in a slightly inefficient way, but produces valid results on big
 *  and mixed endian systems without performing an endian check
 *  which would have comparable cost.
 * Author: Jason W. Bacon
 ***************************************************************************/

void    long2buf(unsigned char *buf,long val)

{
    buf[0] = (val & 0x000000ff);
    buf[1] = (val & 0x0000ff00) >> 8;
    buf[2] = (val & 0x00ff0000) >> 16;
    buf[3] = (val & 0xff000000) >> 24;
}


/****************************************************************************
 * Description: 
 *  Dump an NXT brick command response in hexadecimal, if Debug is set.
 * Author: Jason W. Bacon
 ***************************************************************************/

void    debug_nxt_dump_response(char *response,int bytes,char *cmd)

{
    int     c;
    
    if ( Debug )
    {
	printf("Received %d bytes in response to %s.\n",bytes,cmd);
	for (c = 0; c < bytes; ++c)
	    printf("%02x ", (unsigned char)response[c]);
	putchar('\n');
    }
}


/****************************************************************************
 * Description: 
 *  Show an NXT command in hexadecimal, if Debug is set.
 * Author: Jason W. Bacon
 ***************************************************************************/

void    debug_nxt_dump_cmd(char *cmd,int bytes,char *cmd_name)

{
    int     c;
    
    if ( Debug )
    {
	printf("Sending %s command: %d bytes.\n",cmd_name,bytes);
	for (c = 0; c < bytes; ++c)
	    printf("%02x ", (unsigned char)cmd[c]);
	putchar('\n');
    }
}


/****************************************************************************
 * Description: 
 *  Construct an NXT command containing only a filename.  This is a
 *  common format among the NXT commands.
 * Author: Jason W. Bacon
 ***************************************************************************/

void    nxt_build_file_cmd(char *cmd,int cmd_type,int cmd_code,char *filename)

{
    cmd[0] = cmd_type;
    cmd[1] = cmd_code;
    memset(cmd+2,'\0',20);
    strlcpy(cmd+2,filename,20);
}


/****************************************************************************
 * Description: 
 *  Initialize an NXT command header for file operations.
 * Author: Jason W. Bacon
 ***************************************************************************/

void    nxt_init_buff_header(char *buff,int cmd_code,int file_handle)

{
    buff[0] = NXT_SYSTEM_CMD;
    buff[1] = cmd_code;
    buff[2] = file_handle;
}


/****************************************************************************
 * Description: 
 *  Determine what type of connection is open, if any, to an NXT brick.
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 * Author: Jason W. Bacon
 ***************************************************************************/

nxt_connection_t    nxt_connection_type(rct_nxt_t *nxt)

{
    if ( NXT_BLUETOOTH_IS_OPEN(nxt) )
	return NXT_BLUETOOTH;
    if ( NXT_USB_IS_OPEN(nxt) )
	return NXT_USB;
    return NXT_NO_CONNECTION;
}


/****************************************************************************
 * Description: 
 *  Convert a standard bluetooth address string XX:XX:XX:XX:XX:XX to
 *  binary, and store in the rct_nxt_t structure.
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 * Author: Jason W. Bacon
 ***************************************************************************/

rct_status_t nxt_copy_text_bt_addr(rct_nxt_t *nxt,char *address)

{
    unsigned int a0,a1,a2,a3,a4,a5;
    
    /* Example: 00:16:53:01:11:B9 */
    if ( sscanf(address,"%x:%x:%x:%x:%x:%x",&a0,&a1,&a2,&a3,&a4,&a5) != 6 )
    {
	fprintf(stderr, "Error: %s(): Invalid address string: %s\n",
	    __func__, address);
	fputs("Strings must be of the form: XX:XX:XX:XX:XX:XX\n",stderr);
	exit(EX_DATAERR);
    }
    snprintf((char *)nxt->bluetooth_address,7,"%c%c%c%c%c%c",a0,a1,a2,a3,a4,a5);
    return RCT_OK;
}


rct_status_t nxt_copy_hostent_bt_addr(rct_nxt_t *nxt,char *addr)

{
    int     c;
    
    for (c=5; c>=0; --c)
    {
	printf("%02x ",(unsigned char)addr[c]);
	nxt->bluetooth_address[5-c] = addr[c];
    }
    nxt->bluetooth_address[6] = '\0';
    putchar('\n');
    return RCT_OK;
}


/****************************************************************************
 * Description: 
 *  Upload a firmware file to an nxt brick.
 *
 *  NOTE: Because of the type of ROM used to store firmware in the NXT,
 *  this operation can only be performed a limited number of times.
 *  Therefore, this function should be used sparingly.
 *
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 * Author: Jason W. Bacon
 ***************************************************************************/

rct_status_t    nxt_upload_firmware(rct_nxt_t *nxt,char *file)

{
    return RCT_NOT_IMPLEMENTED;
}


/****************************************************************************
 * Description: 
 *  Download a file from an NXT brick to the local host.
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 * Author: Jason W. Bacon
 ***************************************************************************/

rct_status_t    nxt_download_file(rct_nxt_t *nxt,char *file)

{
    return RCT_NOT_IMPLEMENTED;
}


rct_status_t    nxt_check_response(rct_nxt_t *nxt,char *response,int bytes,
				    int expected_bytes,char *func)

{
    /* Check if NXT is set not to respond */
    if ( nxt->response_mask == NXT_NO_RESPONSE )
	return RCT_OK;

    debug_nxt_dump_response(response,bytes,func);

    if ( bytes != expected_bytes )
    {
	fprintf(stderr, "Error: %s: Expected %d byte response, got %d\n",
		func, expected_bytes, bytes);
	return RCT_COMMAND_FAILED;
    }
    
    if ( response[2] != 0 )
    {
	fprintf(stderr,"Error: %s: Non-zero status in response from NXT.\n",
		func);
	return RCT_COMMAND_FAILED;
    }
    return RCT_OK;
}


void    nxt_response_on(rct_nxt_t *nxt)

{
    nxt->response_mask = NXT_RESPONSE;
}


void    nxt_response_off(rct_nxt_t *nxt)

{
    nxt->response_mask = NXT_NO_RESPONSE;
}


char    *nxt_pc_to_brick_filename(char *filename_on_pc)

{
    char    *base;
	    
    /* Strip off any leading path info. The memmove() function
       properly handles overlapping source and dest. */
    if ( (base = strrchr(filename_on_pc,'/')) != NULL )
	return base+1;
    else
	return filename_on_pc;
}

