/*#ifdef __MSP430FG4619
#include  "../header/bsp_msp430x4xx.h"    // private library - BSP layer
#else*/
#include  "../header/bsp_msp430x2xx.h"    // private library - BSP layer#
//#endif
#ifndef _halGPIO_H_
#define _halGPIO_H_
#include  "../header/app.h"    		// private library - APP layer


extern enum FSMstate state;   // global variable
extern enum SYSmode lpm_mode; // global variable
extern int state1indicator ;
extern int state2LED;
extern int state1LED;
extern int dutyCycle;
extern void enablePB0only(void);
extern void disablePB0only();
extern void sysConfig(void);
extern void print2LEDs(unsigned char);
extern void clrLEDs(void);
extern void toggleLEDs(char);
extern void setLEDs(char);
extern unsigned char readSWs(void);
extern void printRGB(unsigned char);
extern void delay(unsigned int);
extern void enterLPM(unsigned char);
extern void incLEDs(char);
extern void enable_interrupts();
extern void disable_interrupts();
extern void PWM (int);

extern __interrupt void PBs_handler(void);

#endif







