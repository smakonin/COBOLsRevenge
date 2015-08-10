/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include "globals.h"

WINDOW::WINDOW()
{
}

WINDOW::WINDOW(long width, long height, SCREEN *screen, BYTE r, BYTE g, BYTE b, BYTE r_mod, BYTE g_mod, BYTE b_mod)
{
	WINDOW::width = width;
	WINDOW::height = height;
	WINDOW::screen = screen;
	x = -1;
	y = -1;
	WINDOW::r = r;
	WINDOW::g = g;
	WINDOW::b = b;
	WINDOW::r_mod = r_mod;
	WINDOW::g_mod = g_mod;
	WINDOW::b_mod = b_mod;
}

WINDOW::~WINDOW()
{
}

void WINDOW::show(long x, long y, VIEW *view, PLAYER *player)
{
	long tw, th, tx, ty;
	double i;

	if(x == -1 || y == -1)
	{
		x = (screen->get_width() >> 1) - (width >> 1);	
		y = (screen->get_height() >> 1) - (height >> 1);
	}

	tx = WINDOW::x = x;
	ty = WINDOW::y = y;
	tw = width;
	th = height;

	for(i = 0.0; i < 1.0; i += 0.1)
	{
		WINDOW::x = (long)(tx * i);
		WINDOW::y = (long)(ty * i);
		width = (long)(tw * i);
		height = (long)(th * i);
		view->generate_view(player);
		refresh();
		screen->flip_page();
		Sleep(18);
	}

	WINDOW::x = tx;
	WINDOW::y = ty;
	width = tw;
	height = th;
	view->generate_view(player);
	refresh();
	screen->flip_page();
}

void WINDOW::refresh(void)
{
	BYTE r = WINDOW::r;
	BYTE g = WINDOW::g;
	BYTE b = WINDOW::b;
	long t;

	if(x == -1 || y == -1)
		return;

	for(t = 0; t < 6; t++)
	{
		r += r_mod;
		g += g_mod;
		b += b_mod;
		screen->draw_box(x + t, y + t, width - (t * 2), height - (t * 2), _RGB32BIT(0, r, g, b));
	}

	for(; t < 13; t++)
	{
		r -= r_mod;
		g -= g_mod;
		b -= b_mod;
		screen->draw_box(x + t, y + t, width - (t * 2), height - (t * 2), _RGB32BIT(0, r, g, b));
	}

	screen->draw_box(x + t, y + t, width - (t * 2), height - (t * 2), _RGB32BIT(0, r, g, b), true, _RGB32BIT(0, r, g, b));
}

void WINDOW::hide(VIEW *view, PLAYER *player)
{
	long tw, th, tx, ty;
	double i;

	tx = WINDOW::x = x;
	ty = WINDOW::y = y;
	tw = width;
	th = height;

	for(i = 1.0; i > 0.0; i -= 0.1)
	{
		WINDOW::x = (long)(tx * i);
		WINDOW::y = (long)(ty * i);
		width = (long)(tw * i);
		height = (long)(th * i);
		view->generate_view(player);
		refresh();
		screen->flip_page();
		Sleep(18);
	}

	WINDOW::x = tx;
	WINDOW::y = ty;
	width = tw;
	height = th;
	x = -1;
	y = -1;
}

bool WINDOW::is_in(long x, long y)
{
	if(x >= WINDOW::x && y >= WINDOW::y && x <= WINDOW::x + width && y <= WINDOW::y + height)
		return true;
	else
		return false;
}

long WINDOW::get_x(void)
{
	if(x == -1)
		return (screen->get_width() >> 1) - (width >> 1);
	else
		return x;
}

long WINDOW::get_y(void)
{
	if(y == -1)
		return (screen->get_height() >> 1) - (height >> 1);
	else
		return y;
}

long WINDOW::get_width(void)
{
	return width;
}

long WINDOW::get_height(void)
{
	return height;
}

