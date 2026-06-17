/*
 * potentiometer.c
 *
 *  Created on: Jun 17, 2026
 *      Author: you3e
 */

#include "std_types.h"
#include "potentiometer.h"
#include "adc.h"

/* Reads the analog value of potentiometer 1 and returns RPM equivalent value */
uint8 POT1_getValue(void){

	/* Call ADC function to convert Analog signal to digital value */
	uint16 digital_value = ADC_readChannel(POT1_CHANNEL_ID);
	return (uint8)( STEPPER_MOTOR_MIN_RPM + (((uint32)digital_value * (STEPPER_MOTOR_MAX_RPM - STEPPER_MOTOR_MIN_RPM) + (ADC_MAXIMUM_VALUE/2)) / ADC_MAXIMUM_VALUE) );

}

/* Reads the analog value of potentiometer 1 and returns RPM equivalent value */
uint8 POT2_getValue(void){

	/* Call ADC function to convert Analog signal to digital value */
	uint16 digital_value = ADC_readChannel(POT2_CHANNEL_ID);
	return (uint8)( STEPPER_MOTOR_MIN_RPM + (((uint32)digital_value * (STEPPER_MOTOR_MAX_RPM - STEPPER_MOTOR_MIN_RPM) + (ADC_MAXIMUM_VALUE/2)) / ADC_MAXIMUM_VALUE) );

}

/* Reads the analog value of potentiometer 1 and returns RPM equivalent value */
uint8 POT3_getValue(void){

	/* Call ADC function to convert Analog signal to digital value */
	uint16 digital_value = ADC_readChannel(POT3_CHANNEL_ID);
	return (uint8)( STEPPER_MOTOR_MIN_RPM + (((uint32)digital_value * (STEPPER_MOTOR_MAX_RPM - STEPPER_MOTOR_MIN_RPM) + (ADC_MAXIMUM_VALUE/2)) / ADC_MAXIMUM_VALUE) );

}

/* Initiates required pins of potentiometers */
void POT_init(void){
	GPIO_setupPinDirection(POT_PORT_ID, POT1_CHANNEL_ID, PIN_INPUT);
	GPIO_setupPinDirection(POT_PORT_ID, POT2_CHANNEL_ID, PIN_INPUT);
	GPIO_setupPinDirection(POT_PORT_ID, POT3_CHANNEL_ID, PIN_INPUT);
}
