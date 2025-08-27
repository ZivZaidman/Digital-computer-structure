#ifndef _app_H_
#define _app_H_



enum FSMstate{state0,state1,state2,state3,state4}; // global variable
enum SYSmode{mode0,mode1,mode2,mode3,mode4}; // global variable
extern int state1indicator;
extern void BlinkColors (char colors[],int size, int delay_ms);
#endif







