#include  "../header/api.h"         // private library - API layer
#include  "../header/app.h"         // private library - APP layer

enum FSMstate state;
enum SYSmode lpm_mode;
int state_or_set_x=1;
int state_or_getString=1;
int input_slot=0;
int x=500;
char input[16]={0};
char Stringinput[16]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
char colors[8]={0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7};
int tone_series[7]={1000,1250,1500,1750,2000,2250,2500};
int j=0;
//int tone_series[7] = {1001,801,667,571,500,444,400};



void main(void){

  state = state0;  // start in idle state on RESET
  lpm_mode = mode0;     // start in idle state on RESET
  sysConfig();
  lcd_init();
  lcd_clear();

  while(1){
    switch(state){
      case state0: //idle
          lcd_cmd(0x01);
          RGBArrPortOut&=~0x07;
          enterLPM(mode0);
          break;

      case state1:
          lcd_cmd(0x01);
          RGBArrPortOut&=~0x07;
          state1Func();
          break;

      case state2:
          lcd_cmd(0x01);
          RGBArrPortOut&=~0x07;
          state2Func();
          break;

      case state3:
          lcd_cmd(0x01);
          RGBArrPortOut&=~0x07;
          state3Func();
          break;

      case state4:
          //GenPortOut &= ~0x10;
           state3Flag=0;
           state_or_set_x=0;
           RGBArrPortOut&=~0x07;
           state=state0;
        break;

      case state5:
          RGBArrPortOut&=~0x07;
          //readLDR();
          state5Func();
        break;

      case state6:
          lcd_cmd(0x01);
          lcd_counter=0;
          RGBArrPortOut&=~0x07;
        break;

      case state7:
          lcd_cmd(0x01);
          state_or_getString=0;
          __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
          state7Func();
          //state=state0;
    }
  }
}

