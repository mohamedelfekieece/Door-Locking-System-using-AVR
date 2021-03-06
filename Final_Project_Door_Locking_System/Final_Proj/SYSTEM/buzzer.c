 /******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.c
 *
 * Description: source file for buzzer driver
 *
 * Author: Mohamed Elfeki
 *
 *******************************************************************************/
#include "buzzer.h"
#include "gpio.h"
/*
 * description
 * initialize buzzer PIN as output
*/

void BUZZER_init() {

	GPIO_setupPinDirection(BUZZER_OUTPUT_PORT, BUZZER_OUTPUT_PIN, PIN_OUTPUT);
}
/*
 * description
 * Turn on buzzer
 */



void BUZZER_on() {
	GPIO_writePin(BUZZER_OUTPUT_PORT, BUZZER_OUTPUT_PIN, LOGIC_HIGH);
}
/*
 * description
 * Turn off buzzer
 */
void BUZZER_off() {
	GPIO_writePin(BUZZER_OUTPUT_PORT, BUZZER_OUTPUT_PIN, LOGIC_LOW);
}
