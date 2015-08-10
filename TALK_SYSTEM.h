/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#ifndef TALK_SYSTEM_H
#define TALK_SYSTEM_H

#include "globals.h"

#define MAX_QUESTIONS 9

class TALK_SYSTEM 
{
	private:
		SCREEN *screen;
		KEYBOARD *keyboard;
		PLAYER *player;
		MOUSE *mouse;
		FONT *font;
		VIEW *view;
		STATUS_PANEL *status_panel;
		IMAGE *bubble;
		IMAGE *hook;
		TALK_DB *talk_db;
		SOUND_BITE **sounds;
		long question_bag[MAX_QUESTIONS];
		long bubble_x;
		long bubble_y;

	public:    
		TALK_SYSTEM(SCREEN *screen, KEYBOARD *keyboard, PLAYER *player, MOUSE *mouse, VIEW *view, STATUS_PANEL *status_panel, SOUND_BITE **sounds);
		~TALK_SYSTEM();  
		void talk_with(long npc_no, long x, long y, void *ge);
		void show_talk_bubble(long x, long y);
		void show_intro2(void);
		void do_reactions(long no);
};

#endif //TALK_SYSTEM_H