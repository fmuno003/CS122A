/* 	final2.c - 11.16.2016
*	Name & E-mail: Francisco Munoz		fmuno003@ucr.edu
*	CS Login: fmuno003
*	Lab Section: 022
*	Assignment: Final Project
*	Exercise Description:
*
*	This code will contain all the source code for the second micro controller using the
*	bluetooth module that will send information to the simple phone application.
*
*	I acknowledge all content contained herein, excluding template or example code, is my
*	own original work.
*/
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspacce.h>
#include <util/atomic.h>
#include <util/delay>
#include <avr/io.h>

#include "FreeRTOS.h"
#include "tasks.h"
#include "croutine.h"
#include "usart_ATmega1284.h"

enum BluetoothModule{wait, send} blueState;
enum ReceiveData{receive} receiveState;

unsigned char temperature = 0x00;
unsigned char heartbeat = 0x00;

unsigned char receiveData()
{
	unsigned char temp = 0;
	while(1)
	{
		if(USART_HasReceived(0))
		{
			temp = USART_Receive(0);
			return temp;
		}
	}
}
void blueData(unsigned char x)
{
	unsigned char hasSent = 0;
	while(!hasSent)
	{
		if(USART_IsSendReady(1))
		{
			USART_Send(x, 1);
			hasSent = 1;
		}
	}
}
void Receive_Tick()
{
	switch(receiveState) // transitions
	{
		case receive:
			receiveState = receive;
			break;
		default:
			receiveState = receive;
			break;
	}
	switch(receiveState) // actions
	{
		case receive:
			temperature = receiveData();
			USART_Flush(0);
			heartbeat = receiveData();
			USART_Flush(0);
			break;
		default:
			break;
	}
}
void Bluetooth_Tick()
{
	switch(blueState) // transitions
	{
		case wait:
			if((temperature != 0x00) && (heartbeat != 0x00)) 
				blueState = send;
			else
				blueState = wait;
			break;
		case send:
			if((temperature == 0x00) || (heartbeat == 0x00))
				blueState = wait;
			else
				blueState = send;
			break;
		default:
			blueState = wait;
			break;
	}
	switch(blueState) //actions
	{
		case wait:
			break;
		case send:
			blueData(temperature);
			blueData(heartbeat);
			break;
		default:
			break;
	}
}
void Bluetooth_Task()
{
	blueState = wait;
	for(;;)
	{
		Bluetooth_Tick();
		vTaskDelay(100);
	}
}
void Receive_Task()
{
	receiveState = receive;
	for(;;)
	{
		Receive_Tick();
		vTaskDelay(100);
	}
}
void StartFinalPulse(unsigned portBASE_TYPE priority)
{
	xTaskCreate(Bluetooth_Task, (signed portCHAR *) "Bluetooth_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
	XTaskCreate(Receive_Task, (signed portCHAR *) "Receive_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
}
int main(void)
{
	initusart(0);
	initusart(1);
	// USART
	DDRD = 0xFF; PORTD = 0x00;
	
	StartFinalPulse(1);
	vTaskStartScheduler();
}
