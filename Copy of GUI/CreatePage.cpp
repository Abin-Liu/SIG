// CreatePage.cpp : implementation file
//

#include "stdafx.h"
#include "SIG.h"
#include "CreatePage.h"
#include "DicEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCreatePage property page

IMPLEMENT_DYNCREATE(CCreatePage, CPropertyPage)

CCreatePage::CCreatePage() : CPropertyPage(CCreatePage::IDD)
{
	//{{AFX_DATA_INIT(CCreatePage)
	m_dwMaxPwdLen = 0;
	m_dwMinPwdLen = 0;
	m_sPrefix = _T("");
	m_sChannelName = _T("");
	m_nDifficulty = 0;
	m_bAntiIdle = FALSE;
	m_nNameType = 0;
	m_nRetry = 0;
	m_bInGameLimit = FALSE;
	m_dwInGameLimitLength = 0;
	m_nUseFixedPwd = 0;
	m_sFixedPwd = _T("123");
	//}}AFX_DATA_INIT

	m_bPageInitialized = FALSE;
	m_bRandDic = TRUE;
}

CCreatePage::~CCreatePage()
{
}

void CCreatePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreatePage)
	DDX_Control(pDX, IDC_FIXEDPWD, m_wndFixedPwn);
	DDX_Control(pDX, IDC_DIC, m_wndDic);
	DDX_Control(pDX, IDC_INGAMELIMITLENGTH, m_wndInGameLimitLen);
	DDX_Control(pDX, IDC_RETRY, m_wndRetry);
	DDX_Control(pDX, IDC_PREFIX, m_wndPrefix);
	DDX_Control(pDX, IDC_MINPWDLEN, m_wndMinPwdLen);
	DDX_Control(pDX, IDC_MAXPWDLEN, m_wndMaxPwdLen);
	DDX_Control(pDX, IDC_CHANNELNAME, m_wndChannelName);
	DDX_Text(pDX, IDC_MAXPWDLEN, m_dwMaxPwdLen);
	DDV_MinMaxDWord(pDX, m_dwMaxPwdLen, 0, 15);
	DDX_Text(pDX, IDC_MINPWDLEN, m_dwMinPwdLen);
	DDV_MinMaxDWord(pDX, m_dwMinPwdLen, 0, 15);
	DDX_Text(pDX, IDC_PREFIX, m_sPrefix);
	DDV_MaxChars(pDX, m_sPrefix, 9);
	DDX_Text(pDX, IDC_CHANNELNAME, m_sChannelName);
	DDV_MaxChars(pDX, m_sChannelName, 15);
	DDX_Radio(pDX, IDC_RADIO1, m_nDifficulty);
	DDX_Check(pDX, IDC_ANTIIDLE, m_bAntiIdle);
	DDX_Radio(pDX, IDC_NAMETYPE, m_nNameType);
	DDX_Text(pDX, IDC_RETRY, m_nRetry);
	DDV_MinMaxInt(pDX, m_nRetry, 0, 999999);
	DDX_Check(pDX, IDC_INGAMELIMIT, m_bInGameLimit);
	DDX_Text(pDX, IDC_INGAMELIMITLENGTH, m_dwInGameLimitLength);
	DDV_MinMaxDWord(pDX, m_dwInGameLimitLength, 5, 4294967295);
	DDX_Radio(pDX, IDC_USEFIXPWD, m_nUseFixedPwd);
	DDX_Text(pDX, IDC_FIXEDPWD, m_sFixedPwd);
	DDV_MaxChars(pDX, m_sFixedPwd, 15);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCreatePage, CPropertyPage)
	//{{AFX_MSG_MAP(CCreatePage)
	ON_BN_CLICKED(IDC_DIC, OnDic)
	ON_BN_CLICKED(IDC_NAMETYPE, OnNametype)
	ON_BN_CLICKED(IDC_INGAMELIMIT, OnIngamelimit)
	ON_BN_CLICKED(IDC_USEFIXPWD, OnUsefixpwd)
	ON_BN_CLICKED(IDC_RADIO5, OnNametype)
	ON_BN_CLICKED(IDC_RADIO6, OnNametype)
	ON_BN_CLICKED(IDC_RADIO10, OnUsefixpwd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreatePage message handlers

void CCreatePage::LoadSettings(const CD2ConfigData &data)
{
	m_sChannelName = data.m_sChannelName;
	m_nUseFixedPwd = data.m_bUseFixedPwd ? 1 : 0;
	m_sFixedPwd = data.m_sFixedPwd;
	m_sPrefix = data.m_sPrefix;
	m_dwMinPwdLen = data.m_dwMinPwdLen;
	m_dwMaxPwdLen = data.m_dwMaxPwdLen;
	m_nDifficulty = data.m_nDifficulty;
	m_nRetry = data.m_nJoinRetry;
	m_bAntiIdle = data.m_bAntiIdle;
	m_nNameType = data.m_nGameNameType;
	m_bInGameLimit = data.m_bInGameLimit;
	m_dwInGameLimitLength = data.m_dwInGameLimitLen;
	CD2ConfigData::LoadDictionary(m_aLines, GetDictionary());
	m_bRandDic = data.m_bRandDic;
}

void CCreatePage::ApplySettings(CD2ConfigData &data) const
{
	if (m_bPageInitialized)
	{
		CD2ConfigData::SaveDictionary(m_aLines, GetDictionary());
		data.m_bRandDic = m_bRandDic;
		data.m_aNames.Copy(m_aLines);
		data.m_nCurDicIdx = 0;
		data.m_sChannelName = m_sChannelName;
		data.m_bUseFixedPwd = m_nUseFixedPwd;
		data.m_sFixedPwd = m_sFixedPwd;
		data.m_sPrefix = m_sPrefix;
		data.m_dwMinPwdLen = m_dwMinPwdLen;
		data.m_dwMaxPwdLen = m_dwMaxPwdLen;
		data.m_nDifficulty = m_nDifficulty;
		data.m_bAntiIdle = m_bAntiIdle;
		data.m_nGameNameType = m_nNameType;
		data.m_dwInGameLimitLen = m_dwInGameLimitLength;
		data.m_bInGameLimit = m_bInGameLimit;
		data.m_nJoinRetry = m_nRetry;

		if (m_nNameType == 2 && m_aLines.GetSize() < 2)
			data.m_nGameNameType = 0;
	}
}

BOOL CCreatePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here	
	m_wndInGameLimitLen.EnableWindow(m_bInGameLimit);
	m_wndDic.SetIcon(IDI_EDITDIC);
	m_wndDic.SetTooltipText(_T("编辑游戏名词典"));
	m_bPageInitialized = TRUE;
	CheckStats();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCreatePage::IsAntiIdleEnabled() const
{
	return m_bAntiIdle;
}

void CCreatePage::OnDic() 
{
	// TODO: Add your control notification handler code here
	CDicEditDlg dlg(&m_aLines, m_bRandDic);
	if (dlg.DoModal() == IDOK)
	{
		CStringArray aTemp;
		dlg.GetLines(aTemp);

		if (aTemp.GetSize() == 0)
		{
			MessageBox(_T("Null input."), _T("Invalid Input"), MB_ICONEXCLAMATION);
			return;
		}
		else if (aTemp.GetSize() < DIC_MINLINES)
		{
			CString sMsg;
			sMsg.Format(_T("您仅仅提供了%d行互不相同的文字，在创建游戏时有可能会与您自己以前创建的游戏重名。建议您最好提供10行以上互不相同的文字。"), aTemp.GetSize());
			MessageBox(sMsg, _T("Warning"), MB_ICONEXCLAMATION);
			m_aLines.Copy(aTemp);
		}
		else
		{
			m_aLines.Copy(aTemp);
		}
		m_bRandDic = dlg.IsRandDic();
	}
}

void CCreatePage::CheckStats()
{
	GetDlgItem(IDC_PREFIX)->EnableWindow(m_nNameType == 1);
	GetDlgItem(IDC_DIC)->EnableWindow(m_nNameType == 2);
	m_wndFixedPwn.EnableWindow(m_nUseFixedPwd == 1);
	m_wndMinPwdLen.EnableWindow(m_nUseFixedPwd == 0);
	m_wndMaxPwdLen.EnableWindow(m_nUseFixedPwd == 0);
}

void CCreatePage::OnNametype() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CheckStats();
}

void CCreatePage::OnIngamelimit() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_wndInGameLimitLen.EnableWindow(m_bInGameLimit);
}

BOOL CCreatePage::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	g_nActivePage = PAGE_CREATE;
	return CPropertyPage::OnSetActive();
}

void CCreatePage::OnUsefixpwd() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_wndFixedPwn.EnableWindow(m_nUseFixedPwd == 1);
	m_wndMinPwdLen.EnableWindow(m_nUseFixedPwd == 0);
	m_wndMaxPwdLen.EnableWindow(m_nUseFixedPwd == 0);
}
