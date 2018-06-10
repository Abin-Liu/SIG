; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSigRstDlg
LastTemplate=CButton
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "SigRst.h"

ClassCount=3
Class1=CSigRstApp
Class2=CSigRstDlg

ResourceCount=2
Resource1=IDR_MAINFRAME
Class3=CFrameBtn
Resource2=IDD_SIGRST_DIALOG

[CLS:CSigRstApp]
Type=0
HeaderFile=SigRst.h
ImplementationFile=SigRst.cpp
Filter=N

[CLS:CSigRstDlg]
Type=0
HeaderFile=SigRstDlg.h
ImplementationFile=SigRstDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDCANCEL



[DLG:IDD_SIGRST_DIALOG]
Type=1
Class=CSigRstDlg
ControlCount=2
Control1=IDC_MSG,static,1342308865
Control2=IDCANCEL,button,1342242816

[CLS:CFrameBtn]
Type=0
HeaderFile=FrameBtn.h
ImplementationFile=FrameBtn.cpp
BaseClass=CButton
Filter=W
VirtualFilter=BWC

