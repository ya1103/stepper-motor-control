/*
 * plc.h
 *
 *  Created on: Jun 19, 2026
 *      Author: you3e
 */

#ifndef PLC_H_
#define PLC_H_

#include "gpio.h"

/************ Definitions **************/
#define PLC_SIGNAL_PORT			PORTB_ID
#define PLC_SIGNAL_PIN			PIN7_ID

typedef enum {
		START_OPERATION, PAUSE_OPERATION
} PLC_StateType;

/******* Function Prototypes **********/

/* this function sets up the required pins */
void PLC_driver_setup(void);

/* This function should read the targeted pin and return its state */
uint8 checkForPLC_signal(void);

#endif /* PLC_H_ */
