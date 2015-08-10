/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#ifndef WORLD_H
#define WORLD_H

#include "globals.h"

#define FIX_WORLD_COORD(m)      	        ((m)&0x7F)

class WORLD 
{
	private:
		char name[64];
		long width;
		long height;
		long levels;
		long tile_size;
		long level_size;
		BYTE *map_data;
		FILE_LIBRARY file_lib;

	public:
		WORLD();
		WORLD(char *filename);
		~WORLD();
		void load(char *filename);
		void save(char *filename);
		long *get_viewable(long x, long y, long z, long width, long height, bool use_transparancy = false);
		void put_chunk(long x, long y, long z, BYTE chunk);
		BYTE get_chunk(long x, long y, long z);
		long get_height(void);
		long get_width(void);
};

#endif //WORLD_H





