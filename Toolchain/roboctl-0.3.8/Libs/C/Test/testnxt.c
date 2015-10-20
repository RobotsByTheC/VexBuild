#include <stdio.h>
#include <sysexits.h>
#include <roboctl.h>

void    check_status(rct_status_t status);
int     test_nxt(void);

int     main(int argc,char *argv[])

{
    test_nxt();
    return 0;
}


int     test_nxt()

{
    rct_brick_list_t    bricks;
    rct_brick_t         *brick;
    rct_status_t        status;
    int                 tone;
    
    rct_find_bricks(&bricks,RCT_PROBE_DEV_NXT);
    if ( rct_brick_count(&bricks) == 0 )
    {
	fputs("Sorry, no accessible bricks found.\n",stderr);
	return EX_UNAVAILABLE;
    }

    brick = rct_get_brick_from_list(&bricks,0);
    if ( rct_open_brick(brick) == RCT_OK )
    {
	puts("Getting info...");
	check_status(rct_print_device_info(brick));
	
	puts("Uploading pong.rxe...");
	check_status(rct_upload_file(brick,"pong.rxe",RCT_UPLOAD_PLAY_SOUND));
	
	fputs("Running pong.  Check display and press return to continue...",stdout);
	status = rct_start_program(brick,"pong");
	getchar();
	check_status(status);
	
	puts("Stopping pong...");
	check_status(rct_stop_program(brick));
	
	puts("Deleting pong.rxe");
	check_status(rct_delete_file(brick,"pong.rxe"));
	
	puts("Saying 'Woops'...");
	check_status(rct_play_sound_file(brick,RCT_NO_FLAGS,"Woops"));
	
	puts("Playing tones...");
	status = RCT_OK;
	for (tone=200; tone<=14000; tone+=200)
	{
	    printf("%d ",tone);
	    fflush(stdout);
	    if ( rct_play_tone(brick,tone,50) != RCT_OK )
	    {
		puts("Failed.");
		status = RCT_COMMAND_FAILED;
	    }
	}
	putchar('\n');
	check_status(status);
	
	rct_close_brick(brick);
    }
    return 0;
}


void    check_status(rct_status_t status)

{
    if ( status == RCT_OK )
	puts("OK\n");
    else
	puts("**** FAILED ****\n");
    sleep(2);
}

