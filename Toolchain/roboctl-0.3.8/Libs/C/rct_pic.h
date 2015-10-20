#ifndef _TERMIOS_H_
#include <termios.h>
#endif

#define PIC_CMD_MAX         255
#define PIC_RESPONSE_MAX    255
#define PIC_HEX_LINE_MAX    1024

#define PIC_WRITE_BLOCK_SIZE    8
#define PIC_ERASE_BLOCK_SIZE    64
/*
 *  The protocol supports 16-bit block counts according to shtylman.com, but if
 *  the len_low byte is 0 (e.g. 256 erase-blocks), the controller just goes to
 *  the IFI> prompt when you send the erase command packet.  Curiously, it seems
 *  to work fine for erase_blocks > 256.  The problem only occurs when erase_blocks
 *  is exactly 256.
 *  Hence, we erase flash in chunks of 128 or smaller, so that len_low is always
 *  > 0 and len_high is always 0.
 */
#define PIC_MAX_ERASE_BLOCKS    128

/* PIC special characters */
#define CHAR_SI     0x0F    /* Start transmission */
#define CHAR_EOT    0x04    /* End transmission */
#define CHAR_ESC    0x05    /* Escape next character (must precede CHAR_SI,
				CHAR_EOT and CHAR_ESC in payload */

#define PIC_BAUD_RATE   B115200

#ifndef MIN
#define MIN(x,y)    ((x) < (y) ? (x) : (y))
#endif

#ifndef MAX
#define MAX(x,y)    ((x) > (y) ? (x) : (y))
#endif

typedef enum
{
    PIC_GET_BOOTLOADER_VERSION= 0x00,
    PIC_READ_PROGRAM_MEM=       0x01,
    PIC_WRITE_PROGRAM_MEM=      0x02,
    PIC_RETURN_TO_USER_CODE=    0x08,
    PIC_ERASE_PROGRAM_MEM=      0x09
}   pic_cmd_t;

typedef struct
{
    unsigned long   address;
    int             len;
    char            code[PIC_HEX_LINE_MAX+1];
}   pic_line_t;

typedef struct
{
    int             fd;
    int             bootloader_major;
    int             bootloader_minor;
    char            *device;
    char            response[PIC_RESPONSE_MAX+1];
    struct termios  current_port_settings;
    struct termios  original_port_settings;
}   rct_pic_t;

#define PIC_IS_OPEN(p)  ((p)->fd != -1)

