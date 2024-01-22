 /******************************************************************************
 *
 * Module: MC1 APP
 *
 * File Name: app.c
 *
 * Description: Source file for the Application
 *
 * Author: Mahmoud Magdy
 *
 *******************************************************************************/

#include "uart.h"
#include "keypad.h"
#include "lcd.h"
#include "timer1.h"
#include "gpio.h"
#include "avr/io.h"
#include "avr/interrupt.h"
#include "util/delay.h"
#include "matual_commands.h"

/*******************************************************************************
 *                               Global Variables                              *
 *******************************************************************************/


volatile uint8 Timer1Ticks = 0;
uint8 Password[5];

Timer1_ConfigType Timer1_Config = {0 /* Initial Value */, 31250 /* Compare Value */
		, TIMER1_COMPARE_MODE , TIMER1_256_PRESCALER };


/****************************************************************************************/

/*
 * Description :
 * Function to send the password from the MC1 to MC2
 */
void send_password (uint8 *pass_arr)
{
	uint8 Counter;
	for(Counter=0 ; Counter < 5 ; Counter++)
	{
		UART_sendByte(pass_arr[Counter]);
		_delay_ms(15);
	}
}


/*
 * Description :
 * Function to take the password of 5 chars (Numbers Only)
 * from the user and display '*' on the LCD
 */

void take_password (void)
{
	uint8 counter;
	for(counter = 0 ; counter<5 ; counter++)
	{
		Password[counter] = KEYPAD_getPressedKey();
		_delay_ms(100);
		if(Password[counter] >= 0 && Password[counter] <= 9)
		{

			LCD_intgerToString(Password[counter]);
			_delay_ms(500);
			LCD_moveCursor(1,counter);
			LCD_displayCharacter('*');
		}
		else
		{
			counter--;
		}
		_delay_ms(15);
	}
	while(KEYPAD_getPressedKey() != '=');
}

/*
 * Description :
 * Function to increment the tick.
 */

void TIMER1_tick(void)
{
	Timer1Ticks++;
}




int main()
{
	SREG |= (1<<7); /* Enable Global Interrupt */

	uint8 NumOfTries = 0; /* Variable to count the number of wrong tries entered by the user */
	uint8 PassIsCorrectFlag = 0; /* Variable to check on password */
	uint8 PassIsSetFlag = 0; /* Variable to check on password is already setted up or not */
	uint8 PassIsMatchFlag = 0; /* Variable to check if the two passwords matches or not */

	/* UART Configuration */
	UART_ConfigType UART_Configration = {UART_EIGHT_BIT_MODE , UART_DISABLE_PARITY ,
			UART_ONE_STOP_BIT, 9600 /* baud rate */};


	LCD_init();
	UART_init(&UART_Configration);
	Timer1_setCallBack(&TIMER1_tick);

	LCD_moveCursor(0,2);
	LCD_displayString("Door Locker");
	LCD_moveCursor(1,0);
	LCD_displayString("    System  ");
	_delay_ms(1500);
	LCD_clearScreen();
	LCD_displayString("By: ");
	LCD_moveCursor(1,0);
	LCD_displayString(" Mahmoud Magdy");
	_delay_ms(1000);
	LCD_clearScreen();

	/* Ask MC2 if Password is setted */
	UART_sendByte(IS_PASSWORD_SETTED);

	/* Check if the password setted */
	if(UART_recieveByte() == PASSWORD_SETTED)
	{
		/* raise the password set flag */
		PassIsSetFlag = 1;
	}

	while(1)
	{

		/* Check if the password flag isn't setted */
		if(PassIsSetFlag == 0)
		{
			LCD_clearScreen();
			LCD_displayString("plz enter pass: ");
			LCD_moveCursor(1,0);
			_delay_ms(15);
			/* Getting MC2 Ready to receive the first password */
			UART_sendByte(TAKE_FIRST_PASSWORD);

			/* Take the first password from the user*/
			take_password();

			/* Send the first password to MC2 */
			send_password(Password);

			LCD_clearScreen();
			LCD_displayString("re-enter pass");
			LCD_moveCursor(1,0);

			_delay_ms(30);

			/* Getting MC2 Ready to receive the second password */
			UART_sendByte(TAKE_SECOND_PASSWORD);

			/* Take the second password from the user */
			take_password();

			/* Send the second password to MC2 to compare */
			send_password(Password);
			_delay_ms(30);

			/* Send to MC2 to check if the two passwords are matched or not */
			UART_sendByte(IS_PASSWORD_MATCH);

			/* Store the response from MC2 */
			PassIsMatchFlag = UART_recieveByte();
			_delay_ms(30);
			/* Check if the two password matches or not */
			if(PassIsMatchFlag == PASSWORD_MATCHED)
			{
				/* set the password is set flag & reset the password is match flag */
				PassIsSetFlag = 1;
				PassIsMatchFlag = 0;

			}

			else
			{
				LCD_clearScreen();
				LCD_displayString("  Password Not");
				LCD_moveCursor(1,4);
				LCD_displayString("Matched");
				_delay_ms(1500);
				LCD_clearScreen();
			}


		}

		else
		{
			LCD_clearScreen();
			NumOfTries = 0;

			LCD_displayString("+ : Open Door");
			LCD_displayStringRowColumn(1,0,"- : Change Pass");

			switch(KEYPAD_getPressedKey())
			{

			case '+' : /* Open Door Case */
				do{
					/* reset the password is correct flag to make the user enter the password again */
					PassIsCorrectFlag = 0;

					LCD_clearScreen();
					LCD_displayString("plz enter pass:");
					LCD_moveCursor(1,0);

					/* Make MC2 ready to take the setted password */
					UART_sendByte(TAKE_MAIN_PASSWORD);

					/* Take the password */
					take_password();

					/* Send the password to MC2 to check if it is correct or not */
					send_password(Password);

					_delay_ms(30);

					/* Check if the password is correct or not */
					if(UART_recieveByte() == PASSWORD_TRUE)
					{
						/* raise the password is correct flag */
						PassIsCorrectFlag = 1;

						/* Send to MC2 to open the door */
						UART_sendByte(OPEN_DOOR);

						/* Initialize Timer1 with the configuration */
						Timer1_init(&Timer1_Config);

						LCD_clearScreen();

						LCD_displayStringRowColumn(0,0,"    Door Is    ");
						LCD_displayStringRowColumn(1,0,"  Unlocking   ");


						/* Wait for 15 second */
						while(Timer1Ticks != 15 );

						/* reset the timer1 ticks & Timer1 */
						Timer1Ticks = 0;
						Timer1_deInit();


						/* Initialize the Timer1 Again */
						Timer1_init(&Timer1_Config);
						LCD_clearScreen();
						LCD_displayString("    Holding   ");

						/* Wait for 3 second */
						while(Timer1Ticks != 3);

						/* reset the timer1 ticks & Timer1 */
						Timer1Ticks = 0;
						Timer1_deInit();


						/* Initialize the Timer1 Again */
						Timer1_init(&Timer1_Config);
						LCD_clearScreen();
						LCD_displayStringRowColumn(0,0,"    Door Is    ");
						LCD_displayStringRowColumn(1,0,"    Locking    ");

						/* Wait for 15 second */
						while(Timer1Ticks != 15 );

						/* reset the timer1 ticks & Timer1 */
						Timer1Ticks = 0;
						Timer1_deInit();
					}

					else
					{

						NumOfTries++; /* Increment the number of tries */

						LCD_clearScreen();
						LCD_displayString("Wrong Password!");
						LCD_moveCursor(1,0);
						LCD_displayString("Tries Remain: ");
						LCD_intgerToString(3-NumOfTries);
						_delay_ms(2000);

					}

				}while((NumOfTries < 3 ) && !(PassIsCorrectFlag));

				/* Check if the user didn't enter the correct password for 3 times */
				if(PassIsCorrectFlag == 0)
				{
					/* Activate Alarm Mode on MC2 */
					UART_sendByte(OPEN_ALARM);

					/* Activate the Alarm Mode on MC1 */
					LCD_clearScreen();
					Timer1_init(&Timer1_Config);
					while(Timer1Ticks != 60){
						LCD_displayString("    WARNING!    ");

					}
					LCD_clearScreen();
					Timer1Ticks = 0;
					Timer1_deInit();

				}
				break;

			case '-' :

				do{
					PassIsCorrectFlag = 0; /* Reset the password is correct flag */

					LCD_clearScreen();
					LCD_displayString("plz enter pass:");
					LCD_moveCursor(1,0);
					PassIsMatchFlag = 1;
					_delay_ms(100);


					/* Make MC2 ready to take the setted password */
					UART_sendByte(TAKE_MAIN_PASSWORD);

					/* Take the password */
					take_password();

					/* Send the password to MC2 to check if it is correct or not */
					send_password(Password);

					_delay_ms(30);

					/* Check if the password is correct or not */
					if(UART_recieveByte() == PASSWORD_TRUE)
					{
						PassIsSetFlag = 0; /* Clear the password is set flag */
						PassIsCorrectFlag = 1;

					}
					else
					{
						NumOfTries++; /* Increment the number of tries */

						LCD_clearScreen();
						LCD_displayString("Wrong Password!");
						LCD_moveCursor(1,0);
						LCD_displayString("Tries Remain: ");
						LCD_intgerToString(3-NumOfTries);
						_delay_ms(2000);

					}

				}while((NumOfTries < 3) && !(PassIsCorrectFlag));

				if(PassIsCorrectFlag == 0)
				{
					/* Activate Alarm Mode on MC2 */
					UART_sendByte(OPEN_ALARM);

					/* Activate the Alarm Mode on MC1 */
					LCD_clearScreen();
					Timer1_init(&Timer1_Config);
					while(Timer1Ticks != 60){
						LCD_displayString("    WARNING!    ");

					}
					LCD_clearScreen();
					Timer1Ticks = 0;
					Timer1_deInit();

				}
				break;

			}
		}
	}


}


