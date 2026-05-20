
/*
 * tb6600.c
 *
 *  Created on: May 6, 2026
 *      Author: you3e
 */

#include "tb6600.h"


/*
 * Description :
 * Set up required pins for drivers as output and enable the drivers
 */
void TB6600_init(){
	GPIO_setupPinDirection(DRIVER1_PULSE_PORT, DRIVER1_PULSE_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(DRIVER1_DIR_PORT, DRIVER1_DIR_PIN, PIN_OUTPUT);

	//Set direction pin for driver 1 to HIGH
	GPIO_writePin(DRIVER1_DIR_PORT, DRIVER1_DIR_PIN, LOGIC_HIGH);

	GPIO_setupPinDirection(DRIVER2_PULSE_PORT, DRIVER2_PULSE_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(DRIVER2_DIR_PORT, DRIVER2_DIR_PIN, PIN_OUTPUT);

	//Set direction pin for driver 2 to HIGH
	GPIO_writePin(DRIVER2_DIR_PORT, DRIVER2_DIR_PIN, LOGIC_LOW);

	GPIO_setupPinDirection(DRIVER3_PULSE_PORT, DRIVER3_PULSE_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(DRIVER3_DIR_PORT, DRIVER3_DIR_PIN, PIN_OUTPUT);

	//Set direction pin for driver 3 to HIGH
	GPIO_writePin(DRIVER3_DIR_PORT, DRIVER3_DIR_PIN, LOGIC_LOW);

	//Enable all drivers using LOGIC LOW
	GPIO_setupPinDirection(DRIVERS_EN_PORT, DRIVERS_EN_PIN, PIN_OUTPUT);
	GPIO_writePin(DRIVERS_EN_PORT, DRIVERS_EN_PIN, LOGIC_LOW);
}
/*
 * Description:
 * Reverses motor direction by Toggling DIR+ pin
 */
void TB6600_reverse(){

	//Select the DIR of the driver to be reversed
	GPIO_togglePin(DRIVER1_DIR_PORT, DRIVER1_DIR_PIN);

}
