/* $Id: reset.h,v 1.2 2004/02/10 20:41:54 sealep Exp $ */
#ifndef __RESET_H
#define __RESET_H

/* PIC18 Reset-related operations */

#define WDT_ENABLED

#ifndef __18C601
#ifndef __18C801
#define BOR_ENABLED
#define NOT_RBPU RBPU
#endif
#endif

char isMCLR(void);       /* MCLR reset?                   */
void StatusReset(void);  /* Reset the POR and BOR bits    */

#if defined(BOR_ENABLED)
char isBOR(void);        /* BOR reset?                    */
#endif

#if defined(WDT_ENABLED)
char isWDTTO(void);      /* WDT timeout during operation? */
char isWDTWU(void);      /* WDT timeout during sleep?     */
#endif

char isPOR(void);        /* POR reset?                    */
char isWU(void);         /* Wakeup during sleep?          */

char isLVD(void);        /*LVD-- Low voltage detect?      */

#endif /* __RESET_H */
