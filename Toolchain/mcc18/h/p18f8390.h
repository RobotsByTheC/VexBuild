/*-------------------------------------------------------------------------
 * $Id: p18f8390.h,v 1.3 2004/10/12 23:15:16 curtiss Exp $
 * MPLAB-Cxx  PIC18F8390 processor header
 *
 * (c) Copyright 1999-2004 Microchip Technology, All rights reserved
 *-------------------------------------------------------------------------*/

#ifndef __18F8390_H
#define __18F8390_H

extern volatile far  unsigned char       LCDPS;
extern volatile far  union {
  struct {
    unsigned LP0:1;
    unsigned LP1:1;
    unsigned LP2:1;
    unsigned LP3:1;
    unsigned WA:1;
    unsigned LCDA:1;
    unsigned BIASMD:1;
    unsigned WFT:1;
  };
  struct {
    unsigned LCDPS0:1;
    unsigned LCDPS1:1;
    unsigned LCDPS2:1;
    unsigned LCDPS3:1;
  };
} LCDPSbits;
extern volatile far  unsigned char       LCDCON;
extern volatile far  union {
  struct {
    unsigned LMUX0:1;
    unsigned LMUX1:1;
    unsigned CS0:1;
    unsigned CS1:1;
    unsigned :1;
    unsigned WERR:1;
    unsigned SLPEN:1;
    unsigned LCDEN:1;
  };
  struct {
    unsigned :2;
    unsigned LCDCS0:1;
    unsigned LCDCS1:1;
    unsigned :1;
    unsigned LCDWERR:1;
    unsigned LCDSLPEN:1;
  };
} LCDCONbits;
extern volatile far  unsigned char       LCDSE0;
extern volatile far  union {
  struct {
    unsigned SE0:1;
    unsigned SE1:1;
    unsigned SE2:1;
    unsigned SE3:1;
    unsigned SE4:1;
    unsigned SE5:1;
    unsigned SE6:1;
    unsigned SE7:1;
  };
  struct {
    unsigned SEGEN0:1;
    unsigned SEGEN1:1;
    unsigned SEGEN2:1;
    unsigned SEGEN3:1;
    unsigned SEGEN4:1;
    unsigned SEGEN5:1;
    unsigned SEGEN6:1;
    unsigned SEGEN7:1;
  };
} LCDSE0bits;
extern volatile far  unsigned char       LCDSE1;
extern volatile far  union {
  struct {
    unsigned SE8:1;
    unsigned SE9:1;
    unsigned SE10:1;
    unsigned SE11:1;
    unsigned SE12:1;
    unsigned SE13:1;
    unsigned SE14:1;
    unsigned SE15:1;
  };
  struct {
    unsigned SEGEN8:1;
    unsigned SEGEN9:1;
    unsigned SEGEN10:1;
    unsigned SEGEN11:1;
    unsigned SEGEN12:1;
    unsigned SEGEN13:1;
    unsigned SEGEN14:1;
    unsigned SEGEN15:1;
  };
} LCDSE1bits;
extern volatile far  unsigned char       LCDSE2;
extern volatile far  union {
  struct {
    unsigned SE16:1;
    unsigned SE17:1;
    unsigned SE18:1;
    unsigned SE19:1;
    unsigned SE20:1;
    unsigned SE21:1;
    unsigned SE22:1;
    unsigned SE23:1;
  };
  struct {
    unsigned SEGEN16:1;
    unsigned SEGEN17:1;
    unsigned SEGEN18:1;
    unsigned SEGEN19:1;
    unsigned SEGEN20:1;
    unsigned SEGEN21:1;
    unsigned SEGEN22:1;
    unsigned SEGEN23:1;
  };
} LCDSE2bits;
extern volatile far  unsigned char       LCDSE3;
extern volatile far  union {
  struct {
    unsigned SE24:1;
    unsigned SE25:1;
    unsigned SE26:1;
    unsigned SE27:1;
    unsigned SE28:1;
    unsigned SE29:1;
    unsigned SE30:1;
    unsigned SE31:1;
  };
  struct {
    unsigned SEGEN24:1;
    unsigned SEGEN25:1;
    unsigned SEGEN26:1;
    unsigned SEGEN27:1;
    unsigned SEGEN28:1;
    unsigned SEGEN29:1;
    unsigned SEGEN30:1;
    unsigned SEGEN31:1;
  };
} LCDSE3bits;
extern volatile far  unsigned char       LCDSE4;
extern volatile far  union {
  struct {
    unsigned SE32:1;
    unsigned SE33:1;
    unsigned SE34:1;
    unsigned SE35:1;
    unsigned SE36:1;
    unsigned SE37:1;
    unsigned SE38:1;
    unsigned SE39:1;
  };
  struct {
    unsigned SEGEN32:1;
    unsigned SEGEN33:1;
    unsigned SEGEN34:1;
    unsigned SEGEN35:1;
    unsigned SEGEN36:1;
    unsigned SEGEN37:1;
    unsigned SEGEN38:1;
    unsigned SEGEN39:1;
  };
} LCDSE4bits;
extern volatile far  unsigned char       LCDSE5;
extern volatile far  union {
  struct {
    unsigned SE40:1;
    unsigned SE41:1;
    unsigned SE42:1;
    unsigned SE43:1;
    unsigned SE44:1;
    unsigned SE45:1;
    unsigned SE46:1;
    unsigned SE47:1;
  };
  struct {
    unsigned SEGEN40:1;
    unsigned SEGEN41:1;
    unsigned SEGEN42:1;
    unsigned SEGEN43:1;
    unsigned SEGEN44:1;
    unsigned SEGEN45:1;
    unsigned SEGEN46:1;
    unsigned SEGEN47:1;
  };
} LCDSE5bits;
extern volatile near unsigned char       LCDDATA0;
extern volatile near union {
  struct {
    unsigned S0C0:1;
    unsigned S1C0:1;
    unsigned S2C0:1;
    unsigned S3C0:1;
    unsigned S4C0:1;
    unsigned S5C0:1;
    unsigned S6C0:1;
    unsigned S7C0:1;
  };
  struct {
    unsigned SEG0COM0:1;
    unsigned SEG1COM0:1;
    unsigned SEG2COM0:1;
    unsigned SEG3COM0:1;
    unsigned SEG4COM0:1;
    unsigned SEG5COM0:1;
    unsigned SEG6COM0:1;
    unsigned SEG7COM0:1;
  };
} LCDDATA0bits;
extern volatile near unsigned char       LCDDATA1;
extern volatile near union {
  struct {
    unsigned S8C0:1;
    unsigned S9C0:1;
    unsigned S10C0:1;
    unsigned S11C0:1;
    unsigned S12C0:1;
    unsigned S13C0:1;
    unsigned S14C0:1;
    unsigned S15C0:1;
  };
  struct {
    unsigned SEG8COM0:1;
    unsigned SEG9COM0:1;
    unsigned SEG10COM0:1;
    unsigned SEG11COM0:1;
    unsigned SEG12COM0:1;
    unsigned SEG13COM0:1;
    unsigned SEG14COM0:1;
    unsigned SEG15COM0:1;
  };
} LCDDATA1bits;
extern volatile near unsigned char       LCDDATA2;
extern volatile near union {
  struct {
    unsigned S16C0:1;
    unsigned S17C0:1;
    unsigned S18C0:1;
    unsigned S19C0:1;
    unsigned S20C0:1;
    unsigned S21C0:1;
    unsigned S22C0:1;
    unsigned S23C0:1;
  };
  struct {
    unsigned SEG16COM0:1;
    unsigned SEG17COM0:1;
    unsigned SEG18COM0:1;
    unsigned SEG19COM0:1;
    unsigned SEG20COM0:1;
    unsigned SEG21COM0:1;
    unsigned SEG22COM0:1;
    unsigned SEG23COM0:1;
  };
} LCDDATA2bits;
extern volatile near unsigned char       LCDDATA3;
extern volatile near union {
  struct {
    unsigned S24C0:1;
    unsigned S25C0:1;
    unsigned S26C0:1;
    unsigned S27C0:1;
    unsigned S28C0:1;
    unsigned S29C0:1;
    unsigned S30C0:1;
    unsigned S31C0:1;
  };
  struct {
    unsigned SEG24COM0:1;
    unsigned SEG25COM0:1;
    unsigned SEG26COM0:1;
    unsigned SEG27COM0:1;
    unsigned SEG28COM0:1;
    unsigned SEG29COM0:1;
    unsigned SEG30COM0:1;
    unsigned SEG31COM0:1;
  };
} LCDDATA3bits;
extern volatile near unsigned char       LCDDATA4;
extern volatile near union {
  struct {
    unsigned S32C0:1;
    unsigned S33C0:1;
    unsigned S34C0:1;
    unsigned S35C0:1;
    unsigned S36C0:1;
    unsigned S37C0:1;
    unsigned S38C0:1;
    unsigned S39C0:1;
  };
  struct {
    unsigned SEG32COM0:1;
    unsigned SEG33COM0:1;
    unsigned SEG34COM0:1;
    unsigned SEG35COM0:1;
    unsigned SEG36COM0:1;
    unsigned SEG37COM0:1;
    unsigned SEG38COM0:1;
    unsigned SEG39COM0:1;
  };
} LCDDATA4bits;
extern volatile near unsigned char       LCDDATA5;
extern volatile near union {
  struct {
    unsigned S40C0:1;
    unsigned S41C0:1;
    unsigned S42C0:1;
    unsigned S43C0:1;
    unsigned S44C0:1;
    unsigned S45C0:1;
    unsigned S46C0:1;
    unsigned S47C0:1;
  };
  struct {
    unsigned SEG40COM0:1;
    unsigned SEG41COM0:1;
    unsigned SEG42COM0:1;
    unsigned SEG43COM0:1;
    unsigned SEG44COM0:1;
    unsigned SEG45COM0:1;
    unsigned SEG46COM0:1;
    unsigned SEG47COM0:1;
  };
} LCDDATA5bits;
extern volatile near unsigned char       LCDDATA6;
extern volatile near union {
  struct {
    unsigned S0C1:1;
    unsigned S1C1:1;
    unsigned S2C1:1;
    unsigned S3C1:1;
    unsigned S4C1:1;
    unsigned S5C1:1;
    unsigned S6C1:1;
    unsigned S7C1:1;
  };
  struct {
    unsigned SEG0COM1:1;
    unsigned SEG1COM1:1;
    unsigned SEG2COM1:1;
    unsigned SEG3COM1:1;
    unsigned SEG4COM1:1;
    unsigned SEG5COM1:1;
    unsigned SEG6COM1:1;
    unsigned SEG7COM1:1;
  };
} LCDDATA6bits;
extern volatile near unsigned char       LCDDATA7;
extern volatile near union {
  struct {
    unsigned S8C1:1;
    unsigned S9C1:1;
    unsigned S10C1:1;
    unsigned S11C1:1;
    unsigned S12C1:1;
    unsigned S13C1:1;
    unsigned S14C1:1;
    unsigned S15C1:1;
  };
  struct {
    unsigned SEG8COM1:1;
    unsigned SEG9COM1:1;
    unsigned SEG10COM1:1;
    unsigned SEG11COM1:1;
    unsigned SEG12COM1:1;
    unsigned SEG13COM1:1;
    unsigned SEG14COM1:1;
    unsigned SEG15COM1:1;
  };
} LCDDATA7bits;
extern volatile near unsigned char       LCDDATA8;
extern volatile near union {
  struct {
    unsigned S16C1:1;
    unsigned S17C1:1;
    unsigned S18C1:1;
    unsigned S19C1:1;
    unsigned S20C1:1;
    unsigned S21C1:1;
    unsigned S22C1:1;
    unsigned S23C1:1;
  };
  struct {
    unsigned SEG16COM1:1;
    unsigned SEG17COM1:1;
    unsigned SEG18COM1:1;
    unsigned SEG19COM1:1;
    unsigned SEG20COM1:1;
    unsigned SEG21COM1:1;
    unsigned SEG22COM1:1;
    unsigned SEG23COM1:1;
  };
} LCDDATA8bits;
extern volatile near unsigned char       LCDDATA9;
extern volatile near union {
  struct {
    unsigned S24C1:1;
    unsigned S25C1:1;
    unsigned S26C1:1;
    unsigned S27C1:1;
    unsigned S28C1:1;
    unsigned S29C1:1;
    unsigned S30C1:1;
    unsigned S31C1:1;
  };
  struct {
    unsigned SEG24COM1:1;
    unsigned SEG25COM1:1;
    unsigned SEG26COM1:1;
    unsigned SEG27COM1:1;
    unsigned SEG28COM1:1;
    unsigned SEG29COM1:1;
    unsigned SEG30COM1:1;
    unsigned SEG31COM1:1;
  };
} LCDDATA9bits;
extern volatile near unsigned char       LCDDATA10;
extern volatile near union {
  struct {
    unsigned S32C1:1;
    unsigned S33C1:1;
    unsigned S34C1:1;
    unsigned S35C1:1;
    unsigned S36C1:1;
    unsigned S37C1:1;
    unsigned S38C1:1;
    unsigned S39C1:1;
  };
  struct {
    unsigned SEG32COM1:1;
    unsigned SEG33COM1:1;
    unsigned SEG34COM1:1;
    unsigned SEG35COM1:1;
    unsigned SEG36COM1:1;
    unsigned SEG37COM1:1;
    unsigned SEG38COM1:1;
    unsigned SEG39COM1:1;
  };
} LCDDATA10bits;
extern volatile near unsigned char       RCSTA2;
extern volatile near union {
  struct {
    unsigned RX9D:1;
    unsigned OERR:1;
    unsigned FERR:1;
    unsigned ADEN:1;
    unsigned CREN:1;
    unsigned SREN:1;
    unsigned RX9:1;
    unsigned SPEN:1;
  };
  struct {
    unsigned RCD8:1;
    unsigned :5;
    unsigned RC9:1;
  };
  struct {
    unsigned :6;
    unsigned NOT_RC8:1;
  };
  struct {
    unsigned :6;
    unsigned RC8_9:1;
  };
} RCSTA2bits;
extern volatile near unsigned char       TXSTA2;
extern volatile near union {
  struct {
    unsigned TX9D:1;
    unsigned TRMT:1;
    unsigned BRGH:1;
    unsigned :1;
    unsigned SYNC:1;
    unsigned TXEN:1;
    unsigned TX9:1;
    unsigned CSRC:1;
  };
  struct {
    unsigned TXD8:1;
    unsigned :5;
    unsigned TX8_9:1;
  };
  struct {
    unsigned :6;
    unsigned NOT_TX8:1;
  };
} TXSTA2bits;
extern volatile near unsigned char       TXREG2;
extern volatile near unsigned char       RCREG2;
extern volatile near unsigned char       SPBRG2;
extern volatile near unsigned char       LCDDATA11;
extern volatile near union {
  struct {
    unsigned S40C1:1;
    unsigned S41C1:1;
    unsigned S42C1:1;
    unsigned S43C1:1;
    unsigned S44C1:1;
    unsigned S45C1:1;
    unsigned S46C1:1;
    unsigned S47C1:1;
  };
  struct {
    unsigned SEG40COM1:1;
    unsigned SEG41COM1:1;
    unsigned SEG42COM1:1;
    unsigned SEG43COM1:1;
    unsigned SEG44COM1:1;
    unsigned SEG45COM1:1;
    unsigned SEG46COM1:1;
    unsigned SEG47COM1:1;
  };
} LCDDATA11bits;
extern volatile near unsigned char       LCDDATA12;
extern volatile near union {
  struct {
    unsigned S0C2:1;
    unsigned S1C2:1;
    unsigned S2C2:1;
    unsigned S3C2:1;
    unsigned S4C2:1;
    unsigned S5C2:1;
    unsigned S6C2:1;
    unsigned S7C2:1;
  };
  struct {
    unsigned SEG0COM2:1;
    unsigned SEG1COM2:1;
    unsigned SEG2COM2:1;
    unsigned SEG3COM2:1;
    unsigned SEG4COM2:1;
    unsigned SEG5COM2:1;
    unsigned SEG6COM2:1;
    unsigned SEG7COM2:1;
  };
} LCDDATA12bits;
extern volatile near unsigned char       LCDDATA13;
extern volatile near union {
  struct {
    unsigned S8C2:1;
    unsigned S9C2:1;
    unsigned S10C2:1;
    unsigned S11C2:1;
    unsigned S12C2:1;
    unsigned S13C2:1;
    unsigned S14C2:1;
    unsigned S15C2:1;
  };
  struct {
    unsigned SEG8COM2:1;
    unsigned SEG9COM2:1;
    unsigned SEG10COM2:1;
    unsigned SEG11COM2:1;
    unsigned SEG12COM2:1;
    unsigned SEG13COM2:1;
    unsigned SEG14COM2:1;
    unsigned SEG15COM2:1;
  };
} LCDDATA13bits;
extern volatile near unsigned char       LCDDATA14;
extern volatile near union {
  struct {
    unsigned S16C2:1;
    unsigned S17C2:1;
    unsigned S18C2:1;
    unsigned S19C2:1;
    unsigned S20C2:1;
    unsigned S21C2:1;
    unsigned S22C2:1;
    unsigned S23C2:1;
  };
  struct {
    unsigned SEG16COM2:1;
    unsigned SEG17COM2:1;
    unsigned SEG18COM2:1;
    unsigned SEG19COM2:1;
    unsigned SEG20COM2:1;
    unsigned SEG21COM2:1;
    unsigned SEG22COM2:1;
    unsigned SEG23COM2:1;
  };
} LCDDATA14bits;
extern volatile near unsigned char       LCDDATA15;
extern volatile near union {
  struct {
    unsigned S24C2:1;
    unsigned S25C2:1;
    unsigned S26C2:1;
    unsigned S27C2:1;
    unsigned S28C2:1;
    unsigned S29C2:1;
    unsigned S30C2:1;
    unsigned S31C2:1;
  };
  struct {
    unsigned SEG24COM2:1;
    unsigned SEG25COM2:1;
    unsigned SEG26COM2:1;
    unsigned SEG27COM2:1;
    unsigned SEG28COM2:1;
    unsigned SEG29COM2:1;
    unsigned SEG30COM2:1;
    unsigned SEG31COM2:1;
  };
} LCDDATA15bits;
extern volatile near unsigned char       LCDDATA16;
extern volatile near union {
  struct {
    unsigned S32C2:1;
    unsigned S33C2:1;
    unsigned S34C2:1;
    unsigned S35C2:1;
    unsigned S36C2:1;
    unsigned S37C2:1;
    unsigned S38C2:1;
    unsigned S39C2:1;
  };
  struct {
    unsigned SEG32COM2:1;
    unsigned SEG33COM2:1;
    unsigned SEG34COM2:1;
    unsigned SEG35COM2:1;
    unsigned SEG36COM2:1;
    unsigned SEG37COM2:1;
    unsigned SEG38COM2:1;
    unsigned SEG39COM2:1;
  };
} LCDDATA16bits;
extern volatile near unsigned char       LCDDATA17;
extern volatile near union {
  struct {
    unsigned S40C2:1;
    unsigned S41C2:1;
    unsigned S42C2:1;
    unsigned S43C2:1;
    unsigned S44C2:1;
    unsigned S45C2:1;
    unsigned S46C2:1;
    unsigned S47C2:1;
  };
  struct {
    unsigned SEG40COM2:1;
    unsigned SEG41COM2:1;
    unsigned SEG42COM2:1;
    unsigned SEG43COM2:1;
    unsigned SEG44COM2:1;
    unsigned SEG45COM2:1;
    unsigned SEG46COM2:1;
    unsigned SEG47COM2:1;
  };
} LCDDATA17bits;
extern volatile near unsigned char       LCDDATA18;
extern volatile near union {
  struct {
    unsigned S0C3:1;
    unsigned S1C3:1;
    unsigned S2C3:1;
    unsigned S3C3:1;
    unsigned S4C3:1;
    unsigned S5C3:1;
    unsigned S6C3:1;
    unsigned S7C3:1;
  };
  struct {
    unsigned SEG0COM3:1;
    unsigned SEG1COM3:1;
    unsigned SEG2COM3:1;
    unsigned SEG3COM3:1;
    unsigned SEG4COM3:1;
    unsigned SEG5COM3:1;
    unsigned SEG6COM3:1;
    unsigned SEG7COM3:1;
  };
} LCDDATA18bits;
extern volatile near unsigned char       LCDDATA19;
extern volatile near union {
  struct {
    unsigned S8C3:1;
    unsigned S9C3:1;
    unsigned S10C3:1;
    unsigned S11C3:1;
    unsigned S12C3:1;
    unsigned S13C3:1;
    unsigned S14C3:1;
    unsigned S15C3:1;
  };
  struct {
    unsigned SEG8COM3:1;
    unsigned SEG9COM3:1;
    unsigned SEG10COM3:1;
    unsigned SEG11COM3:1;
    unsigned SEG12COM3:1;
    unsigned SEG13COM3:1;
    unsigned SEG14COM3:1;
    unsigned SEG15COM3:1;
  };
} LCDDATA19bits;
extern volatile near unsigned char       LCDDATA20;
extern volatile near union {
  struct {
    unsigned S16C3:1;
    unsigned S17C3:1;
    unsigned S18C3:1;
    unsigned S19C3:1;
    unsigned S20C3:1;
    unsigned S21C3:1;
    unsigned S22C3:1;
    unsigned S23C3:1;
  };
  struct {
    unsigned SEG16COM3:1;
    unsigned SEG17COM3:1;
    unsigned SEG18COM3:1;
    unsigned SEG19COM3:1;
    unsigned SEG20COM3:1;
    unsigned SEG21COM3:1;
    unsigned SEG22COM3:1;
    unsigned SEG23COM3:1;
  };
} LCDDATA20bits;
extern volatile near unsigned char       LCDDATA21;
extern volatile near union {
  struct {
    unsigned S24C3:1;
    unsigned S25C3:1;
    unsigned S26C3:1;
    unsigned S27C3:1;
    unsigned S28C3:1;
    unsigned S29C3:1;
    unsigned S30C3:1;
    unsigned S31C3:1;
  };
  struct {
    unsigned SEG24COM3:1;
    unsigned SEG25COM3:1;
    unsigned SEG26COM3:1;
    unsigned SEG27COM3:1;
    unsigned SEG28COM3:1;
    unsigned SEG29COM3:1;
    unsigned SEG30COM3:1;
    unsigned SEG31COM3:1;
  };
} LCDDATA21bits;
extern volatile near unsigned char       LCDDATA22;
extern volatile near union {
  struct {
    unsigned S32C3:1;
    unsigned S33C3:1;
    unsigned S34C3:1;
    unsigned S35C3:1;
    unsigned S36C3:1;
    unsigned S37C3:1;
    unsigned S38C3:1;
    unsigned S39C3:1;
  };
  struct {
    unsigned SEG32COM3:1;
    unsigned SEG33COM3:1;
    unsigned SEG34COM3:1;
    unsigned SEG35COM3:1;
    unsigned SEG36COM3:1;
    unsigned SEG37COM3:1;
    unsigned SEG38COM3:1;
    unsigned SEG39COM3:1;
  };
} LCDDATA22bits;
extern volatile near unsigned char       LCDDATA23;
extern volatile near union {
  struct {
    unsigned S40C3:1;
    unsigned S41C3:1;
    unsigned S42C3:1;
    unsigned S43C3:1;
    unsigned S44C3:1;
    unsigned S45C3:1;
    unsigned S46C3:1;
    unsigned S47C3:1;
  };
  struct {
    unsigned SEG40COM3:1;
    unsigned SEG41COM3:1;
    unsigned SEG42COM3:1;
    unsigned SEG43COM3:1;
    unsigned SEG44COM3:1;
    unsigned SEG45COM3:1;
    unsigned SEG46COM3:1;
    unsigned SEG47COM3:1;
  };
} LCDDATA23bits;
extern volatile near unsigned char       BAUDCTL1;
extern volatile near struct {
  unsigned ABDEN:1;
  unsigned WUE:1;
  unsigned :1;
  unsigned BRG16:1;
  unsigned SCKP:1;
  unsigned :1;
  unsigned RCMT:1;
  unsigned ABDOVF:1;
} BAUDCTL1bits;
extern volatile near unsigned char       SPBRGH1;
extern volatile near unsigned char       PORTA;
extern volatile near union {
  struct {
    unsigned RA0:1;
    unsigned RA1:1;
    unsigned RA2:1;
    unsigned RA3:1;
    unsigned RA4:1;
    unsigned RA5:1;
    unsigned RA6:1;
    unsigned RA7:1;
  };
  struct {
    unsigned AN0:1;
    unsigned AN1:1;
    unsigned AN2:1;
    unsigned AN3:1;
    unsigned T0CKI:1;
    unsigned AN4:1;
    unsigned OSC2:1;
    unsigned OSC1:1;
  };
  struct {
    unsigned :2;
    unsigned VREFM:1;
    unsigned VREFP:1;
    unsigned :1;
    unsigned LVDIN:1;
    unsigned CLKO:1;
    unsigned CLKI:1;
  };
  struct {
    unsigned :2;
    unsigned SEG16:1;
    unsigned SEG17:1;
    unsigned SEG14:1;
    unsigned SEG15:1;
  };
} PORTAbits;
extern volatile near unsigned char       PORTB;
extern volatile near union {
  struct {
    unsigned RB0:1;
    unsigned RB1:1;
    unsigned RB2:1;
    unsigned RB3:1;
    unsigned RB4:1;
    unsigned RB5:1;
    unsigned RB6:1;
    unsigned RB7:1;
  };
  struct {
    unsigned INT0:1;
    unsigned INT1:1;
    unsigned INT2:1;
    unsigned INT3:1;
    unsigned KBI0:1;
    unsigned KBI1:1;
    unsigned KBI2:1;
    unsigned KBI3:1;
  };
  struct {
    unsigned :3;
    unsigned CCP2B:1;
    unsigned :1;
    unsigned PGM:1;
    unsigned PGC:1;
    unsigned PGD:1;
  };
  struct {
    unsigned :1;
    unsigned SEG8:1;
    unsigned SEG9:1;
    unsigned SEG10:1;
    unsigned SEG11:1;
  };
} PORTBbits;
extern volatile near unsigned char       PORTC;
extern volatile near union {
  struct {
    unsigned RC0:1;
    unsigned RC1:1;
    unsigned RC2:1;
    unsigned RC3:1;
    unsigned RC4:1;
    unsigned RC5:1;
    unsigned RC6:1;
    unsigned RC7:1;
  };
  struct {
    unsigned T1OSO:1;
    unsigned T1OSI:1;
    unsigned CCP1:1;
    unsigned SCK:1;
    unsigned SDI:1;
    unsigned SDO:1;
    unsigned TX:1;
    unsigned RX:1;
  };
  struct {
    unsigned T13CKI:1;
    unsigned CCP2C:1;
    unsigned :1;
    unsigned SCL:1;
    unsigned SDA:1;
    unsigned :1;
    unsigned CK:1;
    unsigned DT:1;
  };
  struct {
    unsigned :2;
    unsigned SEG13:1;
    unsigned SEG12:1;
  };
} PORTCbits;
extern volatile near unsigned char       PORTD;
extern volatile near union {
  struct {
    unsigned RD0:1;
    unsigned RD1:1;
    unsigned RD2:1;
    unsigned RD3:1;
    unsigned RD4:1;
    unsigned RD5:1;
    unsigned RD6:1;
    unsigned RD7:1;
  };
  struct {
    unsigned AD0:1;
    unsigned AD1:1;
    unsigned AD2:1;
    unsigned AD3:1;
    unsigned AD4:1;
    unsigned AD5:1;
    unsigned AD6:1;
    unsigned AD7:1;
  };
  struct {
    unsigned SEG0:1;
    unsigned SEG1:1;
    unsigned SEG2:1;
    unsigned SEG3:1;
    unsigned SEG4:1;
    unsigned SEG5:1;
    unsigned SEG6:1;
    unsigned SEG7:1;
  };
} PORTDbits;
extern volatile near unsigned char       PORTE;
extern volatile near union {
  struct {
    unsigned RE0:1;
    unsigned RE1:1;
    unsigned RE2:1;
    unsigned RE3:1;
    unsigned RE4:1;
    unsigned RE5:1;
    unsigned RE6:1;
    unsigned RE7:1;
  };
  struct {
    unsigned :7;
    unsigned CCP2E:1;
  };
  struct {
    unsigned AD8:1;
    unsigned AD9:1;
    unsigned AD10:1;
    unsigned AD11:1;
    unsigned AD12:1;
    unsigned AD13:1;
    unsigned AD14:1;
    unsigned AD15:1;
  };
  struct {
    unsigned LCDBIAS1:1;
    unsigned LCDBIAS2:1;
    unsigned LCDBIAS3:1;
    unsigned COM0:1;
    unsigned COM1:1;
    unsigned COM2:1;
    unsigned COM3:1;
    unsigned SEG31:1;
  };
  struct {
    unsigned BIAS1:1;
    unsigned BIAS2:1;
    unsigned BIAS3:1;
  };
} PORTEbits;
extern volatile near unsigned char       PORTF;
extern volatile near union {
  struct {
    unsigned RF0:1;
    unsigned RF1:1;
    unsigned RF2:1;
    unsigned RF3:1;
    unsigned RF4:1;
    unsigned RF5:1;
    unsigned RF6:1;
    unsigned RF7:1;
  };
  struct {
    unsigned AN5:1;
    unsigned AN6:1;
    unsigned AN7:1;
    unsigned AN8:1;
    unsigned AN9:1;
    unsigned AN10:1;
    unsigned AN11:1;
    unsigned SS:1;
  };
  struct {
    unsigned :1;
    unsigned C2OUTF:1;
    unsigned C1OUTF:1;
    unsigned :2;
    unsigned CVREFF:1;
  };
  struct {
    unsigned SEG18:1;
    unsigned SEG19:1;
    unsigned SEG20:1;
    unsigned SEG21:1;
    unsigned SEG22:1;
    unsigned SEG23:1;
    unsigned SEG24:1;
    unsigned SEG25:1;
  };
} PORTFbits;
extern volatile near unsigned char       PORTG;
extern volatile near union {
  struct {
    unsigned RG0:1;
    unsigned RG1:1;
    unsigned RG2:1;
    unsigned RG3:1;
    unsigned RG4:1;
    unsigned RG5:1;
  };
  struct {
    unsigned :1;
    unsigned TX2:1;
    unsigned RX2:1;
    unsigned :1;
    unsigned MCLR:1;
  };
  struct {
    unsigned :1;
    unsigned CK2:1;
    unsigned DT2:1;
    unsigned VPP:1;
  };
  struct {
    unsigned SEG30:1;
    unsigned SEG29:1;
    unsigned SEG28:1;
    unsigned SEG27:1;
    unsigned SEG26:1;
  };
} PORTGbits;
extern volatile near unsigned char       PORTH;
extern volatile near union {
  struct {
    unsigned RH0:1;
    unsigned RH1:1;
    unsigned RH2:1;
    unsigned RH3:1;
    unsigned RH4:1;
    unsigned RH5:1;
    unsigned RH6:1;
    unsigned RH7:1;
  };
  struct {
    unsigned SEG47:1;
    unsigned SEG46:1;
    unsigned SEG45:1;
    unsigned SEG44:1;
    unsigned SEG40:1;
    unsigned SEG41:1;
    unsigned SEG42:1;
    unsigned SEG43:1;
  };
} PORTHbits;
extern volatile near unsigned char       PORTJ;
extern volatile near union {
  struct {
    unsigned RJ0:1;
    unsigned RJ1:1;
    unsigned RJ2:1;
    unsigned RJ3:1;
    unsigned RJ4:1;
    unsigned RJ5:1;
    unsigned RJ6:1;
    unsigned RJ7:1;
  };
  struct {
    unsigned SEG32:1;
    unsigned SEG33:1;
    unsigned SEG34:1;
    unsigned SEG35:1;
    unsigned SEG39:1;
    unsigned SEG38:1;
    unsigned SEG37:1;
    unsigned SEG36:1;
  };
} PORTJbits;
extern volatile near unsigned char       LATA;
extern volatile near struct {
  unsigned LATA0:1;
  unsigned LATA1:1;
  unsigned LATA2:1;
  unsigned LATA3:1;
  unsigned LATA4:1;
  unsigned LATA5:1;
  unsigned LATA6:1;
  unsigned LATA7:1;
} LATAbits;
extern volatile near unsigned char       LATB;
extern volatile near struct {
  unsigned LATB0:1;
  unsigned LATB1:1;
  unsigned LATB2:1;
  unsigned LATB3:1;
  unsigned LATB4:1;
  unsigned LATB5:1;
  unsigned LATB6:1;
  unsigned LATB7:1;
} LATBbits;
extern volatile near unsigned char       LATC;
extern volatile near struct {
  unsigned LATC0:1;
  unsigned LATC1:1;
  unsigned LATC2:1;
  unsigned LATC3:1;
  unsigned LATC4:1;
  unsigned LATC5:1;
  unsigned LATC6:1;
  unsigned LATC7:1;
} LATCbits;
extern volatile near unsigned char       LATD;
extern volatile near struct {
  unsigned LATD0:1;
  unsigned LATD1:1;
  unsigned LATD2:1;
  unsigned LATD3:1;
  unsigned LATD4:1;
  unsigned LATD5:1;
  unsigned LATD6:1;
  unsigned LATD7:1;
} LATDbits;
extern volatile near unsigned char       LATE;
extern volatile near struct {
  unsigned LATE0:1;
  unsigned LATE1:1;
  unsigned LATE2:1;
  unsigned LATE3:1;
  unsigned LATE4:1;
  unsigned LATE5:1;
  unsigned LATE6:1;
  unsigned LATE7:1;
} LATEbits;
extern volatile near unsigned char       LATF;
extern volatile near struct {
  unsigned LATF0:1;
  unsigned LATF1:1;
  unsigned LATF2:1;
  unsigned LATF3:1;
  unsigned LATF4:1;
  unsigned LATF5:1;
  unsigned LATF6:1;
  unsigned LATF7:1;
} LATFbits;
extern volatile near unsigned char       LATG;
extern volatile near struct {
  unsigned LATG0:1;
  unsigned LATG1:1;
  unsigned LATG2:1;
  unsigned LATG3:1;
  unsigned LATG4:1;
} LATGbits;
extern volatile near unsigned char       LATH;
extern volatile near struct {
  unsigned LATH0:1;
  unsigned LATH1:1;
  unsigned LATH2:1;
  unsigned LATH3:1;
  unsigned LATH4:1;
  unsigned LATH5:1;
  unsigned LATH6:1;
  unsigned LATH7:1;
} LATHbits;
extern volatile near unsigned char       LATJ;
extern volatile near struct {
  unsigned LATJ0:1;
  unsigned LATJ1:1;
  unsigned LATJ2:1;
  unsigned LATJ3:1;
  unsigned LATJ4:1;
  unsigned LATJ5:1;
  unsigned LATJ6:1;
  unsigned LATJ7:1;
} LATJbits;
extern volatile near unsigned char       DDRA;
extern volatile near struct {
  unsigned RA0:1;
  unsigned RA1:1;
  unsigned RA2:1;
  unsigned RA3:1;
  unsigned RA4:1;
  unsigned RA5:1;
  unsigned RA6:1;
  unsigned RA7:1;
} DDRAbits;
extern volatile near unsigned char       TRISA;
extern volatile near struct {
  unsigned TRISA0:1;
  unsigned TRISA1:1;
  unsigned TRISA2:1;
  unsigned TRISA3:1;
  unsigned TRISA4:1;
  unsigned TRISA5:1;
  unsigned TRISA6:1;
  unsigned TRISA7:1;
} TRISAbits;
extern volatile near unsigned char       DDRB;
extern volatile near struct {
  unsigned RB0:1;
  unsigned RB1:1;
  unsigned RB2:1;
  unsigned RB3:1;
  unsigned RB4:1;
  unsigned RB5:1;
  unsigned RB6:1;
  unsigned RB7:1;
} DDRBbits;
extern volatile near unsigned char       TRISB;
extern volatile near struct {
  unsigned TRISB0:1;
  unsigned TRISB1:1;
  unsigned TRISB2:1;
  unsigned TRISB3:1;
  unsigned TRISB4:1;
  unsigned TRISB5:1;
  unsigned TRISB6:1;
  unsigned TRISB7:1;
} TRISBbits;
extern volatile near unsigned char       DDRC;
extern volatile near struct {
  unsigned RC0:1;
  unsigned RC1:1;
  unsigned RC2:1;
  unsigned RC3:1;
  unsigned RC4:1;
  unsigned RC5:1;
  unsigned RC6:1;
  unsigned RC7:1;
} DDRCbits;
extern volatile near unsigned char       TRISC;
extern volatile near struct {
  unsigned TRISC0:1;
  unsigned TRISC1:1;
  unsigned TRISC2:1;
  unsigned TRISC3:1;
  unsigned TRISC4:1;
  unsigned TRISC5:1;
  unsigned TRISC6:1;
  unsigned TRISC7:1;
} TRISCbits;
extern volatile near unsigned char       DDRD;
extern volatile near struct {
  unsigned RD0:1;
  unsigned RD1:1;
  unsigned RD2:1;
  unsigned RD3:1;
  unsigned RD4:1;
  unsigned RD5:1;
  unsigned RD6:1;
  unsigned RD7:1;
} DDRDbits;
extern volatile near unsigned char       TRISD;
extern volatile near struct {
  unsigned TRISD0:1;
  unsigned TRISD1:1;
  unsigned TRISD2:1;
  unsigned TRISD3:1;
  unsigned TRISD4:1;
  unsigned TRISD5:1;
  unsigned TRISD6:1;
  unsigned TRISD7:1;
} TRISDbits;
extern volatile near unsigned char       DDRE;
extern volatile near struct {
  unsigned RE0:1;
  unsigned RE1:1;
  unsigned RE2:1;
  unsigned RE3:1;
  unsigned RE4:1;
  unsigned RE5:1;
  unsigned RE6:1;
  unsigned RE7:1;
} DDREbits;
extern volatile near unsigned char       TRISE;
extern volatile near struct {
  unsigned TRISE0:1;
  unsigned TRISE1:1;
  unsigned TRISE2:1;
  unsigned TRISE3:1;
  unsigned TRISE4:1;
  unsigned TRISE5:1;
  unsigned TRISE6:1;
  unsigned TRISE7:1;
} TRISEbits;
extern volatile near unsigned char       DDRF;
extern volatile near struct {
  unsigned RF0:1;
  unsigned RF1:1;
  unsigned RF2:1;
  unsigned RF3:1;
  unsigned RF4:1;
  unsigned RF5:1;
  unsigned RF6:1;
  unsigned RF7:1;
} DDRFbits;
extern volatile near unsigned char       TRISF;
extern volatile near struct {
  unsigned TRISF0:1;
  unsigned TRISF1:1;
  unsigned TRISF2:1;
  unsigned TRISF3:1;
  unsigned TRISF4:1;
  unsigned TRISF5:1;
  unsigned TRISF6:1;
  unsigned TRISF7:1;
} TRISFbits;
extern volatile near unsigned char       DDRG;
extern volatile near struct {
  unsigned RG0:1;
  unsigned RG1:1;
  unsigned RG2:1;
  unsigned RG3:1;
  unsigned RG4:1;
} DDRGbits;
extern volatile near unsigned char       TRISG;
extern volatile near struct {
  unsigned TRISG0:1;
  unsigned TRISG1:1;
  unsigned TRISG2:1;
  unsigned TRISG3:1;
  unsigned TRISG4:1;
} TRISGbits;
extern volatile near unsigned char       DDRH;
extern volatile near struct {
  unsigned RH0:1;
  unsigned RH1:1;
  unsigned RH2:1;
  unsigned RH3:1;
  unsigned RH4:1;
  unsigned RH5:1;
  unsigned RH6:1;
  unsigned RH7:1;
} DDRHbits;
extern volatile near unsigned char       TRISH;
extern volatile near struct {
  unsigned TRISH0:1;
  unsigned TRISH1:1;
  unsigned TRISH2:1;
  unsigned TRISH3:1;
  unsigned TRISH4:1;
  unsigned TRISH5:1;
  unsigned TRISH6:1;
  unsigned TRISH7:1;
} TRISHbits;
extern volatile near unsigned char       DDRJ;
extern volatile near struct {
  unsigned RJ0:1;
  unsigned RJ1:1;
  unsigned RJ2:1;
  unsigned RJ3:1;
  unsigned RJ4:1;
  unsigned RJ5:1;
  unsigned RJ6:1;
  unsigned RJ7:1;
} DDRJbits;
extern volatile near unsigned char       TRISJ;
extern volatile near struct {
  unsigned TRISJ0:1;
  unsigned TRISJ1:1;
  unsigned TRISJ2:1;
  unsigned TRISJ3:1;
  unsigned TRISJ4:1;
  unsigned TRISJ5:1;
  unsigned TRISJ6:1;
  unsigned TRISJ7:1;
} TRISJbits;
extern volatile near unsigned char       OSCTUNE;
extern volatile near struct {
  unsigned TUN0:1;
  unsigned TUN1:1;
  unsigned TUN2:1;
  unsigned TUN3:1;
  unsigned TUN4:1;
  unsigned TUN5:1;
  unsigned PLLEN:1;
  unsigned INTSRC:1;
} OSCTUNEbits;
extern volatile near unsigned char       PIE1;
extern volatile near union {
  struct {
    unsigned TMR1IE:1;
    unsigned TMR2IE:1;
    unsigned CCP1IE:1;
    unsigned SSPIE:1;
    unsigned TX1IE:1;
    unsigned RC1IE:1;
    unsigned ADIE:1;
  };
  struct {
    unsigned :4;
    unsigned TXIE:1;
    unsigned RCIE:1;
  };
} PIE1bits;
extern volatile near unsigned char       PIR1;
extern volatile near union {
  struct {
    unsigned TMR1IF:1;
    unsigned TMR2IF:1;
    unsigned CCP1IF:1;
    unsigned SSPIF:1;
    unsigned TX1IF:1;
    unsigned RC1IF:1;
    unsigned ADIF:1;
  };
  struct {
    unsigned :4;
    unsigned TXIF:1;
    unsigned RCIF:1;
  };
} PIR1bits;
extern volatile near unsigned char       IPR1;
extern volatile near union {
  struct {
    unsigned TMR1IP:1;
    unsigned TMR2IP:1;
    unsigned CCP1IP:1;
    unsigned SSPIP:1;
    unsigned TX1IP:1;
    unsigned RC1IP:1;
    unsigned ADIP:1;
  };
  struct {
    unsigned :4;
    unsigned TXIP:1;
    unsigned RCIP:1;
  };
} IPR1bits;
extern volatile near unsigned char       PIE2;
extern volatile near struct {
  unsigned CCP2IE:1;
  unsigned TMR3IE:1;
  unsigned LVDIE:1;
  unsigned BCLIE:1;
  unsigned EEIE:1;
  unsigned :1;
  unsigned CMIE:1;
  unsigned OSCFIE:1;
} PIE2bits;
extern volatile near unsigned char       PIR2;
extern volatile near struct {
  unsigned CCP2IF:1;
  unsigned TMR3IF:1;
  unsigned LVDIF:1;
  unsigned BCLIF:1;
  unsigned EEIF:1;
  unsigned :1;
  unsigned CMIF:1;
  unsigned OSCFIF:1;
} PIR2bits;
extern volatile near unsigned char       IPR2;
extern volatile near struct {
  unsigned CCP2IP:1;
  unsigned TMR3IP:1;
  unsigned LVDIP:1;
  unsigned BCLIP:1;
  unsigned EEIP:1;
  unsigned :1;
  unsigned CMIP:1;
  unsigned OSCFIP:1;
} IPR2bits;
extern volatile near unsigned char       PIE3;
extern volatile near struct {
  unsigned :4;
  unsigned TX2IE:1;
  unsigned RC2IE:1;
} PIE3bits;
extern volatile near unsigned char       PIR3;
extern volatile near struct {
  unsigned :4;
  unsigned TX2IF:1;
  unsigned RC2IF:1;
} PIR3bits;
extern volatile near unsigned char       IPR3;
extern volatile near struct {
  unsigned :4;
  unsigned TX2IP:1;
  unsigned RC2IP:1;
} IPR3bits;
extern volatile near unsigned char       EECON1;
extern volatile near struct {
  unsigned RD:1;
  unsigned WR:1;
  unsigned WREN:1;
  unsigned WRERR:1;
  unsigned FREE:1;
  unsigned :1;
  unsigned CFGS:1;
} EECON1bits;
extern volatile near unsigned char       RCSTA;
extern volatile near union {
  struct {
    unsigned RX9D:1;
    unsigned OERR:1;
    unsigned FERR:1;
    unsigned ADEN:1;
    unsigned CREN:1;
    unsigned SREN:1;
    unsigned RX9:1;
    unsigned SPEN:1;
  };
  struct {
    unsigned :3;
    unsigned ADDEN:1;
  };
} RCSTAbits;
extern volatile near unsigned char       RCSTA1;
extern volatile near union {
  struct {
    unsigned RX9D:1;
    unsigned OERR:1;
    unsigned FERR:1;
    unsigned ADEN:1;
    unsigned CREN:1;
    unsigned SREN:1;
    unsigned RX9:1;
    unsigned SPEN:1;
  };
  struct {
    unsigned :3;
    unsigned ADDEN:1;
  };
} RCSTA1bits;
extern volatile near unsigned char       TXSTA;
extern volatile near struct {
  unsigned TX9D:1;
  unsigned TRMT:1;
  unsigned BRGH:1;
  unsigned :1;
  unsigned SYNC:1;
  unsigned TXEN:1;
  unsigned TX9:1;
  unsigned CSRC:1;
} TXSTAbits;
extern volatile near unsigned char       TXSTA1;
extern volatile near struct {
  unsigned TX9D:1;
  unsigned TRMT:1;
  unsigned BRGH:1;
  unsigned :1;
  unsigned SYNC:1;
  unsigned TXEN:1;
  unsigned TX9:1;
  unsigned CSRC:1;
} TXSTA1bits;
extern volatile near unsigned char       TXREG;
extern volatile near unsigned char       TXREG1;
extern volatile near unsigned char       RCREG1;
extern volatile near unsigned char       SPBRG;
extern volatile near unsigned char       SPBRG1;
extern volatile near unsigned char       T3CON;
extern volatile near union {
  struct {
    unsigned TMR3ON:1;
    unsigned TMR3CS:1;
    unsigned NOT_T3SYNC:1;
    unsigned T3CCP1:1;
    unsigned T3CKPS0:1;
    unsigned T3CKPS1:1;
    unsigned T3CCP2:1;
    unsigned RD16:1;
  };
  struct {
    unsigned :2;
    unsigned T3SYNC:1;
  };
  struct {
    unsigned :2;
    unsigned T3INSYNC:1;
  };
} T3CONbits;
extern volatile near unsigned char       TMR3L;
extern volatile near unsigned char       TMR3H;
extern volatile near unsigned char       CMCON;
extern volatile near struct {
  unsigned CM0:1;
  unsigned CM1:1;
  unsigned CM2:1;
  unsigned CIS:1;
  unsigned C1INV:1;
  unsigned C2INV:1;
  unsigned C1OUT:1;
  unsigned C2OUT:1;
} CMCONbits;
extern volatile near unsigned char       CVRCON;
extern volatile near struct {
  unsigned CVR0:1;
  unsigned CVR1:1;
  unsigned CVR2:1;
  unsigned CVR3:1;
  unsigned CVREF:1;
  unsigned CVRR:1;
  unsigned CVROE:1;
  unsigned CVREN:1;
} CVRCONbits;
extern volatile near unsigned char       CCP2CON;
extern volatile near union {
  struct {
    unsigned CCP2M0:1;
    unsigned CCP2M1:1;
    unsigned CCP2M2:1;
    unsigned CCP2M3:1;
    unsigned DCCP2Y:1;
    unsigned DCCP2X:1;
  };
  struct {
    unsigned :4;
    unsigned CCP2Y:1;
    unsigned CCP2X:1;
  };
  struct {
    unsigned :4;
    unsigned DC2B0:1;
    unsigned DC2B1:1;
  };
} CCP2CONbits;
extern volatile near unsigned            CCPR2;
extern volatile near unsigned char       CCPR2L;
extern volatile near unsigned char       CCPR2H;
extern volatile near unsigned char       CCP1CON;
extern volatile near union {
  struct {
    unsigned CCP1M0:1;
    unsigned CCP1M1:1;
    unsigned CCP1M2:1;
    unsigned CCP1M3:1;
    unsigned DCCP1Y:1;
    unsigned DCCP1X:1;
  };
  struct {
    unsigned :4;
    unsigned CCP1Y:1;
    unsigned CCP1X:1;
  };
  struct {
    unsigned :4;
    unsigned DC1B0:1;
    unsigned DC1B1:1;
  };
} CCP1CONbits;
extern volatile near unsigned            CCPR1;
extern volatile near unsigned char       CCPR1L;
extern volatile near unsigned char       CCPR1H;
extern volatile near unsigned char       ADCON2;
extern volatile near struct {
  unsigned ADCS0:1;
  unsigned ADCS1:1;
  unsigned ADCS2:1;
  unsigned ACQT0:1;
  unsigned ACQT1:1;
  unsigned ACQT2:1;
  unsigned :1;
  unsigned ADFM:1;
} ADCON2bits;
extern volatile near unsigned char       ADCON1;
extern volatile near struct {
  unsigned PCFG0:1;
  unsigned PCFG1:1;
  unsigned PCFG2:1;
  unsigned PCFG3:1;
  unsigned VCFG0:1;
  unsigned VCFG1:1;
} ADCON1bits;
extern volatile near unsigned char       ADCON0;
extern volatile near union {
  struct {
    unsigned ADON:1;
    unsigned GO_DONE:1;
    unsigned CHS0:1;
    unsigned CHS1:1;
    unsigned CHS2:1;
    unsigned CHS3:1;
  };
  struct {
    unsigned :1;
    unsigned DONE:1;
  };
  struct {
    unsigned :1;
    unsigned GO:1;
  };
  struct {
    unsigned :1;
    unsigned NOT_DONE:1;
  };
} ADCON0bits;
extern volatile near unsigned            ADRES;
extern volatile near unsigned char       ADRESL;
extern volatile near unsigned char       ADRESH;
extern volatile near unsigned char       SSPCON2;
extern volatile near struct {
  unsigned SEN:1;
  unsigned RSEN:1;
  unsigned PEN:1;
  unsigned RCEN:1;
  unsigned ACKEN:1;
  unsigned ACKDT:1;
  unsigned ACKSTAT:1;
  unsigned GCEN:1;
} SSPCON2bits;
extern volatile near unsigned char       SSPCON1;
extern volatile near struct {
  unsigned SSPM0:1;
  unsigned SSPM1:1;
  unsigned SSPM2:1;
  unsigned SSPM3:1;
  unsigned CKP:1;
  unsigned SSPEN:1;
  unsigned SSPOV:1;
  unsigned WCOL:1;
} SSPCON1bits;
extern volatile near unsigned char       SSPSTAT;
extern volatile near union {
  struct {
    unsigned BF:1;
    unsigned UA:1;
    unsigned R_W:1;
    unsigned S:1;
    unsigned P:1;
    unsigned D_A:1;
    unsigned CKE:1;
    unsigned SMP:1;
  };
  struct {
    unsigned :2;
    unsigned I2C_READ:1;
    unsigned I2C_START:1;
    unsigned I2C_STOP:1;
    unsigned I2C_DAT:1;
  };
  struct {
    unsigned :2;
    unsigned NOT_W:1;
    unsigned :2;
    unsigned NOT_A:1;
  };
  struct {
    unsigned :2;
    unsigned NOT_WRITE:1;
    unsigned :2;
    unsigned NOT_ADDRESS:1;
  };
  struct {
    unsigned :2;
    unsigned READ_WRITE:1;
    unsigned :2;
    unsigned DATA_ADDRESS:1;
  };
  struct {
    unsigned :2;
    unsigned R:1;
    unsigned :2;
    unsigned D:1;
  };
} SSPSTATbits;
extern volatile near unsigned char       SSPADD;
extern volatile near unsigned char       SSPBUF;
extern volatile near unsigned char       T2CON;
extern volatile near struct {
  unsigned T2CKPS0:1;
  unsigned T2CKPS1:1;
  unsigned TMR2ON:1;
  unsigned T2OUTPS0:1;
  unsigned T2OUTPS1:1;
  unsigned T2OUTPS2:1;
  unsigned T2OUTPS3:1;
} T2CONbits;
extern volatile near unsigned char       PR2;
extern volatile near unsigned char       TMR2;
extern volatile near unsigned char       T1CON;
extern volatile near union {
  struct {
    unsigned TMR1ON:1;
    unsigned TMR1CS:1;
    unsigned NOT_T1SYNC:1;
    unsigned T1OSCEN:1;
    unsigned T1CKPS0:1;
    unsigned T1CKPS1:1;
    unsigned T1RUN:1;
    unsigned RD16:1;
  };
  struct {
    unsigned :2;
    unsigned T1SYNC:1;
  };
  struct {
    unsigned :2;
    unsigned T1INSYNC:1;
  };
} T1CONbits;
extern volatile near unsigned char       TMR1L;
extern volatile near unsigned char       TMR1H;
extern volatile near unsigned char       RCON;
extern volatile near union {
  struct {
    unsigned NOT_BOR:1;
    unsigned NOT_POR:1;
    unsigned NOT_PD:1;
    unsigned NOT_TO:1;
    unsigned NOT_RI:1;
    unsigned :1;
    unsigned SBOREN:1;
    unsigned IPEN:1;
  };
  struct {
    unsigned BOR:1;
    unsigned POR:1;
    unsigned PD:1;
    unsigned TO:1;
    unsigned RI:1;
  };
} RCONbits;
extern volatile near unsigned char       WDTCON;
extern volatile near union {
  struct {
    unsigned SWDTEN:1;
  };
  struct {
    unsigned SWDTE:1;
  };
} WDTCONbits;
extern volatile near unsigned char       HLVDCON;
extern volatile near union {
  struct {
    unsigned LVDL0:1;
    unsigned LVDL1:1;
    unsigned LVDL2:1;
    unsigned LVDL3:1;
    unsigned LVDEN:1;
    unsigned IRVST:1;
    unsigned :1;
    unsigned VDIRMAG:1;
  };
  struct {
    unsigned LVV0:1;
    unsigned LVV1:1;
    unsigned LVV2:1;
    unsigned LVV3:1;
    unsigned :1;
    unsigned BGST:1;
  };
} HLVDCONbits;
extern volatile near unsigned char       OSCCON;
extern volatile near struct {
  unsigned SCS0:1;
  unsigned SCS1:1;
  unsigned FLTS:1;
  unsigned OSTS:1;
  unsigned IRCF0:1;
  unsigned IRCF1:1;
  unsigned IRCF2:1;
  unsigned IDLEN:1;
} OSCCONbits;
extern volatile near unsigned char       T0CON;
extern volatile near struct {
  unsigned T0PS0:1;
  unsigned T0PS1:1;
  unsigned T0PS2:1;
  unsigned PSA:1;
  unsigned T0SE:1;
  unsigned T0CS:1;
  unsigned T08BIT:1;
  unsigned TMR0ON:1;
} T0CONbits;
extern volatile near unsigned char       TMR0L;
extern volatile near unsigned char       TMR0H;
extern          near unsigned char       STATUS;
extern          near struct {
  unsigned C:1;
  unsigned DC:1;
  unsigned Z:1;
  unsigned OV:1;
  unsigned N:1;
} STATUSbits;
extern          near unsigned            FSR2;
extern          near unsigned char       FSR2L;
extern          near unsigned char       FSR2H;
extern volatile near unsigned char       PLUSW2;
extern volatile near unsigned char       PREINC2;
extern volatile near unsigned char       POSTDEC2;
extern volatile near unsigned char       POSTINC2;
extern          near unsigned char       INDF2;
extern          near unsigned char       BSR;
extern          near unsigned            FSR1;
extern          near unsigned char       FSR1L;
extern          near unsigned char       FSR1H;
extern volatile near unsigned char       PLUSW1;
extern volatile near unsigned char       PREINC1;
extern volatile near unsigned char       POSTDEC1;
extern volatile near unsigned char       POSTINC1;
extern          near unsigned char       INDF1;
extern          near unsigned char       WREG;
extern          near unsigned            FSR0;
extern          near unsigned char       FSR0L;
extern          near unsigned char       FSR0H;
extern volatile near unsigned char       PLUSW0;
extern volatile near unsigned char       PREINC0;
extern volatile near unsigned char       POSTDEC0;
extern volatile near unsigned char       POSTINC0;
extern          near unsigned char       INDF0;
extern volatile near unsigned char       INTCON3;
extern volatile near union {
  struct {
    unsigned INT1IF:1;
    unsigned INT2IF:1;
    unsigned INT3IF:1;
    unsigned INT1IE:1;
    unsigned INT2IE:1;
    unsigned INT3IE:1;
    unsigned INT1IP:1;
    unsigned INT2IP:1;
  };
  struct {
    unsigned INT1F:1;
    unsigned INT2F:1;
    unsigned INT3F:1;
    unsigned INT1E:1;
    unsigned INT2E:1;
    unsigned INT3E:1;
    unsigned INT1P:1;
    unsigned INT2P:1;
  };
} INTCON3bits;
extern volatile near unsigned char       INTCON2;
extern volatile near union {
  struct {
    unsigned RBIP:1;
    unsigned INT3IP:1;
    unsigned TMR0IP:1;
    unsigned INTEDG3:1;
    unsigned INTEDG2:1;
    unsigned INTEDG1:1;
    unsigned INTEDG0:1;
    unsigned NOT_RBPU:1;
  };
  struct {
    unsigned :1;
    unsigned INT3P:1;
    unsigned T0IP:1;
    unsigned :4;
    unsigned RBPU:1;
  };
} INTCON2bits;
extern volatile near unsigned char       INTCON;
extern volatile near union {
  struct {
    unsigned RBIF:1;
    unsigned INT0IF:1;
    unsigned TMR0IF:1;
    unsigned RBIE:1;
    unsigned INT0IE:1;
    unsigned TMR0IE:1;
    unsigned PEIE:1;
    unsigned GIE:1;
  };
  struct {
    unsigned :1;
    unsigned INT0F:1;
    unsigned T0IF:1;
    unsigned :1;
    unsigned INT0E:1;
    unsigned T0IE:1;
    unsigned GIEL:1;
    unsigned GIEH:1;
  };
} INTCONbits;
extern          near unsigned            PROD;
extern          near unsigned char       PRODL;
extern          near unsigned char       PRODH;
extern volatile near unsigned char       TABLAT;
extern volatile near unsigned short long TBLPTR;
extern volatile near unsigned char       TBLPTRL;
extern volatile near unsigned char       TBLPTRH;
extern volatile near unsigned char       TBLPTRU;
extern volatile near unsigned short long PC;
extern volatile near unsigned char       PCL;
extern volatile near unsigned char       PCLATH;
extern volatile near unsigned char       PCLATU;
extern volatile near unsigned char       STKPTR;
extern volatile near union {
  struct {
    unsigned STKPTR0:1;
    unsigned STKPTR1:1;
    unsigned STKPTR2:1;
    unsigned STKPTR3:1;
    unsigned STKPTR4:1;
    unsigned :1;
    unsigned STKUNF:1;
    unsigned STKOVF:1;
  };
  struct {
    unsigned :7;
    unsigned STKFUL:1;
  };
} STKPTRbits;
extern          near unsigned short long TOS;
extern          near unsigned char       TOSL;
extern          near unsigned char       TOSH;
extern          near unsigned char       TOSU;

#pragma varlocate 15 LCDPS
#pragma varlocate 15 LCDPSbits
#pragma varlocate 15 LCDCON
#pragma varlocate 15 LCDCONbits
#pragma varlocate 15 LCDSE0
#pragma varlocate 15 LCDSE0bits
#pragma varlocate 15 LCDSE1
#pragma varlocate 15 LCDSE1bits
#pragma varlocate 15 LCDSE2
#pragma varlocate 15 LCDSE2bits
#pragma varlocate 15 LCDSE3
#pragma varlocate 15 LCDSE3bits
#pragma varlocate 15 LCDSE4
#pragma varlocate 15 LCDSE4bits
#pragma varlocate 15 LCDSE5
#pragma varlocate 15 LCDSE5bits

/*-------------------------------------------------------------------------
 * Some useful defines for inline assembly stuff
 *-------------------------------------------------------------------------*/
#define ACCESS 0
#define BANKED 1

/*-------------------------------------------------------------------------
 * Some useful macros for inline assembly stuff
 *-------------------------------------------------------------------------*/
#define Nop()    {_asm nop _endasm}
#define ClrWdt() {_asm clrwdt _endasm}
#define Sleep()  {_asm sleep _endasm}
#define Reset()  {_asm reset _endasm}

#define Rlcf(f,dest,access)  {_asm movlb f rlcf f,dest,access _endasm}
#define Rlncf(f,dest,access) {_asm movlb f rlncf f,dest,access _endasm}
#define Rrcf(f,dest,access)  {_asm movlb f rrcf f,dest,access _endasm}
#define Rrncf(f,dest,access) {_asm movlb f rrncf f,dest,access _endasm}
#define Swapf(f,dest,access) {_asm movlb f swapf f,dest,access _endasm }

/*-------------------------------------------------------------------------
 * A fairly inclusive set of registers to save for interrupts.
 * These are locations which are commonly used by the compiler.
 *-------------------------------------------------------------------------*/
#define INTSAVELOCS TBLPTR, TABLAT, PROD


#endif
