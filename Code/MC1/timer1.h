 /******************************************************************************
 *
 * Module: timer1
 *
 * File Name: timer.h
 *
 * Description: Header file for the Timer1 driver
 *
 * Author: Mahmoud Magdy
 *
 *******************************************************************************/

#ifndef TIMER1_H_
#define TIMER1_H_

#include "std_types.h"


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

typedef enum{
	TIMER1_STOP , TIMER1_NO_PRESCALER , TIMER1_8_PRESCALER , TIMER1_64_PRESCALER ,
	TIMER1_256_PRESCALER , TIMER1_1024_PRESCALER
}Timer1_Prescaler;


typedef enum{
	TIMER1_NORMAL_MODE , TIMER1_COMPARE_MODE = 8
}Timer1_Mode;


typedef struct{
	uint16 initial_value;
	uint16 compare_value; /* It will be used in compare mode only */
	Timer1_Prescaler prescaler;
	Timer1_Mode mode;
}Timer1_ConfigType;



/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/



/*
 * Description :
 * Function to initialize the Timer Driver:
 *  1. Inserting the initial value for the timer.
 *  2. Selecting Timer1 Prescaler.
 *  3. Selecting Timer1 Mode.
 */
void Timer1_init(const Timer1_ConfigType * Config_Ptr);




/*
 * Description :
 * Function to deinitialization Timer1.
 */
void Timer1_deInit(void);




/*
 * Description :
 * Function to set the Call Back function address.
 */
void Timer1_setCallBack(void(*a_ptr)(void));

#endif /* TIMER1_H_ */
