
#include "rct_nxt_output.h"

/*
 *  General NXT USB stuff
 */
 
#define USB_ID_VENDOR_LEGO  0x0694
#define USB_ID_PRODUCT_NXT  0x0002

#define USB_TIMEOUT         1000

#define USB_STATUS_SUCCESS  0x00

#define NXT_BUFF_LEN        100
#define NXT_RESPONSE_MAX    1024
#define NXT_NAME_LEN        15
#define NXT_FILENAME_MAX    19

typedef enum
{
    NXT_NOT_PRESENT = 1,
    NXT_CONFIGURATION_ERROR,
    NXT_IN_USE,
    NXT_WRITE_ERROR,
    NXT_READ_ERROR,
    NXT_SAMBA_PROTOCOL_ERROR,
    NXT_FILE_ERROR,
    NXT_INVALID_FIRMWARE
}   nxt_status_t;

typedef enum
{
    NXT_NO_CONNECTION,
    NXT_BLUETOOTH,
    NXT_USB
}   nxt_connection_t;

#define NXT_VENDOR(dev)     ((dev)->descriptor.idVendor)
#define NXT_PRODUCT(dev)    ((dev)->descriptor.idProduct)

/* USB parameters for the NXT brick */
#define NXT_USB_INTERFACE       0
#define NXT_USB_OUT_ENDPOINT    0x01
#define NXT_USB_IN_ENDPOINT     0x82

/* Command codes */
#define NXT_DIRECT_CMD      0x00
#define NXT_SYSTEM_CMD      0x01
#define NXT_RESPONSE        0x00
#define NXT_NO_RESPONSE     0x80

// NXT constants
#define NXT_NoResponseMask   0x80
#define NXT_CmdReply         0x02
#define NXT_DirectCmdNoReply (NXT_DirectCmd | NXT_NoResponseMask)
#define NXT_SystemCmdNoReply (NXT_SystemCmd | NXT_NoResponseMask)
#define NXT_MaxBytes         64
#define NXT_NameMaxLen       15

/* Direct commands */
#define NXT_DC_START_PROGRAM            0x00
#define NXT_DC_STOP_PROGRAM             0x01
#define NXT_DC_PLAY_SOUND_FILE          0x02
#define NXT_DC_PLAY_TONE                0x03
#define NXT_DC_SET_OUTPUT_STATE         0x04
#define NXT_DC_SET_INPUT_MODE           0x05
#define NXT_DC_GET_OUTPUT_STATE         0x06
#define NXT_DC_GET_INPUT_VALUES         0x07
#define NXT_DC_RESET_INPUT_SCALED_VALUE 0x08
#define NXT_DC_MESSAGE_WRITE            0x09
#define NXT_DC_RESET_MOTOR_POSITION     0x0A
#define NXT_DC_BATTERY_LEVEL            0x0B
#define NXT_DC_STOP_SOUND_PLAYBACK      0x0C
#define NXT_DC_KEEP_ALIVE               0x0D
#define NXT_DC_LS_GET_STATUS            0x0E
#define NXT_DC_LS_WRITE                 0x0F
#define NXT_DC_LS_READ                  0x10
#define NXT_DC_GET_CURRENT_PROGRAM_NAME 0x11
#define NXT_DC_GET_BUTTON_STATE         0x12
#define NXT_DC_MESSAGE_READ             0x13

/* System commands */
#define NXT_SC_OPEN_READ            0x80
#define NXT_SC_OPEN_WRITE           0x81
#define NXT_SC_READ                 0x82
#define NXT_SC_WRITE                0x83
#define NXT_SC_CLOSE                0x84
#define NXT_SC_DELETE               0x85
#define NXT_SC_FIND_FIRST           0x86
#define NXT_SC_FIND_NEXT            0x87
#define NXT_SC_GET_VERSIONS         0x88
#define NXT_SC_OPEN_WRITE_LINEAR    0x89
#define NXT_SC_OPEN_READ_LINEAR     0x8A
#define NXT_SC_OPEN_WRITE_DATA      0x8B
#define NXT_SC_OPEN_APPEND_DATA     0x8C
#define NXT_SC_UNKNOWN1             0x8D
#define NXT_SC_UNKNOWN2             0x8E
#define NXT_SC_UNKNOWN3             0x8F
#define NXT_SC_FIND_FIRST_MODULE    0x90
#define NXT_SC_FIND_NEXT_MODULE     0x91
#define NXT_SC_CLOSE_MODULE_HANDLE  0x92
#define NXT_SC_UNKNOWN4             0x93
#define NXT_SC_IO_MAP_READ          0x94
#define NXT_SC_IO_MAP_WRITE         0x95
#define NXT_SC_UNKNOWN5             0x96
#define NXT_SC_BOOT_COMMAND         0x97
#define NXT_SC_SET_BRICK_NAME       0x98
#define NXT_SC_UNKNOWN6             0x99
#define NXT_SC_GET_BT_ADDRESS       0x9A
#define NXT_SC_GET_DEVICE_INFO      0x9B
#define NXT_SC_UNKNOWN7             0x9C
#define NXT_SC_UNKNOWN8             0x9D
#define NXT_SC_UNKNOWN9             0x9E
#define NXT_SC_UNKNOWN10            0x9F
#define NXT_SC_DELETE_USER_FLASH    0xA0
#define NXT_SC_POLL_COMMAND_LEN     0xA1
#define NXT_SC_POLL_COMMAND         0xA2
#define NXT_SC_RENAME_FILE          0xA3
#define NXT_SC_BT_FACTORY_RESET     0xA4

// NXT status codes
#define NXT_STATUS_SUCCESS              0x00
#define NXT_STATUS_NO_MORE_HANDLE       0x81
#define NXT_STATUS_NO_SPACE             0x82
#define NXT_STATUS_NO_MORE_FILES        0x83
#define NXT_STATUS_EOF_EXPECTED         0x84
#define NXT_STATUS_EOF                  0x85
#define NXT_STATUS_NOT_LINEAR_FILE      0x86
#define NXT_STATUS_FILE_NOT_FOUND       0x87
#define NXT_STATUS_HANDLE_CLOSED        0x88
#define NXT_STATUS_NO_LINEAR_SPACE      0x89
#define NXT_STATUS_UNDEFINED_ERROR      0x8a
#define NXT_STATUS_FILE_IS_BUSY         0x8b
#define NXT_STATUS_NO_WRITE_BUFS        0x8c
#define NXT_STATUS_APPEND_NOT_POSS      0x8d
#define NXT_STATUS_FILE_IS_FULL         0x8e
#define NXT_STATUS_FILE_EXISTS          0x8f
#define NXT_STATUS_MODULE_NOT_FOUND     0x90
#define NXT_STATUS_OUT_OF_BOUNDARY      0x91
#define NXT_STATUS_ILLEGAL_FILENAME     0x92
#define NXT_STATUS_ILLEGAL_HANDLE       0x93

/* NXT parameters */
typedef struct
{
    char                    name[NXT_NAME_LEN+1];
    unsigned long           free_flash;
    int                     is_in_reset_mode;
    unsigned int            firmware_major;
    unsigned int            firmware_minor;
    unsigned int            protocol_major;
    unsigned int            protocol_minor;
    unsigned int            battery_level;
    unsigned int            response_mask;  /* 0x00 = respond, 0x80 = no */

    /* Used only for USB connections */
    unsigned int            usb_bus;
    unsigned int            usb_dev_num;
    struct usb_device       *usb_dev;
    struct usb_dev_handle   *usb_handle;

    /* Used only for bluetooth connections */
    //char                    *bluetooth_passcode;
    //char                    *bluetooth_pin;
    int                     bluetooth_fd;
    unsigned char           bluetooth_address[7];
    unsigned long           bluetooth_signal_strength;
    
    nxt_output_state_t      port[3];
}   rct_nxt_t;


/* Get and set macros */
#define NXT_SET_USB_DEV(n,d)        ((n)->usb_dev = (d))
#define NXT_SET_BLUETOOTH_DEV(n,a,b,c,d,e,f) \
	(snprintf((n)->bluetooth_address,7,"%c%c%c%c%c%c", \
	(a),(b),(c),(d),(e),(f)))

#define NXT_BLUETOOTH_IS_OPEN(nxt)  ((nxt)->bluetooth_fd != -1)
#define NXT_USB_IS_OPEN(nxt)        ((nxt)->usb_handle != NULL)
#define NXT_IS_OPEN(nxt) (\
	    NXT_BLUETOOTH_IS_OPEN((nxt)) || NXT_USB_IS_OPEN((nxt)) \
	    )

