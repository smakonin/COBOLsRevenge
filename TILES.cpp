/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include "globals.h"

TILES::TILES(char *index_filename, char *data_filename, SCREEN *screen)
{
	long cur_index;
	long size;
	FILE *fp = NULL;
	
	TILES::screen = screen;
	chunks = NULL;
	tiles = NULL;

	size = file_lib.file_size(index_filename);
	if(size == -1)
		throw ERRORS(ERR_MISSING_FILE, index_filename);

	TILES::count = size / sizeof(CHUNK);
	chunks = new CHUNK[TILES::count];
	if(!chunks)
		throw ERRORS(ERR_ALLOC_MEM, "Tile Index");

	fp = fopen(index_filename, "rb");
	if(!fp)
		throw ERRORS(ERR_MISSING_FILE, index_filename);

	fread(chunks, size, 1, fp);
	fclose(fp);
	fp = NULL;

	load(data_filename);
	
	tile_size = width * height;

	tiles = new IMAGE *[IMAGE_LIBRARY::count];
	for(cur_index = 0; cur_index < IMAGE_LIBRARY::count; cur_index++)
	{
		tiles[cur_index] = new IMAGE(width, height, screen->get_ddobj7());
		tiles[cur_index]->put_pixels(images + (cur_index * tile_size));
		tiles[cur_index]->set_key_colour(trans_colour);
	}
}

TILES::~TILES()
{
	long cur_index;

	delete chunks;	

	for(cur_index = 0; cur_index < IMAGE_LIBRARY::count; cur_index++)
		delete tiles[cur_index];

	delete tiles;
}

IMAGE *TILES::get_tile(long chunk_number, long anim_index, long call_no)
{
	long tile_no;

	if(call_no == 0 && chunks[chunks[chunk_number].under_chunk].under_chunk)
	{
		tile_no = chunks[chunks[chunks[chunk_number].under_chunk].under_chunk].sprite_index[anim_index % SPRITE_FRAMES];
		return tiles[tile_no];
	}
	if(call_no == 1 && chunks[chunk_number].under_chunk)
	{
		tile_no = chunks[chunks[chunk_number].under_chunk].sprite_index[anim_index % SPRITE_FRAMES];
		return tiles[tile_no];
	}
	if(call_no == 2)
	{
		tile_no = chunks[chunk_number].sprite_index[anim_index % SPRITE_FRAMES];
		return tiles[tile_no];
	}

	return NULL;
}

long TILES::chunks_count(void)
{
	return TILES::count;
}

long TILES::tiles_count(void)
{
	return IMAGE_LIBRARY::count;
}

bool TILES::is_walkable(long chunk_number)
{
	return chunks[chunk_number].walkable;
}

bool TILES::is_wall(long chunk_number)
{
	return chunks[chunk_number].wall;
}

WORD TILES::reaction(long chunk_number)
{
	return chunks[chunk_number].reaction;
}

char *TILES::get_desc(long chunk_number)
{
	return chunks[chunk_number].desc;
}