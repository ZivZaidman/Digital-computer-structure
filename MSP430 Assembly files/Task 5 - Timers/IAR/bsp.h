#ifndef _bsp_H_
#define _bsp_H_

#include  <msp430.h>          // MSP430x2xx
//#include  "msp430g2553.h"//
// #include  <msp430xG46x.h>  // MSP430x4xx

#define  DataSegStart      0x200
#define  CodeSegStart      0xC000
#define  StackTosStart     0x0400

#define   debounceVal      250
#define   delay62_5ms      0xFFFF
#define   delay0_8_62_5ms  0xCABB
#define   delay300us       0x66
#define   delay100us       0x1A
#define   delay200us       0x34
#define   delay15ms        0x4FFF
#define   delay5ms         0x1FFF

// LEDs abstraction
#define LEDsArrPort        &P1OUT
#define LEDsArrPortDir     &P1DIR
#define LEDsArrPortSel     &P1SEL
#define PWMArrPort         &P2OUT


// Switches abstraction
#define SWsArrPort         &P2IN
#define SWsArrPortDir      &P2DIR
#define SWsArrPortSel      &P2SEL
#define SWmask             0x0F

// PushButtons abstraction
#define PBsArrPort	   &P2IN 
#define PBsArrIntPend	   &P2IFG 
#define PBsArrIntEn	   &P2IE
#define PBsArrIntEdgeSel   &P2IES
#define PBsArrPortSel      &P2SEL 
#define PBsArrPortDir      &P2DIR 
#define PB0                0x1
#define PB1                0x2
#define PB2                0x4
#define PB3                0x8


// LCDs abstraction
#define LCDsDataArrPort        &P1OUT
#define LCDsDataArrPortSel     &P1SEL
#define LCDsDataArrPortDir     &P1DIR
#define LCDsControlArrPort     &P2OUT
#define LCDsControlArrSel      &P2SEL
#define LCDsControlArrPortDir  &P2DIR
#define LCDe               0x20
#define LCDrs              0x40
#define LCDrw              0x80



//  Timers abstraction
//  Timer ver
#define TimerSource0       TASSEL_0 /* Timer A clock source select: 0 - TACLK */
#define TimerSource1       TASSEL_1 /* Timer A clock source select: 0 - ACLK */
#define TimerSource2       TASSEL_2 /* Timer A clock source select: 0 - SMCLK */
#define TimerSource3       TASSEL_3 /* Timer A clock source select: 0 - INCLK */
#define TimerCLR           TACLR
#define TimerIE            TAIE
#define TimerCCIE          CCIE
#define TimerIFG           TAIFG

//Timer0
#define Timer0Reg           TA0R
#define Timer0Control       TA0CTL
#define Timer0CCTL0         TA0CCTL0
#define Timer0CCTL1         TA0CCTL1
#define Timer0CCTL2         TA0CCTL2
#define Timer0CR0           TA0CCR0
#define Timer0CR1           TA0CCR1
#define Timer0CR2           TA0CCR2

//Timer1
#define Timer1Reg           TA1R
#define Timer1Control       TA1CTL
#define Timer1CCTL0         TA1CCTL0
#define Timer1CCTL1         TA1CCTL1
#define Timer1CCTL2         TA1CCTL2
#define Timer1CR0           TA1CCR0
#define Timer1CR1           TA1CCR1
#define Timer1CR2           TA1CCR2
#endif



