#ifndef __SPI_H
#define __SPI_H

/* PIC18 SPI peripheral library header */


/* SSPSTAT REGISTER */

// Master SPI mode only

#define   SMPEND        0x80           // Input data sample at end of data out             
#define   SMPMID        0x00           // Input data sample at middle of data out

#define   MODE_00       0              // Setting for SPI bus Mode 0,0
//CKE           0x40                   // SSPSTAT register 
//CKP           0x00                   // SSPCON1 register 

#define   MODE_01       1              // Setting for SPI bus Mode 0,1
//CKE           0x00                   // SSPSTAT register 
//CKP           0x00                   // SSPCON1 register

#define   MODE_10       2              // Setting for SPI bus Mode 1,0
//CKE           0x40                   // SSPSTAT register
//CKP           0x10                   // SSPCON1 register

#define   MODE_11       3              // Setting for SPI bus Mode 1,1
//CKE           0x00                   // SSPSTAT register
//CKP           0x10                   // SSPCON1 register

/* SSPCON1 REGISTER */
#define   SSPENB        0x20           // Enable serial port and configures SCK, SDO, SDI

#define   SPI_FOSC_4    0              // SPI Master mode, clock = Fosc/4
#define   SPI_FOSC_16   1              // SPI Master mode, clock = Fosc/16
#define   SPI_FOSC_64   2              // SPI Master mode, clock = Fosc/64
#define   SPI_FOSC_TMR2 3              // SPI Master mode, clock = TMR2 output/2
#define   SLV_SSON      4              // SPI Slave mode, /SS pin control enabled
#define   SLV_SSOFF     5              // SPI Slave mode, /SS pin control disabled


/*  25Cxxx EEPROM instruction set */
#define   SPI_WREN          6              // write enable latch
#define   SPI_WRDI          4              // reset the write enable latch
#define   SPI_RDSR          5              // read status register
#define   SPI_WRSR          1              // write status register
#define   SPI_READ          3              // read data from memory
#define   SPI_WRITE         2              // write data to memory

/*  Bits within status register of 25Cxxx */
#define   WIP           0              // write in progress status
#define   WEL           1              // write enable latch status
#define   BP0           2              // block protection bit status
#define   BP1           3              // block protection bit status


/* FUNCTION PROTOTYPES */

#define PARAM_SCLASS auto


/* These devices have two SPI modules */
#if defined(__18F64J15) || defined(__18F65J10) || defined(__18F65J15) || \
    defined(__18F66J10) || defined(__18F66J15) || defined(__18F67J10) || \
    defined(__18F84J15) || defined(__18F85J10) || defined(__18F85J15) || \
    defined(__18F86J10) || defined(__18F86J15) || defined(__18F87J10) || \
    defined(__18F6627)  || defined(__18F6722)  || \
    defined(__18F8627)  || defined(__18F8722) 

/* ***** SPI1 ***** */

/* CloseSPI1
 * Disable SPI1 module
 */
#define  CloseSPI1()      (SSP1CON1 &=0xDF)
#define  CloseSPI CloseSPI1

/* DataRdySPI1
 * Test if SSP1BUF register is full
 */
#define  DataRdySPI1()    (SSP1STATbits.BF)
#define DataRdySPI DataRdySPI1

/* ReadSPI1
 * Read byte from SSP1BUF register
 */
unsigned char ReadSPI1( void );
#define ReadSPI ReadSPI1

/* getcSPI1
 * Read byte from SSP1BUF register
 */
#define  getcSPI1  ReadSPI1
#define getcSPI getcSPI1

/* OpenSPI1
 */
void OpenSPI1( PARAM_SCLASS unsigned char sync_mode,
               PARAM_SCLASS unsigned char bus_mode,
               PARAM_SCLASS unsigned char smp_phase );
#define OpenSPI OpenSPI1

/* WriteSPI1
 * Write byte to SSP1BUF register
 */
unsigned char WriteSPI1( PARAM_SCLASS unsigned char data_out );
#define WriteSPI WriteSPI1

/* putcSPI1
 * Write byte to SSP1BUF register
 */
#define  putcSPI1  WriteSPI1
#define  putcSPI putcSPI1

/* getsSPI1
 * Write string to SSP1BUF
 */
void getsSPI1( PARAM_SCLASS unsigned char *rdptr, PARAM_SCLASS unsigned char length );
#define getsSPI getsSPI1

/* putsSPI1
 * Read string from SSP1BUF
 */
void putsSPI1( PARAM_SCLASS unsigned char *wrptr );
#define putsSPI putsSPI1

/* ***** SPI2 ***** */

/* CloseSPI2
 * Disable SPI2 module
 */
#define  CloseSPI2()      (SSP2CON1 &=0xDF)


/* DataRdySPI2
 * Test if SSP2BUF register is full
 */
#define  DataRdySPI2()    (SSP2STATbits.BF)

/* ReadSPI2
 * Read byte from SSP2BUF register
 */
unsigned char ReadSPI2( void );

/* getcSPI2
 * Read byte from SSP2BUF register
 */
#define  getcSPI2  ReadSPI2

/* OpenSPI2
 */
void OpenSPI2( PARAM_SCLASS unsigned char sync_mode,
               PARAM_SCLASS unsigned char bus_mode,
               PARAM_SCLASS unsigned char smp_phase );

/* WriteSPI2
 * Write byte to SSP2BUF register
 */
unsigned char WriteSPI2( PARAM_SCLASS unsigned char data_out );

/* putcSPI2
 * Write byte to SSP2BUF register
 */
#define  putcSPI2  WriteSPI2

/* getsSPI2
 * Write string to SSP2BUF
 */
void getsSPI2( PARAM_SCLASS unsigned char *rdptr, PARAM_SCLASS unsigned char length );

/* putsSPI2
 * Read string from SSP2BUF
 */
void putsSPI2( PARAM_SCLASS unsigned char *wrptr );

#else

/* ***** SPI ***** */

/* CloseSPI
 * Disable SPI module
 */
#define  CloseSPI()      (SSPCON1 &=0xDF)


/* DataRdySPI
 * Test if SSPBUF register is full
 */
#define  DataRdySPI()    (SSPSTATbits.BF)

/* ReadSPI
 * Read byte from SSPBUF register
 */
unsigned char ReadSPI( void );

/* getcSPI
 * Read byte from SSPBUF register
 */
#define  getcSPI  ReadSPI

/* OpenSPI
 */
void OpenSPI( PARAM_SCLASS unsigned char sync_mode,
              PARAM_SCLASS unsigned char bus_mode,
              PARAM_SCLASS unsigned char smp_phase );

/* WriteSPI
 * Write byte to SSPBUF register
 */
unsigned char WriteSPI( PARAM_SCLASS unsigned char data_out );

/* putcSPI
 * Write byte to SSPBUF register
 */
#define  putcSPI  WriteSPI

/* getsSPI
 * Write string to SSPBUF
 */
void getsSPI( PARAM_SCLASS unsigned char *rdptr, PARAM_SCLASS unsigned char length );


/* putsSPI
 * Read string from SSPBUF
 */
void putsSPI( PARAM_SCLASS unsigned char *wrptr );

#endif 

#endif  /* __SPI_H */

