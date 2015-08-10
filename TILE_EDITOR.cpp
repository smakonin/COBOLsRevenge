/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include "tile_editor.h"

#define SCREEN_BPP                  32   

#define TE_COPYRIGHT_LINES 4
char *te_copyright_text[TE_COPYRIGHT_LINES] = 
{
	"Tile Editor Version 2.1",
	"Copyright � 1998-2000 Makonin Consulting Corp.",
	"",
	"Loading, please wait..."
};

TILE_EDITOR::TILE_EDITOR(HWND main_window_handle, HINSTANCE hinstance_app)
{
	long count;

	ENGINE::main_window_handle = main_window_handle;
	ENGINE::hinstance_app = hinstance_app;

	big_one.top = 11; 
	big_one.left = 11;
	big_one.bottom = 300; 
	big_one.right = 300;

	cur_tile = r = g = b = 0;
	memset(fr, 0, sizeof(fr));
	memset(fg, 0, sizeof(fg));
	memset(fb, 0, sizeof(fb));
	
	screen = NULL;
	font = NULL;
	input_master = NULL;
	keyboard = NULL;
	mouse = NULL;
	tile = NULL;
	big_tile = NULL;
	imlib = NULL;
	main_menu = NULL;

	try
	{
		screen = new SCREEN(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, main_window_handle, DOUBLE_BUF, FULL_SCREEN);	
		input_master = new INPUT_MASTER(hinstance_app);
		keyboard = new KEYBOARD(input_master, main_window_handle);
		mouse = new MOUSE(input_master, screen, main_window_handle);
		mouse->set_cursor("static/defltcur.cr");
		font = new FONT("Arial", 16, screen);
		font->set_colour(PURPLE_FONT, true);
		imlib = new IMAGE_LIBRARY("static/tlimg.cr");
		imlib->set_key_colour(0x00FFFFFF);
		tile = new IMAGE(imlib->get_width(), imlib->get_height(), screen->get_ddobj7());
		big_tile = new IMAGE(tile->get_width() * 9 + 1, tile->get_height() * 9 + 1, screen->get_ddobj7());

		screen->clear(0);
		for(count = 0; count < TE_COPYRIGHT_LINES; count++)
			font->print(te_copyright_text[count], 10, 10 + (16 * count));		
		screen->flip_page();
		Sleep(888);

		main_menu = new MENU(screen, NULL, "Arial", 16);
		main_menu->setup_window(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0, 0, 0, 0);
		main_menu->setup_input(mouse, keyboard, NULL, NULL);
			
		main_menu->add_menu_item("Previous Tile",  BRIGHT_FONT, PURPLE_FONT,   0, 24 * 16, 95, font->get_height());
		main_menu->add_menu_item("Next Tile",      BRIGHT_FONT, PURPLE_FONT,   0, 25 * 16, 95, font->get_height());
		main_menu->add_menu_item("Goto Begining",  BRIGHT_FONT, PURPLE_FONT,   0, 26 * 16, 95, font->get_height());
		main_menu->add_menu_item("Goto End",       BRIGHT_FONT, PURPLE_FONT,   0, 27 * 16, 95, font->get_height());
		main_menu->add_menu_item("Fix Trans",      BRIGHT_FONT, PURPLE_FONT,   0, 28 * 16, 95, font->get_height());
		
		main_menu->add_menu_item("New Tile",       BRIGHT_FONT, PURPLE_FONT, 100, 24 * 16, 95, font->get_height());
		main_menu->add_menu_item("Copy Tile",      BRIGHT_FONT, PURPLE_FONT, 100, 25 * 16, 95, font->get_height());
		main_menu->add_menu_item("Clear Tile",     BRIGHT_FONT, PURPLE_FONT, 100, 26 * 16, 95, font->get_height());
		main_menu->add_menu_item("Delete Tile",    BRIGHT_FONT, PURPLE_FONT, 100, 27 * 16, 95, font->get_height());
		main_menu->add_menu_item("Trans Colour",   BRIGHT_FONT, PURPLE_FONT, 100, 28 * 16, 95, font->get_height());

		main_menu->add_menu_item("Rotate Tile",    BRIGHT_FONT, PURPLE_FONT, 200, 24 * 16, 95, font->get_height());
		main_menu->add_menu_item("Flip Horz",      BRIGHT_FONT, PURPLE_FONT, 200, 25 * 16, 95, font->get_height());
		main_menu->add_menu_item("Flip Vert",      BRIGHT_FONT, PURPLE_FONT, 200, 26 * 16, 95, font->get_height());
		
		main_menu->add_menu_item("Import BMP",     BRIGHT_FONT, PURPLE_FONT, 450, 25 * 16, 95, font->get_height());
		main_menu->add_menu_item("Save File",      BRIGHT_FONT, PURPLE_FONT, 450, 26 * 16, 95, font->get_height());
		main_menu->add_menu_item("Exit T.E.2",     BRIGHT_FONT, PURPLE_FONT, 450, 27 * 16, 95, font->get_height());

		main_menu->add_menu_item("Shift Down",     BRIGHT_FONT, PURPLE_FONT, 200, 27 * 16, 95, font->get_height());
		main_menu->add_menu_item("Shift Right",    BRIGHT_FONT, PURPLE_FONT, 200, 28 * 16, 95, font->get_height());

		main_menu->add_menu_item("Diagnal V to H", BRIGHT_FONT, PURPLE_FONT, 300, 24 * 16, 95, font->get_height());
		main_menu->add_menu_item("Diagnal H to V", BRIGHT_FONT, PURPLE_FONT, 300, 25 * 16, 95, font->get_height());

		update_big_tile(0);

		mouse->show();
	}
	catch(ERRORS e)
	{	
		throw e;
	}
}

TILE_EDITOR::~TILE_EDITOR()
{
	delete main_menu;
	delete big_tile;
	delete tile;
	delete imlib;
}


void TILE_EDITOR::update_big_tile(long tile_no)
{
	long x, y;
	DWORD colour;

	tile->put_pixels(imlib->get_starting_ptr(tile_no));
	
	for(y = 0; y < tile->get_height(); y ++)
	{
		for(x = 0; x < tile->get_width(); x ++)
		{
			colour = tile->get_pixel(x, y);
			big_tile->draw_box(x * 9, y * 9, 9, 9, colour, true, colour);
		}
	}
}

void TILE_EDITOR::update_screen(long tile_no, long r, long g, long b, long *fr, long *fg, long *fb)
{
	long count;
	char temp[64];

	screen->draw_box(10, 10, 290, 290, _RGB_WHITE);
	font->print(te_copyright_text[0], 320, 0);
	font->print(te_copyright_text[1], 320, 16);

	screen->draw_box(601, (10 * 16) - 1, 25, 25, _RGB_WHITE, true, _RGB32BIT(0, r, g, b));

	font->print("Red: ", 320, 4 * 16);
	screen->draw_box(370, (4 * 16) - 1, 257, 18, _RGB_WHITE);
	for(count = 0; count < 256; count++)
		screen->draw_box(371 + count, 4 * 16, 0, 16, _RGB32BIT(0, count, 0, 0));

	font->print("Green: ", 320, 6 * 16);
	screen->draw_box(370, (6 * 16) - 1, 257, 18, _RGB_WHITE);
	for(count = 0; count < 256; count++)
		screen->draw_box(371 + count, 6 * 16, 0, 16, _RGB32BIT(0, 0, count, 0));

	font->print("Blue: ", 320, 8 * 16);
	screen->draw_box(370, (8 * 16) - 1, 257, 18, _RGB_WHITE);
	for(count = 0; count < 256; count++)
		screen->draw_box(371 + count, 8 * 16, 0, 16, _RGB32BIT(0, 0, 0, count));
	
	for(count = 0; count < 12; count++)
	{
		sprintf(temp, "F%u", count + 1);
		font->print(temp, 320 + ((count % 6) * 48), (11 + (3 * (count / 6))) * 16);
		screen->draw_box(320 + ((count % 6) * 48), (12 + (3 * (count / 6))) * 16, 25, 25, _RGB_WHITE, true, _RGB32BIT(0, fr[count], fg[count], fb[count]));
	}

	sprintf(temp, "Current Tile: %u", tile_no);
	font->print(temp, 0, 20 * 16);

	sprintf(temp, "Current Colour: 0x%08X", _RGB32BIT(0, r, g, b));
	font->print(temp, 0, 21 * 16);
	sprintf(temp, "Colour Comp: Red [0x%02X] Green [0x%02X] Blue[0x%02X]", r, g, b);
	font->print(temp, 0, 22 * 16);

	screen->draw_box(379, 289, 33, 33, _RGB_WHITE);
	screen->draw_box(449, 289, 97, 97, _RGB_WHITE);

	tile->put_pixels(imlib->get_starting_ptr(tile_no));

	screen->put_image(380, 290, tile);
	screen->put_image(11, 11, big_tile);

	screen->put_image(450, 290, tile);
	screen->put_image(450, 290 + 32, tile);
	screen->put_image(450, 290 + 64, tile);

	screen->put_image(450 + 32, 290, tile);
	screen->put_image(450 + 32, 290 + 32, tile);
	screen->put_image(450 + 32, 290 + 64, tile);

	screen->put_image(450 + 64, 290, tile);
	screen->put_image(450 + 64, 290 + 32, tile);
	screen->put_image(450 + 64, 290 + 64, tile);
}

bool TILE_EDITOR::update(char *error, void *parms, int num_parms)
{
	RECT rec;
	long count;
	char filename[88];
	DWORD pixel;
	long selected_item;

	mouse->update();
	keyboard->update();

	record_tick_count();
	
	if(mouse->get_x() > big_one.left && mouse->get_x() < big_one.right && mouse->get_y() > big_one.top && mouse->get_y() < big_one.bottom)
	{
		if(mouse->left_button())
		{
			imlib->plot_pixel(cur_tile, (mouse->get_x() - big_one.left - 2) / 9, (mouse->get_y() - big_one.top - 2) / 9, _RGB32BIT(0, r, g, b));
			update_big_tile(cur_tile);
		}
		else if(mouse->right_button())
		{
			pixel = tile->get_pixel((mouse->get_x() - big_one.left - 2) / 9, (mouse->get_y() - big_one.top - 2) / 9);

			r = _RGB32RED(pixel);
			g = _RGB32GREEN(pixel);
			b = _RGB32BLUE(pixel);
		}
	}

	rec.top = 4 * 16;
	rec.bottom = rec.top + 16;
	rec.left = 372;
	rec.right = rec.left + 256;
	if(mouse->left_button() && mouse->get_x() >= rec.left && mouse->get_x() <= rec.right && mouse->get_y() >= rec.top && mouse->get_y() <= rec.bottom)
		r = (mouse->get_x() - rec.left);

	rec.top = 6 * 16;
	rec.bottom = rec.top + 16;
	rec.left = 372;
	rec.right = rec.left + 256;
	if(mouse->left_button() && mouse->get_x() >= rec.left && mouse->get_x() <= rec.right && mouse->get_y() >= rec.top && mouse->get_y() <= rec.bottom)
		g = (mouse->get_x() - rec.left);

	rec.top = 8 * 16;
	rec.bottom = rec.top + 16;
	rec.left = 372;
	rec.right = rec.left + 256;
	if(mouse->left_button() && mouse->get_x() >= rec.left && mouse->get_x() <= rec.right && mouse->get_y() >= rec.top && mouse->get_y() <= rec.bottom)
		b = (mouse->get_x() - rec.left);

	for(count = 0; count < 12; count++)
	{
		rec.top = (12 + (3 * (count / 6))) * 16;
		rec.bottom = rec.top + 25;
		rec.left = 321 + ((count % 6) * 48);
		rec.right = rec.left + 25;

		if(mouse->get_x() >= rec.left && mouse->get_x() <= rec.right && mouse->get_y() >= rec.top && mouse->get_y() <= rec.bottom)
		{
			if(mouse->right_button())
			{
				fr[count] = r;
				fg[count] = g;
				fb[count] = b;
			}
			else if(mouse->left_button())
			{
				r = fr[count];
				g = fg[count];
				b = fb[count];
			}
		}
	}

	if(keyboard->was_pressed(DIK_ESCAPE))
	{
		SendMessage(main_window_handle, WM_CLOSE, 0, 0);
		return true;
	}


	if(keyboard->was_pressed(DIK_RSHIFT, false) || keyboard->was_pressed(DIK_LSHIFT, false))
	{
		if(keyboard->was_pressed(DIK_LEFT, false))
		{
			cur_tile--;
			if(cur_tile == -1)
				cur_tile = imlib->get_count() - 1;
			update_big_tile(cur_tile);
		}
		
		if(keyboard->was_pressed(DIK_RIGHT, false))
		{
			cur_tile++;
			cur_tile %= imlib->get_count();
			update_big_tile(cur_tile);
		}
	}
	else
	{
		if(keyboard->was_pressed(DIK_LEFT))
		{
			cur_tile--;
			if(cur_tile == -1)
				cur_tile = imlib->get_count() - 1;
			update_big_tile(cur_tile);
		}
		
		if(keyboard->was_pressed(DIK_RIGHT))
		{
			cur_tile++;
			cur_tile %= imlib->get_count();
			update_big_tile(cur_tile);
		}
	}
	
	if(keyboard->was_pressed(DIK_F1))
	{
		if(keyboard->was_pressed(DIK_RSHIFT, false) || keyboard->was_pressed(DIK_LSHIFT, false))
		{
			fr[0] = r;
			fg[0] = g;
			fb[0] = b;
		}
		else
		{
			r = fr[0];
			g = fg[0];
			b = fb[0];
		}
	}

	if(keyboard->was_pressed(DIK_F2))
	{
		if(keyboard->was_pressed(DIK_RSHIFT, false) || keyboard->was_pressed(DIK_LSHIFT, false))
		{
			fr[1] = r;
			fg[1] = g;
			fb[1] = b;
		}
		else
		{
			r = fr[1];
			g = fg[1];
			b = fb[1];
		}
	}

	if(keyboard->was_pressed(DIK_F3))
	{
		if(keyboard->was_pressed(DIK_RSHIFT, false) || keyboard->was_pressed(DIK_LSHIFT, false))
		{
			fr[2] = r;
			fg[2] = g;
			fb[2] = b;
		}
		else
		{
			r = fr[2];
			g = fg[2];
			b = fb[2];
		}
	}

	if(keyboard->was_pressed(DIK_F4))
	{
		if(keyboard->was_pressed(DIK_RSHIFT, false) || keyboard->was_pressed(DIK_LSHIFT, false))
		{
			fr[3] = r;
			fg[3] = g;
			fb[3] = b;
		}
		else
		{
			r = fr[3];
			g = fg[3];
			b = fb[3];
		}
	}

	if(keyboard->was_pressed(DIK_F5))
	{
		if(keyboard->was_pressed(DIK_RSHIFT, false) || keyboard->was_pressed(DIK_LSHIFT, false))
		{
			fr[4] = r;
			fg[4] = g;
			fb[4] = b;
		}
		else
		{
			r = fr[4];
			g = fg[4];
			b = fb[4];
		}
	}

	if(keyboard->was_pressed(DIK_F6))
	{
		if(keyboard->was_pressed(DIK_RSHIFT, false) || keyboard->was_pressed(DIK_LSHIFT, false))
		{
			fr[5] = r;
			fg[5] = g;
			fb[5] = b;
		}
		else
		{
			r = fr[5];
			g = fg[5];
			b = fb[5];
		}
	}

	if(keyboard->was_pressed(DIK_F7))
	{
		if(keyboard->was_pressed(DIK_RSHIFT, false) || keyboard->was_pressed(DIK_LSHIFT, false))
		{
			fr[6] = r;
			fg[6] = g;
			fb[6] = b;
		}
		else
		{
			r = fr[6];
			g = fg[6];
			b = fb[6];
		}
	}

	if(keyboard->was_pressed(DIK_F8))
	{
		if(keyboard->was_pressed(DIK_RSHIFT, false) || keyboard->was_pressed(DIK_LSHIFT, false))
		{
			fr[7] = r;
			fg[7] = g;
			fb[7] = b;
		}
		else
		{
			r = fr[7];
			g = fg[7];
			b = fb[7];
		}
	}

	if(keyboard->was_pressed(DIK_F9))
	{
		if(keyboard->was_pressed(DIK_RSHIFT, false) || keyboard->was_pressed(DIK_LSHIFT, false))
		{
			fr[8] = r;
			fg[8] = g;
			fb[8] = b;
		}
		else
		{
			r = fr[8];
			g = fg[8];
			b = fb[8];
		}
	}

	if(keyboard->was_pressed(DIK_F10))
	{
		if(keyboard->was_pressed(DIK_RSHIFT, false) || keyboard->was_pressed(DIK_LSHIFT, false))
		{
			fr[9] = r;
			fg[9] = g;
			fb[9] = b;
		}
		else
		{
			r = fr[9];
			g = fg[9];
			b = fb[9];

		}
	}
	if(keyboard->was_pressed(DIK_F11))
	{
		if(keyboard->was_pressed(DIK_RSHIFT, false) || keyboard->was_pressed(DIK_LSHIFT, false))
		{
			fr[10] = r;
			fg[10] = g;
			fb[10] = b;
		}
		else
		{
			r = fr[10];
			g = fg[10];
			b = fb[10];
		}
	}

	if(keyboard->was_pressed(DIK_F12))
	{
		if(keyboard->was_pressed(DIK_RSHIFT, false) || keyboard->was_pressed(DIK_LSHIFT, false))
		{
			fr[11] = r;
			fg[11] = g;
			fb[11] = b;
		}
		else
		{
			r = fr[11];
			g = fg[11];
			b = fb[11];
		}
	}

	if(keyboard->was_pressed(DIK_B))
	{
		cur_tile = 0;
		update_big_tile(cur_tile);
	}

	if(keyboard->was_pressed(DIK_E))
	{
		cur_tile = imlib->get_count() - 1;
		update_big_tile(cur_tile);
	}

	if(keyboard->was_pressed(DIK_S))
	{
		screen->clear(_RGB32BIT(0, 0xFF, 0x34, 0x12));
		screen->flip_page();
		imlib->save("static/tlimg.cr");
	}

	if(keyboard->was_pressed(DIK_A))
	{
		imlib->add(cur_tile);
		update_big_tile(cur_tile);
	}

	screen->clear(0);
	selected_item = main_menu->show(NULL, false, false);

	switch(selected_item)
	{
		case 0: //prev tile
			cur_tile--;
			if(cur_tile == -1)
				cur_tile = imlib->get_count() - 1;
			update_big_tile(cur_tile);
			break;
		case 1: //next tile
			cur_tile++;
			cur_tile %= imlib->get_count();
			update_big_tile(cur_tile);
			break;
		case 2: //goto begining
			cur_tile = 0;
			update_big_tile(cur_tile);
			break;
		case 3: //goto end
			cur_tile = imlib->get_count() - 1;
			update_big_tile(cur_tile);
			break;
		case 4: //fix trans
			imlib->replace_colour(cur_tile, 0x00F8FCF8, 0x00FFFFFF);
			update_big_tile(cur_tile);
			break;
		case 5: //new
			imlib->add();
			cur_tile = imlib->get_count() - 1;
			update_big_tile(cur_tile);
			break;
		case 6: //copy
			imlib->copy(cur_tile);
			//cur_tile = imlib->get_count() - 1;
			//update_big_tile(cur_tile);
			break;
		case 7: //clear
			imlib->clear(cur_tile, _RGB32BIT(0, r, g, b));
			update_big_tile(cur_tile);
			break;
		case 8: //delete
			imlib->remove(cur_tile);
			cur_tile %= imlib->get_count();
			update_big_tile(cur_tile);
			break;
		case 9: //trans colour
			r = 0xFF;
			g = 0xFF;
			b = 0xFF;
			break;
		case 10: //rotate
			imlib->rotate(cur_tile);
			update_big_tile(cur_tile);
			break;
		case 11: //flip horz
			imlib->flip_horz(cur_tile);
			update_big_tile(cur_tile);
			break;
		case 12: //flip vert
			imlib->flip_vert(cur_tile);
			update_big_tile(cur_tile);
			break;
		case 13: //import
			memset(filename, 0, sizeof(filename));
			count = 0;
			font->set_colour(RED_FONT, true);
			while(true && !keyboard->was_pressed(DIK_ESCAPE))
			{
				screen->clear(0);
				font->print("What is the BMP Filename to Import:", 10, 16);
				if(filename)
					font->print(filename, 10, 32);
				screen->flip_page();

				while(!filename[count] && !keyboard->was_pressed(DIK_ESCAPE))
				{
					keyboard->update();
					filename[count] = keyboard->getch();
				}

				if(filename[count] == '\b' && count > 0)
				{
					filename[count] = '\0';
					count--;
					filename[count] = '\0';
				}
				else if(filename[count] == '\r')
				{
					filename[count] = '\0';
					break;
				}
				else
				{
					if(count > 64)
						filename[count] = '\0';
					else
						count++;
				}			
			}
			font->set_colour(PURPLE_FONT, true);
			if(!keyboard->was_pressed(DIK_ESCAPE))
			{
				IMAGE *img;
				img = new IMAGE(screen->get_ddobj7());
				img->load_bmp(filename);
				imlib->add(img);
				delete img;
				cur_tile = imlib->get_count() - 1;
			}
			update_big_tile(cur_tile);
			break;
		case 14: //save
			screen->clear(_RGB32BIT(0, 0xFF, 0x34, 0x12));
			screen->flip_page();
			imlib->save("static/tlimg.cr");
			break;
		case 15: //exit
			SendMessage(main_window_handle, WM_CLOSE, 0, 0);
			return true;
			break;

		case 16: //shift down
			imlib->shift_down(cur_tile);
			update_big_tile(cur_tile);
			break;

		case 17: //shift right
			imlib->shift_right(cur_tile);
			update_big_tile(cur_tile);
			break;

		case 18: //diagnal v to h
			imlib->diagnal_copy_vtoh(cur_tile);
			update_big_tile(cur_tile);
			break;

		case 19: //diagnal h to v
			imlib->diagnal_copy_htov(cur_tile);
			update_big_tile(cur_tile);
			break;
	}

	update_screen(cur_tile, r, g, b, fr, fg, fb);
	mouse->update();
	screen->flip_page();

	wait_for_tick_count(FPS_60);
	incroment_counters();

	return false;
}


