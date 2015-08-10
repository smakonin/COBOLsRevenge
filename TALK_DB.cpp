/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include "globals.h"

TALK_DB::TALK_DB()
{
	fp = fopen("static/tlkdb.cr", "rb");
}

TALK_DB::~TALK_DB()
{
	fclose(fp);
}

char *TALK_DB::find_response(long npc_no, long *seq_no, bool *end_talk, PLAYER *player)
{
	char *phrase = NULL;
	long count = 0;

	fseek(fp, 0, SEEK_SET);

	while(!feof(fp))
	{
		fread(&slice, sizeof(TALK_SLICE), 1, fp);
		if(slice.npc == npc_no && slice.link == *seq_no && slice.type == 'R')
		{
			if(slice.quest == 255 || player->quests.done[slice.quest] == slice.done)
			{
				if(slice.gold <= player->gold)
				{
					if(slice.item == 255 || (player->items[slice.item].in_inventory == slice.in_inv && player->items[slice.item].is_used == slice.used))
					{
						phrase = slice.phrase;
						break;
					}
				}
			}
		}
		count++;
	}
	
	*end_talk = (slice.action == 1) ? true : false;
	*seq_no = count;
	return phrase;
}

char *TALK_DB::find_response(long x, long y, long z)
{
	char *phrase = NULL;
	long count = 0;

	fseek(fp, 0, SEEK_SET);

	while(!feof(fp))
	{
		fread(&slice, sizeof(TALK_SLICE), 1, fp);
		if(slice.x == x && slice.y == y && slice.z == z)
		{
			phrase = slice.phrase;
			break;
		}
		count++;
	}
	
	return phrase;
}

void TALK_DB::find_questions(long npc_no, long seq_no, long *question_bag, long max_questions, long remove_no, PLAYER *player)
{
	long count = 0;
	long i;

	fseek(fp, 0, SEEK_SET);

	while(!feof(fp))
	{
		fread(&slice, sizeof(TALK_SLICE), 1, fp);
		if(slice.npc == npc_no && slice.link == seq_no && slice.type == 'Q')
		{
			if(slice.quest == 255 || player->quests.done[slice.quest] == slice.done)
			{
				if(slice.gold <= player->gold)
				{
					if(slice.item == 255 || (player->items[slice.item].in_inventory == slice.in_inv && player->items[slice.item].is_used == slice.used))
					{
						for(i = 0; i < max_questions; i++)
						{
							if(!question_bag[i])
							{
								question_bag[i] = count;
								break;
							}
						}
					}
				}
			}
		}
		count++;
	}

	if(remove_no == -1)
		remove_question(question_bag, max_questions, remove_no);
	else
		remove_question(question_bag, max_questions, question_bag[remove_no]);
}

void TALK_DB::remove_question(long *question_bag, long max_questions, long remove_no)
{
	long i, j;

	for(i = 0; i < max_questions; i++)
	{
		if(!question_bag[i] || question_bag[i] == remove_no)
		{
			if(question_bag[i] == remove_no)
				question_bag[i] = 0;

			for(j = i + 1; j < max_questions; j++)
			{
				if(question_bag[j])
				{
					question_bag[i] = question_bag[j];
					question_bag[j] = 0;
					break;
				}
			}
		}
	}
}

char *TALK_DB::find_question(long seq_no)
{
	long count = 0;

	fseek(fp, 0, SEEK_SET);

	while(count != seq_no + 1 && !feof(fp))
	{
		fread(&slice, sizeof(TALK_SLICE), 1, fp);
		count++;
	}

	return slice.phrase;
}

long TALK_DB::get_reaction(long seq_no)
{
	long count = 0;

	fseek(fp, 0, SEEK_SET);

	while(count != seq_no + 1 && !feof(fp))
	{
		fread(&slice, sizeof(TALK_SLICE), 1, fp);
		count++;
	}

	return slice.action;
}
