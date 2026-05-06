/*
 * timer.c
 *
 *  Created on: Oct 20, 2025
 *      Author: Yousef
 */
#include "timer.h"
#include "std_types.h"
#include "common_macros.h" /* To use the macros like SET_BIT */
#include <avr/io.h> /* To use Timer Registers */
#include <avr/interrupt.h> /* For Timers ISR */

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtrTimer0)(void) = NULL_PTR;
static volatile void (*g_callBackPtrTimer1)(void) = NULL_PTR;
static volatile void (*g_callBackPtrTimer2)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_OVF_vect)
{
	if(g_callBackPtrTimer0 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtrTimer0)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtrTimer0 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtrTimer0)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtrTimer1 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtrTimer1)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtrTimer1 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtrTimer1)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER2_OVF_vect)
{
	if(g_callBackPtrTimer2 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtrTimer2)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER2_COMP_vect)
{
	if(g_callBackPtrTimer2 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtrTimer2)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void Timer_init(const Timer_ConfigType * Config_Ptr){
	if(Config_Ptr->timer_id == Timer0){
		/* Enabled if not operating in pwm mode */
		TCCR0 |= (1<<FOC0);

		/* Insert initial value for timer */
		TCNT0 = (uint8) Config_Ptr->timer_InitialValue;

		/* Enable clock timer */
		TCCR0 |= Config_Ptr->timer_clock;

		if(Config_Ptr->timer_mode == Compare){
			/* Enable WGM01 for compare mode
			 * Enable OC0 toggle on compare match */
			TCCR0 |= (1<<WGM01) | (1 << COM00);

			/* Insert compare value */
			OCR0 = (uint8) Config_Ptr->timer_CompareMatchValue;

			/* Clear first two bits */
			TIMSK &= 0xFC;

			/* Enable compare interrupt */
			TIMSK |= (1<<OCIE0);
		} else if(Config_Ptr->timer_mode == Normal){

			/* Clear first two bits */
			TIMSK &= 0xFC;

			/* Enable normal interrupt */
			TIMSK |= (1<<TOIE0);
		}
	} else if(Config_Ptr->timer_id == Timer1){
		/* Enabled if not operating in pwm mode */
		TCCR1A |= (1<<FOC1A) | (1<<FOC1B);

		/* Enable timer clock */
		TCCR1B |= Config_Ptr->timer_clock;

		/* Insert initial value */
		TCNT1 = Config_Ptr->timer_InitialValue;

		if(Config_Ptr->timer_mode == Compare){
			/* Enable compare mode */
			TCCR1B |= (1<<WGM12);

			/* Enable OC1A toggle on compare match */
			TCCR1A |= (1<<COM1A0);

			/* Insert compare value */
			OCR1A = Config_Ptr->timer_CompareMatchValue;

			/* Clear bits 4 and 2 */
			TIMSK &= 0xEB;

			/* Enable compare interrupt */
			TIMSK |= (1<<OCIE1A);
		} else if(Config_Ptr->timer_mode == Normal){
			/* Clear bits 4 and 2 */
			TIMSK &= 0xEB;

			/* Enable normal interrupt */
			TIMSK |= (1<<TOIE1);
		}
	} else if(Config_Ptr->timer_id == Timer2)
	{
		/* Enabled if not operating in pwm mode */
		TCCR2 |= (1<<FOC2);

		/* Insert initial value for timer */
		TCNT2 = (uint8) Config_Ptr->timer_InitialValue;

		/* Enable clock timer */
		TCCR2 |= Config_Ptr->timer_clock;

		if(Config_Ptr->timer_mode == Compare)
		{
			/* Enable compare mode
			 * Enable OC20 toggle on compare match */
			TCCR2 |= (1<<WGM21) | (1<<COM20);

			/* Insert compare value */
			OCR2 = (uint8) Config_Ptr->timer_CompareMatchValue;

			/* Clear first two bits */
			TIMSK &= 0xFC;

			/* Enable compare interrupt */
			TIMSK |= (1<<OCIE2);
		} else if(Config_Ptr->timer_mode == Normal)
		{
			/* Clear first two bits */
			TIMSK &= 0xFC;

			/* Enable normal interrupt */
			TIMSK |= (1<<TOIE2);
		}
	}
}

void Timer_deInit(Timer_ID_Type timer_id){
	if(timer_id == Timer0){
		/* Clear first 3 bits in register to stop clock */
		TCCR0 &= 0xF8;

		/* Disable compare mode */
		TCCR0 &= ~(1<<WGM01);
	} else if(timer_id == Timer1){
		/* Clear first 3 bits in register to stop clock */
		TCCR1B &= 0xF8;

		/* Disable compare mode */
		TCCR1B &= ~(1<<WGM12);
	} else if(timer_id == Timer2){
		/* Clear first 3 bits in register to stop clock */
		TCCR2 &= 0xF8;

		/* Disable compare mode */
		TCCR0 &= ~(1<<WGM21);
	}
}

void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type timer_ID ){
	if(timer_ID == Timer0){
		g_callBackPtrTimer0 = a_ptr;
	} else if(timer_ID == Timer1){
		g_callBackPtrTimer1 = a_ptr;
	} else if(timer_ID == Timer2){
		g_callBackPtrTimer2 = a_ptr;
	}
}


void Timer_resetCounter(void){
	TCNT0 = 0;
	TCNT1 = 0;
	TCNT2 = 0;
}






























