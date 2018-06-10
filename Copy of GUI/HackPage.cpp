// HackPage.cpp : implementation file
//

#include "stdafx.h"
#include "SIG.h"
#include "HackPage.h"
#include "Ini.h"
#include "strsplit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LPCTSTR IGNORE_MODULES[] = { _T("D2Hackit"), _T("SIG"), _T("d2jsp"), NULL };

/////////////////////////////////////////////////////////////////////////////
// CHackPage property page

IMPLEMENT_DYNCREATE(CHackPage, CPropertyPage)

CHackPage::CHackPage() : CPropertyPage(CHackPage::IDD)
{
	//{{AFX_DATA_INIT(CHackPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bPageInitialized = FALSE;
}

CHackPage::~CHackPage()
{
}

void CHackPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHackPage)
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHackPage, CPropertyPage)
	//{{AFX_MSG_MAP(CHackPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHackPage message handlers

BOOL CHackPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_bPageInitialized = TRUE;
	m_wndList.SetExtendedStyle(LVS_EX_CHECKBOXES);
	m_wndList.InsertColumn(0, NULL, LVCFMT_LEFT, 200);

	GetAvailableModuleNames();
	GetAutoLoadModuleNames();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHackPage::GetAvailableModuleNames()
{
	CFileFind ff;

	// d2h
	BOOL b = ff.FindFile(GetHackDir() + _T("\\*.d2h"));
	while (b)
	{
		b = ff.FindNextFile();
		if (!IsInIgnoreList(ff.GetFileTitle()))
			m_wndList.InsertItem(INT_MAX, ff.GetFileTitle());
	}

	// dll
	b = ff.FindFile(GetHackDir() + _T("\\*.dll"));
	while (b)
	{
		b = ff.FindNextFile();
		if (!IsInIgnoreList(ff.GetFileTitle()))
			m_wndList.InsertItem(INT_MAX, ff.GetFileTitle());
	}

	ff.Close();
}

void CHackPage::GetAutoLoadModuleNames()
{	
	for (int i = 0; i < m_aAutoLoads.GetSize(); i++)
	{
		for (int j = 0; j < m_wndList.GetItemCount(); j++)
		{
			if (m_aAutoLoads[i].CompareNoCase(m_wndList.GetItemText(j, 0)) == 0)
				m_wndList.SetCheck(j, TRUE);
		}
	}
}

void CHackPage::UpdateAutoLoadModuleNames()
{
	m_aAutoLoads.RemoveAll();
	for (int i = 0; i < m_wndList.GetItemCount(); i++)
	{
		if (m_wndList.GetCheck(i))
			m_aAutoLoads.Add(m_wndList.GetItemText(i, 0));
	}
}

BOOL CHackPage::IsInIgnoreList(const CString &sName) const
{
	for (int i = 0; IGNORE_MODULES[i] != NULL; i++)
	{
		if (sName.CompareNoCase(IGNORE_MODULES[i]) == 0)
			return TRUE;
	}

	return FALSE;
}

void CHackPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateAutoLoadModuleNames();
	CPropertyPage::OnOK();
}

void CHackPage::LoadSettings(const CD2ConfigData &data)
{
	m_aAutoLoads.Copy(data.m_aD2HModules);
}

void CHackPage::ApplySettings(CD2ConfigData &data) const
{
	if (m_bPageInitialized)
	{
		data.m_aD2HModules.Copy(m_aAutoLoads);
	}
}

BOOL CHackPage::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	g_nActivePage = PAGE_HACKS;
	return CPropertyPage::OnSetActive();
}
