#ifndef __USART_H
#define __USART_H

/* PIC18 USART peripheral libraries. */

/* There are three library modules, corresponding to register names:
 *  USART1  (TXSTA1, RCSTA1, etc.)
 *  USART2  (TXSTA2, RCSTA2, etc.)
 *  USART  (TXSTA, RCSTA, etc.)
 *  Each module is defined only for those devices for which the register
 *  names are defined.
 */

 /* Corresponding to each module, there are several routines: 
  *
  * The 'open' routine takes two parameters:
  *   - 'config' is the bitwise 'and' of the appropriate configuration
  *     bit masks (defined below);
  *   - 'spbrg' is the baud rate.
  * The registers associated with the USART module are set according to these
  * parameters; then, the transmitter and receiver are enabled.
  *
  * The 'datardy' routine returns 1 if data has been received, 0 otherwise.
  *
  * The 'read' routine returns the received byte.  It also sets the framing
  * and overrun error status bits (FRAME_ERROR & OVERRUN_ERROR) if necessary; 
  * also, the status receive bit 8 (RX_NINE) is significant if 9-bit mode 
  * is enabled.
  * (See status bit structure definition below).
  *
  * The 'write' routine accepts the byte to transmit.  If 9-bit mode is 
  * enabled, the status trasmit bit 8 (TX_NINE) is also trasmitted.
  *
  * The 'gets' routine accepts a buffer and the buffer length in bytes as
  * parameters.  It fills the buffer with bytes as they are received; it will
  * wait for data if necessary in order to fill the entire buffer.
  *
  * The 'puts' routine accepts a null-terminated byte string.  All bytes
  * are transmitted, including the null character.  It will wait until the
  * USART is not busy in order to transmit all the bytes.
  *
  * The 'putrs' routine is identical to 'puts', except the byte string
  * resides in ROM.
  *
  * The 'close' routine disables the receiver, the transmitter, and the 
  * interrupts for both.
  *
  * The 'busy' routine returns 1 if the transmit shift register is not empty;
  * otherwise, it returns 0.
  *
  * For devices with enhanced USART capability, an additional 'baud'
  * routine is provided.  This routine takes a 'config' parameter, which
  * is a bitwise 'and' of the baud configuration bit masks (see below).
  * The BAUDCON (a.k.a. BAUDCTL) register is configured appropriately.
  */

/* Change this to near if building small memory model versions of
 * the libraries. */
#define MEM_MODEL far

/* storage class of library routine parameters; pre-built with auto;
 * do not change unless you rebuild the libraries with the new storage class */ 
#define PARAM_SCLASS auto

/* Configuration bit masks to be 'anded' together and passed as the 'config'
 * parameter to the 'open' routine. */
#define USART_TX_INT_ON   0b11111111  // Transmit interrupt on
#define USART_TX_INT_OFF  0b01111111  // Transmit interrupt off
#define USART_RX_INT_ON   0b11111111  // Receive interrupt on
#define USART_RX_INT_OFF  0b10111111  // Receive interrupt off
#define USART_BRGH_HIGH   0b11111111  // High baud rate
#define USART_BRGH_LOW    0b11101111  // Low baud rate
#define USART_CONT_RX     0b11111111  // Continuous reception
#define USART_SINGLE_RX   0b11110111  // Single reception
#define USART_SYNC_MASTER 0b11111111  // Synchrounous master mode
#define USART_SYNC_SLAVE  0b11111011  // Synchrounous slave mode
#define USART_NINE_BIT    0b11111111  // 9-bit data
#define USART_EIGHT_BIT   0b11111101  // 8-bit data
#define USART_SYNCH_MODE  0b11111111  // Synchronous mode
#define USART_ASYNCH_MODE 0b11111110  // Asynchronous mode

/* These devices have enhanced USARTs. */
#if defined(__18F6585) || defined(__18F6680) || \
    defined(__18F8585) || defined(__18F8680) || \
    defined(__18F2480) || defined(__18F2580) || \
    defined(__18F4480) || defined(__18F4580) || \
    defined(__18F2585) || defined(__18F2680) || \
    defined(__18F4585) || defined(__18F4680) || \
    defined(__18F64J15) || defined(__18F65J10) || defined(__18F65J15) || \
    defined(__18F66J10) || defined(__18F66J15) || defined(__18F67J10) || \
    defined(__18F84J15) || defined(__18F85J10) || defined(__18F85J15) || \
    defined(__18F86J10) || defined(__18F86J15) || defined(__18F87J10) || \
    defined(__18F1220) || defined(__18F1320) || \
    defined(__18F6525) || defined(__18F6621) || \
    defined(__18F8525) || defined(__18F8621) || \
    defined(__18F2515) || defined(__18F2525) || \
    defined(__18F2610) || defined(__18F2620) || \
    defined(__18F4515) || defined(__18F4525) || \
    defined(__18F4610) || defined(__18F4620) || \
    defined(__18F6310) || defined(__18F6390) || \
    defined(__18F6410) || defined(__18F6490) || \
    defined(__18F8310) || defined(__18F8390) || \
    defined(__18F8410) || defined(__18F8490) || \
    defined(__18F2455) || defined(__18F2550) || \
    defined(__18F4455) || defined(__18F4550) || \
    defined(__18F2510) || defined(__18F2520) || \
    defined(__18F4410) || defined(__18F4420) || \
    defined(__18F4510) || defined(__18F4520)
/* The baud configuration bit masks to be 'anded' together and passed to
 * the 'baud' routine. */
#define BAUD_IDLE_CLK_HIGH  0b11111111  // idle state for clock is a high level
#define BAUD_IDLE_CLK_LOW   0b11101111  // idle state for clock is a low level
#define BAUD_16_BIT_RATE    0b11111111  // 16-bit baud generation rate
#define BAUD_8_BIT_RATE     0b11110111  // 8-bit baud generation rate
#define BAUD_WAKEUP_ON      0b11111111  // RX pin monitored
#define BAUD_WAKEUP_OFF     0b11111101  // RX pin not monitored
#define BAUD_AUTO_ON        0b11111111  // auto baud rate measurement enabled
#define BAUD_AUTO_OFF       0b11111110  // auto baud rate measurement disabled
#endif


/* Only these devices have two USART modules: USART1 & USART2. */
#if defined(__18F6520) || defined(__18F6620) || defined(__18F6720) || \
    defined(__18F8520) || defined(__18F8620) || defined(__18F8720) || \
    defined(__18F64J15) || defined(__18F65J10) || defined(__18F65J15) || \
    defined(__18F66J10) || defined(__18F66J15) || defined(__18F67J10) || \
    defined(__18F84J15) || defined(__18F85J10) || defined(__18F85J15) || \
    defined(__18F86J10) || defined(__18F86J15) || defined(__18F87J10) || \
    defined(__18F6627) || defined(__18F6722) || \
    defined(__18F8627) || defined(__18F8722) || \
    defined(__18F6525) || defined(__18F6621) || \
    defined(__18F8525) || defined(__18F8621) || \
    defined(__18F6310) || defined(__18F6390) || \
    defined(__18F6410) || defined(__18F6490) || \
    defined(__18F8310) || defined(__18F8390) || \
    defined(__18F8410) || defined(__18F8490)


/* ***** USART1 ***** */

/* status bits */
union USART1
{
  unsigned char val;
  struct
  {
    unsigned RX_NINE:1;         // Receive Bit 8 if 9-bit mode is enabled
    unsigned TX_NINE:1;         // Transmit Bit 8 if 9-bit mode is enabled
    unsigned FRAME_ERROR:1;     // Framing Error for USART
    unsigned OVERRUN_ERROR:1;   // Overrun Error for USART
    unsigned fill:4;
  };
};


void Open1USART (PARAM_SCLASS unsigned char config, PARAM_SCLASS char spbrg);
#if defined(__18F6525) || defined(__18F6621) || \
    defined(__18F8525) || defined(__18F8621) || \
    defined(__18F64J15) || defined(__18F65J10) || defined(__18F65J15) || \
    defined(__18F66J10) || defined(__18F66J15) || defined(__18F67J10) || \
    defined(__18F84J15) || defined(__18F85J10) || defined(__18F85J15) || \
    defined(__18F86J10) || defined(__18F86J15) || defined(__18F87J10) || \
    defined(__18F6627) || defined(__18F6722) || \
    defined(__18F8627) || defined(__18F8722)

#define DataRdy1USART( ) (PIR1bits.RCIF)
#else
#define DataRdy1USART( ) (PIR1bits.RC1IF)
#endif
char Read1USART (void);
void Write1USART (PARAM_SCLASS char data);
void gets1USART (PARAM_SCLASS char *buffer, PARAM_SCLASS unsigned char len);
void puts1USART (PARAM_SCLASS char *data);
void putrs1USART (PARAM_SCLASS const MEM_MODEL rom char *data);
#define getc1USART Read1USART
#define putc1USART Write1USART
#define Close1USART( ) RCSTA1&=0b01001111,TXSTA1bits.TXEN=0,PIE1&=0b11001111
#define Busy1USART( )  (!TXSTA1bits.TRMT)
#if defined(__18F6525) || defined(__18F6621) || \
    defined(__18F8525) || defined(__18F8621) || \
    defined(__18F6310) || defined(__18F6390) || \
    defined(__18F6410) || defined(__18F6490) || \
    defined(__18F8310) || defined(__18F8390) || \
    defined(__18F8410) || defined(__18F8490) || \
    defined(__18F64J15) || defined(__18F65J10) || defined(__18F65J15) || \
    defined(__18F66J10) || defined(__18F66J15) || defined(__18F67J10) || \
    defined(__18F84J15) || defined(__18F85J10) || defined(__18F85J15) || \
    defined(__18F86J10) || defined(__18F86J15) || defined(__18F87J10) || \
    defined(__18F6627) || defined(__18F6722) || \
    defined(__18F8627) || defined(__18F8722)

void baud1USART (PARAM_SCLASS unsigned char baudconfig);
#endif


/* ***** USART2 ***** */

/* status bits */
union USART2
{
  unsigned char val;
  struct
  {
    unsigned RX_NINE:1;         // Receive Bit 8 if 9-bit mode is enabled
    unsigned TX_NINE:1;         // Transmit Bit 8 if 9-bit mode is enabled
    unsigned FRAME_ERROR:1;     // Framing Error for USART
    unsigned OVERRUN_ERROR:1;   // Overrun Error for USART
    unsigned fill:4;
  };
};

void Open2USART (PARAM_SCLASS unsigned char config, PARAM_SCLASS char spbrg);
#define DataRdy2USART( ) (PIR3bits.RC2IF)
char Read2USART (void);
void Write2USART (PARAM_SCLASS char data);
void gets2USART (PARAM_SCLASS char *buffer, PARAM_SCLASS unsigned char len);
void puts2USART (PARAM_SCLASS char *data);
void putrs2USART (PARAM_SCLASS const MEM_MODEL rom char *data);
#define getc2USART Read2USART
#define putc2USART Write2USART
#define Close2USART( ) RCSTA2&=0b01001111,TXSTA2bits.TXEN=0,PIE3&=0b11001111
#define Busy2USART( ) (!TXSTA2bits.TRMT)
#if defined(__18F6525) || defined(__18F6621) || \
    defined(__18F8525) || defined(__18F8621) || \
    defined(__18F64J15) || defined(__18F65J10) || defined(__18F65J15) || \
    defined(__18F66J10) || defined(__18F66J15) || defined(__18F67J10) || \
    defined(__18F84J15) || defined(__18F85J10) || defined(__18F85J15) || \
    defined(__18F86J10) || defined(__18F86J15) || defined(__18F87J10) || \
    defined(__18F6627) || defined(__18F6722) || \
    defined(__18F8627) || defined(__18F8722)
void baud2USART (PARAM_SCLASS unsigned char baudconfig);
#endif

#else


/* ***** USART (TXSTA, RCSTA, etc.) ***** */

/* status bits */
union USART
{
  unsigned char val;
  struct
  {
    unsigned RX_NINE:1;         // Receive Bit 8 if 9-bit mode is enabled
    unsigned TX_NINE:1;         // Transmit Bit 8 if 9-bit mode is enabled
    unsigned FRAME_ERROR:1;     // Framing Error for USART
    unsigned OVERRUN_ERROR:1;   // Overrun Error for USART
    unsigned fill:4;
  };
};

void OpenUSART (PARAM_SCLASS unsigned char config, PARAM_SCLASS unsigned spbrg);
#define DataRdyUSART( ) (PIR1bits.RCIF)
char ReadUSART (void);
void WriteUSART (PARAM_SCLASS char data);
void getsUSART (PARAM_SCLASS char *buffer, PARAM_SCLASS unsigned char len);
void putsUSART (PARAM_SCLASS char *data);
void putrsUSART (PARAM_SCLASS const MEM_MODEL rom char *data);
#define getcUSART ReadUSART
#define putcUSART WriteUSART
#define CloseUSART( ) RCSTA&=0b01001111,TXSTAbits.TXEN=0,PIE1&=0b11001111
#define BusyUSART( ) (!TXSTAbits.TRMT)
#if defined(__18F6585) || defined(__18F6680) || \
    defined(__18F8585) || defined(__18F8680) || \
    defined(__18F2480) || defined(__18F2580) || \
    defined(__18F4480) || defined(__18F4580) || \
    defined(__18F2585) || defined(__18F2680) || \
    defined(__18F4585) || defined(__18F4680) || \
    defined(__18F1220) || defined(__18F1320) || \
    defined(__18F2515) || defined(__18F2525) || \
    defined(__18F2610) || defined(__18F2620) || \
    defined(__18F4515) || defined(__18F4525) || \
    defined(__18F4610) || defined(__18F4620) || \
    defined(__18F2455) || defined(__18F2550) || \
    defined(__18F4455) || defined(__18F4550) || \
    defined(__18F2510) || defined(__18F2520) || \
    defined(__18F4510) || defined(__18F4520)
void baudUSART (PARAM_SCLASS unsigned char baudconfig);
#endif

#endif

#endif
