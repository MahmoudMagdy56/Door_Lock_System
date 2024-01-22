/******************************************************************************
 *
 * Module: buzzer
 *
 * File Name: buzzer.c
 *
 * Description: Source file for Buzzer
 *
 * Author:Mahmoud Magdy
 *
 *******************************************************************************/

#include "buzzer.h"


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * The Function responsible for setup the direction for the Buzzer pins through the GPIO driver.
 * Stop the Buzzer at the beginning through the GPIO driver.
 */

void BUZZER_Init (void)
{
	GPIO_setupPinDirection(BUZZER_PORT_ID,BUZZER_PIN_ID,PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);
}

/*
 * Description :
 * The Function responsible for activating the Buzzer
 */
void BUZZER_on (void)
{
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_HIGH);
}



/*
 * Description :
 * The Function responsible for deactivating the Buzzer
 */
void BUZZER_off (void)
{
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);
}
