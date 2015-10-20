/****************************************************************************
 *  This file contains NXT-specific functions, which should generally
 *  not be called directly from application programs.  Using the rct_
 *  brick-independent API will result in more portable, lower-maintenance
 *  code.
 ***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <usb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sysexits.h>
#include "roboctl.h"

extern int  Debug;



rct_status_t    nxt_open_file_read(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}


rct_status_t    nxt_open_file_write(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}


rct_status_t    nxt_read_file(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}


/****************************************************************************
 * Description: 
 *  Copy a file from the local host to an NXT brick.
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 *  The file must first be opened using nxt_open_file_write_data?();
 * Author: Jason W. Bacon
 ***************************************************************************/

rct_status_t nxt_write_file(rct_nxt_t *nxt,char *filename,int file_handle)

{
    int     fd,
	    bytes;
    char    buff[65],
	    response[NXT_RESPONSE_MAX+1];
    
    fd = open(filename,O_RDONLY);
    if ( fd < 0 )
    {
	fprintf(stderr,"nxt_write_file(): Cannot open local file %s for reading.\n",filename);
	return RCT_CANNOT_OPEN_FILE;
    }
    
    nxt_init_buff_header(buff,NXT_SC_WRITE,file_handle);
    while ( (bytes = read(fd,buff+3,61)) > 0 )
    {
	/*
	 *  John Hay patch for newer firmware
	 *  Don't recall why I was using null-padded packets
	 */
	//memset(buff+3+bytes,'\0',61-bytes); /* Null pad */
	//bytes = nxt_send_buf(nxt,buff,64);
	bytes = nxt_send_buf(nxt,buff,bytes+3);
	bytes = nxt_recv_buf(nxt,response,NXT_RESPONSE_MAX);
	debug_nxt_dump_response(response,bytes,"NXT_SC_WRITE");
    }
    
    close(fd);
    return RCT_OK;
}


/****************************************************************************
 * Description: 
 *  Close a file handle opened by nxt_open_file_*();
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 * Author: Jason W. Bacon
 ***************************************************************************/

rct_status_t nxt_close_file(rct_nxt_t *nxt,int file_handle)

{
    int     bytes;
    char    buff[3],
	    response[NXT_RESPONSE_MAX+1];
    
    nxt_init_buff_header(buff,NXT_SC_CLOSE,file_handle);
    bytes = nxt_send_buf(nxt,buff,3);
    if ( bytes != 3 )
    {
	fprintf(stderr,"nxt_close_file(): Error sending close command.\n");
	return RCT_COMMAND_FAILED;
    }
    
    bytes = nxt_recv_buf(nxt,response,NXT_RESPONSE_MAX);
    debug_nxt_dump_response(response,bytes,"NXT_SC_CLOSE");
    if ( bytes != 4 )
    {
	fprintf(stderr,"nxt_close_file(): Error closing file.  Received %d byte, expected 4.\n",bytes);
	return RCT_COMMAND_FAILED;
    }
    return RCT_OK;
}


/****************************************************************************
 * Description: 
 *  Attempt to delete a file from an NXT brick.
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 * Author: Jason W. Bacon
 *
 * FIXME: Bad design.  Should not receive full pathname of local file.
 ***************************************************************************/

rct_status_t nxt_delete_file(rct_nxt_t *nxt,char *filename)

{
    int         bytes;
    rct_status_t    status;
    char        cmd[23],
		response[NXT_RESPONSE_MAX+1],
		*filename_on_brick;

    filename_on_brick = nxt_pc_to_brick_filename(filename);
    status = nxt_validate_filename(filename_on_brick,NULL, __func__);
    if ( status != RCT_OK )
	return status;

    nxt_build_file_cmd(cmd,NXT_SYSTEM_CMD,NXT_SC_DELETE,filename_on_brick);
    debug_nxt_dump_cmd(cmd,22,"NXT_SC_DELETE");
    if ( nxt_send_buf(nxt,cmd,22) != 22 )
    {
	fprintf(stderr,"nxt_delete_file(): Error sending delete command.\n");
	return RCT_COMMAND_FAILED;
    }
    bytes = nxt_recv_buf(nxt,response,NXT_RESPONSE_MAX);
    debug_nxt_dump_response(response,bytes,"NXT_SC_DELETE");
    return RCT_OK;
}


rct_status_t    nxt_find_first(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}


rct_status_t    nxt_find_next(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}


/****************************************************************************
 * Description: 
 *  Read firmware version from the NXT.
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 * Author: Jason W. Bacon
 ***************************************************************************/

rct_status_t nxt_get_firmware_version(rct_nxt_t *nxt)

{
    int     bytes;
    char    response[NXT_RESPONSE_MAX + 1];

    /*
     * NXT brick should respond with 5 bytes: 
     *  reply command status level-lsb level-msb
     */
    bytes = nxt_send_simple_cmd(nxt, NXT_SYSTEM_CMD,
			NXT_SC_GET_VERSIONS, response,
			NXT_RESPONSE_MAX);
    if ( bytes == 7 )
    {
	nxt->firmware_major = response[6];
	nxt->firmware_minor = response[5];
	nxt->protocol_major = response[4];
	nxt->protocol_minor = response[3];
    }
    else
    {
	fputs("nxt_get_firmware_version(): command failed.\b",stderr);
	return RCT_COMMAND_FAILED;
    }
    return 0;
}


/****************************************************************************
 * Description: 
 *  Open a file on an NXT brick.
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 * Author: Jason W. Bacon
 *
 * FIXME: Bad design.  Should not receive full pathname of local file.
 ***************************************************************************/

rct_status_t    nxt_open_file_write_linear(rct_nxt_t *nxt, char *filename_on_brick,
					    size_t size)

{
    int         bytes,
		file_handle;
    rct_status_t    status;
    char        cmd[27],
		response[NXT_RESPONSE_MAX+1];
    
    status = nxt_validate_filename(filename_on_brick, NULL, __func__);
    if ( status != RCT_OK )
	return status;

    nxt_build_file_cmd(cmd,NXT_SYSTEM_CMD,NXT_SC_OPEN_WRITE_LINEAR,filename_on_brick);
    
    long2buf((unsigned char *)cmd+22,size);
    
    debug_printf("File size in st is %u\n",(unsigned)size);
    debug_printf("File size in cmd is %ld\n",*(long *)(cmd+22));
    debug_nxt_dump_cmd(cmd,26,"NXT_SC_OPEN_WRITE_LINEAR");
    
    if ( nxt_send_buf(nxt,cmd,26) != 26 )
    {
	fprintf(stderr,"nxt_open_file_write(): Error sending open command.\n");
	return -1;
    }
    bytes = nxt_recv_buf(nxt,response,NXT_RESPONSE_MAX);
    debug_nxt_dump_response(response,bytes,"NXT_SC_OPEN_WRITE_LINEAR");
    if ( bytes != 4 )
    {
	fprintf(stderr,"nxt_open_file_write(): Bad response to open command.  Got %d bytes, expected 4.\n",bytes);
	return -1;
    }
    status = response[2];
    if ( status == 0 )
    {
	file_handle = (unsigned char)response[3];
	return file_handle;
    }
    else
	return -1;
}


rct_status_t    nxt_open_file_read_linear(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}


/****************************************************************************
 * Description: 
 *  Open a file on an NXT brick.
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 * Author: Jason W. Bacon
 *
 * FIXME: Bad design.  Should not receive full pathname of local file.
 ***************************************************************************/

rct_status_t    nxt_open_file_write_data(rct_nxt_t *nxt,char *filename_on_brick, size_t size)

{
    int         bytes,
		file_handle;
    rct_status_t    status;
    char        cmd[27],
		response[NXT_RESPONSE_MAX+1];
    
    status = nxt_validate_filename(filename_on_brick, NULL, __func__);
    if ( status != RCT_OK )
	return status;

    nxt_build_file_cmd(cmd,NXT_SYSTEM_CMD,NXT_SC_OPEN_WRITE_DATA,filename_on_brick);
    
    /* Add file-size in guaranteed little-endian format */
    long2buf((unsigned char *)cmd+22,size);
    
    debug_printf("File size in st is %u\n",(unsigned)size);
    debug_printf("File size in cmd is %ld\n",*(long *)(cmd+22));
    debug_nxt_dump_cmd(cmd,26,"NXT_SC_OPEN_WRITE_DATA");
    
    if ( nxt_send_buf(nxt,cmd,26) != 26 )
    {
	fprintf(stderr,"nxt_open_file_write(): Error sending open command.\n");
	return -1;
    }
    bytes = nxt_recv_buf(nxt,response,NXT_RESPONSE_MAX);
    debug_nxt_dump_response(response,bytes,"NXT_SC_OPEN_WRITE_DATA");
    if ( bytes != 4 )
    {
	fprintf(stderr,"nxt_open_file_write(): Bad response to open command.  Got %d bytes, expected 4.\n",bytes);
	return -1;
    }
    status = response[2];
    if ( status == 0 )
    {
	file_handle = (unsigned char)response[3];
	return file_handle;
    }
    else
	return -1;
}


rct_status_t    nxt_open_file_append_data(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}


rct_status_t    nxt_boot(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}


rct_status_t    nxt_set_brick_name(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}


/****************************************************************************
 * Description: 
 *  Get device information from an NXT brick, as defined by the
 *  GET_DEVICE_INFO system command.
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 * Author: Jason W. Bacon
 ***************************************************************************/

rct_status_t nxt_get_device_info(rct_nxt_t *nxt)

{
    int     bytes;
    char    response[NXT_RESPONSE_MAX + 1];

    /*
     * NXT brick should respond with 5 bytes: 
     *  reply command status level-lsb level-msb
     */
    bytes = nxt_send_simple_cmd(nxt, NXT_SYSTEM_CMD,
			NXT_SC_GET_DEVICE_INFO, response,
			NXT_RESPONSE_MAX);
    if ( bytes == 33 )
    {
	strlcpy(nxt->name,response+3,NXT_NAME_LEN);
	memcpy(nxt->bluetooth_address,response+18,6);
	nxt->bluetooth_signal_strength = buf2long((unsigned char *)response+25);
	nxt->free_flash = buf2long((unsigned char *)response+29);
    }
    else
    {
	fputs("nxt_get_device_info() failed.\n",stderr);
	return RCT_COMMAND_FAILED;
    }
    return RCT_OK;
}


rct_status_t    nxt_delete_user_flash(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}


rct_status_t    nxt_poll_command_length(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}


rct_status_t    nxt_poll(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}


rct_status_t    nxt_bluetooth_factory_reset(rct_nxt_t *nxt)

{
    int     bytes;
    char    response[NXT_RESPONSE_MAX + 1];

    /*
     * NXT brick should respond with 5 bytes: 
     *  reply command status level-lsb level-msb
     */
    bytes = nxt_send_simple_cmd(nxt, NXT_SYSTEM_CMD,
			NXT_SC_BT_FACTORY_RESET, response,
			NXT_RESPONSE_MAX);
    if ( bytes != 3 )
    {
	fputs("nxt_bluetooth_factory_reset() failed.\n",stderr);
	return RCT_COMMAND_FAILED;
    }
    return RCT_OK;
}


rct_status_t    nxt_message(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}


rct_status_t    nxt_error_message_back_to_host(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}


rct_status_t    nxt_request_first_module(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}


rct_status_t    nxt_request_next_module(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}


rct_status_t    nxt_close_module_handle(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}


rct_status_t    nxt_read_io_map(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}


rct_status_t    nxt_write_io_map(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}

