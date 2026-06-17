/*
 * limit_switch.h
 *
 *  Created on: May 19, 2026
 *      Author: you3e
 */

#ifndef LIMIT_SWITCH_H_
#define LIMIT_SWITCH_H_

#include "gpio.h"
#include "std_types.h"


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define LS1_NO_PORT     PORTD_ID
#define LS1_NO_PIN      PIN1_ID

#define LS2_NO_PORT     PORTD_ID
#define LS2_NO_PIN		PIN2_ID

#define LS_TRIGGERED	LOGIC_LOW
#define LS_UNTRIGGERED	LOGIC_HIGH
/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description:
 * Sets up required pins of limit switch as required
 */
void LS_init();

/*
 * Description:
 * Checks if limit switch 1 is triggered or not
 * and returns status
 */
boolean LS1_check();

/*
 * Description:
 * Checks if limit switch 2 is triggered or not
 * and returns status
 */
boolean LS2_check();

#endif /* LIMIT_SWITCH_H_ */
