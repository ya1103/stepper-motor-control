/*
 * potentiometer.h
 *
 *  Created on: Jun 17, 2026
 *      Author: you3e
 */

#ifndef POTENTIOMETER_H_
#define POTENTIOMETER_H_


#include "std_types.h"
#include "adc.h"
#include "gpio.h"

/****************** Definitions ********************/

#define POT1_CHANNEL_ID							0
#define POT2_CHANNEL_ID							1
#define POT3_CHANNEL_ID							2
#define POT_PORT_ID								PORTA_ID
#define POTENTIOMETER_MAX_VOLT_VALUE			5
#define STEPPER_MOTOR_MAX_RPM					50
#define STEPPER_MOTOR_MIN_RPM   				10

/****************** Function Prototype ************/

/* Reads the analog value of potentiometer 1 and returns RPM equivalent value */
uint8 POT1_getValue(void);

/* Reads the analog value of potentiometer 2 and returns RPM equivalent value */
uint8 POT2_getValue(void);

/* Reads the analog value of potentiometer 3 and returns RPM equivalent value */
uint8 POT3_getValue(void);

/* Initiates required pins of potentiometers */
void POT_init(void);

#endif /* POTENTIOMETER_H_ */
