/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.8 --- 9/26/2016 13:38:44 PST
*/

#include "rims.h"

char arrayPattern[10] = {0xFF, 0xF2, 0xE1, 0x55, 0x01, 0x83, 0x0A, 0xA2, 0xC0, 0xAA}; 
unsigned char SM1_Clk;
void TimerISR() {
   SM1_Clk = 1;
}

enum SM1_States { SM1_Pattern_1, SM1_Pattern_2, SM1_Pattern_3, SM1_Pattern_4, SM1_Pattern_5, SM1_Pattern_6, SM1_Pattern_8, SM1_Pattern_9, SM1_Pattern_10, SM1_Pattern_7 } SM1_State;

TickFct_State_machine_1() {
   switch(SM1_State) { // Transitions
      case -1:
         SM1_State = SM1_Pattern_1;
         break;
         case SM1_Pattern_1: 
         if (!A0) {
            SM1_State = SM1_Pattern_2;
         }
         else if (A0) {
            SM1_State = SM1_Pattern_10;
         }
         break;
      case SM1_Pattern_2: 
         if (!A0) {
            SM1_State = SM1_Pattern_3;
         }
         else if (A0) {
            SM1_State = SM1_Pattern_1;
         }
         break;
      case SM1_Pattern_3: 
         if (A0) {
            SM1_State = SM1_Pattern_2;
         }
         else if (!A0) {
            SM1_State = SM1_Pattern_4;
         }
         break;
      case SM1_Pattern_4: 
         if (A0) {
            SM1_State = SM1_Pattern_3;
         }
         else if (!A0) {
            SM1_State = SM1_Pattern_5;
         }
         break;
      case SM1_Pattern_5: 
         if (A0) {
            SM1_State = SM1_Pattern_4;
         }
         else if (!A0) {
            SM1_State = SM1_Pattern_6;
         }
         break;
      case SM1_Pattern_6: 
         if (!A0) {
            SM1_State = SM1_Pattern_7;
         }
         else if (A0) {
            SM1_State = SM1_Pattern_5;
         }
         break;
      case SM1_Pattern_8: 
         if (!A0) {
            SM1_State = SM1_Pattern_9;
         }
         else if (A0) {
            SM1_State = SM1_Pattern_7;
         }
         break;
      case SM1_Pattern_9: 
         if (!A0) {
            SM1_State = SM1_Pattern_10;
         }
         else if (A0) {
            SM1_State = SM1_Pattern_8;
         }
         break;
      case SM1_Pattern_10: 
         if (!A0) {
            SM1_State = SM1_Pattern_1;
         }
         else if (A0) {
            SM1_State = SM1_Pattern_9;
         }
         break;
      case SM1_Pattern_7: 
         if (!A0) {
            SM1_State = SM1_Pattern_8;
         }
         else if (A0) {
            SM1_State = SM1_Pattern_6;
         }
         break;
      default:
         SM1_State = SM1_Pattern_1;
   } // Transitions

   switch(SM1_State) { // State actions
      case SM1_Pattern_1:
         B = arrayPattern[0];
         break;
      case SM1_Pattern_2:
         B = arrayPattern[1];
         break;
      case SM1_Pattern_3:
         B = arrayPattern[2];
         break;
      case SM1_Pattern_4:
         B = arrayPattern[3];
         break;
      case SM1_Pattern_5:
         B = arrayPattern[4];
         break;
      case SM1_Pattern_6:
         B = arrayPattern[5];
         break;
      case SM1_Pattern_8:
         B = arrayPattern[7];
         break;
      case SM1_Pattern_9:
         B = arrayPattern[8];
         break;
      case SM1_Pattern_10:
         B = arrayPattern[9];
         break;
      case SM1_Pattern_7:
         B = arrayPattern[6];
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