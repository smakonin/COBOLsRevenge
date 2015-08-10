/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include "globals.h"
#include "engine.h"

class MAP_EDITOR : protected ENGINE 
{
	private:
		WORLD *world;
		TILES *tiles;
		VIEW *view;
		MENU *edit_menu;
		LOCATION loc;
		long mx, my;
		BYTE cur_chunk;
		bool all_chunk_view;

	public:
		MAP_EDITOR(HWND main_window_handle, HINSTANCE hinstance_app);
		~MAP_EDITOR();
		bool update(char *error, void *parms = NULL, int num_parms = 0);
};

#endif //MAP_EDITOR_H
