/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#ifndef MENU_ITEM_H
#define MENU_ITEM_H

#include "globals.h"

class MENU_ITEM 
{
	private:
		FONT *font;
		MOUSE *mouse;
		char text[1024];
		long x;
		long y;
		long width;
		long height;
		long select_colour;
		long normal_colour;

	public:
		MENU_ITEM(MOUSE *mouse, FONT *font, char *text, long x, long y, long width, long height, long select_colour, long normal_colour);
		~MENU_ITEM();
		bool show(void);
};

#endif //MENU_ITEM_H
