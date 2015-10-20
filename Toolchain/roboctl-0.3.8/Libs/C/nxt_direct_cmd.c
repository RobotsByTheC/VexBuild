/****************************************************************************
 *  This file contains NXT-specific functions, which should generally
 *  not be called directly from application programs.  Using the rct_
 *  brick-independent API will result in more portable, lower-maintenance
 *  code.
 ***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <usb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sysexits.h>
#include "roboctl.h"

extern int  Debug;



/****************************************************************************
 * Description: 
 *  Attempt to start a program file on an NXT brick.
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 *  0       0x00 or 0x80
 *  1       0x00
 *  2-21    filaname (asciiz 15.3 + null)
 * Author: Jason W. Bacon
 ***************************************************************************/

rct_status_t nxt_start_program(rct_nxt_t *nxt,char *raw_filename)

{
    int         bytes;
    rct_status_t    status;
    char        response[NXT_RESPONSE_MAX+1],
		filename[NXT_FILENAME_MAX+1];
    
    strlcpy(filename,raw_filename,NXT_FILENAME_MAX);

    /* Make sure this is a valid sound file name */
    if ( (status = nxt_validate_filename(filename,".rxe", __func__)) != RCT_OK )
	return status;

    bytes = nxt_send_cmd(nxt,NXT_DIRECT_CMD,NXT_DC_START_PROGRAM,
	    response,NXT_RESPONSE_MAX,"%s",filename);
    return  nxt_check_response(nxt,response,bytes,3,"NXT_DC_START_PROGRAM");
}


/****************************************************************************
 * Description: 
 *  Attempt to stop the currently running program on an NXT brick.
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 *  0       0x00 or 0x80
 *  1       0x01
 * Author: Jason W. Bacon
 ***************************************************************************/

rct_status_t nxt_stop_program(rct_nxt_t *nxt)

{
    int         bytes;
    char        response[NXT_RESPONSE_MAX+1];
    
    bytes = nxt_send_simple_cmd(nxt,NXT_DIRECT_CMD,
			NXT_DC_STOP_PROGRAM,response,
			NXT_RESPONSE_MAX);
    
    if (bytes != 3)
    {
	fputs("nxt_stop_program(): Error sending stop command.\n", stderr);
	return RCT_COMMAND_FAILED;
    }
    return RCT_OK;
}


/****************************************************************************
 * Description: 
 *  Play the specified sound file.
 *  0       0x00 or 0x80
 *  1       0x02
 *  2       loop (boolean 0 or 1)
 *  3-22    filename (asciiz 15.3 + null)
 * Author: 
 ***************************************************************************/

rct_status_t    nxt_play_sound_file(rct_nxt_t *nxt,rct_flag_t flags,
				    char * const raw_filename)

{
    int         bytes;
    rct_status_t    status;
    char        response[NXT_RESPONSE_MAX+1],
		filename[NXT_FILENAME_MAX+1];

    /* Make a private copy in case users want to send a string constant */
    strlcpy(filename,raw_filename,NXT_FILENAME_MAX);
    
    /* Make sure this is a valid sound file name */
    if ( (status = nxt_validate_filename(filename,".rso", __func__)) != RCT_OK )
	return status;

    /* Send command with a char value of 0 or 1 for loop flag */
    debug_printf("nxt_play_sound_file(%d,%s)\n",flags,filename);
    bytes = nxt_send_cmd(nxt,NXT_DIRECT_CMD,NXT_DC_PLAY_SOUND_FILE,
	    response,NXT_RESPONSE_MAX,"%c%s",(flags == RCT_LOOP),filename);
    return  nxt_check_response(nxt,response,bytes,3,"NXT_DC_PLAY_SOUND_FILE");
}

/****************************************************************************
 * Description: 
 *  Play a tone of the given frequency (200 - 1400 Hz) and duration.
 *  0       0x00 or 0x80
 *  1       0x03
 *  2-3     frequency (uword, 200 - 1400Hz)
 *  4-5     duration, ms (uword - range not documented)
 * Author: 
 ***************************************************************************/

rct_status_t    nxt_play_tone(rct_nxt_t *nxt,int herz,int milliseconds)

{
    int         bytes;
    char        response[NXT_RESPONSE_MAX+1];

    if ( (herz < 200) || (herz > 14000) )
    {
	fprintf(stderr,"nxt_play_tone(): Invalid frequency: %d Hz.\n",herz);
	return RCT_INVALID_DATA;
    }
    bytes = nxt_send_cmd(nxt,NXT_DIRECT_CMD,NXT_DC_PLAY_TONE,
	    response,NXT_RESPONSE_MAX,"%w%w",herz,milliseconds);
    return  nxt_check_response(nxt,response,bytes,3,"NXT_DC_PLAY_SOUND_FILE");
}

/****************************************************************************
 * Description: 
 *  0       0x00 or 0x80
 *  1       0x04
 *  2       output port (0 - 2, or 0xFF = all for simple control)
 *  3       power set point (-100 to 100)
 *  4       mode byte (bit field)
 *  5       regulation mode (ubyte, enumerated)
 *  6       turn ratio (sbyte, -100 to 100)
 *  7       runstate (ubyte, enumerated)
 *  8-12?   tacholimit (ulong, 0 = forever)
 * Author: 
 ***************************************************************************/

rct_status_t    nxt_set_output_state(rct_nxt_t *nxt,int port,int power,
				    nxt_output_mode_t mode,
				    nxt_output_regulation_mode_t regulation,
				    int ratio,
				    nxt_output_runstate_t runstate,
				    unsigned long tacholimit)

{
    int         bytes;
    char        response[NXT_RESPONSE_MAX+1];
    
    bytes = nxt_send_cmd(nxt,NXT_DIRECT_CMD,NXT_DC_SET_OUTPUT_STATE,
	    response,NXT_RESPONSE_MAX,"%c%c%c%c%c%c%l",port,power,mode,
	    regulation,ratio,runstate,tacholimit);
    return  nxt_check_response(nxt,response,bytes,3,"NXT_DC_SET_OUTPUT_STATE");
}

/****************************************************************************
 * Description: 
 *  0       0x00 or 0x80
 *  1       0x05
 *  2       input port (0-3)
 *  3       sensor type (enumerated)
 *  4       sensor mode (enumerated)
 * Author: 
 ***************************************************************************/

rct_status_t    nxt_set_input_mode(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}

/****************************************************************************
 * Description: 
 *  
 * Author: 
 ***************************************************************************/

rct_status_t    nxt_get_output_state(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}


rct_status_t    nxt_get_input_values(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}


rct_status_t    nxt_reset_input_scaled_value(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}


rct_status_t    nxt_message_write(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}


rct_status_t    nxt_reset_motor_position(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}


/****************************************************************************
 * Description: 
 *  Read the battery level from an NXT brick.
 *  The rct_nxt_t structure must first be initialized using nxt_init_struct(),
 *  which is normally called (indirectly) by rct_find_bricks().
 * Author: Jason W. Bacon
 ***************************************************************************/

rct_status_t    nxt_get_battery_level(rct_nxt_t *nxt)

{
    int    bytes;
    char   response[NXT_RESPONSE_MAX + 1];

    /*
     * NXT brick should respond with 5 bytes: 
     * reply command status level-lsb level-msb
     */
    bytes = nxt_send_simple_cmd(nxt, NXT_DIRECT_CMD,
			NXT_DC_BATTERY_LEVEL, response,
			NXT_RESPONSE_MAX);
    nxt_check_response(nxt,response,bytes,5,"NXT_SC_BATTERY_LEVEL");
    if ( bytes == 5 )
    {
	nxt->battery_level = buf2short((unsigned char *)response+3);
	return RCT_OK;
    }
    else
    {
	return RCT_COMMAND_FAILED;
    }
}


rct_status_t    nxt_stop_sound_playback(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}

/****************************************************************************
 * Command:
 *  0       0x00 or 0x80
 *  1       0x0D
 * Response:
 *  0       0x02
 *  1       0x0D
 *  2       Status
 *  3-6     Current sleep time limit (ulong)
 * Author: 
 ***************************************************************************/

rct_status_t    nxt_keep_alive(rct_nxt_t *nxt)

{
    int    bytes;
    char   response[NXT_RESPONSE_MAX + 1];

    /*
     * NXT brick should respond with 7 bytes: 
     * reply command status level-lsb level-msb
     */
    bytes = nxt_send_simple_cmd(nxt, NXT_DIRECT_CMD,
			NXT_DC_KEEP_ALIVE, response,
			NXT_RESPONSE_MAX);
    nxt_check_response(nxt,response,bytes,7,"NXT_DC_KEEP_ALIVE");
    if ( bytes == 7 )
    {
	return RCT_OK;
    }
    else
    {
	return RCT_COMMAND_FAILED;
    }
}


rct_status_t    nxt_ls_get_status(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}


rct_status_t    nxt_ls_write(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}


rct_status_t    nxt_ls_read(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}


rct_status_t    nxt_get_current_program_name(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}


rct_status_t    nxt_message_read(rct_nxt_t *nxt)

{
    return  RCT_NOT_IMPLEMENTED;
}

