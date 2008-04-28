# Microsoft Developer Studio Project File - Name="DccNiTghtmare" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=DccNiTghtmare - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DccNiTghtmare.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DccNiTghtmare.mak" CFG="DccNiTghtmare - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DccNiTghtmare - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "DccNiTghtmare - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DccNiTghtmare - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x416 /d "NDEBUG"
# ADD RSC /l 0x416 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib SDL.lib SDL_image.lib opengl32.lib openal32.lib cal3d.lib ogg.lib glu32.lib vorbis.lib vorbisenc.lib vorbisfile.lib sdlmain.lib intl.lib SDL_ttf.lib /nologo /subsystem:windows /machine:I386 /out:"bin/DccNiTghtmare.exe"

!ELSEIF  "$(CFG)" == "DccNiTghtmare - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "C:\Arquivos de programas\Microsoft Visual Studio\VC98\Include\SDL" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x416 /d "_DEBUG"
# ADD RSC /l 0x416 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib SDL.lib SDL_image.lib opengl32.lib openal32.lib cal3d.lib ogg.lib glu32.lib vorbis.lib vorbisenc.lib vorbisfile.lib sdlmain.lib /nologo /subsystem:windows /debug /machine:I386 /out:"bin/DccNiTghtmare.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "DccNiTghtmare - Win32 Release"
# Name "DccNiTghtmare - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\classes\actions.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ia\agent.cpp
# End Source File
# Begin Source File

SOURCE=.\src\classes\align.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\alignwindow.cpp
# End Source File
# Begin Source File

SOURCE=.\src\etc\animodel.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\aspectwindow.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ia\astar.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\attwindow.cpp
# End Source File
# Begin Source File

SOURCE=.\src\classes\barter.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\barterwindow.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gui\button.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\camera.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\character.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\charwindow.cpp
# End Source File
# Begin Source File

SOURCE=.\src\classes\classes.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\classwindow.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\collision.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gui\color.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\culling.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\cursor.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gui\cxsel.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\dccnit.cpp
# End Source File
# Begin Source File

SOURCE=.\src\classes\defs.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\src\etc\dirs.cpp
# End Source File
# Begin Source File

SOURCE=.\src\etc\distance.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gui\dntfont.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gui\draw.cpp
# End Source File
# Begin Source File

SOURCE=.\src\etc\extensions.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gui\farso.cpp
# End Source File
# Begin Source File

SOURCE=.\src\classes\feats.cpp
# End Source File
# Begin Source File

SOURCE=.\src\fight\fightGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\src\fight\fightSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gui\filesel.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ia\fuzzy.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ia\fuzzyfunction.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ia\fuzzyrule.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ia\fuzzyvariable.cpp
# End Source File
# Begin Source File

SOURCE=.\src\particle\grass.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gui\guilist.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gui\healthBar.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ia\iafuncs.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ia\iascript.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ia\iastack.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ia\iavariable.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\initial.cpp
# End Source File
# Begin Source File

SOURCE=.\src\fight\initiative.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gui\interface.cpp
# End Source File
# Begin Source File

SOURCE=.\src\classes\inventory.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\inventwindow.cpp
# End Source File
# Begin Source File

SOURCE=.\src\classes\itemslot.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\itemwindow.cpp
# End Source File
# Begin Source File

SOURCE=.\src\particle\lake.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gui\listtext.cpp
# End Source File
# Begin Source File

SOURCE=.\src\map\map.cpp
# End Source File
# Begin Source File

SOURCE=.\src\map\mapfog.cpp
# End Source File
# Begin Source File

SOURCE=.\src\map\maplights.cpp
# End Source File
# Begin Source File

SOURCE=.\src\map\mapobject.cpp
# End Source File
# Begin Source File

SOURCE=.\src\map\maproad.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gui\menu.cpp
# End Source File
# Begin Source File

SOURCE=.\src\etc\message3d.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gui\messages.cpp
# End Source File
# Begin Source File

SOURCE=.\src\particle\meteor.cpp
# End Source File
# Begin Source File

SOURCE=.\src\classes\mission.cpp
# End Source File
# Begin Source File

SOURCE=.\src\etc\modellist.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\modstate.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gui\mouse.cpp
# End Source File
# Begin Source File

SOURCE=.\src\classes\object.cpp
# End Source File
# Begin Source File

SOURCE=.\src\sound\ogg_stream.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\options.cpp
# End Source File
# Begin Source File

SOURCE=.\src\etc\overlaps.cpp
# End Source File
# Begin Source File

SOURCE=.\src\particle\part1.cpp
# End Source File
# Begin Source File

SOURCE=.\src\particle\part2.cpp
# End Source File
# Begin Source File

SOURCE=.\src\particle\part3.cpp
# End Source File
# Begin Source File

SOURCE=.\src\particle\part4.cpp
# End Source File
# Begin Source File

SOURCE=.\src\particle\part5.cpp
# End Source File
# Begin Source File

SOURCE=.\src\particle\part6.cpp
# End Source File
# Begin Source File

SOURCE=.\src\particle\part7.cpp
# End Source File
# Begin Source File

SOURCE=.\src\particle\partcontroller.cpp
# End Source File
# Begin Source File

SOURCE=.\src\particle\particle.cpp
# End Source File
# Begin Source File

SOURCE=.\src\particle\partlist.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ia\pattAgent.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gui\picture.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ia\potentAgent.cpp
# End Source File
# Begin Source File

SOURCE=.\src\classes\race.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\racewindow.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gui\rolbar.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gui\seltext.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\shadow.cpp
# End Source File
# Begin Source File

SOURCE=.\src\classes\skills.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\skillwindow.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\sky.cpp
# End Source File
# Begin Source File

SOURCE=.\src\sound\sndfx.cpp
# End Source File
# Begin Source File

SOURCE=.\src\sound\sound.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\sun.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gui\tabbutton.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\teste.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gui\textbar.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gui\textbox.cpp
# End Source File
# Begin Source File

SOURCE=.\src\classes\thing.cpp
# End Source File
# Begin Source File

SOURCE=.\src\lang\translate.cpp
# End Source File
# Begin Source File

SOURCE=.\src\etc\userinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\util.cpp
# End Source File
# Begin Source File

SOURCE=.\src\particle\wave.cpp
# End Source File
# Begin Source File

SOURCE=.\src\classes\weapon.cpp
# End Source File
# Begin Source File

SOURCE=.\src\particle\wind.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gui\window.cpp
# End Source File
# Begin Source File

SOURCE=.\src\classes\xp.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\classes\actions.h
# End Source File
# Begin Source File

SOURCE=.\src\ia\agent.h
# End Source File
# Begin Source File

SOURCE=.\src\classes\align.h
# End Source File
# Begin Source File

SOURCE=.\src\engine\alignwindow.h
# End Source File
# Begin Source File

SOURCE=.\src\etc\animodel.h
# End Source File
# Begin Source File

SOURCE=.\src\engine\aspectwindow.h
# End Source File
# Begin Source File

SOURCE=.\src\ia\astar.h
# End Source File
# Begin Source File

SOURCE=.\src\engine\attwindow.h
# End Source File
# Begin Source File

SOURCE=.\src\classes\barter.h
# End Source File
# Begin Source File

SOURCE=.\src\engine\barterwindow.h
# End Source File
# Begin Source File

SOURCE=.\src\gui\button.h
# End Source File
# Begin Source File

SOURCE=.\src\engine\camera.h
# End Source File
# Begin Source File

SOURCE=.\src\engine\character.h
# End Source File
# Begin Source File

SOURCE=.\src\engine\charwindow.h
# End Source File
# Begin Source File

SOURCE=.\src\classes\classes.h
# End Source File
# Begin Source File

SOURCE=.\src\engine\classwindow.h
# End Source File
# Begin Source File

SOURCE=.\src\engine\collision.h
# End Source File
# Begin Source File

SOURCE=.\src\gui\color.h
# End Source File
# Begin Source File

SOURCE=.\src\config_win.h
# End Source File
# Begin Source File

SOURCE=.\src\engine\culling.h
# End Source File
# Begin Source File

SOURCE=.\src\engine\cursor.h
# End Source File
# Begin Source File

SOURCE=.\src\engine\dccnit.h
# End Source File
# Begin Source File

SOURCE=.\src\classes\defs.h
# End Source File
# Begin Source File

SOURCE=.\src\engine\dialog.h
# End Source File
# Begin Source File

SOURCE=.\src\classes\dices.h
# End Source File
# Begin Source File

SOURCE=.\src\etc\dirs.h
# End Source File
# Begin Source File

SOURCE=.\src\etc\distance.h
# End Source File
# Begin Source File

SOURCE=.\src\gui\dntfont.h
# End Source File
# Begin Source File

SOURCE=.\src\gui\draw.h
# End Source File
# Begin Source File

SOURCE=.\src\etc\extensions.h
# End Source File
# Begin Source File

SOURCE=.\src\gui\farso.h
# End Source File
# Begin Source File

SOURCE=.\src\classes\feats.h
# End Source File
# Begin Source File

SOURCE=.\src\fight\fightGroup.h
# End Source File
# Begin Source File

SOURCE=.\src\fight\fightSystem.h
# End Source File
# Begin Source File

SOURCE=.\src\gui\filesel.h
# End Source File
# Begin Source File

SOURCE=.\src\ia\fuzzy.h
# End Source File
# Begin Source File

SOURCE=.\src\ia\fuzzyfunction.h
# End Source File
# Begin Source File

SOURCE=.\src\ia\fuzzyrule.h
# End Source File
# Begin Source File

SOURCE=.\src\ia\fuzzyvariable.h
# End Source File
# Begin Source File

SOURCE=.\src\particle\grass.h
# End Source File
# Begin Source File

SOURCE=.\src\gui\guilist.h
# End Source File
# Begin Source File

SOURCE=.\src\gui\guiobject.h
# End Source File
# Begin Source File

SOURCE=.\src\gui\healthBar.h
# End Source File
# Begin Source File

SOURCE=.\src\ia\iafuncs.h
# End Source File
# Begin Source File

SOURCE=.\src\ia\iascript.h
# End Source File
# Begin Source File

SOURCE=.\src\ia\iastack.h
# End Source File
# Begin Source File

SOURCE=.\src\ia\iavariable.h
# End Source File
# Begin Source File

SOURCE=.\src\engine\initial.h
# End Source File
# Begin Source File

SOURCE=.\src\fight\initiative.h
# End Source File
# Begin Source File

SOURCE=.\src\gui\interface.h
# End Source File
# Begin Source File

SOURCE=.\src\classes\inventory.h
# End Source File
# Begin Source File

SOURCE=.\src\engine\inventwindow.h
# End Source File
# Begin Source File

SOURCE=.\src\engine\itemwindow.h
# End Source File
# Begin Source File

SOURCE=.\src\gui\janela.h
# End Source File
# Begin Source File

SOURCE=.\src\particle\lake.h
# End Source File
# Begin Source File

SOURCE=.\src\gui\lista.h
# End Source File
# Begin Source File

SOURCE=.\src\gui\listtext.h
# End Source File
# Begin Source File

SOURCE=.\src\map\map.h
# End Source File
# Begin Source File

SOURCE=.\src\map\mapfog.h
# End Source File
# Begin Source File

SOURCE=.\src\map\maplights.h
# End Source File
# Begin Source File

SOURCE=.\src\map\mapobject.h
# End Source File
# Begin Source File

SOURCE=.\src\map\mapobjeto.h
# End Source File
# Begin Source File

SOURCE=.\src\map\maproad.h
# End Source File
# Begin Source File

SOURCE=.\src\gui\menu.h
# End Source File
# Begin Source File

SOURCE=.\src\etc\message3d.h
# End Source File
# Begin Source File

SOURCE=.\src\gui\messages.h
# End Source File
# Begin Source File

SOURCE=.\src\particle\meteor.h
# End Source File
# Begin Source File

SOURCE=.\src\classes\mission.h
# End Source File
# Begin Source File

SOURCE=.\src\etc\modellist.h
# End Source File
# Begin Source File

SOURCE=.\src\classes\modifier.h
# End Source File
# Begin Source File

SOURCE=.\src\engine\modstate.h
# End Source File
# Begin Source File

SOURCE=.\src\gui\mouse.h
# End Source File
# Begin Source File

SOURCE=.\src\classes\object.h
# End Source File
# Begin Source File

SOURCE=.\src\gui\objeto.h
# End Source File
# Begin Source File

SOURCE=.\src\sound\ogg_stream.h
# End Source File
# Begin Source File

SOURCE=.\src\engine\options.h
# End Source File
# Begin Source File

SOURCE=.\src\etc\overlaps.h
# End Source File
# Begin Source File

SOURCE=.\src\particle\part1.h
# End Source File
# Begin Source File

SOURCE=.\src\particle\part2.h
# End Source File
# Begin Source File

SOURCE=.\src\particle\part3.h
# End Source File
# Begin Source File

SOURCE=.\src\particle\part4.h
# End Source File
# Begin Source File

SOURCE=.\src\particle\part5.h
# End Source File
# Begin Source File

SOURCE=.\src\particle\part6.h
# End Source File
# Begin Source File

SOURCE=.\src\particle\part7.h
# End Source File
# Begin Source File

SOURCE=.\src\particle\partcontroller.h
# End Source File
# Begin Source File

SOURCE=.\src\particle\particle.h
# End Source File
# Begin Source File

SOURCE=.\src\particle\partlist.h
# End Source File
# Begin Source File

SOURCE=.\src\ia\pattAgent.h
# End Source File
# Begin Source File

SOURCE=.\src\engine\personagens.h
# End Source File
# Begin Source File

SOURCE=.\src\gui\picture.h
# End Source File
# Begin Source File

SOURCE=.\src\ia\potentAgent.h
# End Source File
# Begin Source File

SOURCE=.\src\classes\race.h
# End Source File
# Begin Source File

SOURCE=.\src\engine\racewindow.h
# End Source File
# Begin Source File

SOURCE=.\src\gui\seltext.h
# End Source File
# Begin Source File

SOURCE=.\src\engine\shadow.h
# End Source File
# Begin Source File

SOURCE=.\src\classes\skills.h
# End Source File
# Begin Source File

SOURCE=.\src\engine\skillwindow.h
# End Source File
# Begin Source File

SOURCE=.\src\engine\sky.h
# End Source File
# Begin Source File

SOURCE=.\src\sound\sndfx.h
# End Source File
# Begin Source File

SOURCE=.\src\sound\sound.h
# End Source File
# Begin Source File

SOURCE=.\src\engine\sun.h
# End Source File
# Begin Source File

SOURCE=.\src\gui\tabbutton.h
# End Source File
# Begin Source File

SOURCE=.\src\gui\textbar.h
# End Source File
# Begin Source File

SOURCE=.\src\gui\textbox.h
# End Source File
# Begin Source File

SOURCE=.\src\classes\thing.h
# End Source File
# Begin Source File

SOURCE=.\src\lang\translate.h
# End Source File
# Begin Source File

SOURCE=.\src\etc\userinfo.h
# End Source File
# Begin Source File

SOURCE=.\src\engine\util.h
# End Source File
# Begin Source File

SOURCE=.\src\particle\wave.h
# End Source File
# Begin Source File

SOURCE=.\src\classes\weapon.h
# End Source File
# Begin Source File

SOURCE=.\src\particle\wind.h
# End Source File
# Begin Source File

SOURCE=.\src\gui\window.h
# End Source File
# Begin Source File

SOURCE=.\src\classes\xp.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
