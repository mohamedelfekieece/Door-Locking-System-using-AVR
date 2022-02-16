/*
 * buzzer.c
 *
 *  Created on:
 *      Author:
 */
#include "buzzer.h"
#include "gpio.h"

void BUZZER_init() {

	GPIO_setupPinDirection(BUZZER_OUTPUT_PORT, BUZZER_OUTPUT_PIN, PIN_OUTPUT);
}
void BUZZER_on() {
	GPIO_writePin(BUZZER_OUTPUT_PORT, BUZZER_OUTPUT_PIN, LOGIC_HIGH);
}
void BUZZER_off() {
	GPIO_writePin(BUZZER_OUTPUT_PORT, BUZZER_OUTPUT_PIN, LOGIC_LOW);
}
