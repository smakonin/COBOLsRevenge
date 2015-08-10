/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#ifndef AUDIO_MIXER_H
#define AUDIO_MIXER_H

#include "globals.h"

class AUDIO_MIXER 
{
	private:
		LPDIRECTSOUND lpds;
		HWND window_handle;
		long sound_dbls;
		long music_dbls;

	public:
		AUDIO_MIXER(HWND window_handle);
		~AUDIO_MIXER();  
		void volume(long sound_dbls, long music_dbls);
		long get_sound_volume(void);
		long get_music_volume(void);
		LPDIRECTSOUND get_dsobj(void);
		HWND get_handle(void);
};

#endif //AUDIO_MIXER_H
