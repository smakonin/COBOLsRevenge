/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include "globals.h"
#include "game_engine.h"

TALK_SYSTEM::TALK_SYSTEM(SCREEN *screen, KEYBOARD *keyboard, PLAYER *player, MOUSE *mouse, VIEW *view, STATUS_PANEL *status_panel, SOUND_BITE **sounds)
{
	TALK_SYSTEM::screen = screen;
	TALK_SYSTEM::keyboard = keyboard;
	TALK_SYSTEM::player = player;
	TALK_SYSTEM::mouse = mouse;
	TALK_SYSTEM::view = view;
	TALK_SYSTEM::status_panel = status_panel;
	TALK_SYSTEM::sounds = sounds;

	bubble = new IMAGE(screen->get_ddobj7());
	bubble->load("static/bblpic.cr");

	hook = new IMAGE(screen->get_ddobj7());
	hook->load("static/hookpic.cr");

	font = new FONT("Arial", 12, screen);
	font->set_colour(GOLDEN_FONT, true);

	bubble_x = (screen->get_width() - bubble->get_width()) >> 1;
}

TALK_SYSTEM::~TALK_SYSTEM()
{
	delete bubble;
	delete hook;
	delete font;
}

void TALK_SYSTEM::talk_with(long npc_no, long x, long y, void *ge)
{
	MENU *talk_menu;
	long item_select;
	long talk_seq_no;
	bool end_talk;
	char *response;
	long reaction;
	
	talk_db = new TALK_DB();			
	memset(&question_bag, 0, sizeof(long) *MAX_QUESTIONS);
	talk_seq_no = 0;
	end_talk = false;
	item_select = -1;

	view->fix_xy( &x, &y);

	mouse->set_cursor("static/defltcur.cr");
	
	reaction = 0;
	while(!end_talk)
	{
		response = strdup(talk_db->find_response(npc_no, &talk_seq_no, &end_talk, player));

		do_reactions(reaction);
		while(!mouse->left_button())
		{
			view->generate_view(player);
			show_talk_bubble(x, y);
			font->print(response, bubble_x + 20, bubble_y + 20, 80);
			status_panel->show(true);
			mouse->update();
			screen->flip_page();
			((GAME_ENGINE *)ge)->const_pole();
		}
		free(response);

		if(!end_talk)
		{
			talk_menu = new MENU(screen, NULL, "Arial", 12);
			talk_menu->setup_window(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0, 0, 0, 0);
			talk_menu->setup_input(mouse, keyboard, NULL, NULL);

			talk_db->find_questions(npc_no, talk_seq_no, question_bag, MAX_QUESTIONS, item_select, player);

			show_talk_bubble(-1, -1);

			for(item_select = 0; item_select < MAX_QUESTIONS; item_select++)
				if(question_bag[item_select])
					talk_menu->add_menu_item(talk_db->find_question(question_bag[item_select]), BRIGHT_FONT, GOLDEN_FONT, bubble_x + 80, bubble_y + 20 + (item_select * font->get_height()), bubble->get_width() - 45, font->get_height());
			
			Sleep(500);
			mouse->update();

			item_select = -1;
			while(item_select == -1)
			{
				view->generate_view(player);
				show_talk_bubble(-1, -1);
				font->print("You say:", bubble_x + 20, bubble_y + 20);
				item_select = talk_menu->show(NULL, false, false);
				status_panel->show(true);
				mouse->update();
				screen->flip_page();
				((GAME_ENGINE *)ge)->const_pole();
			}
			reaction = talk_db->get_reaction(question_bag[item_select]);
			delete talk_menu;
			talk_seq_no = question_bag[item_select];
		}
	}

	Sleep(500);
	mouse->update();

	delete talk_db;
}

void TALK_SYSTEM::show_talk_bubble(long x, long y)
{
	if(x == -1 && y == -1)
	{
		bubble_y = 239 - hook->get_height() - bubble->get_height();
		screen->put_image(bubble_x, bubble_y, bubble, true);
		screen->put_image(328, 235 - hook->get_height(), hook, true);
	}
	else
	{
		bubble_y = (y + 16) - hook->get_height() - bubble->get_height();
		screen->put_image(bubble_x, bubble_y, bubble, true);
		screen->put_image(x + 8, (y + 12) - hook->get_height(), hook, true);
	}
}

void TALK_SYSTEM::show_intro2(void)
{
	char *response;

	talk_db = new TALK_DB();			

	mouse->set_cursor("static/defltcur.cr");
	
	response = strdup(talk_db->find_question(20));
	while(!mouse->left_button())
	{
		view->generate_view(player);
		show_talk_bubble(-1, -1);
		font->print(response, bubble_x + 20, bubble_y + 20);
		mouse->update();
		screen->flip_page();
	}	
	free(response);
	
	Sleep(500);
	mouse->update();
	
	response = strdup(talk_db->find_question(21));
	while(!mouse->left_button())
	{
		view->generate_view(player);
		show_talk_bubble(-1, -1);
		font->print(response, bubble_x + 20, bubble_y + 20);
		mouse->update();
		screen->flip_page();
	}	
	free(response);

	Sleep(500);
	mouse->update();
	
	response = strdup(talk_db->find_question(22));
	while(!mouse->left_button())
	{
		view->generate_view(player);
		show_talk_bubble(-1, -1);
		font->print(response, bubble_x + 20, bubble_y + 20);
		mouse->update();
		screen->flip_page();
	}	
	free(response);
	
	delete talk_db;
}

void TALK_SYSTEM::do_reactions(long no)
{
	switch(no)
	{
		case 2:
			player->quests.done[3] = true;
			talk_db->remove_question(question_bag, MAX_QUESTIONS, 9);
			sounds[5]->play();
			break;
		case 3:
			talk_db->remove_question(question_bag, MAX_QUESTIONS, 8);
			break;
		case 4:
			player->quests.done[5] = true;
			player->populace[46].loc.x--;
			sounds[5]->play();
			break;
		case 5:
			player->items[0].is_visible = true;
			talk_db->remove_question(question_bag, MAX_QUESTIONS, 84);
			sounds[5]->play();
			break;
		case 6:
			talk_db->remove_question(question_bag, MAX_QUESTIONS, 83);
			break;
		case 7:
			player->quests.done[4] = true;
			player->items[0].is_used = true;
			sounds[5]->play();
			break;
		case 8:
			sounds[2]->play();
			player->gold--;
			player->food++;
			break;
		case 9:
			player->quests.done[0] = true;
			break;
		case 10:
			player->populace[14].loc.z = 4;
			break;
		case 11:
			player->items[1].is_used = true;
			player->quests.done[7] = true;
			sounds[5]->play();
			break;
		case 12:
			player->populace[15].loc.x = 64;
			player->populace[15].loc.y = 3;
			player->populace[15].loc.z = 4;
			break;
		case 13:
			player->quests.done[8] = true;
			sounds[5]->play();
			break;
		case 14:
			player->items[3].in_inventory = true;
			sounds[5]->play();
			break;
		case 15:
			player->items[2].in_inventory = true;
			player->gold -= 30;
			talk_db->remove_question(question_bag, MAX_QUESTIONS, 150);
			sounds[5]->play();
			break;
		case 16:
			player->quests.done[2] = true;
			player->populace[9].loc.x = 117;
			player->populace[9].loc.y = 24;
			player->populace[9].loc.z = 2;
			player->populace[9].job_loc1.x = 116;
			player->populace[9].job_loc1.y = 23;
			player->populace[9].job_loc1.z = 2;
			player->populace[9].job_loc2.x = 121;
			player->populace[9].job_loc2.y = 24;
			player->populace[9].job_loc2.z = 2;
			sounds[8]->play();
			break;
		case 17:
			talk_db->remove_question(question_bag, MAX_QUESTIONS, 161);
			player->quests.done[6] = true;
			player->populace[9].job = 'S';
			sounds[8]->play();
			break;
		case 18:
			player->quests.done[1] = true;
			player->populace[9].loc.z = 0;
			sounds[8]->play();
			break;
	}
}