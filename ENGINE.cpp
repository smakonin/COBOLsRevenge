/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include "engine.h"

ENGINE::ENGINE()
{
	anim_index = cloak_ticker = 0;
}

ENGINE::~ENGINE()
{
	mouse->hide();
	delete audio_mixer;
	delete small_font;
	delete font;
	delete mouse;
	delete keyboard;
	delete input_master;
	delete screen;
}

void ENGINE::incroment_counters(void)
{
	anim_index++;
	anim_index %= ANIM_FRAMES;
	cloak_ticker++;
	cloak_ticker2++;
}

void ENGINE::record_tick_count(void)
{
	tick_count = GetTickCount();
}

void ENGINE::record_tick_count2(void)
{
	cloak_ticker2 = 0;
}

bool ENGINE::check_tick_count(DWORD additional_count)
{
	if(tick_count + additional_count > GetTickCount())
		return false;
	else
		return true;
}

void ENGINE::wait_for_tick_count(DWORD additional_count)
{
	while(!check_tick_count(additional_count));
}

long ENGINE::get_anim_index(void)
{
	return anim_index;
}

long ENGINE::get_cloak_ticker(void)
{
	return cloak_ticker;
}

long ENGINE::get_cloak_ticker2(void)
{
	return cloak_ticker2;
}