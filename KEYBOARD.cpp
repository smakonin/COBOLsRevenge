/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include "globals.h"

KEYBOARD::KEYBOARD(INPUT_MASTER *input_master, HWND window_handle)
{
	KEYBOARD::input_master = input_master;
	lpdikeyboard = NULL;
	KEYBOARD::window_handle = window_handle;

	if(input_master->get_diobj()->CreateDevice(GUID_SysKeyboard, &lpdikeyboard, NULL) != DI_OK)
		throw ERROR("Unable to create DInput Keyboard");

	if(lpdikeyboard->SetCooperativeLevel(window_handle, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND) != DI_OK)
		throw ERROR("Unable to Set Keyboard Cooperative Level");

	if(lpdikeyboard->SetDataFormat(&c_dfDIKeyboard) != DI_OK)
		throw ERROR("Unable to Set Keyboard Data Format");

	if(lpdikeyboard->Acquire() != DI_OK)
		throw ERROR("Unable to Acquire Keyboard");

	memset(keys_down, 0, sizeof(keys_down));
}

KEYBOARD::~KEYBOARD()
{
	if(lpdikeyboard)
	{
		lpdikeyboard->Unacquire();
		lpdikeyboard->Release();
	}
}

void KEYBOARD::update(void)
{
	lpdikeyboard->GetDeviceState(256, (LPVOID)keyboard_state);
}

bool KEYBOARD::was_pressed(long key, bool menu_select)
{
	bool cur_state;

	cur_state = (keyboard_state[key] & 0x80) ? true : false;

	if(menu_select)
	{
		if(keys_down[key] && !cur_state)
		{
			keys_down[key] = false;
			return true;
		}
		else if(!keys_down[key] && cur_state)
		{
			keys_down[key] = true;
			return false;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return cur_state;
	}
}

char KEYBOARD::getch(void)
{
	if(was_pressed(DIK_A))
		return 'A';
	else if(was_pressed(DIK_B))
		return 'B';
	else if(was_pressed(DIK_C))
		return 'C';
	else if(was_pressed(DIK_D))
		return 'D';
	else if(was_pressed(DIK_E))
		return 'E';
	else if(was_pressed(DIK_F))
		return 'F';
	else if(was_pressed(DIK_G))
		return 'G';
	else if(was_pressed(DIK_H))
		return 'H';
	else if(was_pressed(DIK_I))
		return 'I';
	else if(was_pressed(DIK_J))
		return 'J';
	else if(was_pressed(DIK_K))
		return 'K';
	else if(was_pressed(DIK_L))
		return 'L';
	else if(was_pressed(DIK_M))
		return 'M';
	else if(was_pressed(DIK_N))
		return 'N';
	else if(was_pressed(DIK_O))
		return 'O';
	else if(was_pressed(DIK_P))
		return 'P';
	else if(was_pressed(DIK_Q))
		return 'Q';
	else if(was_pressed(DIK_R))
		return 'R';
	else if(was_pressed(DIK_S))
		return 'S';
	else if(was_pressed(DIK_T))
		return 'T';
	else if(was_pressed(DIK_U))
		return 'U';
	else if(was_pressed(DIK_V))
		return 'V';
	else if(was_pressed(DIK_W))
		return 'W';
	else if(was_pressed(DIK_X))
		return 'X';
	else if(was_pressed(DIK_Y))
		return 'Y';
	else if(was_pressed(DIK_Z))
		return 'Z';
	else if(was_pressed(DIK_1))
		return '1';
	else if(was_pressed(DIK_2))
		return '2';
	else if(was_pressed(DIK_3))
		return '3';
	else if(was_pressed(DIK_4))
		return '4';
	else if(was_pressed(DIK_5))
		return '5';
	else if(was_pressed(DIK_6))
		return '6';
	else if(was_pressed(DIK_7))
		return '7';
	else if(was_pressed(DIK_8))
		return '8';
	else if(was_pressed(DIK_9))
		return '9';
	else if(was_pressed(DIK_0))
		return '0';	
	else if(was_pressed(DIK_PERIOD))
		return '.';
	else if(was_pressed(DIK_RETURN))
		return '\r';
	else if(was_pressed(DIK_BACKSLASH))
		return '\\';
	else if(was_pressed(DIK_BACKSPACE))
		return '\b';
	else
		return 0;
}