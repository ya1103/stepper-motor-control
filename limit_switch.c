/*
 * limit_switch.c
 *
 *  Created on: May 19, 2026
 *      Author: you3e
 */


#include "limit_switch.h"
#include "common_macros.h"
#include "std_types.h"

/*
 * Description:
 * Sets up required pins of limit switch as required
 */
void LS_init(){
	// Configure LS1_NO as input
	GPIO_setupPinDirection(LS1_NO_PORT, LS1_NO_PIN, PIN_INPUT);
	//Enable internal pull-up
	GPIO_writePin(LS1_NO_PORT, LS1_NO_PIN, LOGIC_HIGH);

	// Configure LS2_NO as input
	GPIO_setupPinDirection(LS2_NO_PORT, LS2_NO_PIN, PIN_INPUT);
	//Enable internal pull-up
	GPIO_writePin(LS2_NO_PORT, LS2_NO_PIN, LOGIC_HIGH);
}

/*
 * Description:
 * Checks if limit switch 1 is triggered or not
 * and returns status
 */
boolean LS1_check(){
	boolean status;
	if( GPIO_readPin(LS1_NO_PORT, LS1_NO_PIN) == LS_TRIGGERED ){
		status = LS_TRIGGERED;
	} else{
		status = LS_UNTRIGGERED;
	}
	return status;
}

/*
 * Description:
 * Checks if limit switch 2 is triggered or not
 * and returns status
 */
boolean LS2_check(){
	boolean status;
	if( GPIO_readPin(LS2_NO_PORT, LS2_NO_PIN) == LS_TRIGGERED ){
		status = LS_TRIGGERED;
	} else{
		status = LS_UNTRIGGERED;
	}
	return status;
}
