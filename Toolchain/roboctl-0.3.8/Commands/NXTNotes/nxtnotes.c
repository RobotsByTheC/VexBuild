#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sysexits.h>
#include <termios.h>
#include <legoctl.h>
#include "nxtnotes.h"
#include "protos.h"

int     Debug = 0;

int     main(int argc,char *argv[])

{
    lct_brick_list_t    bricks;
    lct_brick_t         *brick;
    lct_note_t          notes[] = { LCT_NOTE_A3,
				    LCT_NOTE_B3,
				    LCT_NOTE_C4,
				    LCT_NOTE_D4,
				    LCT_NOTE_E4,
				    LCT_NOTE_F4,
				    LCT_NOTE_G4
				},
			sharp[] = { LCT_NOTE_AS3,
				    0,
				    LCT_NOTE_CS4,
				    LCT_NOTE_DS4,
				    0,
				    LCT_NOTE_FS4,
				    LCT_NOTE_GS4
				};
    int                 ch, 
			scale = 1,
			freq;
    struct termios  cooked,
		    raw;
    
    tcgetattr(0,&cooked);
    raw = cooked;
    raw.c_lflag &= ~(ECHO|ICANON);
    
    lct_find_bricks(&bricks,LCT_PROBE_DEV_NXT);
    debug_printf("Found %d bricks...\n",lct_brick_count(&bricks));
    if ( lct_brick_count(&bricks) == 0 )
    {
	fputs("Sorry, no accessible bricks found.\n",stderr);
	exit(EX_UNAVAILABLE);
    }
    
    brick = lct_get_brick_from_list(&bricks,0);
    if ( lct_open_brick(brick) == LCT_OK )
    {
	lct_print_device_info(brick);
	tcsetattr(0,TCSANOW,&raw);
	while ( (ch = getchar()) != 'q' )
	{
	    if ( (ch == ',') && (scale > 1) )
		scale >>= 1;
	    else if ( (ch == '.') && (scale <= 32) )
		scale <<= 1;
	    else
	    {
		if ( (ch >= 'a') && (ch <= 'g') )
		    freq = notes[ch-'a'] * scale;
		else if ( (ch >= 'A') && (ch <= 'G') )
		    freq = sharp[ch-'A'] * scale;
		else
		    freq = 0;   /* Invalid key */
		if ( (freq >= LCT_NOTE_GS3) && (freq <= LCT_NOTE_DS8) )
		{
		    printf("%d %d\n",scale,freq);
		    lct_play_tone(brick,freq,200);
		}
	    }
	}
	tcsetattr(0,TCSANOW,&cooked);
	lct_close_brick(brick);
    }
    return EX_OK;
}

