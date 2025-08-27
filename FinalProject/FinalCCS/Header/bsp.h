#ifndef _bsp_H_
#define _bsp_H_

#include  <msp430g2553.h>          // MSP430x2xx
//#include  <msp430xG46x.h>  // MSP430x4xx
#include <stdint.h>


#define   debounceVal      3000
//#define   LEDs_SHOW_RATE   0xFFFF  // 62_5ms

/*
//RGB abstraction
#define RGBArrPortOut      P2OUT
#define RGBArrPortSel      P2SEL
#define RGBArrPortDir      P2DIR

// LEDs abstraction
#define LEDsArrPort        P9OUT
#define LEDsArrPortDir     P9DIR
#define LEDsArrPortSel     P9SEL
*/

// LCDs abstraction
#define LCD_DATA_WRITE     P2OUT
#define LCD_DATA_DIR       P2DIR
#define LCD_DATA_READ      P2IN
#define LCD_DATA_SEL       P2SEL
#define LCD_CTL_SEL        P1SEL

// Joystick abstraction
#define JoyStickPortOUT     P1OUT
#define JoyStickPortSEL     P1SEL
#define JoyStickPortDIR     P1DIR
#define JoyStickPortIN      P1IN
#define JoyStickIntEdgeSel  P1IES
#define JoyStickIntEN       P1IE
#define JoyStickIntPend     P1IFG
#define JoyStickPortREN     P1REN


// Stepmotor abstraction
#define StepmotorPortOUT     P2OUT
#define StepmotorPortSEL     P2SEL
#define StepmotorPortDIR     P2DIR
/*#define StepmotorPortIN      P2IN
#define StepmotorIntEdgeSel  P2IES
#define StepmotorIntEN       P2IE
#define StepmotorIntPend     P2IFG*/

//     UART abstraction
#define UARTArrPortSel        P1SEL
#define UARTArrPortSel2       P1SEL2

#endif
