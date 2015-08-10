/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#ifndef MOVIE_H
#define MOVIE_H

#include "globals.h"

class MOVIE : public IMAGE_LIBRARY, private SOUND_BITE
{
	private:
		SCREEN *screen;
		IMAGE **frames;
		KEYBOARD *keyboard;
		RECT rec;

	public:
		MOVIE(char *filename, SCREEN *screen, AUDIO_MIXER *audio_mixer, KEYBOARD *keyboard);
		~MOVIE();
		bool play(long fps);
		void set_position(long x, long y, long width, long height);
};

#endif //MOVIE_H
