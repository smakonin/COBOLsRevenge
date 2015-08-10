/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include "globals.h"

INPUT_MASTER::INPUT_MASTER(HINSTANCE main_instance)
{
	lpdi = NULL;
	INPUT_MASTER::main_instance = main_instance;

	if(DirectInputCreate(main_instance, DIRECTINPUT_VERSION, &lpdi, NULL) != DI_OK)
		throw ERROR("Unable to create Direct Input");
}

INPUT_MASTER::~INPUT_MASTER()
{
	if(lpdi)
		lpdi->Release();
}

LPDIRECTINPUT INPUT_MASTER::get_diobj(void)
{
	return lpdi;
}
