/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include "globals.h"

MENU::MENU(SCREEN *screen, char *banner_filename, char *font_name, long font_size)
{
	banner = NULL;
	mouse = NULL;
	font = NULL;
	keyboard = NULL;
	win = NULL;
	items = new MENU_ITEM *[MAX_ITEMS];
	no_of_items = 0;
	MENU::screen = screen;

	font = new FONT(font_name, font_size, screen);
	font->set_colour(PURPLE_FONT, true);

	if(banner_filename)
	{
		banner = new IMAGE(screen->get_ddobj7());
		banner->load(banner_filename);
	}
}

MENU::~MENU()
{
	long cur_index;

	delete banner;

	for(cur_index = 0; cur_index < no_of_items; cur_index++)
		delete items[cur_index];

	delete items;
	delete win;
	delete font;
}

void MENU::setup_window(long win_width, long win_height, BYTE win_r, BYTE win_g, BYTE win_b, BYTE win_r_mod, BYTE win_g_mod, BYTE win_b_mod)
{
	win = new WINDOW(win_width, win_height, screen, win_r, win_g, win_b, win_r_mod, win_g_mod, win_b_mod);
}

void MENU::setup_input(MOUSE *mouse, KEYBOARD *keyboard, char *select_cursor, char *hourglass)
{
	MENU::mouse = mouse;
	MENU::keyboard = keyboard;
	
	if(select_cursor)
		strcpy(MENU::select_cursor, select_cursor);
	else
		MENU::select_cursor[0] = '\0';

	if(hourglass)
		strcpy(MENU::hourglass, hourglass);
	else
		MENU::hourglass[0] = '\0';
}

void MENU::add_menu_item(char *text, long select_colour, long normal_colour, long x, long y, long width, long height)
{

	MENU::font = font;
	font_scheme = font->get_colour();

	if(no_of_items == MAX_ITEMS - 1)
		throw ERROR("To Many Menu Items");

	if(x == -1 || y == -1 || width == -1 || height == -1)
	{
		x = win->get_x() + (TOP_BUFFER * 3);
		y = win->get_y() + TOP_BUFFER + banner->get_height() + (no_of_items * font->get_height());
		width = win->get_width() - (TOP_BUFFER * 4);
		height = font->get_height();
	}
	items[no_of_items] = new MENU_ITEM(mouse, font, text, x, y, width, height, select_colour, normal_colour);
	no_of_items++;
}

long MENU::show(VIEW *view, bool show_win, bool modal, PLAYER *player)
{
	long cur_index;
	long selected_item;

	mouse->show();
	if(show_win)
		win->show(-1, -1, view, player);

	keyboard->update(); 
	selected_item = -1;
	
	if(modal)
	{
		while(!keyboard->was_pressed(DIK_ESCAPE) && selected_item == -1) 
		{
			keyboard->update(); 
			mouse->update(); 
			view->generate_view(player);
			win->refresh();
			screen->put_image((screen->get_width() >> 1) - (banner->get_width() >> 1), win->get_y() + TOP_BUFFER, banner, true);

			for(cur_index = 0; cur_index < no_of_items; cur_index++)
			{
				if(items[cur_index]->show())
					selected_item = cur_index;
			}

			if(strlen(select_cursor) && strlen(hourglass))
			{
				if(win->is_in(mouse->get_x(), mouse->get_y()))
					mouse->set_cursor(select_cursor);
				else
					mouse->set_cursor(hourglass);
			}

			mouse->update(); 
			screen->flip_page();
		}
	}
	else
	{
		mouse->update(); 

		for(cur_index = 0; cur_index < no_of_items; cur_index++)
		{
			if(items[cur_index]->show())
				selected_item = cur_index;
		}
	}

	return selected_item;
}

void MENU::hide(VIEW *view, PLAYER *player)
{
	win->hide(view, player);
}