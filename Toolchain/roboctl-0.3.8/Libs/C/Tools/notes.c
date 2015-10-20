#include <stdio.h>
#include <math.h>

int     main(int argc,char *argv[])

{
    double  f = 440.0,
	    ratio = pow(2,1.0/12.0);
    
    while ( f > 17.0 )
    {
	f /= ratio;
    }
    
    while ( f < 4979.0 )
    {
	printf("    LCT_NOTE_    = %6.0f, /* %6.2f */\n",rint(f),f);
	f *= ratio;
    }
    
    return 0;
}

