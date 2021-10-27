/*
 * buzzer.h
 *
 *  Created on: Oct 20, 2021
 *      Author: koko
 */

#ifndef BUZZER_H_
#define BUZZER_H_


#define BUZZER_PORT PORTB_ID
#define BUZZER_PIN	PIN7_ID


void BUZZER_init(void);
void BUZZER_on(void);
void BUZZER_off(void);

#endif /* BUZZER_H_ */
