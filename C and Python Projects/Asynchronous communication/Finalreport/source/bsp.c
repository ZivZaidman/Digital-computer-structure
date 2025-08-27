#include  "../header/bsp.h"    // private library - BSP layer

//-----------------------------------------------------------------------------
//           GPIO configuration
//-----------------------------------------------------------------------------
void GPIOconfig(void){
 // volatile unsigned int i; // in case of while loop usage

  WDTCTL = WDTHOLD | WDTPW;     // Stop WDT

  // LCD configuration
  LCD_DATA_WRITE &= ~0xFF;
  LCD_DATA_DIR |= 0xF0;    // P1.4-P1.7 To Output('1')
  LCD_DATA_SEL &= ~0xF0;   // Bit clear P1.4-P1.7
  LCD_CTL_SEL  &= ~0xE0;   // Bit clear P2.5-P2.7

  // RGB led Setup
    RGBArrPortOut        &= ~0x07;
    RGBArrPortSel        &= ~0x07;
    RGBArrPortDir        |= 0x07;

  //LED configuration
/*  LEDsArrPortSel &= ~0xFF;            // GPIO capability
  LEDsArrPortDir |= 0xFF;             // output dir
  LEDsArrPort = 0x00;                 // clear all LEDs
*/

  // Generator Setup
  //From the table at CCIx p2.4
  //GenPortDir &=  ~BIT4;               // P2.4 Input Capture = '1'
  //GenPortSel |=  BIT4;              // P2.4 Select = '1'

  // Buzzer Setup
  BuzzPortDir |= BIT4;             // P2.4 Output compare - '1'
  BuzzPortSel &= ~BIT4;             // P2.4 Select = '1'
  BuzzPortOut &= ~BIT4;             // P2.4 out = '0'

  // PushButtons Setup
  PBsArrPortSel &= ~0x0F;           //
  PBsArrPortOut &= ~0x0F;            // Set P1Out to '0'
  PBsArrPortDir &= ~0x0F;            // P1.0-2 - Input ('0')
  //PBsArrPortDir |= 0x08;             // P1.3 - Output ('1')
  PBsArrIntEdgeSel |= 0x03;          // pull-up mode   P1.0-P1.1 - '1'
  PBsArrIntEdgeSel &= ~0x0C;         // pull-down mode  P1.2 - '0'
  PBsArrIntEn |= 0x0F;               // P1.0-2 - '1'
  PBsArrIntPend &= ~0xFF;            // clear pending interrupts P1.0-P1.3 all P1

  /*// Keypad Setup
    KeypadPortSel &= ~0xFF;
    KeypadPortDIR = 0x0F; //10.0-10.3 output, 10.4-10.7 input
    KeypadPortIN &= ~0xF0;
    KeypadPortOUT = 0x00; // CLR output

    // Keypad IRQ Setup
    KeypadIRQPortSel &= ~BIT1;
    KeypadIRQPortDir &= ~BIT1;             // P2.1 input
    KeypadIRQIntEdgeSel |= BIT1;         // pull-up mode  P2.1 - '1'
    KeypadIRQIntEn |= BIT1;               // P2.1 - '1'
    KeypadIRQIntPend &= ~0xFF;            // clear pending interrupts P2.1
*/
/*
  // switch Setup
   SWsArrPortSel &= ~0x09;           //
//   SWsArrIntREN |= 0x01;
   SWsArrPortOut &= ~0x01;            // Set P2Out to '0'
   SWsArrPortDir &= ~0x09;            // P2.0 - Input ('0')
   SWsArrIntEdgeSel &= ~0x09;         // pull-down mode  P2.0 - '0'
   SWsArrIntEn |= 0x09;               // P1.0-2 - '1'
   SWsArrIntPend &= ~0x09;            // clear pending interrupts P2.0

*/
  _BIS_SR(GIE);                     // enable interrupts globally
}
//---------------------------------------------------------------
//                    DMA configuration
//---------------------------------------------------------------
/*void DMA1config(){
    DMACTL0 = DMA0TSEL_0+DMA1TSEL_8+DMA2TSEL_0;// start DMA transfer with TBCCR0_CCIFG bit
    DMA1CTL = DMADT_1+DMASBDB+DMAIE;// Block trunsfer,
    // dest and src type: byte, enable interrupt
    DMA0CTL = DMADT_1 + DMASBDB + DMADSTINCR_3 + DMASRCINCR_3; // block mode , byte-to-byte , enable
    DMA1SZ=0x0001;
    //DMA0SZ=38;
    DMA1DA = (unsigned int)&LEDsArrPort;
    DMA2CTL = DMADT_1 + DMASBDB + DMADSTINCR_3 + DMASRCINCR_3; // block mode , byte-to-byte , enable
}*/
//--------------------------------------------------------------
//                 UART configuration
//--------------------------------------------------------------
void UARTConfig(void){
    WDTCTL = WDTHOLD | WDTPW;        // Stop WDT

    if (CALBC1_1MHZ==0xFF)                    // If calibration constant erased
    {
//      while(1);                               // do not load, trap CPU!!
    }
    DCOCTL = 0;                               // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
    DCOCTL = CALDCO_1MHZ;
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

//------------------------------------------------------------
//              Timer0 A0 configuration - state1
//-------------------------------------------------------------
void TIMER0_A0_config(void){
    WDTCTL = WDTPW + WDTHOLD;
    // Stop WDT
//    TA0CCTL0 = CCIE;
    TA0CTL = TASSEL_2 + MC_0 + ID_3;
    //  select: 2 - SMCLK ; control: 3 - Up/Down  ; divider: 3 - /8
    /*
     * F(SMCLK)= 2^20 Hz
     * F(SMCLK)/divider= 2^17 Hz
     * count to 0xFFFF up/down in seconds: F(SMLCK)/0xFFFF*2 = 1sec
     */

}
//-----------------------------------------------------
//              Timer1 A0 configuration - state3
//-----------------------------------------------------
void TIMER1_A0_config(void){
    WDTCTL = WDTPW + WDTHOLD;
    // Stop WDT
//    TA0CCTL0 = CCIE;
    TA1CTL = TASSEL_2 + MC_0 + ID_3;
    //  select: 2 - SMCLK ; control: 3 - Up/Down  ; divider: 3 - /8
    /*
     * F(SMCLK)= 2^20 Hz
     * F(SMCLK)/divider= 2^17 Hz
     * count to 0xFFFF up/down in seconds: F(SMLCK)/0xFFFF*2 = 1sec
     */

}
//-------------------------------------------------------------------------------------
//            ADC configuration
//-------------------------------------------------------------------------------------
void ADCconfig(void){
      ADC10CTL0 = ADC10SHT_2 + SREF_0;          // 16*ADCLK+ Turn on, set ref to Vcc and Gnd, and Enable Interrupt
      ADC10CTL1 = INCH_3 + ADC10SSEL_3;         // Input A3 and SMCLK, was |
      ADC10AE0 |= BIT3;                         // P1.3 ADC option select
}
