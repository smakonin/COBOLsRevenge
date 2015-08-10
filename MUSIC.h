/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#ifndef MUSIC_H
#define MUSIC_H

#include "globals.h"

#define MULTI_TO_WIDE(x, y)         MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED, y, -1, x, _MAX_PATH);

class MUSIC 
{
	private:
		AUDIO_MIXER *audio_mixer;
		IDirectMusicPerformance *dm_perf;
		IDirectMusicLoader *dm_loader;
		IDirectMusicSegment *dm_segment;
		IDirectMusicSegmentState *dm_segstate;
		IDirectMusicSegment *load_midi_seg(IDirectMusicLoader* loader, WCHAR *filename);

	public:
		MUSIC(AUDIO_MIXER *audio_mixer);
		~MUSIC();
		void play(char *filename);
		void stop(void);
		void check_status(void);
};

#endif //MUSIC_H
