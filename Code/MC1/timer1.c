 /******************************************************************************
 *
 * Module: timer1
 *
 * File Name: timer.c
 *
 * Description: Source file for the Timer1 driver
 *
 * Author: Mahmoud Magdy
 *
 *******************************************************************************/

#include "timer1.h"
#include "avr/interrupt.h"
#include "avr/io.h"


/*******************************************************************************
 *                               Global Variables                              *
 *******************************************************************************/

void (*calling_ISR)(void) = NULL_PTR;


/*******************************************************************************
 *                               ISR                                           *
 *******************************************************************************/

/* ISR for Overflow Mode */
ISR (TIMER1_OVF_vect)
{
	if(calling_ISR != NULL_PTR)
	{
		(*calling_ISR)();
	}
}


/* ISR for Compare Mode */
ISR (TIMER1_COMPA_vect)
{
	if(calling_ISR != NULL_PTR)
	{
		(*calling_ISR)();
	}
}


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
void Timer1_init(const Timer1_ConfigType * Config_Ptr)
{
	TCNT1 = Config_Ptr ->initial_value ;  /* Set the initial value for Timer1 */

	if(Config_Ptr -> mode == TIMER1_NORMAL_MODE)
	{
		TIMSK |= (1<<TOIE1);  /* Enable Timer1 Overflow Interrupt */

		/*  1. Active for non-PWM mode.
		 *  2. Choose Normal Mode.
		 *  3. Disable All Features except Normal Mode Features.
		 */
		TCCR1A = (1<<FOC1A);

		TCCR1B = Config_Ptr -> prescaler; /* Set the prescaler for Timer1 */

	}

	else
	{
		OCR1A = Config_Ptr -> compare_value ; /* Put the compare value for Timer1 */

		TIMSK |= (1<<OCIE1A);  /* Output Compare A Match Interrupt Enable */

		/*  1. Active for non-PWM mode.
		 *  2. Choose CTC Mode.
		 */
		TCCR1A = (1<<FOC1A);


		/*
		 * Choose the prescaler and mode.
		 */
		TCCR1B = Config_Ptr->prescaler | Config_Ptr->mode;
	}
}


/*
 * Description :
 * Function to deinitialization Timer1.
 */
void Timer1_deInit(void)
{
	TCNT1 = 0;

	TCCR1B = 0;
}


/*
 * Description :
 * Function to set the Call Back function address.
 */
void Timer1_setCallBack(void(*a_ptr)(void))
{
	calling_ISR = a_ptr;
}
