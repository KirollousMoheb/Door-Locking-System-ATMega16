/******************************************************************************
 *
 * File Name: HMI_ECU.C
 *
 * Description: Source file for the Microcontroller 2 main application
 *
 * Author: Kirollous Moheb
 *
 *******************************************************************************/

#include "CONTROL_ECU.h"

int main(void){
	/*Enable Global Interrupts*/
	SREG |= (1<<7);
	/*Initialize the UART*/
	UART_init(&uart_config);
	/*Initialize the Buzzer*/
	BUZZER_init();

	while(1){
		/*CONTROL ECU is always waiting for the HMI ECU stae's*/
		switch(CONTROL_receiveState()){
		/*If password is sent from HMI ECU*/
		case SEND_PASSWORD:
			/*Save the password in g_password_arr array*/
			CONTROL_receivePassword(g_password_arr);
			break;
		/*If confirmation password is sent from HMI ECU*/
		case SEND_CONFIRMATION_PASSWORD:
			/*Save the confirmation password in g_password_confirm_arr*/
			CONTROL_receivePassword(g_password_confirm_arr);
			/*Check if both password are matched*/
			if(CONTROL_matchPassword(g_password_arr,g_password_confirm_arr)){
				/*If they are matched:
				 * 1)Save the password in the memmory
				 * 2)Read the password from the memory and save it in g_saved_password array
				 * 3)g_saved_password array is the array to be used after this point
				 * 4)Send Matched state to HMI ECU
				 * */
				CONTROL_savePassInMemory();
				CONTROL_readPassFromMemory(g_saved_password);
				CONTROL_sendState(MATCHED);
			}else{
				/*Otherwise sent Not Matched state*/
				CONTROL_sendState(NOT_MATCHED);

			}
			break;


		case CHECK:
			/*Check state is sent from the HMI ECU in case of:
			 * 1)Opening the door
			 * 2)Changing the password
			 * It's used to check the user's entered password with the saved password in memory
			 * Whenever one of these two operations are needed
			 * */
			CONTROL_receivePassword(g_password_arr);
			CONTROL_readPassFromMemory(g_saved_password);
			/*If password is matched send Matched state and make number of wrong tries 0
			 * Otherwise send Not Matched
			 * */
			if(CONTROL_matchPassword(g_password_arr,g_saved_password)){
				CONTROL_sendState(MATCHED);
				g_wrong_tries=0;
			}else{
				CONTROL_sendState(NOT_MATCHED);
			}
			break;
			/*In case the Password is matched Open Door state is sent to open the door*/
		case OPEN_DOOR:
			/*Set the call back function to manage the time for the motor to :
			 * 1)Rotate clockwisely for 15 seconds
			 * 2)Stop for 3 seconds
			 * 3)Rotate Anti_Clockwisely for 15 seconds
			 * 4)Stop*/
			TIMER_setCallBack(Door_stateCallBack,TIMER0);
			/*Initialize the motor*/
			DcMotor_Init();
			/*Make the timer Starts counting*/
			TIMER_init(&timer_Config);
			break;


		case ALARM:
			/*Everytime the password is wrong in case of opening the door or changing the password
			 * HMI ECU receives Not Matched State and Sends Alarm state
			 * Each time we increment the number of wrong tries
			 * */
			g_wrong_tries++;
			if(g_wrong_tries==MAX_ERRORS){/*If number of wrong tries reaches the maximum allowable numbers*/
				/*We start the buzzer for one minute*/
				g_wrong_tries=0;
				g_alarm_ticks=0;
				TIMER_setCallBack(CONTROL_alarmCallBack,TIMER0);
				TIMER_init(&timer_Config);
				while(g_alarm_ticks<NUMBER_OF_OVERFLOWS_1_minute){
					BUZZER_on();
				}
				if(g_alarm_ticks==NUMBER_OF_OVERFLOWS_1_minute){
					BUZZER_off();
					TIMER_deinit(TIMER0);
					g_alarm_ticks=0;
				}

			}
			break;
		}
	}


	return 0;
}


/*******************************************************************************
 *                      Functions Definitions                                  *
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
void CONTROL_sendState(uint8 state){
	/*Send Ready for HMI ECU*/
	UART_sendByte(READY);
	/*Wait for HMI ECU to send back Ready state*/
	while(UART_recieveByte()!=READY);
	/*Send the required state to HMI ECU*/
	UART_sendByte(state);
	/*Wait for HMI ECU to send that it received the state*/
	while(UART_recieveByte()!=DONE);
}
/***************************************************************************************************
 * [Function Name]:          CONTROL_receiveState
 *
 * [Description]:            Function responsible for receiving the operation number be done by HMI ECU
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
uint8 CONTROL_receiveState(void){
	/*Wait until HMI ECU sends that it's ready*/
	while(UART_recieveByte() != READY){};
	/*Send Ready to HMI ECU to make it send the state*/
	UART_sendByte(READY);
	/*Receive the sent state in a global variable called state*/
	state=UART_recieveByte();
	/*Tell HMI ECU that the state it sent is received*/
	UART_sendByte(DONE);
	return state;
}
/***************************************************************************************************
 * [Function Name]:          CONTROL_matchPassword
 *
 * [Description]:            Function responsible for checking if the password and the confirmation password are matched
 *
 * [Arguments]:              The original password and the confirmation password
 *
 * [Returns]:                true or false
 ***************************************************************************************************/
boolean CONTROL_matchPassword(uint8 arr1[],uint8 arr2[]){
	/*Simple function comparing the two passwords array and return true if they match*/
	uint8 i=0;
	uint8 match_count=0;
	for(i=0;i<PASSWORD_SIZE;i++){
		if(arr1[i]==arr2[i]){
			match_count++;
		}
	}
	/*If match count is equal to the Password's size then all elements are matched and return true*/
	if(match_count==PASSWORD_SIZE){
		return TRUE;
		/*otherwise return false*/
	}else{
		return FALSE;
	}

}
/***************************************************************************************************
 * [Function Name]:          CONTROL_receivePassword
 *
 * [Description]:            Function responsible for receiving the password sent by HMI through Uart in an array
 *
 * [Arguments]:              An array where the password is to be saved
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void CONTROL_receivePassword(uint8 arr[]){

	uint8 i=0;
	for(i=0;i<PASSWORD_SIZE;i++){
		arr[i]=UART_recieveByte();
	}


}
/***************************************************************************************************
 * [Function Name]:          CONTROL_savePassInMemory
 *
 * [Description]:            Function responsible for saving the password after matching inside the EEPROM
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void CONTROL_savePassInMemory(void){
	EEPROM_init();
	int i=0;
	for(i=0;i<PASSWORD_SIZE;i++){
		/*The delay is essential to prevent writing or reading all the bytes in or from the same adress*/

		EEPROM_writeByte(EEPROM_ADRESS+i,g_password_arr[i]);
		_delay_ms(10);

	}
}
/***************************************************************************************************
 * [Function Name]:          CONTROL_readPassFromMemory
 *
 * [Description]:            Function responsible for reading the password saved inside the EEPROM
 *
 * [Arguments]:              Adress of the array which the password read from EEPROM is to be saved into
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void CONTROL_readPassFromMemory(uint8 *arr){
	EEPROM_init();
	int i=0;
	for(i=0;i<PASSWORD_SIZE;i++){
		EEPROM_readByte(EEPROM_ADRESS+i,&arr[i]);
		/*The delay is essential to prevent writing or reading all the bytes in or from the same adress*/
		_delay_ms(10);

	}
}
/***************************************************************************************************
 * [Function Name]:          Door_stateCallBack
 *
 * [Description]:            Call back function responsible for operating the motor according to timer0 ticks
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void Door_stateCallBack(void){
	g_ticks++;
	/*If timer counts is less than 15 seconds rotate the motor clockwise*/
	if(g_ticks<NUMBER_OF_OVERFLOWS_15_seconds){
		DcMotor_Rotate(CLOCKWISE);
	}
	/*If we reached 15 seconds stop the motor for 3 seconds*/
	if(g_ticks==NUMBER_OF_OVERFLOWS_15_seconds){
		DcMotor_Rotate(STOP);
	}
	/*After 3 seconds rotate the motor anti-clockwisely*/
	if(g_ticks==NUMBER_OF_OVERFLOWS_18_seconds){
		DcMotor_Rotate(ANTI_CLOCKWISE);
	}
	/*After another 15 seconds stop the motor and the timer*/
	if(g_ticks==NUMBER_OF_OVERFLOWS_33_seconds){
		DcMotor_Rotate(STOP);
		TIMER_deinit(TIMER0);
		g_ticks=0;
	}
}
/***************************************************************************************************
 * [Function Name]:          CONTROL_alarmCallBack
 *
 * [Description]:            Call back function responsible for operating the Buzzer for 1 minute
 *
 * [Arguments]:              VOID
 *
 * [Returns]:                VOID
 ***************************************************************************************************/
void CONTROL_alarmCallBack(void){
	g_alarm_ticks++;
	/*Count 1 minute for the buzzer*/
	if(g_ticks>NUMBER_OF_OVERFLOWS_1_minute){
		TIMER_deinit(TIMER0);
		g_alarm_ticks=0;
	}
}
