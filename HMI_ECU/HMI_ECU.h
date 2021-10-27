/******************************************************************************
 *
 * File Name: HMI_ECU.h
 *
 * Description: Header file for the Microcontroller 1 main application
 *
 * Author: Kirollous Moheb
 *
 *******************************************************************************/

#ifndef HMI_ECU_H_
#define HMI_ECU_H_


#include "timer.h"
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "avr/interrupt.h"
#include "std_types.h"
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
uint8 g_wrong_tries=0;
uint8 g_key_pressed;
uint8 state;
uint8 menu_selector=0;
uint8 create_confirm_flag=0;
uint16 g_ticks=0;
uint16 g_alarm_ticks=0;
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/


/***************************************************************************************************
 * [Function Name]:          HMI_sendState
 *
 * [Description]:            Function responsible for sending the operation number to be done by HMI ECU
 *
 * [Arguments]:              uint8 state
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void HMI_sendState(uint8 state);
/***************************************************************************************************
 * [Function Name]:          HMI_receiveState
 *
 * [Description]:            Function responsible for receiving the operation number  be done by Control ECU
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
uint8 HMI_receiveState(void);
/***************************************************************************************************
 * [Function Name]:          HMI_sendPassword
 *
 * [Description]:            Function responsible sending user enetered password using UART to Control ECU
 *
 * [Arguments]:              An array to be sent using Uart
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void HMI_sendPassword(uint8 arr[]);
/***************************************************************************************************
 * [Function Name]:          HMI_passwordMismatchDisplay
 *
 * [Description]:            Function responsible for displaying a message on the LCD Screen when a mismatch
 * 							 occurs when creating a password
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void HMI_passwordMismatchDisplay(void);
/***************************************************************************************************
 * [Function Name]:          HMI_getPassword
 *
 * [Description]:            Function responsible for receiving a password from the user using the keypad
 *
 * [Arguments]:              An array where password will be saved
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void HMI_getPassword(uint8 arr[]);
/***************************************************************************************************
 * [Function Name]:          HMI_createPassword
 *
 * [Description]:            Function responsible for creating a password for the first time
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void HMI_createPassword(void);
/***************************************************************************************************
 * [Function Name]:          HMI_confirmPassword
 *
 * [Description]:            Function responsible for taking the confirmation  password entered the first time
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void HMI_confirmPassword(void);
/***************************************************************************************************
 * [Function Name]:          HMI_createPasswordMenu
 *
 * [Description]:            Function responsible for displaying the creation password menu
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void HMI_createPasswordMenu(void);
/***************************************************************************************************
 * [Function Name]:          HMI_alarmCallBack
 *
 * [Description]:            Call back function for displaying alarm message
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void HMI_alarmCallBack(void);
/***************************************************************************************************
 * [Function Name]:          HMI_alarmTrigger
 *
 * [Description]:            Function responsible for displaying an alarm message for 1 minute
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void HMI_alarmTrigger(void);
/***************************************************************************************************
 * [Function Name]:          HMI_oldPasswordDisplay
 *
 * [Description]:            Function  displays to the user to enter the old password
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void HMI_oldPasswordDisplay(void);
/***************************************************************************************************
 * [Function Name]:          Door_stateDisplayCallBack
 *
 * [Description]:            Call back function for timer0 to calculate the time of displaying the door's states
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void Door_stateDisplayCallBack(void);
/***************************************************************************************************
 * [Function Name]:          HMI_optionsMenu
 *
 * [Description]:            Function displays the menu of options after creating a password succesfuly
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void HMI_optionsMenu(void);
/***************************************************************************************************
 * [Function Name]:          HMI_doorStatesDisplay
 *
 * [Description]:            Function displays the door states when opening or closing
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void HMI_doorStatesDisplay(void);


#endif /* HMI_ECU_H_ */
