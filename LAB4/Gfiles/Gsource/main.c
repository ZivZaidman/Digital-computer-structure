#include  "../header/api.h"    		// private library - API layer
#include  "../header/app.h"    		// private library - APP layer
#include  <stdio.h>


//------------------------Roy Kislev-----------------------------
//----------------------Michael Grenader-------------------------

enum FSMstate state;
enum SYSmode lpm_mode;


void main(void){
  
  state = state9;  // start in idle state on RESET
  lpm_mode = mode0;     // start in idle state on RESET
  sysConfig();     // Configure GPIO, Stop Timers, Init LCD


  while(1){
	switch(state){
	case state9: //idle
	    IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
	    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
	    break;
	case state1: // Blink RGB LED
	    blinkRGB();
	    RGBArrPortOut = 0;
	    break;
	case state2: // Count up onto LCD
	    count_up_LCD();
	    break;

	case state3: ; // Count down onto LCD
	    count_down_LCD();
		break;
		
	case state4: // Change Delay Time in ms
	    change_delay_time();
		break;

    case state5: // Measure Potentiometer 3-digit value
        measure_pot();
        break;

    case state6: // Clear Counts and LCD
        clear_counters();
        break;

    case state7: //Real Time
        PB1_send_to_PC();
        break;

    case state8: // Print Menu to PC
        state = state9;
        break;
		
	}
  }
}

  
  
  
  
  
  
