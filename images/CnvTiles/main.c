/**********************************************************************
*
*	This file is Copyright (C) 1999, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>
#include <malloc.h>
#include <string.h>  
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>   
#include <time.h>
#include <math.h>

#include "C:\PKW32DCL\IMPLODE.H"

#define TILE_DB_NAME "TILES.RAW"
#define PALETTE_NAME "PALETTE.PAL"
#define NEW_TILES_NAME "TLIMG.CR"
#define _RGB16BIT555(r, g, b)       ((b % 32) + ((g % 32) << 5) + ((r % 32) << 10))
#define _RGB16BIT565(r, g, b)       ((b % 32) + ((g % 64) << 6) + ((r % 32) << 11))
#define _RGB32BIT(a,r,g,b)          ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))

typedef unsigned char 	BYTE;
typedef unsigned short 	WORD;
typedef unsigned long 	DWORD;

typedef struct COLOUR_TYPE
{
   BYTE red;
   BYTE green;
   BYTE blue;
}COLOUR,*COLOUR_PTR;

typedef struct PALETTE_TYPE
{
   COLOUR colour[256];
}PALETTE,*PALETTE_PTR;

typedef struct IMG_LIB_TYPE
{
	long width;
	long height;
	long pixel_size;
	long trans_colour;
	long frames;
	long comp_size;
}IMG_LIB_TYPE,*IMG_LIB_TYPE_PTR;

BYTE *tiles;								
PALETTE pal;
IMG_LIB_TYPE il_hdr;
WORD tile_sliver[32];

DWORD FileSize(char*FileName)
{
	DWORD length;
	int file;

	file=open(FileName,O_RDONLY+O_BINARY);
	length=filelength(file);
	close(file);
	return(length);
}  

//****************************************************************************
typedef struct PARAM_TYPE
{
	char *source_ptr;
	char *dest_ptr;
	long source_offset;
	long dest_offset;
	long comp_size;
	long uncomp_size;
	long buf_size;
	long crc;
	long orig_crc;
}PARAM;

unsigned int read_uncomp(char *buf, unsigned int *size, void *param)
{
	PARAM *ptr = (PARAM *)param;
	
	if (ptr->uncomp_size == 0L)  
		return 0;
	
	if (ptr->uncomp_size < (long)*size)
		*size = (long)ptr->uncomp_size;
	
	memcpy(buf, ptr->source_ptr + ptr->source_offset, *size);
	ptr->source_offset += (long)*size;
	ptr->uncomp_size -= (long)*size;
	ptr->crc = crc32(buf, size, (unsigned long *)&ptr->crc);
	
	return *size;
}

unsigned int read_comp(char *buf, unsigned int *size, void *param)
{
	PARAM *ptr = (PARAM *)param;
	
	if (ptr->comp_size == 0L)  
		return 0;
	
	if (ptr->comp_size < (long)*size)
		*size = (long)ptr->comp_size;
	
	memcpy(buf, ptr->source_ptr + ptr->source_offset, *size);
	ptr->source_offset += (long)*size;
	ptr->comp_size -= (long)*size;
	
	return *size;
}

void write_comp(char *buf, unsigned int *size, void *param)
{
	PARAM *ptr = (PARAM *)param;
	
	if (ptr->comp_size + (long)*size > ptr->buf_size)
		exit(1);

	memcpy(ptr->dest_ptr + ptr->dest_offset, buf, *size);
	ptr->dest_offset += (long)*size;
	ptr->comp_size += (long)*size;
}

void write_uncomp(char *buf, unsigned int *size, void *param)
{
	PARAM *ptr = (PARAM *)param;
	
	if (ptr->comp_size + (long)*size > ptr->buf_size)
		exit(1);

	memcpy(ptr->dest_ptr + ptr->dest_offset, buf, *size);
	ptr->dest_offset += (long)*size;
	ptr->uncomp_size += (long)*size;
	ptr->crc = crc32(buf, size, (unsigned long *)&ptr->crc);
}

void *compress(void *buf, long *size)
{
	char *work_buf;
	unsigned int type;
	unsigned int dsize;
	PARAM param;
	
	param.uncomp_size = *size;
	param.buf_size = param.uncomp_size;
	param.comp_size = 0L;
	param.source_ptr = (char *)buf;
	
	param.dest_ptr = (char *)malloc(param.buf_size);
	
	if (param.source_ptr == NULL || param.dest_ptr == NULL)
		exit(1);
		
	work_buf = (char *)malloc(CMP_BUFFER_SIZE);
	if (work_buf == NULL)
		exit(1);
	
	type  = CMP_BINARY;
	dsize = 4096;
	
	param.source_offset = 0L;
	param.dest_offset = 0L;
	param.crc = (long) -1;
	implode(read_uncomp, write_comp, work_buf, &param, &type, &dsize); 
	param.orig_crc = ~param.crc;

	*size = param.comp_size;
	free(work_buf);
	free(buf);
	return (void *)param.dest_ptr;
}

void *uncompress(void *buf, long *size, long est_size)
{
	char *work_buf;
	unsigned int type;
	unsigned int dsize;
	unsigned int error;
	PARAM param;
	
	param.comp_size = *size;
	param.buf_size = param.comp_size;
	param.uncomp_size = 0L;
	param.source_ptr = (char *)buf;
	
	param.dest_ptr = (char *)malloc(est_size);
	
	if (param.source_ptr == NULL || param.dest_ptr == NULL)
		exit(1);
		
	work_buf = (char *)malloc(EXP_BUFFER_SIZE);
	if (work_buf == NULL)
		exit(1);
	
	type  = CMP_BINARY;
	dsize = 4096;
	
	param.source_offset = 0L;
	param.dest_offset = 0L;
	param.crc = (long) -1;
	error = explode(read_comp, write_uncomp, work_buf, &param);
	param.crc = ~param.crc;

	if(error)
		exit(1);

	*size = param.comp_size;
	free(work_buf);
	free(buf);
	return (void *)param.dest_ptr;	
}
//****************************************************************************

void main(void)
{
	FILE*fp;
	DWORD i,j;
	DWORD size;
	BYTE *tiles_ptr;
	long t;
	int r,g,b;
	long raw_size;
	char *raw_data;
	char *ptr;
	char *cmp_buf;

	size = FileSize(TILE_DB_NAME);
	tiles=(BYTE *)malloc(size);                   	
	if(tiles==NULL)
	{
		exit(0);
	}
	fp=fopen(TILE_DB_NAME,"rb");
	fread(tiles,(WORD)size,1,fp);
	fclose(fp);                   

	fp=fopen(PALETTE_NAME,"rb");
	fread(&pal,sizeof(PALETTE),1,fp);
	fclose(fp);     

	fp=fopen(NEW_TILES_NAME,"wb");

	il_hdr.width = 32;
	il_hdr.height = 32;
	il_hdr.frames = 113;
	il_hdr.pixel_size = 2;
	il_hdr.trans_colour = 0xFFDF;

	raw_size = (il_hdr.width * il_hdr.height) * il_hdr.frames * il_hdr.pixel_size;
	raw_data = (char *)malloc(raw_size);
	
	tiles_ptr = tiles;
	size/=16;
	ptr = raw_data;
	for(i=0;i<size;i++)
	{
		for(j=0;j<32;j+=2)
		{
			t = *tiles_ptr;

			r = pal.colour[t].red >> 3,
			g = pal.colour[t].green >> 3,
			b = pal.colour[t].blue >> 3;
					
			tile_sliver[j] = _RGB16BIT565(r,g,b);
			tile_sliver[j+1] = tile_sliver[j];
			tiles_ptr++;
		}
		memcpy(ptr, tile_sliver, sizeof(tile_sliver));
		ptr += sizeof(tile_sliver);
		memcpy(ptr, tile_sliver, sizeof(tile_sliver));
		ptr += sizeof(tile_sliver);
	}

	cmp_buf = (char *)compress(raw_data, &raw_size);

	il_hdr.comp_size = raw_size;
	fwrite(&il_hdr,sizeof(il_hdr),1,fp);

	fwrite(cmp_buf, raw_size,1,fp);

	fclose(fp);     
}