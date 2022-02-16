/******************************************************************************
 *
 * Module: Timer0
 *
 * File Name: timer0.h
 *
 * Description: Header file for the AVR Timer0 driver
 *
 * Author: Mohamed Elfeki
 *
 *******************************************************************************/

#ifndef TIMER0_H_
#define TIMER0_H_
#include "std_types.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum //Enum to determine F_CPU prescaler
{
	TIMER_NO_CLOCK,TIMER_F_CPU_CLOCK,TIMER_F_CPU_8,TIMER_F_CPU_64,TIMER_F_CPU_256,TIMER_F_CPU_1024
}Timer0_Prescaler;

typedef enum//Enum to determine Timer Mode
{
	OVF,CTC
}Timer0_Mode;

typedef struct
{
	Timer0_Prescaler prescaler;
	Timer0_Mode mode;
	uint16 initialValue;
	uint16 compValue;
}Timer0_ConfigType;



/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/* Description :
 * this is a function that is used to initialize Timer0
 * Timer0 could be initialized in compare mode or in over flow mode
 * we can determine the required frequency on which timer operate by using prescaler
 */
void Timer0_Init(const Timer0_ConfigType * Config_Ptr);
/*Description:
 *Function to set the Call Back function address.
 */
void Timer0_setCallBack(void(*a_ptr)(void));

/*Description:
 *Function to disable the Timer0 to stop Timer0
 */
void Timer0_Deint(void);



#endif /* TIMER0_H_ */
