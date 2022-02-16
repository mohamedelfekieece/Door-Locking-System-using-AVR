/*
 * buzzer.h
 *
 *  Created on:
 *      Author:
 */

#ifndef BUZZER_H_
#define BUZZER_H_
#include "std_types.h"
/*******************************************************************************
 *                                 Definitions                                 *
 *******************************************************************************/
#define BUZZER_OUTPUT_PORT PORTD_ID// buzzer port
#define BUZZER_OUTPUT_PIN PIN2_ID//buzzer pin
/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
/*
 * description
 * initialize buzzer PIN as output
 */
void BUZZER_init();
/*
 * description
 * Turn on buzzer
 */
void BUZZER_on();
/*
 * description
 * Turn off buzzer
 */

void BUZZER_off();
#endif /* BUZZER_H_ */
