/**********************************************************************
 *
 *	This file is Copyright (C) 1999, Stephen Makonin.
 *	All Rights Reserved.
 *
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>


#define _RGB16BIT565(r, g, b) ((b % 32) + ((g % 64) << 5) + ((r % 32) << 11))



void main(void)
{
	unsigned short c;
	unsigned short r, g, b;

	r = 12;
	g = 55;
	b = 43;
	printf("\nred = 0x%04X, green = 0x%04X, blue = 0x%04X", r, g, b);
	printf("\nred = 0x%04X, green = 0x%04X, blue = 0x%04X", r % 32, g % 64, b % 32);
	printf("\nred = 0x%04X, green = 0x%04X, blue = 0x%04X", (r % 32) << 11, (g % 64) << 6, (b % 32));
	c = _RGB16BIT565(r, g, b);
	printf("\ncolour = 0x%04X", c);


	r = (c & 0xF800) >> 11;
	g = (c & 0x07E0) >> 5;
	b = (c & 0x001F);
	printf("\nred = 0x%04X, green = 0x%04X, blue = 0x%04X", r, g, b);
	c = _RGB16BIT565(r, g, b);
	printf("\ncolour = 0x%04X", c);

	printf("\n");
	getch();
}