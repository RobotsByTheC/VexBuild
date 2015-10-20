#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sysexits.h>
#include <roboctl.h>
#include "legoctl_cmd.h"
#include "protos.h"

extern int     Debug;

int     main(int argc,char *argv[])

{
    arg_t   arg_data = {"",NULL,0,0};
    rct_cmd_t   cmd = RCT_CMD_STATUS;
    unsigned int    flags = RCT_PROBE_DEV_ALL;

    switch(argc)
    {
	case    1:
	    legoctl_usage(argv[0]);
	    break;
	default:
	    break;
    }

    if ( parse_args(argc,argv,&cmd,&arg_data,&flags) == RCT_OK )
    {
	return legoctl(cmd,&arg_data,flags);
    }
    else
	return EX_USAGE;
}


int     legoctl(rct_cmd_t cmd,arg_t *arg_data,unsigned int flags)

{
    rct_brick_list_t    bricks;

    rct_find_bricks(&bricks,arg_data->bluetooth_name,RCT_PROBE_ALL);
    debug_printf("Found %d bricks...\n",rct_brick_count(&bricks));
    if ( rct_brick_count(&bricks) == 0 )
    {
	fputs("Sorry, no accessible bricks found.\n",stderr);
	exit(EX_UNAVAILABLE);
    }
    
    switch(cmd)
    {
	case    RCT_CMD_STATUS:
	case    RCT_CMD_STOP:
	    return multi_brick_cmd(&bricks,cmd,flags);
	case    RCT_CMD_UPLOAD:
	case    RCT_CMD_DELETE:
	case    RCT_CMD_START:
	case    RCT_CMD_PLAY_SOUND:
	    return file_cmd(&bricks,arg_data->filename,cmd,flags);
	case    RCT_CMD_PLAY_TONE:
	    return play_tone(&bricks,arg_data->herz,arg_data->milliseconds);
	case    RCT_CMD_DOWNLOAD:
	case    RCT_CMD_FIRM_UP:
	case    RCT_CMD_FIRM_DOWN:
	    fputs("This command is not yet implemented.\n",stderr);
	    break;
	default:
	    fprintf(stderr,"Internal error: Invalid command: %d.\n",cmd);
	    fputs("This is a program bug.  Please report it to the author(s).\n",stderr);
	    exit(EX_SOFTWARE);
    }
    return 0;
}


int     multi_brick_cmd(rct_brick_list_t *bricks,rct_cmd_t cmd,unsigned int flags)

{
    rct_brick_t         *brick;
    int     c;
    
    for (c=0; c<rct_brick_count(bricks); ++c)
    {
	brick = rct_get_brick_from_list(bricks,c);
	if ( rct_open_brick(brick) == RCT_OK )
	{
	    switch(cmd)
	    {
		case    RCT_CMD_STATUS:
		    /* Print device info */
		    if ( rct_print_device_info(brick) != 0 )
			return 1;
		    
		    /* Print firmware version */
		    rct_print_firmware_version(brick);
		    
		    /* Print battery level */
		    rct_print_battery_level(brick);
		    break;
		case    RCT_CMD_STOP:
		    rct_stop_program(brick);
		    break;
		default:
		    break;
	    }
	    
	    rct_close_brick(rct_get_brick_from_list(bricks,c));
	}
	else
	{
	    fprintf(stderr,"Error opening brick.\n");
	    return 1;
	}
    }
    return 0;
}


int     file_cmd(rct_brick_list_t *bricks,char *filename,rct_cmd_t cmd,
		    unsigned int flags)

{
    rct_brick_t *brick;
    rct_flag_t  loop;
    
    switch(rct_brick_count(bricks))
    {
	case    0:
	    fputs("Sorry, accessible no bricks found.\n",stderr);
	    exit(EX_UNAVAILABLE);
	case    1:
	    brick = rct_get_brick_from_list(bricks,0);
	    if ( rct_open_brick(brick) == RCT_OK )
	    {
		switch(cmd)
		{
		    case    RCT_CMD_START:
			rct_start_program(brick,filename);
			break;
		    case    RCT_CMD_UPLOAD:
			if ( flags & RCT_OVERWRITE )
			    rct_delete_file(brick,filename);
			rct_upload_file(brick,filename,RCT_UPLOAD_PLAY_SOUND);
			break;
		    case    RCT_CMD_DELETE:
			rct_delete_file(brick,filename);
			break;
		    case    RCT_CMD_PLAY_SOUND:
			loop = ((flags & RCT_LOOP) != 0);
			rct_play_sound_file(brick,loop,filename);
			break;
		    default:
			break;
		}
		rct_close_brick(brick);
	    }
	    break;
	default:
	    fputs("Error: multiple bricks connected.  Don't know which one to upload to.\n",stderr);
	    exit(EX_DATAERR);
    }
    return 0;
}


int     play_tone(rct_brick_list_t *bricks,int herz,int milliseconds)

{
    rct_brick_t *brick;
    
    switch(rct_brick_count(bricks))
    {
	case    0:
	    fputs("Sorry, accessible no bricks found.\n",stderr);
	    exit(EX_UNAVAILABLE);
	case    1:
	    brick = rct_get_brick_from_list(bricks,0);
	    if ( rct_open_brick(brick) == RCT_OK )
	    {
		rct_play_tone(brick,herz,milliseconds);
		rct_close_brick(brick);
	    }
	    break;
	default:
	    fputs("Error: multiple bricks connected.  Don't know which one to upload to.\n",stderr);
	    exit(EX_DATAERR);
    }
    return 0;
}


void    legoctl_usage(char *progname)

{
    fputs("Usage:\n",stderr);
    fprintf(stderr,"\t%s [flags] status\n",progname);
    fprintf(stderr,"\t%s [flags] upload <filename> [slot #]\n",progname);
    fprintf(stderr,"\t%s [flags] playsound <filename>\n",progname);
    fprintf(stderr,"\t%s [flags] playtone <herz> <milliseconds>\n",progname);
    //fprintf(stderr,"\t%s [flags] download <filename|all> [slot #]\n",progname);
    fprintf(stderr,"\t%s [flags] delete <filename> [slot #]\n",progname);
    //fprintf(stderr,"\t%s [flags] firmware_up <filename>\n",progname);
    //fprintf(stderr,"\t%s [flags] firmware_down <filename>\n",progname);
    fprintf(stderr,"\t%s [flags] start <filename|slot #>\n",progname);
    fprintf(stderr,"\t%s [flags] stop\n",progname);
    fputs("\nFlags:\n",stderr);
    //fputs("\t--nxt       probe for NXT only\n",stderr);
    //fputs("\t--usb       probe USB busses only\n",stderr);
    //fputs("\t--bluetooth probe bluetooth interfaces only\n",stderr);
    //fputs("\t--rcx       probe for RCX only\n",stderr);
    fputs("\t--overwrite overwrite existing files on brick\n",stderr);
    fputs("\t--loop      repeat command indefinitely\n",stderr);
    fputs("\t--debug     enable debugging output\n",stderr);
    fputs("\t--btname name  Specify a non-default bluetooth name\n", stderr);
    exit(EX_USAGE);
}


int     parse_args(int argc,char *argv[],rct_cmd_t *cmd,arg_t *arg_data,
	unsigned int *flags)

{
    int     arg;
    char    *end;
    
    for (arg = 1; argv[arg] != NULL; ++arg)
    {
	if ( strcmp(argv[arg],"status") == 0 )
	{
	    *cmd = RCT_CMD_STATUS;
	}
	else if ( strcmp(argv[arg],"upload") == 0 )
	{
	    *cmd = RCT_CMD_UPLOAD;
	    /* The next argument should be the last */
	    if ( arg == argc - 2 )
		arg_data->filename = argv[++arg];
	    else
		legoctl_usage(argv[0]);
	}
	else if ( strcmp(argv[arg],"playsound") == 0 )
	{
	    *cmd = RCT_CMD_PLAY_SOUND;
	    /* The next argument should be the last */
	    if ( arg == argc - 2 )
		arg_data->filename = argv[++arg];
	    else
		legoctl_usage(argv[0]);
	}
	else if ( strcmp(argv[arg],"playtone") == 0 )
	{
	    *cmd = RCT_CMD_PLAY_TONE;
	    /* The next argument should be the last */
	    if ( arg == argc - 3 )
	    {
		arg_data->herz = strtol(argv[++arg],&end,0);
		if ( end == argv[arg] )
		{
		    fprintf(stderr,"Invalid herz value: '%s'\n",argv[arg]);
		    legoctl_usage(argv[0]);
		}
		arg_data->milliseconds = strtol(argv[++arg],&end,0);
		if ( end == argv[arg] )
		{
		    fprintf(stderr,"Invalid milliseconds value: '%s'\n",argv[arg]);
		    legoctl_usage(argv[0]);
		}
	    }
	    else
		legoctl_usage(argv[0]);
	}
	else if ( strcmp(argv[arg],"download") == 0 )
	{
	    *cmd = RCT_CMD_DOWNLOAD;
	}
	else if ( strcmp(argv[arg],"firmware_up") == 0 )
	{
	    *cmd = RCT_CMD_FIRM_UP;
	    /* The next argument should be the last */
	    /*if ( arg == argc - 2 )
		arg_data->filename = argv[++arg];
	    else
		legoctl_usage(argv[0]);
	    */
	}
	else if ( strcmp(argv[arg],"firmware_down") == 0 )
	{
	    *cmd = RCT_CMD_FIRM_DOWN;
	}
	else if ( strcmp(argv[arg],"delete") == 0 )
	{
	    *cmd = RCT_CMD_DELETE;
	    /* The next argument should be the last */
	    if ( arg == argc - 2 )
		arg_data->filename = argv[++arg];
	    else
		legoctl_usage(argv[0]);
	}
	else if ( strcmp(argv[arg],"start") == 0 )
	{
	    *cmd = RCT_CMD_START;
	    /* The next argument should be the last */
	    if ( arg == argc - 2 )
		arg_data->filename = argv[++arg];
	    else
		legoctl_usage(argv[0]);
	}
	else if ( strcmp(argv[arg],"stop") == 0 )
	{
	    *cmd = RCT_CMD_STOP;
	}
	else if ( strcmp(argv[arg],"--nxt") == 0 )
	{
	    *flags |= RCT_PROBE_DEV_NXT;
	}
	else if ( strcmp(argv[arg],"--rcx") == 0 )
	{
	    *flags |= RCT_PROBE_DEV_RCX;
	}
	else if ( strcmp(argv[arg],"--overwrite") == 0 )
	{
	    *flags |= RCT_OVERWRITE;
	}
	else if ( strcmp(argv[arg],"--loop") == 0 )
	{
	    *flags |= RCT_LOOP;
	}
	else if ( strcmp(argv[arg],"--debug") == 0 )
	{
	    Debug = 1;
	}
	else if ( strcmp(argv[arg],"--btname") == 0 )
	{
	    arg_data->bluetooth_name = argv[++arg];
	}
	else
	{
	    fprintf(stderr,"Invalid option: %s\n",argv[arg]);
	    legoctl_usage(argv[0]);
	}
    }
    return RCT_OK;
}

