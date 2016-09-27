# Lab 2 FreeRTOS

In this lab, you will learn how to use a real time operating system (RTOS) on your AVR microprocessor.


------------
Exercises:
------------

1. Blink three LEDs connected to PD0, PD2, and PD4 at a rate of 1000ms. Use only one task to complete this functionality.

2. From the previous lab we had you implement LEDs that blinked at adifferent rates. PD0 at a rate of 500ms, PD2 at a rate of 1000ms and PD4 at a rate of 2500ms. Implement the same functionality using the FreeRTOS library. Use multiple tasks to complete this functionality and make them visible in your code.

3. Now we want to implement a state machine design where the LEDs will cycle through each LED one after another. Once it reaches the last LED it will bounce and go in the opposite direction. (Try simplifying your desigsn by using the shift operator.)

4. (challenge) Expand upon exercise 3 by adding a button which will reverse the direction of the LED cycle whenever it is pressed. (Hint: Review priorites to implement this.)

5. (For fun - No credit) Implement two taskes that use the same LED and assign different priorities to them. Then play around with some implementations where you can see how priority matters. (You don't have to demo this.)
