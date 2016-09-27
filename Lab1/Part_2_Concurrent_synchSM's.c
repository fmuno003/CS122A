/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.8 --- 9/26/2016 15:21:5 PST
*/

#include "rims.h"

char array[12] = {0x58, 0x24, 0x90, 0x78, 0xF4, 0xA0, 0x68, 0xC4, 0xF0, 0x28, 0xA4, 0xB0};
int i = 0;
char arrayA[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
int j = 0;
char arrayPattern[10] = {0xFF, 0xF2, 0xE1, 0x55, 0x01, 0x83, 0x0A, 0xA2, 0xC0, 0xAA}; 
int h = 0;
unsigned char TimerFlag = 0;
void TimerISR() {
   TimerFlag = 1;
}


enum SM1_States { SM1_s1, SM1_WAIT } SM1_State;

TickFct_Music_Accompaniment() {
   
   switch(SM1_State) { // Transitions
      case -1:
         SM1_State = SM1_WAIT;
         break;
      case SM1_s1:
         if (1) {
            SM1_State = SM1_s1;
            if ( i + 1 == 12) {i = 0;}
else { i = i + 1;}
         }
         else if (A5) {
            SM1_State = SM1_WAIT;
         }
         break;
      case SM1_WAIT:
         if (A7 && !A6 && !A5) {
            SM1_State = SM1_s1;
         }
         break;
      default:
         SM1_State = SM1_WAIT;
      } // Transitions

   switch(SM1_State) { // State actions
      case SM1_s1:
         B = array[i];
         break;
      case SM1_WAIT:
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}

enum SM2_States { SM2_WAIT, SM2_s2 } SM2_State;

TickFct_Bouncing_LED() {
   
   switch(SM2_State) { // Transitions
      case -1:
         SM2_State = SM2_WAIT;
         break;
      case SM2_WAIT:
         if (!A7 & !A6 && !A5) {
            SM2_State = SM2_s2;
         }
         break;
      case SM2_s2:
         if (A5) {
            SM2_State = SM2_WAIT;
         }
         else if (1) {
            SM2_State = SM2_s2;
            if(A == 0x01) 
{ if (j - 1 == -1){j = 7;}
else {j = j -1;}
}
else if(A == 0x00) 
{ if(j + 1 == 8) {j = 0;}
else {j =  j + 1;}
}
         }
         break;
      default:
         SM2_State = SM2_WAIT;
      } // Transitions

   switch(SM2_State) { // State actions
      case SM2_WAIT:
         break;
      case SM2_s2:
         B = arrayA[j];
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}

enum SM3_States { SM3_WAIT, SM3_Pattern } SM3_State;

TickFct_Pattern_Sequencer() {
   
   switch(SM3_State) { // Transitions
      case -1:
         SM3_State = SM3_WAIT;
         break;
      case SM3_WAIT:
         if (!A7 && A6 && !A5) {
            SM3_State = SM3_Pattern;
         }
         break;
      case SM3_Pattern:
         if (A5) {
            SM3_State = SM3_WAIT;
         }
         else if (1) {
            SM3_State = SM3_Pattern;
            if ((h + 1 == 10) && (!A0))
{h = 0;}
else if((h - 1 == -1) && (A0))
{h = 9;}
else if(!A0)
{h = h + 1;}
else
{h = h  - 1;}
         }
         break;
      default:
         SM3_State = SM3_WAIT;
      } // Transitions

   switch(SM3_State) { // State actions
      case SM3_WAIT:
         break;
      case SM3_Pattern:
         B = arrayPattern[h];
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}

enum SM4_States { SM4_WAIT, SM4_FULL } SM4_State;

TickFct_FULL() {
   
   switch(SM4_State) { // Transitions
      case -1:
         SM4_State = SM4_WAIT;
         break;
      case SM4_WAIT:
         if (A7 && A6 & !A5) {
            SM4_State = SM4_FULL;
         }
         break;
      case SM4_FULL:
         if (A5) {
            SM4_State = SM4_WAIT;
         }
         break;
      default:
         SM4_State = SM4_WAIT;
      } // Transitions

   switch(SM4_State) { // State actions
      case SM4_WAIT:
         break;
      case SM4_FULL:
         B = 0xFF;
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
   SM3_State = -1;
   SM4_State = -1;
   while(1) {
      TickFct_Music_Accompaniment();
      TickFct_Bouncing_LED();
      TickFct_Pattern_Sequencer();
      TickFct_FULL();
      while (!TimerFlag);
      TimerFlag = 0;
   }
}