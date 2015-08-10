/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#ifndef FONT_H
#define FONT_H

#include "globals.h"

#define BLUE_FONT                   0
#define RED_FONT                    1
#define PURPLE_FONT                 2
#define BLUE_FONT2                  3
#define PURPLE_FONT2                4
#define BRIGHT_FONT                 5
#define GOLDEN_FONT                 6

class FONT 
{
	private:
		HFONT font_data;
		COLORREF colour;
		COLORREF shadow_colour;		
		SCREEN *screen;
		bool show_shadow;
		long height;
		long scheme;

	public:
		FONT(char *fontname, long size, SCREEN *screen);
		~FONT();
		void set_colour(long scheme, bool show_shadow);
		void print(char *text, long x, long y);
		void print(char *text, long x, long y, long wrap_length);
		long get_height(void);
		long get_colour(void);
};

#endif //FONT_H
