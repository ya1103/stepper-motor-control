/*
 * timer.h
 *
 *  Created on: Oct 20, 2025
 *      Author: Yousef
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"


/*******************************************************************************
 *                    		Definitions       			                        *
 *******************************************************************************/

/* ID for each timer mode */
typedef enum{
	Timer0, Timer1, Timer2
}Timer_ID_Type;

/* Clock prescaler */
typedef enum
{
	/* Timer0 and Timer1 clock types */
	NO_CLOCK, F_CPU_CLOCK, F_CPU_8, F_CPU_64, F_CPU_256, F_CPU_1024,

	/* Timer2 clocktypes */
	Timer2_8 = 2,
	Timer2_32,
	Timer2_64,
	Timer2_128,
	Timer2_256,
	Timer2_1024
}Timer_ClockType;

typedef enum{
	Normal, Compare
}Timer_ModeType;

typedef struct{
	uint16 timer_InitialValue;
	uint16 timer_CompareMatchValue;
	Timer_ID_Type timer_id;
	Timer_ClockType timer_clock;
	Timer_ModeType timer_mode;
}Timer_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void Timer_init(const Timer_ConfigType * Config_Ptr);

/* Disable timer clock */
void Timer_deInit(Timer_ID_Type timer_id);

/* the call back function by ISR */
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type timer_ID );

/* Reset all count registers for timers */
void Timer_resetCounter(void);

/* Updates timer OCR value which is passed as an argument */
void Timer_updateCompareValue(uint8 value, Timer_ID_Type timer_ID);

#endif /* TIMER_H_ */
