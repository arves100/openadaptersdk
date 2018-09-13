/*
	Mobile Adapter GB Comunication Library
*/

#include <gb/gb.h>
#include <stdio.h>

extern unsigned char SendPrinterByte(unsigned char byte);

#define RecvPrinterByte() SendPrinterByte(0x4B)

void _send_command(unsigned char command, unsigned char body_length, unsigned char* body)
{
	unsigned char i = 0;
	unsigned int checksum = 0;
	
	SendPrinterByte(0x99); // Preamble
	SendPrinterByte(0x66); // Start Packet
	SendPrinterByte(command); // Command ID
	SendPrinterByte(0x00); // Packet 1
	SendPrinterByte(0x00); // Packet 2
	SendPrinterByte(body_length);

	checksum = command + body_length;
	for (; i < body_length; i++)
	{ 
		SendPrinterByte(*(body+i));
		checksum += (*(body+i)); 
	}
	
	SendPrinterByte(checksum>>8);
	SendPrinterByte(checksum&0xFF);
	SendPrinterByte(0x80); // Device ID
	SendPrinterByte(0x00); // Status byte
}

int _recv_command(unsigned char* command, unsigned char* body_length, unsigned char* body)
{
	unsigned char i = 0;
	unsigned char c = 0;
	
	if (RecvPrinterByte() != 0x99) // Preamble
		return 0;
	
	if (RecvPrinterByte() != 0x66)
		return 0;

	*command = RecvPrinterByte(); // Command ID
		
	RecvPrinterByte(); // Packet 01
	RecvPrinterByte(); // Packet 02
	
	*body_length = RecvPrinterByte(); // Length
	
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

int start_comunication()
{
	unsigned char body[8];
	unsigned char command = 0;
	unsigned char length = 0;
	
	SendPrinterByte(0x4B);
	
	_send_command(0x10, 8, "NINTENDO");

	if (_recv_command(&command, &length, body) == 0)
		return 0;
	
	if (command != 0x10) return 0;

	if (ustrcmp(body, (unsigned int)length, (unsigned char*)"NINTENDO", 8) == 0) return 0;
	
	return 1;
}
