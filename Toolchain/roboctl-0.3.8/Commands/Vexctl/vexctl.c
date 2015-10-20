#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>
#include <roboctl.h>
#include "vexctl_cmd.h"
#include "protos.h"

extern int     Debug;

int     main(int argc,char *argv[])

{
    rct_cmd_t   cmd = RCT_CMD_STATUS;
    arg_t       arg_data = { NULL, DEFAULT_DEV };
    unsigned int    flags = 0;

    switch(argc)
    {
	case    1:
	    vexctl_usage(argv[0]);
	    break;
	default:
	    break;
    }

    if ( parse_args(argc,argv,&cmd,&arg_data,&flags) == RCT_OK )
    {
	return vexctl(cmd,&arg_data,flags);
    }
    else
	vexctl_usage(argv[0]);
    return 0;
}


int     vexctl(rct_cmd_t cmd,arg_t *arg_data,unsigned int flags)

{
    rct_status_t    status;
    rct_pic_t       pic;
    
    printf("Opening %s...\n",arg_data->device);
    fflush(stdout);
    if ( vex_open_controller(&pic,arg_data->device) != RCT_OK )
    {
	fprintf(stderr,"vexctl(): Could not open %s.\n",arg_data->device);
#if defined(__CYGWIN__)
	fputs("Check Device Manager->Ports to determine the correct com port.\n", stderr);
	fputs("Use vexctl --dev /dev/port ...\n", stderr);
	fputs("Example: vexctl --dev /dev/com5 --term upload prog.hex\n", stderr);
#endif
	return RCT_OPEN_FAILED;
    }
    switch(cmd)
    {
	case    RCT_CMD_STATUS:
	    status = vex_status(&pic);
	    break;
	case    RCT_CMD_UPLOAD:
	    status = vex_upload_program(&pic,arg_data->filename);
	    break;
	case    RCT_CMD_NONE:
	    status = RCT_OK;
	    break;
	default:
	    fprintf(stderr,"Internal error: Invalid command: %d.\n",cmd);
	    fputs("This is a program bug.  Please report it to the author(s).\n",stderr);
	    exit(EX_SOFTWARE);
    }
    
    if ( flags & VEX_MONITOR )
	monitor_controller(&pic);
    
    vex_close_controller(&pic);
    
    if ( (status == RCT_OK) && (flags & VEX_LAUNCH_PROG) )
	return system(arg_data->launch_prog);
    else
	return status;
}


void    vexctl_usage(char *progname)

{
    fputs("Usage:\n",stderr);
    fprintf(stderr,"\t%s [flags] [status]\n",progname);
    fprintf(stderr,"\t%s [flags] [upload <filename>]\n",progname);
    fputs("\nFlags:\n",stderr);
    fputs("\t--debug        Enable debugging output.\n",stderr);
    fputs("\t--dev dev      Use serial port dev instead of the default.\n",stderr);
    fputs("\t--launch prog  Launch prog after successful command.\n",stderr);
    fputs("\t--monitor      Monitor output after successful command.\n",stderr);
    exit(EX_USAGE);
}


int     parse_args(int argc,char *argv[],rct_cmd_t *cmd,
		    arg_t *arg_data,unsigned int *flags)

{
    int     arg;

    *cmd = RCT_CMD_NONE;
    
    if ( parse_global_flags(argc,argv,cmd,arg_data,flags,&arg) != RCT_OK )
	return RCT_USAGE;

    if ( strcmp(argv[arg],"status") == 0 )
    {
	*cmd = RCT_CMD_STATUS;
	if ( arg != argc-1 )
	    vexctl_usage(argv[0]);
    }
    else if ( strcmp(argv[arg],"upload") == 0 )
    {
	*cmd = RCT_CMD_UPLOAD;
	/* The next argument should be the last */
	if ( arg == argc - 2 )
	    arg_data->filename = argv[++arg];
	else
	    vexctl_usage(argv[0]);
    }
    
    if ( *cmd == RCT_CMD_NONE )
    {
	if ( *flags & VEX_MONITOR )
	{
	    return RCT_OK;
	}
	else
	{
	    vexctl_usage(argv[0]);
	    /*
	     *  vexctl_usage() exits, but the compiler will complain about
	     *  reaching the end of a non-void function without this return.
	     */
	    return RCT_OK;
	}
    }
    else
	return RCT_OK;
}


int     parse_global_flags(int argc,char *argv[],rct_cmd_t *cmd,
		    arg_t *arg_data,unsigned int *flags,int *argp)

{
    int arg = *argp;
    extern int  Debug;
    char        *temp;
    
    for (arg=1; (arg < argc) && (argv[arg][0] == '-'); ++arg)
    {
	if ( strcmp(argv[arg],"--debug") == 0 )
	{
	    Debug = 1;
	}
	else if ( strcmp(argv[arg],"--dev") == 0 )
	{
	    arg_data->device = argv[++arg];
	}
	else if ( strcmp(argv[arg],"--launch") == 0 )
	{
	    *flags |= VEX_LAUNCH_PROG;
	    arg_data->launch_prog = argv[++arg];
	}
	else if ( strcmp(argv[arg],"--monitor") == 0 )
	{
	    *flags |= VEX_MONITOR;
	}
	else
	{
	    fprintf(stderr,"Invalid flag: %s\n",argv[arg]);
	    vexctl_usage(argv[0]);
	}
    }
    
    if ( arg == argc )
    {
	if ( *flags & VEX_MONITOR )
	    return RCT_OK;
	else
	    vexctl_usage(argv[0]);
    }
    
    /* This is a pointer compare, not a string compare */
    if ( strcmp(arg_data->device, DEFAULT_DEV) )
    {
	if ( (temp = getenv("VEXCTL_DEV")) != NULL )
	{
	    arg_data->device = temp;
	}
    }
    
    *argp = arg;
    return RCT_OK;
}


void    monitor_controller(rct_pic_t *pic)

{
    char    ch;

    while ( read(pic->fd, &ch, 1) == 1 )
	putchar(ch);
}

