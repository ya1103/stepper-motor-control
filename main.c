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
#include "plc.h"
#include <avr/io.h>
#include <avr/delay.h>


/*Function that computes OCR value from RPM argument
 * Returned value should update the OCR register in selected timer later on
 * Expected RPM parameter should range from 10 to 50 which was set in potentiometer.h file
 * Returned OCR value will also range from 22 to 116*/
uint8 CalculateCompareValueFromRPM(uint8 RPM){
	/* We should apply this formula
	 *
	 * OCRnA = 		0.3 * f_clk_IO
				------------------------  - 1
         	 	 	2 * uStep * N * RPM
	Where:
 	 - f_clk_IO : clock frequency (Hz)
 	 - uStep	: microstep by tb6600 driver which is set manually on the hardware drive
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


	//initiate required pin for PLC signal detection
	PLC_driver_setup();

	/* PLC signal flag
	 * Initial value should be FALSE until signal is received */
	boolean PLC_signal_flag = FALSE;


	//Wait until PLC sends logic high signal then initiate all required drivers
	while( !( checkForPLC_signal() == PAUSE_OPERATION ) );

	//Update Flag, as if we exit the above while loop that means we received the signal from PLC
	PLC_signal_flag = TRUE;

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

	//Flag that indicates if operation has been paused in any possible case
	boolean OperationPaused = FALSE;

	//Flag that indicates if Stepper Motor 1 has been paused by Timer0 due limit switch detection case
	boolean	timer0_Paused = FALSE;

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

		//Check PLC signal and update flag
		if(checkForPLC_signal() == PAUSE_OPERATION){
			PLC_signal_flag = FALSE;
		} else{
			PLC_signal_flag = TRUE;
		}

		/*If PLC signal is still high keep operation performing normally
		 *If signal flag is logic low, then stop stepper motors by stopping timers*/
		if(PLC_signal_flag == TRUE){

			/* Check if operation was paused before
			 * If yes then re-initiate timers to let stepper motors operate back again
			 * If not then do nothing */
			if(OperationPaused == TRUE){
				//initiate timer0
				Timer_init(&T0);

				//initiate timer1
				Timer_init(&T1);

				//initiate timer2
				Timer_init(&T2);

				//Update flag to avoid re-initiating timers on every loop
				OperationPaused = FALSE;
			}

			/*it is a safer approach to check if timer0 is already operating or not, before updating any register in it
			 * we only check flag for this timer as there is another condition which de-inits the timer in special case
			 * during the limit switch operation
			 *
			 */
			if(timer0_Paused == FALSE){

				//Fetch RPM value required from potentiometer1 which is set by user
				stepperMotor1_RPM = POT1_getValue();
				//Calculate New compare match value from RPM by calling function
				Timer0_NewCompareValue = CalculateCompareValueFromRPM(stepperMotor1_RPM);
				//Update the compare match value in the related timer's structure
				T0.timer_CompareMatchValue = Timer0_NewCompareValue;
				//Update compare value of targeted timer
				Timer_updateCompareValue(Timer0_NewCompareValue, Timer0);

			}

			//Fetch RPM value required from potentiometer1 which is set by user
			stepperMotor2_RPM = POT2_getValue();
			//Calculate New compare match value from RPM by calling function
			Timer1_NewCompareValue = CalculateCompareValueFromRPM(stepperMotor2_RPM);
			//Update the compare match value in the related timer's structure
			T1.timer_CompareMatchValue = Timer1_NewCompareValue;
			//Update compare value of targeted timer
			Timer_updateCompareValue(Timer1_NewCompareValue, Timer1);

			//Fetch RPM value required from potentiometer1 which is set by user
			stepperMotor3_RPM = POT3_getValue();
			//Calculate New compare match value from RPM by calling function
			Timer2_NewCompareValue = CalculateCompareValueFromRPM(stepperMotor3_RPM);
			//Update the compare match value in the related timer's structure
			T2.timer_CompareMatchValue = Timer2_NewCompareValue;
			//Update compare value of targeted timer
			Timer_updateCompareValue(Timer2_NewCompareValue, Timer2);


			//delay to resolve bouncing effect due to mechanical switch
			_delay_ms(20);

			if((LS1_check() == LS_TRIGGERED) && (LS2_check() == LS_TRIGGERED)){
				//in this case it is abnormal so stop pulses for the stepper motor
				Timer_deInit(Timer0);
				//update flag
				timer0_Paused = TRUE;

			} else{

				if(timer0_Paused == TRUE){
					//then re-initiate Timer0 and update flag
					Timer_init(&T0);
					timer0_Paused = FALSE;
				} else{
					//do nothing
				}

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
		} else{
			//If PLC signal has been set LOW pause the operation
			Timer_deInit(Timer0);
			Timer_deInit(Timer1);
			Timer_deInit(Timer2);

			OperationPaused = TRUE;
		}
	}
}
