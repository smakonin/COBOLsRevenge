/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "globals.h"
#include "engine.h"

class GAME_ENGINE : protected ENGINE 
{
	private:
		WORLD *world;
		TILES *tiles;
		VIEW *view;
		MUSIC *music;
		MENU *main_menu;
		PLAYER *player;
		TALK_SYSTEM *talk_system;
		STATUS_PANEL *status_panel;
		SOUND_BITE *sounds[11];
		IMAGE *end_image;		
		IMAGE *begin_image;		
		long sequence_mode;
		long old_sequence_mode;
		void wait_for_esc(void);

	public:
		GAME_ENGINE(HWND main_window_handle, HINSTANCE hinstance_app);
		~GAME_ENGINE();
		bool update(char *error, void *parms = NULL, int num_parms = 0);
		void company_logo(void);
		void introduction(void);
		void conclusion(void);
		void do_reactions(long no);
		void const_pole(void);
};

#endif //GAME_ENGINE_H
