#ifndef __SWI2C16_H
#define __SWI2C16_H

/* PIC18 software I2C interface header */

/*****   COMMON FUNCTION PROTOTYPES   *****/

void SWStopI2C( void );                // Generate bus stop condition
void SWStartI2C( void );               // Generate bus start condition
void SWRestartI2C( void );             // Generate bus restart condition
void SWStopI2C( void );                // Generate bus stop condition
void SWNotAckI2C( void );              // Generate bus Not Ack condition



// USER NEEDS TO DEFINE DATA AND CLOCK PINS. RESISTORS ARE REQUIRED ON 
// DATA AND CLOCK PINS. 

#if defined(__18F1220) || defined(__18F1320)
#define  DATA_LOW   TRISBbits.TRISB4 = 0; // define macro for data pin output
#define  DATA_HI    TRISBbits.TRISB4 = 1; // define macro for data pin input
#define  DATA_LAT   LATBbits.LATB4        // define macro for data pin latch
#define  DATA_PIN   PORTBbits.RB4         // define macro for data pin

#define  CLOCK_LOW  TRISBbits.TRISB3 = 0; // define macro for clock pin output
#define  CLOCK_HI   TRISBbits.TRISB3 = 1; // define macro for clock pin input
#define  SCLK_LAT   LATBbits.LATB3        // define macro for clock pin latch
#define  SCLK_PIN   PORTBbits.RB3         // define macro for clock pin

#elif defined(__18F2455) || defined(__18F2550) || \
      defined(__18F4455) || defined(__18F4550)

#define  DATA_LOW   TRISBbits.TRISB0 = 0; // define macro for data pin output
#define  DATA_HI    TRISBbits.TRISB0 = 1; // define macro for data pin input
#define  DATA_LAT   LATBbits.LATB0        // define macro for data pin latch
#define  DATA_PIN   PORTBbits.RB0         // define macro for data pin

#define  CLOCK_LOW  TRISBbits.TRISB1 = 0; // define macro for clock pin output
#define  CLOCK_HI   TRISBbits.TRISB1 = 1; // define macro for clock pin input
#define  SCLK_LAT   LATBbits.LATB1        // define macro for clock pin latch
#define  SCLK_PIN   PORTBbits.RB1         // define macro for clock pin

#else

#define  DATA_LOW   TRISCbits.TRISC4 = 0; // define macro for data pin output
#define  DATA_HI    TRISCbits.TRISC4 = 1; // define macro for data pin input
#define  DATA_LAT   LATCbits.LATC4        // define macro for data pin latch
#define  DATA_PIN   PORTCbits.RC4         // define macro for data pin

#define  CLOCK_LOW  TRISCbits.TRISC3 = 0; // define macro for clock pin output
#define  CLOCK_HI   TRISCbits.TRISC3 = 1; // define macro for clock pin input
#define  SCLK_LAT   LATCbits.LATC3        // define macro for clock pin latch
#define  SCLK_PIN   PORTCbits.RC3         // define macro for clock pin
#endif

/*****   FUNCTION PROTOTYPES FOR PIC18CXXX   *****/

unsigned char SWAckI2C( void );        // Read bus ACK condition
unsigned char Clock_test( void );
unsigned int  SWReadI2C( void );       // Read in single byte 
unsigned char SWWriteI2C( auto unsigned char data_out ); // Write out single byte
unsigned char SWGetsI2C( auto unsigned char *rdptr, auto unsigned char length );   // Read in string from I2C module
unsigned char SWPutsI2C( auto unsigned char *wrptr );    // Write string to I2C module

#define  SWPutcI2C    SWWriteI2C
#define  SWGetcI2C    SWReadI2C
#define  SWNotAckI2C  SWAckI2C

#endif
/*  end  swi2c16.h file */
