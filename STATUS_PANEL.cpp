/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include "globals.h"

STATUS_PANEL::STATUS_PANEL(SCREEN *screen, TILES *tiles, PLAYER *player, MOUSE *mouse)
{
	STATUS_PANEL::screen = screen;
	STATUS_PANEL::tiles = tiles;
	STATUS_PANEL::mouse = mouse;
	STATUS_PANEL::player = player;

	image = new IMAGE(screen->get_ddobj7());
	image->load("static/statpnl.cr");
	pos_x = (screen->get_width() - image->get_width()) >> 1;
	pos_y = screen->get_height() - image->get_height() - 10;

	font = new FONT("Arial", 12, screen);
	font->set_colour(GOLDEN_FONT, true);
}

STATUS_PANEL::~STATUS_PANEL()
{
	delete font;
	delete image;
}

void STATUS_PANEL::show(bool in_talk_mode)
{
	char tmp_str[128];
	long tmp_num, i;

	screen->put_image(pos_x, pos_y, image);


	sprintf(tmp_str, "%u", player->gold);
	font->print(tmp_str, pos_x + 23, pos_y + 23);

	sprintf(tmp_str, "%u", player->food);
	font->print(tmp_str, pos_x + 85, pos_y + 23);

	if(player->energy)
	{
		tmp_num = (long)(137.00 * (player->energy  / 100.00)) - 1;
		screen->draw_box(pos_x + 148, pos_y + 21, tmp_num, 13, _RGB32BIT(0, 0xFF, 0, 0), true, _RGB32BIT(0, 0xFF, 0, 0));
	}
	sprintf(tmp_str, "%3.1f%%", player->energy);
	font->print(tmp_str, pos_x + 205, pos_y + 23);

	tmp_num = 308;
	for(i = player->max_items - 1; i > -1; i--)
	{
		if(player->items[i].in_inventory && !player->items[i].is_used)
		{
			screen->put_image(pos_x + tmp_num, pos_y + 10 + player->items[i].inv_vert_mod, tiles->get_tile(player->items[i].chunk_no, 0, 2), true);
			tmp_num += (tiles->get_width() + 5);
		}
	}

	if(mouse->get_x() > pos_x + 66 && mouse->get_x() < pos_x + 66 + 66 && mouse->get_y() > pos_y + 3 && mouse->get_y() < pos_y + 41)
		screen->draw_box(pos_x + 66, pos_y + 3, 66, 39, _RGB_WHITE);

	if(in_talk_mode)
		player->msg_no = 0;

	if(player->msg_no)
	{
		switch(player->msg_no)
		{
			case 1:
				print_text("You must get closer!");
				break;
			case 2:
				print_text("Need to use magic!");
				break;
			case 3:
				print_text("Yummy... tastes good!");
				break;
			case 4:
				print_text("This door is locked!");
				break;
			case 5:
				print_text("Game is now saved!");
				break;
			case 6:
				print_text("Unable to save!");
				break;
			default:
				if(player->msg_no > 90000 && player->msg_no <= 99999)
				{
					sprintf(tmp_str, "You found %u gold!", player->msg_no - 90000);
					print_text(tmp_str);
				}
				break;
		}
	}
	else
	{
		if(player->is_dead())
			print_text("Thou hast parished!");
	}
}

bool STATUS_PANEL::is_mouse_in(void)
{
	if(mouse->get_x() >= pos_x && mouse->get_x() < pos_x + image->get_width())
		if(mouse->get_y() >= pos_y && mouse->get_y() < pos_y + image->get_height())
			return true;

	return false;
}

char *STATUS_PANEL::get_desc(void)
{
	if(mouse->get_x() > pos_x + 66 && mouse->get_x() < pos_x + 66 + 66 && mouse->get_y() > pos_y + 3 && mouse->get_y() < pos_y + 41)
		return "click here to eat";
	else
		return "status panel";
}

bool STATUS_PANEL::is_mouse_in_food(void)
{
	if(mouse->get_x() > pos_x + 66 && mouse->get_x() < pos_x + 66 + 66 && mouse->get_y() > pos_y + 3 && mouse->get_y() < pos_y + 41)
		return true;

	return false;
}

void STATUS_PANEL::print_text(char *text)
{
	font->print(text, 278, 252);
}
