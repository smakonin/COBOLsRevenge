/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#ifndef TALK_DB_H
#define TALK_DB_H

#include "globals.h"

class TALK_SLICE
{
	public:
		char phrase[256];
		WORD link;
		char type;
		BYTE action;
		WORD npc;
		WORD filler;
		WORD x;
		WORD y;
		WORD z;
		BYTE quest;
		bool done;
		short gold;
		short item;
		bool in_inv;
		bool used;
};

class TALK_DB 
{
	private:
		FILE *fp;
		TALK_SLICE slice;

	public:
		~TALK_DB();
		TALK_DB();
		char *find_response(long npc_no, long *seq_no, bool *end_talk, PLAYER *player);
		char *find_response(long x, long y, long z);
		void find_questions(long npc_no, long seq_no, long *question_bag, long max_questions, long remove_no, PLAYER *player);
		void remove_question(long *question_bag, long max_questions, long remove_no);
		char *find_question(long seq_no);
		long get_reaction(long seq_no);
};

#endif //TALK_DB_H
