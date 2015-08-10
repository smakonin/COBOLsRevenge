/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include "globals.h"

MOVIE::MOVIE(char *filename, SCREEN *screen, AUDIO_MIXER *audio_mixer, KEYBOARD *keyboard)
{
	long cur_index;
	FILE *fp = NULL;

	MOVIE::screen = screen;
	MOVIE::keyboard = keyboard;
	SOUND_BITE::audio_mixer = audio_mixer;
	frames = NULL;

	fp = fopen(filename, "rb");
	if(!fp)
		throw ERRORS(ERR_MISSING_FILE, filename);
	
	IMAGE_LIBRARY::load(NULL, fp);
	SOUND_BITE::load(NULL, fp);

	fclose(fp);
	
	frames = new IMAGE *[count];
	for(cur_index = 0; cur_index < count; cur_index++)
	{
		frames[cur_index] = new IMAGE(width, height, screen->get_ddobj7());
		frames[cur_index]->put_pixels(images + (cur_index * (height * width)));
	}

	rec.top = 0;
	rec.bottom = height;
	rec.left = 0;
	rec.right = width;
}

MOVIE::~MOVIE()
{
	long cur_index;

	for(cur_index = 0; cur_index < count; cur_index++)
		delete frames[cur_index];

	delete frames;
}

inline bool MOVIE::play(long fps)
{
	long cur_index;
	long rate;

	rate = 1000 / fps;

	SOUND_BITE::play();

	for(cur_index = 0; cur_index < count; cur_index++)
	{
		keyboard->update();

		screen->put_image(&rec, frames[cur_index]);
		screen->flip_page();

		if(keyboard->was_pressed(DIK_ESCAPE))
			return false;

		Sleep(rate);
	}

	return true;
}

inline void MOVIE::set_position(long x, long y, long width, long height)
{
	rec.top = y;
	rec.bottom = y + height;
	rec.left = x;
	rec.right = x + width;
}
