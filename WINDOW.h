/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#ifndef WINDOW_H
#define WINDOW_H

#include "globals.h"

class WINDOW 
{
	private:
		long x;
		long y;
		long height;
		long width;
		BYTE r;
		BYTE g;
		BYTE b;
		BYTE r_mod;
		BYTE g_mod;
		BYTE b_mod;
		SCREEN *screen;

	public:
		WINDOW();
		WINDOW(long width, long height, SCREEN *screen, BYTE r, BYTE g, BYTE b, BYTE r_mod, BYTE g_mod, BYTE b_mod);
		~WINDOW();
		void show(long x, long y, VIEW *view, PLAYER *player = NULL);
		void refresh(void);
		void hide(VIEW *view, PLAYER *player = NULL);
		bool is_in(long x, long y);
		long get_x(void);
		long get_y(void);
		long get_width(void);
		long get_height(void);
};

#endif //WINDOW_H
