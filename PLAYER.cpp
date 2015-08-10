/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include "globals.h"

PLAYER::PLAYER()
{
	doors = NULL;
	energy_fields = NULL;
	items = NULL;
	populace = NULL;
	vegies = NULL;
	leavers = NULL;
	talking_to = NO_NPC;
	msg_no = 0;

	new_game();
}

PLAYER::~PLAYER()
{
	if(doors)
		delete doors;

	if(energy_fields)
		delete energy_fields;

	if(items)
		delete items;

	if(populace)
		delete populace;

	if(vegies)
		delete vegies;

	if(leavers)
		delete leavers;

}

void PLAYER::save_game(void)
{
	FILE *fp;

	fp = fopen("player.sav", "wb");

	fwrite(&loc, sizeof(loc), 1, fp);
	fwrite(&chunk_no, sizeof(chunk_no), 1, fp);
	fwrite(&energy, sizeof(energy), 1, fp);
	fwrite(&gold, sizeof(gold), 1, fp);
	fwrite(&food, sizeof(food), 1, fp);

	fwrite(&max_doors, sizeof(max_doors), 1, fp);
	fwrite(doors, sizeof(DOOR), max_doors, fp);

	fwrite(&max_energy_fields, sizeof(max_energy_fields), 1, fp);
	fwrite(energy_fields, sizeof(ENERGY_FIELD), max_energy_fields, fp);

	fwrite(&max_items, sizeof(max_items), 1, fp);
	fwrite(items, sizeof(ITEM), max_items, fp);

	fwrite(&max_npcs, sizeof(max_npcs), 1, fp);
	fwrite(populace, sizeof(PERSON), max_npcs, fp);

	fwrite(&max_vegies, sizeof(max_vegies), 1, fp);
	fwrite(vegies, sizeof(FOOD), max_vegies, fp);

	fwrite(&max_leavers, sizeof(max_leavers), 1, fp);
	fwrite(leavers, sizeof(LEAVER), max_leavers, fp);

	fwrite(&max_chests, sizeof(max_chests), 1, fp);
	fwrite(chests, sizeof(CHEST), max_chests, fp);

	fwrite(&max_teleporters, sizeof(max_teleporters), 1, fp);
	fwrite(teleporters, sizeof(TELEPORTER), max_teleporters, fp);

	fwrite(&quests, sizeof(quests), 1, fp);

	fclose(fp);
}

void PLAYER::load_game(void)
{
	FILE *fp;

	fp = fopen("player.sav", "rb");

	fread(&loc, sizeof(loc), 1, fp);
	fread(&chunk_no, sizeof(chunk_no), 1, fp);
	fread(&energy, sizeof(energy), 1, fp);
	fread(&gold, sizeof(gold), 1, fp);
	fread(&food, sizeof(food), 1, fp);

	fread(&max_doors, sizeof(max_doors), 1, fp);
	fread(doors, sizeof(DOOR), max_doors, fp);

	fread(&max_energy_fields, sizeof(max_energy_fields), 1, fp);
	fread(energy_fields, sizeof(ENERGY_FIELD), max_energy_fields, fp);

	fread(&max_items, sizeof(max_items), 1, fp);
	fread(items, sizeof(ITEM), max_items, fp);

	fread(&max_npcs, sizeof(max_npcs), 1, fp);
	fread(populace, sizeof(PERSON), max_npcs, fp);

	fread(&max_vegies, sizeof(max_vegies), 1, fp);
	fread(vegies, sizeof(FOOD), max_vegies, fp);

	fread(&max_leavers, sizeof(max_leavers), 1, fp);
	fread(leavers, sizeof(LEAVER), max_leavers, fp);

	fread(&max_chests, sizeof(max_chests), 1, fp);
	fread(chests, sizeof(CHEST), max_chests, fp);

	fread(&max_teleporters, sizeof(max_teleporters), 1, fp);
	fread(teleporters, sizeof(TELEPORTER), max_teleporters, fp);

	fread(&quests, sizeof(quests), 1, fp);

	fclose(fp);
}

void PLAYER::new_game(void)
{
	int i;

	PLAYER::~PLAYER();

	loc.x = 26;
	loc.y = 10;
	loc.z = 4;

	chunk_no = 41;
	energy = 83.2;
	gold = 10;
	food = 1;

	max_doors = 15;
	doors = new DOOR[max_doors];
	memset(doors, 0, sizeof(DOOR) * max_doors);
	
	doors[0].chunk_no = 67;
	doors[0].need_key = false;
	doors[0].is_open = false;
	doors[0].loc.x = 8;
	doors[0].loc.y = 30;
	doors[0].loc.z = 4;

	doors[1].chunk_no = 67;
	doors[1].need_key = false;
	doors[1].is_open = false;
	doors[1].loc.x = 8;
	doors[1].loc.y = 20;
	doors[1].loc.z = 4;

	doors[2].chunk_no = 67;
	doors[2].need_key = false;
	doors[2].is_open = false;
	doors[2].loc.x = 8;
	doors[2].loc.y = 15;
	doors[2].loc.z = 4;

	doors[3].chunk_no = 67;
	doors[3].need_key = false;
	doors[3].is_open = false;
	doors[3].loc.x = 54;
	doors[3].loc.y = 29;
	doors[3].loc.z = 4;

	doors[4].chunk_no = 67;
	doors[4].need_key = false;
	doors[4].is_open = false;
	doors[4].loc.x = 62;
	doors[4].loc.y = 29;
	doors[4].loc.z = 0;

	doors[5].chunk_no = 67;
	doors[5].need_key = false;
	doors[5].is_open = false;
	doors[5].loc.x = 58;
	doors[5].loc.y = 36;
	doors[5].loc.z = 4;

	doors[6].chunk_no = 67;
	doors[6].need_key = false;
	doors[6].is_open = false;
	doors[6].loc.x = 36;
	doors[6].loc.y = 55;
	doors[6].loc.z = 4;

	doors[7].chunk_no = 67;
	doors[7].need_key = false;
	doors[7].is_open = false;
	doors[7].loc.x = 48;
	doors[7].loc.y = 15;
	doors[7].loc.z = 5;

	doors[8].chunk_no = 67;
	doors[8].need_key = false;
	doors[8].is_open = false;
	doors[8].loc.x = 48;
	doors[8].loc.y = 17;
	doors[8].loc.z = 5;

	doors[9].chunk_no = 67;
	doors[9].need_key = false;
	doors[9].is_open = false;
	doors[9].loc.x = 52;
	doors[9].loc.y = 20;
	doors[9].loc.z = 5;

	doors[10].chunk_no = 67;
	doors[10].need_key = false;
	doors[10].is_open = false;
	doors[10].loc.x = 56;
	doors[10].loc.y = 15;
	doors[10].loc.z = 5;

	doors[11].chunk_no = 67;
	doors[11].need_key = false;
	doors[11].is_open = false;
	doors[11].loc.x = 56;
	doors[11].loc.y = 17;
	doors[11].loc.z = 5;

	doors[12].chunk_no = 68;
	doors[12].need_key = true;
	doors[12].is_open = false;
	doors[12].loc.x = 35;
	doors[12].loc.y = 22;
	doors[12].loc.z = 5;

	doors[13].chunk_no = 68;
	doors[13].need_key = false;
	doors[13].is_open = false;
	doors[13].loc.x = 25;
	doors[13].loc.y = 15;
	doors[13].loc.z = 6;

	doors[14].chunk_no = 68;
	doors[14].need_key = false;
	doors[14].is_open = false;
	doors[14].loc.x = 37;
	doors[14].loc.y = 15;
	doors[14].loc.z = 6;

	max_energy_fields = 7;
	energy_fields = new ENERGY_FIELD[max_energy_fields];
	memset(energy_fields, 0, sizeof(ENERGY_FIELD) * max_energy_fields);

	energy_fields[0].chunk_no = 74;
	energy_fields[0].is_dispelled = false;
	energy_fields[0].loc.x = 38;
	energy_fields[0].loc.y = 23;
	energy_fields[0].loc.z = 4;

	energy_fields[1].chunk_no = 74;
	energy_fields[1].is_dispelled = false;
	energy_fields[1].loc.x = 33;
	energy_fields[1].loc.y = 57;
	energy_fields[1].loc.z = 4;

	energy_fields[2].chunk_no = 74;
	energy_fields[2].is_dispelled = false;
	energy_fields[2].loc.x = 29;
	energy_fields[2].loc.y = 22;
	energy_fields[2].loc.z = 3;

	energy_fields[3].chunk_no = 74;
	energy_fields[3].is_dispelled = false;
	energy_fields[3].loc.x = 29;
	energy_fields[3].loc.y = 24;
	energy_fields[3].loc.z = 3;

	energy_fields[4].chunk_no = 74;
	energy_fields[4].is_dispelled = false;
	energy_fields[4].loc.x = 17;
	energy_fields[4].loc.y = 22;
	energy_fields[4].loc.z = 3;

	energy_fields[5].chunk_no = 74;
	energy_fields[5].is_dispelled = false;
	energy_fields[5].loc.x = 17;
	energy_fields[5].loc.y = 24;
	energy_fields[5].loc.z = 3;

	energy_fields[6].chunk_no = 74;
	energy_fields[6].is_dispelled = false;
	energy_fields[6].loc.x = 121;
	energy_fields[6].loc.y = 23;
	energy_fields[6].loc.z = 3;

	max_items = 4;
	items = new ITEM[max_items];
	memset(items, 0, sizeof(ITEM) * max_items);

	items[0].chunk_no = 73;
	items[0].in_inventory = false;
	items[0].is_used = false;
	items[0].loc.x = 36;
	items[0].loc.y = 12;
	items[0].loc.z = 4;
	items[0].inv_vert_mod = 5;
	items[0].is_visible = false;

	items[1].chunk_no = 52;
	items[1].in_inventory = false;
	items[1].is_used = false;
	items[1].loc.x = 32;
	items[1].loc.y = 17;
	items[1].loc.z = 5;
	items[1].inv_vert_mod = 0;
	items[1].is_visible = true;

	items[2].chunk_no = 213;
	items[2].in_inventory = false;
	items[2].is_used = false;
	items[2].loc.x = 0;
	items[2].loc.y = 0;
	items[2].loc.z = 0;
	items[2].inv_vert_mod = -4;
	items[2].is_visible = false;

	items[3].chunk_no = 53;
	items[3].in_inventory = false;
	items[3].is_used = false;
	items[3].loc.x = 0;
	items[3].loc.y = 0;
	items[3].loc.z = 0;
	items[3].inv_vert_mod = 0;//-3;
	items[3].is_visible = false;

	max_npcs = 48;
	populace = new PERSON[max_npcs];
	memset(populace, 0, sizeof(PERSON) * max_npcs);
	
	populace[0].chunk_no = 206;
	populace[0].job = 'S';
	populace[0].loc.x = 23;
	populace[0].loc.y = 11;
	populace[0].loc.z = 4;

	populace[1].chunk_no = 204;
	populace[1].job = 'R';
	populace[1].loc.x = 10;
	populace[1].loc.y = 13;
	populace[1].loc.z = 4;
	populace[1].job_mod = 1;
	populace[1].job_loc1.x = 1;
	populace[1].job_loc1.y = 6;
	populace[1].job_loc1.z = 4;
	populace[1].job_loc2.x = 11;
	populace[1].job_loc2.y = 14;
	populace[1].job_loc2.z = 4;

	populace[2].chunk_no = 197;
	populace[2].job = 'R';
	populace[2].loc.x = 54;
	populace[2].loc.y = 16;
	populace[2].loc.z = 4;	
	populace[2].job_mod = 1;
	populace[2].job_loc1.x = 52;
	populace[2].job_loc1.y = 13;
	populace[2].job_loc1.z = 4;
	populace[2].job_loc2.x = 57;
	populace[2].job_loc2.y = 17;
	populace[2].job_loc2.z = 4;

	populace[3].chunk_no = 196;
	populace[3].job = 'R';
	populace[3].loc.x = 58;
	populace[3].loc.y = 40;
	populace[3].loc.z = 4;
	populace[3].job_mod = 1;
	populace[3].job_loc1.x = 54;
	populace[3].job_loc1.y = 37;
	populace[3].job_loc1.z = 4;
	populace[3].job_loc2.x = 62;
	populace[3].job_loc2.y = 44;
	populace[3].job_loc2.z = 4;

	populace[4].chunk_no = 195;
	populace[4].job = 'R';
	populace[4].loc.x = 34;
	populace[4].loc.y = 59;
	populace[4].loc.z = 4;
	populace[4].job_mod = 1;
	populace[4].job_loc1.x = 29;
	populace[4].job_loc1.y = 52;
	populace[4].job_loc1.z = 4;
	populace[4].job_loc2.x = 40;
	populace[4].job_loc2.y = 61;
	populace[4].job_loc2.z = 4;

	populace[5].chunk_no = 161;
	populace[5].job = 'H';
	populace[5].loc.x = 37;
	populace[5].loc.y = 11;
	populace[5].loc.z = 4;
	populace[5].job_mod = 1;
	populace[5].job_loc1.x = 35;
	populace[5].job_loc1.y = 11;
	populace[5].job_loc1.z = 4;
	populace[5].job_loc2.x = 39;
	populace[5].job_loc2.y = 11;
	populace[5].job_loc2.z = 4;

	populace[6].chunk_no = 160;
	populace[6].job = 'S';
	populace[6].loc.x = 33;
	populace[6].loc.y = 32;
	populace[6].loc.z = 4;

	populace[7].chunk_no = 159;
	populace[7].job = 'S';
	populace[7].loc.x = 13;
	populace[7].loc.y = 41;
	populace[7].loc.z = 4;

	populace[8].chunk_no = 158;
	populace[8].job = 'R';
	populace[8].loc.x = 5;
	populace[8].loc.y = 32;
	populace[8].loc.z = 4;
	populace[8].job_mod = 1;
	populace[8].job_loc1.x = 1;
	populace[8].job_loc1.y = 24;
	populace[8].job_loc1.z = 4;
	populace[8].job_loc2.x = 17;
	populace[8].job_loc2.y = 47;
	populace[8].job_loc2.z = 4;

	populace[9].chunk_no = 157;
	populace[9].job = 'R';
	populace[9].loc.x = 31;
	populace[9].loc.y = 10;
	populace[9].loc.z = 6;
	populace[9].job_mod = 1;
	populace[9].job_loc1.x = 26;
	populace[9].job_loc1.y = 9;
	populace[9].job_loc1.z = 6;
	populace[9].job_loc2.x = 36;
	populace[9].job_loc2.y = 13;
	populace[9].job_loc2.z = 6;

	populace[10].chunk_no = 123;
	populace[10].job = 'R';
	populace[10].loc.x = 32;
	populace[10].loc.y = 13;
	populace[10].loc.z = 6;
	populace[10].job_mod = 1;
	populace[10].job_loc1.x = 31;
	populace[10].job_loc1.y = 11;
	populace[10].job_loc1.z = 6;
	populace[10].job_loc2.x = 39;
	populace[10].job_loc2.y = 19;
	populace[10].job_loc2.z = 6;

	populace[11].chunk_no = 88;
	populace[11].job = 'R';
	populace[11].loc.x = 56;
	populace[11].loc.y = 19;
	populace[11].loc.z = 4;	
	populace[11].job_mod = 1;
	populace[11].job_loc1.x = 56;
	populace[11].job_loc1.y = 14;
	populace[11].job_loc1.z = 4;
	populace[11].job_loc2.x = 58;
	populace[11].job_loc2.y = 28;
	populace[11].job_loc2.z = 4;

	populace[12].chunk_no = 210;
	populace[12].job = 'T';
	populace[12].loc.x = 68;
	populace[12].loc.y = 57;
	populace[12].loc.z = 4;	
	populace[12].job_mod = 1;
	populace[12].job_loc1.x = 59;
	populace[12].job_loc1.y = 46;
	populace[12].job_loc1.z = 4;
	populace[12].job_loc2.x = 76;
	populace[12].job_loc2.y = 65;
	populace[12].job_loc2.z = 4;

	populace[13].chunk_no = 89;
	populace[13].job = 'R';
	populace[13].loc.x = 54;
	populace[13].loc.y = 44;
	populace[13].loc.z = 4;	
	populace[13].job_mod = 1;
	populace[13].job_loc1.x = 51;
	populace[13].job_loc1.y = 36;
	populace[13].job_loc1.z = 4;
	populace[13].job_loc2.x = 65;
	populace[13].job_loc2.y = 45;
	populace[13].job_loc2.z = 4;

	populace[14].chunk_no = 198;
	populace[14].job = 'S';
	populace[14].loc.x = 48;
	populace[14].loc.y = 14;
	populace[14].loc.z = 0;	

	populace[15].chunk_no = 218;
	populace[15].job = 'R';
	populace[15].loc.x = 64;
	populace[15].loc.y = 3;
	populace[15].loc.z = 0;	
	populace[15].job_mod = 1;
	populace[15].job_loc1.x = 58;
	populace[15].job_loc1.y = 0;
	populace[15].job_loc1.z = 4;
	populace[15].job_loc2.x = 66;
	populace[15].job_loc2.y = 10;
	populace[15].job_loc2.z = 4;

	populace[16].chunk_no = 211;
	populace[16].job = 'S';
	populace[16].loc.x = 21;
	populace[16].loc.y = 26;
	populace[16].loc.z = 4;	

	populace[17].chunk_no = 211;
	populace[17].job = 'S';
	populace[17].loc.x = 21;
	populace[17].loc.y = 26;
	populace[17].loc.z = 5;	

	populace[18].chunk_no = 211;
	populace[18].job = 'S';
	populace[18].loc.x = 21;
	populace[18].loc.y = 26;
	populace[18].loc.z = 6;	

	populace[19].chunk_no = 211;
	populace[19].job = 'S';
	populace[19].loc.x = 21;
	populace[19].loc.y = 26;
	populace[19].loc.z = 7;	

	populace[20].chunk_no = 211;
	populace[20].job = 'S';
	populace[20].loc.x = 21;
	populace[20].loc.y = 7;
	populace[20].loc.z = 4;	

	populace[21].chunk_no = 211;
	populace[21].job = 'S';
	populace[21].loc.x = 21;
	populace[21].loc.y = 7;
	populace[21].loc.z = 5;	

	populace[22].chunk_no = 211;
	populace[22].job = 'S';
	populace[22].loc.x = 21;
	populace[22].loc.y = 7;
	populace[22].loc.z = 6;	

	populace[23].chunk_no = 211;
	populace[23].job = 'S';
	populace[23].loc.x = 21;
	populace[23].loc.y = 7;
	populace[23].loc.z = 7;	

	populace[24].chunk_no = 198;
	populace[24].job = 'S';
	populace[24].loc.x = 41;
	populace[24].loc.y = 7;
	populace[24].loc.z = 4;	

	populace[25].chunk_no = 198;
	populace[25].job = 'S';
	populace[25].loc.x = 41;
	populace[25].loc.y = 7;
	populace[25].loc.z = 5;	

	populace[26].chunk_no = 198;
	populace[26].job = 'S';
	populace[26].loc.x = 41;
	populace[26].loc.y = 7;
	populace[26].loc.z = 6;	

	populace[27].chunk_no = 198;
	populace[27].job = 'S';
	populace[27].loc.x = 41;
	populace[27].loc.y = 7;
	populace[27].loc.z = 7;	

	populace[28].chunk_no = 198;
	populace[28].job = 'S';
	populace[28].loc.x = 41;
	populace[28].loc.y = 26;
	populace[28].loc.z = 4;	

	populace[29].chunk_no = 198;
	populace[29].job = 'S';
	populace[29].loc.x = 41;
	populace[29].loc.y = 26;
	populace[29].loc.z = 5;	

	populace[30].chunk_no = 198;
	populace[30].job = 'S';
	populace[30].loc.x = 41;
	populace[30].loc.y = 26;
	populace[30].loc.z = 6;	

	populace[31].chunk_no = 198;
	populace[31].job = 'S';
	populace[31].loc.x = 41;
	populace[31].loc.y = 26;
	populace[31].loc.z = 7;	

	populace[32].chunk_no = 211;
	populace[32].job = 'S';
	populace[32].loc.x = 38;
	populace[32].loc.y = 17;
	populace[32].loc.z = 4;	

	populace[33].chunk_no = 198;
	populace[33].job = 'S';
	populace[33].loc.x = 37;
	populace[33].loc.y = 15;
	populace[33].loc.z = 4;	

	populace[34].chunk_no = 211;
	populace[34].job = 'R';
	populace[34].loc.x = 26;
	populace[34].loc.y = 16;
	populace[34].loc.z = 5;	
	populace[34].job_mod = 1;
	populace[34].job_loc1.x = 23;
	populace[34].job_loc1.y = 10;
	populace[34].job_loc1.z = 5;
	populace[34].job_loc2.x = 39;
	populace[34].job_loc2.y = 21;
	populace[34].job_loc2.z = 5;

	populace[35].chunk_no = 198;
	populace[35].job = 'R';
	populace[35].loc.x = 33;
	populace[35].loc.y = 13;
	populace[35].loc.z = 5;	
	populace[35].job_mod = 1;
	populace[35].job_loc1.x = 23;
	populace[35].job_loc1.y = 10;
	populace[35].job_loc1.z = 5;
	populace[35].job_loc2.x = 39;
	populace[35].job_loc2.y = 21;
	populace[35].job_loc2.z = 5;

	populace[36].chunk_no = 211;
	populace[36].job = 'R';
	populace[36].loc.x = 26;
	populace[36].loc.y = 10;
	populace[36].loc.z = 5;	
	populace[36].job_mod = 1;
	populace[36].job_loc1.x = 23;
	populace[36].job_loc1.y = 10;
	populace[36].job_loc1.z = 5;
	populace[36].job_loc2.x = 39;
	populace[36].job_loc2.y = 21;
	populace[36].job_loc2.z = 5;

	populace[37].chunk_no = 211;
	populace[37].job = 'S';
	populace[37].loc.x = 25;
	populace[37].loc.y = 10;
	populace[37].loc.z = 6;	

	populace[38].chunk_no = 211;
	populace[38].job = 'S';
	populace[38].loc.x = 28;
	populace[38].loc.y = 8;
	populace[38].loc.z = 6;	

	populace[39].chunk_no = 211;
	populace[39].job = 'S';
	populace[39].loc.x = 29;
	populace[39].loc.y = 15;
	populace[39].loc.z = 6;	

	populace[40].chunk_no = 198;
	populace[40].job = 'S';
	populace[40].loc.x = 34;
	populace[40].loc.y = 8;
	populace[40].loc.z = 6;	

	populace[41].chunk_no = 198;
	populace[41].job = 'S';
	populace[41].loc.x = 37;
	populace[41].loc.y = 10;
	populace[41].loc.z = 6;	

	populace[42].chunk_no = 198;
	populace[42].job = 'S';
	populace[42].loc.x = 33;
	populace[42].loc.y = 15;
	populace[42].loc.z = 6;	

	populace[43].chunk_no = 211;
	populace[43].job = 'S';
	populace[43].loc.x = 29;
	populace[43].loc.y = 23;
	populace[43].loc.z = 7;	

	populace[44].chunk_no = 198;
	populace[44].job = 'S';
	populace[44].loc.x = 33;
	populace[44].loc.y = 23;
	populace[44].loc.z = 7;	

	populace[45].chunk_no = 176;
	populace[45].job = 'A';
	populace[45].loc.x = 33;
	populace[45].loc.y = 27;
	populace[45].loc.z = 4;
	populace[45].job_mod = 1;
	populace[45].job_loc1.x = 17;
	populace[45].job_loc1.y = 0;
	populace[45].job_loc1.z = 4;
	populace[45].job_loc2.x = 45;
	populace[45].job_loc2.y = 29;
	populace[45].job_loc2.z = 4;

	populace[46].chunk_no = 198;
	populace[46].job = 'S';
	populace[46].loc.x = 31;
	populace[46].loc.y = 17;
	populace[46].loc.z = 7;	

	populace[47].chunk_no = 158;
	populace[47].job = 'R';
	populace[47].loc.x = 42;
	populace[47].loc.y = 55;
	populace[47].loc.z = 4;
	populace[47].job_mod = 1;
	populace[47].job_loc1.x = 29;
	populace[47].job_loc1.y = 51;
	populace[47].job_loc1.z = 4;
	populace[47].job_loc2.x = 46;
	populace[47].job_loc2.y = 67;
	populace[47].job_loc2.z = 4;

	max_vegies = 16;
	vegies = new FOOD[max_vegies];
	memset(vegies, 0, sizeof(FOOD) * max_vegies);

	vegies[0].chunk_no = 85;
	vegies[0].picked = false;
	vegies[0].grow_count = 0;
	vegies[0].loc.x = 5;
	vegies[0].loc.y = 9;
	vegies[0].loc.z = 4;

	vegies[1].chunk_no = 85;
	vegies[1].picked = false;
	vegies[1].grow_count = 0;
	vegies[1].loc.x = 5;
	vegies[1].loc.y = 10;
	vegies[1].loc.z = 4;

	vegies[2].chunk_no = 85;
	vegies[2].picked = false;
	vegies[2].grow_count = 0;
	vegies[2].loc.x = 5;
	vegies[2].loc.y = 11;
	vegies[2].loc.z = 4;

	vegies[3].chunk_no = 86;
	vegies[3].picked = false;
	vegies[3].grow_count = 0;
	vegies[3].loc.x = 8;
	vegies[3].loc.y = 7;
	vegies[3].loc.z = 4;

	vegies[4].chunk_no = 86;
	vegies[4].picked = false;
	vegies[4].grow_count = 0;
	vegies[4].loc.x = 8;
	vegies[4].loc.y = 8;
	vegies[4].loc.z = 4;

	vegies[5].chunk_no = 86;
	vegies[5].picked = false;
	vegies[5].grow_count = 0;
	vegies[5].loc.x = 8;
	vegies[5].loc.y = 9;
	vegies[5].loc.z = 4;

	vegies[6].chunk_no = 86;
	vegies[6].picked = false;
	vegies[6].grow_count = 0;
	vegies[6].loc.x = 4;
	vegies[6].loc.y = 16;
	vegies[6].loc.z = 4;

	vegies[7].chunk_no = 87;
	vegies[7].picked = false;
	vegies[7].grow_count = 0;
	vegies[7].loc.x = 11;
	vegies[7].loc.y = 6;
	vegies[7].loc.z = 4;

	vegies[8].chunk_no = 87;
	vegies[8].picked = false;
	vegies[8].grow_count = 0;
	vegies[8].loc.x = 11;
	vegies[8].loc.y = 7;
	vegies[8].loc.z = 4;

	vegies[9].chunk_no = 87;
	vegies[9].picked = false;
	vegies[9].grow_count = 0;
	vegies[9].loc.x = 11;
	vegies[9].loc.y = 8;
	vegies[9].loc.z = 4;

	vegies[10].chunk_no = 87;
	vegies[10].picked = false;
	vegies[10].grow_count = 0;
	vegies[10].loc.x = 11;
	vegies[10].loc.y = 11;
	vegies[10].loc.z = 4;

	vegies[11].chunk_no = 85;
	vegies[11].picked = false;
	vegies[11].grow_count = 0;
	vegies[11].loc.x = 29;
	vegies[11].loc.y = 59;
	vegies[11].loc.z = 4;

	vegies[12].chunk_no = 85;
	vegies[12].picked = false;
	vegies[12].grow_count = 0;
	vegies[12].loc.x = 29;
	vegies[12].loc.y = 60;
	vegies[12].loc.z = 4;

	vegies[13].chunk_no = 85;
	vegies[13].picked = false;
	vegies[13].grow_count = 0;
	vegies[13].loc.x = 29;
	vegies[13].loc.y = 61;
	vegies[13].loc.z = 4;

	vegies[14].chunk_no = 87;
	vegies[14].picked = false;
	vegies[14].grow_count = 0;
	vegies[14].loc.x = 31;
	vegies[14].loc.y = 58;
	vegies[14].loc.z = 4;

	vegies[15].chunk_no = 87;
	vegies[15].picked = false;
	vegies[15].grow_count = 0;
	vegies[15].loc.x = 31;
	vegies[15].loc.y = 59;
	vegies[15].loc.z = 4;

	max_leavers = 5;
	leavers = new LEAVER[max_leavers];
	memset(leavers, 0, sizeof(LEAVER) * max_leavers);

	leavers[0].chunk_no[0] = 31;
	leavers[0].chunk_no[1] = 11;
	leavers[0].is_open = false;
	leavers[0].loc.x = 27;
	leavers[0].loc.y = 23;
	leavers[0].loc.z = 4;
	leavers[0].mod_chunk_no[0] = 34;
	leavers[0].mod_loc[0].x = 30;
	leavers[0].mod_loc[0].y = 26;
	leavers[0].mod_loc[0].z = 4;
	leavers[0].mod_chunk_no[1] = 34;
	leavers[0].mod_loc[1].x = 31;
	leavers[0].mod_loc[1].y = 26;
	leavers[0].mod_loc[1].z = 4;
	leavers[0].mod_chunk_no[2] = 34;
	leavers[0].mod_loc[2].x = 32;
	leavers[0].mod_loc[2].y = 26;
	leavers[0].mod_loc[2].z = 4;
	leavers[0].mod_chunk_no[3] = 35;
	leavers[0].mod_loc[3].x = 30;
	leavers[0].mod_loc[3].y = 27;
	leavers[0].mod_loc[3].z = 4;
	leavers[0].mod_chunk_no[4] = 35;
	leavers[0].mod_loc[4].x = 31;
	leavers[0].mod_loc[4].y = 27;
	leavers[0].mod_loc[4].z = 4;
	leavers[0].mod_chunk_no[5] = 35;
	leavers[0].mod_loc[5].x = 32;
	leavers[0].mod_loc[5].y = 27;
	leavers[0].mod_loc[5].z = 4;
	leavers[0].mod_chunk_no[6] = 35;
	leavers[0].mod_loc[6].x = 30;
	leavers[0].mod_loc[6].y = 28;
	leavers[0].mod_loc[6].z = 4;
	leavers[0].mod_chunk_no[7] = 35;
	leavers[0].mod_loc[7].x = 31;
	leavers[0].mod_loc[7].y = 28;
	leavers[0].mod_loc[7].z = 4;
	leavers[0].mod_chunk_no[8] = 35;
	leavers[0].mod_loc[8].x = 32;
	leavers[0].mod_loc[8].y = 28;
	leavers[0].mod_loc[8].z = 4;
	leavers[0].mod_chunk_no[9] = 35;
	leavers[0].mod_loc[9].x = 30;
	leavers[0].mod_loc[9].y = 29;
	leavers[0].mod_loc[9].z = 4;
	leavers[0].mod_chunk_no[10] = 35;
	leavers[0].mod_loc[10].x = 31;
	leavers[0].mod_loc[10].y = 29;
	leavers[0].mod_loc[10].z = 4;
	leavers[0].mod_chunk_no[11] = 35;
	leavers[0].mod_loc[11].x = 32;
	leavers[0].mod_loc[11].y = 29;
	leavers[0].mod_loc[11].z = 4;
	leavers[0].mod_chunk_no[12] = 36;
	leavers[0].mod_loc[12].x = 30;
	leavers[0].mod_loc[12].y = 30;
	leavers[0].mod_loc[12].z = 4;
	leavers[0].mod_chunk_no[13] = 36;
	leavers[0].mod_loc[13].x = 31;
	leavers[0].mod_loc[13].y = 30;
	leavers[0].mod_loc[13].z = 4;
	leavers[0].mod_chunk_no[14] = 36;
	leavers[0].mod_loc[14].x = 32;
	leavers[0].mod_loc[14].y = 30;
	leavers[0].mod_loc[14].z = 4;

	leavers[1].chunk_no[0] = 31;
	leavers[1].chunk_no[1] = 11;
	leavers[1].is_open = false;
	leavers[1].loc.x = 59;
	leavers[1].loc.y = 12;
	leavers[1].loc.z = 3;
	leavers[1].mod_chunk_no[0] = 20;
	leavers[1].mod_loc[0].x = 57;
	leavers[1].mod_loc[0].y = 7;
	leavers[1].mod_loc[0].z = 3;

	leavers[2].chunk_no[0] = 31;
	leavers[2].chunk_no[1] = 11;
	leavers[2].is_open = false;
	leavers[2].loc.x = 21;
	leavers[2].loc.y = 11;
	leavers[2].loc.z = 3;
	leavers[2].mod_chunk_no[0] = 214;
	leavers[2].mod_loc[0].x = 26;
	leavers[2].mod_loc[0].y = 23;
	leavers[2].mod_loc[0].z = 3;
	leavers[2].mod_chunk_no[1] = 214;
	leavers[2].mod_loc[1].x = 20;
	leavers[2].mod_loc[1].y = 23;
	leavers[2].mod_loc[1].z = 3;

	leavers[3].chunk_no[0] = 31;
	leavers[3].chunk_no[1] = 11;
	leavers[3].is_open = false;
	leavers[3].loc.x = 117;
	leavers[3].loc.y = 23;
	leavers[3].loc.z = 3;
	leavers[3].mod_chunk_no[0] = 214;
	leavers[3].mod_loc[0].x = 8;
	leavers[3].mod_loc[0].y = 23;
	leavers[3].mod_loc[0].z = 2;
	leavers[3].mod_chunk_no[1] = 214;
	leavers[3].mod_loc[1].x = 123;
	leavers[3].mod_loc[1].y = 23;
	leavers[3].mod_loc[1].z = 2;

	leavers[4].chunk_no[0] = 31;
	leavers[4].chunk_no[1] = 11;
	leavers[4].is_open = false;
	leavers[4].loc.x = 42;
	leavers[4].loc.y = 0;
	leavers[4].loc.z = 3;
	leavers[4].mod_chunk_no[0] = 34;
	leavers[4].mod_loc[0].x = 23;
	leavers[4].mod_loc[0].y = 17;
	leavers[4].mod_loc[0].z = 3;
	leavers[4].mod_chunk_no[1] = 35;
	leavers[4].mod_loc[1].x = 23;
	leavers[4].mod_loc[1].y = 18;
	leavers[4].mod_loc[1].z = 3;
	leavers[4].mod_chunk_no[2] = 35;
	leavers[4].mod_loc[2].x = 23;
	leavers[4].mod_loc[2].y = 19;
	leavers[4].mod_loc[2].z = 3;
	leavers[4].mod_chunk_no[3] = 35;
	leavers[4].mod_loc[3].x = 23;
	leavers[4].mod_loc[3].y = 20;
	leavers[4].mod_loc[3].z = 3;
	leavers[4].mod_chunk_no[4] = 35;
	leavers[4].mod_loc[4].x = 23;
	leavers[4].mod_loc[4].y = 21;
	leavers[4].mod_loc[4].z = 3;
	leavers[4].mod_chunk_no[5] = 35;
	leavers[4].mod_loc[5].x = 23;
	leavers[4].mod_loc[5].y = 22;
	leavers[4].mod_loc[5].z = 3;
	leavers[4].mod_chunk_no[6] = 35;
	leavers[4].mod_loc[6].x = 23;
	leavers[4].mod_loc[6].y = 23;
	leavers[4].mod_loc[6].z = 3;
	leavers[4].mod_chunk_no[7] = 35;
	leavers[4].mod_loc[7].x = 23;
	leavers[4].mod_loc[7].y = 24;
	leavers[4].mod_loc[7].z = 3;
	leavers[4].mod_chunk_no[8] = 35;
	leavers[4].mod_loc[8].x = 23;
	leavers[4].mod_loc[8].y = 25;
	leavers[4].mod_loc[8].z = 3;
	leavers[4].mod_chunk_no[9] = 35;
	leavers[4].mod_loc[9].x = 23;
	leavers[4].mod_loc[9].y = 26;
	leavers[4].mod_loc[9].z = 3;
	leavers[4].mod_chunk_no[10] = 35;
	leavers[4].mod_loc[10].x = 23;
	leavers[4].mod_loc[10].y = 27;
	leavers[4].mod_loc[10].z = 3;
	leavers[4].mod_chunk_no[11] = 35;
	leavers[4].mod_loc[11].x = 23;
	leavers[4].mod_loc[11].y = 28;
	leavers[4].mod_loc[11].z = 3;
	leavers[4].mod_chunk_no[12] = 35;
	leavers[4].mod_loc[12].x = 23;
	leavers[4].mod_loc[12].y = 29;
	leavers[4].mod_loc[12].z = 3;
	leavers[4].mod_chunk_no[13] = 35;
	leavers[4].mod_loc[13].x = 23;
	leavers[4].mod_loc[13].y = 30;
	leavers[4].mod_loc[13].z = 3;
	leavers[4].mod_chunk_no[14] = 36;
	leavers[4].mod_loc[14].x = 23;
	leavers[4].mod_loc[14].y = 31;
	leavers[4].mod_loc[14].z = 3;

	max_chests = 8;
	chests = new CHEST[max_chests];
	memset(chests, 0, sizeof(CHEST) * max_chests);

	chests[0].chunk_no[0] = 9;
	chests[0].chunk_no[1] = 10;
	chests[0].is_open = false;
	chests[0].gold = 10;
	chests[0].loc.x = 57;
	chests[0].loc.y = 6;
	chests[0].loc.z = 3;

	chests[1].chunk_no[0] = 9;
	chests[1].chunk_no[1] = 10;
	chests[1].is_open = false;
	chests[1].gold = 5;
	chests[1].loc.x = 29;
	chests[1].loc.y = 58;
	chests[1].loc.z = 3;

	chests[2].chunk_no[0] = 9;
	chests[2].chunk_no[1] = 10;
	chests[2].is_open = false;
	chests[2].gold = 10;
	chests[2].loc.x = 51;
	chests[2].loc.y = 21;
	chests[2].loc.z = 5;

	chests[3].chunk_no[0] = 9;
	chests[3].chunk_no[1] = 10;
	chests[3].is_open = false;
	chests[3].gold = 15;
	chests[3].loc.x = 6;
	chests[3].loc.y = 27;
	chests[3].loc.z = 4;

	chests[4].chunk_no[0] = 9;
	chests[4].chunk_no[1] = 10;
	chests[4].is_open = false;
	chests[4].gold = 5;
	chests[4].loc.x = 37;
	chests[4].loc.y = 17;
	chests[4].loc.z = 5;

	chests[5].chunk_no[0] = 9;
	chests[5].chunk_no[1] = 10;
	chests[5].is_open = false;
	chests[5].gold = 20;
	chests[5].loc.x = 36;
	chests[5].loc.y = 17;
	chests[5].loc.z = 5;

	chests[6].chunk_no[0] = 9;
	chests[6].chunk_no[1] = 10;
	chests[6].is_open = false;
	chests[6].gold = 6;
	chests[6].loc.x = 35;
	chests[6].loc.y = 17;
	chests[6].loc.z = 5;

	chests[7].chunk_no[0] = 9;
	chests[7].chunk_no[1] = 10;
	chests[7].is_open = false;
	chests[7].gold = 8;
	chests[7].loc.x = 34;
	chests[7].loc.y = 17;
	chests[7].loc.z = 5;

	max_teleporters = 8;
	teleporters = new TELEPORTER[max_teleporters];
	memset(teleporters, 0, sizeof(TELEPORTER) * max_teleporters);

	teleporters[0].go_to = 1;
	teleporters[0].loc.x = 26;
	teleporters[0].loc.y = 23;
	teleporters[0].loc.z = 3;

	teleporters[1].go_to = 0;
	teleporters[1].loc.x = 20;
	teleporters[1].loc.y = 23;
	teleporters[1].loc.z = 3;

	teleporters[2].go_to = 3;
	teleporters[2].loc.x = 8;
	teleporters[2].loc.y = 23;
	teleporters[2].loc.z = 2;

	teleporters[3].go_to = 2;
	teleporters[3].loc.x = 123;
	teleporters[3].loc.y = 23;
	teleporters[3].loc.z = 2;

	teleporters[4].go_to = 5;
	teleporters[4].loc.x = 31;
	teleporters[4].loc.y = 15;
	teleporters[4].loc.z = 7;

	teleporters[5].go_to = 4;
	teleporters[5].loc.x = 0;
	teleporters[5].loc.y = 89;
	teleporters[5].loc.z = 4;

	teleporters[6].go_to = 7;
	teleporters[6].loc.x = 120;
	teleporters[6].loc.y = 98;
	teleporters[6].loc.z = 4;

	teleporters[7].go_to = 6;
	teleporters[7].loc.x = 22;
	teleporters[7].loc.y = 57;
	teleporters[7].loc.z = 4;

	for(i = 0; i < MAX_QUESTS; i++)
		quests.done[i] = false;
}

bool PLAYER::does_saved_game_exist(void)
{
	return file_lib.does_file_exist("player.sav");
}

long *PLAYER::get_viewable(long x, long y, long z, long width, long height, long world_width, long world_height)
{
	long cur;
	long cur2;
	long *viewable;
	long *tplane;
	long tw, th;

	viewable = new long[width * height];
	memset(viewable, 0, sizeof(long) * width * height);

	tplane = new long[world_width * world_height];
	memset(tplane, 0, sizeof(long) * world_width * world_height);

	for(cur = 0; cur < max_doors; cur++)
		if(z == doors[cur].loc.z && !doors[cur].is_open)
			tplane[(doors[cur].loc.y * world_width) + doors[cur].loc.x] = doors[cur].chunk_no;

	for(cur = 0; cur < max_vegies; cur++)
		if(z == vegies[cur].loc.z && !vegies[cur].picked)
			tplane[(vegies[cur].loc.y * world_width) + vegies[cur].loc.x] = vegies[cur].chunk_no;

	for(cur = 0; cur < max_items; cur++)
		if(z == items[cur].loc.z && !items[cur].in_inventory && items[cur].is_visible)
			tplane[(items[cur].loc.y * world_width) + items[cur].loc.x] = items[cur].chunk_no;

	for(cur = 0; cur < max_energy_fields; cur++)
		if(z == energy_fields[cur].loc.z && !energy_fields[cur].is_dispelled)
			tplane[(energy_fields[cur].loc.y * world_width) + energy_fields[cur].loc.x] = energy_fields[cur].chunk_no;

	for(cur = 0; cur < max_chests; cur++)
		if(z == chests[cur].loc.z)
			tplane[(chests[cur].loc.y * world_width) + chests[cur].loc.x] = chests[cur].chunk_no[chests[cur].is_open];

	for(cur = 0; cur < max_leavers; cur++)
	{
		if(z == leavers[cur].loc.z)
			tplane[(leavers[cur].loc.y * world_width) + leavers[cur].loc.x] = leavers[cur].chunk_no[leavers[cur].is_open];

		if(leavers[cur].is_open)
		{
			for(cur2 = 0; cur2 < 32; cur2++)
			{
				if(!leavers[cur].mod_chunk_no[cur2])
					break;

				if(z == leavers[cur].mod_loc[cur2].z)
					tplane[(leavers[cur].mod_loc[cur2].y * world_width) + leavers[cur].mod_loc[cur2].x] = leavers[cur].mod_chunk_no[cur2];
			}
		}
	}

	if(!quests.done[3] && z == 4)
		tplane[(9 * world_width) + 23] = 144;

	for(th = 0; th < height; th++)
		for(tw = 0; tw < width; tw++)
			viewable[(th * width) + tw] = tplane[(FIX_WORLD_COORD(y + th) * world_width) + FIX_WORLD_COORD(x + tw)];

	delete tplane;
	return viewable;
}

long PLAYER::get_chunk(long x, long y, long z, long world_width, long world_height)
{
	long cur;
	long cur2;
	long *tplane;
	long chunk;

	tplane = new long[world_width * world_height];
	memset(tplane, 0, sizeof(long) * world_width * world_height);

	for(cur = 0; cur < max_doors; cur++)
		if(z == doors[cur].loc.z && !doors[cur].is_open)
			tplane[(doors[cur].loc.y * world_width) + doors[cur].loc.x] = doors[cur].chunk_no;

	for(cur = 0; cur < max_vegies; cur++)
		if(z == vegies[cur].loc.z && !vegies[cur].picked)
			tplane[(vegies[cur].loc.y * world_width) + vegies[cur].loc.x] = vegies[cur].chunk_no;

	for(cur = 0; cur < max_items; cur++)
		if(z == items[cur].loc.z && !items[cur].in_inventory && items[cur].is_visible)
			tplane[(items[cur].loc.y * world_width) + items[cur].loc.x] = items[cur].chunk_no;

	for(cur = 0; cur < max_energy_fields; cur++)
		if(z == energy_fields[cur].loc.z && !energy_fields[cur].is_dispelled)
			tplane[(energy_fields[cur].loc.y * world_width) + energy_fields[cur].loc.x] = energy_fields[cur].chunk_no;

	for(cur = 0; cur < max_chests; cur++)
		if(z == chests[cur].loc.z)
			tplane[(chests[cur].loc.y * world_width) + chests[cur].loc.x] = chests[cur].chunk_no[chests[cur].is_open];

	for(cur = 0; cur < max_leavers; cur++)
	{
		if(z == leavers[cur].loc.z)
			tplane[(leavers[cur].loc.y * world_width) + leavers[cur].loc.x] = leavers[cur].chunk_no[leavers[cur].is_open];

		if(leavers[cur].is_open)
		{
			for(cur2 = 0; cur2 < 32; cur2++)
			{
				if(!leavers[cur].mod_chunk_no[cur2])
					break;

				if(z == leavers[cur].mod_loc[cur2].z)
					tplane[(leavers[cur].mod_loc[cur2].y * world_width) + leavers[cur].mod_loc[cur2].x] = leavers[cur].mod_chunk_no[cur2];
			}
		}
	}

	chunk = tplane[(y * world_width) + x];

	delete tplane;
	return chunk;
}

long *PLAYER::get_viewable_npcs(long x, long y, long z, long width, long height, long world_width, long world_height)
{
	long cur;
	long *viewable;
	long *tplane;
	long tw, th;

	viewable = new long[width * height];
	memset(viewable, 0, sizeof(long) * width * height);

	tplane = new long[world_width * world_height];
	memset(tplane, 0, sizeof(long) * world_width * world_height);

	for(cur = 0; cur < max_npcs; cur++)
		if(z == populace[cur].loc.z)
		{
			if(populace[cur].job == 'A')
			{
				if(populace[cur].job_mod)
					tplane[(populace[cur].loc.y * world_width) + populace[cur].loc.x] = populace[cur].chunk_no;
			}
			else
			{
				tplane[(populace[cur].loc.y * world_width) + populace[cur].loc.x] = populace[cur].chunk_no;
			}
		}

	for(th = 0; th < height; th++)
		for(tw = 0; tw < width; tw++)
			viewable[(th * width) + tw] = tplane[(FIX_WORLD_COORD(y + th) * world_width) + FIX_WORLD_COORD(x + tw)];

	delete tplane;
	return viewable;
}

void PLAYER::update_dynamic(bool (*walkable_func)(PLAYER *, long, long, long, SCREEN *, WORLD *, TILES *), SCREEN *screen, WORLD *world, TILES *tiles)
{
	bool people_around;
	long tx, ty;
	long cur;
	long cur2;
	long cur3;

	for(cur = 0; cur < max_doors; cur++)
	{
		if(doors[cur].is_open)
		{
			people_around = false;
			tx = abs(doors[cur].loc.x - loc.x);
			ty = abs(doors[cur].loc.y - loc.y);
			if(!(tx > 1 || ty > 1) && doors[cur].loc.z == loc.z)
				people_around = true;

			if(!people_around)
			{
				for(cur2 = 0; cur2 < max_npcs; cur2++)
				{
					tx = abs(doors[cur].loc.x - populace[cur2].loc.x);
					ty = abs(doors[cur].loc.y - populace[cur2].loc.y);
					if(!(tx > 1 || ty > 1) && doors[cur].loc.z == populace[cur2].loc.z)
					{
						people_around = true;
						break;
					}
				}
			}

			if(!people_around)
				doors[cur].is_open = false;
		}
	}


	for(cur = 0; cur < max_vegies; cur++)
	{
		if(vegies[cur].picked)
		{
			vegies[cur].grow_count++;
			if(vegies[cur].grow_count > 128)
			{
				vegies[cur].grow_count = 0;
				vegies[cur].picked = false;                                                                                       
			}
		}
	}

	for(cur = 0; cur < max_npcs; cur++)
	{
		tx = 0;
		ty = 0;
		switch(populace[cur].job)
		{
			case 'T':
				cur2 = rand() % 100;   		
   				if(abs(populace[cur].loc.x - loc.x) <= 1 && abs(populace[cur].loc.y - loc.y) <= 1)
   					if(cur2 > 74 && gold > 0)
   						gold--;
			case 'A':
			case 'R':
				cur2 = rand() % 8;
				switch(cur2)
				{
					case 7:
					case 0:
						ty = -1;
						break;
					case 1:
					case 2:
						tx = 1;
						break;
					case 3:
					case 4:
						ty = 1;
						break;
					case 5:
					case 6:
						tx = -1;
						break;
				}
				break;
			case 'V':
				if(populace[cur].loc.y + populace[cur].job_mod != LIMIT(populace[cur].loc.y + populace[cur].job_mod, populace[cur].job_loc1.y, populace[cur].job_loc2.y))
					populace[cur].job_mod = LIMIT(populace[cur].loc.y + populace[cur].job_mod, populace[cur].job_loc1.y, populace[cur].job_loc2.y) - populace[cur].loc.y + populace[cur].job_mod;
				ty = populace[cur].job_mod;
				break;
			case 'H':
				if(populace[cur].loc.x + populace[cur].job_mod != LIMIT(populace[cur].loc.x + populace[cur].job_mod, populace[cur].job_loc1.x, populace[cur].job_loc2.x))
					populace[cur].job_mod = LIMIT(populace[cur].loc.x + populace[cur].job_mod, populace[cur].job_loc1.x, populace[cur].job_loc2.x) - (populace[cur].loc.x + populace[cur].job_mod);
				tx = populace[cur].job_mod;
				break;
		}
		if(populace[cur].job == 'A')
		{
			cur2 = get_chunk(LIMIT(populace[cur].loc.x + tx, populace[cur].job_loc1.x, populace[cur].job_loc2.x), LIMIT(populace[cur].loc.y + ty, populace[cur].job_loc1.y, populace[cur].job_loc2.y), populace[cur].loc.z, world->get_width(), world->get_height());
			if(!cur2)
				cur2 = world->get_chunk(LIMIT(populace[cur].loc.x + tx, populace[cur].job_loc1.x, populace[cur].job_loc2.x), LIMIT(populace[cur].loc.y + ty, populace[cur].job_loc1.y, populace[cur].job_loc2.y), populace[cur].loc.z);
			
			if(cur2 == 62)
			{
				populace[cur].loc.x = LIMIT(populace[cur].loc.x + tx, populace[cur].job_loc1.x, populace[cur].job_loc2.x);
				populace[cur].loc.y = LIMIT(populace[cur].loc.y + ty, populace[cur].job_loc1.y, populace[cur].job_loc2.y);
				if(populace[cur].job == 'A')
				{
					cur3 = rand() % 100;   		
					if(cur3 > 93)
						populace[cur].job_mod = true;
					else
						populace[cur].job_mod = false;

				}
			}
			else if(cur2 == 35)
			{
				populace[cur].loc.x = LIMIT(populace[cur].loc.x + tx, populace[cur].job_loc1.x, populace[cur].job_loc2.x);
				populace[cur].loc.y = LIMIT(populace[cur].loc.y + ty, populace[cur].job_loc1.y, populace[cur].job_loc2.y);
				populace[cur].job_mod = false;
			}
		}
		else
		{
			if(walkable_func(this, LIMIT(populace[cur].loc.x + tx, populace[cur].job_loc1.x, populace[cur].job_loc2.x), LIMIT(populace[cur].loc.y + ty, populace[cur].job_loc1.y, populace[cur].job_loc2.y), populace[cur].loc.z, screen, world, tiles))
			{
				populace[cur].loc.x = LIMIT(populace[cur].loc.x + tx, populace[cur].job_loc1.x, populace[cur].job_loc2.x);
				populace[cur].loc.y = LIMIT(populace[cur].loc.y + ty, populace[cur].job_loc1.y, populace[cur].job_loc2.y);
			}
		}
	}
}

long PLAYER::manipulate_dynamic(long x, long y, long z, bool with_spell)
{
	long cur;

	for(cur = 0; cur < max_energy_fields; cur++)
		if(energy_fields[cur].loc.z == z && energy_fields[cur].loc.x == x && energy_fields[cur].loc.y == y && !energy_fields[cur].is_dispelled)
		{
   			if(abs(x - loc.x) <= 1 && abs(y - loc.y) <= 1)
			{
				if(with_spell)
				{
					energy_fields[cur].is_dispelled = true;
					return 3;
				}
				else
				{
					msg_no = 2;
					return 7;
				}
			}
			else
			{
				msg_no = 1;
				return 7;
			}
		}

	for(cur = 0; cur < max_doors; cur++)
		if(doors[cur].loc.z == z && doors[cur].loc.x == x && doors[cur].loc.y == y && !doors[cur].is_open)
		{
   			if(abs(x - loc.x) <= 1 && abs(y - loc.y) <= 1)
			{
				if(doors[cur].need_key && !items[2].in_inventory)
				{
					msg_no = 4;
					return 8;
				}
				else
				{
					doors[cur].is_open = true;
					return 4;
				}
			}
			else
			{
				msg_no = 1;
				return 7;
			}
		}

	for(cur = 0; cur < max_vegies; cur++)
		if(vegies[cur].loc.z == z && vegies[cur].loc.x == x && vegies[cur].loc.y == y && !vegies[cur].picked)
		{
   			if(abs(x - loc.x) <= 1 && abs(y - loc.y) <= 1)
			{
				vegies[cur].picked = true;
				food++;
				return 1;
			}
			else
			{
				msg_no = 1;
				return 7;
			}
		}

	for(cur = 0; cur < max_items; cur++)
		if(items[cur].loc.z == z && items[cur].loc.x == x && items[cur].loc.y == y && !items[cur].in_inventory && items[cur].is_visible)
		{
   			if(abs(x - loc.x) <= 1 && abs(y - loc.y) <= 1)
			{
				items[cur].in_inventory = true;
				return 5;
			}
			else
			{
				msg_no = 1;
				return 7;
			}
		}

	for(cur = 0; cur < max_leavers; cur++)
		if(leavers[cur].loc.z == z && leavers[cur].loc.x == x && leavers[cur].loc.y == y)
		{
   			if(abs(x - loc.x) <= 1 && abs(y - loc.y) <= 1)
			{
				leavers[cur].is_open = !leavers[cur].is_open;
				return 2;
			}
			else
			{
				msg_no = 1;
				return 7;
			}
		}

	for(cur = 0; cur < max_chests; cur++)
		if(chests[cur].loc.z == z && chests[cur].loc.x == x && chests[cur].loc.y == y)
		{
   			if(abs(x - loc.x) <= 1 && abs(y - loc.y) <= 1)
			{
				chests[cur].is_open = !chests[cur].is_open;

				if(chests[cur].is_open && chests[cur].gold)
				{
					gold += chests[cur].gold;
					msg_no = 90000 + chests[cur].gold;
					chests[cur].gold = 0;
					return 6;
				}
				return 4;
			}
			else
			{
				msg_no = 1;
				return 7;
			}
		}

	for(cur = 0; cur < max_npcs; cur++)
		if(populace[cur].loc.z == z && populace[cur].loc.x == x && populace[cur].loc.y == y)
		{
   			if(abs(x - loc.x) <= 3 && abs(y - loc.y) <= 3 && populace[cur].chunk_no != 176)
			{
				talking_to = cur;
			}
			else
			{
				msg_no = 1;
				return 7;
			}
		}

	return 0;
}

void PLAYER::teleport(void)
{
	long cur;

	for(cur = 0; cur < max_teleporters; cur++)
		if(teleporters[cur].loc.z == loc.z && teleporters[cur].loc.x == loc.x && teleporters[cur].loc.y == loc.y)
		{
			loc.x = teleporters[teleporters[cur].go_to].loc.x;
			loc.y = teleporters[teleporters[cur].go_to].loc.y;
			loc.z = teleporters[teleporters[cur].go_to].loc.z;
			break;
		}
}

bool PLAYER::is_dead(void)
{
	return (energy == 0);
}

bool PLAYER::has_won(void)
{
	return quests.done[1];
}