/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include "map_editor.h"

#define SCREEN_BPP                  32   
#define VIEW_WIDTH                  21  
#define VIEW_HEIGHT                 15
#define VIEW_POS_X                  -16
#define VIEW_POS_Y                  0
#define PC_TILE                     0

#define MMENU_COLOUR_R              0x43
#define MMENU_COLOUR_G              0x00
#define MMENU_COLOUR_B              0x40
#define MMENU_COLOUR_R_MOD          18
#define MMENU_COLOUR_G_MOD          0
#define MMENU_COLOUR_B_MOD          0
#define MMENU_WIDTH                 404
#define MMENU_HEIGHT                300

#define ME_COPYRIGHT_LINES 4
char *me_copyright_text[ME_COPYRIGHT_LINES] = 
{
	"Map Editor Version 2.1",
	"Copyright � 1998-2000 Makonin Consulting Corp.",
	"",
	"Loading, please wait..."
};

MAP_EDITOR::MAP_EDITOR(HWND main_window_handle, HINSTANCE hinstance_app)
{
	long count;

	ENGINE::main_window_handle = main_window_handle;
	ENGINE::hinstance_app = hinstance_app;

	screen = NULL;
	font = NULL;
	input_master = NULL;
	keyboard = NULL;
	mouse = NULL;
	world = NULL;
	tiles = NULL;
	view = NULL;
	audio_mixer = NULL;
	edit_menu = NULL;

	loc.x = 20;
	loc.y = 20;
	loc.z = 4;
	cur_chunk = 0;
	all_chunk_view = false;

	try
	{
		screen = new SCREEN(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, main_window_handle, DOUBLE_BUF, FULL_SCREEN);																							 
		audio_mixer = new AUDIO_MIXER(main_window_handle);

		input_master = new INPUT_MASTER(hinstance_app);
		keyboard = new KEYBOARD(input_master, main_window_handle);
		mouse = new MOUSE(input_master, screen, main_window_handle);
		mouse->set_cursor("static/targtcur.cr");

		font = new FONT("Arial", 16, screen);
		font->set_colour(PURPLE_FONT, true);

		edit_menu = new MENU(screen, NULL, "Arial", 16);
		edit_menu->setup_window(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0, 0, 0, 0);
		edit_menu->setup_input(mouse, keyboard, NULL, NULL);
		edit_menu->add_menu_item("Level Up", BRIGHT_FONT, PURPLE_FONT, 10, 24 * font->get_height(), 95, font->get_height());
		edit_menu->add_menu_item("Level Down", BRIGHT_FONT, PURPLE_FONT, 10, 25 * font->get_height(), 95, font->get_height());
		edit_menu->add_menu_item("Save Map", BRIGHT_FONT, PURPLE_FONT, 10, 27 * font->get_height(), 95, font->get_height());
		edit_menu->add_menu_item("Choose Chunk", BRIGHT_FONT, PURPLE_FONT, 10, 22 * font->get_height(), 95, font->get_height());
		edit_menu->add_menu_item("Exit MkMap", BRIGHT_FONT, PURPLE_FONT, 10, 28 * font->get_height(), 95, font->get_height());
		
		screen->clear(0);
		for(count = 0; count < ME_COPYRIGHT_LINES; count++)
			font->print(me_copyright_text[count], 10, 10 + (16 * count));		
		screen->flip_page();
		Sleep(888);

		world = new WORLD("static/wrlddat.cr");
		tiles = new TILES("static/tlndx.cr", "static/tlimg.cr", screen);
		view = new VIEW(VIEW_WIDTH, VIEW_HEIGHT, VIEW_POS_X, VIEW_POS_Y, screen, world, tiles);
	}
	catch(ERRORS e)
	{
		throw e;
	}
}

MAP_EDITOR::~MAP_EDITOR()
{
	delete view;
	delete tiles;
	delete world;
	delete edit_menu;
}

bool MAP_EDITOR::update(char *error, void *parms, int num_parms)
{
	long item_select;
	static bool quitting = false;

	if(quitting)
		return false;

	try
	{	
		keyboard->update();
		mouse->update();

		record_tick_count();

		if(keyboard->was_pressed(DIK_LEFT, false))
			loc.x--;

		if(keyboard->was_pressed(DIK_RIGHT, false))
			loc.x++;

		if(keyboard->was_pressed(DIK_UP, false))
			loc.y--;

		if(keyboard->was_pressed(DIK_DOWN, false))
			loc.y++;				

		loc.x = FIX_WORLD_COORD(loc.x);
		loc.y = FIX_WORLD_COORD(loc.y);

		mouse->update();

		mx = FIX_WORLD_COORD(FIX_WORLD_COORD(loc.x - (VIEW_WIDTH >> 1)) + ((mouse->get_x() + 16) >> 5));
		my = FIX_WORLD_COORD(FIX_WORLD_COORD(loc.y - (VIEW_HEIGHT >> 1)) + (mouse->get_y() >> 5));

		if(all_chunk_view)
		{
			screen->clear(0);
			view->generate_all_chunk_view(get_anim_index());
			if(mouse->left_button(true))
			{
				cur_chunk = view->get_chunk_selected(mouse->get_x(), mouse->get_y());
				all_chunk_view = false;
			}
		}
		else
		{
			if(mouse->get_y() >= 350 && mouse->get_x() <= 150)
			{
			}
			else
			{
				if(mouse->left_button())
					world->put_chunk(mx, my, loc.z, cur_chunk);

				if(mouse->right_button())
					cur_chunk = world->get_chunk(mx, my, loc.z);
			}

			view->generate_view(loc.x, loc.y, loc.z, get_anim_index());
		}

		item_select = edit_menu->show(NULL, false, false);
		switch(item_select)
		{
			case 0:
				loc.z++;
				if(loc.z > 7) loc.z = 0;
				break;
			case 1:
				loc.z--;
				if(loc.z < 0) loc.z = 7;
				break;
			case 2:
				world->save("static/wrlddat.cr");
				break;
			case 3:
				all_chunk_view = true;
				break;
			case 4:
				quitting = true;
				SendMessage(main_window_handle, WM_CLOSE, 0, 0);
				return false;
				break;
		}

		char tstr[256];
		sprintf(tstr, "Mouse: (%3u, %3u)", mx, my);
		font->print(tstr, 500, 27 * font->get_height());

		sprintf(tstr, "Location: (%3u, %3u, %2u)", loc.x, loc.y, loc.z);
		font->print(tstr, 500, 28 * font->get_height());

		sprintf(tstr, "Chunk: %3u", cur_chunk);
		font->print(tstr, 550, 25 * font->get_height());

		screen->draw_box(500, 394, 35, 35, _RGB_BLACK);
		screen->draw_box(501, 395, 33, 33, _RGB_WHITE);
		screen->put_image(502, 396, tiles->get_tile(cur_chunk, get_anim_index(), 0), true);
		screen->put_image(502, 396, tiles->get_tile(cur_chunk, get_anim_index(), 1), true);
		screen->put_image(502, 396, tiles->get_tile(cur_chunk, get_anim_index(), 2), true);

		mouse->update();
		screen->flip_page();

		wait_for_tick_count(FPS_15);
		incroment_counters();
	}
	catch(ERRORS e)
	{	
		strcpy(error, e.get_message());
		return true;
	}

	return false;
}

