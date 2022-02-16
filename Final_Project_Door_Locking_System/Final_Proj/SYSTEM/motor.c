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
#include"avr/io.h"
#include"std_types.h"

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
	GPIO_setupPinDirection(PORTC_ID,PIN6_ID, PIN_OUTPUT);//set bit 6 in PORTB as output pin
	GPIO_setupPinDirection(PORTC_ID,PIN7_ID, PIN_OUTPUT);//set bit 7 in PORTB as output pin


	GPIO_writePin(PORTC_ID, PIN6_ID, LOGIC_LOW);//initialize the DC Motor OFF
	GPIO_writePin(PORTC_ID, PIN7_ID, LOGIC_LOW);//initialize the DC Motor OFF

}


/*
 Description:
 The function responsible for rotate the DC Motor CW/ or A-CW or
 stop the motor based on the state input state value.
 */
void DcMotor_rotate(DcMotor_State state)
{

	if(state == STOP)//condition to determine the direction of rotation of the motor
	{
		GPIO_writePin(PORTC_ID, PIN6_ID, LOGIC_LOW);
		GPIO_writePin(PORTC_ID, PIN7_ID, LOGIC_LOW);
	}else if(state == CCW)
	{
		GPIO_writePin(PORTC_ID, PIN6_ID, LOGIC_LOW);
		GPIO_writePin(PORTC_ID, PIN7_ID, LOGIC_HIGH);
	}else if(state == CW)
	{
		GPIO_writePin(PORTC_ID, PIN6_ID, LOGIC_HIGH);
		GPIO_writePin(PORTC_ID, PIN7_ID, LOGIC_LOW);
	}

}
