/**
	Mobile Adapter GB Comunication Library
	
	This library allows the comunication of a GameBoy Color/Advance
	with a Mobile Adapter GB
	
	TODO: Add pretty much the rest
	TODO: Add checksum verification code in receive
*/
#include <gb/gb.h>
#include <stdio.h>

#include "mbacl.h"

#include <stdio.h>

#define DBG_ALL

/**
	This function is the external sending function used in Gameboy Printer (contained inside comm.s)
*/
extern unsigned char SendPrinterByte(unsigned char byte);

#define RecvPrinterByte() SendPrinterByte(0x4B)

/**
	Function: _send_command_extra
	Description: Internal function used to send command to the adapter
	Parameters:
		command = The command number
		body_length = The length of the body
		body = An array that contains the body data to be sended
		extra_body_length = The length of the extra body
		extra_body = The length of the extra body
*/
void _send_command_extra(unsigned char command, unsigned char body_length, unsigned char* body, unsigned char extra_body_length, unsigned char* extra_body)
{
	unsigned char i = 0;
	unsigned int checksum = 0;
	
	SendPrinterByte(0x99); // Preamble
	SendPrinterByte(0x66); // Start Packet
	SendPrinterByte(command); // Command ID
	SendPrinterByte(0x00); // Packet 1
	SendPrinterByte(0x00); // Packet 2
	SendPrinterByte(body_length+extra_body_length);

	checksum = command + body_length;
	
	if (extra_body != NULL && extra_body_length > 0)
		checksum += extra_body_length;
	
	for (; i < body_length; i++)
	{ 
		SendPrinterByte(*(body+i));
		checksum += (*(body+i)); 
	}
	
	for (i = 0; i < extra_body_length; i++)
	{
		SendPrinterByte(*(extra_body+i));
		checksum += (*(extra_body+i));
	}
	
	SendPrinterByte(checksum>>8);
	SendPrinterByte(checksum&0xFF);
	SendPrinterByte(0x80); // Device ID
	SendPrinterByte(0x00); // Status byte
}

/**
	Function: _send_command
	Description: Internal function used to send command to the adapter
	Parameters:
		command = The command number
		body_length = The length of the body
		body = An array that contains the body data to be sended
*/
void _send_command(unsigned char command, unsigned char body_length, unsigned char* body)
{
	_send_command_extra(command, body_length, body, 0, NULL);
}

/**
	Function: _recv_command
	Description: Internal function used to receive command from the adapter
	Parameters:
		command = A pointer that will se the command received
		body_length = A pointer that will save the body length
		body = An array that will be used to save the content
		body_size = The maxum size that the body can fit
		
	Return:
		-2 = Checksum verification failed
		-1 = The body size is lesser than the body length
		0 = No preamble and packet start command received
		1 = Success
*/
int _recv_command(unsigned char* command, unsigned char* body_length, unsigned char* body, unsigned char body_size)
{
	unsigned char i = 0;
	unsigned char c = 0;
	
	if (RecvPrinterByte() != 0x99) // Preamble
	{
#ifdef DBG_ALL
		printf("Preamble fail, next: %u\n", (unsigned int)RecvPrinterByte());
#endif
		return 0;
	}
	
	if (RecvPrinterByte() != 0x66)
	{
#ifdef DBG_ALL
		printf("Preamble x2 fail, next %u\n", (unsigned int)RecvPrinterByte());
#endif
		return 0;
	}

	*command = RecvPrinterByte(); // Command ID
		
	RecvPrinterByte(); // Packet 01
	RecvPrinterByte(); // Packet 02
	
	*body_length = RecvPrinterByte(); // Length
	
	if (i > body_size)
		return -1;
	
	for (; i < *body_length; i++)
		*(body+i) = RecvPrinterByte();
	
	RecvPrinterByte(); // Checksum 1
	RecvPrinterByte(); // Checksum 2
	RecvPrinterByte(); // Device ID
	RecvPrinterByte(); // Status byte
	
	return 1;
}

int ustrcmp(const unsigned char* cmp1, unsigned int len1, unsigned char* cmp2, unsigned int len2)
{
	unsigned int c = 0;

	while (1)
	{
		if (c > (len1-1)) return 1;
		if (c > (len2-1)) return 1;

		if ((*(cmp1+c)) != (*(cmp2+c)))
		{
			return 0;
		}
		
		c += 1;
	}
	
	return 1;
}

unsigned int ustrlen(const unsigned char* str)
{
	unsigned int i = 0;
	
	while ((*(str+i))!='\0')
	{
		i++;
	}
	
	return i;
}

int mbgb_start_comunication()
{
	unsigned char body[8];
	unsigned char command = 0;
	unsigned char length = 0;
	
	//SendPrinterByte(0x4B);
	
	_send_command(0x10, 8, "NINTENDO");

	if (_recv_command(&command, &length, body, 8) < 0)
	{
#ifdef DBG_ALL
		printf("DEBUG: Error recv\n");
#endif
		return -1;
	}
	
	if (command != 0x10)
	{
#ifdef DBG_ALL
		printf("DEBUG: Commad is not 0x10, is %u\n", (unsigned int)command);
#endif
		return 0;
	}
	
	if (ustrcmp(body, (unsigned int)length, (unsigned char*)"NINTENDO", 8) == 0) 
	{
#ifdef DBG_ALL
		printf("DEBUG: %s != NINTENDO\n", (char*)body);
#endif
		return 0;
	}
	
	return 1;
}

void mbgb_close_comunication()
{
	unsigned char command = 0, length = 0;
	_send_command(0x11, 0, NULL);
	//_recv_command(&command, &length, NULL, 0);
}

int mbgb_is_line_busy()
{
	unsigned char command = 0, length = 0;
	unsigned char body[1];
	_send_command(0x17, 0, NULL);
	
	if (_recv_command(&command, &length, body, 1) < 1)
		return -1;
	
	if (body[0] == 0x00)
		return 0;
	
	if (body[0] == 0x04 || body[0] == 0x05)
		return 1;
	
	return -1;
}

void mbgb_dial_telephone(unsigned char* phone)
{
	unsigned char command = 0, length = 0;

	_send_command_extra(0x12, 1, 0x00, ustrlen(phone), phone);
	_recv_command(&command, &length, NULL, 0);
}

int mbgb_login(unsigned char* username, unsigned char* password)
{
	
}
