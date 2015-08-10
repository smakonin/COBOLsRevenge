/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#ifndef MOUSE_H
#define MOUSE_H

#include "globals.h"

#define MOUSE_LEFT_BUTTON           0
#define MOUSE_RIGHT_BUTTON          1

class MOUSE
{
	private:
		INPUT_MASTER *input_master;
		SCREEN *screen;
		IMAGE *cursor;
		LPDIRECTINPUTDEVICE lpdimouse;
		DIMOUSESTATE mouse_state;
		bool visible;
		HWND window_handle;
		long x;
		long y;
		long centre_x;
		long centre_y;
		DWORD trans_colour;
		char name[256];
		bool right_button_down;
		bool left_button_down;

	public:
		MOUSE(INPUT_MASTER *input_master, SCREEN *screen, HWND window_handle);
		~MOUSE();
		void set_cursor(char *filename, DWORD trans_colour, long centre_x, long centre_y);
		void set_cursor(char *filename);
		void save(char *filename);
		void update(void);
		long get_x(void);
		long get_y(void);
		bool right_button(bool menu_select = false);
		bool left_button(bool menu_select = false);
		void show(void);
		void hide(void);
};

#endif //MOUSE_H
