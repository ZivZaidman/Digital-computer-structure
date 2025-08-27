#ifndef _api_H_
#define _api_H_


#include  "../header/halGPIO.h"     // private library - HAL layer


extern void printSWs2LEDs(void);
extern void printArr2SWs(char Arr[], int size, unsigned int rate);
extern void state1Func();
extern void state2Func();
extern void state3Func();
extern int state1indicator ;
//extern void setLEDs(char);


#endif







