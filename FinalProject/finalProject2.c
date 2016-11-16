/* 	finalProject2.c - 11.16.2016
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

enum BluetoothModule{init, wait2, send} blueState;

void Bluetooth_Tick()
{
	switch(blueState) // transitions
	{
		
	}
	switch(blueState) //actions
	{
		
	}
}
void Bluetooth_Task()
{
	blueState = wait2;
	for(;;)
	{
		Bluetooth_Tick();
		vTaskDelay(100);
	}
}
void StartFinalPulse(unsigned portBASE_TYPE priority)
{
	xTaskCreate(Bluetooth_Task, (signed portCHAR *) "Bluetooth_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
}
int main(void)
{
	DDRD = 0x00; PORTD = 0x00;
	StartFinalPulse(1);
	vTaskStartScheduler();
}
