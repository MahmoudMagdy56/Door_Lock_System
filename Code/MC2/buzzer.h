/******************************************************************************
 *
 * Module: buzzer
 *
 * File Name: buzzer.h
 *
 * Description: Header file for Buzzer
 *
 * Author: Mahmoud Magdy
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include "gpio.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define BUZZER_PORT_ID			PORTA_ID
#define BUZZER_PIN_ID			PIN0_ID


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * The Function responsible for setup the direction for the Buzzer pins through the GPIO driver.
 * Stop the Buzzer at the beginning through the GPIO driver.
 */

void BUZZER_Init (void);



/*
 * Description :
 * The Function responsible for activating the Buzzer
 */
void BUZZER_on (void);



/*
 * Description :
 * The Function responsible for deactivating the Buzzer
 */
void BUZZER_off (void);


#endif /* BUZZER_H_ */
