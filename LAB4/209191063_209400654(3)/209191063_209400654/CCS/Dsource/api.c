#include  "../header/api.h"    		// private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer
unsigned int start;
unsigned int end;
int count;
char counterToPrint[16];
unsigned int buzzerGen[7] = {1001,801,667,571,500,444,400};
//unsigned int buzzerGen[7] = {1048,838,700,600,524,466,419};//1/(freq/smclk)


void intNumToString(int num){
  int num2 = num;
  int counter = 0;
  while(num2!=0){
    num2 = num2/10;
    counter++;
  }
  while(counter>0){
      counterToPrint[counter-1] = (num%10+'0');
      num = num/10;
      counter--; 
    }
}
//---------------------------------------------------------------------
//            timerA0On
//---------------------------------------------------------------------
void timerA0On(){
  int temp = x;
  while(temp >500){
      enable_TimerA0();
      enterLPM(0);
      temp -= 500;
  }
  if (temp != 0){
      int temp2 = temp;
      temp = temp<<7;          // we did y*128 insted of y*131 = y*0xffff/500
      TA0CCR0 = temp + temp2 + temp2 + temp2;
      enable_TimerA0();
      enterLPM(0);
      TA0CCR0 = 0xFFFF;      // value of 0.5 sec
  }
}
//---------------------------------------------------------------------
//            RGB switch
//---------------------------------------------------------------------
void RGBSwitch(){
    int count = 4;
    while(state == state1){
        printToRGB(count);
        if(count == 7)
            count = 0;
        else
            count++;
        timerA0On();
    }
    printToRGB(0);
}
//---------------------------------------------------------------------
//            countUP
//---------------------------------------------------------------------
void countUP(){
    lcd_clear();
    lcd_home();
    cursor_off;
    while(state == state2){
        cursor_off;
        lcd_clear();
        lcd_home();
        intNumToString(countUp);
        lcd_puts(counterToPrint);
        countUp++;
        lcd_home();
        timerA0On();
    }

}
//---------------------------------------------------------------------
//            buzzer series gen
//---------------------------------------------------------------------
void buzzer_gen(){
    unsigned int buz_pos = 0;
    enable_TimerA1_buz();
    while(state == state3){
        TA1CCR0 = buzzerGen[buz_pos];
        TA1CCR2 = TA1CCR0 >> 1;
        timerA0On();
        if(buz_pos == 6)
            buz_pos = 0;
        else
            buz_pos++;
    }
    disable_TimerA1();
}
//---------------------------------------------------------------------
//            set x
//---------------------------------------------------------------------
void set_X(){
    state_or_set_x = 0;
}
//---------------------------------------------------------------------
//            readLDR
//---------------------------------------------------------------------
void readLDR(){
    unsigned int voltage_level;
    int output_voltage;
    //float output_voltage;
    int smallerthen1 = 0;
    lcd_clear();
    lcd_home();
    while(state == state5){
        enable_ADC10();
        __bis_SR_register(LPM0_bits + GIE);
        voltage_level = ADC10MEM; //+ voltage_level;
        ADC10CTL0 &= ~ADC10ON;
        output_voltage = voltage_level << 5;//*32
        //output_voltage = voltage_level*3.3;
        if(output_voltage < 10000){
        //if(output_voltage < 1000){
            smallerthen1 = 1;
        }
        intNumToString(output_voltage);
        int i,index = 0;
        for(i = 0; i < 5; i++){
            cursor_off;
            if(i == 0){
                if(smallerthen1)
                    lcd_data('0');
                else{
                    lcd_data(counterToPrint[index]);
                    index++;
                }
            }
            else if(i == 1)
                lcd_data('.');
            else{
                lcd_data(counterToPrint[index]);
                index++;
            }
        }
        lcd_home();
        smallerthen1 = 0;
    }
    int i;
    for( i=0 ; i < 16 ; i++)
            counterToPrint[i] = 0;

}
//---------------------------------------------------------------------
//            reset X and Clr LCD
//---------------------------------------------------------------------
void resetXandClrLCD(){
    int i=0;
    for( i=0 ; i < 16 ; i++)
        counterToPrint[i] = 0;

    countUp = 0;
    lcd_clear();
}










