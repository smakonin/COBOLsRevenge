/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#include "tile_editor.h"
#include "map_editor.h"
#include "game_engine.h"

#define PROJECT_NAME                "COBOL's Revenge"
#define WINDOW_CLASS_NAME           "WINCLASS1"
#define TIMER_ANIM_INDEX            1
#define ANIM_SPEED                  50

HWND main_window_handle = NULL;
HINSTANCE hinstance_app = NULL;

TILE_EDITOR *te = NULL;
MAP_EDITOR *me = NULL;
GAME_ENGINE *ge = NULL;

long engine_to_use = 0;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow)
{
	WNDCLASSEX winclass;
	HWND hwnd;
	MSG	msg;
	char error[256];
	bool got_error;

	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra	= 0;
	winclass.cbWndExtra	= 0;
	winclass.hInstance = hinstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW); 
	winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = WINDOW_CLASS_NAME;
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&winclass))
		return(0);

	if(!(hwnd = CreateWindowEx(NULL, WINDOW_CLASS_NAME, PROJECT_NAME, WS_POPUP | WS_VISIBLE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL,	hinstance, NULL)))
		return(0);

	hinstance_app = hinstance;
	main_window_handle = hwnd;
	got_error  = false;

	ShowCursor(FALSE);

	try
	{
		if(!strcmp(lpcmdline, "te"))
		{
			te = new TILE_EDITOR(main_window_handle, hinstance_app);
			engine_to_use = 1;
		}
		else if(!strcmp(lpcmdline, "me"))
		{
			me = new MAP_EDITOR(main_window_handle, hinstance_app);
			engine_to_use = 2;
		}
		else
		{
			ge = new GAME_ENGINE(main_window_handle, hinstance_app);
			engine_to_use = 3;
		}
	}
	catch(ERRORS e)
	{	
		strcpy(error, e.get_message());
		got_error  = true;
	}

	if(!got_error)
	{
		while(true)
		{
			if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{ 
				if (msg.message == WM_QUIT)
					break;
				
				TranslateMessage(&msg);
				
				DispatchMessage(&msg);
			}

			switch(engine_to_use)
			{
				case 1:
					got_error = te->update(error);
					break;
				case 2:
					got_error = me->update(error);
					break;
				case 3:
					got_error = ge->update(error);
					break;
			}
			if(got_error)
				SendMessage(main_window_handle, WM_CLOSE, 0, 0);
		}
	}

	ShowCursor(TRUE);

	if(got_error)
		MessageBox(main_window_handle, error, PROJECT_NAME, MB_ICONEXCLAMATION | MB_OK);

	return(msg.wParam);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch(msg)
	{	
		case WM_CREATE: 
			SetTimer(hwnd, TIMER_ANIM_INDEX, ANIM_SPEED, NULL);
			return 0;
			break;
			
		case WM_TIMER:
			switch(wparam)
            {
				case TIMER_ANIM_INDEX:
					switch(engine_to_use)
					{
						case 1:
							/////te->incroment_counters();
							break;
						case 2:
							/////me->incroment_counters();
							break;
						case 3:
							////ge->incroment_counters();
							break;
					}
					break;
			}
			return 0;
			break;

		case WM_PAINT: 
			hdc = BeginPaint(hwnd, &ps);	 
			EndPaint(hwnd, &ps);
			return 0;
			break;
			
		case WM_DESTROY: 
			KillTimer(hwnd, TIMER_ANIM_INDEX);
			PostQuitMessage(0);
			return 0;
			break;
			
		default:
			break;
    }
	
	return DefWindowProc(hwnd, msg, wparam, lparam);
	
}
