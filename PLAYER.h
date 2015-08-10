/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"

#define NO_NPC 999

class LOCATION
{
	public:
		long x;
		long y;
		long z;
};

class DOOR
{
	public:
		LOCATION loc;
		bool is_open;
		bool need_key;
		long chunk_no;
};

class ITEM
{
	public:
		LOCATION loc;
		bool in_inventory;
		bool is_used;
		long chunk_no;
		long inv_vert_mod;
		bool is_visible;
};

class ENERGY_FIELD
{
	public:
		LOCATION loc;
		bool is_dispelled;
		long chunk_no;
};

class FOOD
{
	public:
		LOCATION loc;
		bool picked;
		long grow_count;
		long chunk_no;
};

class PERSON
{
	public:
		LOCATION loc;
		long chunk_no;
		char job;
		char job_mod;
		LOCATION job_loc1;
		LOCATION job_loc2;
};

class LEAVER
{
	public:
		LOCATION loc;
		long chunk_no[2];
		bool is_open;
		LOCATION mod_loc[32];
		long mod_chunk_no[32];
};

class CHEST
{
	public:
		LOCATION loc;
		long chunk_no[2];
		bool is_open;
		long gold;
};

class TELEPORTER
{
	public:
		LOCATION loc;
		long go_to;
};

#define MAX_QUESTS 32

class QUESTS
{
	public:
		bool done[MAX_QUESTS];
		//0 spoke_to_son
		//1 king_is_dead
		//2 king_has_moved
		//3 show_secret_door
		//4 know_password
		//5 gained_access
		//6 king_frozen
		//7 know_word_of_power
		//8 know_paralize
};

class PLAYER
{
	public:
		FILE_LIBRARY file_lib;

		LOCATION loc;
		long chunk_no;
		double energy;
		long gold;
		long food;
		long talking_to;
		long msg_no;

		long max_doors;
		DOOR *doors;
		long max_energy_fields;
		ENERGY_FIELD *energy_fields;
		long max_items;
		ITEM *items;
		long max_npcs;
		PERSON *populace;
		long max_vegies;
		FOOD *vegies;
		long max_leavers;
		LEAVER *leavers;
		long max_chests;
		CHEST *chests;
		long max_teleporters;
		TELEPORTER *teleporters;

		QUESTS quests;

		PLAYER();
		~PLAYER();
		void save_game(void);
		void load_game(void);
		void new_game(void);
		bool does_saved_game_exist(void);
		long *get_viewable(long x, long y, long z, long width, long height, long world_width, long world_height);
		long get_chunk(long x, long y, long z, long world_width, long world_height);
		long *get_viewable_npcs(long x, long y, long z, long width, long height, long world_width, long world_height);
		void update_dynamic(bool (*walkable_func)(PLAYER *, long, long, long, SCREEN *, WORLD *, TILES *), SCREEN *screen, WORLD *world, TILES *tiles);
		long manipulate_dynamic(long x, long y, long z, bool with_spell);
		void teleport(void);
		bool is_dead(void);
		bool has_won(void);
};

#endif //PLAYER_H
