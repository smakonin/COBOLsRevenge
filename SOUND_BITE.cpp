/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include "globals.h"

SOUND_BITE::SOUND_BITE()
{
	audio_mixer = NULL;
	memset(&format_settings, 0, sizeof(WAVEFORMATEX));
	lpsdsbuf = NULL;
}

SOUND_BITE::SOUND_BITE(AUDIO_MIXER *audio_mixer)
{
	SOUND_BITE::audio_mixer = audio_mixer;

	memset(&format_settings, 0, sizeof(WAVEFORMATEX));
	lpsdsbuf = NULL;
}

SOUND_BITE::~SOUND_BITE()
{
	if(lpsdsbuf)
		lpsdsbuf->Release();
}

void SOUND_BITE::load(char *filename, FILE *ext_fp)
{
	FILE *fp = ext_fp;
	UCHAR *snd_buffer, *audio_ptr_1 = NULL, *audio_ptr_2 = NULL;
	DWORD audio_length_1 = 0, audio_length_2 = 0;

	SOUND_BITE();

	if(!ext_fp)
	{
		fp = fopen(filename, "rb");
		if(!fp)
			throw ERRORS(ERR_MISSING_FILE, filename);
	}

	fread(&format_settings, sizeof(WAVEFORMATEX), 1, fp);
	fread(&size, sizeof(size), 1, fp);
	snd_buffer = new UCHAR[size];
	fread(snd_buffer, size, 1, fp);

	memset(&dsbd, 0, sizeof(DSBUFFERDESC));
	dsbd.dwSize	= sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE;
	dsbd.dwBufferBytes = size;
	dsbd.lpwfxFormat = &format_settings;

	if(FAILED(audio_mixer->get_dsobj()->CreateSoundBuffer(&dsbd, &lpsdsbuf, NULL)))
	{
		delete snd_buffer;
		throw ERRORS("Unable to create DSound Buffer");
	} 

	if(FAILED(lpsdsbuf->Lock(0, size, (void **) &audio_ptr_1, &audio_length_1, (void **)&audio_ptr_2, &audio_length_2, DSBLOCK_FROMWRITECURSOR)))
		throw ERRORS("Unable to lock DSound Buffer");

	memcpy(audio_ptr_1, snd_buffer, audio_length_1);
	memcpy(audio_ptr_2, (snd_buffer+audio_length_1),audio_length_2);

	if(FAILED(lpsdsbuf->Unlock(audio_ptr_1, audio_length_1, audio_ptr_2, audio_length_2)))
		throw ERRORS("Unable to unlock DSound Buffer");
	
	delete snd_buffer;	

	if(!ext_fp)
	{
		fclose(fp);
		fp = NULL;
	}
}

void SOUND_BITE::save(char *filename)
{
	FILE *fp = NULL;
	UCHAR *snd_buffer, *audio_ptr_1 = NULL, *audio_ptr_2 = NULL;
	DWORD audio_length_1 = 0, audio_length_2 = 0;

	if(FAILED(lpsdsbuf->Lock(0, size, (void **) &audio_ptr_1, &audio_length_1, (void **)&audio_ptr_2, &audio_length_2, DSBLOCK_FROMWRITECURSOR)))
		throw ERRORS("Unable to lock DSound Buffer");

	snd_buffer = new UCHAR[size];
	memcpy(snd_buffer, audio_ptr_1, audio_length_1);
	memcpy((snd_buffer + audio_length_1), audio_ptr_2, audio_length_2);

	if(FAILED(lpsdsbuf->Unlock(audio_ptr_1, audio_length_1, audio_ptr_2, audio_length_2)))
		throw ERRORS("Unable to unlock DSound Buffer");
	
	fp = fopen(filename, "wb");
	if(!fp)
		throw ERRORS(ERR_MISSING_FILE, filename);

	fwrite(&format_settings, sizeof(WAVEFORMATEX), 1, fp);
	fwrite(&size, sizeof(size), 1, fp);
	fwrite(snd_buffer, size, 1, fp);

	delete snd_buffer;
	fclose(fp);
	fp = NULL;
}

void SOUND_BITE::load_wav(char *filename)
{
	HMMIO  hwav;
	MMCKINFO parent, child;
	UCHAR *snd_buffer, *audio_ptr_1 = NULL, *audio_ptr_2 = NULL;
	DWORD audio_length_1 = 0, audio_length_2 = 0;
			
	parent.ckid = (FOURCC)0;
	parent.cksize = 0;
	parent.fccType = (FOURCC)0;
	parent.dwDataOffset = 0;
	parent.dwFlags = 0;

	child = parent;

	if((hwav = mmioOpen(filename, NULL, MMIO_READ | MMIO_ALLOCBUF)) == NULL)
   		throw ERRORS(ERR_MISSING_FILE, filename);

	parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	if(mmioDescend(hwav, &parent, NULL, MMIO_FINDRIFF))
    {
		mmioClose(hwav, 0);
   		throw ERRORS(ERR_NOT_WAV_FILE, filename);
    }

	child.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if(mmioDescend(hwav, &child, &parent, 0))
    {
		mmioClose(hwav, 0);
   		throw ERRORS(ERR_NOT_WAV_FILE, filename);
    }

	if(mmioRead(hwav, (char *)&format_settings, sizeof(format_settings)) != sizeof(format_settings))
    {
	    mmioClose(hwav, 0);
   		throw ERRORS(ERR_NOT_WAV_FILE, filename);
    }

	if(format_settings.wFormatTag != WAVE_FORMAT_PCM)
    {
		mmioClose(hwav, 0);
   		throw ERRORS(ERR_NOT_WAV_FILE, filename);
    }

	if(mmioAscend(hwav, &child, 0))
	{
		mmioClose(hwav, 0);
		throw ERRORS(ERR_NOT_WAV_FILE, filename);
	}	

	child.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if(mmioDescend(hwav, &child, &parent, MMIO_FINDCHUNK))
    {
		mmioClose(hwav, 0);
   		throw ERRORS(ERR_NOT_WAV_FILE, filename);
    }	

	snd_buffer = (UCHAR *)malloc(child.cksize);
	mmioRead(hwav, (char *)snd_buffer, child.cksize);
	mmioClose(hwav, 0);

	memset(&dsbd, 0, sizeof(DSBUFFERDESC));
	dsbd.dwSize	= sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE;
	dsbd.dwBufferBytes = child.cksize;
	dsbd.lpwfxFormat = &format_settings;

	size = child.cksize;

	if(FAILED(audio_mixer->get_dsobj()->CreateSoundBuffer(&dsbd, &lpsdsbuf, NULL)))
	{
		free(snd_buffer);
		throw ERRORS("Unable to create DSound Buffer");
	} 

	if(FAILED(lpsdsbuf->Lock(0, child.cksize, (void **) &audio_ptr_1, &audio_length_1, (void **)&audio_ptr_2, &audio_length_2, DSBLOCK_FROMWRITECURSOR)))
		throw ERRORS("Unable to lock DSound Buffer");

	memcpy(audio_ptr_1, snd_buffer, audio_length_1);
	memcpy(audio_ptr_2, (snd_buffer+audio_length_1),audio_length_2);

	if(FAILED(lpsdsbuf->Unlock(audio_ptr_1, audio_length_1, audio_ptr_2, audio_length_2)))
		throw ERRORS("Unable to unlock DSound Buffer");

	free(snd_buffer);
}

void SOUND_BITE::play(void)
{
	lpsdsbuf->Stop();
	lpsdsbuf->SetVolume(audio_mixer->get_sound_volume());
	lpsdsbuf->Play(0, 0, 0);
}
