/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include "globals.h"

FONT::FONT(char *fontname, long size, SCREEN *screen)
{
	FONT::screen = screen;
	height = size;

	font_data = CreateFont(size, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, fontname);
	if(!font_data)
		throw ERRORS(ERR_NOT_LOAD_FONT, fontname);
}

FONT::~FONT()
{
	DeleteObject(font_data);
}

void FONT::set_colour(long scheme, bool show_shadow)
{
	FONT::show_shadow = show_shadow;
	FONT::scheme = scheme;

	switch(scheme)
	{
		case BLUE_FONT:
			shadow_colour=RGB(51, 51, 251);
			colour=RGB(51, 0, 135);
			break;
		case RED_FONT:
			shadow_colour=RGB(123, 15, 0);
			colour=RGB(255, 15, 0);
			break;
		case PURPLE_FONT:
			shadow_colour=RGB(67, 19, 143);
			colour=RGB(123, 123, 223);
			break;
		case BLUE_FONT2:
			shadow_colour=RGB(51, 0, 135);
			colour=RGB(51, 51, 251);
			break;
		case PURPLE_FONT2:
			shadow_colour=RGB(51, 51, 251);
			colour=RGB(123, 123, 223);
			break;
		case BRIGHT_FONT:
			shadow_colour=RGB(67, 19, 143);
			colour=RGB(199, 199, 243);
			break;
		case GOLDEN_FONT:
			shadow_colour=RGB(19, 53, 0);
			colour=RGB(255, 227, 166);
			break;
		default:
			return;
	}
}

void FONT::print(char *text, long x, long y)
{
	HDC hdc;

	screen->get_image()->GetDC(&hdc);

	SelectObject(hdc, font_data);

	SetBkMode(hdc, TRANSPARENT);
	if(show_shadow)
	{
		SetTextColor(hdc, shadow_colour);
		ExtTextOut(hdc, x + 1, y + 1, 0, 0, text, strlen(text), 0);
	}
	SetTextColor(hdc, colour);
	ExtTextOut(hdc, x, y, 0, 0, text, strlen(text), 0);

	screen->get_image()->ReleaseDC(hdc);
}

void FONT::print(char *text, long x, long y, long wrap_length)
{
	char line[10000];
	long count_str, count_line;
	long num_of_lines;
	                                	                                
	count_str = 0;	                       
	count_line = 0;
	num_of_lines = 0;

	while(text[count_str] != '\0')
	{
		if(!count_line)                 
		{
			while(text[count_str++] == ' ');
			count_str--;
		}
		line[count_line] = text[count_str];
		count_str++;                                                          
		count_line++;
		line[count_line] = '\0';
		if(text[count_str] == '\0')
		{
			print(line, x, y + (num_of_lines * height));
			num_of_lines++;
			memset(line, 0, sizeof(line));
			count_line = 0;		
		}
		else if(count_line == wrap_length)
		{
			if(text[count_str] == ' ' || text[count_str - 1] == ' ')
			{                
				print(line, x, y + (num_of_lines * height));
				num_of_lines++;
				memset(line, 0, sizeof(line));
				count_line = 0;
		   }
		   else
		   {
				count_str--;   
				count_line--;
				while(text[count_str] != ' ')
				{
					line[count_line] = ' ';
					count_str--;					
					count_line--;         
					if(count_line <= 0)
						break;
				}
				print(line, x, y + (num_of_lines * height));
				num_of_lines++;
				memset(line, 0, sizeof(line));
				count_line = 0;
		   }
		}   
	}	
}

long FONT::get_height(void)
{
	return height;
}

long FONT::get_colour(void)
{
	return scheme;
}
