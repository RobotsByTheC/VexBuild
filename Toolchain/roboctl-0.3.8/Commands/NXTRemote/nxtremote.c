#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <sysexits.h>
#include <roboctl.h>
#include <libgamepad/gamepad.h>
#include "nxtremote.h"

/* 
 *  FreeBSD:
 *  
 *  For instructions on enabling the PC joystick port, run "man joy"
 *
 *  /dev/joy returns values roughly from 0 to 1000 for x and y.
 *  Joysticks can be noisy: x and y are not steady, but spontaneously
 *  vary.  Noise on my Kraft joystick is generally only 1 or 2.  My
 *  (cheap) PC Mission varies by as much as 30 units.  Libgamepad
 *  filters out the noise by ignoring small changes.
 *
 *  USB gamepads are identified as /dev/uhid*.
 */


int     main(int argc,char *argv[])

{
    int     x, y, c, bytes;
    extern int  Debug;
    rct_brick_list_t    bricks;
    rct_brick_t         *brick;
    gamepad_t           *gp;
    settings_t          settings;
    char                *bt_name = NULL;    /* Defaults to NXT */
    
    Debug = 0;
    
    process_args(argc,argv,&bt_name,&settings);
    
    /* Open gamepad. Use O_NONBLOCK to put USB gamepads into immediate
       mode.  This allows sampling of the device state instead of
       the device sending packets for every change.  Analog joysticks
       really always work this way, but the gameport_analog_read() routine
       simulates blocking I/O by looping until the device state changes
       (with usleep() to keep CPU usage down) if O_NONBLOCK is not set. */
       
    /*
     *  FIXME: O_NONBLOCK causes read() failed errors on FreeBSD 8+.
     *  Apparently the return values changed for /dev/uhid0.
     */
    
    gp = gamepad_open(settings.device,GAMEPAD_INTERFACE_AUTO,
		    O_RDONLY);//|O_NONBLOCK);
    if ( gp == NULL )
    {
	fprintf(stderr,"Cannot open %s.\n",settings.device);
	return EX_UNAVAILABLE;
    }
    
    /* Open connection to NXT. */
    rct_find_bricks(&bricks,bt_name,RCT_PROBE_DEV_NXT);
    brick = rct_get_brick_from_list(&bricks,0);
    if ( rct_open_brick(brick) != RCT_OK )
    {
	return EX_UNAVAILABLE;
    }
    rct_print_device_info(brick);
    
    /* Disable NXT response packets to reduce communication overhead. */
    nxt_response_off(&brick->nxt);

    c =0;
    while ( (bytes = gamepad_read(gp)) >= 0 )
    {
	if ( bytes > 0 )
	{
	    /* Get joystick coordinates in Cartesian plane from -100 to +100 */
	    x = joy_scaled_x(gamepad_x(gp),gamepad_max_x(gp));
	    y = -joy_scaled_y(gamepad_y(gp),gamepad_max_y(gp));
	    
	    control_motion(brick,x,y,&settings);
    
	    if ( settings.button )
		control_implement_with_button(brick,gamepad_button(gp,1),
			gamepad_button(gp,2),&settings);
	    else
		control_implement_with_joystick(brick,
			-gamepad_z(gp),gamepad_max_z(gp), &settings);

	    /* NXT will go to sleep unless it's running a program, so reset
	       timer occasionally. */
	    if ( ++c % 1000 == 0 )
		nxt_keep_alive(&brick->nxt);
	}
    }
    gamepad_close(gp);
    rct_close_brick(brick);
    return EX_OK;
}


/*
 *  Convert raw joystick input (typically 0 for left/up and some large
 *  number for right/down) to Cartesian coordinates betweeen -100 and 100.
 */

int     joy_scaled_x(int x,int max_x)

{
    /* Scale to range of -100 (left) to +100 (right) */
    x = (x - (max_x/2)) / (max_x/200);
    if ( x < 0 )
	return MAX(x,-100);
    else
	return MIN(x,100);
}


int     joy_scaled_y(int y,int max_y)

{
    /* Scale to range of -100 (down) to +100 (up) */
    y = -((y - (max_y/2)) / (max_y/200));
    if ( y < 0 )
	return MAX(y,-100);
    else
	return MIN(y,100);
}


void    control_motion(rct_brick_t *brick,int x,int y,settings_t *settings)

{
    int     left_speed,
	    right_speed;
    
    //arcade_drive(x, y, 100, 100, 4.5f, &left_speed, &right_speed);
    
    /* Scale down according to settings */
    //left_speed = left_speed * settings->wheel_power / 100;
    //right_speed = right_speed * settings->wheel_power / 100;
    
    printf("Joystick: %4d %4d   Power: %4d %4d  ",
	    x,y,left_speed,right_speed);
    fflush(stdout);
    
    nxt_set_output_state(&(brick->nxt),DRIVE_MOTOR_PORT,y,
	NXT_MODE_MOTORON,NXT_REGULATION_MODE_IDLE,
	0,NXT_RUN_STATE_RUNNING,0);
    nxt_set_output_state(&(brick->nxt),STEER_MOTOR_PORT,-x,
	NXT_MODE_MOTORON,NXT_REGULATION_MODE_IDLE,
	0,NXT_RUN_STATE_RUNNING,0);
}


/**
 *  Control a simple motor-driven implement using two joystick/gamepad
 *  buttons.  Full power one way if button1 is pressed, and full power the
 *  other way if button2 is pressed.
 */

void    control_implement_with_button(rct_brick_t *brick,int b1,int b2,settings_t *settings)

{
    static int  old_implement_power = 0,
		implement_power;
    
    if ( b2 == 1 )
	implement_power = settings->implement_power;
    else if ( b1 == 1 )
	implement_power = -settings->implement_power;
    else
	implement_power=0;
    
    if ( implement_power != old_implement_power )
    {
	nxt_set_output_state(&(brick->nxt),IMPLEMENT_MOTOR_PORT,
	    implement_power,
	    NXT_MODE_MOTORON,NXT_REGULATION_MODE_IDLE,
	    0,NXT_RUN_STATE_RUNNING,0);
    }
    printf("Implement power: %4d\r",implement_power);
    old_implement_power = implement_power;
}


/**
 *  Control a simple motor-driven implement by mapping joystick Y 
 *  position information to motor power.
 */

void    control_implement_with_joystick(rct_brick_t *brick,int z,int max_z,
			    settings_t *settings)

{
    static int  old_implement_power = 0,
		implement_power;
    
    
    /* Get percent power between 0 and 100 */
    implement_power = 100 * (2 * z - max_z) / max_z;
    
    /* Warp by taking the 4th root of the motor power, and normalize back
       to a range of 0 to 100 by dividing by 100^(1/4) */
    if ( implement_power >= 0 )
	implement_power = settings->implement_power * 
				pow((double)implement_power,0.25) / 3.16;
    else
	implement_power = -settings->implement_power *
				pow((double)-implement_power,0.25) / 3.16;
    
    if ( implement_power != old_implement_power )
    {
	nxt_set_output_state(&(brick->nxt),IMPLEMENT_MOTOR_PORT,
	    implement_power,
	    NXT_MODE_MOTORON,NXT_REGULATION_MODE_IDLE,
	    0,NXT_RUN_STATE_RUNNING,0);
    }
    printf("Implement power: %4d\r",implement_power);
    old_implement_power = implement_power;
}


int     process_args(int argc,char **argv,char **bt_name, settings_t *settings)

{
    char    *end;
    
    switch(argc)
    {
	case    7:
	    if ( strcmp(argv[1],"--btname") != 0 )
		usage(argv);
	    *bt_name = argv[2];
	    argv += 2;
	case    5:
	    settings->device=argv[1];
	    settings->wheel_power = strtol(argv[2],&end,10);
	    if ( end-argv[2] != strlen(argv[2]) )
	    {
		fprintf(stderr,"%s is not a valid power value.  Must be from 0 to 100.\n",argv[2]);
		exit(EX_USAGE);
	    }
	    settings->implement_power = strtol(argv[3],&end,10);
	    if ( end-argv[3] != strlen(argv[3]) )
	    {
		fprintf(stderr,"%s is not a valid power value.  Must be from 0 to 100.\n",argv[3]);
		exit(EX_USAGE);
	    }
	    if ( strcmp(argv[4],"button") == 0 )
		settings->button = 1;
	    else if ( strcmp(argv[4],"joystick") == 0 )
		settings->button = 0;
	    else
		fputs("Last argument must be 'button' or 'joystick'\n",stderr);
	    break;
	default:
	    usage(argv);
    }
    return 0;
}


void    usage(char *argv[])

{
    fprintf(stderr,"\nUsage:\n\n%s [--btname name] <device> <max wheel power>\n\t<max implement power> <implement control>\n",argv[0]);
    fputs("(Default btname = NXT)\n",stderr);
    fputs("\nExamples:\n",stderr);
    fprintf(stderr,"    %s /dev/joy0 85 63 button\n",argv[0]);
    fprintf(stderr,"    %s /dev/uhid0 90 68 joystick\n",argv[0]);
    exit(EX_USAGE);
}

