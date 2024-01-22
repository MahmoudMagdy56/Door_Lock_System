 /******************************************************************************
 *
 * Module: MC2 APP
 *
 * File Name: app.c
 *
 * Description: Source file for the Application
 *
 * Author: Mahmoud Magdy
 *
 *******************************************************************************/

#include "buzzer.h"
#include "dc-motor.h"
#include "pwm.h"
#include "matual_commands.h"
#include "twi.h"
#include "external_eeprom.h"
#include "avr/interrupt.h"
#include "util/delay.h"
#include "gpio.h"
#include "avr/io.h"
#include "timer1.h"
#include "uart.h"


/*******************************************************************************
 *                               Global Variables                              *
 *******************************************************************************/

volatile uint8 Timer1Ticks = 0 ;
Timer1_ConfigType Timer1_Config = {0 /* Initial Value */, 31250 /* Compare Value */
		, TIMER1_COMPARE_MODE , TIMER1_256_PRESCALER };


/*******************************************************************************
 *                                   Functions                                 *
 *******************************************************************************/
/*
 * Description :
 * Function responsible for receiving the password from MC1
 */
void receive_password(uint8 *Password)
{
	uint8 counter ;
	for(counter = 0 ; counter < 5  ; counter++)
	{
		Password[counter] = UART_recieveByte();
		_delay_ms(15);

	}
}

/*
 * Description :
 * Function responsible for increment timer1 ticks
 */
void timer1_tick(void){
	Timer1Ticks++;
}

/*************************************************************************************/

int main()
{
	SREG |= (1<<7); /* Enable Interrupts */

	uint8 Password1[5],Password2[5];  /* For storing 2 passwords */
	uint8 PassIsSetFlag = 0; 	/* to check if the password setted successfully */
	uint8 PassIsMatching = 0;     /* to check if the two password matches */

	uint8 TempVariable = 0;
	uint8 Counter = 0;

	UART_ConfigType UART_Configuration = {UART_EIGHT_BIT_MODE , UART_DISABLE_PARITY ,
			UART_ONE_STOP_BIT , 9600 };

	TWI_ConfigType TWI_Configuration = {0x01 , BAUDRATE_400 };

	UART_init(&UART_Configuration);
	TWI_init(&TWI_Configuration);

	DcMotor_Init();
	BUZZER_Init();


	Timer1_setCallBack(&timer1_tick);

	EEPROM_writeByte(0xFF , 0);


	_delay_ms(15);

	while(1)
	{
		switch(UART_recieveByte())
		{

		case IS_PASSWORD_SETTED :
			PassIsSetFlag = 0;

			/* start read the password set flag from EEPROM */
			EEPROM_readByte(0xFF , &PassIsSetFlag);

			/* Check if the password is setted */
			if(PassIsSetFlag)
			{
				/* Send confirming to MC1 */
				UART_sendByte(PASSWORD_SETTED);
			}
			else
			{
				UART_sendByte(PASSWORD_NOTSETTED);
			}
			break;

		case TAKE_FIRST_PASSWORD :

			/* Start receiving first password */
			receive_password(Password1);
			break;

		case TAKE_SECOND_PASSWORD :

			/* Start receiving second password */
			receive_password(Password2);
			break;

		case IS_PASSWORD_MATCH :

			/* raise password is matching flag */
			PassIsMatching = 1;

			for(Counter = 0 ; Counter<5 ; Counter++)
			{
				/* if the two password is not matched */
				if(Password1[Counter] != Password2[Counter])
				{
					/* Clear password is matching flag */
					PassIsMatching = 0;
					break;
				}

			}
			/* Check if the two password are matching */
			if(PassIsMatching)
			{
				/* send informing to MC1 */
				UART_sendByte(PASSWORD_MATCHED);

				/* Store the password set flag into EEPROM */
				EEPROM_writeByte(0xFF , PassIsMatching);
				_delay_ms(15);

				/* Store the setted password into EEPROM */
				for(Counter = 0; Counter<5 ; Counter++)
				{
					EEPROM_writeByte(Counter, Password1[Counter]);
					_delay_ms(15);
				}

			}

			else
			{
				/* Send informing to MC1 that the two password are unmatched */
				UART_sendByte(PASSWORD_NOTMATCHED);

			}
			_delay_ms(15);
			break;

		case TAKE_MAIN_PASSWORD :

			PassIsMatching = 1;

			/* Receive the password */
			receive_password(Password1);

			/* Compare the entered password with the stored password */
			for(Counter=0 ; Counter<5 ; Counter++)
			{
				EEPROM_readByte(Counter , &TempVariable);

				/* Check if is match or not */
				if(Password1[Counter] != TempVariable)
				{
					PassIsMatching = 0;
				}
				_delay_ms(15);

			}
			/* Check if the entered password is match with the setted password */
			if(PassIsMatching)
			{
				UART_sendByte(PASSWORD_TRUE);
			}
			else
			{
				UART_sendByte(PASSWORD_FALSE);
			}
			break;


		case OPEN_DOOR :
			/* Unlock the door then hold then lock */

			Timer1_init(&Timer1_Config);

			DcMotor_Rotate(CW , 100);
			/* wait for 15 seconds */
			while(Timer1Ticks != 15);

			_delay_ms(15);
			/* reset the timer ticks */
			Timer1Ticks = 0;


			Timer1_deInit();
			Timer1_init(&Timer1_Config);


			DcMotor_Rotate(OFF , 0);
			/* wait for 3 seconds */
			while(Timer1Ticks != 3);


			/* reset the timer ticks */
			Timer1Ticks = 0;

			_delay_ms(15);
			Timer1_deInit();
			Timer1_init(&Timer1_Config);

			DcMotor_Rotate(A_CW , 100);
			/* wait for 15 seconds */
			while(Timer1Ticks != 15);

			/* reset the timer ticks */
			Timer1Ticks = 0;
			Timer1_deInit();

			DcMotor_Rotate(OFF , 0);
			break;

		case OPEN_ALARM :

			Timer1_init(&Timer1_Config);
			BUZZER_on();
			/* Wait 60 seconds */
			while(Timer1Ticks != 60);

			/* reset the timer ticks */
			Timer1Ticks = 0;

			Timer1_deInit();
			BUZZER_off();
			break;


		}
	}
}
