#ifndef __CAPTURE_H
#define __CAPTURE_H

/* PIC18 capture peripheral routines. */

/* There are five library modules, corresponding to register names:
 *  CAPTURE1  (CCPCON1)
 *  CAPTURE2  (CCPCON2)
 *  CAPTURE3  (CCPCON3)
 *  CAPTURE4  (CCPCON4)
 *  CAPTURE5  (CCPCON5)
 *  ECAPTURE1  (ECCPCON1)
 *  Each module is defined only for those devices for which the register
 *  name is defined.
 *  Note that the 'E' prefix indicates PWM enhanced capability (in ECCPCON1,
 *  for example); however, the absence of the 'E' prefix does not imply
 *  lack of enhanced capability.  The usage of the 'E' prefix is indicative 
 *  of the register naming convention in the datasheets, not the
 *  functionality.  Note also that these modules deal with capture
 *  functionality only; for PWM functionality, see the header 'pwm.h'.
 */

 /* For each module, there are three routines: an 'open' routine,
  * a 'read' routine, and a 'close' routine.  The 'open' routine
  * configures the edge for capture as well as optionally enables the
  * interrupt.  The 'read' routine returns the 16-bit captured
  * value, and sets the overflow status bit if required.  The 'close'
  * routine disables the module, including the interrupt.
  */

/* For each module, a bit in this byte holds the overflow status.
 * Following a read, the bit will be set to indicate overflow. */
extern union capstatus
{
  struct
  {
    unsigned Cap1OVF:1; /* CAPTURE1 */
#if !defined(__18F248)  && !defined(__18F258)  && \
    !defined(__18F448)  && !defined(__18F458)  && \
    !defined(__18F2480) && !defined(__18F2580) && \
    !defined(__18F4480) && !defined(__18F4580) && \
    !defined(__18F2585) && !defined(__18F2680) && \
    !defined(__18F4585) && !defined(__18F4680) && \
    !defined(__18F1220) && !defined(__18F1320)
    unsigned Cap2OVF:1; /* CAPTURE2 */
#endif
#if defined(__18F6520) || defined(__18F6620) || defined(__18F6720) || \
    defined(__18F8520) || defined(__18F8620) || defined(__18F8720) || \
    defined(__18F64J15) || defined(__18F65J10) || defined(__18F65J15) || \
    defined(__18F66J10) || defined(__18F66J15) || defined(__18F67J10) || \
    defined(__18F84J15) || defined(__18F85J10) || defined(__18F85J15) || \
    defined(__18F86J10) || defined(__18F86J15) || defined(__18F87J10) || \
    defined(__18F6525) || defined(__18F6621) || \
    defined(__18F8525) || defined(__18F8621) || \
    defined(__18F6310) || defined(__18F8310) || \
    defined(__18F6410) || defined(__18F8410) || \
    defined(__18F6627) || defined(__18F6722) || \
    defined(__18F8627) || defined(__18F8722)
    unsigned Cap3OVF:1; /* CAPTURE3 */
#endif
#if defined(__18F6520) || defined(__18F6620) || defined(__18F6720) || \
    defined(__18F8520) || defined(__18F8620) || defined(__18F8720) || \
    defined(__18F64J15) || defined(__18F65J10) || defined(__18F65J15) || \
    defined(__18F66J10) || defined(__18F66J15) || defined(__18F67J10) || \
    defined(__18F84J15) || defined(__18F85J10) || defined(__18F85J15) || \
    defined(__18F86J10) || defined(__18F86J15) || defined(__18F87J10) || \
    defined(__18F6525) || defined(__18F6621) || \
    defined(__18F8525) || defined(__18F8621) || \
    defined(__18F6627) || defined(__18F6722) || \
    defined(__18F8627) || defined(__18F8722)
    unsigned Cap4OVF:1; /* CAPTURE4 */
#endif
#if defined(__18F6520) || defined(__18F6620) || defined(__18F6720) || \
    defined(__18F8520) || defined(__18F8620) || defined(__18F8720) || \
    defined(__18F64J15) || defined(__18F65J10) || defined(__18F65J15) || \
    defined(__18F66J10) || defined(__18F66J15) || defined(__18F67J10) || \
    defined(__18F84J15) || defined(__18F85J10) || defined(__18F85J15) || \
    defined(__18F86J10) || defined(__18F86J15) || defined(__18F87J10) || \
    defined(__18F6525) || defined(__18F6621) || \
    defined(__18F8525) || defined(__18F8621) || \
    defined(__18F6627) || defined(__18F6722) || \
    defined(__18F8627) || defined(__18F8722)
    unsigned Cap5OVF:1; /* CAPTURE5 */
#endif
#if defined(__18F448)  || defined(__18F458) || \
    defined(__18F4480) || defined(__18F4580) || \
    defined(__18F4585) || defined(__18F4680) 
    unsigned ECap1OVF:1; /* ECAPTURE1 */
#endif
  };
  unsigned :8;

} CapStatus;

/* used to hold the 16-bit captured value */
union CapResult
{
 unsigned int lc;
 char bc[2];
};


/* storage class of library routine parameters; pre-built with auto;
 * do not change unless you rebuild the libraries with the new storage class */ 
#define PARAM_SCLASS auto


/* Interrupt bit mask to be 'anded' with the edge mask and passed as
 * the 'config' parameter to the 'open' routines. */
#define CAPTURE_INT_ON   0b11111111  /* enable interrupt */
#define CAPTURE_INT_OFF  0b01111111  /* disable nterrupt */


/* ***** CAPTURE1 (CCP1CON) ***** */

/* CAPTURE1 edge mask -- to be 'anded' with the interrupt mask and
 * passed as the 'config' parameter. */
#define C1_EVERY_FALL_EDGE     0b10000100  /* Every falling edge      */
#define C1_EVERY_RISE_EDGE     0b10000101  /* Every rising edge       */
#define C1_EVERY_4_RISE_EDGE   0b10000110  /* Every 4th rising edge   */
#define C1_EVERY_16_RISE_EDGE  0b10000111  /* Every 16th rising edge  */

void OpenCapture1 (PARAM_SCLASS unsigned char config);
unsigned int ReadCapture1 (void);
void CloseCapture1 (void);


/* ***** CAPTURE2 (CCP2CON) ***** */

#if !defined(__18F248)  && !defined(__18F258)  && \
    !defined(__18F448)  && !defined(__18F458)  && \
    !defined(__18F2480) && !defined(__18F2580) && \
    !defined(__18F4480) && !defined(__18F4580) && \
    !defined(__18F2585) && !defined(__18F2680) && \
    !defined(__18F4585) && !defined(__18F4680) && \
    !defined(__18F1220) && !defined(__18F1320)

/* CAPTURE2 edge mask -- to be 'anded' with the interrupt mask and
 * passed as the 'config' parameter. */
#define C2_EVERY_FALL_EDGE     0b10000100  /* Every falling edge     */
#define C2_EVERY_RISE_EDGE     0b10000101  /* Every rising edge      */
#define C2_EVERY_4_RISE_EDGE   0b10000110  /* Every 4th rising edge  */
#define C2_EVERY_16_RISE_EDGE  0b10000111  /* Every 16th rising edge */

void OpenCapture2 (PARAM_SCLASS unsigned char config);
unsigned int ReadCapture2 (void);
void CloseCapture2 (void);

#endif


/* ***** CAPTURE3 (CCP3CON) ***** */

#if defined(__18F6520) || defined(__18F6620) || defined(__18F6720) || \
    defined(__18F8520) || defined(__18F8620) || defined(__18F8720) || \
    defined(__18F64J15) || defined(__18F65J10) || defined(__18F65J15) || \
    defined(__18F66J10) || defined(__18F66J15) || defined(__18F67J10) || \
    defined(__18F84J15) || defined(__18F85J10) || defined(__18F85J15) || \
    defined(__18F86J10) || defined(__18F86J15) || defined(__18F87J10) || \
    defined(__18F6525) || defined(__18F6621) || \
    defined(__18F8525) || defined(__18F8621) || \
    defined(__18F6310) || defined(__18F8310) || \
    defined(__18F6410) || defined(__18F8410) || \
    defined(__18F6627) || defined(__18F6722) || \
    defined(__18F8627) || defined(__18F8722)

/* CAPTURE3 edge mask -- to be 'anded' with the interrupt mask and
 * passed as the 'config' parameter. */
#define C3_EVERY_FALL_EDGE     0b10000100  /* Every falling edge     */
#define C3_EVERY_RISE_EDGE     0b10000101  /* Every rising edge      */
#define C3_EVERY_4_RISE_EDGE   0b10000110  /* Every 4th rising edge  */
#define C3_EVERY_16_RISE_EDGE  0b10000111  /* Every 16th rising edge */

void OpenCapture3 (PARAM_SCLASS unsigned char config);
unsigned int ReadCapture3 (void);
void CloseCapture3 (void);

#endif


/* ***** CAPTURE4 (CCP4CON) ***** */

#if defined(__18F6520) || defined(__18F6620) || defined(__18F6720) || \
    defined(__18F8520) || defined(__18F8620) || defined(__18F8720) || \
    defined(__18F64J15) || defined(__18F65J10) || defined(__18F65J15) || \
    defined(__18F66J10) || defined(__18F66J15) || defined(__18F67J10) || \
    defined(__18F84J15) || defined(__18F85J10) || defined(__18F85J15) || \
    defined(__18F86J10) || defined(__18F86J15) || defined(__18F87J10) || \
    defined(__18F6525) || defined(__18F6621) || \
    defined(__18F8525) || defined(__18F8621) || \
    defined(__18F6627) || defined(__18F6722) || \
    defined(__18F8627) || defined(__18F8722)

/* CAPTURE4 edge mask -- to be 'anded' with the interrupt mask and
 * passed as the 'config' parameter. */
#define C4_EVERY_FALL_EDGE     0b10000100  /* Every falling edge     */
#define C4_EVERY_RISE_EDGE     0b10000101  /* Every rising edge      */
#define C4_EVERY_4_RISE_EDGE   0b10000110  /* Every 4th rising edge  */
#define C4_EVERY_16_RISE_EDGE  0b10000111  /* Every 16th rising edge */

void OpenCapture4 (PARAM_SCLASS unsigned char config);
unsigned int ReadCapture4 (void);
void CloseCapture4 (void);

#endif


/* ***** CAPTURE5 (CCP5CON) ***** */

#if defined(__18F6520) || defined(__18F6620) || defined(__18F6720) || \
    defined(__18F8520) || defined(__18F8620) || defined(__18F8720) || \
    defined(__18F64J15) || defined(__18F65J10) || defined(__18F65J15) || \
    defined(__18F66J10) || defined(__18F66J15) || defined(__18F67J10) || \
    defined(__18F84J15) || defined(__18F85J10) || defined(__18F85J15) || \
    defined(__18F86J10) || defined(__18F86J15) || defined(__18F87J10) || \
    defined(__18F6525) || defined(__18F6621) || \
    defined(__18F8525) || defined(__18F8621) || \
    defined(__18F6627) || defined(__18F6722) || \
    defined(__18F8627) || defined(__18F8722)

/* CAPTURE5 edge mask -- to be 'anded' with the interrupt mask and
 * passed as the 'config' parameter. */
#define C5_EVERY_FALL_EDGE     0b10000100  /* Every falling edge     */
#define C5_EVERY_RISE_EDGE     0b10000101  /* Every rising edge      */
#define C5_EVERY_4_RISE_EDGE   0b10000110  /* Every 4th rising edge  */
#define C5_EVERY_16_RISE_EDGE  0b10000111  /* Every 16th rising edge */

void OpenCapture5 (PARAM_SCLASS unsigned char config);
unsigned int ReadCapture5 (void);
void CloseCapture5 (void);

#endif


/* ***** ECAPTURE1 (ECCP1CON) ***** */

#if defined(__18F448)  || defined(__18F458) || \
    defined(__18F4480) || defined(__18F4580) || \
    defined(__18F4585) || defined(__18F4680) 

/* ECAPTURE1 edge mask -- to be 'anded' with the interrupt mask and
 * passed as the 'config' parameter. */
#define EC1_EVERY_FALL_EDGE     0b10000100  /* Every falling edge     */
#define EC1_EVERY_RISE_EDGE     0b10000101  /* Every rising edge      */
#define EC1_EVERY_4_RISE_EDGE   0b10000110  /* Every 4th rising edge  */
#define EC1_EVERY_16_RISE_EDGE  0b10000111  /* Every 16th rising edge */

void OpenECapture1 (PARAM_SCLASS unsigned char config);
unsigned int ReadECapture1 (void);
void CloseECapture1 (void);

#endif

#endif
