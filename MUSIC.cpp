/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include "globals.h"

MUSIC::MUSIC(AUDIO_MIXER *audio_mixer)
{
	MUSIC::audio_mixer = audio_mixer;
	dm_perf = NULL;
	dm_loader = NULL;
	dm_segment = NULL;
	dm_segstate = NULL;

	CoInitialize(NULL);

	if(FAILED(CoCreateInstance(CLSID_DirectMusicPerformance, NULL, CLSCTX_INPROC, IID_IDirectMusicPerformance, (void**)&dm_perf)))
		throw ERROR("Unable to create DMusic Performance");

	if(FAILED(dm_perf->Init(NULL, audio_mixer->get_dsobj(), audio_mixer->get_handle())))
		throw ERROR("Unable to Init DMusic");

	if(FAILED(dm_perf->AddPort(NULL)))
		throw ERROR("Unable to Add DMusic Port");

    if(FAILED(CoCreateInstance(CLSID_DirectMusicLoader, NULL, CLSCTX_INPROC, IID_IDirectMusicLoader, (void**)&dm_loader)))
		throw ERROR("Unable to create DMusic Loader");
}

MUSIC::~MUSIC()
{
	stop();

	dm_perf->CloseDown();
	dm_perf->Release();     

	dm_loader->Release();     

	CoUninitialize(); 
}

IDirectMusicSegment *MUSIC::load_midi_seg(IDirectMusicLoader* loader, WCHAR *filename)
{
	DMUS_OBJECTDESC ObjDesc; 
	HRESULT hr;
	IDirectMusicSegment* pSegment = NULL;
 
	char szDir[_MAX_PATH];
	WCHAR wszDir[_MAX_PATH];
	
	if(_getcwd(szDir, _MAX_PATH) == NULL)
	{
		return NULL;
	}
	
	MULTI_TO_WIDE(wszDir, szDir);
	
	hr = loader->SetSearchDirectory(GUID_DirectMusicAllTypes, wszDir, FALSE);
	
	if (FAILED(hr)) 
		return NULL;
	
	ZeroMemory(&ObjDesc, sizeof(DMUS_OBJECTDESC));
	ObjDesc.dwSize = sizeof(DMUS_OBJECTDESC);
	ObjDesc.guidClass = CLSID_DirectMusicSegment;
	wcscpy(ObjDesc.wszFileName, filename );
	
	ObjDesc.dwValidData = DMUS_OBJ_CLASS | DMUS_OBJ_FILENAME;
	
	hr = loader->GetObject(&ObjDesc,IID_IDirectMusicSegment, (void**) &pSegment);
	
	if (FAILED(hr))
		return(0);
	
	hr = pSegment->SetParam(GUID_StandardMIDIFile,-1, 0, 0, (void*)dm_perf);
	
	if (FAILED(hr))
		return(0);
	
	hr = pSegment->SetParam(GUID_Download, -1, 0, 0, (void*)dm_perf);
	
	if (FAILED(hr))
		return(0);
	
	return pSegment;

} 

void MUSIC::play(char *filename)
{
	WCHAR wfilename[_MAX_PATH];

	stop(); 

	MULTI_TO_WIDE(wfilename, filename);
	dm_segment = load_midi_seg(dm_loader, wfilename);
	if(!dm_segment)
		throw ERRORS(ERR_MISSING_FILE, filename);

	dm_perf->PlaySegment(dm_segment, 0, 0, &dm_segstate);
}

void MUSIC::stop(void)
{
	dm_perf->Stop(NULL, NULL, 0, 0); 

	if(dm_segstate)
	{
		dm_segment->SetParam(GUID_Unload, -1, 0, 0, (void*)dm_perf); 
		dm_segment->Release(); 	
		dm_segstate = NULL;
	}
}

void MUSIC::check_status(void)
{
	if(dm_segstate)
		if(dm_perf->IsPlaying(dm_segment, NULL) != S_OK)
			dm_perf->PlaySegment(dm_segment, 0, 0, &dm_segstate);
}