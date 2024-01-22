/*
 * Module: DC-Motor
 *
 * File Name: dc-motor.c
 *
 * Description: source file for DC-Motor driver
 *
 * Author: Mahmoud Magdy Mahmoud Ali
*/


#include "dc-motor.h"
#include <avr/io.h>
#include "gpio.h"
#include "pwm.h"


/*
 * Description :
 * The Function responsible for setup the direction for the two motor pins through the GPIO driver.
 * Stop at the DC-Motor at the beginning through the GPIO driver.
 */
void DcMotor_Init(void){
	/* setup the two motor pins as output pins */
	GPIO_setupPinDirection(DC_MOTOR_IN1_PORT_ID ,DC_MOTOR_IN1_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_IN2_PORT_ID ,DC_MOTOR_IN2_PIN_ID, PIN_OUTPUT);

	/* stop the motor at the beginning */
	GPIO_writePin(DC_MOTOR_IN1_PORT_ID, DC_MOTOR_IN1_PIN_ID, LOGIC_LOW);
	GPIO_writePin(DC_MOTOR_IN2_PORT_ID, DC_MOTOR_IN2_PIN_ID, LOGIC_LOW);
}

/*
 * Description :
 * The function responsible for rotate the DC Motor CW/ or A-CW or stop the motor based on the state input state value.
 * Send the required duty cycle to the PWM driver based on the required speed value.
 */
void DcMotor_Rotate(DcMotor_State state,uint8 speed){

	/* Make a PWM of duty cycle based on the speed */
	PWM_Timer0_Start(speed);

	/* Control the direction based on the speed argument */
	switch(state){

	case OFF:
		GPIO_writePin(DC_MOTOR_IN1_PORT_ID, DC_MOTOR_IN1_PIN_ID, LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_IN2_PORT_ID, DC_MOTOR_IN2_PIN_ID, LOGIC_LOW);
		break;

	case CW:
		GPIO_writePin(DC_MOTOR_IN1_PORT_ID, DC_MOTOR_IN1_PIN_ID, LOGIC_HIGH);
		GPIO_writePin(DC_MOTOR_IN2_PORT_ID, DC_MOTOR_IN2_PIN_ID, LOGIC_LOW);
		break;

	case A_CW:
		GPIO_writePin(DC_MOTOR_IN1_PORT_ID, DC_MOTOR_IN1_PIN_ID, LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_IN2_PORT_ID, DC_MOTOR_IN2_PIN_ID, LOGIC_HIGH);
		break;

		}

}



