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

#define WORLD_NAME "world.cr"
#define NEW_WORLD_NAME "wrlddat.cr"
#define _RGB16BIT555(r, g, b)       ((b % 32) + ((g % 32) << 5) + ((r % 32) << 10))
#define _RGB16BIT565(r, g, b)       ((b % 32) + ((g % 64) << 6) + ((r % 32) << 11))
#define _RGB32BIT(a,r,g,b)          ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))

typedef unsigned char 	BYTE;
typedef unsigned short 	WORD;
typedef unsigned long 	DWORD;

typedef struct WRD_LIB_TYPE
{
	char name[64];
	long width;
	long height;
	long levels;
	long tile_size;
	long comp_size;
}WRD_LIB_TYPE,*WRD_LIB_TYPE_PTR;

BYTE *map;								
WRD_LIB_TYPE il_hdr;

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
	DWORD size;
	long raw_size;
	char *cmp_buf;

	size = FileSize(WORLD_NAME);
	map =(BYTE *)malloc(size);                   	
	if(map==NULL)
	{
		exit(0);
	}
	fp=fopen(WORLD_NAME,"rb");
	fread(map,(WORD)size,1,fp);
	fclose(fp);                   

	fp=fopen(NEW_WORLD_NAME,"wb");

	memset(il_hdr.name, 0, sizeof(il_hdr.name));
	strcpy(il_hdr.name, "IBUM");
	il_hdr.width = 64;
	il_hdr.height = 64;
	il_hdr.levels = 8;
	il_hdr.tile_size = 1;

	raw_size = size;
	
	cmp_buf = (char *)compress(map, &raw_size);

	il_hdr.comp_size = raw_size;
	fwrite(&il_hdr,sizeof(il_hdr),1,fp);

	fwrite(cmp_buf, raw_size,1,fp);

	fclose(fp);     
}