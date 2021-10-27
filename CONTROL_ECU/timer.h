/******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.h
 *
 * Description: Header file for the AVR TIMER driver
 *
 * Author: Kirollous Moheb
 *
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_
#include "std_types.h"
/*******************************************************************************
 *                                DEFINITIONS		                            *
 *******************************************************************************/

/*******************************************************************************
 *                                TIMER0 REGISTERS                              *
 *******************************************************************************/


#define TIMER0_CONTROL_REGISTER				TCCR0
#define TIMER0_INITIAL_VALUE_REGISTER		TCNT0
#define TIMER0_COMPARE_VALUE_REGISTER		OCR0
#define TIMER0_INTERRUPT_MASK_REGISTER		TIMSK
#define TIMER0_INTERRUPT_FLAG_REGISTER		TIFR
/*******************************************************************************
 *                                TIMER0 PINS                              *
 *******************************************************************************/
/*TIMER1_CONTROL_REGIRSTER PINS*/

#define TIMER0_FORCE_OUTPUT_COMPARE_BIT       FOC0
#define TIMER0_WAVE_FORM_GENERATION_BIT0      WGM00
#define TIMER0_WAVE_FORM_GENERATION_BIT1      WGM01
#define TIMER0_COMPARE_OUTPUT_MODE_BIT0       COM00
#define TIMER0_COMPARE_OUTPUT_MODE_BIT1       COM01
#define TIMER0_CLOCK_SELECT_BIT0              CS00
#define TIMER0_CLOCK_SELECT_BIT1              CS01
#define TIMER0_CLOCK_SELECT_BIT2              CS02

/*TIMER1_INTERRUPT_FLAG_REGISTER PINS*/

#define TIMER0_OUTPUT_OVERFLOW_INTERRUPT       TOIE0
#define TIMER0_OUTPUT_COMPARE_MATCH_INTERRUPT  OCIE0


/*TIMER_0_CLOCK_CLEAR_MASK*/
#define TIMER0_CLOCK_CLEAR_MASK        			0XF8
/*******************************************************************************
 *                                TIMER1 REGISTERS                              *
 *******************************************************************************/


#define TIMER1_CONTROL_REGISTER_A			TCCR1A
#define TIMER1_CONTROL_REGISTER_B			TCCR1B
#define TIMER1_INITIAL_VALUE_REGISTER		TCNT1
#define TIMER1_COMPARE_VALUE_REGISTER_A		OCR1A
#define TIMER1_INTERRUPT_MASK_REGISTER		TIMSK
#define TIMER1_INTERRUPT_FLAG_REGISTER		TIFR
/*******************************************************************************
 *                                TIMER1 PINS                              *
 *******************************************************************************/
/*TIMER1_CONTROL_REGIRSTER PINS*/
#define TIMER1_FORCE_OUTPUT_COMPARE_BIT_A       FOC1A
#define TIMER1_FORCE_OUTPUT_COMPARE_BIT_B       FOC1B
#define TIMER1_WAVE_FORM_GENERATION_BIT12      WGM12
#define TIMER1_WAVE_FORM_GENERATION_BIT13      WGM13
/*TIMER1_INTERRUPT_FLAG_REGISTER PINS*/
#define TIMER1_OUTPUT_OVERFLOW_INTERRUPT       TOIE1
#define TIMER1_OUTPUT_COMPARE_MATCH_INTERRUPT  OCIE1A


/*TIMER_1_CLOCK_CLEAR_MASK*/
#define TIMER1_CLOCK_CLEAR_MASK        			0XF8

/*******************************************************************************
 *                                TIMER2 REGISTERS                              *
 *******************************************************************************/


#define TIMER2_CONTROL_REGISTER				TCCR2
#define TIMER2_INITIAL_VALUE_REGISTER		TCNT2
#define TIMER2_COMPARE_VALUE_REGISTER		OCR2
#define TIMER2_INTERRUPT_MASK_REGISTER		TIMSK
#define TIMER2_INTERRUPT_FLAG_REGISTER		TIFR
/*******************************************************************************
 *                                TIMER2 PINS                              *
 *******************************************************************************/
/*TIMER2_CONTROL_REGIRSTER PINS*/
#define TIMER2_FORCE_OUTPUT_COMPARE_BIT       FOC2
#define TIMER2_WAVE_FORM_GENERATION_BIT0      WGM20
#define TIMER2_WAVE_FORM_GENERATION_BIT1      WGM21
#define TIMER2_COMPARE_OUTPUT_MODE_BIT0       COM20
#define TIMER2_COMPARE_OUTPUT_MODE_BIT1       COM21
#define TIMER2_CLOCK_SELECT_BIT0              CS20
#define TIMER2_CLOCK_SELECT_BIT1              CS21
#define TIMER2_CLOCK_SELECT_BIT2              CS22

/*TIMER2_INTERRUPT_FLAG_REGISTER PINS*/

#define TIMER2_OUTPUT_OVERFLOW_INTERRUPT       TOIE2
#define TIMER2_OUTPUT_COMPARE_MATCH_INTERRUPT  OCIE2
/*TIMER_2_CLOCK_CLEAR_MASK*/
#define TIMER2_CLOCK_CLEAR_MASK        			0XF8





/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

typedef enum{
	TIMER0_NO_CLOCK,TIMER0_F_CPU_CLOCK,TIMER0_F_CPU_8,TIMER0_F_CPU_64,TIMER0_F_CPU_256,TIMER0_F_CPU_1024
}Timer0_Clock;
typedef enum{
	TIMER1_NO_CLOCK,TIMER1_F_CPU_CLOCK,TIMER1_F_CPU_8,TIMER1_F_CPU_64,TIMER1_F_CPU_256,TIMER1_F_CPU_1024
}Timer1_Clock;
typedef enum{
	TIMER2_NO_Clock,TIMER2_F_CPU_CLOCK,TIMER2_F_CPU_8,TIMER2_F_CPU_32,TIMER2_F_CPU_64,TIMER2_F_CPU_128,TIMER2_F_CPU_256,TIMER2_F_CPU_1024
}Timer2_Clock;

typedef enum{
	NORMAL,COMPARE
}Timer_Mode;
typedef enum{
	TIMER0,TIMER1,TIMER2
}Timer_Type;

typedef struct{
	Timer_Type	timer_type;
	uint8 		timer_clock;
	Timer_Mode	timer_mode;
	uint32		timer_initialValue;
	uint32		timer_compareValue;

}Timer_Config;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void TIMER_init(const Timer_Config* Config_Ptr);
void TIMER_setCallBack(void(*a_ptr)(void),Timer_Type timer_type);
void TIMER_stop(Timer_Type timer_type);
void TIMER_deinit(Timer_Type timer_type);

#endif /* TIMER_H_ */
