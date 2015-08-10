# Microsoft Developer Studio Project File - Name="COBOLsRevenge" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=COBOLsRevenge - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "COBOLsRevenge.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "COBOLsRevenge.mak" CFG="COBOLsRevenge - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "COBOLsRevenge - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "COBOLsRevenge - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/GAMES.SC/COBOLsRevenge", QGAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "COBOLsRevenge - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "final/"
# PROP Intermediate_Dir "junk/"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G5 /Gz /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"LIBC" /out:"Release/COBOL's Revenge.exe"

!ELSEIF  "$(CFG)" == "COBOLsRevenge - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "debug\"
# PROP Intermediate_Dir "debug\"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /G5 /Gz /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"LIBC" /nodefaultlib:"LIBCD" /out:"Debug/COBOL's Revenge.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "COBOLsRevenge - Win32 Release"
# Name "COBOLsRevenge - Win32 Debug"
# Begin Group "Library"

# PROP Default_Filter ".lib"
# Begin Source File

SOURCE=C:\mssdk\lib\ddraw.lib
# End Source File
# Begin Source File

SOURCE=C:\mssdk\lib\dxguid.lib
# End Source File
# Begin Source File

SOURCE=C:\Pkw32dcl\Implode.lib
# End Source File
# Begin Source File

SOURCE=C:\mssdk\lib\dsound.lib
# End Source File
# Begin Source File

SOURCE=C:\mssdk\lib\dinput.lib
# End Source File
# Begin Source File

SOURCE=C:\mssdk\lib\amstrmid.lib
# End Source File
# Begin Source File

SOURCE=C:\mssdk\lib\quartz.lib
# End Source File
# Begin Source File

SOURCE=C:\mssdk\lib\strmiids.lib
# End Source File
# End Group
# Begin Group "Resources"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\Script1.rc
# End Source File
# End Group
# Begin Group "Headers"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AUDIO_MIXER.h
# End Source File
# Begin Source File

SOURCE=.\ENGINE.h
# End Source File
# Begin Source File

SOURCE=.\ERRORS.h
# End Source File
# Begin Source File

SOURCE=.\FILE_LIBRARY.h
# End Source File
# Begin Source File

SOURCE=.\FONT.h
# End Source File
# Begin Source File

SOURCE=.\GAME_ENGINE.h
# End Source File
# Begin Source File

SOURCE=.\globals.h
# End Source File
# Begin Source File

SOURCE=.\IMAGE.h
# End Source File
# Begin Source File

SOURCE=.\IMAGE_LIBRARY.h
# End Source File
# Begin Source File

SOURCE=.\INPUT_MASTER.h
# End Source File
# Begin Source File

SOURCE=.\KEYBOARD.h
# End Source File
# Begin Source File

SOURCE=.\MAP_EDITOR.h
# End Source File
# Begin Source File

SOURCE=.\MENU.h
# End Source File
# Begin Source File

SOURCE=.\MENU_ITEM.h
# End Source File
# Begin Source File

SOURCE=.\MOUSE.h
# End Source File
# Begin Source File

SOURCE=.\MOVIE.h
# End Source File
# Begin Source File

SOURCE=.\MUSIC.h
# End Source File
# Begin Source File

SOURCE=.\PLAYER.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\SCREEN.h
# End Source File
# Begin Source File

SOURCE=.\SOUND_BITE.h
# End Source File
# Begin Source File

SOURCE=.\STATUS_PANEL.h
# End Source File
# Begin Source File

SOURCE=.\TALK_DB.h
# End Source File
# Begin Source File

SOURCE=.\TALK_SYSTEM.h
# End Source File
# Begin Source File

SOURCE=.\TILE_EDITOR.h
# End Source File
# Begin Source File

SOURCE=.\TILES.h
# End Source File
# Begin Source File

SOURCE=.\VIEW.h
# End Source File
# Begin Source File

SOURCE=.\WINDOW.h
# End Source File
# Begin Source File

SOURCE=.\WORLD.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AUDIO_MIXER.cpp
# End Source File
# Begin Source File

SOURCE=.\comments.cpp
# End Source File
# Begin Source File

SOURCE=.\ENGINE.cpp
# End Source File
# Begin Source File

SOURCE=.\ERRORS.cpp
# End Source File
# Begin Source File

SOURCE=.\FILE_LIBRARY.cpp
# End Source File
# Begin Source File

SOURCE=.\FONT.cpp
# End Source File
# Begin Source File

SOURCE=.\GAME_ENGINE.cpp
# End Source File
# Begin Source File

SOURCE=.\IMAGE.cpp
# End Source File
# Begin Source File

SOURCE=.\IMAGE_LIBRARY.cpp
# End Source File
# Begin Source File

SOURCE=.\INPUT_MASTER.cpp
# End Source File
# Begin Source File

SOURCE=.\KEYBOARD.cpp
# End Source File
# Begin Source File

SOURCE=.\mainwin.cpp
# End Source File
# Begin Source File

SOURCE=.\MAP_EDITOR.cpp
# End Source File
# Begin Source File

SOURCE=.\MENU.cpp
# End Source File
# Begin Source File

SOURCE=.\MENU_ITEM.cpp
# End Source File
# Begin Source File

SOURCE=.\MOUSE.cpp
# End Source File
# Begin Source File

SOURCE=.\MOVIE.cpp
# End Source File
# Begin Source File

SOURCE=.\MUSIC.cpp
# End Source File
# Begin Source File

SOURCE=.\PLAYER.cpp
# End Source File
# Begin Source File

SOURCE=.\SCREEN.cpp
# End Source File
# Begin Source File

SOURCE=.\SOUND_BITE.cpp
# End Source File
# Begin Source File

SOURCE=.\STATUS_PANEL.cpp
# End Source File
# Begin Source File

SOURCE=.\TALK_DB.cpp
# End Source File
# Begin Source File

SOURCE=.\TALK_SYSTEM.cpp
# End Source File
# Begin Source File

SOURCE=.\TILE_EDITOR.cpp
# End Source File
# Begin Source File

SOURCE=.\TILES.cpp
# End Source File
# Begin Source File

SOURCE=.\VIEW.cpp
# End Source File
# Begin Source File

SOURCE=.\WINDOW.cpp
# End Source File
# Begin Source File

SOURCE=.\WORLD.cpp
# End Source File
# End Group
# End Target
# End Project
