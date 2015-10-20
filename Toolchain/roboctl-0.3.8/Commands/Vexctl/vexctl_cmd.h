
typedef struct
{
    char    *filename;
    char    *device;
    char    *launch_prog;
}   arg_t;

#if defined(__FreeBSD__)
/* Prolific USB to serial driver */
/* Standard serial port #define DEFAULT_DEV "/dev/cuad0" */
#define DEFAULT_DEV "/dev/cuaU0"
#define VEX_TERMINAL "cutecom"

#elif defined(__NetBSD__)
/* Prolific USB to serial driver */
/* Standard serial port #define DEFAULT_DEV "/dev/cuad0" */
#define DEFAULT_DEV "/dev/cuaU0"
#define VEX_TERMINAL "cutecom"

#elif defined(__linux__)
/* Prolific USB to serial driver */
#define DEFAULT_DEV "/dev/ttyUSB0"
#define VEX_TERMINAL "cutecom"

#elif defined(__APPLE__)
#define DEFAULT_DEV "/dev/cu.usbserial"
/* Opensource driver from SourceForge */
/* #define DEFAULT_DEV     "/dev/cu.PL2303-0000101D" */
#define VEX_TERMINAL "cutecom"

/* com3 (ttyS2) (on a Gateway laptop) just echoes the command, even though
   there's no real device there. This makes it hard to detect communication
   errors. Test on other machines. */
#elif defined(__CYGWIN__)
#define DEFAULT_DEV "/dev/com4"
#define VEX_TERMINAL "Terminal"     /* Bray++ Terminal */

#elif defined(sun)
#define DEFAULT_DEV "/dev/cua/0"
#define VEX_TERMINAL "cutecom"

#endif

/* Bit flags */
#define VEX_LAUNCH_PROG 0x0001
#define VEX_MONITOR     0X0002

