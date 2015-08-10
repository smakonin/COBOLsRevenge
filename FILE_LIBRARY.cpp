/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include "globals.h"

unsigned int read_uncomp(char *buf, unsigned int *size, void *param)
{
	COMPRESS_PARAM *ptr = (COMPRESS_PARAM *)param;
	
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
	COMPRESS_PARAM *ptr = (COMPRESS_PARAM *)param;
	
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
	COMPRESS_PARAM *ptr = (COMPRESS_PARAM *)param;
	
	if (ptr->comp_size + (long)*size > ptr->buf_size)
		throw ERRORS("Compressed data will overflow buffer. Increase size of buffer!");

	memcpy(ptr->dest_ptr + ptr->dest_offset, buf, *size);
	ptr->dest_offset += (long)*size;
	ptr->comp_size += (long)*size;
}

void write_uncomp(char *buf, unsigned int *size, void *param)
{
	COMPRESS_PARAM *ptr = (COMPRESS_PARAM *)param;
	
	if (ptr->comp_size + (long)*size > ptr->buf_size)
	   throw ERRORS("Uncompressed data will overflow buffer. Increase size of buffer!");

	memcpy(ptr->dest_ptr + ptr->dest_offset, buf, *size);
	ptr->dest_offset += (long)*size;
	ptr->uncomp_size += (long)*size;
	ptr->crc = crc32(buf, size, (unsigned long *)&ptr->crc);
}

void *FILE_LIBRARY::compress(void *buf, long *size)
{
	char *work_buf;
	unsigned int type;
	unsigned int dsize;
	COMPRESS_PARAM param;
	
	param.uncomp_size = *size;
	param.buf_size = param.uncomp_size;
	param.comp_size = 0L;
	param.source_ptr = (char *)buf;
	
	param.dest_ptr = new char[param.buf_size];
	
	if (param.source_ptr == NULL || param.dest_ptr == NULL)
		throw ERRORS("Unable to allocate compress buffers.");
		
	work_buf = new char[CMP_BUFFER_SIZE];
	if (work_buf == NULL)
		throw ERRORS("Unable to allocate compress work buffer.");
	
	type  = CMP_BINARY;
	dsize = 4096;
	
	param.source_offset = 0L;
	param.dest_offset = 0L;
	param.crc = (long) -1;
	implode(read_uncomp, write_comp, work_buf, &param, &type, &dsize); 
	param.orig_crc = ~param.crc;

	*size = param.comp_size;
	delete work_buf;
	delete buf;
	return (void *)param.dest_ptr;
}

void *FILE_LIBRARY::uncompress(void *buf, long *size, long est_size)
{
	char *work_buf;
	unsigned int type;
	unsigned int dsize;
	unsigned int error;
	COMPRESS_PARAM param;
	
	param.comp_size = *size;
	param.buf_size = est_size;
	param.uncomp_size = 0L;
	param.source_ptr = (char *)buf;
	
	param.dest_ptr = new char[est_size];
	
	if (param.source_ptr == NULL || param.dest_ptr == NULL)
		throw ERRORS("Unable to allocate uncompress buffers.");
		
	work_buf = new char[EXP_BUFFER_SIZE];
	if (work_buf == NULL)
		throw ERRORS("Unable to allocate uncompress work buffer.");
	
	type  = CMP_BINARY;
	dsize = 4096;
	
	param.source_offset = 0L;
	param.dest_offset = 0L;
	param.crc = (long) -1;
	error = explode(read_comp, write_uncomp, work_buf, &param);
	param.crc = ~param.crc;

	if(error)
		throw ERRORS("Error in compressed data!");

	*size = param.comp_size;
	delete work_buf;
	return (void *)param.dest_ptr;	
}

long FILE_LIBRARY::file_size(char *filename)
{
	long length;
	int file;

	file = open(filename, O_RDONLY + O_BINARY);
	length = filelength(file);
	close(file);

	return(length);
} 

bool FILE_LIBRARY::does_file_exist(char *filename)
{
	_finddata_t data;

	if(_findfirst(filename, &data) == -1)
		return false;
	else
		return true;
}
