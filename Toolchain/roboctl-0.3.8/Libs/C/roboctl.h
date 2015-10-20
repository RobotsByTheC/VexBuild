#ifndef _TIME_H_
#include <sys/time.h>
#endif

#include "rct_machdep.h"
#include "rct_rcx.h"
#include "rct_nxt.h"
#include "rct_pic.h"

#define     RCT_MAX_BRICKS      64
#define     RCT_FIRMWARE_LEN    64

typedef enum
{
    RCT_NO_FLAGS=           0,
    RCT_PROBE_DEV_NXT=      0x0001,
    RCT_PROBE_DEV_RCX=      0x0002,
    RCT_PROBE_DEV_ALL=      (RCT_PROBE_DEV_NXT|RCT_PROBE_DEV_RCX),

    RCT_PROBE_INT_USB=      0x0010,
    RCT_PROBE_INT_BLUETOOTH=0x0020,
    RCT_PROBE_INT_SERIAL=   0x0030,
    RCT_PROBE_INT_ALL=      \
	(RCT_PROBE_INT_USB|RCT_PROBE_INT_BLUETOOTH|RCT_PROBE_INT_SERIAL),

    RCT_PROBE_ALL=          RCT_PROBE_DEV_ALL|RCT_PROBE_INT_ALL,

    RCT_OVERWRITE=          0x0100,
    RCT_LOOP=               0x0200, 
    RCT_UPLOAD_PLAY_SOUND = 0x0400
}   rct_flag_t;

// USB ID codes
#define     RCT_VENDOR_LEGO         0x0694
#define     RCT_PRODUCT_NXT         0x0002

typedef enum
{
    X = 1,
    Y = 2,
    Z = X|Y
}   junk_t;

typedef enum
{
    RCT_OK = 0,
    RCT_INVALID_BRICK_TYPE,
    RCT_NOT_IMPLEMENTED,
    RCT_OPEN_FAILED,
    RCT_COMMAND_FAILED,
    RCT_NOT_CONNECTED,
    RCT_INVALID_FILENAME,
    RCT_CANNOT_STAT_FILE,
    RCT_CANNOT_OPEN_FILE,
    RCT_CANNOT_CLAIM_INTERFACE,
    RCT_CANNOT_CREATE_SOCKET,
    RCT_CANNOT_CONNECT_SOCKET,
    RCT_CANNOT_BIND_SOCKET,
    RCT_INVALID_DATA,
    RCT_USAGE
}   rct_status_t;

typedef enum {
    RCT_CMD_NONE,
    RCT_CMD_STATUS,
    RCT_CMD_UPLOAD,
    RCT_CMD_DOWNLOAD,
    RCT_CMD_FIRM_UP,
    RCT_CMD_FIRM_DOWN,
    RCT_CMD_DELETE,
    RCT_CMD_START,
    RCT_CMD_STOP,
    RCT_CMD_PLAY_SOUND,
    RCT_CMD_PLAY_TONE
}   rct_cmd_t;

typedef enum
{
    RCT_RCX,
    RCT_NXT,
    RCT_VEX
}   rct_brick_type_t;

/* 
 *  Musical notes for *_play_tone().
 *  The NXT bottoms out at 200Hz.  Lower frequencies are included for
 *  bricks supported in the future.  There are no notes defined for
 *  frequencies above 4978 Hz.  I guess nobody can sing that high, but
 *  I wouldn't know; I'm just an engineer.
 */

typedef enum
{
#if 0    
    RCT_NOTE_C0  =     16, /*  16.35 */
    RCT_NOTE_CS0 =     17, /*  17.32 */
    RCT_NOTE_D0  =     18, /*  18.35 */
    RCT_NOTE_DS0 =     19, /*  19.45 */
    RCT_NOTE_E0  =     21, /*  20.60 */
    RCT_NOTE_F0  =     22, /*  21.83 */
    RCT_NOTE_FS0 =     23, /*  23.12 */
    RCT_NOTE_G0  =     24, /*  24.50 */
    RCT_NOTE_GS0 =     26, /*  25.96 */
    RCT_NOTE_A0  =     27, /*  27.50 */
    RCT_NOTE_AS0 =     29, /*  29.14 */
    RCT_NOTE_B0  =     31, /*  30.87 */
    RCT_NOTE_C1  =     33, /*  32.70 */
    RCT_NOTE_CS1 =     35, /*  34.65 */
    RCT_NOTE_D1  =     37, /*  36.71 */
    RCT_NOTE_DS1 =     39, /*  38.89 */
    RCT_NOTE_E1  =     41, /*  41.20 */
    RCT_NOTE_F1  =     44, /*  43.65 */
    RCT_NOTE_FS1 =     46, /*  46.25 */
    RCT_NOTE_G1  =     49, /*  49.00 */
    RCT_NOTE_GS1 =     52, /*  51.91 */
    RCT_NOTE_A1  =     55, /*  55.00 */
    RCT_NOTE_AS1 =     58, /*  58.27 */
    RCT_NOTE_B1  =     62, /*  61.74 */
    RCT_NOTE_C2  =     65, /*  65.41 */
    RCT_NOTE_CS2 =     69, /*  69.30 */
    RCT_NOTE_D2  =     73, /*  73.42 */
    RCT_NOTE_DS2 =     78, /*  77.78 */
    RCT_NOTE_E2  =     82, /*  82.41 */
    RCT_NOTE_F2  =     87, /*  87.31 */
    RCT_NOTE_FS2 =     92, /*  92.50 */
    RCT_NOTE_G2  =     98, /*  98.00 */
    RCT_NOTE_GS2 =    104, /* 103.83 */
    RCT_NOTE_A2  =    110, /* 110.00 */
    RCT_NOTE_AS2 =    117, /* 116.54 */
    RCT_NOTE_B2  =    123, /* 123.47 */
    RCT_NOTE_C3  =    131, /* 130.81 */
    RCT_NOTE_CS3 =    139, /* 138.59 */
    RCT_NOTE_D3  =    147, /* 146.83 */
    RCT_NOTE_DS3 =    156, /* 155.56 */
    RCT_NOTE_E3  =    165, /* 164.81 */
    RCT_NOTE_F3  =    175, /* 174.61 */
    RCT_NOTE_FS3 =    185, /* 185.00 */
    RCT_NOTE_G3  =    196, /* 196.00 */
#endif
    RCT_NOTE_GS3 =    208, /* 207.65 */
    RCT_NOTE_A3  =    220, /* 220.00 */
    RCT_NOTE_AS3 =    233, /* 233.08 */
    RCT_NOTE_B3  =    247, /* 246.94 */
    RCT_NOTE_C4  =    262, /* 261.63 */
    RCT_NOTE_CS4 =    277, /* 277.18 */
    RCT_NOTE_D4  =    294, /* 293.66 */
    RCT_NOTE_DS4 =    311, /* 311.13 */
    RCT_NOTE_E4  =    330, /* 329.63 */
    RCT_NOTE_F4  =    349, /* 349.23 */
    RCT_NOTE_FS4 =    370, /* 369.99 */
    RCT_NOTE_G4  =    392, /* 392.00 */
    RCT_NOTE_GS4 =    415, /* 415.30 */
    RCT_NOTE_A4  =    440, /* 440.00 */
    RCT_NOTE_AS4 =    466, /* 466.16 */
    RCT_NOTE_B4  =    494, /* 493.88 */
    RCT_NOTE_C5  =    523, /* 523.25 */
    RCT_NOTE_CS5 =    554, /* 554.37 */
    RCT_NOTE_D5  =    587, /* 587.33 */
    RCT_NOTE_DS5 =    622, /* 622.25 */
    RCT_NOTE_E5  =    659, /* 659.26 */
    RCT_NOTE_F5  =    698, /* 698.46 */
    RCT_NOTE_FS5 =    740, /* 739.99 */
    RCT_NOTE_G5  =    784, /* 783.99 */
    RCT_NOTE_GS5 =    831, /* 830.61 */
    RCT_NOTE_A5  =    880, /* 880.00 */
    RCT_NOTE_AS5 =    932, /* 932.33 */
    RCT_NOTE_B5  =    988, /* 987.77 */
    RCT_NOTE_C6  =   1047, /* 1046.50 */
    RCT_NOTE_CS6 =   1109, /* 1108.73 */
    RCT_NOTE_D6  =   1175, /* 1174.66 */
    RCT_NOTE_DS6 =   1245, /* 1244.51 */
    RCT_NOTE_E6  =   1319, /* 1318.51 */
    RCT_NOTE_F6  =   1397, /* 1396.91 */
    RCT_NOTE_FS6 =   1480, /* 1479.98 */
    RCT_NOTE_G6  =   1568, /* 1567.98 */
    RCT_NOTE_GS6 =   1661, /* 1661.22 */
    RCT_NOTE_A6  =   1760, /* 1760.00 */
    RCT_NOTE_AS6 =   1865, /* 1864.66 */
    RCT_NOTE_B6  =   1976, /* 1975.53 */
    RCT_NOTE_C7  =   2093, /* 2093.00 */
    RCT_NOTE_CS7 =   2217, /* 2217.46 */
    RCT_NOTE_D7  =   2349, /* 2349.32 */
    RCT_NOTE_DS7 =   2489, /* 2489.02 */
    RCT_NOTE_E7  =   2637, /* 2637.02 */
    RCT_NOTE_F7  =   2794, /* 2793.83 */
    RCT_NOTE_FS7 =   2960, /* 2959.96 */
    RCT_NOTE_G7  =   3136, /* 3135.96 */
    RCT_NOTE_GS7 =   3322, /* 3322.44 */
    RCT_NOTE_A7  =   3520, /* 3520.00 */
    RCT_NOTE_AS7 =   3729, /* 3729.31 */
    RCT_NOTE_B7  =   3951, /* 3951.07 */
    RCT_NOTE_C8  =   4186, /* 4186.01 */
    RCT_NOTE_CS8 =   4435, /* 4434.92 */
    RCT_NOTE_D8  =   4699, /* 4698.64 */
    RCT_NOTE_DS8 =   4978  /* 4978.03 */
}   rct_note_t;

typedef struct
{
    rct_brick_type_t    brick_type;
    
    // Brick-dependent stuff
    union
    {
	rct_rcx_t   rcx;
	rct_nxt_t   nxt;
	rct_pic_t   vex;
    };
}   rct_brick_t;

typedef struct
{
    rct_brick_t bricks[RCT_MAX_BRICKS];
    int         count;
}   rct_brick_list_t;

#include "rct_protos.h"

/** @} */

