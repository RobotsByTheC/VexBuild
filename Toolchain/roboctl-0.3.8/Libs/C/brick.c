#include <stdio.h>
#include "roboctl.h"

/**
 * \defgroup roboctl Robot Communication API
 *
 *  libroboctl is a C Application Programming Interface (API)
 *  for communication with robot bricks such as Lego and VEX.  It implements
 *  the basic functions of the controller communication protocols, plus
 *  higher level functions such as file uploading, downloading, etc.
 *
 *  The library allows programs written in C (and other languages
 *  capable of linking to C routines) to locate robo bricks on available
 *  communication links (e.g. USB, Bluetooth), open a connection to the
 *  brick, send commands and data, and retrieve data.
 *
 *  The top-layer C API is brick and interface independent, so you can
 *  in many cases write one program that will work with any brick over
 *  any communication interface. E.g., uploading a program file or checking
 *  the battery level is essentially the same for all bricks and it makes
 *  no difference whether we use a serial port, USB, or Bluetooth to 
 *  perform these tasks.  
 *
 *  Although use of the device independent API is encouraged for the
 *  sake of minimizing code changes for new bricks, it is not required.
 *  Some operations, are of course, specific to a certain type of brick
 *  or only make sense for certain communication links.  For example,
 *  using NXT direct commands to remote-control an NXT robot connected
 *  via a USB cable could be problematic or disastrous for the robot.
 *  In these cases, a program might want a Bluetooth connection or nothing.
 *
 *  Example program to print some basic device info:
 *
 *  \include print_status.c
 *
 *  \section seealso See also
 *  nbc(1), nxc(1), nqc(1), legoctl(1), vexctl(1), ape(1)
 * @{
 */
 
/*
 * Class:           brick
 * Structure type:  rct_brick_t
 *
 * This file contains a brick-independent API for operations common to
 * all bricks, such as file uploads, start/stop program, status check, etc.
 * Programs using libroboctl should use this abstract API as much as
 * possible, and leave the brick-dependent details to the underlying
 * layers of the library.
 *
 * All functions, both brick-independent and brick-specific, return
 * brick-independent RCT error codes of the enumerated type rct_status_t.
 * Brick-specific functions store brick-specific error
 * codes in the brick-specific structure.  The brick-specific error codes
 * can be retrieved through the abstract API using rct_error_status(brick).
 */

/**
 *  \brief  Initialize a brick structure.
 *  \param brick - Pointer to an uninitialized brick stucture.
 *  \param type - RCT_NXT or RCT_RCX.
 *  \author Jason W. Bacon
 *
 *  This function initializes the brick structure, including the brick 
 *  dependent substructures for NXT, RCX, etc.  This function must be
 *  called before any other functions that operate on the structure.
 *  Calling this function on a brick structure that has already been
 *  initialized and worked with may have disastrous results.
 *  
 *  Supported bricks:
 *      - NXT
 *      - RCX
 */

void    rct_init_brick_struct(rct_brick_t *brick,rct_brick_type_t type)

{
    brick->brick_type = type;
    switch(type)
    {
	case    RCT_NXT:
	    nxt_init_struct(&brick->nxt);
	    break;
	case    RCT_RCX:
	    rcx_init_struct(&brick->rcx);
	    break;
	case    RCT_VEX:
	    pic_init_struct(&brick->vex);
	    break;
	default:
	    break;
    }
}


/**
 *  \brief  Open a connection to a brick.
 *  \param  brick - Pointer to an initialized brick stucture.
 *  \author Jason W. Bacon
 *
 *  Open a connection to specified brick.  The connection handle is
 *  stored within the rct_brick_t structure.  The structure must be
 *  initialized with rct_init_brick() first.  This is normally done
 *  by rct_find_*().
 *
 *  Supported bricks/interfaces:
 *      - NXT
 *          - USB
 *          - Bluetooth
 */

rct_status_t     rct_open_brick(rct_brick_t * brick)

{
    switch (brick->brick_type)
    {
	case RCT_NXT:
	    return nxt_open_brick(&(brick->nxt));
	case RCT_RCX:
	    return rcx_open_brick(&(brick->rcx));
	case RCT_VEX:
	    return pic_open_controller(&(brick->vex),"/dev/cuad0");
	default:
	    break;
    }
    return RCT_INVALID_BRICK_TYPE;
}


/**
 *  \brief  Upload a file to the brick.
 *  \param  brick - Pointer to a brick structure with an open connection.
 *  \param  filename - name of the file on the local computer.
 *  \param  flags - upload mode
 *  \author Jason W. Bacon
 *
 *  Upload the specified file (e.g. an executable or sound file) to the
 *  brick.
 *  The brick must first be opened with rct_open_brick().
 *
 *  Valid flags:
 *      - RCT_NO_FLAGS
 *      - RCT_OVERWRITE - replace the file if it already exists
 *          on the brick.  Otherwise, upload file will fail and
 *          and return an error code.
 */

rct_status_t     rct_upload_file(rct_brick_t * brick, char *filename,
				rct_flag_t flags)
{
    switch (brick->brick_type)
    {
	case RCT_NXT:
	    return nxt_upload_file(&brick->nxt, filename,flags);
	case RCT_VEX:
	    return vex_upload_program(&brick->vex, filename);
	default:
	    break;
    }
    return RCT_INVALID_BRICK_TYPE;
}


/**
 *  \brief  Instruct the brick to play a sound file.
 *  \param  brick - Pointer to a brick structure with an open connection.
 *  \param  flags - Bit flags to control play mode.
 *  \param  filename - Name of sound file (must be on brick already).
 *  \author Jason W. Bacon
 *
 *  Play the specified sound file on the brick.  The filename must
 *  end in ".rso", and must have been previously uploaded to the brick.
 *  The brick must first be opened with rct_open_brick().
 *
 *  Valid flags:
 *      - RCT_NO_FLAGS
 *      - RCT_LOOP - Play file repeatedly until manually stopped
 *      by a stop command or the NXT stop button.
 *
 *  Supported bricks:
 *      - NXT
 */

rct_status_t     rct_play_sound_file(rct_brick_t * brick, 
				    rct_flag_t flags, char *filename)
{
    switch (brick->brick_type)
    {
	case RCT_NXT:
	    return nxt_play_sound_file(&brick->nxt, flags, filename);
	default:
	    break;
    }
    return RCT_INVALID_BRICK_TYPE;
}


/**
 *  \brief  Instruct the robo brick to play a tone.
 *  \param  brick - Pointer to a brick structure with an open connection.
 *  \param  herz - Frequency of the tone (200 - 14000Hz).
 *  \param  milliseconds - Duration of the tone.
 *  \author Jason W. Bacon
 *
 *  Play the specified tone for the specified duration.
 *  The brick must first be opened with rct_open_brick().
 *
 *  Supported bricks:
 *      - NXT
 */

rct_status_t     rct_play_tone(rct_brick_t * brick, 
				    int herz,int milliseconds)

{
    switch (brick->brick_type)
    {
	case RCT_NXT:
	    return nxt_play_tone(&brick->nxt, herz, milliseconds);
	default:
	    break;
    }
    return RCT_INVALID_BRICK_TYPE;
}


/****************************************************************************
 * Description: 
 *  Delete the named file from the brick, if it exists.
 *  The brick must first be opened with rct_open_brick().
 * Author:  Jason W. Bacon
 ***************************************************************************/

rct_status_t     rct_delete_file(rct_brick_t * brick, char *filename)

{
    switch (brick->brick_type)
    {
	case RCT_NXT:
	    return nxt_delete_file(&brick->nxt, filename);
	default:
	    break;
    }
    return RCT_INVALID_BRICK_TYPE;
}


/****************************************************************************
 * Description: 
 *  Run the named file.  The file must already exist on the brick, and
 *  must be a valid executable.
 *  The brick must first be opened with rct_open_brick().
 * Author:  Jason W. Bacon
 ***************************************************************************/

rct_status_t     rct_start_program(rct_brick_t * brick, char *filename)

{
    switch (brick->brick_type)
    {
	case RCT_NXT:
	    return nxt_start_program(&brick->nxt, filename);
	case RCT_VEX:
	    return pic_return_to_user_code(&brick->vex);
	default:
	    break;
    }
    return RCT_INVALID_BRICK_TYPE;
}


/****************************************************************************
 * Description: 
 *  Stop the program currently running on the brick.
 *  The brick must first be opened with rct_open_brick().
 * Author:  Jason W. Bacon
 ***************************************************************************/

rct_status_t     rct_stop_program(rct_brick_t * brick)

{
    switch (brick->brick_type)
    {
	case RCT_NXT:
	    return nxt_stop_program(&brick->nxt);
	default:
	    break;
    }
    return RCT_INVALID_BRICK_TYPE;
}


/****************************************************************************
 * Description: 
 *  Download the named file from the brick to the local computer.  The
 *  brick must first be opened with rct_open_brick.
 * Author:  Jason W. Bacon
 ***************************************************************************/

rct_status_t     rct_download_file(rct_brick_t * brick, char *filename)

{
    switch (brick->brick_type)
    {
	case RCT_NXT:
	    return nxt_download_file(&brick->nxt,filename);
	default:
	    break;
    }
    return RCT_INVALID_BRICK_TYPE;
}


/****************************************************************************
 * Description: 
 *  Close the connection to the brick.  The brick must first be opened with
 *  rct_open_brick().
 * Author:  Jason W. Bacon
 ***************************************************************************/

rct_status_t     rct_close_brick(rct_brick_t * brick)

{
    switch (brick->brick_type)
    {
	case RCT_NXT:
	    return nxt_close_brick(&brick->nxt);
	case RCT_VEX:
	    return pic_close_controller(&brick->vex);
	default:
	    break;
    }
    return RCT_OK;
}


/****************************************************************************
 * Description: 
 *  Retrieve the battery level from the brick.  The brick must first be
 *  opened with rct_open_brick().
 * Author:  Jason W. Bacon
 ***************************************************************************/

rct_status_t     rct_get_battery_level(rct_brick_t * brick)

{
    switch (brick->brick_type)
    {
	case RCT_NXT:
	    return nxt_get_battery_level(&brick->nxt);
	    break;
	default:
	    break;
    }
    return RCT_INVALID_BRICK_TYPE;
}


/****************************************************************************
 * Description: 
 *  Print the brick's battery level to stdout.
 *  The brick must first be opened with rct_open_brick().
 * Author:  Jason W. Bacon
 ***************************************************************************/

rct_status_t     rct_print_battery_level(rct_brick_t * brick)

{
    switch (brick->brick_type)
    {
	case RCT_NXT:
	    return nxt_print_battery_level(&brick->nxt);
	default:
	    break;
    }
    return RCT_INVALID_BRICK_TYPE;
}


/****************************************************************************
 * Description: 
 *  Retrieve the brick's firmware version.
 *  The brick must first be opened with rct_open_brick().
 * Author: 
 ***************************************************************************/

rct_status_t     rct_get_firmware_version(rct_brick_t * brick)

{
    switch (brick->brick_type)
    {
	case RCT_NXT:
	    return nxt_get_firmware_version(&brick->nxt);
	case RCT_VEX:
	    return pic_get_bootloader_version(&brick->vex);
	default:
	    break;
    }
    return RCT_INVALID_BRICK_TYPE;
}


/****************************************************************************
 * Description: 
 *  Print the brick's firmware version
 *  The brick must first be opened with rct_open_brick().
 * Author: 
 ***************************************************************************/

rct_status_t     rct_print_firmware_version(rct_brick_t * brick)

{
    switch (brick->brick_type)
    {
	case RCT_NXT:
	    return nxt_print_firmware_version(&brick->nxt);
	case RCT_VEX:
	    return pic_print_bootloader_version(&brick->vex);
	default:
	    break;
    }
    return RCT_INVALID_BRICK_TYPE;
}


/****************************************************************************
 * Description: 
 *  Print interesting information about the brick.
 *  The information collected varies from one brick type to the next,
 *  so output from this function will vary.
 *  The brick must first be opened with rct_open_brick().
 * Author: Jason W. Bacon
 ***************************************************************************/

rct_status_t     rct_print_device_info(rct_brick_t * brick)

{
    switch (brick->brick_type)
    {
	case RCT_NXT:
	    return nxt_print_device_info(&brick->nxt);
	case RCT_VEX:
	    return pic_print_bootloader_version(&brick->vex);
	default:
	    break;
    }
    return RCT_INVALID_BRICK_TYPE;
}

/**
 *  \brief
 *  \param
 *  \param
 *  \param
 *  \author
 */

rct_status_t    rct_motor_on(rct_brick_t *brick,int port,int power)

{
    switch (brick->brick_type)
    {
	case RCT_NXT:
	    return nxt_set_output_state(&brick->nxt,port,power,
		    NXT_MODE_MOTORON,NXT_REGULATION_MODE_MOTOR_SPEED,
		    0,NXT_RUN_STATE_RUNNING,0);
	default:
	    break;
    }
    return RCT_INVALID_BRICK_TYPE;
}

/** @} */

