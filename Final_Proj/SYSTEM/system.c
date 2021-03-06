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
#include"uart.h"
#include"timer0.h"
#include"util/delay.h"
#include"motor.h"
#include"lcd.h"
#include"buzzer.h"
#include"twi.h"
#include"external_eeprom.h"
#define READY 1
#define NOT_READY 2
#define DOOR_OPEN 918
#define DOOR_CLOSE 2020
#define DOOR_HOLD 1102
#define BUZZER 3662
#define MCU_1_READY 0x10
Timer0_ConfigType timer0_config = { TIMER_F_CPU_1024, OVF, 0 };
/********************************************************************************
 *                      global variables                                  *
 *******************************************************************************/
uint8 savedPassword[5];
uint8 recievedPassword[7];
uint16 Tick = 0;
uint8 match;
/********************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
void savePassword(uint8 *recieved) {
	uint8 i;
	for (i = 0; i < 5; i++) {
		EEPROM_writeByte(0x0311 + i, recievedPassword[i]); //to save password in EEPROM
		_delay_ms(10);

	}

}

void readPassword(uint8 *savedPassword) {
	uint8 i;
	for (i = 0; i < 5; i++) {
		EEPROM_readByte(0x0311 + i, &savedPassword[i]); //to take password from EEPROM
		_delay_ms(10);

	}

}
void Timer0_Tick(void) {
	Tick++;
}


void checkForPassword2(void) {
	uint8 password[5];
	uint8 count;
	//UART_receiveString(recievedPassword);

	uint8 fault = 0;
	while (fault!=3) {
		UART_receiveString(recievedPassword);
		for (count = 0; count < 5; count++) {
			EEPROM_readByte(0x0311 + count, &password[count]);
			if (recievedPassword[count] != password[count]) {
				fault++;
				LCD_intgerToString(fault);
				UART_sendByte(NOT_READY);
				break;
			}
		}
		if (count == 5 && fault != 3) {

			UART_sendByte(READY);
			match = 1;
			break;
		} else {
			match = 0;
		}

	}
}

int main() {

	UART_ConfigType UART_Config = { ONE_STOP_BIT, EIGHT_BIT, 9600, DISABLED };
	TWI_ConfigType TWI_Config = { TWI_NO_PRESCALER, 0X01 };
	TWI_init(&TWI_Config);

	SREG |= (1 << 7);
	Timer0_setCallBack(Timer0_Tick); /*Set Timer0 Call back function */
	LCD_init();
	DcMotor_init();
	BUZZER_init();
	UART_init(&UART_Config);
	UART_receiveString(recievedPassword); //take the initial password from user
	savePassword(recievedPassword); //save the initial password from user in EEPROM
	while (1) {

		uint8 key = UART_recieveByte();
		switch (key) {

		case '+': {

			checkForPassword2();
			_delay_ms(10);
			if (match == 1) {
				Timer0_Init(&timer0_config);
				DcMotor_rotate(CW);
				while (Tick != DOOR_OPEN)
					;
				DcMotor_rotate(STOP);
				while (Tick != DOOR_HOLD)
					;
				DcMotor_rotate(CCW);
				while (Tick != DOOR_CLOSE)
					;
				DcMotor_rotate(STOP);
				Timer0_Deint();
				Tick = 0;
				break;
			}

			else if (match == 0) {
				Timer0_Init(&timer0_config);
				BUZZER_on();
				while (Tick != BUZZER)
					;
				BUZZER_off();
				Timer0_Deint();
				Tick = 0;

			}
			break;
		}
		case '-': {
			checkForPassword2();
			_delay_ms(100);
			if (match == 1) {
				UART_receiveString(recievedPassword);
				savePassword(recievedPassword);
				_delay_ms(100);
				break;
			}

			else if (match == 0) {
				Timer0_Init(&timer0_config);
				BUZZER_on();
				while (Tick != BUZZER)
					;
				BUZZER_off();
				Timer0_Deint();
				Tick = 0;
				break;
			}
		}
		}
	}
}
