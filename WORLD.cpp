/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include "globals.h"

WORLD::WORLD()
{
	memset(name, 0, sizeof(name));
	width = 0;
	height = 0;
	levels = 0;
	level_size = 0;
	tile_size = 0;
	map_data = NULL;
}

WORLD::WORLD(char *filename)
{
	WORLD();
	load(filename);
}

WORLD::~WORLD()
{
	delete map_data;
}

void WORLD::load(char *filename)
{
	FILE *fp = NULL;
	long comp_size;

	IMAGE_LIBRARY();

	fp = fopen(filename, "rb");
	if(!fp)
		throw ERRORS(ERR_MISSING_FILE, filename);

	fread(name, sizeof(name), 1, fp);
	fread(&width, sizeof(width), 1, fp);
	fread(&height, sizeof(height), 1, fp);
	fread(&levels, sizeof(levels), 1, fp);
	fread(&tile_size, sizeof(tile_size), 1, fp);
	fread(&comp_size, sizeof(comp_size), 1, fp);

	level_size = width * height * tile_size;

	map_data = new BYTE[comp_size];
	fread(map_data, comp_size, 1, fp);
	map_data = (BYTE *)file_lib.uncompress(map_data, &comp_size, level_size * levels);

	fclose(fp);
	fp = NULL;
}

void WORLD::save(char *filename)
{
	FILE *fp = NULL;
	BYTE *comp_data = NULL;
	BYTE *temp_data = NULL;
	long size;

	fp = fopen(filename, "wb");
	if(!fp)
		throw ERRORS(ERR_MISSING_FILE, filename);

	fwrite(name, sizeof(name), 1, fp);
	fwrite(&width, sizeof(width), 1, fp);
	fwrite(&height, sizeof(height), 1, fp);
	fwrite(&levels, sizeof(levels), 1, fp);
	fwrite(&tile_size, sizeof(tile_size), 1, fp);

	size = width * height * levels;
	temp_data = new BYTE[size];
	memcpy(temp_data, map_data, size);
	comp_data = (BYTE *)file_lib.compress(temp_data, &size);
	fwrite(&size, sizeof(size), 1, fp);
	fwrite(comp_data, size, 1, fp);

	fclose(fp);
	fp = NULL;
	//delete temp_data;
}

long *WORLD::get_viewable(long x, long y, long z, long width, long height, bool use_transparancy)
{
	long th, tw, tz;
	long *viewable;

	viewable = new long[width * height];

	for(th = 0; th < height; th++)
	{
		for(tw = 0; tw < width; tw++)
		{
			tz = z;
			if(use_transparancy)
			{
				while(map_data[(tz * level_size) + (FIX_WORLD_COORD(y + th) * WORLD::width) + FIX_WORLD_COORD(x + tw)] == 0 && tz > 4)
					tz--;
			}
			viewable[(th * width) + tw] = map_data[(tz * level_size) + (FIX_WORLD_COORD(y + th) * WORLD::width) + FIX_WORLD_COORD(x + tw)];
		}
	}	

	return viewable;
}

void WORLD::put_chunk(long x, long y, long z, BYTE chunk)
{
	map_data[(z * level_size) + (FIX_WORLD_COORD(y) * width) + FIX_WORLD_COORD(x)] = chunk;
}

BYTE WORLD::get_chunk(long x, long y, long z)
{
	return map_data[(z * level_size) + (FIX_WORLD_COORD(y) * width) + FIX_WORLD_COORD(x)];
}

long WORLD::get_height(void)
{
	return height;
}

long WORLD::get_width(void)
{
	return width;
}

