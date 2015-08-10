/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#ifndef INPUT_MASTER_H
#define INPUT_MASTER_H

#include "globals.h"

class INPUT_MASTER
{
	private:
		LPDIRECTINPUT lpdi;
		HINSTANCE main_instance;

	public:
		INPUT_MASTER(HINSTANCE main_instance);
		~INPUT_MASTER();
		LPDIRECTINPUT get_diobj(void);
};

#endif //INPUT_MASTER_H
