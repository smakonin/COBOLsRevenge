/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#ifndef MENU_H
#define MENU_H

#include "globals.h"

#define MAX_ITEMS  64
#define TOP_BUFFER 20

class MENU
{
	private:
		IMAGE *banner;
		long no_of_items;
		MENU_ITEM **items;
		MOUSE *mouse;
		KEYBOARD *keyboard;
		SCREEN *screen;
		FONT *font;
		WINDOW *win;
		char select_cursor[128];
		char hourglass[128];
		long font_scheme;

	public:
		MENU(SCREEN *screen, char *banner_filename, char *font_name, long font_size);
		~MENU();
		void setup_window(long win_width, long win_height, BYTE win_r, BYTE win_g, BYTE win_b, BYTE win_r_mod, BYTE win_g_mod, BYTE win_b_mod);
		void setup_input(MOUSE *mouse, KEYBOARD *keyboard, char *select_cursor, char *hourglass);
		void add_menu_item(char *text, long select_colour, long normal_colour, long x = -1, long y = -1, long width = -1, long height = -1);
		long show(VIEW *view, bool show_win = true, bool modal = true, PLAYER *player = NULL);
		void hide(VIEW *view, PLAYER *player = NULL);
};

#endif //MENU_H
