/******************************************************************************
 *
 * Main
 *
 * File Name: system.c
 *
 * Description: Source file for system MC main function
 *
 *Author: Mohamed Elfeki
 *******************************************************************************/


/********************************************************************************
 *                     Includes                              *
 *******************************************************************************/
#include"uart.h"
#include"timer0.h"
#include"util/delay.h"
#include"motor.h"
#include"lcd.h"
#include"buzzer.h"
#include"twi.h"
#include"external_eeprom.h"
/********************************************************************************
 *                     definitions                              *
 *******************************************************************************/
#define READY 1//show that entered password is correct
#define NOT_READY 2//show that entered password is wrong
#define DOOR_OPEN 918//no of the overflows made by timer until door is opened
#define DOOR_CLOSE 2020//no of the overflows made by timer until door is closed
#define DOOR_HOLD 1102//no of the overflows made by timer while door is hold
#define BUZZER 3662//no of the overflows made by timer until Turn off
/********************************************************************************
 *                      global variables                                  *
 *******************************************************************************/
Timer0_ConfigType timer0_config = { TIMER_F_CPU_1024, OVF, 0 };//configuration of timer0
uint8 savedPassword[5];//array take password saved in EEPROM
uint8 recievedPassword[7];//array to receive password given from MC1
uint16 Tick = 0;//count number of ticks
uint8 match;
/********************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/


/*
 * Description :
 * function used to save new password in EEPROM
 */

void savePassword(uint8 *recieved) {
	uint8 i;
	for (i = 0; i < 5; i++) {
		EEPROM_writeByte(0x0311 + i, recievedPassword[i]); //to save password in EEPROM
		_delay_ms(10);

	}

}
/*
 * Description :
 * function to increment number of ticks
 */

void Timer0_Tick(void) {
	Tick++;
}
/*
 * Description :
 * function to check if password given from MC is correct
 * if not correct increment number of wrong entries if number equal 3 fire buzzer
 */

void checkForPassword2(void) {
	uint8 password[5];//array take password saved in EEPROM
	uint8 count;//variable to count number of correct character

	uint8 fault = 0;//variable to count number of wrong entries
	while (fault != 3) {//loop won't terminate until wrong entries =3 or password matched
		UART_receiveString(recievedPassword);//UART receive password from MC1
		for (count = 0; count < 5; count++) {// loop to check that received password matched with one on EEPROM
			EEPROM_readByte(0x0311 + count, &password[count]);
			if (recievedPassword[count] != password[count]) {//condition if password don't match
				fault++;//increase no of wrong entries
				UART_sendByte(NOT_READY);//tell MC1 that password is wrong
				break;
			}
		}
		if (count == 5 && fault != 3) {// condition occur if password matched

			UART_sendByte(READY);//tell MC1 that password is True
			match = 1;//case for true password
			break;
		} else {
			match = 0;//case password is wrong
		}

	}
}

int main() {

	UART_ConfigType UART_Config = { ONE_STOP_BIT, EIGHT_BIT, 9600, DISABLED };//configuration of UART
	TWI_ConfigType TWI_Config = { TWI_NO_PRESCALER, 0X01 };//configuration of I2C
	TWI_init(&TWI_Config);//initialize I2c
	SREG |= (1 << 7);//enable global interrupt
	Timer0_setCallBack(Timer0_Tick); /*Set Timer0 Call back function */
	DcMotor_init();//initialize Motor
	BUZZER_init();//initialize Buzzer
	UART_init(&UART_Config);//initialize UART
	UART_receiveString(recievedPassword); //take the initial password from user
	savePassword(recievedPassword); //save the initial password from user in EEPROM
	while (1) {//main loop

		uint8 key = UART_recieveByte(); //receive whether if user want to open door or to change password
		switch (key) {

		case '+': {//case of open door

			checkForPassword2();//compare the password with one taken from UART
			_delay_ms(10);
			if (match == 1) {//case password matched
				Timer0_Init(&timer0_config);//initialize Timer0
				DcMotor_rotate(CW);//rotate motor CW for  15 seconds
				while (Tick != DOOR_OPEN);
				DcMotor_rotate(STOP);
				while (Tick != DOOR_HOLD);//hold motor for 3 seconds
				DcMotor_rotate(CCW);
				while (Tick != DOOR_CLOSE);//rotate motor CCW for  15 seconds
				DcMotor_rotate(STOP);//Stop Motor
				Timer0_Deint();//close Timer 0
				Tick = 0;
				break;
			}

			else if (match == 0) {//case entered wrong password 3 times
				Timer0_Init(&timer0_config);//initialize timer 0
				BUZZER_on();//turn on buzz for 1 minute
				while (Tick != BUZZER);
				BUZZER_off();//turn off buzz
				Timer0_Deint();
				Tick = 0;

			}
			break;
		}
		case '-': {//case of password change
			checkForPassword2();//compare the password with one taken from UART
			_delay_ms(100);
			if (match == 1) {
				UART_receiveString(recievedPassword);//take entered password by UART
				savePassword(recievedPassword);//save new password in EEPROM
				_delay_ms(100);
				break;
			}

			else if (match == 0) {//case entered wrong password 3 times
				Timer0_Init(&timer0_config);//initialize timer 0
				BUZZER_on();//turn on buzz for 1 minute
				while (Tick != BUZZER);
				BUZZER_off();//turn off buzz
				Timer0_Deint();
				Tick = 0;
				break;
			}
		}
		}
	}
}
