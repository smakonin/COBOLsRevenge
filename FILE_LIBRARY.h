/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#ifndef PKZIP_H
#define PKZIP_H

#include "globals.h"

class COMPRESS_PARAM
{
	public:
		char *source_ptr;
		char *dest_ptr;
		long source_offset;
		long dest_offset;
		long comp_size;
		long uncomp_size;
		long buf_size;
		long crc;
		long orig_crc;
};

class FILE_LIBRARY 
{
	public:
		void *compress(void *buf, long *size);
		void *uncompress(void *buf, long *size, long est_size);
		long file_size(char *filename);
		bool does_file_exist(char *filename);
};

#endif //PKZIP_H
