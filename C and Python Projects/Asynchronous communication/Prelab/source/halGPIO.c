#include  "../header/halGPIO.h"     // private library - HAL layer

int Count=0;
float fl_R=0.0;
float fl_T=0.0;
int th_x_ind=0;
int th_x=0;
int state3FlagThou=0;
char Volt[4]={0};
//--------------------------------------------------------------------
//             System Configuration
//--------------------------------------------------------------------
void sysConfig(void){
    GPIOconfig();
    //TimerBconfig();
    //TIMER1_A2_config();
    //TIMER1_A1_config();
    TIMER1_A0_config();
    TIMER0_A0_config();
    ADCconfig();
    UARTConfig();
}
//-------------------------------------------------------------------
//            Print Voltage to LCD
//-------------------------------------------------------------------
void floatToPrint(int num){
    int i=0;
    Volt[i]=num/1000;
    lcd_data(Volt[0]+0x30);
    lcd_data(0x2E);
    int force=100;
    for (i=1;i<4;i++){
        Volt[i]=(int)((float)num/(float)force)%10;
        force/=10;
    }
    int j=0;
    for (j=1;j<4;j++){
        lcd_data(Volt[j]+0x30);
        }
    lcd_cmd(0x02);
}
//-------------------------------------------------------------------
//            PWM
//-------------------------------------------------------------------
void PWM(int num){
    GenPortOut &= ~0x10;
    startTimer1A0(num);
    __bis_SR_register(LPM0_bits+GIE);
    GenPortOut |= 0x10;
    startTimer1A0(num);
    __bis_SR_register(LPM0_bits+GIE);
}
//-------------------------------------------------------------------
//            start DMA0 - state2
//-------------------------------------------------------------------
/*void startDMA0(src,dst,Bsize,col){
    unsigned int i =src;
    unsigned int j=col;
    DMA0SA=(unsigned int)&data_matrix[i][j];
    DMA0SZ=Bsize;
    DMA0DA=dst;
    //disable_CCIE;
    DMA0CTL |= DMAREQ + DMAEN + DMAIE;
}*/
//-------------------------------------------------------------------
//            start DMA2 - state3
//-------------------------------------------------------------------
/*void startDMA2(src,dst,Bsize){
    //unsigned int i =src;
    DMA0SA=src;
    DMA0SZ=Bsize;
    DMA0DA=dst;
    //disable_CCIE;
    DMA0CTL |= DMAREQ + DMAEN + DMAIE;
}*/
//-------------------------------------------------------------------
//            stop timers
//-------------------------------------------------------------------
void stop_timers(){
    StopTimerB();
    stopTimer0A0();
}
//-------------------------------------------------------------------
//            start Timer0 A0 - count 1 sec
//-------------------------------------------------------------------
/*void startTimer1A2(){
    TA0R = 0x01;
    TA1CCTL2 = CCIE;
    __bis_SR_register(LPM0_bits);
    //enterLPM()
}*/
//--------------------------------------------------------------
//                 start TimerB - state3
//--------------------------------------------------------------
/*void TimerBstart(){
    TB0R=0x00;
    TBCTL |= MC_1; // statrt TimerB
    TB0CCR0 = 0xFFFF;      // value of 0.5 sec = 2^12/2^13
}
void StopTimerB(){
    TBCTL&=MC_0;
}*/
//-------------------------------------------------------------------
//            start ADC
//------------------------------------------------------------------
void startADC(){
    ADC10CTL0 |= ADC10ON + ADC10IE+ ENC + ADC10SC;
}

//------------------------------------------------------------------
//              shutdown ADC
//------------------------------------------------------------------
void ADCoff(){
    ADC10CTL0 &= ~ADC10ON;
}

//--------------------------------------------------------------
//                 start Timer1 A0
//--------------------------------------------------------------
void startTimer1A0(int num1){
    float num2=num1;
    if (num2<=1000.0){
            fl_T = (float)(0.65535*num2); // 0xFFFF/ 1000.0
            TA1CCR0 = (int)fl_T;
            TA1CTL |= TASSEL_2 + MC_1;//+ TACLR; //SMCLK, up mode
            TA1CCTL0 = CCIE;
    }



}
//--------------------------------------------------------------
//                 start Timer0 A0
//--------------------------------------------------------------
void startTimer0A0(int num){

    if (num<=1000){
        fl_R = 65.535*num;
        // 0xFFFF/ 1000.0

    } else{
        int restx=num%1000;
        th_x= num/1000;
        TACCR0=0xFFFF;
        int i;
        for(i=0;i<th_x;i++){
            state3FlagThou=1;
            TA0CTL |= TASSEL_2 + MC_1+ TACLR; //SMCLK, up mode
            //TA0CCTL0 = CCIE;
            th_x_ind=i+1;
            if (state==state3){
                while(state3FlagThou){
                            if (state!=state3){
                                break;
                            }
                            PWM(time);
                }
            }
            if((state!=state3)&&(state!=state4)){
                    __bis_SR_register(LPM0_bits+GIE);}
        }if(restx!=0){
            fl_R = 65.535*restx;
        }

    }
    TACCR0 = (int)fl_R;
    TA0CTL |= TASSEL_2 + MC_1+ TACLR; //SMCLK, up mode
    TA0CCTL0 = CCIE;
}
//------------------------------------------------------------------
//              stop timer1 A0 - state2
//------------------------------------------------------------------
void stopTimer0A0(){
    TA0CTL &= ~MC_1;
}
/*
//---------------------------------------------
//          turn off State1 Timer
//--------------------------------------------
void state1TimerOff(){
    TA1CTL = ~(TASSEL_2 + MC_2 + TAIE);
}*/
//------------------------------------------------------------------
//              shutdown ADC
//------------------------------------------------------------------
/*void ADCoff(){
    ADC10CTL0 &= ~ADC10ON;
}*/

//--------------------------------------------------------------------
//                 start Timer1 A0 -state1
//--------------------------------------------------------------------
/*void  startTimer1A0(){
        TA1R=0x00;
        TA1CTL &= ~TAIFG;
        TA1CTL = TASSEL_2 + MC_2 + TAIE;
        __bis_SR_register(LPM0_bits+GIE);
}*/
    //------------------------------------------
    //          set DMA1 source - state3
    //------------------------------------------
/*    void setDMA1src(int source){
        DMA1SA=source;
    }
    //------------------------------------------
    //          enable DMA1 - state3
    //------------------------------------------
    void enableDMA1(){
        DMA1CTL |= DMAEN;
    }*/
//-----------------------------------------------
/* - - - - - - - LCD interface - - - - - - - - -
//-----------------------------------------------
 *  This code will interface to a standard LCD controller
 *  It uses it in 4 or 8 bit mode.
 */
//#include "msp430g2553.h"


//******************************************************************
// send a command to the LCD
//******************************************************************
void lcd_cmd(unsigned char c){

    LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h

    if (LCD_MODE == FOURBIT_MODE)
    {
        LCD_DATA_WRITE &= ~OUTPUT_DATA;// clear bits before new write
        LCD_DATA_WRITE |= ((c >> 4) & 0x0F) << LCD_DATA_OFFSET;
        lcd_strobe();
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
        LCD_DATA_WRITE |= (c & (0x0F)) << LCD_DATA_OFFSET;
        lcd_strobe();
    }
    else
    {
        LCD_DATA_WRITE = c;
        lcd_strobe();
    }
}
//******************************************************************
// send data to the LCD
//******************************************************************
void lcd_data(unsigned char c){

    LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h

    LCD_DATA_WRITE &= ~OUTPUT_DATA;
    LCD_RS(1);
    if (LCD_MODE == FOURBIT_MODE)
    {
            LCD_DATA_WRITE &= ~OUTPUT_DATA;
                LCD_DATA_WRITE |= ((c >> 4) & 0x0F) << LCD_DATA_OFFSET;
        lcd_strobe();
                LCD_DATA_WRITE &= (0xF0 << LCD_DATA_OFFSET) | (0xF0 >> 8 - LCD_DATA_OFFSET);
                LCD_DATA_WRITE &= ~OUTPUT_DATA;
        LCD_DATA_WRITE |= (c & 0x0F) << LCD_DATA_OFFSET;
        lcd_strobe();
    }
    else
    {
        LCD_DATA_WRITE = c;
        lcd_strobe();
    }
    DelayMs(100);//--------------------dif
    LCD_RS(0);
}
//******************************************************************
// write a string of chars to the LCD
//******************************************************************
void lcd_puts(const char * s,int size){
  int lineCount=0;
  int updown=0;
  while(size){
        size--;
        lineCount++;
        if (lineCount>16){
            lineCount=0;
            if(updown == 0){
                updown=1;
                lcd_cmd(0xC0);
            }else if(updown == 1){
                updown=0;
                lcd_cmd(0x02);
            }

        }
        /*if (*s=="NUL"){
            *s=" ";
        }*/
        lcd_data(*s++);
  }
}
//******************************************************************
// write a Matrix of chars to the LCD
//******************************************************************
void lcd_puts_data_matrix(){
    int i;
    for (i = 0; i < 10; i++) {
        lcd_puts(data_matrix[i], 37 - 1);  // N-1 because we don't need to send the null terminator to lcd_puts
        lcd_cmd(0x02);
        }
}
//******************************************************************
// initialize the LCD
//******************************************************************
void lcd_init(){

    char init_value;

    if (LCD_MODE == FOURBIT_MODE) init_value = 0x3 << LCD_DATA_OFFSET;
        else init_value = 0x3F;

    LCD_RS_DIR(OUTPUT_PIN);
    LCD_EN_DIR(OUTPUT_PIN);
    LCD_RW_DIR(OUTPUT_PIN);
//    LCD_DATA_DIR |= OUTPUT_DATA;
    LCD_RS(0);
    LCD_EN(0);
    LCD_RW(0);

    DelayMs(15);
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
    LCD_DATA_WRITE |= init_value;
    lcd_strobe();
    DelayMs(5);
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
    LCD_DATA_WRITE |= init_value;
    lcd_strobe();
    DelayUs(200);
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
    LCD_DATA_WRITE |= init_value;
    lcd_strobe();

    if (LCD_MODE == FOURBIT_MODE){
        LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h
                LCD_DATA_WRITE &= ~OUTPUT_DATA;
        LCD_DATA_WRITE |= 0x2 << LCD_DATA_OFFSET; // Set 4-bit mode
        lcd_strobe();
        lcd_cmd(0x28); // Function Set
    }
        else lcd_cmd(0x3C); // 8bit,two lines,5x10 dots

    lcd_cmd(0xF); //Display On, Cursor On, Cursor Blink
    lcd_cmd(0x1); //Display Clear
    lcd_cmd(0x6); //Entry Mode
    lcd_cmd(0x80); //Initialize DDRAM address to zero
}
//******************************************************************
// Delay usec functions
//******************************************************************
void DelayUs(unsigned int cnt){

    unsigned char i;
        for(i=cnt ; i>0 ; i--) __delay_cycles(1); // tha command asm("NOP") takes raphly 1usec

}
//******************************************************************
// Delay msec functions
//******************************************************************
void DelayMs(unsigned int cnt){

    unsigned char i;
        for(i=cnt ; i>0 ; i--) DelayUs(1000); // tha command asm("NOP") takes raphly 1usec

}
//******************************************************************
// lcd strobe functions
//******************************************************************
void lcd_strobe(){
  LCD_EN(1);
  __delay_cycles(1);
  __delay_cycles(1);
  LCD_EN(0);
}


//---------------------------------------------------------------------
//            Polling based Delay function
//---------------------------------------------------------------------
void delay(unsigned int t){  //
    volatile unsigned int i;

    for(i=t; i>0; i--);
}

//---------------------------------------------------------------------
//            Enter from LPM0 mode
//---------------------------------------------------------------------
void enterLPM(unsigned char LPM_level){
    if (LPM_level == 0x00)
      _BIS_SR(LPM0_bits);     /* Enter Low Power Mode 0 */
        else if(LPM_level == 0x01)
      _BIS_SR(LPM1_bits);     /* Enter Low Power Mode 1 */
        else if(LPM_level == 0x02)
      _BIS_SR(LPM2_bits);     /* Enter Low Power Mode 2 */
    else if(LPM_level == 0x03)
      _BIS_SR(LPM3_bits);     /* Enter Low Power Mode 3 */
        else if(LPM_level == 0x04)
      _BIS_SR(LPM4_bits);     /* Enter Low Power Mode 4 */
}
//---------------------------------------------------------------------
//            Enable interrupts
//---------------------------------------------------------------------
void enable_interrupts(){
  _BIS_SR(GIE);
}
//---------------------------------------------------------------------
//            Disable interrupts
//---------------------------------------------------------------------
void disable_interrupts(){
  _BIC_SR(GIE);
}
//*********************************************************************
//            TimerA0 Interrupt Service Routine - 1 sec counter
//*********************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
    if (state==state3){
            if (th_x_ind==th_x){
                state3Flag=0;
                state3ind++;
            }
            if (state3ind==7){
                state3ind=0;
            }
            state3FlagThou=0;

    }
    TA0CTL &= ~MC_1;
    LPM0_EXIT;

}
//*********************************************************************
//            Timer1A0 Interrupt Service Routine - 1 sec counter
//*********************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{

    TA1CTL &= ~MC_1;
    TA1CCTL0 &= ~CCIFG;
    LPM0_EXIT;

}

//*********************************************************************
//            ADC10 Vector Interrupt Service Routine
//*********************************************************************
/*#pragma vector = ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
    __bic_SR_register_on_exit(CPUOFF);
    ADC10CTL0 &= ~ADC10IFG;
}*/
//*********************************************************************
//            LED clear
//*********************************************************************
/*void Clear_LEDs(){
    LEDsArrPort=0x00;
}
*/
//*********************************************************************
//            DMA interrupt service routine
//*********************************************************************
/*#pragma vector = DMA_VECTOR
  __interrupt void DMA_ISR(void)
  {
      DMA0CTL &= ~DMAIFG;
      DMA1CTL &= ~DMAIFG;
      __bic_SR_register_on_exit(LPM0_bits);       // Exit LPMx, interrupts enabled
  }
*/
//********************************************************************
//            Port2 Interrupt Service Rotine
//*********************************************************************
/*#pragma vector=PORT2_VECTOR
  __interrupt void Keypad_handler(void){
	delay(300);

	if(state==state1){
	    stopTimer0A0();
        if (KeypadCounter==0){
            lcd_cmd(0x01);
        }else if (KeypadCounter==16){
            lcd_cmd(0xc0);
        }
        if (KeypadCounter<32){
            if(KeypadIRQIntPend & BIT1){    // if keypad has been pressed find value
                KB = -0x09;
                if(LastRowPressesCounter % 3==0){
                    KeypadPortOUT = 0x0E;
                    if ( ( KeypadPortIN & 0x10 ) == 0 )  KB = 22;// F
                    else if ( ( KeypadPortIN & 0x20 ) == 0 )  KB = 18; //B
                    else if ( ( KeypadPortIN & 0x40 ) == 0 )  KB = 0; //0
                    else if ( ( KeypadPortIN & 0x80 ) == 0 )  KB = 17; //A
                }
                else if (LastRowPressesCounter % 3==1){
                    KeypadPortOUT = 0x0E;
                    if ( ( KeypadPortIN & 0x10 ) == 0 )  KB = 41;// Y
                    else if ( ( KeypadPortIN & 0x20 ) == 0 )  KB = 39; //W
                    else if ( ( KeypadPortIN & 0x40 ) == 0 )  KB = 37; //U
                    else if ( ( KeypadPortIN & 0x80 ) == 0 )  KB = 35; //S
                }
                else if (LastRowPressesCounter % 3==2){
                    KeypadPortOUT = 0x0E;
                    if ( ( KeypadPortIN & 0x10 ) == 0 )  KB = 42;// Z
                    else if ( ( KeypadPortIN & 0x20 ) == 0 )  KB = 40; //X
                    else if ( ( KeypadPortIN & 0x40 ) == 0 )  KB = 38; //V
                    else if ( ( KeypadPortIN & 0x80 ) == 0 )  KB = 36; //T
                }

                if(FirstRowsPressesCounter % 2==0){
                    KeypadPortOUT = 0x0D;
                    if ( ( KeypadPortIN & 0x10 ) == 0 )  KB = 21;   //E
                    else if ( ( KeypadPortIN & 0x20 ) == 0 )  KB = 9;//9
                    else if ( ( KeypadPortIN & 0x40 ) == 0 )  KB = 8;//8
                    else if ( ( KeypadPortIN & 0x80 ) == 0 )  KB = 7;//7
    
                    KeypadPortOUT = 0x0B;
                    if ( ( KeypadPortIN & 0x10 ) == 0 )  KB = 20;   //D
                    else if ( ( KeypadPortIN & 0x20 ) == 0 )  KB = 6;//6
                    else if ( ( KeypadPortIN & 0x40 ) == 0 )  KB = 5;//5
                    else if ( ( KeypadPortIN & 0x80 ) == 0 )  KB = 4;//4

                    KeypadPortOUT = 0x07;
                    if ( ( KeypadPortIN & 0x10 ) == 0 )  KB = 19;//C
                    else if ( ( KeypadPortIN & 0x20 ) == 0 )  KB = 3;//3
                    else if ( ( KeypadPortIN & 0x40 ) == 0 )  KB = 2;//2
                    else if ( ( KeypadPortIN & 0x80 ) == 0 )  KB = 1;//1
                }
                else if(FirstRowsPressesCounter % 2==1){
                    KeypadPortOUT = 0x0D;
                    if ( ( KeypadPortIN & 0x10 ) == 0 )  KB = 34;   //R
                    else if ( ( KeypadPortIN & 0x20 ) == 0 )  KB = 33;//Q
                    else if ( ( KeypadPortIN & 0x40 ) == 0 )  KB = 32;//P
                    else if ( ( KeypadPortIN & 0x80 ) == 0 )  KB = 31;//O

                    KeypadPortOUT = 0x0B;
                    if ( ( KeypadPortIN & 0x10 ) == 0 )  KB = 30;   //N
                    else if ( ( KeypadPortIN & 0x20 ) == 0 )  KB = 29;//M
                    else if ( ( KeypadPortIN & 0x40 ) == 0 )  KB = 28;//L
                    else if ( ( KeypadPortIN & 0x80 ) == 0 )  KB = 27;//K

                    KeypadPortOUT = 0x07;
                    if ( ( KeypadPortIN & 0x10 ) == 0 )  KB = 26;//J
                    else if ( ( KeypadPortIN & 0x20 ) == 0 )  KB = 25;//I
                    else if ( ( KeypadPortIN & 0x40 ) == 0 )  KB = 24;//H
                    else if ( ( KeypadPortIN & 0x80 ) == 0 )  KB = 23;//G
                }

                -
                FirstRowsPressesCounter++;
                LastRowPressesCounter++;
                LPM0_EXIT; // must be called from ISR only
                }
        }else if (KeypadCounter==32){
            KeypadCounter=0;
        }
	}
    if((state==state2)||(state==state4)){
          if(KeypadIRQIntPend & BIT1){    // if keypad has been pressed find value
              KA = 75;
              KeypadPortOUT = 0x0E;
              if ( ( KeypadPortIN & 0x10 ) == 0 )  KA = 13;
              else if ( ( KeypadPortIN & 0x20 ) == 0 )  KA = 14;
              else if ( ( KeypadPortIN & 0x40 ) == 0 )  KA = 0;
              else if ( ( KeypadPortIN & 0x80 ) == 0 )  KA = 15;

              KeypadPortOUT = 0x0D;
              if ( ( KeypadPortIN & 0x10 ) == 0 )  KA = 12;
              else if ( ( KeypadPortIN & 0x20 ) == 0 )  KA = 9;
              else if ( ( KeypadPortIN & 0x40 ) == 0 )  KA = 8;
              else if ( ( KeypadPortIN & 0x80 ) == 0 )  KA = 7;

              KeypadPortOUT = 0x0B;
              if ( ( KeypadPortIN & 0x10 ) == 0 )  KA = 11;
              else if ( ( KeypadPortIN & 0x20 ) == 0 )  KA = 6;
              else if ( ( KeypadPortIN & 0x40 ) == 0 )  KA = 5;
              else if ( ( KeypadPortIN & 0x80 ) == 0 )  KA = 4;

              KeypadPortOUT = 0x07;
              if ( ( KeypadPortIN & 0x10 ) == 0 )  KA = 10;
              else if ( ( KeypadPortIN & 0x20 ) == 0 )  KA = 3;
              else if ( ( KeypadPortIN & 0x40 ) == 0 )  KA = 2;
              else if ( ( KeypadPortIN & 0x80 ) == 0 )  KA = 1;

              LPM0_EXIT; // must be called from ISR only
          }
    }
    KeypadIRQIntPend &= ~0xFF; // Reset Flag
  }*/
//*********************************************************************
//            ADC10 Vector Interrupt Service Routine
//*********************************************************************
#pragma vector = ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
    __bic_SR_register_on_exit(CPUOFF);
    ADC10CTL0 &= ~ADC10IFG;
}



//---------------------------------------------------------------------
//            USCI A0/B0 Receive ISR
//---------------------------------------------------------------------
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCI0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
  input[input_slot] = UCA0RXBUF;
  input_slot++;
  if (input[input_slot-1] == '\n')
  {
      if(state_or_set_x){                     //state mode
          switch(input[0]){
              case '1' :
                  state = state1;
                  break;
              case '2' :
                  state = state2;
                  break;
              case '3' :
                  state = state3;
                  break;
              case '4' :
                  state = state4;
                  break;
              case '5' :
                  state = state5;
                  break;
              case '6' :
                  state = state6;
                  break;
              case '7' :
                  state = state0;
                  break;
              case '8' :
                  state = state0;
                  break;
              default:
                  break;
          }
      }
      else{                                   // set x
          x = atoi(input);
          state_or_set_x = 1;
          if (state==state4){
              state=state0;
          }
      }
      input_slot = 0;
//    IE2 |= UCA0TXIE;                        // Enable USCI_A0 TX interrupt
//---------------------------------------------------------------------
//            Exit from a given LPM
//---------------------------------------------------------------------
      switch(lpm_mode){
          case mode0:
           LPM0_EXIT; // must be called from ISR only
           break;

          case mode1:
           LPM1_EXIT; // must be called from ISR only
           break;

          case mode2:
           LPM2_EXIT; // must be called from ISR only
           break;

                  case mode3:
           LPM3_EXIT; // must be called from ISR only
           break;

                  case mode4:
           LPM4_EXIT; // must be called from ISR only
           break;
      }
  }
}

/*
//********************************************************************
//            Port1 Interrupt Service Rotine
//*********************************************************************
#pragma vector=PORT1_VECTOR
  __interrupt void PBs_handler(void){
   
	delay(7500);
//        TA1CTL &= ~MC_2;
//        TA1CTL &= ~MC_1;
//        TA0CTL &= ~MC_3;
//---------------------------------------------------------------------
//            selector of transition between states
//---------------------------------------------------------------------
    if (PBsArrIntPend & PB0) {
//        state = state1;
        PBsArrIntPend &= ~PB0;  // Clear PB0 interrupt flag
        state = state1;
    }
    else if (PBsArrIntPend & PB1) {
        state = state2;
        PBsArrIntPend &= ~PB1;  // Clear PB1 interrupt flag
    }
    else if (PBsArrIntPend & PB2) {
        state = state3;
        PBsArrIntPend &= ~PB2;  // Clear PB2 interrupt flag
    }else if (PBsArrIntPend & PB3){
        state = state4;
        PBsArrIntPend &= ~PB3;  // Clear PB3 interrupt flag
    }

//---------------------------------------------------------------------
//            Exit from a given LPM 
//---------------------------------------------------------------------	
        switch(lpm_mode){
		case mode0:
		 LPM0_EXIT; // must be called from ISR only
		 break;
		 
		case mode1:
		 LPM1_EXIT; // must be called from ISR only
		 break;
		 
		case mode2:
		 LPM2_EXIT; // must be called from ISR only
		 break;
                 
                case mode3:
		 LPM3_EXIT; // must be called from ISR only
		 break;
                 
                case mode4:
		 LPM4_EXIT; // must be called from ISR only
		 break;
	}
        
}

*/
