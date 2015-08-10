/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#ifndef ENGINE_H
#define ENGINE_H

#include "globals.h"

#define ANIM_FRAMES   32
#define FULL_SCREEN   true
#define DOUBLE_BUF    false

class ENGINE 
{
	private:
		long cloak_ticker;
		long anim_index;
		long start_ticker;
		DWORD tick_count;
		long cloak_ticker2;

	protected:
		SCREEN *screen;
		FONT *font;
		FONT *small_font;
		INPUT_MASTER *input_master;
		KEYBOARD *keyboard;
		MOUSE *mouse;
		AUDIO_MIXER *audio_mixer;
		FILE_LIBRARY file_lib;
		HWND main_window_handle;
		HINSTANCE hinstance_app;

	public:
		ENGINE();
		~ENGINE();
		void incroment_counters(void);
		void record_tick_count(void);
		void record_tick_count2(void);
		bool check_tick_count(DWORD additional_count);
		void wait_for_tick_count(DWORD additional_count);
		long get_anim_index(void);
		long get_cloak_ticker(void);
		long get_cloak_ticker2(void);
};

#endif //ENGINE_H

