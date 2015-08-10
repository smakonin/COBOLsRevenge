/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include "globals.h"

MENU_ITEM::MENU_ITEM(MOUSE *mouse, FONT *font, char *text, long x, long y, long width, long height, long select_colour, long normal_colour)
{
	MENU_ITEM::font = font;
	strcpy(MENU_ITEM::text, text);
	MENU_ITEM::x = x;
	MENU_ITEM::y = y;
	MENU_ITEM::mouse = mouse;
	MENU_ITEM::width = width;
	MENU_ITEM::height = height;
	MENU_ITEM::select_colour = select_colour;
	MENU_ITEM::normal_colour = normal_colour;
}

MENU_ITEM::~MENU_ITEM()
{
}

bool MENU_ITEM::show()
{
	long x2, y2;
	long mx, my;

	x2 = x + width;
	y2 = y + height;

	mx = mouse->get_x();
	my = mouse->get_y();

	if(mx > x && my > y && mx < x2 && my < y2)
		font->set_colour(select_colour, true);
	else
		font->set_colour(normal_colour, true);
	
	font->print(text, x, y);

	if(mx > x && my > y && mx < x2 && my < y2 && mouse->left_button(true))
		return true;
	else
		return false;
}
