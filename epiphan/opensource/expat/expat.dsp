# Microsoft Developer Studio Project File - Name="expat" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=expat - Win32 Debug Unicode
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "expat.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "expat.mak" CFG="expat - Win32 Debug Unicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "expat - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "expat - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "expat - Win32 Debug Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE "expat - Win32 Release Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "expat - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "build/win32/i386/release"
# PROP BASE Intermediate_Dir "build/win32/i386/release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "build/win32/i386/release"
# PROP Intermediate_Dir "build/win32/i386/release"
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "." /I "lib" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "XML_UNICODE" /D "XML_DTD" /D "XML_NS" /D "_STATIC" /D "HAVE_EXPAT_CONFIG_H" /D "XML_STATIC" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"build/win32/i386/expat.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "expat - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "build/win32/i386/debug"
# PROP BASE Intermediate_Dir "build/win32/i386/debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "build/win32/i386/debug"
# PROP Intermediate_Dir "build/win32/i386/debug"
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /Z7 /Od /I "." /I "lib" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "XML_UNICODE" /D "XML_DTD" /D "XML_NS" /D "_STATIC" /D "HAVE_EXPAT_CONFIG_H" /D "XML_STATIC" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"build/win32/i386/expat.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "expat - Win32 Debug Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "build/win32/i386/debug"
# PROP BASE Intermediate_Dir "build/win32/i386/debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "build/win32/i386/debug"
# PROP Intermediate_Dir "build/win32/i386/debug"
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MDd /W3 /GX /Z7 /Od /I "." /I "lib" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "XML_UNICODE" /D "XML_DTD" /D "XML_NS" /D "_STATIC" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /Z7 /Od /I "." /I "lib" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "XML_UNICODE" /D "XML_DTD" /D "XML_NS" /D "_STATIC" /D "HAVE_EXPAT_CONFIG_H" /D "XML_STATIC" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"build/win32/i386/expat.bsc"
# ADD BSC32 /nologo /o"build/win32/i386/expat.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "expat - Win32 Release Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "build/win32/i386/release"
# PROP BASE Intermediate_Dir "build/win32/i386/release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "build/win32/i386/release"
# PROP Intermediate_Dir "build/win32/i386/release"
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "." /I "lib" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "XML_UNICODE" /D "XML_DTD" /D "XML_NS" /D "_STATIC" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "." /I "lib" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "XML_UNICODE" /D "XML_DTD" /D "XML_NS" /D "_STATIC" /D "HAVE_EXPAT_CONFIG_H" /D "XML_STATIC" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"build/win32/i386/expat.bsc"
# ADD BSC32 /nologo /o"build/win32/i386/expat.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "expat - Win32 Release"
# Name "expat - Win32 Debug"
# Name "expat - Win32 Debug Unicode"
# Name "expat - Win32 Release Unicode"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\lib\xmlparse.c
# End Source File
# Begin Source File

SOURCE=.\lib\xmlrole.c
# End Source File
# Begin Source File

SOURCE=.\lib\xmltok.c
# End Source File
# Begin Source File

SOURCE=.\lib\xmltok_impl.c

!IF  "$(CFG)" == "expat - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "expat - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "expat - Win32 Debug Unicode"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "expat - Win32 Release Unicode"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\lib\xmltok_ns.c

!IF  "$(CFG)" == "expat - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "expat - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "expat - Win32 Debug Unicode"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "expat - Win32 Release Unicode"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\lib\ascii.h
# End Source File
# Begin Source File

SOURCE=.\lib\asciitab.h
# End Source File
# Begin Source File

SOURCE=.\lib\expat.h
# End Source File
# Begin Source File

SOURCE=.\expat_config.h
# End Source File
# Begin Source File

SOURCE=.\lib\iasciitab.h
# End Source File
# Begin Source File

SOURCE=.\lib\internal.h
# End Source File
# Begin Source File

SOURCE=.\lib\latin1tab.h
# End Source File
# Begin Source File

SOURCE=.\lib\nametab.h
# End Source File
# Begin Source File

SOURCE=.\lib\utf8tab.h
# End Source File
# Begin Source File

SOURCE=.\lib\winconfig.h
# End Source File
# Begin Source File

SOURCE=.\lib\xmlrole.h
# End Source File
# Begin Source File

SOURCE=.\lib\xmltok.h
# End Source File
# Begin Source File

SOURCE=.\lib\xmltok_impl.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\copying.txt
# End Source File
# Begin Source File

SOURCE=.\readme.txt
# End Source File
# End Target
# End Project
