#ifndef _api_H_
#define _api_H_

#include  "../header/halGPIO.h"     // private library - HAL layer



extern void CountDown();
extern void startTimerA0();
extern void freqCounter();
extern void tone_generator();
extern void Signal_shape();
extern int  seconds0;
extern int  seconds1;
extern int  minutes0;
extern int  minutes1;
extern int FirstRowsPressesCounter;
extern int LastRowPressesCounter;
extern int KeypadCounter;
extern void  showToLCD();
extern void  startTimer1A0();
extern unsigned int REdge0;
extern unsigned int REdge1;
extern int Fexter;
extern void PrintFrequncey();
extern void FrequnceyToArray();
extern char digitsArray[];
extern void state3Func();
extern void state4Func();
extern char str1[];
extern char str2[];
extern char str3[];
extern char str4[];
extern char data_matrix[10][38];
extern char strMerge[];
extern void Clear_LEDs();
extern void ClearStrMerge();
extern char str[];
extern void BlockSizeStr();
extern char strSwap[];
extern int lcd_counter;
extern char colors[8];
extern int state3ind;
extern int state3Flag;
extern int tone_series[7];
extern float time;
extern void readLDR();
extern void state5Func();
extern void state7Func();
extern int state_or_getString;
extern char Stringinput[16];
#endif







