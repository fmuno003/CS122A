/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.8 --- 9/26/2016 13:36:47 PST
*/

#include "rims.h"

/*Define user variables and functions for this state machine here.*/
unsigned char SM1_Clk;
void TimerISR() {
   SM1_Clk = 1;
}

enum SM1_States { SM1_B0, SM1_B1, SM1_B2, SM1_B3, SM1_B4, SM1_B5, SM1_B6, SM1_B7 } SM1_State;

TickFct_State_machine_1() {
   switch(SM1_State) { // Transitions
      case -1:
         SM1_State = SM1_B0;
         break;
         case SM1_B0: 
         if (!A0) {
            SM1_State = SM1_B1;
         }
         else if (A0) {
            SM1_State = SM1_B7;
         }
         break;
      case SM1_B1: 
         if (!A0) {
            SM1_State = SM1_B2;
         }
         else if (A0) {
            SM1_State = SM1_B0;
         }
         break;
      case SM1_B2: 
         if (!A0) {
            SM1_State = SM1_B3;
         }
         else if (A0) {
            SM1_State = SM1_B1;
         }
         break;
      case SM1_B3: 
         if (!A0) {
            SM1_State = SM1_B4;
         }
         else if (A0) {
            SM1_State = SM1_B2;
         }
         break;
      case SM1_B4: 
         if (!A0) {
            SM1_State = SM1_B5;
         }
         else if (A0) {
            SM1_State = SM1_B3;
         }
         break;
      case SM1_B5: 
         if (!A0) {
            SM1_State = SM1_B6;
         }
         else if (A0) {
            SM1_State = SM1_B4;
         }
         break;
      case SM1_B6: 
         if (!A0) {
            SM1_State = SM1_B7;
         }
         else if (A0) {
            SM1_State = SM1_B5;
         }
         break;
      case SM1_B7: 
         if (!A0) {
            SM1_State = SM1_B0;
         }
         else if (A0) {
            SM1_State = SM1_B6;
         }
         break;
      default:
         SM1_State = SM1_B0;
   } // Transitions

   switch(SM1_State) { // State actions
      case SM1_B0:
         B = 0x01;
         break;
      case SM1_B1:
         B = 0x02;
         break;
      case SM1_B2:
         B = 0x04;
         break;
      case SM1_B3:
         B = 0x08;
         break;
      case SM1_B4:
         B = 0x10;
         break;
      case SM1_B5:
         B = 0x20;
         break;
      case SM1_B6:
         B = 0x40;
         break;
      case SM1_B7:
         B = 0x80;
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