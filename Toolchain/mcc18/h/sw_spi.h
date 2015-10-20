#ifndef __SW_SPI_H
#define __SW_SPI_H

/* PIC18 Software SPI library header
 *
 * To use the software spi routines, the user must define
 * the port and tris register for each of the CS, DIN, DOUT,
 * and SCK pins.  The SPI mode must also be defined (MODE0,
 * MODE1, MODE2, MODE3).
 *
 * Define the port and pin for each of the software SPI pins
 *  - Chip select pin CS must have a port and tris definition.
 *  - Data in pin DIN must have a port and tris definition.
 *  - Data out pin DOUT must have a port and tris definition.
 *  - Clock pin SCK must have a port and tris definition.
 */

#define SW_CS_PIN PORTBbits.RB2      // Chip Select
#define TRIS_SW_CS_PIN TRISBbits.TRISB2
#define SW_DIN_PIN PORTBbits.RB3     // Data in
#define TRIS_SW_DIN_PIN TRISBbits.TRISB3
#define SW_DOUT_PIN PORTBbits.RB7    // Data out
#define TRIS_SW_DOUT_PIN TRISBbits.TRISB7
#define SW_SCK_PIN PORTBbits.RB6     // Clock
#define TRIS_SW_SCK_PIN TRISBbits.TRISB6

// Define the mode for software SPI
// Refer to the SPI module for PIC17C756 for definitions of CKP and CKE
// Only one mode can be uncommented, otherwise the software will not work
#define MODE0  // CKP=0,CKE=0
//#define MODE1  // CKP=1,CKE=0
//#define MODE2  // CKP=0,CKE=1
//#define MODE3  // CKP=1,CKE=1

#define PARAM_SCLASS auto

/* OpenSWSPI  (SWOpenSPI)
 * Configure Software SPI I/O pins
 */
void OpenSWSPI(void);
#define SWOpenSPI OpenSWSPI

/* WriteSWSPI  (SWWriteSPI)
 * Write a data byte to the SPI, return byte read
 */
char WriteSWSPI(PARAM_SCLASS char);
#define SWWriteSPI WriteSWSPI

/* SetCSSWSPI  (SWSetCSSPI)
 * Sets the CS pin
 */
void SetCSSWSPI(void);   // Sets the CS pin
#define SWSetCSSPI SetCSSWSPI

/* ClearCSSWSPI  (SWClearCSSPI)
 * Clears the CS pin
 */
void ClearCSSWSPI(void);  // Clears the CS pin
#define SWClearCSSPI ClearCSSWSPI

/* putcSWSPI  (SWputcSPI)
 * putc is really write
 */
#define putcSWSPI WriteSWSPI
#define SWputcSPI putcSWSPI

#endif /* __SW_SPI_H */
