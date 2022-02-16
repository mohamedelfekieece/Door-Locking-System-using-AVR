/******************************************************************************
 *
 * Module: Timer0
 *
 * File Name: timer0.c
 *
 * Description: source file for the AVR Timer0 driver
 *
 * Author: Mohamed Elfeki
 *
 *******************************************************************************/
#include "common_macros.h" /* To use the macros like SET_BIT */
#include "timer0.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "std_types.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;
/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}



/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/* Description :
 * this is a function that is used to initialize Timer0
 * Timer0 could be initialized in compare mode or in over flow mode
 * we can determine the required frequency on which timer operate by using prescaler
 */
void Timer0_Init(const Timer0_ConfigType * Config_Ptr)
{
	TCNT0 = Config_Ptr->initialValue; //Set Timer initial value to 0
	TIMSK |= (1<<TOIE0)|(1<<OCIE0); // Enable Timer0 Overflow Interrupt and Timer0 compare Interrupt
	OCR0  = Config_Ptr->compValue; // Set Compare Value

	/* Configure the timer control register
	 * 1. Non PWM mode FOC0=1
	 * 2. Normal Mode WGM01=0 & WGM00=0
	 * 3. Normal Mode COM00=0 & COM01=0
	 */
	TCCR0 = (1<<FOC0);
	TCCR0=(TCCR0&0XF8)|Config_Ptr->prescaler;//determine the used prescaler
	TCCR0=(TCCR0&0XF7)|Config_Ptr->mode;//determine operation by OVF mode or Compare mode

}

/*Description:
 *Function to set the Call Back function address.
 */
void Timer0_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = a_ptr;
}
/*Description:
 *Function to disable the Timer0 to stop Timer0
 */
void Timer0_Deint(void)
{
TCCR0=0;
TCNT0=0;
}






