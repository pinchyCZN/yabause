# Microsoft Developer Studio Project File - Name="yabauseVC6" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=yabauseVC6 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "yabauseVC6.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "yabauseVC6.mak" CFG="yabauseVC6 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "yabauseVC6 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "yabauseVC6 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "yabauseVC6 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "yabauseVC6 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "." /FI"pragma.h" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "yabauseVC6 - Win32 Release"
# Name "yabauseVC6 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "c68k"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\c68k\c68k.c

!IF  "$(CFG)" == "yabauseVC6 - Win32 Release"

!ELSEIF  "$(CFG)" == "yabauseVC6 - Win32 Debug"

# ADD CPP /MTd /I ".\src\c68k\inc" /D "C68K_NO_JUMP_TABLE"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\c68k\c68kexec.c

!IF  "$(CFG)" == "yabauseVC6 - Win32 Release"

!ELSEIF  "$(CFG)" == "yabauseVC6 - Win32 Debug"

# ADD CPP /MTd /I ".\src\c68k\inc" /D "C68K_NO_JUMP_TABLE"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\c68k\gen68k.c

!IF  "$(CFG)" == "yabauseVC6 - Win32 Release"

!ELSEIF  "$(CFG)" == "yabauseVC6 - Win32 Debug"

# PROP Exclude_From_Build 1
# ADD CPP /MTd /I ".\src\c68k\inc" /D "C68K_GEN" /D "__WIN32__" /D "C68K_NO_JUMP_TABLE"

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\src\bios.c
# End Source File
# Begin Source File

SOURCE=".\src\cd-freebsd.c"
# End Source File
# Begin Source File

SOURCE=".\src\cd-linux.c"
# End Source File
# Begin Source File

SOURCE=".\src\cd-macosx.c"
# End Source File
# Begin Source File

SOURCE=".\src\cd-netbsd.c"
# End Source File
# Begin Source File

SOURCE=".\src\cd-windows.c"
# End Source File
# Begin Source File

SOURCE=.\src\cdbase.c
# End Source File
# Begin Source File

SOURCE=.\src\cheat.c
# End Source File
# Begin Source File

SOURCE=.\src\coffelf.c
# End Source File
# Begin Source File

SOURCE=.\src\cs0.c
# End Source File
# Begin Source File

SOURCE=.\src\cs1.c
# End Source File
# Begin Source File

SOURCE=.\src\cs2.c
# End Source File
# Begin Source File

SOURCE=.\src\debug.c
# End Source File
# Begin Source File

SOURCE=.\src\error.c
# End Source File
# Begin Source File

SOURCE=.\src\japmodem.c
# End Source File
# Begin Source File

SOURCE=.\src\m68kc68k.c
# End Source File
# Begin Source File

SOURCE=.\src\m68kcore.c
# End Source File
# Begin Source File

SOURCE=.\src\m68kd.c
# End Source File
# Begin Source File

SOURCE=.\src\m68kq68.c
# End Source File
# Begin Source File

SOURCE=.\src\macjoy.c
# End Source File
# Begin Source File

SOURCE=.\src\memory.c
# End Source File
# Begin Source File

SOURCE=.\src\movie.c
# End Source File
# Begin Source File

SOURCE=.\src\netlink.c
# End Source File
# Begin Source File

SOURCE=.\src\osdcore.c
# End Source File
# Begin Source File

SOURCE=.\src\perdx.c
# End Source File
# Begin Source File

SOURCE=.\src\peripheral.c
# End Source File
# Begin Source File

SOURCE=.\src\perlinuxjoy.c
# End Source File
# Begin Source File

SOURCE=.\src\permacjoy.c
# End Source File
# Begin Source File

SOURCE=.\src\persdljoy.c
# End Source File
# Begin Source File

SOURCE=.\src\profile.c
# End Source File
# Begin Source File

SOURCE=".\src\scr-x.c"
# End Source File
# Begin Source File

SOURCE=.\src\scsp.c
# End Source File
# Begin Source File

SOURCE=.\src\scsp2.c
# End Source File
# Begin Source File

SOURCE=.\src\scu.c
# End Source File
# Begin Source File

SOURCE=.\src\sh2core.c
# End Source File
# Begin Source File

SOURCE=.\src\sh2d.c
# End Source File
# Begin Source File

SOURCE=.\src\sh2iasm.c
# End Source File
# Begin Source File

SOURCE=.\src\sh2idle.c
# End Source File
# Begin Source File

SOURCE=.\src\sh2int.c
# End Source File
# Begin Source File

SOURCE=.\src\sh2trace.c
# End Source File
# Begin Source File

SOURCE=.\src\smpc.c
# End Source File
# Begin Source File

SOURCE=.\src\sndal.c
# End Source File
# Begin Source File

SOURCE=.\src\snddummy.c
# End Source File
# Begin Source File

SOURCE=.\src\snddx.c
# End Source File
# Begin Source File

SOURCE=.\src\sndmac.c
# End Source File
# Begin Source File

SOURCE=.\src\sndsdl.c
# End Source File
# Begin Source File

SOURCE=.\src\sndwav.c
# End Source File
# Begin Source File

SOURCE=".\src\sock-dummy.c"
# End Source File
# Begin Source File

SOURCE=".\src\sock-linux.c"
# End Source File
# Begin Source File

SOURCE=".\src\sock-windows.c"
# End Source File
# Begin Source File

SOURCE=".\src\thr-dummy.c"
# End Source File
# Begin Source File

SOURCE=".\src\thr-linux.c"
# End Source File
# Begin Source File

SOURCE=".\src\thr-macosx.c"
# End Source File
# Begin Source File

SOURCE=".\src\thr-windows.c"
# End Source File
# Begin Source File

SOURCE=.\src\vdp1.c
# End Source File
# Begin Source File

SOURCE=.\src\vdp2.c
# End Source File
# Begin Source File

SOURCE=.\src\vdp2debug.c
# End Source File
# Begin Source File

SOURCE=.\src\vidogl.c
# End Source File
# Begin Source File

SOURCE=.\src\vidshared.c
# End Source File
# Begin Source File

SOURCE=.\src\vidsoft.c
# End Source File
# Begin Source File

SOURCE=.\src\yabause.c
# End Source File
# Begin Source File

SOURCE=.\src\ygl.c
# End Source File
# Begin Source File

SOURCE=.\src\yglshader.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
