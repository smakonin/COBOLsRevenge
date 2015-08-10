/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#ifndef IMAGE_H
#define IMAGE_H

#include "globals.h"

#define BITMAP_ID  0x4D42

class BITMAP_FILE
{
	public:
		BITMAPFILEHEADER fileheader;
		BITMAPINFOHEADER infoheader;
		PALETTEENTRY palette[256];
		BYTE *buffer;
};

class SWM_FILE
{
	public:
		long width;
		long height;
		long pixel_size;
		long trans_colour;
		long comp_size;
};

class IMAGE 
{       
	protected:
		long height;
		long width;
		long actual_bpp;
		RECT dims;
		LPDIRECTDRAWSURFACE7 lpddsurface;
		DDSURFACEDESC2 ddsd;
		LPDIRECTDRAW7 lpdd7;
		LPDIRECTDRAWCLIPPER lpddclipper;
		DWORD *draw_buffer;
		WORD *draw_buffer2;
		long pixel_size_bits;
		long pixel_size_bytes;
		FILE_LIBRARY file_lib;
		void flip_image(BYTE *image, long bytes_per_line, long height);
		void create_surface(long width, long height);
		DWORD *copy_to_buffer(LPDIRECTDRAWSURFACE7 source);
		void setup_clipper(void);

	public:
		IMAGE();
		IMAGE(LPDIRECTDRAW7 lpdd7, long bpp = 32);
		IMAGE(long width, long height, LPDIRECTDRAW7 lpdd7, long bpp = 32);
		~IMAGE();
		void lock_surface(void);
		void unlock_surface(void);
		long get_height(void);
		long get_width(void);
		RECT *get_dims(void);
		LPDIRECTDRAWSURFACE7 get_image(void);
		void put_pixel(long x, long y, DWORD colour);
		void put_pixel(long x, long y, long red, long green, long blue);
		DWORD get_pixel(long x, long y);
		void put_pixels(DWORD *buffer);
		DWORD *get_pixels(void);
		void load(char *filename);
		void save(char *filename);
		void load_bmp(char *filename);
		void set_key_colour(DWORD colour);
		void draw_box(long x, long y, long width, long height, DWORD colour, bool is_solid = false, DWORD fill_colour = 0x000);
		void clear(DWORD colour);
		void put_image(RECT *dest_coord, IMAGE *img, bool use_key_colour = false);
		void put_image(long x, long y, IMAGE *img, bool use_key_colour = false);
		void put_image(long x, long y, long width, long height, IDirectDrawSurface7 *dd, bool use_key_colour = false);
		void fade(long x, long y, long width, long height, double percent);
		long get_bpp_used(void);
};

#endif //IMAGE_H



