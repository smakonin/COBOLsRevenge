/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#ifndef ERRORS_H
#define ERRORS_H

#include "globals.h"

class ERRORS
{
	private:
		long id;
		char message[8888];

	public:
		ERRORS(long id);
		ERRORS(long id, const char *str);
		ERRORS(const char *str);
		ERRORS(const char *str1, const char *str2);
		~ERRORS();
		const char *get_message(void);
};

#endif //ERRORS_H
