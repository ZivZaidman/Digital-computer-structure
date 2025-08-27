#include  "../header/api.h"         // private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer
#include "stdio.h"

int lcd_counter=0;
int state3ind=0;
int state3Flag=0;
float time=0;
int Vlevel=0;
int Vreel=0;
//---------------------------------------------------
//              state1 Function
//---------------------------------------------------
void state1Func(){
    while (state==state1){
        int i;
        for (i=0;i<8;i++){
            if (state!=state1){
                break;
            }
            RGBArrPortOut=colors[i];
            startTimer0A0(x);
            __bis_SR_register(LPM0_bits+GIE);
        }
    }
}
//---------------------------------------------------
//              state2 Function - Seconds Counter
//---------------------------------------------------
void state2Func(){
    while (state==state2){
        lcd_cmd(0x01);
        if (lcd_counter<=9){
            lcd_data(lcd_counter+0x30);
        }else if (lcd_counter<=99){
            lcd_data(lcd_counter/10+0x30);
            lcd_data(lcd_counter%10+0x30);
        }else if (lcd_counter<=999){
            lcd_data(lcd_counter/100+0x30);
            lcd_data((lcd_counter/10)%10+0x30);
            lcd_data(lcd_counter%10+0x30);
        }else if (lcd_counter<=9999){
            lcd_data(lcd_counter/1000+0x30);
            lcd_data((lcd_counter/100)%10+0x30);
            lcd_data((lcd_counter/10)%10+0x30);
            lcd_data(lcd_counter%10+0x30);
        }else if (lcd_counter>=10000){
            lcd_counter=0;
        }

        lcd_counter++;
        startTimer0A0(x);
        __bis_SR_register(LPM0_bits+GIE);
    }
}

//---------------------------------------------------
//              state3 Function - LED printing using DMA
//---------------------------------------------------
void state3Func(){

    while (state==state3){
        state3Flag=1;
        time=50000.0/((float)tone_series[state3ind]);
        //check
        //startTimer1A0(x);
        //__bis_SR_register(LPM0_bits+GIE);
        //check over
        startTimer0A0(x);
        while(state3Flag){
            if (state!=state3){
                break;
            }
            PWM(time);


        }
    }
}


//-----------------------------------------------------
//              state4
//----------------------------------------------------
void state4Func(){
    lcd_cmd(0x01);


}
//-----------------------------------------------------
//              state4
//----------------------------------------------------
/*void readLDR(){
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

}*/
//-----------------------------------------------------
//              state5
//----------------------------------------------------
void state5Func(){
    int j=0;
    for (j=0;j<5;j++){
        lcd_cmd(0x14);
    }
    lcd_puts("volt",4);
    lcd_cmd(0x02);
    while (state==state5){
            startADC();
            __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
            ADCoff();
            Vlevel= ADC10MEM;
            Vreel=((float)Vlevel/1024.0)*3800;
            floatToPrint(Vreel);
    }
}
//-----------------------------------------------------
//              state7
//----------------------------------------------------
void state7Func(){
    while (gobacktosleepind==0){
        __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
    }
    //if (dontPrintInd == 0){
    lcd_puts(Stringinput,16);
//    }
    int j=0;
    for (j=0;j<16;j++){
        Stringinput[j]=0x20;
        input[j]=0x20;
    }
    if (state==state7){
        state=state0;
    }

}

