/**********************************************************************
*
*	This file is Copyright (C) 1998-2000, Stephen Makonin.
*	All Rights Reserved.
*
**********************************************************************/

#ifndef GLOBALS_H
#define GLOBALS_H

#define WIN32_LEAN_AND_MEAN  
#define DIRECTINPUT_VERSION 0x0700

#include <windows.h>  
#include <process.h> 
#include <windowsx.h> 
#include <mmsystem.h>
#include <iostream.h> 
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h> 
#include <math.h>
#include <io.h>
#include <fcntl.h>
#include <direct.h>

#include <strmif.h>
#include <uuids.h>
#include <control.h>

#include <ddraw.h>
#include <dsound.h>
#include <dmksctrl.h>
#include <dmusici.h>
#include <dmusicc.h>
#include <dmusicf.h>
#include <dinput.h>

#include <implode.h>

#define SCREEN_WIDTH                640  
#define SCREEN_HEIGHT               480
#define FPS_15                      66
#define FPS_30                      33
#define FPS_60                      16
#define _RGB_BLACK                  0x00000000
#define _RGB_WHITE                  0x00FFFFFF

const double PI = 3.1415926535;

#define _RGB16BIT555(r,g,b)         (((b)%32) + (((g)%32) << 5) + (((r)%32) << 10))
#define _RGB16BIT565(r,g,b)         (((b)%32) + (((g)%64) << 6) + (((r)%32) << 11))
#define _RGB32BIT(a,r,g,b)          ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))
#define _RGB32RED(c)                (((BYTE *)&c)[2])
#define _RGB32GREEN(c)              (((BYTE *)&c)[1])
#define _RGB32BLUE(c)               (((BYTE *)&c)[0])
#define LIMIT(val,l,h)              ((val) < (l) ? (l) : (val) > (h) ? (h) : (val))
#define DDRAW_INIT_STRUCT(ddstruct) {memset(&ddstruct, 0, sizeof(ddstruct)); ddstruct.dwSize = sizeof(ddstruct);}

#include "resource.h"
#include "errors.h"
#include "file_library.h"
#include "input_master.h"
#include "keyboard.h"
#include "audio_mixer.h"
#include "sound_bite.h"
#include "music.h"
#include "image.h"
#include "screen.h"
#include "mouse.h"
#include "world.h"
#include "image_library.h"
#include "tiles.h"
#include "player.h"
#include "view.h"
#include "movie.h"
#include "font.h"
#include "window.h"
#include "menu_item.h"
#include "menu.h"
#include "talk_db.h"
#include "status_panel.h"
#include "talk_system.h"

#endif //GLOBALS_H