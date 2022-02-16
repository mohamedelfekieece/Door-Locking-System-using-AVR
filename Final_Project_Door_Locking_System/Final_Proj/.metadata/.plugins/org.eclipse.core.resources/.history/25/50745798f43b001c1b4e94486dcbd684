 /******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: motor.c
 *
 * Description: source file for DC Motor
 *
 * Author: Mohamed Elfeki
 *
 *******************************************************************************/

/*******************************************************************************
                        Includes
 *******************************************************************************/
#include"motor.h"
#include"gpio.h"
#include"common_macros.h"
#include"pwm.h"
#include"avr/io.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 Description
 The Function responsible for setup the direction for the two
 motor pins through the GPIO driver.
 Stop DC-Motor at the beginning through the GPIO driver.

 * */
void DcMotor_init(void)
{
	GPIO_setupPinDirection(PORTB_ID,PIN0_ID, PIN_OUTPUT);//set bit 0 in PORTB as output pin
	GPIO_setupPinDirection(PORTB_ID,PIN1_ID, PIN_OUTPUT);//set bit 1 in PORTB as output pin


	GPIO_writePin(PORTB_ID, PIN0_ID, LOGIC_LOW);//initialize the DC Motor OFF
	GPIO_writePin(PORTB_ID, PIN1_ID, LOGIC_LOW);//initialize the DC Motor OFF

}


/*
 Description:
 The function responsible for rotate the DC Motor CW/ or A-CW or
 stop the motor based on the state input state value.
 Send the required duty cycle to the PWM driver based on the
 required speed value.*/
void DcMotor_rotate(DcMotor_State state,uint8 speed)
{
	Timer0_PWM_Init(speed);//function to control speed of motor using Timer0 PWM mode

	if(state == STOP)//condition to determine the direction of rotation of the motor
	{
		GPIO_writePin(PORTB_ID, PIN0_ID, LOGIC_LOW);
		GPIO_writePin(PORTB_ID, PIN1_ID, LOGIC_LOW);
	}else if(state == CCW)
	{
		GPIO_writePin(PORTB_ID, PIN0_ID, LOGIC_LOW);
		GPIO_writePin(PORTB_ID, PIN1_ID, LOGIC_HIGH);
	}else if(state == CW)
	{
		GPIO_writePin(PORTB_ID, PIN0_ID, LOGIC_HIGH);
		GPIO_writePin(PORTB_ID, PIN1_ID, LOGIC_LOW);
	}

}
