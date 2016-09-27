/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.8 --- 9/26/2016 15:36:44 PST
*/

#include "rims.h"

char array[13] = {0x01, 0x04, 0x02, 0x08, 0x04, 0x10, 0x08, 0x20, 0x10, 0x40, 0x20, 0x80, 0x40};
int i = 0;
char line[16] = {0x01, 0x02, 0x04, 0x08, 0x80, 0x40, 0x20, 0x10, 0x80, 0x40, 0x20, 0x10, 0x01, 0x02, 0x04, 0x08};
int h = 0;
unsigned char TimerFlag = 0;
void TimerISR() {
   TimerFlag = 1;
}


enum SM1_States { SM1_WAIT, SM1_s2 } SM1_State;

TickFct_LINE() {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
   switch(SM1_State) { // Transitions
      case -1:
         SM1_State = SM1_WAIT;
         break;
      case SM1_WAIT:
         if (A6) {
            SM1_State = SM1_s2;
         }
         break;
      case SM1_s2:
         if (A5) {
            SM1_State = SM1_WAIT;
         }
         else if (1) {
            SM1_State = SM1_s2;
            if(!A0)
{if(h + 1 == 16) {h = 0;}
else {h += 1;}
}
else if(A0)
{if(h - 1 == -1) {h = 15;}
else {h -= 1;}
}
         }
         break;
      default:
         SM1_State = SM1_WAIT;
      } // Transitions

   switch(SM1_State) { // State actions
      case SM1_WAIT:
         break;
      case SM1_s2:
         B = line[h];
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}

enum SM2_States { SM2_WAIT, SM2_ALTERNATE } SM2_State;

TickFct_LED_MACHINE() {
   
   switch(SM2_State) { // Transitions
      case -1:
         SM2_State = SM2_WAIT;
         break;
      case SM2_WAIT:
         if (A7) {
            SM2_State = SM2_ALTERNATE;
         }
         break;
      case SM2_ALTERNATE:
         if (A5) {
            SM2_State = SM2_WAIT;
         }
         else if (1) {
            SM2_State = SM2_ALTERNATE;
            if(!A0)
{if( i + 1 == 12) {i = 0;}
else {i+=1;}}
else if(A0)
{if(i - 1 == -1) {i = 11;}
else{i-=1;}
}
         }
         break;
      default:
         SM2_State = SM2_WAIT;
      } // Transitions

   switch(SM2_State) { // State actions
      case SM2_WAIT:
         break;
      case SM2_ALTERNATE:
         B = array[i];
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}
int main() {
   B = 0; //Init outputs
   TimerSet(1000);
   TimerOn();
   SM1_State = -1;
   SM2_State = -1;
   while(1) {
      TickFct_LINE();
      TickFct_LED_MACHINE();
      while (!TimerFlag);
      TimerFlag = 0;
   }
}