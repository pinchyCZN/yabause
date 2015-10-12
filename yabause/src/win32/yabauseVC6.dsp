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
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "." /FI"pragma.h" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "__i386__" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib comctl32.lib /nologo /subsystem:windows /profile /machine:I386

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
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /FI"pragma.h" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "HAVE_C68K" /D "__i386__" /D "SH2_DYNAREC" /D "DEBUG" /D "SCSP_DEBUG" /D "VDP1_DEBUG" /D "VDP2_DEBUG" /D "SMPC_DEBUG" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Ws2_32.lib comctl32.lib /nologo /subsystem:windows /profile /debug /machine:I386

!ENDIF 

# Begin Target

# Name "yabauseVC6 - Win32 Release"
# Name "yabauseVC6 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "c68k"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\c68k\c68k.c

!IF  "$(CFG)" == "yabauseVC6 - Win32 Release"

# ADD CPP /I "..\c68k\inc" /D "C68K_NO_JUMP_TABLE"

!ELSEIF  "$(CFG)" == "yabauseVC6 - Win32 Debug"

# ADD CPP /MTd /I "..\c68k\inc" /D "C68K_NO_JUMP_TABLE"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\c68k\c68kexec.c

!IF  "$(CFG)" == "yabauseVC6 - Win32 Release"

# ADD CPP /I "..\c68k\inc" /D "C68K_NO_JUMP_TABLE"
# SUBTRACT CPP /O<none>

!ELSEIF  "$(CFG)" == "yabauseVC6 - Win32 Debug"

# ADD CPP /MTd /I "..\c68k\inc" /D "C68K_NO_JUMP_TABLE"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\c68k\gen68k.c
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "win32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\input.c
# End Source File
# Begin Source File

SOURCE=main.c
# End Source File
# Begin Source File

SOURCE=resize.c
# End Source File
# Begin Source File

SOURCE=.\vc6stuff.c
# End Source File
# End Group
# Begin Group "titan"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\titan\titan.c
# End Source File
# End Group
# Begin Group "sh2_dynarec"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\sh2_dynarec\linkage_x86.s

!IF  "$(CFG)" == "yabauseVC6 - Win32 Release"

!ELSEIF  "$(CFG)" == "yabauseVC6 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\sh2_dynarec\sh2_dynarec.c
# End Source File
# End Group
# Begin Source File

SOURCE=..\bios.c
# End Source File
# Begin Source File

SOURCE="..\cd-windows.c"
# End Source File
# Begin Source File

SOURCE=..\cdbase.c
# End Source File
# Begin Source File

SOURCE=..\cheat.c
# End Source File
# Begin Source File

SOURCE=..\coffelf.c
# End Source File
# Begin Source File

SOURCE=..\cs0.c
# End Source File
# Begin Source File

SOURCE=..\cs1.c
# End Source File
# Begin Source File

SOURCE=..\cs2.c
# End Source File
# Begin Source File

SOURCE=..\debug.c
# End Source File
# Begin Source File

SOURCE=..\error.c
# End Source File
# Begin Source File

SOURCE=..\japmodem.c
# End Source File
# Begin Source File

SOURCE=..\m68kc68k.c
# End Source File
# Begin Source File

SOURCE=..\m68kcore.c
# End Source File
# Begin Source File

SOURCE=..\m68kd.c
# End Source File
# Begin Source File

SOURCE=..\m68kq68.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\memory.c
# End Source File
# Begin Source File

SOURCE=..\movie.c
# End Source File
# Begin Source File

SOURCE=..\netlink.c
# End Source File
# Begin Source File

SOURCE=..\osdcore.c
# End Source File
# Begin Source File

SOURCE=..\perdx.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\peripheral.c
# End Source File
# Begin Source File

SOURCE=..\persdljoy.c
# End Source File
# Begin Source File

SOURCE=..\profile.c
# End Source File
# Begin Source File

SOURCE=.\resource.rc
# End Source File
# Begin Source File

SOURCE=..\scsp.c
# End Source File
# Begin Source File

SOURCE=..\scsp2.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\scu.c
# End Source File
# Begin Source File

SOURCE=..\sh2core.c
# End Source File
# Begin Source File

SOURCE=..\sh2d.c
# End Source File
# Begin Source File

SOURCE=..\sh2iasm.c
# End Source File
# Begin Source File

SOURCE=..\sh2idle.c
# End Source File
# Begin Source File

SOURCE=..\sh2int.c
# End Source File
# Begin Source File

SOURCE=..\sh2trace.c
# End Source File
# Begin Source File

SOURCE=..\smpc.c
# End Source File
# Begin Source File

SOURCE=..\sndal.c
# End Source File
# Begin Source File

SOURCE=..\snddummy.c
# End Source File
# Begin Source File

SOURCE=..\snddx.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\sndsdl.c
# End Source File
# Begin Source File

SOURCE=..\sndwav.c
# End Source File
# Begin Source File

SOURCE="..\sock-dummy.c"
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE="..\sock-windows.c"
# End Source File
# Begin Source File

SOURCE="..\thr-dummy.c"
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE="..\thr-windows.c"
# End Source File
# Begin Source File

SOURCE=..\vdp1.c
# End Source File
# Begin Source File

SOURCE=..\vdp2.c
# End Source File
# Begin Source File

SOURCE=..\vdp2debug.c
# End Source File
# Begin Source File

SOURCE=..\vidogl.c
# End Source File
# Begin Source File

SOURCE=..\vidshared.c
# End Source File
# Begin Source File

SOURCE=..\vidsoft.c
# End Source File
# Begin Source File

SOURCE=..\yabause.c
# End Source File
# Begin Source File

SOURCE=..\ygl.c
# End Source File
# Begin Source File

SOURCE=..\yglshader.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\SNDVisual.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
