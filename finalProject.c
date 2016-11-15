/*	finalProject.c - 11.14.2016
 *	Name & E-mail:  - Francisco Munoz		fmuno003@ucr.edu
 *	CS Login:  fmuno003
 *	Lab Section: 022
 *	Assignment: Final Project 
 *	Exercise Description:
 *
 *	This code will contain all the source code for the first micro controller using the sensors, such as the LM35 temperature sensors, 
 *	SEN-11574 Pulse sensor, and the IR sensors.
 *
 *	I acknowledge all content contained herein, excluding template or example 
 *	code, is my own original work.
 */ 

#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>
#include <util/delay.h>
#include <avr/io.h>

#include "FreeRTOS.h"
#include "tasks.h"
#include "croutine.h"

unsigned short reflectSensor = 0x0000;
int temperatureSensor = 0;
unsigned short pulseSensor = 0x0000;
float temperature = 0.0;

enum ReflectanceSensor {wait, read} state;
enum PulseSensor{wait1, read1} pulseState;
enum TemperatureSensor{pause, readIn, calculate} tempState;

void ADC_init()
{
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: Enables analog-to-digital conversion
	// ADSC: Starts analog-to-digital conversion
	// ADATE: Enables auto-triggering, allowing for constant analog to digital conversions.
}
float degreeConversion(int temperatureSensor, int selection)
{
	if(selection == 1) // Conversion to Fahrenheit
	{
		
	}
	else if(selection == 2) // Conversion to Celsius
	{
		
	}
	else if(selection == 3) // Conversion to Kelvin
	{
		
	}
}
void Reflectance_Tick()
{
	switch(state) //transitions
	{
		default:
			break;
	}
	switch(state) // actions
	{
		default:
			break;
	}
}
void Pulse_Tick()
{
	switch(pulseState) // transitions
	{
		default:
			break;
	}
	switch(pulseState) // actions
	{
		default:
			break;
	}
}
void Temperature_Tick()
{
	switch(tempState) // transitions
	{
		default:
			break;
	}
	switch(tempState) // actions
	{
		default:
			break;
	}
}
void Reflectance_Task()
{
	state = wait;
	for(;;)
	{
		Reflectance_Tick();
		vTaskDelay(100);
	}
}
void Pulse_Task()
{
	pulseState = wait1;
	for(;;)
	{
		Pulse_Tick();
		vTaskDelay(100);
	}
}
void Temperature_Task()
{
	tempState = pause;
	for(;;)
	{
		Temperature_Tick();
		vTaskDelay(100);
	}
}
void StartFinalPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Reflectance_Task, (signed portCHAR *) "Reflectance_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
	xTaskCreate(Pulse_Task, signed portCHAR *) "Pulse_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
	xTaskCreate(Temperature_Task, signed portCHAR *) "Temperature_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
}
int main(void)
{
	ADC_init;
	// Temperature Sensor
	DDRA = 0x00; PORTA = 0x00;
	// Heart Rate Sensor
	DDRB = 0x00; PORTB = 0x00;
	// Reflectance Sensor
	DDRC = 0x00; PORTC = 0x00;
	//USART
	DDRD = 0x00; PORTD = 0x00;
	
	StartFinalPulse(1);
	vTaskStartScheduler();
}