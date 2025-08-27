#include  "../header/bsp_430G2553.h"

//-----------------------------------------------------------------------------  
//           GPIO congiguration
//-----------------------------------------------------------------------------
void GPIOconfig(void){
 // volatile unsigned int i; // in case of while loop usage
  
  WDTCTL = WDTHOLD | WDTPW;		// Stop WDT
   
  if (CALBC1_1MHZ==0xFF)                    // If calibration constant erased
  {
    while(1);                               // do not load, trap CPU!!
  }
  DCOCTL = 0;                               // Select lowest DCOx and MODx settings
  BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
  DCOCTL = CALDCO_1MHZ;

/*
  // PushButtons Setup
  PBsArrPortSel &= ~0x07;
  PBsArrPortDir &= ~0x07;
  PBsArrIntEdgeSel |= 0x03;  	     // pull-up mode
  PBsArrIntEdgeSel &= ~0x04;         // pull-down mode
  PBsArrIntEn |= 0x07;
  PBsArrIntPend &= ~0x07;            // clear pending interrupts 
  
  */

  // RGB led Setup
  RGBArrPortOut        &= ~0x07;
  RGBArrPortSel        &= ~0x07;
  RGBArrPortDir        |= 0x07;

  // LDR Setup
  //LDRArrPortIN         &= ~0x08;
  LDRArrPortSel        &= ~0x08;
  LDRArrPortDir        &= ~0x08;
  
  //buzer Setup 
   BUZER_OUT           &= ~0x10;
   BUZER_DIR           |= 0x10;
   BUZER_SEL           |= 0x10;
   
  //LCD Setup 
   LCD_DATA_WRITE      &= ~0xFF;
   LCD_DATA_DIR        |= 0xF0;
   LCD_DATA_SEL        &= ~0xF0;
   LCD_CTL_SEL         &= ~0xE0;
  
  _BIS_SR(GIE);                     // enable interrupts globally
}                             
//------------------------------------------------------------------------------------- 
//            Timers congiguration 
//-------------------------------------------------------------------------------------
void TIMERconfig(void){
        
	TA0CTL = TASSEL_2+ TACLR +ID_3;  // timer is off SMCLK /8 = 0.5 sec interupt
    TA0CCTL0 = CCIE;    //interupt enable
    TA0CCR0 = 0xFFFF;      // value of 0.5 sec

    TA1CTL = TASSEL_2 +TACLR;        // smclk ,
    TA1CCTL2 = OUTMOD_0;             // for state3
//    TA1CCTL2 = CM_1 + SCS + CAP;     // for state1
} 
//------------------------------------------------------------------------------------- 
//            ADC congiguration 
//-------------------------------------------------------------------------------------
void ADCconfig(void){
	
	ADC10CTL0 &= ~0x0002;// enc off
    ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE;   //set ref 0-3v, sample rate= 8*(1/2^20)
    ADC10CTL1 = INCH_3+ADC10SSEL_3; //A3,SMCLK
    ADC10AE0 |= BIT3;
}              

//-------------------------------------------------------------------------------------
//            UART congiguration
//-------------------------------------------------------------------------------------
void UARTconfig(void){
    UARTArrPortSel       |= BIT1 + BIT2;
    UARTArrPortSel2      |= BIT1 + BIT2;
    //  UARTArrPortDir       |= BIT1 + BIT2; //RXLED + TXLED
    //  UARTArrPortOUT       &= ~(BIT1 + BIT2);
    UCA0CTL1             |= UCSSEL_2;
    UCA0BR0              = 104;
    UCA0BR1              = 0x00;
    UCA0MCTL             = UCBRS0;
    UCA0CTL1             &= ~UCSWRST;
    IE2                  |= UCA0RXIE;
}
             
             
            
  

