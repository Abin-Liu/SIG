; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CD2GatewayDlg
LastTemplate=CEdit
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "D2Gateway.h"

ClassCount=6
Class1=CD2GatewayApp
Class2=CD2GatewayDlg
Class3=CAboutDlg

ResourceCount=9
Resource1=IDR_MENU1 (Chinese (P.R.C.))
Resource2=IDR_MAINFRAME
Class4=CSingleSelChkList
Resource3=IDD_D2GATEWAY_DIALOG
Class5=CRealmDlg
Resource4=IDD_REALMDLG
Class6=CTestEdit
Resource5=IDD_ABOUTBOX
Resource6=IDD_ABOUTBOX (Chinese (P.R.C.))
Resource7=IDD_REALMDLG (Chinese (P.R.C.))
Resource8=IDD_D2GATEWAY_DIALOG (Chinese (P.R.C.))
Resource9=IDR_MENU1

[CLS:CD2GatewayApp]
Type=0
HeaderFile=D2Gateway.h
ImplementationFile=D2Gateway.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CD2GatewayDlg]
Type=0
HeaderFile=D2GatewayDlg.h
ImplementationFile=D2GatewayDlg.cpp
Filter=D
LastObject=IDC_DISABLESKIPTOOPEN
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=D2GatewayDlg.h
ImplementationFile=D2GatewayDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=6
Control1=IDC_STATIC,static,1342308480
Control2=IDC_STATIC,static,1342308352
Control3=IDOK,button,1342373889
Control4=IDC_STATIC,static,1342177283
Control5=IDC_URL,static,1342308480
Control6=IDC_ABIN,static,1342308480

[DLG:IDD_D2GATEWAY_DIALOG]
Type=1
Class=CD2GatewayDlg
ControlCount=11
Control1=IDC_LIST1,SysListView32,1342275613
Control2=IDC_DISABLESKIPTOOPEN,button,1342242819
Control3=IDC_ADD,button,1342242816
Control4=IDC_EDIT,button,1342242816
Control5=IDC_REMOVE,button,1342242816
Control6=IDC_IMPORT,button,1342242816
Control7=IDC_BATCHIMPORT,button,1342242816
Control8=IDC_EXPORT,button,1342242816
Control9=IDC_BATCHEXPORT,button,1342242816
Control10=IDC_APPLY,button,1476460544
Control11=IDCANCEL,button,1342242816

[CLS:CSingleSelChkList]
Type=0
HeaderFile=SingleSelChkList.h
ImplementationFile=SingleSelChkList.cpp
BaseClass=CListCtrl
Filter=W
VirtualFilter=FWC

[DLG:IDD_REALMDLG]
Type=1
Class=CRealmDlg
ControlCount=8
Control1=IDC_STATIC,static,1342308352
Control2=IDC_NAME,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_BNETIP,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_PREFERRED,edit,1350631552
Control7=IDOK,button,1342242817
Control8=IDCANCEL,button,1342242816

[CLS:CRealmDlg]
Type=0
HeaderFile=RealmDlg.h
ImplementationFile=RealmDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

[MNU:IDR_MENU1]
Type=1
Class=CD2GatewayDlg
Command1=IDCANCEL
Command2=ID_HELP_HELP
Command3=ID_HELP_ABOUT
CommandCount=3

[CLS:CTestEdit]
Type=0
HeaderFile=TestEdit.h
ImplementationFile=TestEdit.cpp
BaseClass=CEdit
Filter=W
VirtualFilter=WC

[MNU:IDR_MENU1 (Chinese (P.R.C.))]
Type=1
Class=?
Command1=IDCANCEL
Command2=ID_HELP_HELP
Command3=ID_HELP_ABOUT
CommandCount=3

[DLG:IDD_REALMDLG (Chinese (P.R.C.))]
Type=1
Class=?
ControlCount=8
Control1=IDC_STATIC,static,1342308352
Control2=IDC_NAME,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_BNETIP,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_PREFERRED,edit,1350631552
Control7=IDOK,button,1342242817
Control8=IDCANCEL,button,1342242816

[DLG:IDD_ABOUTBOX (Chinese (P.R.C.))]
Type=1
Class=?
ControlCount=6
Control1=IDC_STATIC,static,1342308480
Control2=IDC_STATIC,static,1342308352
Control3=IDOK,button,1342373889
Control4=IDC_STATIC,static,1342177283
Control5=IDC_URL,static,1342308480
Control6=IDC_ABIN,static,1342308480

[DLG:IDD_D2GATEWAY_DIALOG (Chinese (P.R.C.))]
Type=1
Class=?
ControlCount=11
Control1=IDC_LIST1,SysListView32,1342275613
Control2=IDC_DISABLESKIPTOOPEN,button,1342242819
Control3=IDC_ADD,button,1342242816
Control4=IDC_EDIT,button,1342242816
Control5=IDC_REMOVE,button,1342242816
Control6=IDC_IMPORT,button,1342242816
Control7=IDC_BATCHIMPORT,button,1342242816
Control8=IDC_EXPORT,button,1342242816
Control9=IDC_BATCHEXPORT,button,1342242816
Control10=IDC_APPLY,button,1476460544
Control11=IDCANCEL,button,1342242816

