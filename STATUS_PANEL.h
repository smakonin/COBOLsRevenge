/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#ifndef STATUS_PANEL_H
#define STATUS_PANEL_H

#include "globals.h"

class STATUS_PANEL 
{
	private:
		SCREEN *screen;
		TILES *tiles;
		PLAYER *player;
		MOUSE *mouse;
		IMAGE *image;
		FONT *font;
		long pos_x;
		long pos_y;

	public:    
		STATUS_PANEL(SCREEN *screen, TILES *tiles, PLAYER *player, MOUSE *mouse);
		~STATUS_PANEL();
		void show(bool in_talk_mode = false);
		bool is_mouse_in(void);
		char *get_desc(void);
		bool is_mouse_in_food(void);
		void print_text(char *text);
};

#endif //STATUS_PANEL_H