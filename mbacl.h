#ifndef _MBGB_CL_H_
#define _MBCB_CL_H_

/**
	Function: mbgb_start_comunication
	Description: Starts the comunication with the Adapter
	Return:
		-1 when the received wrong adapter command
		0 when the received command is wrong
		1 on success
*/
int mbgb_start_comunication();

/**
	Function: mbgb_close_comunication
	Description: Close the comunication with the Adapter
*/
void mbgb_close_comunication();

/**
	Function: mbgb_is_line_busy
	Description: Check if the line is busy
	Return:
		-1 on error
		0 if the line is not busy
		1 if the line is busy
*/
int mbgb_is_line_busy();

/**
	Function: mbgb_dial_telephone
	Description: Dial the specified telephone
	
	NOTE: The telephone char must have '\0' as terminatior
*/
void mbgb_dial_telephone(unsigned char* phone);

/**
	Function: mbgb_login
	Description: Login to DION
	
	NOTE: Line must be free otherwise the login will fail
*/
int mbgb_login(unsigned char* username, unsigned char* password);

/**
	Function: mbgb_get_loginid
	Description: Get the Login ID (gXXXXXXXXX)
	
	The array MUST BE a 10 character or greater
*/


#endif /* _MBCB_CL_H_ */
