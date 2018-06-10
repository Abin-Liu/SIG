// StatisticsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SIG.h"
#include "StatisticsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticsDlg dialog


CStatisticsDlg::CStatisticsDlg(const CSigStats* pStats, CWnd* pParent /*=NULL*/)
	: m_pStats(pStats), CDialog(CStatisticsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatisticsDlg)
	m_sStartTime = _T("");
	m_sCrashCount = _T("");
	m_sCreateSuccess = _T("");
	m_sEndTime = _T("");
	m_sInGameAvg = _T("");
	m_sInGameMax = _T("");
	m_sInGameMin = _T("");
	m_sTotalTime = _T("");
	m_sTotalInGameTime = _T("");
	m_sCreateTotal = _T("");
	m_sInGameCur = _T("");
	//}}AFX_DATA_INIT
}

void CStatisticsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticsDlg)
	DDX_Text(pDX, IDC_STARTTIME, m_sStartTime);
	DDX_Text(pDX, IDC_CRASH_COUNT, m_sCrashCount);
	DDX_Text(pDX, IDC_CREATE_SUCCESS, m_sCreateSuccess);
	DDX_Text(pDX, IDC_ENDTIME, m_sEndTime);
	DDX_Text(pDX, IDC_INGAME_AVG, m_sInGameAvg);
	DDX_Text(pDX, IDC_INGAME_MAX, m_sInGameMax);
	DDX_Text(pDX, IDC_INGAME_MIN, m_sInGameMin);
	DDX_Text(pDX, IDC_TOTALTIME, m_sTotalTime);
	DDX_Text(pDX, IDC_TOTALINGAMETIME, m_sTotalInGameTime);
	DDX_Text(pDX, IDC_CREATE_TOT, m_sCreateTotal);
	DDX_Text(pDX, IDC_INGAME_CUR, m_sInGameCur);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticsDlg, CDialog)
	//{{AFX_MSG_MAP(CStatisticsDlg)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticsDlg message handlers

BOOL CStatisticsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	LoadData();
	SetTimer(ID_TMR_STATISTICS, 1000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStatisticsDlg::LoadData()
{
	m_sStartTime = m_pStats->m_startTime.Format();

	if (m_pStats->m_stopTime.GetStatus() == COleDateTime::invalid)
	{
		SetDlgItemText(IDC_LBLENDTIME, _T("现在时间："));
		m_sEndTime = COleDateTime::GetCurrentTime().Format();
	}
	else
	{
		SetDlgItemText(IDC_LBLENDTIME, _T("终止时间："));
		m_sEndTime = m_pStats->m_stopTime.Format();
	}

	m_sTotalTime = GetTimeString(m_pStats->m_dwTotalStartTime);
	m_sTotalInGameTime.Format(_T("%s (%.2f%%)"), GetTimeString(m_pStats->m_dwTotalInGameTime), m_pStats->GetInGameRate() * 100.0);
	m_sCrashCount.Format(_T("%d次"), m_pStats->m_dwTotalCrash);
	m_sCreateTotal.Format(_T("%d次"), m_pStats->m_dwTotalCreate);
	m_sCreateSuccess.Format(_T("%d次 (%.2f%%)"), m_pStats->m_dwSuccessCreate, m_pStats->GetCreateRate() * 100.0);
	m_sInGameCur = GetTimeString(m_pStats->m_dwCurGameTime);
	m_sInGameMax = GetTimeString(m_pStats->m_dwMaxInGameTime);
	m_sInGameMin = GetTimeString(m_pStats->m_dwMinInGameTime);
	m_sInGameAvg = GetTimeString(m_pStats->GetAvgInGameTime());
	
	UpdateData(FALSE);
}


void CStatisticsDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	KillTimer(ID_TMR_STATISTICS);
}

void CStatisticsDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == ID_TMR_STATISTICS)
	{
		LoadData();
	}
	
	CDialog::OnTimer(nIDEvent);
}


BOOL CStatisticsDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_LBUTTONDOWN
		|| pMsg->message == WM_RBUTTONDOWN
		|| pMsg->message == WM_MBUTTONDOWN
		|| pMsg->message == WM_KEYDOWN)
	{
		OnCancel();
	}
	return CDialog::PreTranslateMessage(pMsg);
}
