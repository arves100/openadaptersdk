/*
	Mobile Adapter GB test app
*/
#include <gb/gb.h>
#include <stdio.h>

#include "mbacl.h"

int main()
{
	if (start_comunication() == 0)
		printf("Cannot start comunication!");
		
	return 0;
}