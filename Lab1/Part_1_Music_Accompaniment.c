/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.8 --- 9/26/2016 14:8:17 PST
*/

#include "rims.h"

char array[12] = {0x58, 0x24, 0x90, 0x78, 0xF4, 0xA0, 0x68, 0xC4, 0xF0, 0x28, 0xA4, 0xB0};
i = 0;
unsigned char SM1_Clk;
void TimerISR() {
   SM1_Clk = 1;
}

enum SM1_States { SM1_s1 } SM1_State;

TickFct_State_machine_1() {
   switch(SM1_State) { // Transitions
      case -1:
         SM1_State = SM1_s1;
         break;
         case SM1_s1: 
         if (1) {
            SM1_State = SM1_s1;
            if ( i + 1 == 12)
            {
            i = 0;
            }
            else
            { 
            i = i + 1;
            }
         }
         break;
      default:
         SM1_State = SM1_s1;
   } // Transitions

   switch(SM1_State) { // State actions
      case SM1_s1:
         B = array[i];
         break;
      default: // ADD default behaviour below
      break;
   } // State actions

}

int main() {

   const unsigned int periodState_machine_1 = 1000; // 1000 ms default
   TimerSet(periodState_machine_1);
   TimerOn();
   
   SM1_State = -1; // Initial state
   B = 0; // Init outputs

   while(1) {
      TickFct_State_machine_1();
      while(!SM1_Clk);
      SM1_Clk = 0;
   } // while (1)
} // Main