/*	final.c - 11.14.2016
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
#include "task.h"
#include "croutine.h"
#include "usart_ATmega1284.h"

unsigned short reflectSensor, temperatureSensor, temperatureSensor2, pulseSensor = 0x0000;
unsigned char pulseRate;
int temperature = 0;
int heartbeat, heart, counter = 0;
int power = 0;
int i = 0;

enum ReflectanceSensor {read} state;
enum PulseSensor{hold, process} pulseState;
enum TemperatureSensor{wait, readIn} tempState;
enum USARTCommunication{wait1, send} usartState;
	
void ADC_init()
{
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: Enables analog-to-digital conversion
	// ADSC: Starts analog-to-digital conversion
	// ADATE: Enables auto-triggering, allowing for constant analog to digital conversions.
}
void convert()
{
	ADCSRA |=(1<<ADSC);//start ADC conversion
	while ( !(ADCSRA & (1<<ADIF)));//wait till ADC conversion
	
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
void Data(unsigned char x)
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
int heartConversion(int heartRate)
{
	int temp = 0;
	temp = heartRate * 12;
	return temp;
}
int degreeConversion(unsigned short temperatureSensor, unsigned short temperatureSensor2)
{
	float temp, temp2 = 0.0;
	int realTemp = 0;
	temp = (temperatureSensor * 500) / 1024;
	temp2 = (temperatureSensor2 * 500) / 1024;
	realTemp = (temp + temp2) / 2;
	return realTemp;
}
void Set_A2D_PIN(unsigned char pinNum)
{
	ADMUX = (pinNum <= 0x07) ? pinNum : ADMUX;
	// Allow channel to stabilize
	static unsigned char i = 0;
	for(i = 0; i < 15; ++i){asm("nop");}
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
			sendData(temperature);
			USART_Flush(0);
			Data(heart);
			USART_Flush(0);
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
			reflectSensor = ~PINB & 0x01;
			if(reflectSensor == 0x0000)
				power = 0;
			else if(reflectSensor != 0x0000)
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
			counter = 0;
			break;
		case process:
			Set_A2D_PIN(0x01);
			convert();
			pulseSensor = ADC;
			if(pulseSensor != 0x001B)
				PORTD = 0x80;
				++heartbeat;
			if(counter >= 5000){
				heart = heartConversion(heartbeat);
				PORTC = heart;
				heartbeat = 0;
				counter = 0;
			}
			break;
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
				tempState = wait;
			else if (power == 1)
				tempState = readIn;
			break;
		case readIn:
			if(power == 1)
				tempState = readIn;
			else if(power == 0)
				tempState = wait;
		default:
			tempState = wait;
			break;
	}
	switch(tempState) // actions
	{
		case wait:
			break;
		case readIn:
			Set_A2D_PIN(0x02);
			convert();
			temperatureSensor = ADC;
			Set_A2D_PIN(0x03);
			convert();
			temperatureSensor2 = ADC;
			temperature = degreeConversion(temperatureSensor, temperatureSensor2);
			break;
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
		vTaskDelay(500);
	}
}
void Pulse_Task()
{
	pulseState = hold;
	for(;;)
	{
		Pulse_Tick();
		vTaskDelay(1);
		++counter;
	}
}
void Temperature_Task()
{
	tempState = wait;
	for(;;)
	{
		Temperature_Tick();
		vTaskDelay(500);
	}
}
void USART_Task()
{
	usartState = wait;
	for(;;)
	{
		USART_Tick();
		vTaskDelay(500);
	}
}
void StartFinalPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Reflectance_Task, (signed portCHAR *) "Reflectance_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
	xTaskCreate(Pulse_Task, (signed portCHAR *) "Pulse_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
	xTaskCreate(Temperature_Task, (signed portCHAR *) "Temperature_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
	xTaskCreate(USART_Task, (signed portCHAR *) "USART_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
}
int main(void)
{
	initUSART(0);
	initUSART(1);
	ADC_init();
	// Temperature Sensor
	// Heart Rate Sensor
	DDRA = 0x00; PORTA = 0xFF;
	//Reflectance Sensor
	DDRB = 0x00; PORTB = 0xFF;
	//USART
	DDRD = 0xFF; PORTD = 0x00;
	
	StartFinalPulse(1);
	vTaskStartScheduler();
}
