/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#ifndef VIEW_H
#define VIEW_H

#include "globals.h"

class VIEW 
{
	private:
		long height;
		long width;
		long pos_x;
		long pos_y;
		long corner_x;
		long corner_y;
		long tiles_in_vew;
		long *in_view_static;
		long *in_view_dynamic;
		long *in_view_npc;
		double slope_ul2lr;
		double slope_ur2ll;
		SCREEN *screen;
		WORLD *world;
		TILES *tiles;
		LOCATION last_loc;
		long last_anim_index;

	public:
		VIEW(long view_width, long view_height, long pos_x, long pos_y, SCREEN *screen, WORLD *world, TILES *tiles);
		~VIEW();
		long generate_view(PLAYER *player, long anim_index, bool do_reactions);
		void generate_view(long x, long y, long z, long anim_index);
		bool __stdcall generate_view(PLAYER *player, long x, long y, long z);
		void generate_view(PLAYER *player = NULL);
		long get_quadrant(long x, long y);
		void generate_all_chunk_view(long anim_index);
		BYTE get_chunk_selected(long x, long y);
		char *get_chunk_desc(long x, long y, PLAYER *player);
		long get_top_chunk(long x, long y);
		long use_chunk(PLAYER *player, long x, long y, bool with_spell);
		long get_height(void);
		long get_width(void);
		void fix_xy(long *x, long *y);
};

#endif //VIEW_H

