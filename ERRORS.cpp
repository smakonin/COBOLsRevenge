/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include "globals.h"

extern "C"
{
	HWND main_window_handle;
	HINSTANCE hinstance_app;
}

ERRORS::ERRORS(long id, const char *str)
{
	char res_buf[7777];

	ERRORS::id = id;
	LoadString(hinstance_app, id, res_buf, sizeof(res_buf) - 1);
	sprintf(message, res_buf, str);	
}

ERRORS::ERRORS(long id)
{
	char res_buf[7777];

	ERRORS::id = id;
	LoadString(hinstance_app, id, res_buf, sizeof(res_buf) - 1);
	strcpy(message, res_buf);	
}

ERRORS::ERRORS(const char *str)
{
	strcpy(message, str);	
}

ERRORS::ERRORS(const char *str1, const char *str2)
{
	sprintf(message, str1, str2);	
}

ERRORS::~ERRORS()
{
	memset(message, 0, sizeof(message));
}

const char *ERRORS::get_message(void)
{
	return message;
}


