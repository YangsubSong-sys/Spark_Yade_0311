# Microsoft Developer Studio Project File - Name="BiOpTroCon" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=BiOpTroCon - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BiOpTroCon.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BiOpTroCon.mak" CFG="BiOpTroCon - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BiOpTroCon - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "BiOpTroCon - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BiOpTroCon - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /ZI /Od /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D VERSION=\"v1.6.0\" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Shlwapi.lib /nologo /stack:0x200000 /subsystem:windows /incremental:yes /debug /machine:I386 /ignore:4084 /ignore:4089
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "BiOpTroCon - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D VERSION=\"v1.6.0\" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 D64HDLL.lib msimg32.lib Shlwapi.lib /nologo /stack:0x200000 /subsystem:windows /debug /machine:I386 /pdbtype:sept /ignore:4084 /ignore:4089
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "BiOpTroCon - Win32 Release"
# Name "BiOpTroCon - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BiOpTroCon.cpp
# End Source File
# Begin Source File

SOURCE=.\BiOpTroCon.rc
# End Source File
# Begin Source File

SOURCE=.\BiOpTroConDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\BiOpTroConView.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildView1.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildView10.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildView2.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildView3.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildView4.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildView5.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildView6.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildView7.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildView8.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildView9.cpp
# End Source File
# Begin Source File

SOURCE=.\comidaq.cpp
# End Source File
# Begin Source File

SOURCE=.\comiled.cpp
# End Source File
# Begin Source File

SOURCE=.\comitoggleswitch.cpp
# End Source File
# Begin Source File

SOURCE=.\commandbutton.cpp
# End Source File
# Begin Source File

SOURCE=.\Contec.cpp
# End Source File
# Begin Source File

SOURCE=.\Dio.cpp
# End Source File
# Begin Source File

SOURCE=.\DioD64H.cpp
# End Source File
# Begin Source File

SOURCE=.\DioD96.cpp
# End Source File
# Begin Source File

SOURCE=.\File.cpp
# End Source File
# Begin Source File

SOURCE=.\FileSysInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\font.cpp
# End Source File
# Begin Source File

SOURCE=.\Globals.cpp
# End Source File
# Begin Source File

SOURCE=.\GRs.cpp
# End Source File
# Begin Source File

SOURCE=.\IniFile.cpp
# End Source File
# Begin Source File

SOURCE=.\labelcontrol.cpp
# End Source File
# Begin Source File

SOURCE=.\LeftBarDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\mdccheckbox.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeMan11.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeMan12.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeMan14.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeMan15.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeMan16.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeMan17.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeManSpark.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeRun11.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeSer12.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeSer13.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeSer15.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeSet11.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeSet12.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeSet13.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeSet15.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeSet16.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeSet17.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeSet19.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeSet21.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeSub11.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeSub13.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeSub15.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeSub17.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeSub18.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeSub19.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeSub20.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeSub22.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeSub25.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeSub31.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeSub71.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeSub72.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeView16.cpp
# End Source File
# Begin Source File

SOURCE=.\msflexgrid.cpp
# End Source File
# Begin Source File

SOURCE=.\MyCtrlBar.cpp
# End Source File
# Begin Source File

SOURCE=.\MyRichEditCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\PassWord1.cpp
# End Source File
# Begin Source File

SOURCE=.\picture.cpp
# End Source File
# Begin Source File

SOURCE=.\rowcursor.cpp
# End Source File
# Begin Source File

SOURCE=.\SaveAsNewBdl.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TrueColorToolbar.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\A_Define1.h
# End Source File
# Begin Source File

SOURCE=.\A_Define101.h
# End Source File
# Begin Source File

SOURCE=.\A_Define102.h
# End Source File
# Begin Source File

SOURCE=.\A_Define103.h
# End Source File
# Begin Source File

SOURCE=.\A_Define2.h
# End Source File
# Begin Source File

SOURCE=.\BiOpTroCon.h
# End Source File
# Begin Source File

SOURCE=.\BiOpTroConDoc.h
# End Source File
# Begin Source File

SOURCE=.\BiOpTroConView.h
# End Source File
# Begin Source File

SOURCE=.\ChildView1.h
# End Source File
# Begin Source File

SOURCE=.\ChildView10.h
# End Source File
# Begin Source File

SOURCE=.\ChildView2.h
# End Source File
# Begin Source File

SOURCE=.\ChildView3.h
# End Source File
# Begin Source File

SOURCE=.\ChildView4.h
# End Source File
# Begin Source File

SOURCE=.\ChildView5.h
# End Source File
# Begin Source File

SOURCE=.\ChildView6.h
# End Source File
# Begin Source File

SOURCE=.\ChildView7.h
# End Source File
# Begin Source File

SOURCE=.\ChildView8.h
# End Source File
# Begin Source File

SOURCE=.\ChildView9.h
# End Source File
# Begin Source File

SOURCE=.\comidaq.h
# End Source File
# Begin Source File

SOURCE=.\ComiDaqApi.h
# End Source File
# Begin Source File

SOURCE=.\ComiDaqComm.h
# End Source File
# Begin Source File

SOURCE=.\comiled.h
# End Source File
# Begin Source File

SOURCE=.\comitoggleswitch.h
# End Source File
# Begin Source File

SOURCE=.\commandbutton.h
# End Source File
# Begin Source File

SOURCE=.\Contec.h
# End Source File
# Begin Source File

SOURCE=.\Dio.h
# End Source File
# Begin Source File

SOURCE=.\DioD64H.h
# End Source File
# Begin Source File

SOURCE=.\DioD96.h
# End Source File
# Begin Source File

SOURCE=.\File.h
# End Source File
# Begin Source File

SOURCE=.\FileSysInfo.h
# End Source File
# Begin Source File

SOURCE=.\font.h
# End Source File
# Begin Source File

SOURCE=.\Globals.h
# End Source File
# Begin Source File

SOURCE=.\Gnet1.h
# End Source File
# Begin Source File

SOURCE=.\GRs.h
# End Source File
# Begin Source File

SOURCE=.\IniFile.h
# End Source File
# Begin Source File

SOURCE=.\IO.h
# End Source File
# Begin Source File

SOURCE=.\labelcontrol.h
# End Source File
# Begin Source File

SOURCE=.\LeftBarDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\mdccheckbox.h
# End Source File
# Begin Source File

SOURCE=.\ModeAuto11.h
# End Source File
# Begin Source File

SOURCE=.\ModeMan11.h
# End Source File
# Begin Source File

SOURCE=.\ModeMan12.h
# End Source File
# Begin Source File

SOURCE=.\ModeMan14.h
# End Source File
# Begin Source File

SOURCE=.\ModeMan15.h
# End Source File
# Begin Source File

SOURCE=.\ModeMan16.h
# End Source File
# Begin Source File

SOURCE=.\ModeMan17.h
# End Source File
# Begin Source File

SOURCE=.\ModeManSpark.h
# End Source File
# Begin Source File

SOURCE=.\ModeRun11.h
# End Source File
# Begin Source File

SOURCE=.\ModeSer12.h
# End Source File
# Begin Source File

SOURCE=.\ModeSer13.h
# End Source File
# Begin Source File

SOURCE=.\ModeSer15.h
# End Source File
# Begin Source File

SOURCE=.\ModeSet11.h
# End Source File
# Begin Source File

SOURCE=.\ModeSet12.h
# End Source File
# Begin Source File

SOURCE=.\ModeSet13.h
# End Source File
# Begin Source File

SOURCE=.\ModeSet15.h
# End Source File
# Begin Source File

SOURCE=.\ModeSet16.h
# End Source File
# Begin Source File

SOURCE=.\ModeSet17.h
# End Source File
# Begin Source File

SOURCE=.\ModeSet19.h
# End Source File
# Begin Source File

SOURCE=.\ModeSet21.h
# End Source File
# Begin Source File

SOURCE=.\ModeSub11.h
# End Source File
# Begin Source File

SOURCE=.\ModeSub13.h
# End Source File
# Begin Source File

SOURCE=.\ModeSub15.h
# End Source File
# Begin Source File

SOURCE=.\ModeSub17.h
# End Source File
# Begin Source File

SOURCE=.\ModeSub18.h
# End Source File
# Begin Source File

SOURCE=.\ModeSub19.h
# End Source File
# Begin Source File

SOURCE=.\ModeSub20.h
# End Source File
# Begin Source File

SOURCE=.\ModeSub22.h
# End Source File
# Begin Source File

SOURCE=.\ModeSub25.h
# End Source File
# Begin Source File

SOURCE=.\ModeSub31.h
# End Source File
# Begin Source File

SOURCE=.\ModeSub71.h
# End Source File
# Begin Source File

SOURCE=.\ModeSub72.h
# End Source File
# Begin Source File

SOURCE=.\ModeView16.h
# End Source File
# Begin Source File

SOURCE=.\msflexgrid.h
# End Source File
# Begin Source File

SOURCE=.\MyCtrlBar.h
# End Source File
# Begin Source File

SOURCE=.\MyRichEditCtrl.h
# End Source File
# Begin Source File

SOURCE=.\PassWord1.h
# End Source File
# Begin Source File

SOURCE=.\PCI_D64H.h
# End Source File
# Begin Source File

SOURCE=.\picture.h
# End Source File
# Begin Source File

SOURCE=.\Print.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\rowcursor.h
# End Source File
# Begin Source File

SOURCE=.\SaveAsNewBdl.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TrueColorToolbar.h
# End Source File
# Begin Source File

SOURCE=.\UniDAQ.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\AUTO1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\AUTO2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\AUTO3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\backgroundA1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\backgroundA2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BiOpTro.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BiOpTroCon.ico
# End Source File
# Begin Source File

SOURCE=.\res\BiOpTroCon.rc2
# End Source File
# Begin Source File

SOURCE=.\res\BiOpTroConDoc.ico
# End Source File
# Begin Source File

SOURCE=".\0 icon\bmp2comp.bmp"
# End Source File
# Begin Source File

SOURCE=".\0 icon\BMP3.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\ExtBarDemo.ico
# End Source File
# Begin Source File

SOURCE=.\res\ExtBarDemoDoc.ico
# End Source File
# Begin Source File

SOURCE=".\res\GRIM1-2.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\GRIM1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MSN_Back.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MSN_Back_Alpha.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MSN_Icon.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MSN_Logo.bmp
# End Source File
# Begin Source File

SOURCE=".\res\NABI-2.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\NABI2-2.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\pic111.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar_Back.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TrueColor_Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TrueColor_Toolbar_Mono.bmp
# End Source File
# Begin Source File

SOURCE=.\MSIMG32.LIB
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section BiOpTroCon : {04598FC4-866C-11CF-AB7C-00AA00C08FCF}
# 	2:5:Class:CCommandButton
# 	2:10:HeaderFile:commandbutton.h
# 	2:8:ImplFile:commandbutton.cpp
# End Section
# Section BiOpTroCon : {8BD21D43-EC42-11CE-9E0D-00AA006002F3}
# 	2:5:Class:CMdcCheckBox
# 	2:10:HeaderFile:mdccheckbox.h
# 	2:8:ImplFile:mdccheckbox.cpp
# End Section
# Section BiOpTroCon : {978C9E23-D4B0-11CE-BF2D-00AA003F40D0}
# 	2:21:DefaultSinkHeaderFile:labelcontrol.h
# 	2:16:DefaultSinkClass:CLabelControl
# End Section
# Section BiOpTroCon : {0C3875DF-C1B2-491B-9999-70E9DFA405B0}
# 	2:5:Class:CCOMILed
# 	2:10:HeaderFile:comiled1.h
# 	2:8:ImplFile:comiled1.cpp
# End Section
# Section BiOpTroCon : {5F4DF280-531B-11CF-91F6-C2863C385E30}
# 	2:5:Class:CMSFlexGrid
# 	2:10:HeaderFile:msflexgrid.h
# 	2:8:ImplFile:msflexgrid.cpp
# End Section
# Section BiOpTroCon : {B88B9FB8-1ACB-4718-AE71-4651D56FE25A}
# 	2:5:Class:CComiDaq
# 	2:10:HeaderFile:comidaq1.h
# 	2:8:ImplFile:comidaq1.cpp
# End Section
# Section BiOpTroCon : {6262D3A0-531B-11CF-91F6-C2863C385E30}
# 	2:21:DefaultSinkHeaderFile:msflexgrid.h
# 	2:16:DefaultSinkClass:CMSFlexGrid
# End Section
# Section BiOpTroCon : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font.h
# 	2:8:ImplFile:font.cpp
# End Section
# Section BiOpTroCon : {03AF249E-119E-4569-838E-167E929EC6DA}
# 	2:21:DefaultSinkHeaderFile:bigfilecontrol.h
# 	2:16:DefaultSinkClass:CBigFileControl
# End Section
# Section BiOpTroCon : {209656A8-857B-4E7B-8EB5-C54AF2B55608}
# 	2:21:DefaultSinkHeaderFile:comiled1.h
# 	2:16:DefaultSinkClass:CCOMILed
# End Section
# Section BiOpTroCon : {D638FBDD-9AFD-4C6D-B709-3246179BB116}
# 	2:21:DefaultSinkHeaderFile:comidaq1.h
# 	2:16:DefaultSinkClass:CComiDaq
# End Section
# Section BiOpTroCon : {D7053240-CE69-11CD-A777-00DD01143C57}
# 	2:21:DefaultSinkHeaderFile:commandbutton.h
# 	2:16:DefaultSinkClass:CCommandButton
# End Section
# Section BiOpTroCon : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture
# 	2:10:HeaderFile:picture.h
# 	2:8:ImplFile:picture.cpp
# End Section
# Section BiOpTroCon : {C37E2B7B-FA0C-4093-8103-61ABC4FADFB9}
# 	2:5:Class:CComiToggleSwitch
# 	2:10:HeaderFile:comitoggleswitch1.h
# 	2:8:ImplFile:comitoggleswitch1.cpp
# End Section
# Section BiOpTroCon : {2FEA5270-3610-4610-8C6F-8A5B15C48E45}
# 	2:21:DefaultSinkHeaderFile:comitoggleswitch1.h
# 	2:16:DefaultSinkClass:CComiToggleSwitch
# End Section
# Section BiOpTroCon : {CCDD63B4-96DB-475D-A26F-35B6B5A45AE2}
# 	2:5:Class:CBigFileControl
# 	2:10:HeaderFile:bigfilecontrol.h
# 	2:8:ImplFile:bigfilecontrol.cpp
# End Section
# Section BiOpTroCon : {04598FC1-866C-11CF-AB7C-00AA00C08FCF}
# 	2:5:Class:CLabelControl
# 	2:10:HeaderFile:labelcontrol.h
# 	2:8:ImplFile:labelcontrol.cpp
# End Section
# Section BiOpTroCon : {8BD21D40-EC42-11CE-9E0D-00AA006002F3}
# 	2:21:DefaultSinkHeaderFile:mdccheckbox.h
# 	2:16:DefaultSinkClass:CMdcCheckBox
# End Section
# Section BiOpTroCon : {9F6AA700-D188-11CD-AD48-00AA003C9CB6}
# 	2:5:Class:CRowCursor
# 	2:10:HeaderFile:rowcursor.h
# 	2:8:ImplFile:rowcursor.cpp
# End Section
