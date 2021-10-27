/******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.c
 *
 * Description: Source file for the AVR TIMER driver
 *
 * Author: Kirollous Moheb
 *
 *******************************************************************************/

#include "timer.h"
#include "avr/io.h"
#include "common_macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
static volatile void (*g_TIMER0_callBackPtr)(void) = NULL_PTR;
static volatile void (*g_TIMER1_callBackPtr)(void) = NULL_PTR;
static volatile void (*g_TIMER2_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                      TIMER 0 ISR                               			    *
 *******************************************************************************/
ISR(TIMER0_OVF_vect)
{
	if(g_TIMER0_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_TIMER0_callBackPtr)(); /* another method to call the function using pointer to function g_TIMER0_callBackPtr(); */
	}
}

ISR(TIMER0_COMP_vect)
{
	if(g_TIMER0_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application*/
		(*g_TIMER0_callBackPtr)(); /* another method to call the function using pointer to function g_TIMER2_callBackPtr(); */
	}
}
/*******************************************************************************
 *                      TIMER 1 ISR                               			    *
 *******************************************************************************/
ISR(TIMER1_OVF_vect)
{
	if(g_TIMER1_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_TIMER1_callBackPtr)(); /* another method to call the function using pointer to function g_TIMER1_callBackPtr(); */
	}
}
ISR(TIMER1_COMPA_vect)
{
	if(g_TIMER1_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_TIMER1_callBackPtr)(); /* another method to call the function using pointer to function g_TIMER1_callBackPtr(); */
	}
}
/*******************************************************************************
 *                      TIMER 2 ISR                               			    *
 *******************************************************************************/
ISR(TIMER2_OVF_vect)
{
	if(g_TIMER2_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application*/
		(*g_TIMER2_callBackPtr)(); /* another method to call the function using pointer to function g_TIMER2_callBackPtr(); */
	}
}

ISR(TIMER2_COMP_vect)
{
	if(g_TIMER2_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application*/
		(*g_TIMER2_callBackPtr)(); /* another method to call the function using pointer to function g_TIMER2_callBackPtr(); */
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/


void TIMER_init(const Timer_Config* Config_Ptr){
	switch(Config_Ptr->timer_type){
	case TIMER0:
		TIMER_deinit(TIMER0);

		TIMER0_INITIAL_VALUE_REGISTER=(Config_Ptr->timer_initialValue)&(0XFF);
		/*add clock to first 3 bits*/
		TIMER0_CONTROL_REGISTER=(TIMER0_CONTROL_REGISTER&TIMER0_CLOCK_CLEAR_MASK)|(Config_Ptr->timer_clock);
		/*Clear COM00 and COM01*/
		CLEAR_BIT(TIMER0_CONTROL_REGISTER , TIMER0_COMPARE_OUTPUT_MODE_BIT0);
		CLEAR_BIT(TIMER0_CONTROL_REGISTER , TIMER0_COMPARE_OUTPUT_MODE_BIT1);
		switch(Config_Ptr->timer_mode){

		case NORMAL:

			/*
			 * Make FOC0 to be Active as it is overflow mode non-pwm
			 */
			TIMER0_CONTROL_REGISTER = SET_BIT(TIMER0_CONTROL_REGISTER, TIMER0_FORCE_OUTPUT_COMPARE_BIT);



			/*
			 * Normal Overflow mode:
			 * Clear WGM00 bit in TCCR0 register
			 * Clear WGM01 bit in TCCR0 register
			 */
			CLEAR_BIT(TIMER0_CONTROL_REGISTER , TIMER0_WAVE_FORM_GENERATION_BIT0);
			CLEAR_BIT(TIMER0_CONTROL_REGISTER , TIMER0_WAVE_FORM_GENERATION_BIT1);

			/*Enable Interrupts for overflow mode*/
			SET_BIT(TIMER0_INTERRUPT_MASK_REGISTER, TIMER0_OUTPUT_OVERFLOW_INTERRUPT);
			break;
		case COMPARE:


			TIMER0_COMPARE_VALUE_REGISTER = ((Config_Ptr->timer_compareValue)) & 0XFF;


			/*
			 * Make FOC0 to be Active as it is overflow mode non-pwm
			 */
			TIMER0_CONTROL_REGISTER = SET_BIT(TIMER0_CONTROL_REGISTER, TIMER0_FORCE_OUTPUT_COMPARE_BIT);


			/*
			 * Compare mode:
			 * Clear WGM00 bit in TCCR0 register
			 * Set WGM01 bit in TCCR0 register
			 */
			CLEAR_BIT(TIMER0_CONTROL_REGISTER , TIMER0_WAVE_FORM_GENERATION_BIT0);
			SET_BIT(TIMER0_CONTROL_REGISTER , TIMER0_WAVE_FORM_GENERATION_BIT1);

			/*Enable Interrupts for Compare mode*/
			SET_BIT(TIMER0_INTERRUPT_MASK_REGISTER, TIMER0_OUTPUT_COMPARE_MATCH_INTERRUPT);
			break;
		}


		break;
		case TIMER1:
			TIMER_deinit(TIMER1);
			TIMER1_INITIAL_VALUE_REGISTER=((Config_Ptr->timer_initialValue)) & 0XFFFF;

			TIMER1_CONTROL_REGISTER_B=(TIMER1_CONTROL_REGISTER_B&0XF8)|(Config_Ptr->timer_clock);
			/*
			 *
			 *              OC1A/OC1B disconnected
			 *              Clear COM1A0/COM1A1 bits in TCCR1A register
			 *              Clear COM1B0/COM1B1 bits in TCCR1A register
			 */
			TIMER1_CONTROL_REGISTER_A = TIMER1_CONTROL_REGISTER_A & 0X0F;


			switch(Config_Ptr->timer_mode){
			case NORMAL:
				/*
				 * Normal Overflow mode:
				 * Clear WGM10/WMG11 bit in TCCR1A register
				 *Clear WGM12/WGM13 bit in TCCR1B register
				 */
				TIMER1_CONTROL_REGISTER_A = TIMER1_CONTROL_REGISTER_A & 0XFC ;
				TIMER1_CONTROL_REGISTER_B = TIMER1_CONTROL_REGISTER_B & 0XE7 ;

				/*
				 * Make FOC1A/FOC1B to be Active as it is a non_pwm mode
				 */
				TIMER1_CONTROL_REGISTER_A =  SET_BIT(TIMER1_CONTROL_REGISTER_A,TIMER1_FORCE_OUTPUT_COMPARE_BIT_A);
				TIMER1_CONTROL_REGISTER_A =  SET_BIT(TIMER1_CONTROL_REGISTER_A,TIMER1_FORCE_OUTPUT_COMPARE_BIT_B);

				/*
				 * Enable Timer1 overflow interrupt
				 */
				TIMER1_INTERRUPT_MASK_REGISTER = SET_BIT(TIMER1_INTERRUPT_MASK_REGISTER,TIMER1_OUTPUT_OVERFLOW_INTERRUPT);

				break;
			case COMPARE:
				/*
				 * Compare mode:
				 * Clear WGM10/WGM11 bits in TCCR1A register
				 * Set WGM12 bit in TCCR1B register
				 * Clear WGM13 bit in TCCR1B register
				 */
				TIMER1_CONTROL_REGISTER_A = TIMER1_CONTROL_REGISTER_A & 0XFC ;
				TIMER1_CONTROL_REGISTER_B = SET_BIT(TIMER1_CONTROL_REGISTER_B, TIMER1_WAVE_FORM_GENERATION_BIT12);
				TIMER1_CONTROL_REGISTER_B = CLEAR_BIT(TIMER1_CONTROL_REGISTER_B, TIMER1_WAVE_FORM_GENERATION_BIT13);

				/*
				 *  Make FOC1A/FOC1B to be Active as it is compare mode
				 */
				TIMER1_CONTROL_REGISTER_A =  SET_BIT(TIMER1_CONTROL_REGISTER_A,TIMER1_FORCE_OUTPUT_COMPARE_BIT_A);
				TIMER1_CONTROL_REGISTER_A =  SET_BIT(TIMER1_CONTROL_REGISTER_A,TIMER1_FORCE_OUTPUT_COMPARE_BIT_B);

				/*
				 * Configure Compare match value for Timer1 to start count from it
				 * 0XFFFF so that value doesn't exceed 16_bits
				 */
				TIMER1_COMPARE_VALUE_REGISTER_A = ((Config_Ptr->timer_compareValue)) & 0XFFFF;

				/*
				 * Enable Timer1 compare match interrupt
				 */
				TIMER1_INTERRUPT_MASK_REGISTER = SET_BIT(TIMER1_INTERRUPT_MASK_REGISTER,TIMER1_OUTPUT_COMPARE_MATCH_INTERRUPT);
				break;
			}

			break;
			case TIMER2:
				TIMER_deinit(TIMER2);


				/*
				 * Configure initial value for Timer2 to start count from it
				 * 0XFF so that value doesn't exceed 8_bits
				 */

				TIMER2_INITIAL_VALUE_REGISTER = ( (Config_Ptr->timer_initialValue) ) & 0XFF;

				/*
				 * TIMER Pre-scaler value for Timer2 in TCCR2 Register
				 * 0XF8 to make sure that the least 3-bits in TCCR2 register=0
				 * ORing with the chosen timer Pre-scaler to enter it into the least 3-bits
				 */
				TIMER2_CONTROL_REGISTER = (TIMER2_CONTROL_REGISTER  & TIMER2_CLOCK_CLEAR_MASK) | (Config_Ptr->timer_clock);

				/*
				 *  Compare mode:
				 *              OC2 disconnected
				 *              Clear COM20 bit in TCCR2 register
				 *              Clear COM21 bit in TCCR2 register
				 */
				TIMER2_CONTROL_REGISTER  = CLEAR_BIT(TIMER2_CONTROL_REGISTER, TIMER2_COMPARE_OUTPUT_MODE_BIT0);
				TIMER2_CONTROL_REGISTER  = CLEAR_BIT(TIMER2_CONTROL_REGISTER, TIMER2_COMPARE_OUTPUT_MODE_BIT1);

				switch(Config_Ptr->timer_mode)
				{
				case NORMAL:

					/*
					 * Normal Overflow mode:
					 *                      Clear WGM00 bit in TCCR0 register
					 *                      Clear WGM01 bit in TCCR0 register
					 */
					TIMER2_CONTROL_REGISTER  = CLEAR_BIT(TIMER2_CONTROL_REGISTER , TIMER2_WAVE_FORM_GENERATION_BIT0);
					TIMER2_CONTROL_REGISTER  = CLEAR_BIT(TIMER2_CONTROL_REGISTER , TIMER2_WAVE_FORM_GENERATION_BIT1);

					/*
					 * Make FOC2 to be Active as it is overflow mode
					 */
					TIMER2_CONTROL_REGISTER =  SET_BIT(TIMER2_CONTROL_REGISTER ,TIMER2_FORCE_OUTPUT_COMPARE_BIT);


					/*
					 * Enable Timer2 overflow interrupt
					 */
					TIMER2_INTERRUPT_MASK_REGISTER = SET_BIT(TIMER2_INTERRUPT_MASK_REGISTER,TIMER2_OUTPUT_OVERFLOW_INTERRUPT);

					break;


				case COMPARE:

					/*
					 *  Compare mode:
					 *               Clear WGM20 bit in TCCR2 register
					 *               Clear WGM21 bit in TCCR2 register
					 */
					TIMER2_CONTROL_REGISTER  = CLEAR_BIT(TIMER2_CONTROL_REGISTER , TIMER2_WAVE_FORM_GENERATION_BIT0);
					TIMER2_CONTROL_REGISTER  = SET_BIT(TIMER2_CONTROL_REGISTER , TIMER2_WAVE_FORM_GENERATION_BIT1);

					/*
					 *  Make FOC2 to be Active as it is compare mode
					 */
					TIMER2_CONTROL_REGISTER =  SET_BIT(TIMER2_CONTROL_REGISTER , TIMER2_FORCE_OUTPUT_COMPARE_BIT);

					/*
					 * Configure compare match value for Timer2 to start count from it
					 * 0XFF so that value doesn't exceed 8_bits
					 */

					TIMER2_COMPARE_VALUE_REGISTER = ((Config_Ptr->timer_compareValue)) & 0XFF;

					/*
					 * Enable Timer2 compare match interrupt
					 */
					TIMER2_INTERRUPT_MASK_REGISTER = SET_BIT(TIMER2_INTERRUPT_MASK_REGISTER, TIMER2_OUTPUT_COMPARE_MATCH_INTERRUPT);


					break;
				}
	}
}



void TIMER_setCallBack(void(*a_ptr)(void),Timer_Type timer_type){
	switch(timer_type){
	case TIMER0:
		g_TIMER0_callBackPtr=a_ptr;
		break;
	case TIMER1:
		g_TIMER1_callBackPtr=a_ptr;
		break;
	case TIMER2:
		g_TIMER2_callBackPtr=a_ptr;
		break;
	}
}



void TIMER_stop(Timer_Type timer_type){
	switch(timer_type){
	case TIMER0:
		TIMER0_CONTROL_REGISTER=( TIMER0_CONTROL_REGISTER & TIMER0_CLOCK_CLEAR_MASK );
		break;
	case TIMER1:
		TIMER1_CONTROL_REGISTER_B=(TIMER1_CONTROL_REGISTER_B&TIMER1_CLOCK_CLEAR_MASK);
		break;
	case TIMER2:
		TIMER2_CONTROL_REGISTER=(TIMER2_CONTROL_REGISTER&TIMER2_CLOCK_CLEAR_MASK);

		break;
	}
}



void TIMER_deinit(Timer_Type timer_type){
	switch(timer_type){
	case TIMER0:
		TIMER0_CONTROL_REGISTER=0x00;
		TIMER0_INITIAL_VALUE_REGISTER=0x00;
		TIMER0_COMPARE_VALUE_REGISTER=0x00;
		TIMER0_INTERRUPT_MASK_REGISTER=0x00;
		break;
	case TIMER1:
		TIMER1_CONTROL_REGISTER_A=0;
		TIMER1_CONTROL_REGISTER_B=0;
		TIMER1_INITIAL_VALUE_REGISTER=0;
		TIMER1_COMPARE_VALUE_REGISTER_A=0;
		TIMER1_INTERRUPT_MASK_REGISTER=0;
		break;
	case TIMER2:
		TIMER2_CONTROL_REGISTER=0x00;
		TIMER2_INITIAL_VALUE_REGISTER=0x00;
		TIMER2_COMPARE_VALUE_REGISTER=0x00;
		TIMER2_INTERRUPT_MASK_REGISTER=0x00;
		break;
	}
}

