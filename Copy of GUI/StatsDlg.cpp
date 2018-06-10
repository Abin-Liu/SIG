// StatsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SIG.h"
#include "StatsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatsDlg dialog


CStatsDlg::CStatsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStatsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CStatsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatsDlg)
	DDX_Control(pDX, IDC_LBL, m_wndLabel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatsDlg, CDialog)
	//{{AFX_MSG_MAP(CStatsDlg)
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatsDlg message handlers

BOOL CStatsDlg::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CStatsDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_LBUTTONDOWN
		|| pMsg->message == WM_RBUTTONDOWN
		|| pMsg->message == WM_MBUTTONDOWN
		|| pMsg->message == WM_KEYDOWN)
	{
		ShowWindow(SW_HIDE);
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CStatsDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CBrush brush;
	brush.CreateSolidBrush(::GetSysColor(COLOR_INFOBK));
	CBrush* p = dc.SelectObject(&brush);
	CRect rect;
	GetClientRect(&rect);
	dc.RoundRect(&rect, CPoint(10, 10));
	dc.SelectObject(p);
	brush.DeleteObject();
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CStatsDlg::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	ShowWindow(SW_HIDE);
}

void CStatsDlg::SetStatsPtr(CD2PlayerStats *pStats)
{
	m_pStats = pStats;
}

void CStatsDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if (!bShow || m_pStats == NULL)
		return;

	SetText();
}

void CStatsDlg::SetPlayerNameClass(LPCTSTR lpszName, LPCTSTR lpszClass)
{
	m_sPlayerName = lpszName;
	m_sClass = lpszClass;
}

BOOL CStatsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndLabel.SetBkColor(::GetSysColor(COLOR_INFOBK));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStatsDlg::SetText()
{
	if (m_pStats == NULL)
		return;

	CString sAll, str;
	int nNum = 0;

	sAll.Format(_T("%s\t%s\tLevel %d\n\n"), m_sPlayerName, m_sClass, m_pStats->GetStats(D2PS_LEVEL));	
	
	str.Format(_T("STR %d\tDEX %d\tVIT %d\tENG %d\n"), m_pStats->GetStats(D2PS_STRENGTH), m_pStats->GetStats(D2PS_DESTERITY), m_pStats->GetStats(D2PS_VITALITY), m_pStats->GetStats(D2PS_ENERGY));
	sAll += str;

	str.Format(_T("Life %d/%d\tMana %d/%d\tStamina %d/%d\n"), m_pStats->GetStats(D2PS_HP), m_pStats->GetStats(D2PS_MAXHP), m_pStats->GetStats(D2PS_MANA), m_pStats->GetStats(D2PS_MAXMANA), m_pStats->GetStats(D2PS_STAMINA), m_pStats->GetStats(D2PS_MAXSTAMINA));
	sAll += str;

	nNum = m_pStats->GetStats(D2PS_FIRERESIST);
	str.Format(_T("Fire Resist\t\t%3d/%3d/%3d\n"), nNum, nNum - 40, nNum - 100);
	sAll += str;

	nNum = m_pStats->GetStats(D2PS_COLDRESIST);
	str.Format(_T("Cold Resist\t\t%3d/%3d/%3d\n"), nNum, nNum - 40, nNum - 100);
	sAll += str;

	nNum = m_pStats->GetStats(D2PS_LIGHTNINGRESIST);
	str.Format(_T("Lightning Resist\t%3d/%3d/%3d\n"), nNum, nNum - 40, nNum - 100);
	sAll += str;

	nNum = m_pStats->GetStats(D2PS_POISONRESIST);
	str.Format(_T("Poison Resist\t\t%3d/%3d/%3d\n"), nNum, nNum - 40, nNum - 100);
	sAll += str;

	str.Format(_T("Physical Resist %d\tMagic Resist %d\n"), m_pStats->GetStats(D2PS_DAMAGERESIST), m_pStats->GetStats(D2PS_MAGICRESIST));
	sAll += str;
	
	str.Format(_T("PDR %d\tMDR %d\n"), m_pStats->GetStats(D2PS_DAMAGEREDUCTION), m_pStats->GetStats(D2PS_MAGICDAMAGEREDUCTION));
	sAll += str;

	str.Format(_T("Fire Absorb\t\t%d + %d%%\n"), m_pStats->GetStats(D2PS_FIREABSORB), m_pStats->GetStats(D2PS_FIREABSORBPERCENT));
	sAll += str;

	str.Format(_T("Cold Absorb\t\t%d + %d%%\n"), m_pStats->GetStats(D2PS_COLDABSORB), m_pStats->GetStats(D2PS_COLDABSORBPERCENT));
	sAll += str;

	str.Format(_T("Lightning Absorb\t%d + %d%%\n"), m_pStats->GetStats(D2PS_LIGHTNINGABSORB), m_pStats->GetStats(D2PS_LIGHTNINGABSORBPERCENT));
	sAll += str;


	str.Format(_T("Inventory Gold %d\tStash Gold %d\n"), m_pStats->GetStats(D2PS_GOLD), m_pStats->GetStats(D2PS_GOLDBANK));
	sAll += str;
	
	str.Format(_T("Crushing Blow %d%%\tDeadly Strike %d%%\n"), m_pStats->GetStats(D2PS_CRUSHINGBLOW), m_pStats->GetStats(D2PS_DEADLYSTRIKE));
	sAll += str;

	str.Format(_T("IAS %d%%\tFRW %d%%\tFBR %d%%\tFHR %d%%\tFCR %d%%\n"),
		m_pStats->GetStats(D2PS_IAS), m_pStats->GetStats(D2PS_FASTERRUNWALK), m_pStats->GetStats(D2PS_FASTERBLOCK), m_pStats->GetStats(D2PS_FASTERHITRECOVERY), m_pStats->GetStats(D2PS_FASTERCAST));
	sAll +=str;

	str.Format(_T("Block %d%%\tItem Durability %d/%d\n"), m_pStats->GetStats(D2PS_TOBLOCK), m_pStats->GetStats(D2PS_DURABILITY), m_pStats->GetStats(D2PS_MAXDURABILITY));
	sAll += str;

	str.Format(_T("Magic Find %d%%"), m_pStats->GetStats(D2PS_MAGICFIND));
	sAll +=str;
	
	m_wndLabel.SetText(sAll);
}
