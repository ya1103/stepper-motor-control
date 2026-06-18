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
#include "potentiometer.h"
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

/*Function that computes OCR value from RPM argument
 * Returned value should update the OCR register in selected timer
 * Expected RPM parameter should range from 10 to 50 which was set in potentiometer.h file
 * Returned OCR value will also range from 22 to 116*/
uint8 CalculateCompareValueFromRPM(uint8 RPM){
	/* We should apply this formula
	 *
	 * OCRnA = 		0.3 * f_clk_IO
        		----------------------  - 1
         	 	 	64 * N * RPM
	Where:
 	 - f_clk_IO : clock frequency (Hz)
	 - N        : timer prescaler
 	 - RPM      : desired revolutions per minute
 	 - OCRnA    : output compare register value */

	float constant_value = 0.3;
	uint8 prescalarOfTimers = 64;
	uint8 microStepByDriver = 32;

	uint32 numerator = (constant_value * 16000000); // 0.3 * Fclock
    uint32 denominator = 2 * microStepByDriver * prescalarOfTimers * RPM; // 64 * N * RPM (N=64)

    uint32 NewCompareValue = (numerator / denominator) - 1;

	return NewCompareValue;
}

int main(){
	//enable global interrupts
	sei();

	//configuration settings for timer0
	Timer_ConfigType T0 = {
		0,
		250,
		Timer0,
		F_CPU_64,
		Compare
	};

	//configuration settings for timer1
	Timer_ConfigType T1 = {
		0,
		250,
		Timer1,
		F_CPU_64,
		Compare
	};

	//configuration settings for timer2
	Timer_ConfigType T2 = {
		0,
		250,
		Timer2,
		F_CPU_64,
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

	//Initiate potentiometers
	POT_init();

	//Flag for limit switch 1 to indicate if it was triggered before or not
	boolean LS1_FLAG = FALSE;

	//Flag for limit switch 2 to indicate if it was triggered before or not
	boolean LS2_FLAG = FALSE;

	//RPM value that will be fetched by potentiometer1
	uint8 stepperMotor1_RPM;

	//RPM value that will be fetched by potentiometer2
	uint8 stepperMotor2_RPM;

	//RPM value that will be fetched by potentiometer3
	uint8 stepperMotor3_RPM;

	//NEW OCR0 value that will be calculated from required RPM
	uint8 Timer0_NewCompareValue;

	//NEW OCRA1 value that will be calculated from required RPM
	uint8 Timer1_NewCompareValue;

	//NEW OCR2 value that will be calculated from required RPM
	uint8 Timer2_NewCompareValue;


	while(1){

		//Fetch RPM value required from potentiometer1 which is set by user
		stepperMotor1_RPM = POT1_getValue();
		//Calculate New compare match value from RPM by calling function
		Timer0_NewCompareValue = CalculateCompareValueFromRPM(stepperMotor1_RPM);
		//Update compare value of targeted timer
		Timer_updateCompareValue(Timer0_NewCompareValue, Timer0);


		//Fetch RPM value required from potentiometer1 which is set by user
		stepperMotor2_RPM = POT2_getValue();
		//Calculate New compare match value from RPM by calling function
		Timer1_NewCompareValue = CalculateCompareValueFromRPM(stepperMotor2_RPM);
		//Update compare value of targeted timer
		Timer_updateCompareValue(Timer1_NewCompareValue, Timer1);

		//Fetch RPM value required from potentiometer1 which is set by user
		stepperMotor3_RPM = POT3_getValue();
		//Calculate New compare match value from RPM by calling function
		Timer2_NewCompareValue = CalculateCompareValueFromRPM(stepperMotor3_RPM);
		//Update compare value of targeted timer
		Timer_updateCompareValue(Timer2_NewCompareValue, Timer2);



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
