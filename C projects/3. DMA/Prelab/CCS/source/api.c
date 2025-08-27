#include  "../header/api.h"         // private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer
#include "stdio.h"

int FirstRowsPressesCounter=0;
int LastRowPressesCounter=0;
int KeypadCounter;
char LEDnums[8] = {128,64,32,16,8,4,23,13,40};
char FirstStr[38]={0};
char SecondStr[38]={0};
char FirstSpaces[8]={0};
char SecondSpaces[8]={0};
int temp1;
int temp2;
int col1=0;
int col2=0;
int PrintLocation=0;
//---------------------------------------------------
//              state1 Function
//---------------------------------------------------
void state1Func(){
    //lcd_puts_data_matrix();
    lcd_puts("write an Idiom:",15);
    lcd_cmd(0xc0);
    FirstRowsPressesCounter=0;
    LastRowPressesCounter=0;
    KeypadCounter=0;
    for (KeypadCounter;KeypadCounter<32;){
        enable_interrupts();
        unsigned int i=KeypadCounter;
           if(state==state1){
            __bis_SR_register(LPM0_bits + GIE);
            idiom_recorder[i]=KB;
            lcd_data(0x30+KB);
            lcd_cmd(0x10);
            TIMER0_A0_config();
            startTimer0A0();
            }
           else{
               KeypadCounter=32;
           }

    }
    /*
     idiom recorder:
     * print "write an Idiom" to LCD
     * while (state1){
     *enable keypad interupt
     * sleep+GIE
     * keypad_interupt(prints the lettar, if pressed twice change to next. save in char idiom_recorder
     * fin
     * }
     *
     */

}
//---------------------------------------------------
//              state2 Function - Seconds Counter
//---------------------------------------------------
void state2Func(){
    temp1=0;
    temp2=0;
    col1=0;
    col2=0;
    stop_timers();
    while (state==state2){
        //enable_interrupts();
        lcd_cmd(0x01);
        GPIOconfig();
        getRowNum(&temp1, "1st line 0-9");
        if (state != state2){
            break;
        }
        GPIOconfig();
        getRowNum(&temp2, "2nd line 0-9");
        if (state != state2){
                    break;
        }
        if ((temp1>9)|(temp2>9)){
            lcd_puts("invalid Input",13);
            break;
        }
        BlocksSizeArr(temp1,data_matrix,FirstSpaces);
        BlocksSizeArr(temp2,data_matrix,SecondSpaces);
        int dstAddress = strMerge;
        int i;
        for (i=0;i<8;i++){
            stop_timers();
            disable_TimerBCCIE();
            DMA1config();
            startDMA0(temp1,dstAddress,FirstSpaces[i],col1);
 //           __bis_SR_register(LPM0_bits + GIE);
            dstAddress+=FirstSpaces[i];
            col1+=FirstSpaces[i];
            DMA1config();
            startDMA0(temp2,dstAddress,SecondSpaces[i],col2);
//            __bis_SR_register(LPM0_bits + GIE);
            dstAddress+=SecondSpaces[i];
            col2+=SecondSpaces[i];
        }
        for (i=0;i<8;i++){
           FirstSpaces[i]=0;
           SecondSpaces[i]=0;
        }
        lcd_cmd(0x01);
        lcd_puts(&strMerge[1],30);
        //lcd_cmd(0xC0);
        //lcd_puts(&strMerge[16],16);
        while (state==state2){
            __bis_SR_register(LPM0_bits + GIE);
            if ((state!=state2)|(KA!=10)){
                continue;
            }
            lcd_cmd(0x01);
            lcd_puts(&strMerge[31],30);
            //lcd_cmd(0xC0);
            //lcd_puts(&strMerge[48],16);
            while (state==state2){
                __bis_SR_register(LPM0_bits + GIE);
                if ((state!=state2)|(KA!=10)){
                    continue;
                }
                lcd_cmd(0x01);
                lcd_puts(&strMerge[62],15);
                break;
            }break;
        }
        if(state==state2){
            state=state0;
        }
    }
}
void BlocksSizeArr(int line,char strarr[][38],char indexarr[]) {
    int i = 0;
    int k=line;
    int countzero = 0;
    int temp = 0;
    int j = 0;
    for (i = 0; i < 38; i++){
        if ((strarr[k][i] == 0x20)&(i!=0)) {
            indexarr[j] = i-temp;
            temp = i;
            j++;
        }
        else if (strarr[k][i] == 0) {
            countzero++;
            if (countzero>1){
               i=38;
            }else{
            indexarr[j] = i - temp;
            j++;}

        }
    }
}

void getRowNum(int *temp, const char *prompt) {
    enable_interrupts();
    lcd_cmd(0x01);
    lcd_puts(prompt, 12);
    __bis_SR_register(LPM0_bits + GIE); // Enter low power mode with interrupts enabled
    *temp = KA;
    lcd_cmd(0x01);
}
void ClearStrMerge(){
    int i;
    for (i=0;i<76;i++){
        strMerge[i]=0;
    }
}
/*void getRowNum1(){
    enable_interrupts();
    lcd_puts("1st line 0-9",12);
    __bis_SR_register(LPM0_bits + GIE);
    temp1=KA;
    lcd_cmd(0x01);
}
void getRowNum2(){
    enable_interrupts();
    lcd_puts("2nd line 0-9",12);
    __bis_SR_register(LPM0_bits + GIE);
    temp2=KA;
    lcd_cmd(0x01);
}*/
/*
void state2Func(){
    stop_timers();
    while (state==state2){
        firstLine();
        secondLine();
    }
}

void firstLine(){
    GPIOconfig();
    lcd_cmd(0x01);
    lcd_puts("choose 1st line",15);
    __bis_SR_register(LPM0_bits + GIE);
    temp1=KA;
}

void secondLine(){
    //GPIOconfig();
    KeypadIRQIntEn |= BIT1;               // P2.1 - '1'
    lcd_cmd(0x01);
    lcd_puts("choose 2nd line",15);
    __bis_SR_register(LPM0_bits + GIE);
    temp2=KA;
}*/
//---------------------------------------------------
//              state3 Function - LED printing using DMA
//---------------------------------------------------
void state3Func(){
    TimerBconfig();
    lcd_cmd(0x01);
    LEDsArrPort = 0x00;
    DMA1config();
    disable_TimerBCCIE();
    while(state==state3){
        int i;
        for(i=0;i<8;i++) {
            setDMA1src(&LEDnums[i]);
            enableDMA1();
            TimerBstart();
            __bis_SR_register(LPM0_bits + GIE);
        }
        enable_interrupts();
    }

    }


//----------------------------------------
//          state4 Function
//----------------------------------------
void state4Func(){

}







