/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#ifndef TILE_EDITOR_H
#define TILE_EDITOR_H

#include "globals.h"
#include "engine.h"

class TILE_EDITOR : protected ENGINE 
{
	private:
		IMAGE *tile;
		IMAGE *big_tile;
		IMAGE_LIBRARY *imlib;
		MENU *main_menu;
		RECT big_one;
		long cur_tile;
		long r, g, b;
		long fr[12];
		long fg[12];
		long fb[12];
		void update_big_tile(long tile_no);
		void update_screen(long tile_no, long r, long g, long b, long *fr, long *fg, long *fb);

	public:
		TILE_EDITOR(HWND main_window_handle, HINSTANCE hinstance_app);
		~TILE_EDITOR();
		bool update(char *error, void *parms = NULL, int num_parms = 0);
};

#endif //TILE_EDITOR_H

