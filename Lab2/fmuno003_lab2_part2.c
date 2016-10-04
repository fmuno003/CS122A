/* Partner(s) Name & E-mail: Francisco Munoz	fmuno003@ucr.edu
 * Lab Section: 022
 * Assignment: Lab 2  Exercise 2 
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
enum LEDState {OFF, ON} led_state;
enum LEDState2 {OFF2, ON2} led_state2;
enum LEDState3 {OFF3, ON3} led_state3;

void LEDS_Tick()
{
	//Actions
	switch(led_state){
		case OFF:
			PORTD = PORTD ^ (0x01);
			break;
		case ON:
			PORTD = PORTD | 0x01;
			break;
		default:
			PORTD = PORTD ^ (0x01);
			break;
	}
	//Transitions
	switch(led_state){
		case OFF:
			led_state = ON;
			break;
		case ON:
			led_state = OFF;
			break;
		default:
			led_state = OFF;
			break;
	}
}
void LEDS_Tick2()
{
	//Actions
	switch(led_state2){
		case OFF2:
			PORTD = PORTD ^ (0x04);
			break;
		case ON2:
			PORTD = PORTD | 0X04;
			break;
		default:
			PORTD = PORTD ^ (0x04);
			break;
	}
	//Transitions
	switch(led_state2){
		case OFF2:
			led_state2 = ON2;
			break;
		case ON2:
			led_state2 = OFF2;
			break;
		default:
			led_state2 = OFF2;
			break;
	}
}
void LEDS_Tick3()
{
	switch(led_state3){
		case OFF3:
			PORTD = PORTD ^ (0x10);
			break;
		case ON3:
			PORTD = PORTD | (0x10);
			break;
		default:
			PORTD = PORTD ^ (0x10);
			break;
	}
	//Transitions
	switch(led_state3){
		case OFF3:
			led_state3 = ON3;
			break;
		case ON3:
			led_state3 = OFF3;
			break;
		default:
			led_state3 = OFF3;
			break;
	}
}
void LedSecTask()
{
	led_state = OFF;
	while(1)
	{
		LEDS_Tick();
		vTaskDelay(500);
	}
}
void LedSecTask2()
{
	led_state2 = OFF2;
	while(1)
	{
		LEDS_Tick2();
		vTaskDelay(1000);
	}
}
void LedSecTask3()
{
	led_state3 = OFF3;
	while(1)
	{
		LEDS_Tick3();
		vTaskDelay(2500);
	}
}
void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(LedSecTask2, (signed portCHAR *)"LedSecTask2", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(LedSecTask3, (signed portCHAR *)"LedSecTask3", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRD = 0xFF; 
	//Start Tasks
	StartSecPulse(1);
	//RunSchedular
	vTaskStartScheduler();
	
	return 0;
}