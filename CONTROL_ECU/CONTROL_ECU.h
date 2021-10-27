/******************************************************************************
 *
 * File Name: CONTROL_ECU.h
 *
 * Description: Header file for the Microcontroller 2 main application
 *
 * Author: Kirollous Moheb
 *
 *******************************************************************************/
#ifndef CONTROL_ECU_H_
#define CONTROL_ECU_H_

#include"std_types.h"
#include"uart.h"
#include"common_macros.h"
#include "avr/interrupt.h"

#include"timer.h"
#include "motor.h"
#include "buzzer.h"
#include "external_eeprom.h"
#include <util/delay.h>

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define READY								 0X01
#define DONE								 0X02
#define SEND_PASSWORD						 0X03
#define SEND_CONFIRMATION_PASSWORD			 0X04
#define OPEN_DOOR							 0X05
#define MATCHED								 0X06
#define NOT_MATCHED							 0X07
#define CHECK								 0X08
#define ALARM								 0X09
#define PASSWORD_SIZE						 5
#define MAX_ERRORS							 3



#define NUMBER_OF_OVERFLOWS_15_seconds  	457
#define NUMBER_OF_OVERFLOWS_18_seconds  	548
#define NUMBER_OF_OVERFLOWS_33_seconds  	1005
#define NUMBER_OF_OVERFLOWS_1_minute	  	1831
#define EEPROM_ADRESS						0X0028

/*******************************************************************************
 *                               Configurations                                 *
 *******************************************************************************/
Timer_Config timer_Config={TIMER0,TIMER0_F_CPU_1024,COMPARE,0,255};
UART_config uart_config={ASYNC,9600,DISABLED,ONE,EIGHT};
/*******************************************************************************
 *                                Global Variables                              *
 *******************************************************************************/
uint8 g_password_arr[PASSWORD_SIZE];
uint8 g_password_confirm_arr[PASSWORD_SIZE];
uint8 g_saved_password[PASSWORD_SIZE];
uint8 g_wrong_tries=0;
uint8 state;
uint16 g_ticks=0;
uint16 g_alarm_ticks=0;
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/***************************************************************************************************
 * [Function Name]:          CONTROL_sendState
 *
 * [Description]:            Function responsible for sending the operation number to be done by Control ECU
 *
 * [Arguments]:              uint8 state
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void CONTROL_sendState(uint8 state);
/***************************************************************************************************
 * [Function Name]:          CONTROL_receiveState
 *
 * [Description]:            Function responsible for receiving the operation number be done by HMI ECU
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
uint8 CONTROL_receiveState(void);
/***************************************************************************************************
 * [Function Name]:          CONTROL_matchPassword
 *
 * [Description]:            Function responsible for checking if the password and the confirmation password are matched
 *
 * [Arguments]:              The original password and the confirmation password
 *
 * [Returns]:                true or false
 ***************************************************************************************************/
boolean CONTROL_matchPassword(uint8 arr1[],uint8 arr2[]);
/***************************************************************************************************
 * [Function Name]:          CONTROL_receivePassword
 *
 * [Description]:            Function responsible for receiving the password sent by HMI through Uart in an array
 *
 * [Arguments]:              An array where the password is to be saved
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void CONTROL_receivePassword(uint8 arr[]);
/***************************************************************************************************
 * [Function Name]:          CONTROL_savePassInMemory
 *
 * [Description]:            Function responsible for saving the password after matching inside the EEPROM
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void CONTROL_savePassInMemory(void);
/***************************************************************************************************
 * [Function Name]:          CONTROL_readPassFromMemory
 *
 * [Description]:            Function responsible for reading the password saved inside the EEPROM
 *
 * [Arguments]:              Adress of the array which the password read from EEPROM is to be saved into
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void CONTROL_readPassFromMemory(uint8 *arr);
/***************************************************************************************************
 * [Function Name]:          Door_stateCallBack
 *
 * [Description]:            Call back function responsible for operating the motor according to timer0 ticks
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void Door_stateCallBack(void);
/***************************************************************************************************
 * [Function Name]:          CONTROL_alarmCallBack
 *
 * [Description]:            Call back function responsible for operating the Buzzer for 1 minute
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void CONTROL_alarmCallBack(void);



#endif /* CONTROL_ECU_H_ */
