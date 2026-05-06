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

	GPIO_setupPinDirection(DRIVER2_PULSE_PORT, DRIVER2_PULSE_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(DRIVER2_DIR_PORT, DRIVER2_DIR_PIN, PIN_OUTPUT);

	GPIO_setupPinDirection(DRIVER3_PULSE_PORT, DRIVER3_PULSE_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(DRIVER3_DIR_PORT, DRIVER3_DIR_PIN, PIN_OUTPUT);

	GPIO_setupPinDirection(DRIVERS_EN_PORT, DRIVERS_EN_PIN, PIN_OUTPUT);
	GPIO_writePin(DRIVERS_EN_PORT, DRIVERS_EN_PIN, LOGIC_HIGH);
}
