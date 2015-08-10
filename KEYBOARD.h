/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "globals.h"

class KEYBOARD
{
	private:
		INPUT_MASTER *input_master;
		LPDIRECTINPUTDEVICE lpdikeyboard;
		UCHAR keyboard_state[256];
		HWND window_handle;
		UCHAR keys_down[256];

	public:
		KEYBOARD(INPUT_MASTER *input_master, HWND window_handle);
		~KEYBOARD();
		void update(void);
		bool was_pressed(long key, bool menu_select = true);
		char getch(void);
};

#endif //KEYBOARD_H
