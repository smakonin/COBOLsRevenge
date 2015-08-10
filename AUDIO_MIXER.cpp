/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include "globals.h"

AUDIO_MIXER::AUDIO_MIXER(HWND window_handle)
{
	AUDIO_MIXER::window_handle = window_handle;
	lpds = NULL;

	if(DirectSoundCreate(NULL, &lpds, NULL) != DD_OK)
		throw ERROR("Unalbe to init Direct Sound");

	if(lpds->SetCooperativeLevel(window_handle, DSSCL_NORMAL) != DS_OK)
		throw ERROR("Unalbe to init DSound Cooperative Level");
}

AUDIO_MIXER::~AUDIO_MIXER()
{
	if(lpds)
		lpds->Release();
}

void AUDIO_MIXER::volume(long sound_dbls, long music_dbls)
{
	AUDIO_MIXER::sound_dbls = sound_dbls;
	AUDIO_MIXER::music_dbls = music_dbls;
}

long AUDIO_MIXER::get_sound_volume(void)
{
	return sound_dbls;
}

long AUDIO_MIXER::get_music_volume(void)
{
	return music_dbls;
}

LPDIRECTSOUND AUDIO_MIXER::get_dsobj(void)
{
	return lpds;
}

HWND AUDIO_MIXER::get_handle(void)
{
	return window_handle;
}
