/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#ifndef SCREEN_H
#define SCREEN_H

#include "globals.h"

#define MAX_COLORS_PALETTE          256

class THREAD_COMM
{
	public:
		long id;
		DWORD *fade_in;
		DWORD *fade_out;
		long percent;
		IMAGE *frame;
		long buf_size;
		bool is_done;
};

class SCREEN : public IMAGE
{
	private:
		long height;
		long width;
		long colours;
		long bpp;
		long actual_bpp;
		bool double_buffered;
		bool full_screen;
		IMAGE **fade_frames;
		HWND window_handle;
		LPDIRECTDRAW lpdd;
		LPDIRECTDRAW7 lpdd7;
		LPDIRECTDRAWSURFACE7 lpddsprimary;

	public:
		SCREEN(long height, long width, long bpp, HWND window_handle, bool double_buffered = true, bool full_screen = true);
		~SCREEN();
		void flip_page(void);
		void clear_all(DWORD colour);
		LPDIRECTDRAW7 get_ddobj7(void);
		LPDIRECTDRAW get_ddobj(void);
		LPDIRECTDRAWSURFACE7 get_ddprimary(void);
		void prep_fade(long skip);
		bool fade(long skip, long fps, KEYBOARD * keyboard);
};

#endif //SCREEN_H

