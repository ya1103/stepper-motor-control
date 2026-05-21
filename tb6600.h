/*
 * tb6600.h
 *
 *  Created on: May 6, 2026
 *      Author: you3e
 */

#ifndef TB6600_H_
#define TB6600_H_

#include "gpio.h"
#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Enable pin for all drivers */
#define DRIVERS_EN_PORT		 PORTC_ID
#define DRIVERS_EN_PIN		 PIN3_ID

/* Driver 1 pulse and direction pins */
#define DRIVER1_PULSE_PORT   PORTB_ID
#define DRIVER1_PULSE_PIN    PIN3_ID
#define DRIVER1_DIR_PORT     PORTC_ID
#define DRIVER1_DIR_PIN      PIN0_ID

/* Driver 2 pulse and direction pins */
#define DRIVER2_PULSE_PORT	 PORTD_ID
#define DRIVER2_PULSE_PIN	 PIN5_ID
#define DRIVER2_DIR_PORT     PORTC_ID
#define DRIVER2_DIR_PIN      PIN1_ID

/* Driver 3 pulse and direction pins */
#define DRIVER3_PULSE_PORT   PORTD_ID
#define DRIVER3_PULSE_PIN    PIN7_ID
#define DRIVER3_DIR_PORT     PORTC_ID
#define DRIVER3_DIR_PIN      PIN2_ID

/* Driver definition for stepper motor direction */
#define CLOCKWISE			LOGIC_HIGH
#define ANTICLOCKWISE		LOGIC_LOW

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description:
 * Set up required pins for drivers as output and enable the drivers
 */
void TB6600_init();

/*
 * Description:
 * Reverses motor direction by Toggling DIR+ pin
 */
void TB6600_reverse();

#endif /* TB6600_H_ */
