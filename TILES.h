/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#ifndef TILES_H
#define TILES_H

#include "globals.h"

#define SPRITE_FRAMES				16

class CHUNK 
{
	public:
		WORD sprite_index[SPRITE_FRAMES];
		WORD under_chunk;
		bool walkable;
		bool wall;
		WORD reaction;
		char desc[64];
};

class TILES : public IMAGE_LIBRARY 
{
	private:
		long tile_size;
		long count;
		CHUNK *chunks;
		IMAGE **tiles;
		SCREEN *screen;

	public:
		TILES(char *index_filename, char *data_filename, SCREEN *screen);
		~TILES();
		IMAGE *get_tile(long chunk_number, long anim_index, long call_no);
		long chunks_count(void);
		long tiles_count(void);
		bool is_walkable(long chunk_number);
		bool is_wall(long chunk_number);
		WORD reaction(long chunk_number);
		char *get_desc(long chunk_number);
};

#endif //TILES_H


