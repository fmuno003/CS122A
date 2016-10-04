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
enum LEDState {OFF,ON} led_state;

void LEDS_Init()
{
	led_state = OFF;
}
void LEDS_Tick()
{
	//Actions
	switch(led_state)
	{
		case OFF:
			PORTD = 0x00;
			break;
		case ON:
			PORTD = 0x15;
			break;
		default:
			PORTD = 0x00;
			break;
	}
	//Transitions
	switch(led_state)
	{
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
void LedSecTask()
{
	LEDS_Init();
	for(;;)
	{
		LEDS_Tick();
		vTaskDelay(1000);
	}
}
void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}
int main(void)
{
	DDRA = 0x00; PORTA=0xFF;
	DDRD = 0xFF; 
	//Start Tasks
	StartSecPulse(1);
	//RunSchedular
	vTaskStartScheduler();
	
	return 0;
}