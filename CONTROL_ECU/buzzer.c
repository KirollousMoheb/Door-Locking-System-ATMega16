/*
 * buzzer.c
 *
 *  Created on: Oct 20, 2021
 *      Author: koko
 */

#include "gpio.h"
#include "buzzer.h"

void BUZZER_init(){
	GPIO_setupPinDirection(BUZZER_PORT,BUZZER_PIN,PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT,BUZZER_PIN,LOGIC_LOW);
}
void BUZZER_on(){
	GPIO_writePin(BUZZER_PORT,BUZZER_PIN,LOGIC_HIGH);
}
void BUZZER_off(){
	GPIO_writePin(BUZZER_PORT,BUZZER_PIN,LOGIC_LOW);
}
