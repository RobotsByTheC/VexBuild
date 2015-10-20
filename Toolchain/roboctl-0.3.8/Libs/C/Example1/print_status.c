#include <stdio.h>
#include <sysexits.h>
#include <legoctl/legoctl.h>

/*
 *  To compile:
 *      LOCALBASE ?= /usr/local
 *
 *      cc -o print_status -I${LOCALBASE}/include print_status.c 
 *              -L${LOCALBASE}/lib -llegoctl -lusb -lbluetooth
 */
 
int     main(int argc,char *argv[])

{
    lct_brick_list_t    bricks;
    lct_brick_t         *brick;
    
    lct_find_bricks(&bricks,LCT_PROBE_DEV_NXT);
    if ( lct_brick_count(&bricks) == 0 )
    {
	fputs("Sorry, no accessible bricks found.\n",stderr);
	exit(EX_UNAVAILABLE);
    }
    
    brick = lct_get_brick_from_list(&bricks,0);
    if ( lct_open_brick(brick) == LCT_OK )
    {
	lct_print_device_info(brick);
	lct_close_brick(brick);
    }
    return EX_OK;
}

