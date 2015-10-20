#ifndef __MWIRE_H
#define __MWIRE_H

/* PIC18 microwire peripheral library header */

/* In the following SSPx stands for SSP, SSP1 and SSP2 */

/* SSPxSTAT REGISTER */
#define   MODE_01       1              // Setting for SPIx bus Mode 0,1
//CKE           0x00                   // SSPxSTAT register 
//CKP           0x00                   // SSPxCON1 register

#define   MODE_11       3              // Setting for SPIx bus Mode 1,1
//CKE           0x00                   // SSPxSTAT register
//CKP           0x10                   // SSPxCON1 register

/* SSPxCON1 REGISTER */
#define   SSPENB        0x20           // Enable serial port and configures SCK, SDO, SDI

#define   MWIRE_FOSC_4        0x10     // clock = Fosc/4
#define   MWIRE_FOSC_16       0x11     // clock = Fosc/16
#define   MWIRE_FOSC_64       0x12     // clock = Fosc/64
#define   MWIRE_FOSC_TMR2     0x13     // clock = TMR2 output/2

/* microwire interface definitions */
#if defined(__18F2455) || defined(__18F2550) || \
    defined(__18F4455) || defined(__18F4550)
#define   DO      PORTCbits.RC7        // microwire DO to 18Cxxx
#define   DI      PORTBbits.RB0        // microwire DI to 18Cxxx
#define   SCK     PORTBbits.RB1        // microwire clock to 18Cxxx
#else
#define   DO      PORTCbits.RC5        // microwire DO to 18Cxxx
#define   DI      PORTCbits.RC4        // microwire DI to 18Cxxx
#define   SCK     PORTCbits.RC3        // microwire clock to 18Cxxx
#endif

/* FUNCTION PROTOTYPES */

#define PARAM_SCLASS auto

/* These devices have two microwire modules */
#if defined(__18F64J15) || defined(__18F65J10) || defined(__18F65J15) || \
    defined(__18F66J10) || defined(__18F66J15) || defined(__18F67J10) || \
    defined(__18F84J15) || defined(__18F85J10) || defined(__18F85J15) || \
    defined(__18F86J10) || defined(__18F86J15) || defined(__18F87J10) || \
    defined(__18F6627)  || defined(__18F6722)  || \
    defined(__18F8627)  || defined(__18F8722)

/* ***** mwire1 ***** */

/* CloseMwire1
 * Disable SPI1 peripheral
 */
#define  CloseMwire1() SSP1CON1&=0xDF
#define  CloseMwire CloseMwire1

/* DataRdyMwire1
 * Is microwire1 device ready, write cycle complete?
 */
#define  DataRdyMwire1()    (PORTCbits.RC4)
#define  DataRdyMwire DataRdyMwire1

/* OpenMwire1
 * Open SPI1 module for microwire1 access
 */
void OpenMwire1( PARAM_SCLASS unsigned char sync_mode );
#define OpenMwire OpenMwire1

/* ReadMwire1
 * Read 1 byte from microwire1 device
 */
unsigned char ReadMwire1( PARAM_SCLASS unsigned char high_byte,
                          PARAM_SCLASS unsigned char low_byte );
#define ReadMwire ReadMwire1

/* getcMwire1
 * Read 1 byte from microwire1 device
 */
#define  getcMwire1  ReadMwire1
#define  getcMwire getcMwire1

/* WriteMwire1
 * Write single byte to microwire1 device
 */
unsigned char WriteMwire1( PARAM_SCLASS unsigned char data_out );
#define WriteMwire WriteMwire1

/* putcMwire1
 * Write single byte to microwire1 device
 */
#define  putcMwire1  WriteMwire1
#define  putcMwire putcMwire1

/* getsMwire1
 * Read string in from microwire1 device until null
 */
void getsMwire1( PARAM_SCLASS unsigned char *rdptr, PARAM_SCLASS unsigned char length );
#define getsMwire getsMwire1

/* ***** mwire2 ***** */

/* CloseMwire2
 * Disable SPI2 peripheral
 */
#define  CloseMwire2() SSP2CON1&=0xDF

/* DataRdyMwire2
 * Is microwire2 device ready, write cycle complete?
 */
#define  DataRdyMwire2()    (PORTCbits.RD5)

/* OpenMwire2
 * Open SPI2 module for microwire2 access
 */
void OpenMwire2( PARAM_SCLASS unsigned char sync_mode );

/* ReadMwire2
 * Read 1 byte from microwire2 device
 */
unsigned char ReadMwire2( PARAM_SCLASS unsigned char high_byte,
                          PARAM_SCLASS unsigned char low_byte );

/* getcMwire2
 * Read 1 byte from microwire2 device
 */
#define  getcMwire2  ReadMwire2

/* WriteMwire2
 * Write single byte to microwire2 device
 */
unsigned char WriteMwire2( PARAM_SCLASS unsigned char data_out );

/* putcMwire2
 * Write single byte to microwire2 device
 */
#define  putcMwire2  WriteMwire2

/* getsMwire2
 * Read string in from microwire2 device until null
 */
void getsMwire2( PARAM_SCLASS unsigned char *rdptr, PARAM_SCLASS unsigned char length );

#else

/* CloseMwire
 * Disable SPI peripheral
 */
#if defined(__18F2331) || defined(__18F2431) || \
    defined(__18F4331) || defined(__18F4431)
#define  CloseMwire() SSPCON&=0xDF
#else
#define  CloseMwire() SSPCON1&=0xDF
#endif

/* DataRdyMwire
 * Is microwire device ready, write cycle complete?
 */
#if defined(__18F2455) || defined(__18F2550) || \
    defined(__18F4455) || defined(__18F4550)
#define  DataRdyMwire()    (PORTBbits.RB0)
#else
#define  DataRdyMwire()    (PORTCbits.RC4)
#endif

/* OpenMwire
 * Open SPI module for microwire access
 */
void OpenMwire( PARAM_SCLASS unsigned char sync_mode );

/* ReadMwire
 * Read 1 byte from microwire device
 */
unsigned char ReadMwire( PARAM_SCLASS unsigned char high_byte,
                         PARAM_SCLASS unsigned char low_byte );

/* getcMwire
 * Read 1 byte from microwire device
 */
#define  getcMwire  ReadMwire

/* WriteMwire
 * Write single byte to microwire device
 */
unsigned char WriteMwire( PARAM_SCLASS unsigned char data_out );

/* putcMwire
 * Write single byte to microwire device
 */
#define  putcMwire  WriteMwire

/* getsMwire
 * Read string in from microwire device until null
 */
void getsMwire( PARAM_SCLASS unsigned char *rdptr, PARAM_SCLASS unsigned char length );
#endif
#endif
