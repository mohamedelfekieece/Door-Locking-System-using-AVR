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
/********************************************************************************
 *                     definitions                              *
 *******************************************************************************/
#define READY 1 //show that entered password is correct
#define NOT_READY 2//show that entered password is wrong
#define DOOR_OPEN 918//no of the overflows made by timer until door is opened
#define DOOR_CLOSE 2020//no of the overflows made by timer until door is closed
#define DOOR_HOLD 1102//no of the overflows made by timer while door is hold
#define BUZZER 3662//no of the overflows made by timer until Turn off
/********************************************************************************
 *                      global variables                                  *
 *******************************************************************************/
Timer0_ConfigType timer0_config = { TIMER_F_CPU_1024, OVF, 0 };//configuration of timer0
uint16 Tick = 0;//count number of ticks


/********************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * function used to take new password from the user
 */
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
/*
 * Description :
 * function used to take old password from the user and send it using UART
 */
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
/*
 * Description :
 * function to increment number of ticks
 */

void Timer0_Tick(void) {
	Tick++;
}

/********************************************************************************
 *                      Main function                             *
 *******************************************************************************/

int main(void) {
	UART_ConfigType UART_Config = { ONE_STOP_BIT, EIGHT_BIT, 9600, DISABLED };//configuration of UART
	uint8 i;
	uint8 wrong = 0;//variable to count number of wrong entries
	uint8 password_1[7];//array to save the the entered password from user
	uint8 password_2[5];//array to save the the reentered password from user
	SREG |= (1 << 7);//enable global interrupt
	Timer0_setCallBack(Timer0_Tick); /*Set Timer0 Call back function */

	LCD_init(); //function to initialize LCD Driver
	UART_init(&UART_Config); //function to initialize UART Driver

	while (1) //loop which will stay until the two password are matched
	{
		createPassword(password_1); //the main password of the system
		createPassword(password_2); //the password used for checking
		for (i = 0; i < 5; i++) //loop to check that two password are matched
				{
			if (password_1[i] == password_2[i]) { //do nothing

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
	while (1) {//main loop
		uint8 key;//variable to know if user want to open door or to change password
		uint8 flag = 1;
		uint8 AK;//variable to know if entered password is matched with the saved password
		LCD_clearScreen();
		/*
		 * ask user whether he wants to open door or change password
		 */
		LCD_displayString("+ :Open Door");
		LCD_moveCursor(1, 0);
		LCD_displayString("- :Change Pass");
		key = KEYPAD_getPressedKey();//take the action made by user
		_delay_ms(750);
		UART_sendByte(key);//send the required action to the 2nd MC
		/*
		 * if user want to open door he will choose case 1
		 * if user want to change password he will choose case 2
		 */
		switch (key) {
		case '+': { //if user wanted to open the door
			TakePassword(password_1);//take old password from user and send it to MC2 to check that it matches with old pass
			while (flag) { //loop which w'ont terminate till password matches or user enter wrong pass 3 times
				AK = UART_recieveByte();//MC1 take message from MC2 to till him if password matched or not
				_delay_ms(100);
				if (AK == READY) { //if password matched
					Timer0_Init(&timer0_config);//initialize Timer0
					LCD_clearScreen();
					LCD_displayString("Door is opening");
					while (Tick != DOOR_OPEN);//polling for 15 seconds
					LCD_clearScreen();
					LCD_displayString("Door is hold");
					while (Tick != DOOR_HOLD);//polling for 3 seconds
					LCD_clearScreen();
					LCD_displayString("Door is closing");
					while (Tick != DOOR_CLOSE);//polling for 15 seconds
					Timer0_Deint();//close Timer0
					Tick = 0;
					flag = 0;
					wrong = 0;
				} else if (AK == NOT_READY) { //if password doesn't match
					wrong++;//increase number of wrong entries

					if (wrong == 3) { //number of wrong entries =3 display error message for 1 minute
						flag = 0;
						LCD_clearScreen();
						LCD_displayString("WARNING!!");
						Timer0_Init(&timer0_config);
						while (Tick != BUZZER);//polling for 1 minute
						Timer0_Deint();//close Timer0
						Tick = 0;
						flag = 0;
						wrong = 0;
						break;//break from loop
					}
					TakePassword(password_1);//take old password again

				}
			}
			break;//the break of case '+'
		}
		case '-':{
			TakePassword(password_1);
			while (flag) {//loop which w'ont terminate till password matches or user enter wrong pass 3 times
				AK = UART_recieveByte();
				if (AK == READY) {//if password matched
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
					UART_sendString(password_1);//if the new password is matched when entered two times send new password to MC2 to save it
					flag = 0;
					wrong = 0;
				} else if (AK == NOT_READY) {//if password doesn't match
					wrong++;//increase number of wrong entries
					if (wrong == 3) { //number of wrong entries =3 display error message for 1 minute
						flag = 0;
						LCD_clearScreen();
						LCD_displayString("WARNING!!");
						Timer0_Init(&timer0_config);
						while (Tick != BUZZER);//polling for 1 minute
						Timer0_Deint();//close Timer0
						Tick = 0;
						flag = 0;
						wrong = 0;
						break;//break from loop
					}
					TakePassword(password_1);//take password again as user entered wrong password

				}
			}
			break;//the break of case '-'
		}
		}
	}
}
