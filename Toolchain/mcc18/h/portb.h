#ifndef __PORTB_H
#define __PORTB_H

/* PIC18 PORTB library interface */

/* PORTB Config Bit Defines */
#define PORTB_CHANGE_INT_ON   0b11111111  /* PORTB interrupts on  */
#define PORTB_CHANGE_INT_OFF  0b01111111  /* PORTB interrupts off */
#define PORTB_PULLUPS_ON      0b11111110  /* PORTB pullups on     */
#define PORTB_PULLUPS_OFF     0b11111111  /* PORTB pullups off    */

#define RISING_EDGE_INT  0b11111111 /*Interrupt is set by a rising edge  */
#define FALLING_EDGE_INT 0b11111101 /*Interrupt is set by a falling edge */


/* PORTB Function Prototypes */
#define PARAM_SCLASS auto

/* OpenPORTB
 * Configure PORTB interrupts, resistors
 */
void OpenPORTB(PARAM_SCLASS unsigned char);

/* ClosePORTB
 * Disable PORTB interrupts, resistors
 */
#define ClosePORTB() INTCONbits.RBIE=0, DisablePullups()

/* EnablePullups
 * Enable PORTB resistors
 */
#define EnablePullups() INTCON2bits.RBPU=0

/* DisablePullups
 * Disable PORTB resistors
 */
#define DisablePullups() INTCON2bits.RBPU=1

/* OpenRB0INT
 * Enable RB0INT interrupts, falling edge/rising edge
 * PIC18Cxxx only
 */
void OpenRB0INT(PARAM_SCLASS unsigned char);

/* CloseRB0INT
 * Disable RB0INT interrupts, resistors
 * PIC18Cxxx only
 */
#define CloseRB0INT() INTCONbits.INT0IE=0

/* OpenRB1INT
 * Enable RB1INT interrupts, falling edge/rising edge
 * PIC18Cxxx only
 */
void OpenRB1INT(PARAM_SCLASS unsigned char);

/* CloseRB1INT
 * Disable RB1INT interrupts, resistors
 * PIC18Cxxx only
 */
#define CloseRB1INT() INTCON3bits.INT1IE=0

/* OpenRB2INT
 * Enable RB2INT interrupts, falling edge/rising edge
 * PIC18Cxxx only
 */
void OpenRB2INT(PARAM_SCLASS unsigned char);

/* CloseRB2INT
 * Disable RB2INT interrupts, resistors
 * PIC18Cxxx only
 */
#define CloseRB2INT() INTCON3bits.INT2IE=0

#endif
