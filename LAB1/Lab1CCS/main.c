#include  "../header/api.h"    		// private library - API layer
#include  "../header/app.h"    		// private library - APP layer


enum FSMstate state;
enum SYSmode lpm_mode;
int state1indicator=1;
int state2LED=1;
int state1LED=0;
int dutyCycle=0;
char colors[8]={0x00,0x10,0x20,0x30,0x40,0x50,0x60,0x70};


void main(void){
  

  state = state0;  // start in idle state on RESET
  lpm_mode = mode0;     // start in idle state on RESET
  sysConfig();
  
  while(1){
	switch(state){
	  case state0:
	      clrLEDs();
	      enable_interrupts();
                enterLPM(lpm_mode);
		break;
		 
	  case state1:
	    disable_interrupts();
	    state1Func();
		state1indicator ^= 0x01;
		state1indicator &=0x01;
		state=state0;
		enable_interrupts();

		break;
	  case state2:
		disable_interrupts();
		state2Func();
		state=state0;
		enable_interrupts();
		break;
		
	  case state3:
        clrLEDs();
        state3Func();
        break;


	  case state4:
	    clrLEDs();
	    disable_interrupts();
	    BlinkColors(colors,20,25773);

	    state=state0;
	    enable_interrupts();
	}
  }
}
  
  
  
  
  
  
