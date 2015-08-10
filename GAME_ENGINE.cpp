/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include "game_engine.h"

#define SCREEN_BPP                  32   
#define VIEW_WIDTH                  21  
#define VIEW_HEIGHT                 15
#define VIEW_POS_X                  -16
#define VIEW_POS_Y                  0

#define MMENU_COLOUR_R              0x43
#define MMENU_COLOUR_G              0x00
#define MMENU_COLOUR_B              0x40
#define MMENU_COLOUR_R_MOD          18
#define MMENU_COLOUR_G_MOD          0
#define MMENU_COLOUR_B_MOD          0
#define MMENU_WIDTH                 404
#define MMENU_HEIGHT                300

#define SEQ_LOADING                 10000
#define SEQ_SHOW_LOGO               10001
#define SEQ_PROLOGUE                10002
#define SEQ_MAIN_MENU               10003
#define SEQ_PLAY_GAME               10004
#define SEQ_DEATH                   10005
#define SEQ_WINNER                  10006
#define SEQ_EXIT                    10007
#define SEQ_PROLOGUE_WAIT           10008
#define SEQ_TALKING                 10009
#define SEQ_LOADGAME                10010

#define GE_COPYRIGHT_LINES 5
char *ge_copyright_text[GE_COPYRIGHT_LINES] = 
{
	"COBOL's Revenge - Release 2a",
	"Copyright � 1998-2000 Makonin Consulting Corp.",
	"Music, Copyright � 1996 Joseph Fuoco",
	"",
	"Loading, please wait..."
};

#define INTRO_LINES 25
char *intro_text[INTRO_LINES] = 
{
	"",
	"",
	"...in years since the Name Tag War,",
	"you were once Commander COBOL, a valiant",
	"fighter who fought for Lord Bill.",
	"",
	"Flash backs of countless people who became",
	"nameless would plague your mind and",
	"destroy your career.",
	"",
	"Now you are just COBOL, a peasant who is",
	"shameful of what he did during the war.",
	"",
	"You helped Lord Bill become ruler of Ibum,",
	"and now his evil reign has broken the",
	"will of the people.",
	"",
	"In days before this, you have heard whispers",
	"of more evil deeds.",
	"",
	"You have learned that Lord Bill has sent guards",
	"to imprison those who helped him gain power,",
	"and you are one of them.",
	"",
	"But now it is late, you must get some sleep..."
};

#define ENDING_LINES 2
char *ending_text[ENDING_LINES] = 
{
	"In great reverance the the poeple of Ibum celebrate you vicroty.",
	" In some way you feel a burden has been lifted from your soul.",
};


bool npc_is_walkable(PLAYER *player, long x, long y, long z, SCREEN *screen, WORLD *world, TILES *tiles)
{
	VIEW *npc_view;
	bool ret_code;

	npc_view = new VIEW(3, 3, 1, 1, screen, world, tiles);
	ret_code = npc_view->generate_view(player, x, y, z);

	delete npc_view;
	return ret_code;
}

GAME_ENGINE::GAME_ENGINE(HWND main_window_handle, HINSTANCE hinstance_app)
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
	music = NULL;
	main_menu = NULL;
	player = NULL;

	try
	{
		screen = new SCREEN(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, main_window_handle, DOUBLE_BUF, FULL_SCREEN);																							 
		audio_mixer = new AUDIO_MIXER(main_window_handle);

		input_master = new INPUT_MASTER(hinstance_app);
		keyboard = new KEYBOARD(input_master, main_window_handle);
		mouse = new MOUSE(input_master, screen, main_window_handle);
		mouse->set_cursor("static/defltcur.cr");

		font = new FONT("Arial", 16, screen);
		font->set_colour(PURPLE_FONT, true);

		small_font = new FONT("Arial", 12, screen);
		small_font->set_colour(GOLDEN_FONT, true);

		main_menu = new MENU(screen, "static/prjbanr.cr", "Arial", 28);
		main_menu->setup_window(MMENU_WIDTH, MMENU_HEIGHT, MMENU_COLOUR_R, MMENU_COLOUR_G, MMENU_COLOUR_B, MMENU_COLOUR_R_MOD, MMENU_COLOUR_G_MOD, MMENU_COLOUR_B_MOD);
		main_menu->setup_input(mouse, keyboard, "static/defltcur.cr", "static/hrglscur.cr");
		main_menu->add_menu_item("� Continue Thy Quest", BRIGHT_FONT, PURPLE_FONT);
		main_menu->add_menu_item("� New Game", BRIGHT_FONT, PURPLE_FONT);
		main_menu->add_menu_item("� Save Game", BRIGHT_FONT, PURPLE_FONT);
		main_menu->add_menu_item("� Load Game", BRIGHT_FONT, PURPLE_FONT);
		main_menu->add_menu_item("� Enter Reality", BRIGHT_FONT, PURPLE_FONT);
		
		screen->clear(0);
		for(count = 0; count < GE_COPYRIGHT_LINES; count++)
			font->print(ge_copyright_text[count], 10, 10 + (16 * count));		
		screen->flip_page();
		Sleep(888);

		music = new MUSIC(audio_mixer);
		world = new WORLD("static/wrlddat.cr");
		tiles = new TILES("static/tlndx.cr", "static/tlimg.cr", screen);
		view = new VIEW(VIEW_WIDTH, VIEW_HEIGHT, VIEW_POS_X, VIEW_POS_Y, screen, world, tiles);
		player = new PLAYER();

		sounds[0] = new SOUND_BITE(audio_mixer);
		sounds[0]->load("static/hitsnd.cr");
		sounds[1] = new SOUND_BITE(audio_mixer);
		sounds[1]->load("static/telepsnd.cr");
		sounds[2] = new SOUND_BITE(audio_mixer);
		sounds[2]->load("static/ininvsnd.cr");
		sounds[3] = new SOUND_BITE(audio_mixer);
		sounds[3]->load("static/swtchsnd.cr");
		sounds[4] = new SOUND_BITE(audio_mixer);
		sounds[4]->load("static/opensnd.cr");
		sounds[5] = new SOUND_BITE(audio_mixer);
		sounds[5]->load("static/gtitmsnd.cr");
		sounds[6] = new SOUND_BITE(audio_mixer);
		sounds[6]->load("static/wlkudsnd.cr");
		sounds[7] = new SOUND_BITE(audio_mixer);
		sounds[7]->load("static/eatsnd.cr");
		sounds[8] = new SOUND_BITE(audio_mixer);
		sounds[8]->load("static/magicsnd.cr");
		sounds[9] = new SOUND_BITE(audio_mixer);
		sounds[9]->load("static/coinsnd.cr");
		sounds[10] = new SOUND_BITE(audio_mixer);
		sounds[10]->load("static/bltchsnd.cr");
		sounds[11] = new SOUND_BITE(audio_mixer);
		sounds[11]->load("static/knobsnd.cr");

		status_panel = new STATUS_PANEL(screen, tiles, player, mouse);
		talk_system = new TALK_SYSTEM(screen, keyboard, player, mouse, view, status_panel, sounds);	

		//company_logo();
		introduction();
	}
	catch(ERRORS e)
	{
		throw e;
	}
}

GAME_ENGINE::~GAME_ENGINE()
{
	delete talk_system;
	delete end_image;
	delete sounds[10];
	delete sounds[9];
	delete sounds[8];
	delete sounds[7];
	delete sounds[6];
	delete sounds[5];
	delete sounds[4];
	delete sounds[3];
	delete sounds[2];
	delete sounds[1];
	delete sounds[0];
	delete status_panel;
	delete player;
	delete view;
	delete tiles;
	delete world;
	delete music;
	delete main_menu;
}

bool GAME_ENGINE::update(char *error, void *parms, int num_parms)
{
	long item_select;
	bool update;
	static long reaction = 0;

	try
	{	
		keyboard->update();
		mouse->update();
		update = false;

		switch(sequence_mode)
		{
			case SEQ_EXIT:
				SendMessage(main_window_handle, WM_CLOSE, 0, 0);
				break;
			
			case SEQ_WINNER:
				record_tick_count();
				old_sequence_mode = sequence_mode;
				if(keyboard->was_pressed(DIK_ESCAPE))
					sequence_mode = SEQ_MAIN_MENU;
				if(!(get_cloak_ticker()%15))
					player->update_dynamic(npc_is_walkable, screen, world, tiles);
				reaction = view->generate_view(player, get_anim_index(), update);
				screen->put_image((screen->get_width() - end_image->get_width()) >> 1, 0, end_image);
				conclusion();
				mouse->update();
				screen->flip_page();
				wait_for_tick_count(FPS_15);
				incroment_counters();
				break;
			
			case SEQ_DEATH:
				record_tick_count();
				old_sequence_mode = sequence_mode;
				if(keyboard->was_pressed(DIK_ESCAPE))
					sequence_mode = SEQ_MAIN_MENU;
				if(!(get_cloak_ticker()%15))
					player->update_dynamic(npc_is_walkable, screen, world, tiles);
				reaction = view->generate_view(player, get_anim_index(), update);
				status_panel->show();
				mouse->update();
				screen->flip_page();
				wait_for_tick_count(FPS_15);
				incroment_counters();
				break;

			case SEQ_TALKING:
				talk_system->talk_with(player->talking_to, mouse->get_x(), mouse->get_y(), this);
				old_sequence_mode = sequence_mode;
				sequence_mode = SEQ_PLAY_GAME;
				player->talking_to = NO_NPC;
				break;
			
			case SEQ_PLAY_GAME:
				record_tick_count();

				if(reaction)
				{
					do_reactions(7000 + reaction);
				}
				else
				{
					if(player->is_dead())
					{
						music->stop();				
						music->play("static/deathmus.cr");
						mouse->set_cursor("static/hrglscur.cr");
						sequence_mode = SEQ_DEATH;
					}
					else if(player->has_won())
					{
						music->stop();				
						music->play("static/victorymus.cr");
						mouse->set_cursor("static/hrglscur.cr");
						end_image = new IMAGE(screen->get_ddobj7());
						end_image->load("static/winpic.cr");
						sequence_mode = SEQ_WINNER;
					}
					else
					{
						if(view->get_quadrant(mouse->get_x(), mouse->get_y()) == 1)
						{
							mouse->set_cursor("static/upcur.cr");
							if(mouse->right_button() && !(get_cloak_ticker()%3))
							{
								player->loc.y--;
								update = true;
							}
						}
						else if(view->get_quadrant(mouse->get_x(), mouse->get_y()) == 2)
						{
							mouse->set_cursor("static/leftcur.cr");
							if(mouse->right_button() && !(get_cloak_ticker()%3))
							{
								player->loc.x--;
								update = true;
							}
						}
						else if(view->get_quadrant(mouse->get_x(), mouse->get_y()) == 3)
						{
							mouse->set_cursor("static/rightcur.cr");
							if(mouse->right_button() && !(get_cloak_ticker()%3))
							{
								player->loc.x++;
								update = true;
							}
						}
						else if(view->get_quadrant(mouse->get_x(), mouse->get_y()) == 4)
						{
							mouse->set_cursor("static/downcur.cr");
							if(mouse->right_button() && !(get_cloak_ticker()%3))
							{
								player->loc.y++;
								update = true;
							}
						}

						mouse->update();
						if(mouse->left_button(true))
						{
							if(status_panel->is_mouse_in())
							{
								if(player->food > 0 && status_panel->is_mouse_in_food())
								{
									player->energy = LIMIT(player->energy + 5.5, 0, 100);
									player->food -= 1;
									sounds[7]->play();
									player->msg_no = 3;
									if((rand() % 100) > 79)
									{
										Sleep(1500);
										sounds[10]->play();
									}
									record_tick_count2();
								}
							}
							else
							{
								do_reactions(8000 + view->use_chunk(player, mouse->get_x(), mouse->get_y(), player->items[3].in_inventory));
								update = true;
								old_sequence_mode = sequence_mode;
								if(player->talking_to != NO_NPC)
									 sequence_mode = SEQ_TALKING;
							}
						}

						if(keyboard->was_pressed(DIK_LEFT))
						{
							player->loc.x--;
							update = true;
						}

						if(keyboard->was_pressed(DIK_RIGHT))
						{
							player->loc.x++;
							update = true;
						}

						if(keyboard->was_pressed(DIK_UP))
						{
							player->loc.y--;
							update = true;
						}

						if(keyboard->was_pressed(DIK_DOWN))
						{
							player->loc.y++;				
							update = true;
						}

						player->loc.x = FIX_WORLD_COORD(player->loc.x);
						player->loc.y = FIX_WORLD_COORD(player->loc.y);
					}

					if(keyboard->was_pressed(DIK_ESCAPE))
						sequence_mode = SEQ_MAIN_MENU;
				}

				if(!(get_cloak_ticker()%15))
					player->update_dynamic(npc_is_walkable, screen, world, tiles);

				if(get_cloak_ticker2() > 32)
					player->msg_no = 0;

				reaction = view->generate_view(player, get_anim_index(), update);
				status_panel->show();
				mouse->update();

				if(keyboard->was_pressed(DIK_X, false) && !player->is_dead())
				{
					if(status_panel->is_mouse_in())
						small_font->print(status_panel->get_desc(), mouse->get_x(), mouse->get_y());
					else
						small_font->print(view->get_chunk_desc(mouse->get_x(), mouse->get_y(), player), mouse->get_x(), mouse->get_y());
				}

				screen->flip_page();

				wait_for_tick_count(FPS_15);
				incroment_counters();
				break;
			
			case SEQ_MAIN_MENU:
				view->generate_view(player, get_anim_index(), false);
				item_select = main_menu->show(view, true, true, player);				
				do_reactions(9000 + item_select);
				screen->clear(0);
				view->generate_view(player, get_anim_index(), false);
				main_menu->hide(view, player);
				break;
			
			case SEQ_LOADGAME:
				mouse->show();
				old_sequence_mode = SEQ_PLAY_GAME;
				view->generate_view(player, get_anim_index(), false);
				if(player->does_saved_game_exist())
					player->load_game();
				else
					talk_system->show_intro2();
				music->stop();	
				sequence_mode = SEQ_PLAY_GAME;
				music->play("static/walkmus.cr");
				break;


			case SEQ_PROLOGUE_WAIT:
				screen->clear(0);
				screen->put_image(screen->get_width() - begin_image->get_width(), 0, begin_image);

				font->set_colour(BRIGHT_FONT, true);
				font->print(intro_text[0], 10, 10);
				font->set_colour(PURPLE_FONT, true);
				for(item_select = 1; item_select < INTRO_LINES; item_select++)
					font->print(intro_text[item_select], 10, 10 + (16 * item_select));

				screen->flip_page();

				if(keyboard->was_pressed(DIK_ESCAPE) || check_tick_count(35000))
				{
					sequence_mode = SEQ_LOADGAME;
				}
				break;

		}

		const_pole();
	}
	catch(ERRORS e)
	{	
		strcpy(error, e.get_message());
		return true;
	}
	
	return false;
}

void GAME_ENGINE::wait_for_esc(void)
{
	while(!keyboard->was_pressed(DIK_ESCAPE)) 
	{
		keyboard->update(); 
		mouse->update(); 
		view->generate_view(player);
	}
}

void GAME_ENGINE::company_logo(void)
{
	RECT rec;
	long count;
	double percent = 0.00;
	IMAGE *background = NULL;
	IMAGE *girl_and_rabit = NULL;
	SOUND_BITE *flyby = NULL;
	float rx = 317.00, ry = 139.00;
	float vx, vy;
	float theta;

	try
	{	
		/*MOVIE *movie = NULL;
		movie = new MOVIE("static/ouatmov.cr", screen, audio_mixer, keyboard);
		movie->set_position(0, 90, 640, 300);
		screen->clear_all(0);
		if(movie->play(30))
			sequence_mode = SEQ_PROLOGUE;
		else
			sequence_mode = SEQ_MAIN_MENU;
		delete movie;*/

		/*
		flyby = new SOUND_BITE(audio_mixer);
		flyby->load("static/flybysnd.cr");
		
		background = new IMAGE(screen->get_ddobj7());
		background->load("static/ouatpic.cr");
		girl_and_rabit = new IMAGE(screen->get_ddobj7());
		girl_and_rabit->load("static/gjrpic.cr");
		girl_and_rabit->set_key_colour(_RGB_WHITE);

		screen->clear_all(0);
		flyby->play();
		for(count=0; count < 101; count++)
		{
			record_tick_count();

			theta = (float)(count * 2.5) * (float)PI / (float)180.00;
			vx = (float)10.1000 * (float)cos(theta);
			vy = (float)2.8000 * (float)sin(theta);
			screen->put_image(0, 0, background);
			rec.top = (long)ry;
			rec.bottom = rec.top + (long)(girl_and_rabit->get_height() * percent);
			rec.left = (long)rx - count;
			rec.right = rec.left + (long)(girl_and_rabit->get_width() * percent);
			screen->put_image(&rec, girl_and_rabit, true);
			screen->flip_page();		
			percent = count / 100.00;
			rx += vx;
			ry += vy;

			wait_for_tick_count(FPS_30);
		}
		for(count=0; count >= -400; count -= 10)
		{
			record_tick_count();

			screen->put_image(0, 0, background);
			screen->put_image(count, (long)ry - (count / 5), girl_and_rabit, true);
			screen->flip_page();		

			wait_for_tick_count(FPS_30);
		}
		
		delete background;
		delete girl_and_rabit;
		delete flyby;
		*/
	}
	catch(ERRORS e)
	{
		throw e;
	}
}

void GAME_ENGINE::introduction(void)
{
	try
	{	
		begin_image = new IMAGE(screen->get_ddobj7());
		begin_image->load("static/intropic.cr");

		music->play("static/storymus.cr");

		sequence_mode = SEQ_PROLOGUE_WAIT;
		record_tick_count();
	}
	catch(ERRORS e)
	{
		throw e;
	}
}


void GAME_ENGINE::conclusion(void)
{
	long count;

	try
	{	
		font->set_colour(BLUE_FONT2, false);
		for(count = 0; count < ENDING_LINES; count++)
			font->print(ending_text[count], 142, (16 * count));

		font->set_colour(GOLDEN_FONT, true);
		font->print("-- THE END --", 283, 455);
	}
	catch(ERRORS e)
	{
		throw e;
	}
}

void GAME_ENGINE::do_reactions(long no)
{
	switch(no)
	{
		case 7001:
			player->loc.z += 1;
			sounds[6]->play();
			wait_for_tick_count(1000);
			break;
		case 7002:
			player->loc.z -= 1;
			sounds[6]->play();
			wait_for_tick_count(1000);
			break;
		case 7003:
			player->energy = LIMIT(player->energy - 10, 0, 100);
			sounds[0]->play();
			break;
		case 7004:
			player->energy = LIMIT(player->energy - 20, 0, 100);
			sounds[0]->play();
			break;
		case 7005:
			player->teleport();
			sounds[1]->play();
			break;
		case 8001:
			sounds[2]->play();
			break;
		case 8002:
			sounds[3]->play();
			break;
		case 8003:
			sounds[8]->play();
			break;
		case 8004:
			sounds[4]->play();
			break;
		case 8005:
			sounds[5]->play();
			sounds[2]->play();
			break;
		case 8006:
			sounds[4]->play();
			Sleep(150);
			sounds[9]->play();
			record_tick_count2();
			break;
		case 8007:
			record_tick_count2();
			break;
		case 8008:
			sounds[11]->play();
			record_tick_count2();
			break;
		case 8999:
		case 9000:
			sequence_mode = old_sequence_mode;
			break;
		case 9001:
			music->stop();
			music->play("static/storymus.cr");
			player->new_game();
			talk_system->show_intro2();
			music->stop();	
			sequence_mode = SEQ_PLAY_GAME;
			music->play("static/walkmus.cr");
			break;
		case 9002:
			if(player->is_dead())
			{
				player->msg_no = 6;
			}
			else
			{
				player->save_game();
				sequence_mode = SEQ_PLAY_GAME;
				player->msg_no = 5;
			}
			record_tick_count2();
			break;
		case 9003:
			player->load_game();
			sequence_mode = SEQ_LOADGAME;
			break;
		case 9004:
			sequence_mode = SEQ_EXIT;
			break;
	}
}

void GAME_ENGINE::const_pole(void)
{
	if(!player->is_dead() && !player->has_won())
		music->check_status();
}

