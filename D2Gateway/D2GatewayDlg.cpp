// D2GatewayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "D2Gateway.h"
#include "D2GatewayDlg.h"
#include "IniFile.h"
#include "RealmDlg.h"
#include "HyperLink.h"
#include "FolderDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CHyperLink	m_wndUrl;
	CHyperLink	m_wndAbin;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_URL, m_wndUrl);
	DDX_Control(pDX, IDC_ABIN, m_wndAbin);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CD2GatewayDlg dialog

CD2GatewayDlg::CD2GatewayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CD2GatewayDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CD2GatewayDlg)
	m_bDisableSkipToOpen = TRUE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nCurSel = -1;
}

void CD2GatewayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CD2GatewayDlg)
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	DDX_Check(pDX, IDC_DISABLESKIPTOOPEN, m_bDisableSkipToOpen);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CD2GatewayDlg, CDialog)
	//{{AFX_MSG_MAP(CD2GatewayDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_IMPORT, OnImport)
	ON_BN_CLICKED(IDC_EXPORT, OnExport)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_COMMAND(ID_HELP_HELP, OnHelpHelp)
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	ON_BN_CLICKED(IDC_BATCHIMPORT, OnBatchimport)
	ON_BN_CLICKED(IDC_BATCHEXPORT, OnBatchexport)
	ON_BN_CLICKED(IDC_DISABLESKIPTOOPEN, OnDisableskiptoopen)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SSCL_ITEMCHECKED, OnSSCLChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CD2GatewayDlg message handlers

BOOL CD2GatewayDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	((CD2GatewayApp*)AfxGetApp())->m_st.SetPrevWnd(GetSafeHwnd());
	m_ri.InitPrivateRealms(g_sAppDir + _T("\\D2Gateway.dat"));

	m_wndList.SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
	m_wndList.InsertColumn(0, "   显示名称", LVCFMT_LEFT, 140);
	m_wndList.InsertColumn(1, "服务器IP", LVCFMT_LEFT, 120);
	m_wndList.InsertColumn(2, "子国度", LVCFMT_LEFT, 90);
	ListRealms();

	CheckCurrentRealm();
	UpdateCtrls();
	m_wndList.EnsureVisible(m_nCurSel, FALSE);

	if (!m_ri.IsSkipToOpenDisabled(m_bDisableSkipToOpen))
	{
		m_bDisableSkipToOpen = FALSE;
	}

	UpdateData(FALSE);

	
	// CG: The following block was added by the ToolTips component.
	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);

		// TODO: Use one of the following forms to add controls:
		// m_tooltip.AddTool(GetDlgItem(IDC_), <string-table-id>);
		// m_tooltip.AddTool(GetDlgItem(IDC_), _T(""));
		m_tooltip.AddTool(GetDlgItem(IDC_ADD), _T("添加新的国度服务器资料。"));
		m_tooltip.AddTool(GetDlgItem(IDC_EDIT), _T("更改现有的国度服务器资料。"));
		m_tooltip.AddTool(GetDlgItem(IDC_REMOVE), _T("删除选定的国度服务器资料。"));
		m_tooltip.AddTool(GetDlgItem(IDC_IMPORT), _T("从指定的\"*.REG\"文件中导入国度服务器设置。"));
		m_tooltip.AddTool(GetDlgItem(IDC_BATCHIMPORT), _T("读取一个目录下所有的\"*.REG\"文件，导入符合格式要求的数据。"));
		m_tooltip.AddTool(GetDlgItem(IDC_EXPORT), _T("导出国度服务器设置到指定的\"*.REG\"文件中。"));
		m_tooltip.AddTool(GetDlgItem(IDC_BATCHEXPORT), _T("将所有的国度服务器设置导出到指定的目录下。"));
		m_tooltip.AddTool(GetDlgItem(IDC_APPLY), _T("保存所有数据，将Diablo II默认国度更新为您所选定的服务器。"));
		m_tooltip.AddTool(GetDlgItem(IDCANCEL), _T("退出程序运行。"));
		m_tooltip.AddTool(GetDlgItem(IDC_LIST1), _T("点击国度名称前的选取框以将其设为默认国度。"));
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CD2GatewayDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CD2GatewayDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CD2GatewayDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CD2GatewayDlg::CheckCurrentRealm()
{
	TCHAR szBnetIP[512] = _T("");
	TCHAR szPreferred[512] = _T("");

	if (!m_ri.GetDefaultRealm(szBnetIP, szPreferred))
		return;

	TCHAR szName[512] = _T("");

	if (!m_ri.FindRealmName(szBnetIP, szPreferred, szName))
		return;

	m_wndList.UnCheckAll();

	for (int i = 0; i < m_wndList.GetItemCount(); i++)
	{
		if (m_wndList.GetItemText(i, 0).CompareNoCase(szName) == 0)
		{
			m_wndList.SetCheck(i, TRUE);
			m_wndList.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
			return;
		}
	}
}

void CD2GatewayDlg::OnAdd() 
{
	// TODO: Add your control notification handler code here
	CRealmDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		int nRes = m_ri.AddRealm(dlg.GetRealmName(), dlg.GetBnetIP(), dlg.GetPreferred());

		if (nRes == RI_ERR_OK)
		{
			SetModified(TRUE);
			const int n = m_wndList.InsertItem(INT_MAX, dlg.GetRealmName());
			m_wndList.SetItemText(n, 1, dlg.GetBnetIP());
			m_wndList.SetItemText(n, 2, dlg.GetPreferred());		
		}
		else if (nRes == RI_ERR_ALREADYEXIST)
		{
			CString sErr;
			sErr.Format(_T("国度名\"%s\"已经存在。"), dlg.GetRealmName());
			MessageBox(sErr, _T("重复国度名"), MB_ICONEXCLAMATION);
		}
		else
		{
		}		
	}
}

void CD2GatewayDlg::OnEdit() 
{
	// TODO: Add your control notification handler code here
	if (m_nCurSel < 0)
		return;

	CString sName = m_wndList.GetItemText(m_nCurSel, 0);
	if (m_ri.IsOfficialRealm(sName))
	{
		CString sErr;
		sErr.Format(_T("\"%s\" is a built-in realm which cannot be altered."), sName);
		MessageBox(sErr, _T("Built-in Realm"), MB_ICONEXCLAMATION);
		return;
	}

	CRealmDlg dlg;
	dlg.SetRealmInfo(sName, m_wndList.GetItemText(m_nCurSel, 1), m_wndList.GetItemText(m_nCurSel, 2));
	if (dlg.DoModal() == IDOK)
	{	
		int nRes = m_ri.EditRealm(sName, dlg.GetRealmName(), dlg.GetBnetIP(), dlg.GetPreferred());
		if (nRes == RI_ERR_OK)
		{
			SetModified(TRUE);
			m_wndList.SetItemText(m_nCurSel, 1, dlg.GetBnetIP());
			m_wndList.SetItemText(m_nCurSel, 2, dlg.GetPreferred());
			m_wndList.SetItemText(m_nCurSel, 0, dlg.GetRealmName());
		}
		else if (nRes == RI_ERR_ALREADYEXIST)
		{
			CString sErr;
			sErr.Format(_T("国度名\"%s\"已经存在。"), dlg.GetRealmName());
			MessageBox(sErr, _T("重复国度名"), MB_ICONEXCLAMATION);
		}
		else if (nRes == RI_ERR_BUILTINREALM)
		{
			CString sErr;
			sErr.Format(_T("\"%s\" is a built-in realm which cannot be altered."), sName);
			MessageBox(sErr, _T("Built-in Realm"), MB_ICONEXCLAMATION);
		}
		else
		{
		}
	}
}

void CD2GatewayDlg::OnRemove() 
{
	// TODO: Add your control notification handler code here
	if (m_nCurSel < 0)
		return;

	const CString SNAME = m_wndList.GetItemText(m_nCurSel, 0);
	int nRes = m_ri.RemoveRealm(SNAME);

	if (nRes == RI_ERR_OK)
	{
		SetModified(TRUE);
		const int IDX = m_nCurSel;
		m_wndList.DeleteItem(m_nCurSel);
		if (m_wndList.GetItemCount() > IDX)
			m_wndList.SetItemState(IDX, LVIS_SELECTED, LVIS_SELECTED);
		else
			m_wndList.SetItemState(IDX - 1, LVIS_SELECTED, LVIS_SELECTED);
	}
	else if (nRes == RI_ERR_BUILTINREALM)
	{
		CString sErr;
		sErr.Format(_T("\"%s\" is a built-in realm which cannot be removed."), SNAME);
		MessageBox(sErr, _T("Built-in Realm"), MB_ICONEXCLAMATION);
	}
	else
	{
	}	
}

void CD2GatewayDlg::OnImport() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,
				NULL,
				NULL, 
				OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
				_T("Registry Files(*.reg)|*.reg|All files(*.*)|*.*||"),
				NULL);
	
	if (dlg.DoModal() == IDOK)
	{
		TCHAR szName[512] = _T("");
		TCHAR szBnetIP[512] = _T("");
		TCHAR szPreferred[512] = _T("");	
		int nRes = m_ri.Import(dlg.GetPathName(), szName, szBnetIP, szPreferred);

		if (nRes == RI_ERR_OK)
		{
			SetModified(TRUE);
			int n = m_wndList.InsertItem(INT_MAX, szName);
			m_wndList.SetItemText(n, 1, szBnetIP);
			m_wndList.SetItemText(n, 2, szPreferred);
		}
		else if (nRes == RI_ERR_ALREADYEXIST)
		{
			CString sErr;
			sErr.Format(_T("国度名\"%s\"已经存在。"), szName);
			MessageBox(sErr, _T("重复国度名"), MB_ICONEXCLAMATION);
		}
		else
		{
			CString sErr;
			sErr.Format(_T("无法从文件\"%s\"读取需要的信息。"), dlg.GetPathName());
			MessageBox(sErr, _T("导入"), MB_ICONEXCLAMATION);
		}
	}	
}

void CD2GatewayDlg::OnExport() 
{
	// TODO: Add your control notification handler code here
	if (m_nCurSel < 0)
		return;

	CFileDialog dlg(FALSE,
				_T("reg"),
				NULL, 
				OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
				_T("Registry Files(*.reg)|*.reg|All files(*.*)|*.*||"),
				NULL);

	if (dlg.DoModal() == IDOK)
	{
		int nRes = m_ri.Export(dlg.GetPathName(), m_wndList.GetItemText(m_nCurSel, 0));

		if (nRes == RI_ERR_OK)
		{
		}
		else
		{
			MessageBox(_T("无法导出国度资料到指定的文件。"), _T("导出"), MB_ICONEXCLAMATION);
		}
	}
}

void CD2GatewayDlg::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	POSITION pos = m_wndList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		m_nCurSel = m_wndList.GetNextSelectedItem(pos);
	}
	else
	{
		m_nCurSel = -1;
	}

	UpdateCtrls();

	*pResult = 0;
}

void CD2GatewayDlg::UpdateCtrls()
{
	CString sName = m_wndList.GetItemText(m_nCurSel, 0);
	GetDlgItem(IDC_EDIT)->EnableWindow(m_nCurSel > -1 && !m_ri.IsOfficialRealm(sName));
	GetDlgItem(IDC_REMOVE)->EnableWindow(m_nCurSel > -1 && !m_ri.IsOfficialRealm(sName));
	GetDlgItem(IDC_EXPORT)->EnableWindow(m_nCurSel > -1);
}

void CD2GatewayDlg::SetModified(BOOL bSet)
{
	GetDlgItem(IDC_APPLY)->EnableWindow(bSet);
}

void CD2GatewayDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	if (GetDlgItem(IDC_APPLY)->IsWindowEnabled())
	{
		int nres =MessageBox(_T("You have made changes to the realm data, would you like to save before exit?"), _T("Data Changed"), MB_ICONQUESTION | MB_YESNOCANCEL);
		if (nres == IDCANCEL)
		{
			return;
		}
		else if (nres == IDYES)
		{
			OnApply();
		}
		else
		{
		}
	}

	CDialog::OnCancel();
}

void CD2GatewayDlg::OnApply() 
{
	// TODO: Add your control notification handler code here
	SetModified(FALSE);
	UpdateData(TRUE);
	BOOL bSel = FALSE;
	for (int i = 0; i < m_wndList.GetItemCount(); i++)
	{
		if (m_wndList.GetCheck(i))
		{
			m_ri.SetDefaultRealm(m_wndList.GetItemText(i, 1), m_wndList.GetItemText(i, 2));
			break;
		}
	}

	m_ri.SavePrivateRealms(g_sAppDir + _T("\\D2Gateway.dat"));
	m_ri.DisableSkipToOpen(m_bDisableSkipToOpen);
}

LRESULT CD2GatewayDlg::OnSSCLChanged(WPARAM wParam, LPARAM lParam)
{
	SetModified(TRUE);
	return (LRESULT)0;
}

void CD2GatewayDlg::OnHelpHelp() 
{
	// TODO: Add your command handler code here
	AfxGetApp()->WinHelp(0, 0);
}

void CD2GatewayDlg::OnHelpAbout() 
{
	// TODO: Add your command handler code here
	CAboutDlg dlg;
	dlg.DoModal();
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndAbin.SetURL(_T("mailto:abinn32@163.com"));
	m_wndUrl.SetURL(_T("http://www.cure-team.com/vb/"));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CD2GatewayDlg::OnBatchimport() 
{
	// TODO: Add your control notification handler code here
	CFolderDialog dlg(_T("请选择源文件所在目录"));
	if (dlg.DoModal() != IDOK)
		return;

	DWORD dwCount = 0, dwSucceeded = 0;
	CString sFolder = dlg.GetFolderPath();
	CFileFind ff;
	BOOL b = ff.FindFile(sFolder + _T("\\*.reg"));

	while (b)
	{
		b = ff.FindNextFile();
		CString sFile = ff.GetFilePath();

		dwCount++;

		TCHAR szName[512] = _T("");
		TCHAR szBnetIP[512] = _T("");
		TCHAR szPreferred[512] = _T("");	

		int nRes = m_ri.Import(sFile, szName, szBnetIP, szPreferred);

		if (nRes == RI_ERR_OK)
		{
			SetModified(TRUE);
			dwSucceeded++;
			int n = m_wndList.InsertItem(INT_MAX, szName);
			m_wndList.SetItemText(n, 1, szBnetIP);
			m_wndList.SetItemText(n, 2, szPreferred);
		}		
	}

	// summary
	CString sMsg;
	sMsg.Format(_T("指定的目录下总共找到%d个REG文件，成功导入%d笔数据。"), dwCount, dwSucceeded);
	MessageBox(sMsg, _T("导入"), MB_ICONINFORMATION);

}

void CD2GatewayDlg::OnBatchexport() 
{
	// TODO: Add your control notification handler code here
	CFolderDialog dlg(_T("请选择目标文件存放目录"));
	if (dlg.DoModal() != IDOK)
		return;

	CString sWarning;
	sWarning.Format(_T("您选定了目标文件夹\"%s\"，该目录中如果已经有文件存在，那些文件可能会被永久覆盖。是否继续？"), dlg.GetFolderPath());
	if (MessageBox(sWarning, _T("批量导出"), MB_ICONEXCLAMATION | MB_YESNO) != IDYES)
		return;

	DWORD dwCount = 0, dwSucceeded = 0;
	for (int i = 0; i < m_wndList.GetItemCount(); i++)
	{
		dwCount++;
		CString sName = m_wndList.GetItemText(i, 0);
		CString sFile;
		sFile.Format(_T("%s\\%s.reg"), dlg.GetFolderPath(), sName);
		if (m_ri.Export(sFile, sName) == RI_ERR_OK)
			dwSucceeded++;
	}

	// summary
	CString sMsg;
	sMsg.Format(_T("成功导出%d笔数据到指定目录下。"), dwCount, dwSucceeded);
	MessageBox(sMsg, _T("批量导出"), MB_ICONINFORMATION);
}

BOOL CD2GatewayDlg::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}

void CD2GatewayDlg::ListRealms(int nOpt)
{
	int nIndex = 0;

	while (TRUE)
	{
		TCHAR szName[512] = _T("");
		TCHAR szBnetIP[512] = _T("");
		TCHAR szPreferred[512] = _T("");
		
		if (m_ri.GetRealmByIndex(nIndex++, szName, szBnetIP, szPreferred, nOpt))
		{
			int n = m_wndList.InsertItem(INT_MAX, szName);
			m_wndList.SetItemText(n, 1, szBnetIP);
			m_wndList.SetItemText(n, 2, szPreferred);
		}
		else
		{
			break;
		}
	}
}

void CD2GatewayDlg::OnDisableskiptoopen() 
{
	// TODO: Add your control notification handler code here
	SetModified(TRUE);
}
