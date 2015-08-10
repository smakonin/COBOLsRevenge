/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include "globals.h"

IMAGE::IMAGE()
{
}

IMAGE::IMAGE(LPDIRECTDRAW7 lpdd7, long bpp)
{
	height = 0;
	width = 0;
	dims.bottom = 0;
	dims.left = 0;
	dims.right = 0;
	dims.top = 0;
	DDRAW_INIT_STRUCT(ddsd);
	lpddsurface = NULL;
	draw_buffer = NULL;
	pixel_size_bits = bpp;
	pixel_size_bytes = bpp / 8;
	lpddclipper = NULL;

	IMAGE::lpdd7 = lpdd7;
}

IMAGE::IMAGE(long width, long height, LPDIRECTDRAW7 lpdd7, long bpp)
{
	pixel_size_bits = bpp;
	pixel_size_bytes = bpp / 8;
	IMAGE::lpdd7 = lpdd7;
	lpddclipper = NULL;

	create_surface(width, height);
}

IMAGE::~IMAGE()
{
	if(lpddclipper)
	{
		lpddclipper->Release();
		lpddclipper = NULL;
	}

	if(lpddsurface)
	{
		lpddsurface->Release();
		lpddsurface = NULL;
	}
}

void IMAGE::create_surface(long width, long height)
{
	DDPIXELFORMAT pixel_format;

	IMAGE::width = width;
	IMAGE::height = height;

	DDRAW_INIT_STRUCT(ddsd);
	ddsd.dwSize  = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.dwWidth = width;
	ddsd.dwHeight = height;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;

	dims.left = 0;
	dims.top = 0;
	dims.right = width;
	dims.bottom = height;

	if(FAILED(lpdd7->CreateSurface(&ddsd, &lpddsurface, NULL)))
		throw ERRORS(ERR_ALLOC_MEM, "Image Surface");

	DDRAW_INIT_STRUCT(pixel_format);
	pixel_format.dwSize = sizeof(pixel_format);
	lpddsurface->GetPixelFormat(&pixel_format);
	pixel_size_bits = pixel_format.dwRGBBitCount;
	pixel_size_bytes = pixel_size_bits / 8;
	actual_bpp = get_bpp_used();
}

long IMAGE::get_height(void)
{
	return height;
}

long IMAGE::get_width(void)
{
	return width;
}

RECT *IMAGE::get_dims(void)
{
	return &dims;
}

LPDIRECTDRAWSURFACE7 IMAGE::get_image(void)
{
	return lpddsurface;
}

void IMAGE::lock_surface(void)
{
	DDRAW_INIT_STRUCT(ddsd);
	lpddsurface->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);

	draw_buffer = (DWORD *)ddsd.lpSurface;
	draw_buffer2 = (WORD *)draw_buffer;
}

void IMAGE::unlock_surface(void)
{
	if(FAILED(lpddsurface->Unlock(NULL)))
		return;

	draw_buffer = NULL;
	draw_buffer2 = (WORD *)draw_buffer;
}

void IMAGE::put_pixel(long x, long y, DWORD colour)
{
	BYTE r, g, b;

	lock_surface();
	
	switch(actual_bpp)
	{
		case 15:
			r = _RGB32RED(colour) >> 3;
			g = _RGB32GREEN(colour) >> 3;
			b = _RGB32BLUE(colour) >> 3;
			draw_buffer2[x + y * (ddsd.lPitch >> 1)] = _RGB16BIT555(r, g, b);
			break;

		case 16:
			r = _RGB32RED(colour) >> 3;
			g = _RGB32GREEN(colour) >> 3;
			b = _RGB32BLUE(colour) >> 3;
			draw_buffer2[x + y * (ddsd.lPitch >> 1)] = _RGB16BIT565(r, g, b);
			break;

		default:
			draw_buffer[x + y * (ddsd.lPitch >> 2)] = colour;
			break;
	}
		
	unlock_surface();
}

void IMAGE::put_pixel(long x, long y, long red, long green, long blue)
{
	DWORD pixel;
	BYTE r, g, b;

	lock_surface();

	pixel = _RGB32BIT(0, red, green, blue);

	switch(actual_bpp)
	{
		case 15:
			r = (BYTE)red >> 3;
			g = (BYTE)green >> 3;
			b = (BYTE)blue >> 3;
			draw_buffer2[x + y * (ddsd.lPitch >> 1)] = _RGB16BIT555(r, g, b);
			break;

		case 16:
			r = (BYTE)red >> 3;
			g = (BYTE)green >> 4;
			b = (BYTE)blue >> 3;
			draw_buffer2[x + y * (ddsd.lPitch >> 1)] = _RGB16BIT565(r, g, b);
			break;

		default:
			draw_buffer[x + y * (ddsd.lPitch >> 2)] = pixel;	
			break;
	}

	unlock_surface();
}

DWORD IMAGE::get_pixel(long x, long y)
{
	DWORD pixel;

	lock_surface();
 	pixel = draw_buffer[(y * (ddsd.lPitch >> 2)) + x];	
	unlock_surface();

	return pixel;
}

void IMAGE::put_pixels(DWORD *buffer)
{
	BYTE r, g, b;
	long x, y;

	lock_surface();

	if(actual_bpp == 16 || actual_bpp == 15)
	{
		for(y = 0; y < height; y++)
		{
			for(x = 0; x < width; x++)
			{
				switch(actual_bpp)
				{
					case 15:
						r = _RGB32RED(buffer[x + (y * width)]) >> 3;
						g = _RGB32GREEN(buffer[x + (y * width)]) >> 3;
						b = _RGB32BLUE(buffer[x + (y * width)]) >> 3;
						draw_buffer2[x + y * (ddsd.lPitch >> 1)] = _RGB16BIT555(r, g, b);
						break;

					case 16:
						r = _RGB32RED(buffer[x + (y * width)]) >> 3;
						g = _RGB32GREEN(buffer[x + (y * width)]) >> 3;
						b = _RGB32BLUE(buffer[x + (y * width)]) >> 3;
						draw_buffer2[x + y * (ddsd.lPitch >> 1)] = _RGB16BIT565(r, g, b);
						break;
				}
			}
		}
	}
	else
	{
 		memcpy(draw_buffer, buffer, (width * height * pixel_size_bytes));	
	}

	unlock_surface();
}

DWORD *IMAGE::get_pixels(void)
{
	DWORD *buf;

	buf = new DWORD[width * height];

	lock_surface();
 	memcpy(buf, draw_buffer, (width * height * pixel_size_bytes));	
	unlock_surface();

	return buf;
}

void IMAGE::load(char *filename)
{
	SWM_FILE hdr;
	FILE *fp = NULL;
	BYTE *comp_data = NULL;
	DWORD *data = NULL;
	long index_x;
	long index_y;
	BYTE r, g, b;

	fp = fopen(filename, "rb");
	if(!fp)
		throw ERRORS(ERR_MISSING_FILE, filename);

	fread(&hdr, sizeof(SWM_FILE), 1, fp);
	comp_data = new BYTE[hdr.comp_size];
	fread(comp_data, hdr.comp_size, 1, fp);
	fclose(fp);
	fp = NULL;

	data = (DWORD *)file_lib.uncompress(comp_data, &hdr.comp_size, hdr.height * hdr.width * hdr.pixel_size);

	create_surface(hdr.width, hdr.height);

	lock_surface();

	for(index_y = 0; index_y < height; index_y++)
	{
		for(index_x = 0; index_x < width; index_x++)
		{
			switch(actual_bpp)
			{
				case 15:
					r = _RGB32RED(data[index_x + (index_y * width)]) >> 3;
					g = _RGB32GREEN(data[index_x + (index_y * width)]) >> 3;
					b = _RGB32BLUE(data[index_x + (index_y * width)]) >> 3;
					draw_buffer2[index_x + (index_y * ddsd.lPitch >> 1)] = _RGB16BIT555(r, g, b);
					break;

				case 16:
					r = _RGB32RED(data[index_x + (index_y * width)]) >> 3;
					g = _RGB32GREEN(data[index_x + (index_y * width)]) >> 3;
					b = _RGB32BLUE(data[index_x + (index_y * width)]) >> 3;
					draw_buffer2[index_x + (index_y * ddsd.lPitch >> 1)] = _RGB16BIT565(r, g, b);
					break;

				default:
					draw_buffer[index_x + (index_y * ddsd.lPitch >> 2)] = data[index_x + (index_y * width)];
					break;
			}
		}
	}

	unlock_surface();

	set_key_colour(hdr.trans_colour);

	delete data;
}

void IMAGE::save(char *filename)
{
	SWM_FILE hdr;
	FILE *fp = NULL;
	DWORD *comp_data = NULL;
	DWORD *raw_data = NULL;
	long size;
	long index_x;
	long index_y;

	hdr.height = height;
	hdr.pixel_size = pixel_size_bytes;
	hdr.trans_colour = _RGB_WHITE;
	hdr.width = width;
	size = hdr.height * hdr.width * hdr.pixel_size;
	raw_data = new DWORD[hdr.height * hdr.width];

	lock_surface();

	for(index_y = 0; index_y < height; index_y++)
		for(index_x = 0; index_x < width; index_x++)
			raw_data[index_x + (index_y * width)] = draw_buffer[index_x + (index_y * ddsd.lPitch >> 2)];

	unlock_surface();

	comp_data = (DWORD *)file_lib.compress(raw_data, &size);
	hdr.comp_size = size;

	fp = fopen(filename, "wb");
	if(!fp)
		throw ERRORS(ERR_MISSING_FILE, filename);
	
	fwrite(&hdr, sizeof(SWM_FILE), 1, fp);
	fwrite(comp_data, size, 1, fp);

	delete comp_data;
	fclose(fp);
	fp = NULL;
}

void IMAGE::load_bmp(char *filename)
{
	BITMAP_FILE bitmap;
	long file_handle = 0, index;
	OFSTRUCT file_data;
	int temp_color;
	long index_x;
	long index_y;
	BYTE red;
	BYTE green;
	BYTE blue;
	DWORD pixel;

	bitmap.buffer = NULL;

	if((file_handle = OpenFile(filename, &file_data, OF_READ)) == -1)
	   throw ERRORS(ERR_MISSING_FILE, filename);

	_lread(file_handle, &bitmap.fileheader, sizeof(BITMAPFILEHEADER));

	if(bitmap.fileheader.bfType != BITMAP_ID)
	   throw ERRORS(ERR_MISSING_FILE, filename);

	_lread(file_handle, &bitmap.infoheader, sizeof(BITMAPINFOHEADER));

	if (bitmap.infoheader.biBitCount == 8)
	{
		_lread(file_handle, &bitmap.palette, MAX_COLORS_PALETTE * sizeof(PALETTEENTRY));
		
		for (index=0; index < MAX_COLORS_PALETTE; index++)
		{
			temp_color = bitmap.palette[index].peRed;
			bitmap.palette[index].peRed = bitmap.palette[index].peBlue;
			bitmap.palette[index].peBlue = temp_color;
			bitmap.palette[index].peFlags = PC_NOCOLLAPSE;
		}		
    }
	
	_lseek(file_handle, -(long)(bitmap.infoheader.biSizeImage), SEEK_END);
	
	if (bitmap.infoheader.biBitCount==8 || bitmap.infoheader.biBitCount==16 || bitmap.infoheader.biBitCount==24)
	{
		if(bitmap.buffer)
			free(bitmap.buffer);

		if(!(bitmap.buffer = (BYTE *)malloc(bitmap.infoheader.biSizeImage)))
			throw ERRORS(ERR_ALLOC_MEM, "Image Holding Buffer");

		_lread(file_handle, bitmap.buffer, bitmap.infoheader.biSizeImage);
	}
	else
	{
	   throw ERRORS(ERR_MISSING_FILE, filename);
	}

	flip_image(bitmap.buffer, bitmap.infoheader.biWidth * (bitmap.infoheader.biBitCount / 8), bitmap.infoheader.biHeight);

	create_surface(bitmap.infoheader.biWidth, bitmap.infoheader.biHeight);

	lock_surface();

	for(index_y = 0; index_y < height; index_y++)
    {
		for(index_x = 0; index_x < width; index_x++)
        {
			blue = (bitmap.buffer[index_y * width * 3 + index_x * 3 + 0]);
			green = (bitmap.buffer[index_y * width * 3 + index_x * 3 + 1]);
			red = (bitmap.buffer[index_y * width * 3 + index_x * 3 + 2]);
			
			pixel = _RGB32BIT(0, red, green, blue);
			draw_buffer[index_x + (index_y * ddsd.lPitch >> 2)] = pixel;			
        }		
    }

	unlock_surface();

	if(file_handle) _lclose(file_handle);
	if(bitmap.buffer) free(bitmap.buffer);
	bitmap.buffer = NULL;
}

void IMAGE::flip_image(BYTE *image, long bytes_per_line, long height)
{

	BYTE *buffer;
	long index;
	
	if(!(buffer = (BYTE *)malloc(bytes_per_line * height)))
		throw ERRORS(ERR_ALLOC_MEM, "Temp Flip Buffer");
	
	memcpy(buffer,image, bytes_per_line * height);
	
	for (index=0; index < height; index++)
		memcpy(&image[((height - 1) - index) * bytes_per_line], &buffer[index * bytes_per_line], bytes_per_line);
	
	free(buffer);
}

void IMAGE::set_key_colour(DWORD colour)
{
	DDCOLORKEY colour_key;
	BYTE r, g, b;

	switch(actual_bpp)
	{
		case 15:
			r = _RGB32RED(colour) >> 3;
			g = _RGB32GREEN(colour) >> 3;
			b = _RGB32BLUE(colour) >> 3;
			colour = _RGB16BIT555(r, g, b);
			break;

		case 16:
			r = _RGB32RED(colour) >> 3;
			g = _RGB32GREEN(colour) >> 3;
			b = _RGB32BLUE(colour) >> 3;
			colour = _RGB16BIT565(r, g, b);
			break;
	}
	colour_key.dwColorSpaceLowValue = colour;
	colour_key.dwColorSpaceHighValue = colour;

	lpddsurface->SetColorKey(DDCKEY_SRCBLT, &colour_key);
}

void IMAGE::draw_box(long x, long y, long width, long height, DWORD colour, bool is_solid, DWORD fill_colour)
{
	DDBLTFX ddbltfx;
	long count;
	RECT rec;
	BYTE r, g, b;

	switch(actual_bpp)
	{
		case 15:
			r = _RGB32RED(fill_colour) >> 3;
			g = _RGB32GREEN(fill_colour) >> 3;
			b = _RGB32BLUE(fill_colour) >> 3;
			fill_colour = _RGB16BIT555(r, g, b);
			break;

		case 16:
			r = _RGB32RED(fill_colour) >> 3;
			g = _RGB32GREEN(fill_colour) >> 3;
			b = _RGB32BLUE(fill_colour) >> 3;
			fill_colour = _RGB16BIT565(r, g, b);
			break;
	}

	DDRAW_INIT_STRUCT(ddbltfx);
	ddbltfx.dwFillColor = fill_colour;
	rec.top = y + 1;
	rec.left = x + 1;
	rec.bottom = y + height;
	rec.right = x + width;

	switch(actual_bpp)
	{
		case 15:
			r = _RGB32RED(colour) >> 3;
			g = _RGB32GREEN(colour) >> 3;
			b = _RGB32BLUE(colour) >> 3;
			colour = _RGB16BIT555(r, g, b);
			break;

		case 16:
			r = _RGB32RED(colour) >> 3;
			g = _RGB32GREEN(colour) >> 3;
			b = _RGB32BLUE(colour) >> 3;
			colour = _RGB16BIT565(r, g, b);
			break;
	}

	lock_surface();

	if(actual_bpp == 16 || actual_bpp == 15)
	{
		for(count = 0; count <= width; count++)
		{
			draw_buffer2[(x + count) + y * (ddsd.lPitch >> 1)] = (WORD)colour;
			draw_buffer2[(x + count) + (y + height) * (ddsd.lPitch >> 1)] = (WORD)colour;
		}

		for(count = 0; count <= height; count++)
		{
			draw_buffer2[x + (y + count) * (ddsd.lPitch >> 1)] = (WORD)colour;
			draw_buffer2[(x + width) + (y + count) * (ddsd.lPitch >> 1)] = (WORD)colour;
		}
	}
	else
	{
		for(count = 0; count <= width; count++)
		{
			draw_buffer[(x + count) + y * (ddsd.lPitch >> 2)] = colour;
			draw_buffer[(x + count) + (y + height) * (ddsd.lPitch >> 2)] = colour;
		}

		for(count = 0; count <= height; count++)
		{
			draw_buffer[x + (y + count) * (ddsd.lPitch >> 2)] = colour;
			draw_buffer[(x + width) + (y + count) * (ddsd.lPitch >> 2)] = colour;
		}
	}

	unlock_surface();

	if(is_solid)
		lpddsurface->Blt(&rec, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
}

void IMAGE::clear(DWORD colour)
{
	DDBLTFX ddbltfx;
	
	DDRAW_INIT_STRUCT(ddbltfx);
	ddbltfx.dwFillColor = colour; 

	lpddsurface->Blt(&dims, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
}

DWORD *IMAGE::copy_to_buffer(LPDIRECTDRAWSURFACE7 source)
{
	DWORD *buffer;
	DWORD *src_buf;
	long index_x;
	long index_y;
	DWORD pixel;

	buffer = new DWORD[width * height];

	DDRAW_INIT_STRUCT(ddsd);
	source->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);
	src_buf = (DWORD *)ddsd.lpSurface;

	for(index_y = 0; index_y < height; index_y++)
	{
		for(index_x = 0; index_x < width; index_x++)
		{
			pixel = src_buf[index_x + (index_y * ddsd.lPitch >> 2)];
			buffer[index_x + (index_y * width)] = pixel;
		}
	}

	if(FAILED(source->Unlock(NULL)))
	{
		delete buffer;
		return NULL;
	}

	src_buf = NULL;	
	
	return buffer;
}

void IMAGE::put_image(RECT *dest_coord, IMAGE *img, bool use_key_colour)
{
	if(!img) return;

	if(use_key_colour)
		lpddsurface->Blt(dest_coord, img->get_image(), img->get_dims(), DDBLT_ASYNC | DDBLT_KEYSRC, NULL);
		//lpddsurface->BltFast(dest_coord->left, dest_coord->top, img->get_image(), img->get_dims(), DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	else
		lpddsurface->Blt(dest_coord, img->get_image(), img->get_dims(), DDBLT_ASYNC, NULL);//DDBLT_WAIT
		//lpddsurface->BltFast(dest_coord->left, dest_coord->top, img->get_image(), img->get_dims(), DDBLTFAST_WAIT);
}

void IMAGE::put_image(long x, long y, IMAGE *img, bool use_key_colour)
{
	RECT rec;

	if(!img) return;

	rec.top = y;
	rec.left = x;
	rec.bottom = rec.top + img->get_height();
	rec.right = rec.left + img->get_width();

	put_image(&rec, img, use_key_colour);
}

void IMAGE::put_image(long x, long y, long width, long height, IDirectDrawSurface7 *dd, bool use_key_colour)
{
	RECT rec;

	rec.top = y;
	rec.left = x;
	rec.bottom = rec.top + height;
	rec.right = rec.left + width;

	if(use_key_colour)
		lpddsurface->Blt(&rec, dd, &rec, DDBLT_ASYNC | DDBLT_KEYSRC, NULL);
	else
		lpddsurface->Blt(&rec, dd, &rec, DDBLT_ASYNC, NULL);
}

void IMAGE::setup_clipper(void)
{
}

void IMAGE::fade(long x, long y, long width, long height, double percent)
{
	long i, j;
	BYTE r, g, b;
	DWORD l, c;

	percent /= 100;

	lock_surface();

	for(i = y; i <= height; i++)
	{
			l = i * IMAGE::width;
			for(j = x; j <= width; j++)
			{
				c =draw_buffer[l + j];
				r = (BYTE)(_RGB32RED(c) * percent);
				g = (BYTE)(_RGB32GREEN(c) * percent);
				b = (BYTE)(_RGB32BLUE(c) * percent);
				c = _RGB32BIT(0, r, g, b);
				draw_buffer[l + j] = c;
			}
	}

	unlock_surface();
}

long IMAGE::get_bpp_used(void)
{
	DDPIXELFORMAT ddpixel;

	DDRAW_INIT_STRUCT(ddpixel);
	ddpixel.dwSize = sizeof(ddpixel);
	lpddsurface->GetPixelFormat(&ddpixel);

	if(ddpixel.dwRGBBitCount == 16)
	{
		if(ddpixel.dwGBitMask == 0x000007E0)
			return 16;
		else
			return 15;
	}
	else
	{
		return ddpixel.dwRGBBitCount;
	}
}