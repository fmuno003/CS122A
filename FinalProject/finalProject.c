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
unsigned short temperatureSensor = 0;
unsigned short pulseSensor = 0x0000;
float temperature = 0.0;
int heartbeat = 0;
int power = 0;

enum ReflectanceSensor {read} state;
enum PulseSensor{hold, process} pulseState;
enum TemperatureSensor{pause, readIn} tempState;
enum USARTCommunication{wait, send} usartState;
	
void ADC_init()
{
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: Enables analog-to-digital conversion
	// ADSC: Starts analog-to-digital conversion
	// ADATE: Enables auto-triggering, allowing for constant analog to digital conversions.
}
void sendData(unsigned char x)
{
	unsigned char hasSent = 0;
	while(!hasSent)
	{
		if(USART_IsSendReady(0))
		{
			USART_Send(x, 0);
			hasSent = 1;
		}
	}
}
int heartConversion(unsigned short pulseSensor)
{
	
}
float degreeConversion(unsigned short temperatureSensor)
{
	
}
char hexConversion(unsigned int heartbeat)
{
	// convert from int to hexadecimal
}
char hexaConversion(float temperature)
{
	// convert float to hexadecimal
}
void USART_Tick()
{
	switch(usartState)
	{
		case wait:
			if(power == 0)
				usartState = wait;
			else if(power == 1)
				usartState = send;
			break;
		case send:
			if(power == 1)
				usartState = send;
			else if(power == 0)
				usartState = wait;
			break;
		default:
			break;
	}
	switch(usartState)
	{
		case wait:
			break;
		case send:
			// convert then send data
			break;
		default: 
			break;
	}
}
void Reflectance_Tick()
{
	switch(state) //transitions
	{
		case read:
			state = read;
			break;
		default:
			state = read;
			break;
	}
	switch(state) // actions
	{
		case read:
			reflectSensor = ADC;
			if(reflectSensor == 0x0000)
				power = 0;
			else if(reflectSensor = 0x0000)
				power = 1;
			break;
		default:
			break;
	}
}
void Pulse_Tick()
{
	switch(pulseState) // transitions
	{
		case hold:
			if(power == 0)
				pulseState = hold;
			else if(power == 1)
				pulseState = process;
			break;
		case process:
			if(power == 1)
				pulseState = process;
			else if(power == 0)
				pulseState = hold;
			break;
		default:
			pulseState = hold;
			break;
	}
	switch(pulseState) // actions
	{
		case hold:
			break;
		case process:
			pulseSensor = ADC;
			heartbeat = heartConversion();
		default:
			break;
	}
}
void Temperature_Tick()
{
	switch(tempState) // transitions
	{
		case wait:
			if(power == 0)
				tempState = pause;
			else if (power == 1)
				tempState = readIn;
			break;
		case readIn:
			if(power == 1)
				tempState = readIn;
			else if(power == 0)
				tempState = pause;
		default:
			tempState = pause;
			break;
	}
	switch(tempState) // actions
	{
		case pause:
			break;
		case readIn:
			TemperatureSensor = ADC;
			temperature = degreeConversion(TemperatureSensor);
		default:
			break;
	}
}
void Reflectance_Task()
{
	state = read;
	for(;;)
	{
		Reflectance_Tick();
		vTaskDelay(100);
	}
}
void Pulse_Task()
{
	pulseState = hold;
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
void USART_Task()
{
	usartState = wait;
	for(;;)
	{
		USART_Tick();
		vTaskDelay(100);
	}
}
void StartFinalPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Reflectance_Task, (signed portCHAR *) "Reflectance_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
	xTaskCreate(Pulse_Task, signed portCHAR *) "Pulse_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
	xTaskCreate(Temperature_Task, signed portCHAR *) "Temperature_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
	xTaskCreate(USART_Task, signed portCHAR *) "USART_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
}
int main(void)
{
	ADC_init;
	// Temperature Sensor
	// Heart Rate Sensor
	// Reflectance Sensor
	DDRA = 0x00; PORTA = 0x00;
	
	//USART
	DDRD = 0x00; PORTD = 0x00;
	
	StartFinalPulse(1);
	vTaskStartScheduler();
}
