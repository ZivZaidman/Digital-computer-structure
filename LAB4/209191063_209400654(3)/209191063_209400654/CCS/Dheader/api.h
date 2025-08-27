#ifndef _api_H_
#define _api_H_

#include  "../header/halGPIO.h"     // private library - HAL layer
//#include <string.h>
//extern sprintf;

extern void NumToString(int num);


extern void timerA0On();
extern void RGBSwitch();
extern void countUP();
extern void buzzer_gen();
extern void set_X();
extern void readLDR();
extern void resetXandClrLCD();

#endif
