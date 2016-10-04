/* Partner(s) Name & E-mail: Francisco Munoz	fmuno003@ucr.edu
 * Lab Section: 021
 * Assignment: Lab 2  Exercise 4
 * Exercise Description: [optional - include for your own benefit]
 * 
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/portpins.h>
#include <avr/pgmspace.h>

//FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
enum LEDState {shift_left, shift_right} led_state;
enum BTNState {not_pressed,btn_press,btn_hold} btn_state;

unsigned char GetBit(unsigned char x, unsigned char k)
{
	return ((x & (0x01 << k)) != 0);
}

unsigned char shift;
void readBtn_Tick(){
	//Actions
	switch(btn_state){
		case not_pressed:
			break;
		case btn_press:
			shift = 1;
			break;
		case btn_hold:
			shift = 0;
			break;
		default:
			break;
	}
	//Transitions
	switch(btn_state){
		case not_pressed:
			if(GetBit(PINA,0))
				btn_state = not_pressed;
			else
				btn_state = btn_press;
			break;
		case btn_press:
			btn_state = btn_hold;
			break;
		case btn_hold:
			if(GetBit(PINA,0))
			{
				btn_state = not_pressed;
				shift = 0;
			}
			else
			{
				btn_state = btn_hold;
				shift = 0;
			}
			//btn_state = not_pressed;
			break;
		default:
			btn_state = not_pressed;
			break;
	}
	
}
void LEDS_Tick(){
	//Actions
	switch(led_state){
		case shift_left:
			PORTD = PORTD << 1;
			break;
		case shift_right:
			PORTD = PORTD >> 1;
			break;
		default:
			PORTD = 0;
			break;
	}
	//Transitions
	switch(led_state){
		case shift_left:
			if(PORTD != 0x80 && !shift)
				led_state = shift_left;
			else if((PORTD == 0x80)||shift)
				led_state = shift_right;
			break;
		case shift_right:
			if(PORTD != 0x01 && !shift)
				led_state = shift_right;
			else if((PORTD == 0x01)||shift)
				led_state = shift_left;
			break;
		default:
			led_state = shift_left;
			break;
	}
}
void LedSecTask()
{
	led_state = shift_left;
	for(;;)
	{
		LEDS_Tick();
		vTaskDelay(1000);
	}
}
void BtnSecTask()
{
	shift = 0;
	btn_state = not_pressed;
	for(;;)
	{
		readBtn_Tick();
		vTaskDelay(500);
	}
}
void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(BtnSecTask, (signed portCHAR *)"BtnSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRD = 0xFF; PORTD = 0x01;
	//Start Tasks
	StartSecPulse(1);
	//RunSchedular
	vTaskStartScheduler();
	
	return 0;
}