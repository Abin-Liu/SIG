// D2JspPage.cpp : implementation file
//

#include "stdafx.h"
#include "SIG.h"
#include "D2JspPage.h"
#include "Ini.h"
#include "strsplit.h"
#include "FileManip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CD2JspPage property page

IMPLEMENT_DYNCREATE(CD2JspPage, CPropertyPage)

CD2JspPage::CD2JspPage() : CPropertyPage(CD2JspPage::IDD)
{
	//{{AFX_DATA_INIT(CD2JspPage)
	m_nLoadType = 1;
	m_bUseD2JSP = FALSE;
	//}}AFX_DATA_INIT

	m_bPageInitialized = FALSE;
}

CD2JspPage::~CD2JspPage()
{
}

void CD2JspPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CD2JspPage)
	DDX_Control(pDX, IDC_LIST1, m_wndScripts);
	DDX_Radio(pDX, IDC_AUTOLOAD, m_nLoadType);
	DDX_Check(pDX, IDC_USED2JSP, m_bUseD2JSP);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CD2JspPage, CPropertyPage)
	//{{AFX_MSG_MAP(CD2JspPage)
	ON_BN_CLICKED(IDC_AUTOLOAD, OnAutoload)
	ON_BN_CLICKED(IDC_MANUALLOAD, OnManualload)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
	ON_BN_CLICKED(IDC_USED2JSP, OnUsed2jsp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CD2JspPage message handlers

void CD2JspPage::OnAutoload() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CheckStats();
}

void CD2JspPage::OnManualload() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CheckStats();
}

void CD2JspPage::CheckStats()
{
	m_wndScripts.EnableWindow(m_bUseD2JSP && m_nLoadType == 0);
	GetDlgItem(IDC_AUTOLOAD)->EnableWindow(m_bUseD2JSP);
	GetDlgItem(IDC_MANUALLOAD)->EnableWindow(m_bUseD2JSP);
}

BOOL CD2JspPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_wndScripts.SetExtendedStyle(LVS_EX_CHECKBOXES);
	m_wndScripts.InsertColumn(0, _T("Script Name"), LVCFMT_LEFT, 120);
	m_wndScripts.InsertColumn(1, _T("Target"), LVCFMT_LEFT, 200);

	ListAllAvailableScripts();
	CheckAutoloadScripts();
	CheckStats();

	m_bPageInitialized = TRUE;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CD2JspPage::CheckAutoloadScripts()
{
	CStdioFile file;
	BOOL bFileOpened = FALSE;

	try 
	{
		bFileOpened = file.Open(GetD2JspScriptsDir() + _T("\\default.d2j"), CFile::modeRead);
	} catch (...)
	{
		if (bFileOpened)
			file.Close();

		return;
	}

	if (!bFileOpened)
		return;

	CString sLine;
	while (file.ReadString(sLine))
	{
		sLine.TrimLeft();
		sLine.TrimRight();
		sLine.MakeLower();

		if (sLine.Right(1) != _T(";") || sLine.Left(4) != _T("load"))
			continue;

		sLine.TrimLeft(_T("load"));
		sLine.TrimRight(_T(";"));

		sLine.TrimLeft();
		sLine.TrimRight();
			
		if (sLine.Left(1) != _T("(") || sLine.Right(1) != _T(")"))
			continue;

		sLine.TrimLeft(_T("("));
		sLine.TrimRight(_T(")"));

		sLine.TrimLeft();
		sLine.TrimRight();

		if (sLine.Left(1) != _T("\"") || sLine.Right(1) != _T("\""))
			continue;


		sLine.TrimLeft(_T("\""));
		sLine.TrimRight(_T("\""));

		sLine.TrimLeft();
		sLine.TrimRight();

		if (sLine.Right(4) != _T(".d2j"))
			continue;

		// yes it is
		for (int i = 0; i < m_wndScripts.GetItemCount(); i++)
		{
			CString s = m_wndScripts.GetItemText(i, 1);
			if (m_wndScripts.GetItemText(i, 1).CompareNoCase(sLine) == 0)
					m_wndScripts.SetCheck(i, TRUE);
		}
	}

	file.Close();
}

BOOL CD2JspPage::UpdateAutoloadScripts()
{
	CStdioFile file;
	BOOL bFileOpened = FALSE;

	try 
	{
		bFileOpened = file.Open(GetD2JspScriptsDir() + _T("\\default.d2j"), CFile::modeWrite | CFile::modeCreate);
	} catch (...)
	{
		if (bFileOpened)
			file.Close();

		return FALSE;
	}

	if (!bFileOpened)
		return FALSE;

	COleDateTime now = COleDateTime::GetCurrentTime();
	CString sTimeTag;
	sTimeTag.Format(_T("// Abin, %s\n"), now.Format());
	file.WriteString(_T("//////////////////////////////////////////////////////////////////\n"));
	file.WriteString(_T("//         Default.d2j (D2JSP Entry Point)\n//\n"));
	file.WriteString(_T("// Contents of this file were automatically generated by SIG.\n"));
	file.WriteString(_T("// Please do not modify unless you want to manually configure your\n"));
	file.WriteString(_T("// scripts and load them by yourself.\n//\n"));
	file.WriteString(sTimeTag);
	file.WriteString(_T("//////////////////////////////////////////////////////////////////\n\n"));
	file.WriteString(_T("function main()\n{\n"));

	// insert auto load scripts here
	for (int i = 0; i < m_wndScripts.GetItemCount(); i++)
	{
		if (m_wndScripts.GetCheck(i))
		{
			CString sLine;
			sLine.Format(_T("\tload(\"%s\");\n"), m_wndScripts.GetItemText(i, 1));
			file.WriteString(sLine);
		}
		
	}

	file.WriteString(_T("}\n\n"));
	file.WriteString(_T("//////////////////////////////////////////////////////////////////\n"));
	file.WriteString(_T("//         End of Default.d2j\n"));
	file.WriteString(_T("//////////////////////////////////////////////////////////////////\n\n"));
	file.Close();
	return TRUE;
}

void CD2JspPage::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView != NULL)
	{
		m_wndScripts.SetItemState(pNMListView->iItem, LVIS_SELECTED, LVIS_SELECTED);
	}
	*pResult = 0;
}

void CD2JspPage::LoadSettings(const CD2ConfigData &data)
{
	m_bUseD2JSP = data.m_bUseD2JSP;
	m_nLoadType = data.m_bAutoLoadScripts ? 0 : 1;
}

void CD2JspPage::ApplySettings(CD2ConfigData &data) const
{
	if (m_bPageInitialized)
	{
		data.m_bUseD2JSP = m_bUseD2JSP;
		data.m_bAutoLoadScripts = (m_nLoadType == 0);
	}
}

void CD2JspPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateAutoloadScripts();
	CPropertyPage::OnOK();
}

/*
void CD2JspPage::FindScriptsInFolder(LPCTSTR lpFolder, LPCTSTR lpExclude)
{
	CFileFind ff;
	BOOL b = ff.FindFile(CString(lpFolder) + _T("\\*.*"));

	while (b)
	{
		b = ff.FindNextFile();
		if (ff.IsDots()
			|| (lpExclude != NULL && ff.GetFileName().CompareNoCase(lpExclude) == 0))
			continue;

		if (ff.IsDirectory())
			FindScriptsInFolder(ff.GetFilePath(), lpExclude);

		if (ff.GetFileName().Right(4).CompareNoCase(_T(".d2j")) == 0)
		{
			CString sPath = ff.GetFilePath();
			sPath.Delete(0, GetD2JspScriptsDir().GetLength() + 1);
			sPath.Replace(_T('\\'), _T('/'));
			m_wndScripts.AddItem(ff.GetFileTitle(), sPath);
		}
	}

	ff.Close();
}
*/

void CD2JspPage::ListAllAvailableScripts()
{
	m_wndScripts.DeleteAllItems();
	//FindScriptsInFolder(GetD2JspScriptsDir(), _T("default.d2j"));
	CFileFind ff;
	BOOL b = ff.FindFile(GetD2JspBotsDir() + _T("\\*.d2j"));

	while (b)
	{
		b = ff.FindNextFile();
		if (ff.IsDots() || ff.IsDirectory()
			|| (ff.GetFileTitle().CompareNoCase(_T("default")) == 0))
			continue;

		CString sPath = ff.GetFilePath();
		sPath.Delete(0, GetD2JspScriptsDir().GetLength() + 1);
		sPath.Replace(_T('\\'), _T('/'));
		m_wndScripts.InsertItemEx(INT_MAX, ff.GetFileTitle(), sPath);
	}

	ff.Close();
}

BOOL CD2JspPage::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	g_nActivePage = PAGE_D2JSP;
	return CPropertyPage::OnSetActive();
}

void CD2JspPage::OnUsed2jsp() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CheckStats();
}
