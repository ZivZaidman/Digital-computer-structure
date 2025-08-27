#ifndef _app_H_
#define _app_H_


enum FSMstate{state0,state1,state2,state3,state4,state5,state6,state7,state8,state9}; // global variable
enum SYSmode{mode0,mode1,mode2,mode3,mode4}; // global variable
extern char idiom_recorder[];
extern int KBprev;
extern int KeypadCounterPrev;
extern unsigned int KA;
extern char strMerge[];
extern void Clear_LEDs();
extern void ClearStrMerge();
extern char str[];
extern int state_or_set_x;
extern int input_slot;
extern char input[];
extern int x;
extern char colors[8];
extern int tone_series[7];
#endif





