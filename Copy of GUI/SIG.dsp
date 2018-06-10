# Microsoft Developer Studio Project File - Name="SIG" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SIG - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SIG.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SIG.mak" CFG="SIG - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SIG - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SIG - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SIG - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "SIG - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "SIG - Win32 Release"
# Name "SIG - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AccountDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoFuncs.cpp
# End Source File
# Begin Source File

SOURCE=.\BCMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\CreatePage.cpp
# End Source File
# Begin Source File

SOURCE=.\D2Account.cpp
# End Source File
# Begin Source File

SOURCE=.\D2ConfigData.cpp
# End Source File
# Begin Source File

SOURCE=.\D2HPManaCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\D2JspPage.cpp
# End Source File
# Begin Source File

SOURCE=.\D2PlayerStats.cpp
# End Source File
# Begin Source File

SOURCE=.\D2Spy.cpp
# End Source File
# Begin Source File

SOURCE=.\DelayPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DescDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DicEditDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ExitDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileManip.cpp
# End Source File
# Begin Source File

SOURCE=.\GeneralPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Gradient.cpp
# End Source File
# Begin Source File

SOURCE=.\GrpBtn.cpp
# End Source File
# Begin Source File

SOURCE=.\GUID.CPP
# End Source File
# Begin Source File

SOURCE=.\HackPage.cpp
# End Source File
# Begin Source File

SOURCE=.\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\Ini.cpp
# End Source File
# Begin Source File

SOURCE=.\Label.cpp
# End Source File
# Begin Source File

SOURCE=.\Logger.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginPage.cpp
# End Source File
# Begin Source File

SOURCE=.\MD5Checksum.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgSender.cpp
# End Source File
# Begin Source File

SOURCE=.\MulePage.cpp
# End Source File
# Begin Source File

SOURCE=.\OSInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\PopupMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ResPack.cpp
# End Source File
# Begin Source File

SOURCE=.\SharedMem.cpp
# End Source File
# Begin Source File

SOURCE=.\SIG.cpp
# End Source File
# Begin Source File

SOURCE=.\SIG.rc
# End Source File
# Begin Source File

SOURCE=.\SIGDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SigStats.cpp
# End Source File
# Begin Source File

SOURCE=.\Singleton.cpp
# End Source File
# Begin Source File

SOURCE=.\StatisticsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StatsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StrSplit.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemTray.cpp
# End Source File
# Begin Source File

SOURCE=.\Ticker.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AccountDlg.h
# End Source File
# Begin Source File

SOURCE=.\ArrayEx.h
# End Source File
# Begin Source File

SOURCE=.\AutoFuncs.h
# End Source File
# Begin Source File

SOURCE=.\BCMenu.h
# End Source File
# Begin Source File

SOURCE=.\BtnST.h
# End Source File
# Begin Source File

SOURCE=..\common.h
# End Source File
# Begin Source File

SOURCE=.\CreatePage.h
# End Source File
# Begin Source File

SOURCE=.\D2Account.h
# End Source File
# Begin Source File

SOURCE=.\D2CONFIGDATA.H
# End Source File
# Begin Source File

SOURCE=.\D2HPManaCtrl.h
# End Source File
# Begin Source File

SOURCE=.\D2JspPage.h
# End Source File
# Begin Source File

SOURCE=.\D2PlayerStats.h
# End Source File
# Begin Source File

SOURCE=.\D2Spy.h
# End Source File
# Begin Source File

SOURCE=.\D2WndPosition.h
# End Source File
# Begin Source File

SOURCE=.\DelayPage.h
# End Source File
# Begin Source File

SOURCE=.\DescDlg.h
# End Source File
# Begin Source File

SOURCE=.\DicEditDlg.h
# End Source File
# Begin Source File

SOURCE=.\EditEx.h
# End Source File
# Begin Source File

SOURCE=.\Encrypt.h
# End Source File
# Begin Source File

SOURCE=.\ExitDlg.h
# End Source File
# Begin Source File

SOURCE=.\FileManip.h
# End Source File
# Begin Source File

SOURCE=.\FileMD5Def.h
# End Source File
# Begin Source File

SOURCE=.\GeneralPage.h
# End Source File
# Begin Source File

SOURCE=.\Gradient.h
# End Source File
# Begin Source File

SOURCE=.\GrpBtn.h
# End Source File
# Begin Source File

SOURCE=.\GUID.H
# End Source File
# Begin Source File

SOURCE=.\HackPage.h
# End Source File
# Begin Source File

SOURCE=.\HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\Ini.h
# End Source File
# Begin Source File

SOURCE=.\Label.h
# End Source File
# Begin Source File

SOURCE=.\Logger.h
# End Source File
# Begin Source File

SOURCE=.\LoginPage.h
# End Source File
# Begin Source File

SOURCE=.\MD5Checksum.h
# End Source File
# Begin Source File

SOURCE=.\MsgSender.h
# End Source File
# Begin Source File

SOURCE=.\MulePage.h
# End Source File
# Begin Source File

SOURCE=.\OSInfo.h
# End Source File
# Begin Source File

SOURCE=.\PopupMenu.h
# End Source File
# Begin Source File

SOURCE=.\RealmInfo.h
# End Source File
# Begin Source File

SOURCE=.\ReportCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ResPack.h
# End Source File
# Begin Source File

SOURCE=.\SharedMem.h
# End Source File
# Begin Source File

SOURCE=.\SIG.h
# End Source File
# Begin Source File

SOURCE=.\SIGDlg.h
# End Source File
# Begin Source File

SOURCE=.\SigStats.h
# End Source File
# Begin Source File

SOURCE=.\Singleton.h
# End Source File
# Begin Source File

SOURCE=.\StatisticsDlg.h
# End Source File
# Begin Source File

SOURCE=.\StatsDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StrSplit.h
# End Source File
# Begin Source File

SOURCE=.\SystemTray.h
# End Source File
# Begin Source File

SOURCE=.\Ticker.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\021.bmp
# End Source File
# Begin Source File

SOURCE=.\res\071.bmp
# End Source File
# Begin Source File

SOURCE=.\res\093.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b012.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b018.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b019.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b075.bmp
# End Source File
# Begin Source File

SOURCE=.\res\back.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Bkgnd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BmpBack.bmp
# End Source File
# Begin Source File

SOURCE=.\res\border043.bmp
# End Source File
# Begin Source File

SOURCE=.\res\d2cn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\down.ico
# End Source File
# Begin Source File

SOURCE=.\res\edititem.ico
# End Source File
# Begin Source File

SOURCE=.\res\f30.bmp
# End Source File
# Begin Source File

SOURCE=.\res\f31.bmp
# End Source File
# Begin Source File

SOURCE=.\res\f32.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00004.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00005.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00006.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00007.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00008.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00009.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00010.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00011.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00012.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00013.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00014.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00015.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00016.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00017.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00018.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00019.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00020.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00021.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00022.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00023.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00024.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00025.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\ilight_gray.ico
# End Source File
# Begin Source File

SOURCE=.\res\lemb039n.bmp
# End Source File
# Begin Source File

SOURCE=.\res\light_ai.ico
# End Source File
# Begin Source File

SOURCE=.\res\light_hb.ico
# End Source File
# Begin Source File

SOURCE=.\res\light_lc.ico
# End Source File
# Begin Source File

SOURCE=.\res\light_re.ico
# End Source File
# Begin Source File

SOURCE=.\res\light_rv.ico
# End Source File
# Begin Source File

SOURCE=.\res\light_st.ico
# End Source File
# Begin Source File

SOURCE=.\res\marble.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mulemode.ico
# End Source File
# Begin Source File

SOURCE=.\res\no.ico
# End Source File
# Begin Source File

SOURCE=.\res\open.ico
# End Source File
# Begin Source File

SOURCE=.\res\selfile_.ico
# End Source File
# Begin Source File

SOURCE=.\res\showd2_a.ico
# End Source File
# Begin Source File

SOURCE=.\res\showd2_d.ico
# End Source File
# Begin Source File

SOURCE=.\res\SIG.ico
# End Source File
# Begin Source File

SOURCE=.\res\SIG.rc2
# End Source File
# Begin Source File

SOURCE=.\res\start_activate.ico
# End Source File
# Begin Source File

SOURCE=.\res\start_deactivate.ico
# End Source File
# Begin Source File

SOURCE=.\res\up.ico
# End Source File
# Begin Source File

SOURCE=.\res\yes.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\RealmInfo.lib
# End Source File
# Begin Source File

SOURCE=.\Encrypt.lib
# End Source File
# Begin Source File

SOURCE=.\LoaderEx.lib
# End Source File
# End Target
# End Project
