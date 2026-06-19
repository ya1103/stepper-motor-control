/*
 * plc.c
 *
 *  Created on: Jun 19, 2026
 *      Author: you3e
 */

#include "plc.h"
#include "gpio.h"

/* this function sets up the required pins */
void PLC_driver_setup(void){
	GPIO_setupPinDirection(PLC_SIGNAL_PORT, PLC_SIGNAL_PIN, PIN_INPUT);

	//enable internal pull-up resistor
	GPIO_writePin(PLC_SIGNAL_PORT, PLC_SIGNAL_PIN, LOGIC_HIGH);
}

/* This function should read the targeted pin and return its state */
uint8  checkForPLC_signal(){
	return GPIO_readPin(PLC_SIGNAL_PORT, PLC_SIGNAL_PIN);
}


