#include  "../header/api.h"    		// private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer


//-------------------------------------------------------------
//            Print SWs value onto LEDs
//-------------------------------------------------------------
void printSWs2LEDs(void){
	unsigned char ch;
	
	ch = readSWs();
	print2LEDs(ch);
}
//--------------------------------------------------------------------
//            Print array to LEDs array with rate of LEDs_SHOW_RATE
//--------------------------------------------------------------------            
void printArr2SWs(char Arr[], int size, unsigned int rate){
	unsigned int i;
	
	for(i=0; i<size; i++){
		print2LEDs(Arr[i]);
		delay(rate);
	}
}
//--------------------------------
//                  state 1 function
//---------------------------------

void state1Func(){
    setLEDs(state1LED);
    if (state1indicator==1){
        int i;
        for (i=0;i<20;i++){
            if (state1indicator==0){break;}
            incLEDs(1);
            state1LED++;
            delay(LEDs_SHOW_RATE);  // delay of 62.5 [ms]
        }

    }
    if (state1indicator==0){
        int i;
        for (i=0;i<20;i++){
            if (state1indicator==1){break;}
            incLEDs(-1);
            state1LED--;
            delay(LEDs_SHOW_RATE);  // delay of 62.5 [ms]

        }
    }
}

void state2Func(){
    int k;
    for (k=0;k<11;k++){
        clrLEDs();
        setLEDs(state2LED);
        state2LED = state2LED<<1;
        if (state2LED>0xA0){
            state2LED=1;
        }
        delay(LEDs_SHOW_RATE);      // delay of 62.5 [ms]
    }
}

void state3Func(){
    while(state==state3){
        PWM(1);
        delay(HighDelay);      // delay of 62.5 [ms]
        PWM(0);
        delay(LowDelay);
    /*MCLKFrequnsy = 2^20Hz
     * MCLKTime = 0.956us
     * wanted time: 1/4000s=0.25ms
     *MCLKtick=(0.25*10^-3)/(0.956*10^-6)=261.5
     *delay=11 MCLKticks
     *delay needed to reach 4khz: 261.5/11=~23
     */
    }
}


 
  

