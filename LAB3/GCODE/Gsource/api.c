#include  "../header/api.h"    		// private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer
#include "stdio.h"


// Global Variables
//notes frequency: {1046.5, 1108.73,1174.66,1244.51,1318.51,1396.91,1479.98,1567.98,1661.22,1760.00,1864.66,1975.53};
//-------------------------------------------------------------
//                        Songs Notes
//-------------------------------------------------------------
int song1[] = {note2,note7,note7,note7,note2,note4,note4,note2,note11,note11,note9,note9,note7,note2,note7,note7,note7,note2,
         note4,note4,note2,note11,note11,note9,note9,note7,note2,note7,note7,note7,note2,note7,note7,note7,note7,note7,note7,note2,
         note7,note7,note7,note2,note7,note7,note7,note2,note4,note4,note2,note11,note11,note9,note9,note7}; //uncle moses

int song2[] = {note0,note5,note9,note0,note5,note9,note9,note7,note7,note7,note7,note9,note10,note10,note10,note12,note10,note9,note7,
         note9,note9,note9,note9,note5,note7,note9,note10,note10,note10,note12,note10,note10,note9,note5,note9,note12,note12,
         note0,note4,note7,note10,note9,note7,note5,note5,note5,note5}; // our car

int song3[] = {note4,note7,note4,note4,note7,note4,note12,note11,note9,note7,note5,note2,note5,note9,note7,
         note7,note5,note4,note2,note0,note4,note7,note4,note4,note7,note4,note12,note11,note9,note7,
         note5,note2,note5,note9,note7,note7,note5,note4,note2,note0}; // hands up
//-------------------------------------------------------------
int recorder[RecorderSize];
char index[2];
//-------------------------------------------------------------
//                        Recorder - Task 1
//-------------------------------------------------------------
void tone_recorder(){
    disable_interrupts();  // Disable GIE
    StopAllTimers();    // Stop All Timers(A,B,DMA)
    lcd_clear();
    lcd_home();
    // Check If recording was finished by user or by reaching notes limit
    if(EndOfRecord || i == 32){
        lcd_puts("Finished Rec.");
        lcd_new_line;
        lcd_puts("Total Notes: ");
        sprintf(index,"%d",i);
        lcd_puts(index);
        i = 0; // Reset i
        EndOfRecord = 0;
        state = state0;
    }
    // Check if pressing Key Pad performed incorrectly
    else if (KB == 75 || KB == 14 || KB == 13) {
        lcd_puts("Error");
        lcd_new_line;
        lcd_puts("Try Again!");
    }
    // Show how many notes left to record
    else{
        lcd_puts("Play");
        sprintf(index,"%d",RecorderSize-i);
        lcd_puts(index);
        lcd_puts("more notes");
    }

    enable_interrupts();  // Enable GIE
}
//-------------------------------------------------------------
//                     Audio Player - Task 2
//-------------------------------------------------------------
void audio_player(){
    disable_interrupts();
    StopAllTimers(); // Stop All Timers(A,B,DMA)

    //Init Timer A and Timer B
    TIMER_A0_config(); // For delay
    TIMERB_config();  // For PWM

    //Choose Selected Song according to the keypad number that was pressed
    switch (KB){
    case 1: lcd_reset(); DMA0SA = (int) &song1; DMA0SZ = Song1Size; DMA_config(); lcd_puts("Playing song1"); break;
    case 2: lcd_reset(); DMA0SA = (int) &song2; DMA0SZ = Song2Size; DMA_config(); lcd_puts("Playing song2"); break;
    case 3: lcd_reset(); DMA0SA = (int) &song3; DMA0SZ = Song3Size; DMA_config(); lcd_puts("Playing song3"); break;
    case 4: lcd_reset(); DMA0SA = (int) &recorder; DMA0SZ = RecorderSize; DMA_config(); lcd_puts("Playing recorder"); break;
    case 0: StopAllTimers(); break;
    }
    KB = 0; // Reset Keypad number
    enable_interrupts();
}


// Call LCD reset for printing which song we playing
void lcd_reset(){
    lcd_clear();
    lcd_home();
}
// Show Menu for task 1 after song is finished playing
void Show_Menu_LCD(){
    lcd_reset();
    lcd_puts("Choose song: ");
    lcd_new_line;
    lcd_puts("1. song1");
    scroll = 0;
}
