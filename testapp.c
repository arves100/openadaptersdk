/*
	Mobile Adapter GB test app
*/
#include <gb/gb.h>
#include <stdio.h>

#include "mbacl.h"

unsigned char xdxd[] = {
	0x0A, //Length
	0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x31, //User
	0x05, //Length2
	0x30, 0x30, 0x30, 0x30, 0x61, // Password
	0xD2, 0xC4, 0x03, 0xB7, //Ipv4 DNS1
	0xD2, 0x8D, 0x70, 0xA3  //Ipv4 DNS2
};

extern void _send_command(unsigned char command, unsigned char body_length, unsigned char* body);
	
int main()
{
	int ret = 0;
	
	printf("Mobile Adapter GB Sample Application\n");
	printf("Press A to begin...\n");
	while(joypad() != J_A);
	
	ret = mbgb_start_comunication();
	
	if (ret == -1)
	{
		printf("Comunication error with the adapter\n");
		return 0;
	}
	if (ret == 0)
	{
		printf("Invalid adapter\n");
		return 0;
	}
	
	printf("Comunication started!\n");
	
	ret = mbgb_is_line_busy();
	
	if (ret == -1)
	{
		printf("Comunication error with the adapter\n");
		mbgb_close_comunication();
		return 0;
	}
	
	if (ret == 0)
	{
		printf("Dialing DION...\n");
		mbgb_dial_telephone("#9677");
	}
	else
	{
		mbgb_close_comunication();
		mbgb_start_comunication();
		printf("Dialing DION...\n");
		mbgb_dial_telephone("#9677");
	}
	
	ret = mbgb_is_line_busy();
	
	if (ret != 1)
	{
		printf("Cannot dial DION!\n");
		mbgb_close_comunication();
		return 0;
	}
	
	printf("Connected to DION!\n");

	_send_command(0x21, 24, xdxd);
	
	printf("Comunication closed!\n");
	mbgb_close_comunication();
	return 0;
}