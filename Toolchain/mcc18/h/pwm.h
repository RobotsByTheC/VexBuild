#ifndef __PWM_H
#define __PWM_H

/* PIC18 PWM peripheral routines. */

/* There are five library modules, corresponding to register names:
 *  PWM1  (CCPCON1)
 *  PWM2  (CCPCON2)
 *  PWM3  (CCPCON3)
 *  PWM4  (CCPCON4)
 *  PWM5  (CCPCON5)
 *  EPWM1  (ECCPCON1)
 *  Each module is defined only for those devices for which the register
 *  name is defined.
 *  Note that the 'E' prefix indicates enhanced CCP capability (in ECCPCON1,
 *  for example); however, the absence of the 'E' prefix does not imply
 *  lack of enhanced capability.  The usage of the 'E' prefix is indicative 
 *  of the register naming convention in the datasheets, not the
 *  functionality.
 */

 /* For each module, there are three routines: an 'open' routine,
  * a 'SetDC' routine, and a 'close' routine.
  *
  * The 'open' routine
  *   - sets the PWM mode as single output;
  *   - sets the PWM period, as specified by the routine's parameter;
  *   - configures the CCPx port pin for output;
  *   - begins the PWM operation.
  *
  * The 'SetDC' routine sets the duty cycle, as specified by the routine's
  * parameter.
  *
  * The 'close' routine turns off PWM operation and configures the CCPx port
  * pin to input.
  *
  * For devices with enhanced CCP capability, an additional 'SetOutput'
  * routine is provided.  This routine takes two parameters:
  *   - 'output_config' is the output configuration:
  *         single output 
  *         full bridge output foward
  *         half bridge output
  *         full bridge output reverse
  *   - 'pwm_mode' is the PWM mode: 
  *         PxA,PxC active high; PxB,PxD active high
  *         PxA,PxC active high; PxB,PxD active low 
  *         PxA,PxC active low; PxB,PxD active high
  *         PxA,PxC active low; PxB,PxD active low 
  * These parameters are represented by symbolic constants defined below.
  * The 'SetOutput' routines set the output configuration and PWM mode;
  * they also configure the port pins corresponding to the output
  * configuration for output.  Note that unlike the 'open' routines, these
  * routines do not set the period nor begin the PWM operation.
  */

/* used to hold the 10-bit duty cycle */
union PWMDC
{
    unsigned int lpwm;
    char bpwm[2];
};


/* storage class of library routine parameters; pre-built with auto;
 * do not change unless you rebuild the libraries with the new storage class */ 
#define PARAM_SCLASS auto

/* ***** Enhanced CCP ***** */

/* These devices have enhanced CCP capability.  For PWM, this means up to
 * four outputs.  The 'SetOutput' routines are defined for those modules
 * with this enhanced functionality.  These routines set the output
 * configuration and the PWM mode, as well as configure the appropriate
 * port pins for output.  Note that the port pins configured are those
 * corresponding to microcontroller mode.
 */
#if defined(__18F448)  || defined(__18F458)  || \
    defined(__18F6585) || defined(__18F6680) || \
    defined(__18F8585) || defined(__18F8680) || \
    defined(__18F4480) || defined(__18F4580) || \
    defined(__18F4585) || defined(__18F4680) || \
    defined(__18F1220) || defined(__18F1320) || \
    defined(__18F4220) || defined(__18F4320) || \
    defined(__18F6525) || defined(__18F6621) || \
    defined(__18F8525) || defined(__18F8621) || \
    defined(__18F4515) || defined(__18F4525) || \
    defined(__18F4610) || defined(__18F4620) || \
    defined(__18F4455) || defined(__18F4550) || \
    defined(__18F4410) || defined(__18F4420) || \
    defined(__18F4510) || defined(__18F4520) || \
    defined(__18F64J15) || defined(__18F65J10) || defined(__18F65J15) || \
    defined(__18F66J10) || defined(__18F66J15) || defined(__18F67J10) || \
    defined(__18F84J15) || defined(__18F85J10) || defined(__18F85J15) || \
    defined(__18F86J10) || defined(__18F86J15) || defined(__18F87J10) || \
    defined(__18F6627) || defined(__18F6722) || \
    defined(__18F8627) || defined(__18F8722)

/* The output configuration is passed as the first parameter to the
 * 'SetOutput' routines. */
#define SINGLE_OUT     0b00111111 /* single output */
#define FULL_OUT_FWD   0b01111111 /* full-bridge output forward */
#define HALF_OUT       0b10111111 /* half-bridge output */
#define FULL_OUT_REV   0b11111111 /* full-bridge output reverse */
#define IS_DUAL_PWM(config) ((config) == HALF_OUT)
#define IS_QUAD_PWM(config) ((config) == FULL_OUT_FWD || \
                             (config) == FULL_OUT_REV)
/* The PWM mode is passed as the second parameter to the
 * 'SetOutput' routines. */
#define PWM_MODE_1     0b11111100 /* PxA,PxC active high, PxB,PxD active high */
#define PWM_MODE_2     0b11111101 /* PxA,PxC active high, PxB,PxD active low */
#define PWM_MODE_3     0b11111110 /* PxA,PxC active low, PxB,PxD active high */
#define PWM_MODE_4     0b11111111 /* PxA,PxC active low, PxB,PxD active low */

#endif


/* ***** PWM1 (CCP1CON) ***** */

void OpenPWM1 (PARAM_SCLASS char period);
void SetDCPWM1 (PARAM_SCLASS unsigned int duty_cycle);
#if defined(__18F6585) || defined(__18F6680) || \
    defined(__18F8585) || defined(__18F8680) || \
    defined(__18F1220) || defined(__18F1320) || \
    defined(__18F4220) || defined(__18F4320) || \
    defined(__18F6525) || defined(__18F6621) || \
    defined(__18F8525) || defined(__18F8621) || \
    defined(__18F4515) || defined(__18F4525) || \
    defined(__18F4610) || defined(__18F4620) || \
    defined(__18F4455) || defined(__18F4550) || \
    defined(__18F4510) || defined(__18F4520) || \
    defined(__18F64J15) || defined(__18F65J10) || defined(__18F65J15) || \
    defined(__18F66J10) || defined(__18F66J15) || defined(__18F67J10) || \
    defined(__18F84J15) || defined(__18F85J10) || defined(__18F85J15) || \
    defined(__18F86J10) || defined(__18F86J15) || defined(__18F87J10) || \
    defined(__18F6627) || defined(__18F6722) || \
    defined(__18F8627) || defined(__18F8722)

void SetOutputPWM1 (PARAM_SCLASS unsigned char output_config, 
                    PARAM_SCLASS unsigned char pwm_mode);
#endif
void ClosePWM1 (void);


/* ***** PWM2 (CCP2CON) ***** */

#if !defined(__18F248)  && !defined(__18F258)  && \
    !defined(__18F448)  && !defined(__18F458)  && \
    !defined(__18F2480) && !defined(__18F2580) && \
    !defined(__18F4480) && !defined(__18F4580) && \
    !defined(__18F2585) && !defined(__18F2680) && \
    !defined(__18F4585) && !defined(__18F4680) && \
    !defined(__18F1220) && !defined(__18F1320)

void OpenPWM2 (PARAM_SCLASS char period);
void SetDCPWM2(PARAM_SCLASS unsigned int duty_cycle);
#if defined(__18F6525) || defined(__18F6621) || \
    defined(__18F8525) || defined(__18F8621) || \
    defined(__18F64J15) || defined(__18F65J10) || defined(__18F65J15) || \
    defined(__18F66J10) || defined(__18F66J15) || defined(__18F67J10) || \
    defined(__18F84J15) || defined(__18F85J10) || defined(__18F85J15) || \
    defined(__18F86J10) || defined(__18F86J15) || defined(__18F87J10) || \
    defined(__18F6627) || defined(__18F6722) || \
    defined(__18F8627) || defined(__18F8722)
void SetOutputPWM2 (PARAM_SCLASS unsigned char output_config, 
                    PARAM_SCLASS unsigned char pwm_mode);
#endif
void ClosePWM2 (void);

#endif


/* ***** PWM3 (CC3PCON) ***** */

#if defined(__18F6520) || defined(__18F6620) || defined(__18F6720) || \
    defined(__18F8520) || defined(__18F8620) || defined(__18F8720) || \
    defined(__18F6525) || defined(__18F6621) || \
    defined(__18F8525) || defined(__18F8621) || \
    defined(__18F6310) || defined(__18F8310) || \
    defined(__18F6410) || defined(__18F8410) || \
    defined(__18F64J15) || defined(__18F65J10) || defined(__18F65J15) || \
    defined(__18F66J10) || defined(__18F66J15) || defined(__18F67J10) || \
    defined(__18F84J15) || defined(__18F85J10) || defined(__18F85J15) || \
    defined(__18F86J10) || defined(__18F86J15) || defined(__18F87J10) || \
    defined(__18F6627) || defined(__18F6722) || \
    defined(__18F8627) || defined(__18F8722)

void OpenPWM3 (PARAM_SCLASS char period);
void SetDCPWM3 (PARAM_SCLASS unsigned int duty_cycle);
#if defined(__18F6525) || defined(__18F6621) || \
    defined(__18F8525) || defined(__18F8621) || \
    defined(__18F64J15) || defined(__18F65J10) || defined(__18F65J15) || \
    defined(__18F66J10) || defined(__18F66J15) || defined(__18F67J10) || \
    defined(__18F84J15) || defined(__18F85J10) || defined(__18F85J15) || \
    defined(__18F86J10) || defined(__18F86J15) || defined(__18F87J10) || \
    defined(__18F6627) || defined(__18F6722) || \
    defined(__18F8627) || defined(__18F8722)
void SetOutputPWM3 (PARAM_SCLASS unsigned char output_config,
                    PARAM_SCLASS unsigned char pwm_mode);
#endif
void ClosePWM3 (void);

#endif


/* ***** PWM4 (CCP4CON) ***** */

#if defined(__18F6520) || defined(__18F6620) || defined(__18F6720) || \
    defined(__18F8520) || defined(__18F8620) || defined(__18F8720) || \
    defined(__18F6525) || defined(__18F6621) || \
    defined(__18F8525) || defined(__18F8621) || \
    defined(__18F64J15) || defined(__18F65J10) || defined(__18F65J15) || \
    defined(__18F66J10) || defined(__18F66J15) || defined(__18F67J10) || \
    defined(__18F84J15) || defined(__18F85J10) || defined(__18F85J15) || \
    defined(__18F86J10) || defined(__18F86J15) || defined(__18F87J10) || \
    defined(__18F6627) || defined(__18F6722) || \
    defined(__18F8627) || defined(__18F8722)

void OpenPWM4 (PARAM_SCLASS char period);
void SetDCPWM4 (PARAM_SCLASS unsigned int duty_cycle);
void ClosePWM4 (void);

#endif


/* ***** PWM5 (CCP5CON) ***** */

#if defined(__18F6520) || defined(__18F6620) || defined(__18F6720) || \
    defined(__18F8520) || defined(__18F8620) || defined(__18F8720) || \
    defined(__18F6525) || defined(__18F6621) || \
    defined(__18F8525) || defined(__18F8621) || \
    defined(__18F64J15) || defined(__18F65J10) || defined(__18F65J15) || \
    defined(__18F66J10) || defined(__18F66J15) || defined(__18F67J10) || \
    defined(__18F84J15) || defined(__18F85J10) || defined(__18F85J15) || \
    defined(__18F86J10) || defined(__18F86J15) || defined(__18F87J10) || \
    defined(__18F6627) || defined(__18F6722) || \
    defined(__18F8627) || defined(__18F8722)

void OpenPWM5 (PARAM_SCLASS char period);
void SetDCPWM5 (PARAM_SCLASS unsigned int duty_cycle);
void ClosePWM5 (void);

#endif


/* ***** EPWM1 (ECCP1CON) ***** */

#if defined(__18F448)  || defined(__18F458) || \
    defined(__18F4480) || defined(__18F4580) || \
    defined(__18F4585) || defined(__18F4680)

void OpenEPWM1 (PARAM_SCLASS char period);
void SetDCEPWM1 (PARAM_SCLASS unsigned int duty_cycle);
void SetOutputEPWM1 (PARAM_SCLASS unsigned char output_config,
                     PARAM_SCLASS unsigned char pwm_mode);
void CloseEPWM1 (void);

#endif

#endif
