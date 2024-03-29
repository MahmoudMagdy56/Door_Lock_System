/*
 * Module: DC-Motor
 *
 * File Name: dc-motor.c
 *
 * Description: header file for DC-Motor driver
 *
 * Author: Mahmoud Magdy Mahmoud Ali
*/

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define DC_MOTOR_IN1_PORT_ID			PORTB_ID
#define DC_MOTOR_IN1_PIN_ID				PIN5_ID

#define DC_MOTOR_IN2_PORT_ID			PORTB_ID
#define DC_MOTOR_IN2_PIN_ID				PIN6_ID

#define DC_MOTOR_EN1_PORT_ID			PORTB_ID
#define DC_MOTOR_EN1_PIN_ID				PIN3_ID




/*******************************************************************************
 *                             Type Decelerations                               *
 *******************************************************************************/

typedef enum{
	OFF , CW , A_CW
}DcMotor_State;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * The Function responsible for setup the direction for the two motor pins through the GPIO driver.
 * Stop at the DC-Motor at the beginning through the GPIO driver.
 */
void DcMotor_Init(void);



/*
 * Description :
 * The function responsible for rotate the DC Motor CW/ or A-CW or stop the motor based on the state input state value.
 * Send the required duty cycle to the PWM driver based on the required speed value.
 */
void DcMotor_Rotate(DcMotor_State state,uint8 speed);


#endif /* DC_MOTOR_H_ */
