/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include "globals.h"


void flood_fill(long *data, long x, long y, long width, long height)
{
	if(x > -1 && y > -1 && x < width && y < height)
		data[(width * y) + x] = 2;
	
	if(x > 0 && y > 0 && x < width - 1 && y < height - 1)
	{
		if(data[(width * y) + (x + 1)] == 0)
			flood_fill(data, x + 1, y, width, height);

		if(data[(width * y) + (x - 1)] == 0)
			flood_fill(data, x - 1, y, width, height);

		if(data[(width * (y + 1)) + x] == 0)
			flood_fill(data, x, y + 1, width, height);

		if(data[(width * (y - 1)) + x] == 0)
			flood_fill(data, x, y - 1, width, height);
	}
}

VIEW::VIEW(long view_width, long view_height, long pos_x, long pos_y, SCREEN *screen, WORLD *world, TILES *tiles)
{	
	in_view_npc = NULL;
	in_view_dynamic = NULL;
	in_view_static = NULL;
	height = view_height;
	width = view_width;
	corner_x = width >> 1;
	corner_y = height >> 1;
	VIEW::pos_x = pos_x;
	VIEW::pos_y = pos_y;
	tiles_in_vew = width * height;

	slope_ul2lr = (double)(height) / (double)(width);
	slope_ur2ll = (double)(-1 * height * tiles->get_height()) / (double)(-1 * width * tiles->get_width());

	VIEW::screen = screen;
	VIEW::world = world;
	VIEW::tiles = tiles;
}

VIEW::~VIEW()
{
	if(in_view_npc) 
		delete in_view_npc;

	if(in_view_dynamic) 
		delete in_view_dynamic;

	if(in_view_static) 
		delete in_view_static;
}

long VIEW::generate_view(PLAYER *player, long anim_index, bool do_reactions)
{
	long tx, ty;
	long on_chunk;

	if(in_view_static) 
		delete in_view_static;

	if(in_view_dynamic) 
		delete in_view_dynamic;

	if(in_view_npc) 
		delete in_view_npc;

	last_anim_index = anim_index;

	in_view_static = world->get_viewable(FIX_WORLD_COORD(player->loc.x - corner_x), FIX_WORLD_COORD(player->loc.y - corner_y), player->loc.z, width, height, true);
	in_view_dynamic = player->get_viewable(FIX_WORLD_COORD(player->loc.x - corner_x), FIX_WORLD_COORD(player->loc.y - corner_y), player->loc.z, width, height, world->get_width(), world->get_height());
	in_view_npc = player->get_viewable_npcs(FIX_WORLD_COORD(player->loc.x - corner_x), FIX_WORLD_COORD(player->loc.y - corner_y), player->loc.z, width, height, world->get_width(), world->get_height());
	on_chunk = get_top_chunk(corner_x, corner_y);
	
	if(!tiles->is_walkable(on_chunk))
	{
		player->loc.x = last_loc.x;
		player->loc.y = last_loc.y;
		player->loc.z = last_loc.z;
		generate_view(player);
		return 0;
	}

	if(!player->energy)
		player->chunk_no = 240;

	in_view_npc[(corner_y * width) + corner_x] = player->chunk_no;

	for(ty = 0; ty < height; ty++)
	{
		for(tx = 0; tx < width; tx++)
		{
			screen->put_image((tx * tiles->get_width()) + pos_x, (ty * tiles->get_height()) + pos_y, tiles->get_tile(in_view_static[(ty * width) + tx], anim_index, 0), true);
			screen->put_image((tx * tiles->get_width()) + pos_x, (ty * tiles->get_height()) + pos_y, tiles->get_tile(in_view_static[(ty * width) + tx], anim_index, 1), true);
			screen->put_image((tx * tiles->get_width()) + pos_x, (ty * tiles->get_height()) + pos_y, tiles->get_tile(in_view_static[(ty * width) + tx], anim_index, 2), true);
			if(in_view_dynamic[(ty * width) + tx])
				screen->put_image((tx * tiles->get_width()) + pos_x, (ty * tiles->get_height()) + pos_y, tiles->get_tile(in_view_dynamic[(ty * width) + tx], anim_index, 2), true);
			if(in_view_npc[(ty * width) + tx])
				screen->put_image((tx * tiles->get_width()) + pos_x, (ty * tiles->get_height()) + pos_y, tiles->get_tile(in_view_npc[(ty * width) + tx], anim_index, 2), true);
		}
	}

	last_loc.x = player->loc.x;
	last_loc.y = player->loc.y;
	last_loc.z = player->loc.z;

	if(do_reactions)
		return tiles->reaction(on_chunk);
	else
		return 0;
}

bool VIEW::generate_view(PLAYER *player,long x, long y, long z)
{
	long on_chunk;

	if(in_view_static) 
		delete in_view_static;

	if(in_view_dynamic) 
		delete in_view_dynamic;

	if(in_view_npc) 
		delete in_view_npc;

	in_view_static = world->get_viewable(FIX_WORLD_COORD(x - corner_x), FIX_WORLD_COORD(y - corner_y), z, width, height, true);
	in_view_dynamic = player->get_viewable(FIX_WORLD_COORD(x - corner_x), FIX_WORLD_COORD(y - corner_y), z, width, height, world->get_width(), world->get_height());
	in_view_npc = player->get_viewable_npcs(FIX_WORLD_COORD(x - corner_x), FIX_WORLD_COORD(y - corner_y), z, width, height, world->get_width(), world->get_height());

	if(x == player->loc.x && y == player->loc.y && z == player->loc.z)
		return false;

	on_chunk = get_top_chunk(corner_x, corner_y);

	if(on_chunk == 13 || on_chunk == 40)
		return false;

	return tiles->is_walkable(on_chunk);
}

void VIEW::generate_view(long x, long y, long z, long anim_index)
{
	long tx, ty;

	if(in_view_static) 
		delete in_view_static;

	in_view_static = world->get_viewable(FIX_WORLD_COORD(x - corner_x), FIX_WORLD_COORD(y - corner_y), z, width, height);

	for(ty = 0; ty < height; ty++)
	{
		for(tx = 0; tx < width; tx++)
		{
			screen->put_image((tx * tiles->get_width()) + pos_x, (ty * tiles->get_height()) + pos_y, tiles->get_tile(in_view_static[(ty * width) + tx], anim_index, 0), true);
			screen->put_image((tx * tiles->get_width()) + pos_x, (ty * tiles->get_height()) + pos_y, tiles->get_tile(in_view_static[(ty * width) + tx], anim_index, 1), true);
			screen->put_image((tx * tiles->get_width()) + pos_x, (ty * tiles->get_height()) + pos_y, tiles->get_tile(in_view_static[(ty * width) + tx], anim_index, 2), true);
		}
	}

	last_loc.x = x;
	last_loc.y = y;
	last_loc.z = z;
	last_anim_index = anim_index;
}

void VIEW::generate_view(PLAYER *player)
{
	if(player)
		generate_view(player, last_anim_index, false);
	else
		generate_view(last_loc.x, last_loc.y, last_loc.z, last_anim_index);
}

long VIEW::get_quadrant(long x, long y)
{
	long tx, ty;
	long mx, my;
	long bx, by;

    mx = width * tiles->get_width();
	my = height * tiles->get_height();

	if(x < pos_x || y < pos_y || x >= pos_x + mx || y >= pos_y + my) 
		return 0;

	tx = x - pos_x;
	ty = y - pos_y;
	bx = (long)(ty / slope_ul2lr);
	by = (long)(tx * slope_ul2lr);

	if(tx > bx && tx < mx - bx)
		return 1;
	else if(ty > by && ty < my - by)
		return 2;
	else if(ty < by && ty > my - by)
		return 3;
	else if(tx < bx && tx > mx - bx)
		return 4;
	else
		return 0;
}

void VIEW::generate_all_chunk_view(long anim_index)
{
	long tx, ty, w, h, c;

	w = screen->get_width() / tiles->get_width();
	h = screen->get_height() / tiles->get_height();
	c =0;

	for(ty = 0; ty < h; ty++)
	{
		for(tx = 0; tx < w; tx++)
		{
			screen->put_image(tx * tiles->get_width(), ty * tiles->get_height(), tiles->get_tile(c, anim_index, 0), true);
			screen->put_image(tx * tiles->get_width(), ty * tiles->get_height(), tiles->get_tile(c, anim_index, 1), true);
			screen->put_image(tx * tiles->get_width(), ty * tiles->get_height(), tiles->get_tile(c, anim_index, 2), true);
			c++;
			if(c >= tiles->chunks_count())
				return;
		}
	}

}

BYTE VIEW::get_chunk_selected(long x, long y)
{
	BYTE tx, ty, w, h;
	BYTE c;

	w = (BYTE)(screen->get_width() / tiles->get_width());
	h = (BYTE)(screen->get_height() / tiles->get_height());
	tx = (BYTE)(x / tiles->get_width());
	ty = (BYTE)(y / tiles->get_height());
	
	c = (ty * w) + tx;

	if(c >= tiles->chunks_count())
		c = 0;

	return c;
}

char *VIEW::get_chunk_desc(long x, long y, PLAYER *player)
{
	long tx, ty, c;
	static TALK_DB talk_db;

	tx = (x - pos_x) / tiles->get_width();
	ty = (y - pos_y) / tiles->get_height();
	
	c =get_top_chunk(tx, ty);

	if(c == 26 || c == 30 || c == 28 || c == 23 || c == 27)
	{
		return talk_db.find_response(FIX_WORLD_COORD(player->loc.x + tx - corner_x), FIX_WORLD_COORD(player->loc.y + ty - corner_y), player->loc.z);
	}
	else
	{
		return tiles->get_desc(c);
	}
}

long VIEW::get_top_chunk(long x, long y)
{
	long chunk;

	chunk = in_view_npc[(y * width) + x];
	if(!chunk)
	{
		chunk = in_view_dynamic[(y * width) + x];
		if(!chunk)
			chunk = in_view_static[(y * width) + x];
	}

	return chunk;
}

long VIEW::use_chunk(PLAYER *player, long x, long y, bool with_spell)
{
	long tx, ty;

	tx = FIX_WORLD_COORD(FIX_WORLD_COORD(player->loc.x - (width >> 1)) + ((x - pos_x) / tiles->get_width()));
    ty = FIX_WORLD_COORD(FIX_WORLD_COORD(player->loc.y - (height >> 1)) + ((y - pos_y) / tiles->get_height()));

	return player->manipulate_dynamic(tx, ty, player->loc.z, with_spell);
}

long VIEW::get_height(void)
{
	return height;
}

long VIEW::get_width(void)
{
	return width;
}

void VIEW::fix_xy(long *x, long *y)
{
	*x = ((*x - pos_x) / tiles->get_width()) * tiles->get_width();
    *y = ((*y - pos_y) / tiles->get_height()) * tiles->get_height();
}

