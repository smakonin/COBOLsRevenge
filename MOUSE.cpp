/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include "globals.h"

MOUSE::MOUSE(INPUT_MASTER *input_master, SCREEN *screen, HWND window_handle)
{
	MOUSE::input_master = input_master;
	MOUSE::screen = screen;
	lpdimouse = NULL;
	MOUSE::window_handle = window_handle;
	cursor = NULL;
	memset(name, 0, sizeof(name));

	right_button_down = false;
	left_button_down = false;

	x = screen->get_width() / 2;
	y = screen->get_height() /2;

	ShowCursor(FALSE);

	if(input_master->get_diobj()->CreateDevice(GUID_SysMouse, &lpdimouse, NULL) != DI_OK)
		throw ERROR("Unable to create DInput Mouse");

	if(lpdimouse->SetCooperativeLevel(window_handle, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND) != DI_OK)
		throw ERROR("Unable to Set Mouse Cooperative Level");

	if(lpdimouse->SetDataFormat(&c_dfDIMouse) != DI_OK)
		throw ERROR("Unable to Set Mouse Data Format");

	if(lpdimouse->Acquire() != DI_OK)
		throw ERROR("Unable to Acquire Mouse");
}

MOUSE::~MOUSE()
{
	if(lpdimouse)
	{
		lpdimouse->Unacquire();
		lpdimouse->Release();
	}

	delete cursor;

	ShowCursor(TRUE);
}

void MOUSE::set_cursor(char *filename, DWORD trans_colour, long centre_x, long centre_y)
{
	delete cursor;

	MOUSE::centre_x = centre_x;
	MOUSE::centre_y = centre_y;
	MOUSE::trans_colour = trans_colour;

	cursor = new IMAGE(screen->get_ddobj7());
	cursor->load_bmp(filename);
	cursor->set_key_colour(trans_colour);
}

void MOUSE::set_cursor(char *filename)
{
	FILE *fp = NULL;
	DWORD *tmp_buf = NULL;
	long width;
	long height;
	long size;

	delete cursor;

	fp = fopen(filename, "rb");
	if(!fp)
		throw ERRORS(ERR_MISSING_FILE, filename);
	
	fread(&width, sizeof(width), 1, fp);
	fread(&height, sizeof(height), 1, fp);
	fread(&trans_colour, sizeof(trans_colour), 1, fp);
	fread(&centre_x, sizeof(centre_x), 1, fp);
	fread(&centre_y, sizeof(centre_y), 1, fp);

	size = width * height * 4;
	tmp_buf = new DWORD[size];
	fread(tmp_buf, size, 1, fp);	
	cursor = new IMAGE(width, height, screen->get_ddobj7());
	cursor->put_pixels(tmp_buf);

	cursor->set_key_colour(trans_colour);

	fclose(fp);
	fp = NULL;
	delete tmp_buf;
}

void MOUSE::save(char *filename)
{
	FILE *fp = NULL;
	long size;
	long width;
	long height;

	if(!strcmp(name, filename))
		return;

	strcpy(name, filename);

	fp = fopen(filename, "wb");
	if(!fp)
		throw ERRORS(ERR_MISSING_FILE, filename);

	width = cursor->get_width();
	height = cursor->get_height();
	
	fwrite(&width, sizeof(width), 1, fp);
	fwrite(&height, sizeof(height), 1, fp);
	fwrite(&trans_colour, sizeof(trans_colour), 1, fp);
	fwrite(&centre_x, sizeof(centre_x), 1, fp);
	fwrite(&centre_y, sizeof(centre_y), 1, fp);

	size = width * height * 4;
	fwrite(cursor->get_pixels(), size, 1, fp);

	fclose(fp);
	fp = NULL;
}

void MOUSE::update(void)
{
	long tx, ty;

	if(!FAILED(lpdimouse->GetDeviceState(sizeof(mouse_state), (LPVOID)&mouse_state)))
	{
		x += mouse_state.lX;
		y += mouse_state.lY;

		if(x < 0) x = 0;
		if(y < 0) y = 0;
		if(x >= screen->get_width()) x = screen->get_width() - 1;
		if(y >= screen->get_height()) y = screen->get_height() - 1;

		tx = x - centre_x;
		ty = y - centre_y;

		if(visible)
			screen->put_image(tx, ty, cursor, true);
	}
}

long MOUSE::get_x(void)
{
	return x;
}

long MOUSE::get_y(void)
{
	return y;
}

bool MOUSE::right_button(bool menu_select)
{
	bool cur_state;

	cur_state = (mouse_state.rgbButtons[MOUSE_RIGHT_BUTTON] & 0x80) ? true : false;

	if(menu_select)
	{
		if(right_button_down && !cur_state)
		{
			right_button_down = false;
			return true;
		}
		else if(!right_button_down && cur_state)
		{
			right_button_down = true;
			return false;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return cur_state;
	}
}

bool MOUSE::left_button(bool menu_select)
{
	bool cur_state;

	cur_state = (mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80) ? true : false;

	if(menu_select)
	{
		if(left_button_down && !cur_state)
		{
			left_button_down = false;
			return true;
		}
		else if(!left_button_down && cur_state)
		{
			left_button_down = true;
			return false;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return cur_state;
	}
}

void MOUSE::show(void)
{
	visible = true;
}

void MOUSE::hide(void)
{
	visible = false;
}
