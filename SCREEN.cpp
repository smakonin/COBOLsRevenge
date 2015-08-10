/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include "globals.h"

SCREEN::SCREEN(long width, long height, long bpp, HWND window_handle, bool double_buffered, bool full_screen)
{
	LPRGNDATA region_data = NULL;
	RECT clip_rect;

	pixel_size_bits = 0;
	pixel_size_bytes = 0;
	SCREEN::double_buffered = double_buffered;
	SCREEN::full_screen = full_screen;

	IMAGE::width = width;
	IMAGE::height = height;
	SCREEN::bpp = bpp;
	colours = 2 << (bpp - 1);
	SCREEN::window_handle = window_handle;
	dims.top = 0;
	dims.left = 0;
	dims.right = width;
	dims.bottom = height;

	lpdd = NULL;
	lpdd7 = NULL;
	lpddsprimary = NULL;

	if(FAILED(DirectDrawCreate(NULL, &lpdd, NULL)))
	   throw ERRORS(ERR_NO_SET_GRAPHICS);

	if(FAILED(lpdd->QueryInterface(IID_IDirectDraw7, (LPVOID *)&lpdd7)))
	   throw ERRORS("Unable to Query DDraw Interface");

	if(full_screen)
	{
		if(FAILED(lpdd7->SetCooperativeLevel(window_handle, DDSCL_FULLSCREEN | DDSCL_ALLOWMODEX | DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT)))
		   throw ERRORS("Unable to Set Cooperative Level");

		if(FAILED(lpdd7->SetDisplayMode(width, height, bpp, 0, 0)))
		   throw ERRORS(ERR_NO_SET_GRAPHICS);
	}
	else
	{
		if(FAILED(lpdd7->SetCooperativeLevel(window_handle, DDSCL_NORMAL)))
		   throw ERRORS("Unable to Set Cooperative Level");
	}

	IMAGE::lpdd7 = lpdd7;

	if(double_buffered)
	{
		DDRAW_INIT_STRUCT(ddsd);
		ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		ddsd.dwBackBufferCount = 1;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_COMPLEX | DDSCAPS_FLIP;

		if(FAILED(lpdd7->CreateSurface(&ddsd, &lpddsprimary, NULL)))
			throw ERRORS(ERR_ALLOC_MEM, "Primary Surface");

		ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;

		if(FAILED(lpddsprimary->GetAttachedSurface(&ddsd.ddsCaps, &lpddsurface)))
			throw ERRORS(ERR_ALLOC_MEM, "Back Surface");
	}
	else
	{
		DDRAW_INIT_STRUCT(ddsd);
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

		if(FAILED(lpdd7->CreateSurface(&ddsd, &lpddsprimary, NULL)))
			throw ERRORS(ERR_ALLOC_MEM, "Primary Surface");
		
		create_surface(width, height);
	}


	clip_rect.left = 0;
	clip_rect.top = 0;
	clip_rect.right = width;
	clip_rect.bottom = height;

	if(FAILED(lpdd7->CreateClipper(0, &lpddclipper, NULL)))
		   throw ERRORS("Unable to Create Clipper");

	region_data = (LPRGNDATA)malloc(sizeof(RGNDATAHEADER) + sizeof(RECT));
	memcpy(region_data->Buffer, &clip_rect, sizeof(RECT));

	region_data->rdh.dwSize = sizeof(RGNDATAHEADER);
	region_data->rdh.iType = RDH_RECTANGLES;
	region_data->rdh.nCount = 1;
	region_data->rdh.nRgnSize = sizeof(RECT);

	region_data->rdh.rcBound.left =  clip_rect.left;
	region_data->rdh.rcBound.top =  clip_rect.top;
	region_data->rdh.rcBound.right = clip_rect.right;
	region_data->rdh.rcBound.bottom = clip_rect.bottom;

	if(FAILED(lpddclipper->SetClipList(region_data, 0)))
		   throw ERRORS("Unable to Set Clip List");

	if(FAILED(lpddsurface->SetClipper(lpddclipper)))
		   throw ERRORS("Unable to Set Clipper");

	free(region_data);

	pixel_size_bits = bpp;
	pixel_size_bytes = bpp / 8;

	actual_bpp = get_bpp_used();
}

SCREEN::~SCREEN()
{
	IMAGE::~IMAGE();

	if(lpddsprimary)
		lpddsprimary->Release();

	if(lpdd7)
		lpdd7->Release();
}

void SCREEN::flip_page(void)
{
	if(double_buffered)
	{
		lpddsprimary->Flip(NULL, DDFLIP_WAIT);
	}
	else
	{
		lpdd7->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, 0);
		lpddsprimary->BltFast(0, 0, lpddsurface, &dims, DDBLTFAST_WAIT);
	}
}

void SCREEN::clear_all(DWORD colour)
{
	clear(colour);
	flip_page();
	clear(colour);
}

LPDIRECTDRAW7 SCREEN::get_ddobj7(void)
{
	return lpdd7;
}

LPDIRECTDRAW SCREEN::get_ddobj(void)
{
	return lpdd;
}

LPDIRECTDRAWSURFACE7 SCREEN::get_ddprimary(void)
{
	return lpddsprimary;
}

void __cdecl create_frame(void *var)
{
	long index;
	DWORD p1, p2;
	double in_factor;
	double out_factor;
	BYTE r, g, b;
	DWORD *temp_buf;
	THREAD_COMM *comm;

	comm = (THREAD_COMM *)var;

	temp_buf = new DWORD[comm->buf_size];
	in_factor = comm->percent / 100.00;
	out_factor = (100.00 - comm->percent) / 100.00;

	for(index = 0; index < comm->buf_size; index++)
	{
		p1 = comm->fade_in[index];
		p2 = comm->fade_out[index];
		r = (BYTE)(_RGB32RED(p1) * in_factor) + (BYTE)(_RGB32RED(p2) * out_factor);
		g = (BYTE)(_RGB32GREEN(p1) * in_factor) + (BYTE)(_RGB32GREEN(p2) * out_factor);
		b = (BYTE)(_RGB32BLUE(p1) * in_factor) + (BYTE)(_RGB32BLUE(p2) * out_factor);
		temp_buf[index] = _RGB32BIT(0, r, g, b);
	}

	comm->frame->put_pixels(temp_buf);
	delete temp_buf;
	comm->is_done = true;
	_endthread();
}

void SCREEN::prep_fade(long skip)
{
	DWORD *fade_in;
	DWORD *fade_out;
	long cur_index;
	long no_of_fames;
	THREAD_COMM **comms;

	no_of_fames = 100 / skip;

	fade_in = copy_to_buffer(lpddsurface);
	fade_out = copy_to_buffer(lpddsprimary);

	fade_frames = new IMAGE *[no_of_fames];
	comms = new THREAD_COMM *[no_of_fames];
	for(cur_index = 0; cur_index < no_of_fames; cur_index++)
	{
		fade_frames[cur_index] = new IMAGE(width, height, lpdd7);
		comms[cur_index] = new THREAD_COMM;

		comms[cur_index]->id = cur_index;
		comms[cur_index]->fade_in = fade_in;
		comms[cur_index]->fade_out = fade_out;
		comms[cur_index]->frame = fade_frames[cur_index];
		comms[cur_index]->percent = (cur_index + 1) * skip;
		comms[cur_index]->buf_size = width * height;
		comms[cur_index]->is_done = false;

		_beginthread(create_frame, 0, (void *)comms[cur_index]);
	}	

and_again:
	for(cur_index = 0; cur_index < no_of_fames; cur_index++)
	{
		if(!comms[cur_index]->is_done)
			goto and_again;
	}

	delete comms;
	delete fade_out;
	delete fade_in;
}

bool SCREEN::fade(long skip, long fps, KEYBOARD * keyboard)
{
	long no_of_fames;
	long rate;
	long cur_index;
	bool no_esc = true;

	no_of_fames = 100 / skip;
	rate = 1000 / fps;

	for(cur_index = 0; cur_index < no_of_fames; cur_index++)
	{
		put_image(0, 0, fade_frames[cur_index]);
		flip_page();

		if(keyboard)
		{
			keyboard->update();

			if(keyboard->was_pressed(DIK_ESCAPE))
			{
				no_esc = false;
				break;
			}
		}

		Sleep(rate);
	}

	for(cur_index = 0; cur_index < no_of_fames; cur_index++)
		delete fade_frames[cur_index];
	delete fade_frames;
	return no_esc;
}
