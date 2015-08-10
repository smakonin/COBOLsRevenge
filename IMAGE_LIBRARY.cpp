/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include "globals.h"

IMAGE_LIBRARY::IMAGE_LIBRARY()
{
	width = 0;
	height = 0;
	pixel_size = 4;
	trans_colour = 0x00FFFFFF;
	count = 0;
	images = NULL;
	image_size = 0;
}

IMAGE_LIBRARY::IMAGE_LIBRARY(char *filename)
{
	IMAGE_LIBRARY();
	load(filename);
}

IMAGE_LIBRARY::~IMAGE_LIBRARY()
{
	delete images;
}

void IMAGE_LIBRARY::load(char *filename, FILE *ext_fp)
{
	FILE *fp = ext_fp;
	long comp_size = 0;

	IMAGE_LIBRARY();

	if(!ext_fp)
	{
		fp = fopen(filename, "rb");
		if(!fp)
			throw ERRORS(ERR_MISSING_FILE, filename);
	}

	fread(&width, sizeof(width), 1, fp);
	fread(&height, sizeof(height), 1, fp);
	fread(&pixel_size, sizeof(pixel_size), 1, fp);
	fread(&trans_colour, sizeof(trans_colour), 1, fp);
	fread(&count, sizeof(count), 1, fp);
	fread(&comp_size, sizeof(comp_size), 1, fp);

	image_size = width * height;

	images = new DWORD[comp_size];
	fread(images, comp_size, 1, fp);
	images = (DWORD *)file_lib.uncompress(images, &comp_size, (width * height * count * pixel_size));

	if(!ext_fp)
	{
		fclose(fp);
		fp = NULL;
	}
}

void IMAGE_LIBRARY::save(char *filename)
{
	FILE *fp = NULL;
	DWORD *comp_data = NULL;
	DWORD *temp_data = NULL;
	long size;

	fp = fopen(filename, "wb");
	if(!fp)
		throw ERRORS(ERR_MISSING_FILE, filename);

	fwrite(&width, sizeof(width), 1, fp);
	fwrite(&height, sizeof(height), 1, fp);
	fwrite(&pixel_size, sizeof(pixel_size), 1, fp);
	fwrite(&trans_colour, sizeof(trans_colour), 1, fp);
	fwrite(&count, sizeof(count), 1, fp);

	size = width * height * count * pixel_size;
	temp_data = new DWORD[width * height * count];
	memcpy(temp_data, images, size);
	comp_data = (DWORD *)file_lib.compress(temp_data, &size);	
	fwrite(&size, sizeof(size), 1, fp);
	fwrite(comp_data, size, 1, fp);

	fclose(fp);
	fp = NULL;
	delete comp_data;
}

long IMAGE_LIBRARY::get_width(void)
{
	return width;
}

long IMAGE_LIBRARY::get_height(void)
{
	return height;
}

long IMAGE_LIBRARY::get_count(void)
{
	return count;
}

void IMAGE_LIBRARY::set_key_colour(DWORD colour)
{
	trans_colour = colour;
}

DWORD *IMAGE_LIBRARY::get_starting_ptr(long item_no)
{
	return images + (item_no * image_size);
}

void IMAGE_LIBRARY::remove(long item_no)
{
	DWORD *tmp_buf = NULL;

	if(count > -1)
	{
		tmp_buf = new DWORD[image_size * count - 1];
		memcpy(tmp_buf, images, item_no * (image_size * pixel_size));
		memcpy(tmp_buf + (item_no * image_size), get_starting_ptr(item_no + 1), (((count) - (item_no + 1)) * image_size * pixel_size));
	}

	delete images;
	count--;	
	images = tmp_buf;
}

void IMAGE_LIBRARY::add(void)
{
	DWORD *tmp_buf = NULL;

	tmp_buf = new DWORD[image_size * (count + 1)];
	memcpy(tmp_buf, images, (image_size * pixel_size) * count);
	delete images;
	images = tmp_buf;

	clear(count);

	count++;
}

void IMAGE_LIBRARY::add(IMAGE *src_img)
{
	DWORD *tmp_buf = NULL;

	tmp_buf = new DWORD[image_size * (count + 1)];
	memcpy(tmp_buf, images, (image_size * pixel_size) * count);
	delete images;
	images = tmp_buf;

	tmp_buf = src_img->get_pixels();	
	memcpy(get_starting_ptr(count), tmp_buf, (image_size * pixel_size));

	delete tmp_buf;
	count++;
}

void IMAGE_LIBRARY::add(long before_item_no)
{
	add();
	memcpy(get_starting_ptr(before_item_no + 1), get_starting_ptr(before_item_no), (count - before_item_no - 1) * image_size * pixel_size);
	clear(before_item_no, 0);
}

void IMAGE_LIBRARY::copy(long item_no)
{
	DWORD *tmp_buf = NULL;

	tmp_buf = new DWORD[image_size * (count + 1)];
	memcpy(tmp_buf, images, (image_size * pixel_size) * count);
	delete images;
	images = tmp_buf;

	memcpy(get_starting_ptr(count), get_starting_ptr(item_no), image_size * pixel_size);

	count++;
}

void IMAGE_LIBRARY::clear(long item_no, DWORD colour)
{
	long index;
	DWORD *ptr;

	ptr = get_starting_ptr(item_no);

	for(index = 0; index < image_size; index++)
		ptr[index] = colour;
}

void IMAGE_LIBRARY::plot_pixel(long item_no, long x, long y, DWORD colour)
{
	DWORD *ptr;

	ptr = get_starting_ptr(item_no);
	ptr[(y * width) + x] = colour;
}

void IMAGE_LIBRARY::replace_colour(long item_no, DWORD original_colour, DWORD new_colour)
{
	long index;
	DWORD *ptr;

	ptr = get_starting_ptr(item_no);

	for(index = 0; index < image_size; index++)
		if(ptr[index] == original_colour)
			ptr[index] = new_colour;
}

void IMAGE_LIBRARY::rotate(long item_no)
{
	DWORD *tmp_buf = NULL;
	DWORD *ptr;
	long x;
	long y;

	tmp_buf = new DWORD[image_size];
	ptr = get_starting_ptr(item_no);

	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			tmp_buf[((width - x - 1) * height) + y] = ptr[(y * width) + x];
		}
	}

	memcpy(get_starting_ptr(item_no), tmp_buf, image_size * pixel_size);

	delete tmp_buf;}

void IMAGE_LIBRARY::flip_horz(long item_no)
{
	DWORD *tmp_buf = NULL;
	DWORD *ptr;
	long x;
	long y;

	tmp_buf = new DWORD[image_size];
	ptr = get_starting_ptr(item_no);

	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			tmp_buf[((height - y - 1) * width) + x] = ptr[(y * width) + x];
		}
	}

	memcpy(get_starting_ptr(item_no), tmp_buf, image_size * pixel_size);

	delete tmp_buf;
}

void IMAGE_LIBRARY::flip_vert(long item_no)
{
	DWORD *tmp_buf = NULL;
	DWORD *ptr;
	long x;
	long y;

	tmp_buf = new DWORD[image_size];
	ptr = get_starting_ptr(item_no);

	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			tmp_buf[(y * width) + (width - x - 1)] = ptr[(y * width) + x];
		}
	}

	memcpy(get_starting_ptr(item_no), tmp_buf, image_size * pixel_size);

	delete tmp_buf;
}

void IMAGE_LIBRARY::shift_down(long item_no)
{
	DWORD *tmp_buf = NULL;
	DWORD *ptr;
	long x;
	long y;
	long line;

	tmp_buf = new DWORD[image_size];
	ptr = get_starting_ptr(item_no);
	line = height - 1;

	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			tmp_buf[(y * width) + x] = ptr[(line * width) + x];
		}
		line++;
		line %= height;
	}

	memcpy(get_starting_ptr(item_no), tmp_buf, image_size * pixel_size);

	delete tmp_buf;
}

void IMAGE_LIBRARY::shift_right(long item_no)
{
	DWORD *tmp_buf = NULL;
	DWORD *ptr;
	long x;
	long y;
	long line;

	tmp_buf = new DWORD[image_size];
	ptr = get_starting_ptr(item_no);
	line = width - 1;

	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			tmp_buf[(y * width) + x] = ptr[(y * width) + line];
			line++;
			line %= width;
		}
	}

	memcpy(get_starting_ptr(item_no), tmp_buf, image_size * pixel_size);

	delete tmp_buf;
}

void IMAGE_LIBRARY::diagnal_copy_htov(long item_no)
{
	DWORD *tmp_buf = NULL;
	DWORD *ptr;
	long x;
	long y;

	tmp_buf = new DWORD[image_size];
	ptr = get_starting_ptr(item_no);

	for(y = 0; y < height; y++)
	{
		for(x = 0; x <= y; x++)
		{
			tmp_buf[(x * width) + y] = ptr[(x * width) + y];
			tmp_buf[(y * width) + x] = ptr[(x * width) + y];
		}
	}

	memcpy(get_starting_ptr(item_no), tmp_buf, image_size * pixel_size);

	delete tmp_buf;
}

void IMAGE_LIBRARY::diagnal_copy_vtoh(long item_no)
{
	DWORD *tmp_buf = NULL;
	DWORD *ptr;
	long x;
	long y;

	tmp_buf = new DWORD[image_size];
	ptr = get_starting_ptr(item_no);

	for(y = 0; y < height; y++)
	{
		for(x = 0; x <= y; x++)
		{
			tmp_buf[(x * width) + y] = ptr[(y * width) + x];
			tmp_buf[(y * width) + x] = ptr[(y * width) + x];
		}
	}

	memcpy(get_starting_ptr(item_no), tmp_buf, image_size * pixel_size);

	delete tmp_buf;
}