/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#ifndef IMAGE_LIBRARY_H
#define IMAGE_LIBRARY_H

#include "globals.h"

class IMAGE_LIBRARY 
{

	protected:
		long width;
		long height;
		long pixel_size;
		DWORD trans_colour;
		long count;
		long image_size;
		DWORD *images;
		FILE_LIBRARY file_lib;

	public:
		IMAGE_LIBRARY();
		IMAGE_LIBRARY(char *filename);
		~IMAGE_LIBRARY();
		void load(char *filename, FILE *ext_fp = NULL);
		void save(char *filename);
		long get_width(void);
		long get_height(void);
		long get_count(void);
		void set_key_colour(DWORD colour);
		DWORD *get_starting_ptr(long item_no);
		void remove(long item_no);
		void add(void);
		void add(IMAGE *src_img);
		void add(long before_item_no);
		void copy(long item_no);
		void clear(long item_no, DWORD colour = 0x0000);
		void replace_colour(long item_no, DWORD original_colour, DWORD new_colour);
		void plot_pixel(long item_no, long x, long y, DWORD colour);
		void rotate(long item_no);
		void flip_horz(long item_no);
		void flip_vert(long item_no);
		void shift_down(long item_no);
		void shift_right(long item_no);
		void diagnal_copy_htov(long item_no);
		void diagnal_copy_vtoh(long item_no);
};

#endif //IMAGE_LIBRARY_H

