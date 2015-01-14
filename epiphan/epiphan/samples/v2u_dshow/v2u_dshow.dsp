# Microsoft Developer Studio Project File - Name="v2u_dshow" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=v2u_dshow - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "v2u_dshow.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "v2u_dshow.mak" CFG="v2u_dshow - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "v2u_dshow - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "v2u_dshow - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "v2u_dshow - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "build/release"
# PROP BASE Intermediate_Dir "build/release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "build/release"
# PROP Intermediate_Dir "build/release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../include" /I "../BaseClasses" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "STRSAFE_NO_DEPRECATE" /D "_WIN32_DCOM" /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 winmm.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "v2u_dshow - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "build/debug"
# PROP BASE Intermediate_Dir "build/debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "build/debug"
# PROP Intermediate_Dir "build/debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../include" /I "../BaseClasses" /D "_DEBUG" /D DEBUG=1 /D "WIN32" /D "_CONSOLE" /D "STRSAFE_NO_DEPRECATE" /D "_WIN32_DCOM" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "v2u_dshow - Win32 Release"
# Name "v2u_dshow - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\v2u_dshow.cpp
# End Source File
# Begin Source File

SOURCE=.\v2u_dshow_config_impl.cpp
# End Source File
# Begin Source File

SOURCE=.\v2u_dshow_uuid.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=..\..\include\v2u_defs.h
# End Source File
# Begin Source File

SOURCE=..\..\include\v2u_dshow.h
# End Source File
# Begin Source File

SOURCE=.\v2u_dshow_config.h
# End Source File
# Begin Source File

SOURCE=.\v2u_dshow_config_impl.h
# End Source File
# End Group
# Begin Group "BaseClasses"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\BaseClasses\amvideo.cpp
# End Source File
# Begin Source File

SOURCE=..\BaseClasses\combase.cpp
# End Source File
# Begin Source File

SOURCE=..\BaseClasses\mtype.cpp
# End Source File
# Begin Source File

SOURCE=..\BaseClasses\wxdebug.cpp
# End Source File
# Begin Source File

SOURCE=..\BaseClasses\wxlist.cpp
# End Source File
# Begin Source File

SOURCE=..\BaseClasses\wxutil.cpp
# End Source File
# End Group
# End Target
# End Project
