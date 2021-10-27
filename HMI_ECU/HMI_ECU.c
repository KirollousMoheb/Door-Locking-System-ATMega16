/******************************************************************************
 *
 * File Name: HMI_ECU.c
 *
 * Description: source file for the Microcontroller 1 main application
 *
 * Author: Kirollous Moheb
 *
 *******************************************************************************/

#include "HMI_ECU.h"



int main(void){
	/*Enable Global Interrupts*/

	SREG |= (1<<7);
	/*Initialize the LCD*/

	LCD_init();
	/*Initialize the UART*/

	UART_init(&uart_config);

	while(1){
		if(menu_selector==0){
			HMI_createPasswordMenu();
		}
		if(menu_selector==1){
			HMI_optionsMenu();
		}
		if(menu_selector==2){
			HMI_doorStatesDisplay();
		}

	}


	return 0;
}

/*******************************************************************************
 *                      Functions Definitions                                  *
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
void HMI_sendState(uint8 state){
	/*Send Ready for CONTROL ECU*/
	UART_sendByte(READY);
	/*Wait for CONTROL ECU to send back Ready state*/
	while(UART_recieveByte()!=READY);
	/*Send the required state to CONTROL ECU*/
	UART_sendByte(state);
	/*Wait for CONTROL ECU to send that it received the state*/
	while(UART_recieveByte()!=DONE);
}
/***************************************************************************************************
 * [Function Name]:          HMI_receiveState
 *
 * [Description]:            Function responsible for receiving the operation number  be done by Control ECU
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
uint8 HMI_receiveState(void){
	/*Wait until CONTROL ECU sends that it's ready*/
	while(UART_recieveByte() != READY){};
	/*Send Ready to CONTROL ECU to make it send the state*/
	UART_sendByte(READY);
	/*Receive the sent state in a global variable called state*/
	state=UART_recieveByte();
	/*Tell CONTROL ECU that the state it sent is received*/
	UART_sendByte(DONE);
	return state;
}
/***************************************************************************************************
 * [Function Name]:          HMI_sendPassword
 *
 * [Description]:            Function responsible sending user enetered password using UART to Control ECU
 *
 * [Arguments]:              An array to be sent using Uart
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void HMI_sendPassword(uint8 arr[]){
	/*Send the password saved in an array to the CONTROL ECU through UART*/
	uint8 i=0;
	for(i=0;i<PASSWORD_SIZE;i++){
		UART_sendByte(arr[i]);
	}

}
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
void HMI_passwordMismatchDisplay(void){
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Password Doesn't match");
	_delay_ms(300);
}
/***************************************************************************************************
 * [Function Name]:          HMI_getPassword
 *
 * [Description]:            Function responsible for receiving a password from the user using the keypad
 *
 * [Arguments]:              An array where password will be saved
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void HMI_getPassword(uint8 arr[]){
	uint8 count=0;
	g_key_pressed=KEYPAD_getPressedKey();
	/*Take password until equal key is pressed*/
	while(g_key_pressed!='='){

		g_key_pressed=KEYPAD_getPressedKey();
		/*Don't save any character unless it's between 0 and 9*/
		if(g_key_pressed>=0&&g_key_pressed<=9){
			if(count<PASSWORD_SIZE){
				arr[count]=g_key_pressed;
				LCD_displayCharacter('*');
				count++;
			}
		}

		_delay_ms(350);
	}
}
/***************************************************************************************************
 * [Function Name]:          HMI_createPassword
 *
 * [Description]:            Function responsible for creating a password for the first time
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void HMI_createPassword(void){
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Please Enter Pass:");
	LCD_moveCursor(1,0);
	/*Get Password from the use and save it in g_password_arr array*/
	HMI_getPassword(g_password_arr);
	/*Send "SEND_PASSWORD" state to CONTROL ECU to receive  the password and save it in an array in the CONTROL ECU*/
	HMI_sendState(SEND_PASSWORD);
	/*Send the password to the CONTROL ECU by UART*/
	HMI_sendPassword(g_password_arr);
	create_confirm_flag=1;
}
/***************************************************************************************************
 * [Function Name]:          HMI_confirmPassword
 *
 * [Description]:            Function responsible for taking the confirmation  password entered the first time
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void HMI_confirmPassword(void){
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Please re-enter Password:");
	LCD_moveCursor(1,0);
	HMI_getPassword(g_password_confirm_arr);
	/*Send "SEND_CONFIRMATION_PASSWORD" state to CONTROL ECU to receive the
	 * confirmation password and save it in an array in the CONTROL ECU*/
	HMI_sendState(SEND_CONFIRMATION_PASSWORD);
	/*Send the confirmation password to the CONTROL ECU by UART*/
	HMI_sendPassword(g_password_confirm_arr);
	create_confirm_flag=2;
}
/***************************************************************************************************
 * [Function Name]:          HMI_createPasswordMenu
 *
 * [Description]:            Function responsible for displaying the creation password menu
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void HMI_createPasswordMenu(void){
	if(create_confirm_flag==0){
		HMI_createPassword();
	}
	if(create_confirm_flag==1){
		HMI_confirmPassword();
	}
	/*If create_confirm_flag==2
	 * This means we took the password and it's confirmation from the user*/
	if(create_confirm_flag==2){
		/*We check the state sent by the CONTROL ECU
		 * Telling us if the Password are matched or not*/
		switch(HMI_receiveState()){
		case MATCHED:
			/*If it's matched:
			 *1)make menu_selector=1 to go to the options menu
			 *2)Make create_confirm_flag=3 to not enter this part of the code  again
			 *3) Display "PASSWORD IS MATCHED AND SAVED"
			 * */
			menu_selector=1;
			create_confirm_flag=3;
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"PASSWORD IS MATCHED AND SAVED");
			_delay_ms(350);
			break;
		case NOT_MATCHED:
			/*If the passwords don't match display password mismatch for the user*/
			create_confirm_flag=0;
			HMI_passwordMismatchDisplay();
			_delay_ms(350);
		}
	}

}
/***************************************************************************************************
 * [Function Name]:          HMI_alarmCallBack
 *
 * [Description]:            Call back function for displaying alarm message
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void HMI_alarmCallBack(void){
	g_alarm_ticks++;
}
/***************************************************************************************************
 * [Function Name]:          HMI_alarmTrigger
 *
 * [Description]:            Function responsible for displaying an alarm message for 1 minute
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void HMI_alarmTrigger(void){
	/*This function is called each time the user enters a wrong password
	 * When the password is sent to the CONTROL ECU and checkd and it's found Not Matched
	 * HMI ECU then receives NOT MATCHED state
	 * It's called in this case
	 * */

	/*Sends "ALARM" state to the CONTROL ECU */
	HMI_sendState(ALARM);
	/*Display "WRONG PASSWORD!!" to the user*/
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"WRONG PASSWORD!!");
	_delay_ms(300);
	LCD_clearScreen();
	/*Increment the number of wrong tries*/
	g_wrong_tries++;
	/*If number of wrong tries reaches the maximum allowable numbers:
	 * 1)
	 * 2)
	 * 3)
	 * */
	if(g_wrong_tries==MAX_ERRORS){
		g_alarm_ticks=0;
		g_wrong_tries=0;
		TIMER_setCallBack(HMI_alarmCallBack,TIMER0);
		TIMER_init(&timer_Config);
		while(g_alarm_ticks<=NUMBER_OF_OVERFLOWS_1_minute)
		{

			LCD_displayStringRowColumn(0,0,"ALERT!!!!");
		}
		if(g_alarm_ticks==NUMBER_OF_OVERFLOWS_1_minute){
			menu_selector=1;
			TIMER_deinit(TIMER0);
			g_alarm_ticks=0;
		}

	}
}
/***************************************************************************************************
 * [Function Name]:          HMI_oldPasswordDisplay
 *
 * [Description]:            Function  displays to the user to enter the old password
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void HMI_oldPasswordDisplay(void){
	/*This function is used when the user needs to open the door or change the password
	 * The user is prompted to enter the old password
	 * */
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Please Enter old Pass:");
	LCD_moveCursor(1,0);
	/*Save the password in g_password_arr array*/
	HMI_getPassword(g_password_arr);
	/*Send "CHECK" state to the CONTROL ECU*/
	HMI_sendState(CHECK);
	/*Sends the password to the CONTROL ECU to check if it matches the saved password in the EEPROM*/
	HMI_sendPassword(g_password_arr);

}
/***************************************************************************************************
 * [Function Name]:          Door_stateDisplayCallBack
 *
 * [Description]:            Call back function for timer0 to calculate the time of displaying the door's states
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void Door_stateDisplayCallBack(void){
	g_ticks++;
	if(g_ticks==NUMBER_OF_OVERFLOWS_33_seconds){
		/*If we are done displaying the door's states and the door is stopped*/
		/*Go back to the option's menu using menu_selector=1*/
		menu_selector=1;
		/*Stop the timer*/
		TIMER_deinit(TIMER0);
		g_ticks=0;

	}

}
/***************************************************************************************************
 * [Function Name]:          HMI_optionsMenu
 *
 * [Description]:            Function displays the menu of options after creating a password succesfuly
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void HMI_optionsMenu(void){

	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"(+) Open Door");
	LCD_displayStringRowColumn(1,0,"(-) Change Password");
	switch(KEYPAD_getPressedKey()){

	case '+':
		HMI_oldPasswordDisplay();
		switch(HMI_receiveState()){
		case MATCHED:
			HMI_sendState(OPEN_DOOR);
			g_ticks=0;
			g_wrong_tries=0;
			g_alarm_ticks=0;
			menu_selector=2;
			break;
		case NOT_MATCHED:
			HMI_alarmTrigger();
			break;
		}
		break;
		case '-':
			HMI_oldPasswordDisplay();
			switch(HMI_receiveState()){
			case MATCHED:
				menu_selector=0;
				create_confirm_flag=0;
				g_wrong_tries=0;
				break;
			case NOT_MATCHED:
				HMI_alarmTrigger();
				break;
			}

			break;
	}


}
/***************************************************************************************************
 * [Function Name]:          HMI_doorStatesDisplay
 *
 * [Description]:            Function displays the door states when opening or closing
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void HMI_doorStatesDisplay(void){

	LCD_clearScreen();
	TIMER_init(&timer_Config);
	TIMER_setCallBack(Door_stateDisplayCallBack,TIMER0);

	while(g_ticks < NUMBER_OF_OVERFLOWS_15_seconds)
	{
		LCD_displayStringRowColumn(0,0,"DOOR IS UNLOCKING..");
	}
	while(g_ticks >=NUMBER_OF_OVERFLOWS_15_seconds&&g_ticks< NUMBER_OF_OVERFLOWS_18_seconds)
	{
		LCD_displayStringRowColumn(0,0,"DOOR IS NOW OPEN....");
	}
	while(g_ticks>= NUMBER_OF_OVERFLOWS_18_seconds && g_ticks < NUMBER_OF_OVERFLOWS_33_seconds)
	{

		LCD_displayStringRowColumn(0,0,"DOOR IS LOCKING..");
	}
	if(g_ticks == NUMBER_OF_OVERFLOWS_33_seconds)
	{
		menu_selector=1;
	}

}
