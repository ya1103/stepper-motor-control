/*
 * main.c
 *
 *  Created on: May 2, 2026
 *      Author: you3e
 */

#include "timer.h"
#include "std_types.h"
#include "tb6600.h"
#include "limit_switch.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

unsigned int g_counter = 0;

void T0_callBackFunction(){
	g_counter++;

	//2000000/125 no of micro seconds over time for each tick we get 16000
	if(g_counter == 16000){
		g_counter = 0;
		//TB6600_reverse();
	}
}

int main(){
	//enable global interrupts
	sei();

	//configuration settings for timer0
	Timer_ConfigType T0 = {
		0,
		250,
		Timer0,
		F_CPU_8,
		Compare
	};

	//configuration settings for timer1
	Timer_ConfigType T1 = {
		0,
		250,
		Timer1,
		F_CPU_8,
		Compare
	};

	//configuration settings for timer2
	Timer_ConfigType T2 = {
		0,
		250,
		Timer2,
		F_CPU_8,
		Compare
	};

	Timer_setCallBack(T0_callBackFunction, Timer0);
	//initiate timer0
	Timer_init(&T0);

	//initiate timer1
	Timer_init(&T1);

	//initiate timer2
	Timer_init(&T2);

	//Initiate limit switches
	LS_init();

	//Initiate Stepper Drivers
	TB6600_init();

	//Flag for limit switch 1 to indicate if it was triggered before or not
	boolean LS1_FLAG = FALSE;

	//Flag for limit switch 2 to indicate if it was triggered before or not
	boolean LS2_FLAG = FALSE;
	while(1){

		//delay to resolve bouncing effect due to mechanical switch
		_delay_ms(20);

		if((LS1_check() == LS_TRIGGERED) && (LS2_check() == LS_TRIGGERED)){
			//in this case it is abnormal so stop pulses for the stepper motor
			//Timer_deInit(Timer0);
		} else{
			if(LS1_check() == LS_TRIGGERED){
				//to avoid toggling reverse while limit switch is triggered, we check our flag first.
				if(LS1_FLAG == FALSE){
					TB6600_reverse();
					//after reversing alter the flag to be true
					LS1_FLAG = TRUE;
				}

				//skip to next iteration on each trigger detection
				continue;

				} else{
					/*when limit switch is untriggered reset flag
					 *to allow next reverse for stepper motor
					 */
					LS1_FLAG = FALSE;
				}


			if(LS2_check() == LS_TRIGGERED){
				//to avoid toggling reverse while limit switch is triggered, we check our flag first.
				if(LS2_FLAG == FALSE){
					TB6600_reverse();
					//after reversing alter the flag to be true
					LS2_FLAG = TRUE;
				}

				//skip to next iteration on each trigger detection
				continue;

			} else{
				/*when limit switch is untriggered reset flag
				 *to allow next reverse for stepper motor
				 */
				LS2_FLAG = FALSE;
			}
		}


	}


}
