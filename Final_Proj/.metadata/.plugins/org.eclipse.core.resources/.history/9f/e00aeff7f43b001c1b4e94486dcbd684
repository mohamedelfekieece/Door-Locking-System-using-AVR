 /******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: motor.h
 *
 * Description: header file for DC Motor
 *
 * Author: Mohamed Elfeki
 *
 *******************************************************************************/

#ifndef MOTOR_H_
#define MOTOR_H_

/*******************************************************************************
                                  Includes
 *******************************************************************************/
#include"gpio.h"
#include"common_macros.h"
#include"pwm.h"
#include"avr/io.h"

/*******************************************************************************
 *                                enums                                 *
 *******************************************************************************/
typedef enum {

	STOP,CW,CCW

}DcMotor_State;

/*******************************************************************************
 *                                FUNCTIONS PROTOTYPES                                 *
 *******************************************************************************/
void DcMotor_init(void);
/*
 Description
 The Function responsible for setup the direction for the two
 motor pins through the GPIO driver.
 Stop DC-Motor at the beginning through the GPIO driver.
 * */
void DcMotor_rotate(DcMotor_State state,uint8 speed);
/*
 Description:
 The function responsible for rotate the DC Motor CW/ or A-CW or
 stop the motor based on the state input state value.
 Send the required duty cycle to the PWM driver based on the
 required speed value.*/



#endif /* MOTOR_H_ */

