/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#ifndef SOUND_BITE_H
#define SOUND_BITE_H

#include "globals.h"

class SOUND_BITE 
{
	protected:
		AUDIO_MIXER *audio_mixer;
		LPDIRECTSOUNDBUFFER lpsdsbuf; 
		DSBUFFERDESC dsbd;
		WAVEFORMATEX format_settings;  
		long size;
		FILE_LIBRARY file_lib;

	public:
		SOUND_BITE();
		SOUND_BITE(AUDIO_MIXER *audio_mixer);
		~SOUND_BITE();
		void load(char *filename, FILE *ext_fp = NULL);
		void save(char *filename);
		void load_wav(char *filename);
		void play(void);
};

#endif //SOUND_BITE_H
