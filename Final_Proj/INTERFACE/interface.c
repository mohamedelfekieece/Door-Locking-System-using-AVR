/******************************************************************************
 *
 * Main
 *
 * File Name: interface.c
 *
 * Description: Source file for interface MC main function
 *
 *Author: Mohamed Elfeki
 *******************************************************************************/
#include"keypad.h"
#include"lcd.h"
#include"uart.h"
#include"timer0.h"
#include"util/delay.h"
#include "avr/io.h" /* To use the IO Ports Registers */
#define READY 1
#define NOT_READY 2
#define DOOR_OPEN 918
#define DOOR_CLOSE 2020
#define DOOR_HOLD 1102
#define BUZZER 3662
#define MCU_1_READY 0x10
Timer0_ConfigType timer0_config = { TIMER_F_CPU_1024, OVF, 0 };
/********************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

uint16 Tick = 0;
void createPassword(uint8 *password) {
	uint8 i;
	uint8 key; //get value from keypad
	LCD_clearScreen();
	LCD_displayString("Enter new pass");
	_delay_ms(750);
	LCD_clearScreen();
	for (i = 0; i < 5; i++) //loop to enter 5 numbers from user
			{
		key = KEYPAD_getPressedKey(); //to get entered number from user
		_delay_ms(750);
		if ((key >= 0) && (key <= 9)) {
			password[i] = key;
			LCD_displayCharacter('*');
		}
	}
}
void TakePassword(uint8 *password) {
	uint8 i;
	uint8 key; //get value from keypad
	LCD_clearScreen();
	LCD_displayString("Enter old pass");
	_delay_ms(750);
	LCD_clearScreen();
	for (i = 0; i < 5; i++) //loop to enter 5 numbers from user
			{
		key = KEYPAD_getPressedKey(); //to get entered number from user
		_delay_ms(750);
		if ((key >= 0) && (key <= 9)) {
			password[i] = key;
			LCD_displayCharacter('*');

		}
	}
	password[5] = '#';
	password[6] = '\0';
	UART_sendString(password);
}

void Timer0_Tick(void) {
	Tick++;
}

/********************************************************************************
 *                      Main function                             *
 *******************************************************************************/

int main(void) {
	UART_ConfigType UART_Config = { ONE_STOP_BIT, EIGHT_BIT, 9600, DISABLED };
	uint8 i;
	uint8 wrong = 0;
	uint8 password_1[7];
	uint8 password_2[5];
	SREG |= (1 << 7);
	Timer0_setCallBack(Timer0_Tick); /*Set Timer0 Call back function */

	LCD_init(); //function to initialize LCD Driver
	UART_init(&UART_Config); //function to initialize UART Driver

	while (1) //loop which will stay until the two password are matched
	{
		createPassword(password_1); //the main password of the system
		createPassword(password_2); //the password used for checking
		for (i = 0; i < 5; i++) //loop to check that two password are matched
				{
			if (password_1[i] == password_2[i]) {

			} else {
				LCD_clearScreen();
				LCD_moveCursor(0, 0);
				LCD_displayString("Pass doesn't matched");
				_delay_ms(1000);
				break;

			}
		}
		if (i == 5) //this condition indicate that the two password are matched
				{
			LCD_clearScreen();
			LCD_moveCursor(0, 0);
			LCD_displayString("Pass is correct");
			_delay_ms(1000);
			break;
		}

	}

	password_1[5] = '#';
	password_1[6] = '\0';
	LCD_clearScreen();
	LCD_moveCursor(0, 0);
	UART_sendString(password_1); //send the password taken from user
	while (1) {
		uint8 key;
		uint8 flag = 1;
		uint8 AK;
		LCD_clearScreen();
		LCD_displayString("+ :Open Door");
		LCD_moveCursor(1, 0);
		LCD_displayString("- :Change Pass");
		key = KEYPAD_getPressedKey();
		_delay_ms(750);
		UART_sendByte(key);
		switch (key) {
		case '+': {
			TakePassword(password_1);
			while (flag) {
				AK = UART_recieveByte();
				_delay_ms(100);
				if (AK == READY) {
					Timer0_Init(&timer0_config);
					LCD_clearScreen();
					LCD_displayString("Door is opening");
					while (Tick != DOOR_OPEN)
						;
					LCD_clearScreen();
					LCD_displayString("Door is hold");
					while (Tick != DOOR_HOLD)
						;
					LCD_clearScreen();
					LCD_displayString("Door is closing");
					while (Tick != DOOR_CLOSE)
						;
					Timer0_Deint();
					Tick = 0;
					flag = 0;
					wrong = 0;
				} else if (AK == NOT_READY) {
					wrong++;

					if (wrong == 3) {
						flag = 0;
						LCD_clearScreen();
						LCD_displayString("WARNING!!");
						Timer0_Init(&timer0_config);
						while (Tick != BUZZER)
							;
						Timer0_Deint();
						Tick = 0;
						flag = 0;
						wrong = 0;
						break;
					}
					TakePassword(password_1);

				}
			}
			break;
		}
		case '-':
			TakePassword(password_1);
			while (flag) {
				AK = UART_recieveByte();
				if (AK == READY) {
					while (1) //loop which will stay until the two password are matched
					{
						createPassword(password_1); //the main password of the system
						createPassword(password_2); //the password used for checking
						for (i = 0; i < 5; i++) //loop to check that two password are matched
								{
							if (password_1[i] == password_2[i]) {

							} else {
								LCD_clearScreen();
								LCD_moveCursor(0, 0);
								LCD_displayString("Pass doesn't matched");
								_delay_ms(1000);
								break;

							}
						}
						if (i == 5) //this condition indicate that the two password are matched
								{
							LCD_clearScreen();
							LCD_moveCursor(0, 0);
							LCD_displayString("Pass is correct");
							_delay_ms(1000);
							break;
						}

					}

					password_1[5] = '#';
					password_1[6] = '\0';
					UART_sendString(password_1);
					flag = 0;
					wrong = 0;
				} else if (AK == NOT_READY) {
					wrong++;
					if (wrong == 3) {
						flag = 0;
						LCD_clearScreen();
						LCD_displayString("WARNING!!");
						Timer0_Init(&timer0_config);
						while (Tick != BUZZER)
							;
						Timer0_Deint();
						Tick = 0;
						flag = 0;
						wrong = 0;
						break;
					}
					TakePassword(password_1);

				}
			}
			break;
		}
	}
}
