#include <stdio.h>
#include <stdarg.h>

int Debug = 0;

int     debug_printf(char *format,...)

{
    int     status = 0;
    va_list ap;
    
    if ( Debug )
    {
	va_start(ap,format);
	status = vfprintf(stdout,format,ap);
	fflush(stdout);
	va_end(ap);
    }
    return status;
}

