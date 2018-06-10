// D2HPManaCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "D2HPManaCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define HP_EMPTY_COLOR	RGB(212, 192, 192)
#define HP_FULL_COLOR	RGB(212, 0, 0)
#define MANA_EMPTY_COLOR	RGB(192, 192, 212)
#define MANA_FULL_COLOR	RGB(0, 0, 212)

/////////////////////////////////////////////////////////////////////////////
// CD2HPManaCtrl

CD2HPManaCtrl::CD2HPManaCtrl()
{
	m_bStarted = FALSE;
	m_pStats = NULL;
	m_wHP = 0;
	m_wMaxHP = 0;
	m_iHPPercent = 0;
	m_wMana = 0;
	m_wMaxMana = 0;
	m_iManaPercent = 0;
}

CD2HPManaCtrl::~CD2HPManaCtrl()
{
}


BEGIN_MESSAGE_MAP(CD2HPManaCtrl, CStatic)
	//{{AFX_MSG_MAP(CD2HPManaCtrl)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CD2HPManaCtrl message handlers

void CD2HPManaCtrl::SetStatsPtr(CD2PlayerStats *pStats)
{
	m_pStats = pStats;
	m_statsDlg.SetStatsPtr(pStats);
}

BOOL CD2HPManaCtrl::Start(DWORD dwInterval)
{
	m_wHP = 0;
	m_wMaxHP = 0;
	m_iHPPercent = 0;
	m_wMana = 0;
	m_wMaxMana = 0;
	m_iManaPercent = 0;
	
	if (m_pStats == NULL || dwInterval == 0 || !::IsWindow(m_hWnd))
		return FALSE;

	m_bStarted = TRUE;
	ShowWindow(SW_SHOW);
	SetTimer(0, dwInterval, NULL);
	return TRUE;
}

void CD2HPManaCtrl::Stop()
{
	KillTimer(0);
	m_bStarted = FALSE;
	if (::IsWindow(m_statsDlg.m_hWnd))
		m_statsDlg.ShowWindow(SW_HIDE);
	ShowWindow(SW_HIDE);
}

void CD2HPManaCtrl::OnDestroy() 
{
	CStatic::OnDestroy();
	
	// TODO: Add your message handler code here
	KillTimer(0);
	m_wndTooltip.DestroyWindow();
}

void CD2HPManaCtrl::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 0 && m_bStarted && m_pStats && IsWindowVisible())
	{
		CalcRects();
		WORD wHP = (WORD)m_pStats->GetStats(D2PS_HP);
		WORD wMaxHP = (WORD)m_pStats->GetStats(D2PS_MAXHP);
		WORD wMana = (WORD)m_pStats->GetStats(D2PS_MANA);
		WORD wMaxMana = (WORD)m_pStats->GetStats(D2PS_MAXMANA);

		BOOL bNeedDrawHP = wHP != m_wHP || wMaxHP != m_wMaxHP;
		BOOL bNeedDrawMana = wMana != m_wMana || wMaxMana != m_wMaxMana;

		if (bNeedDrawHP || bNeedDrawMana)
		{
			Update(wHP, wMaxHP, wMana, wMaxMana);

			if (bNeedDrawHP)
				RedrawWindow(&m_rcHP);

			if (bNeedDrawMana)
				RedrawWindow(&m_rcMana);
		}		
	}
	CStatic::OnTimer(nIDEvent);
}

void CD2HPManaCtrl::OnPaint() 
{
	if (!m_bStarted || m_pStats == NULL)
		return;

	CPaintDC dc(this); // device context for painting

	int nWidth = 0;
	CRect rect;

	// Draw HP
	dc.FillSolidRect(&m_rcHP, HP_EMPTY_COLOR);
	nWidth = int((double)m_rcHP.Width() * (double)m_iHPPercent / 100.0);
	rect = m_rcHP;
	rect.left = rect.right - nWidth;
	dc.FillSolidRect(&rect, HP_FULL_COLOR);

	// Draw mana
	dc.FillSolidRect(&m_rcMana, MANA_EMPTY_COLOR);
	nWidth = int((double)m_rcMana.Width() * (double)m_iManaPercent / 100.0);
	rect = m_rcMana;
	rect.left = rect.right - nWidth;
	dc.FillSolidRect(&rect, MANA_FULL_COLOR);
}

void CD2HPManaCtrl::Update(WORD wHP, WORD wMaxHP, WORD wMana, WORD wMaxMana)
{
	m_wHP = wHP;
	m_wMaxHP = wMaxHP;
	m_wMana = wMana;
	m_wMaxMana = wMaxMana;
	m_iHPPercent = GetPercent(m_wHP, m_wMaxHP);
	m_iManaPercent = GetPercent(m_wMana, m_wMaxMana);

	if (::IsWindow(m_wndTooltip.m_hWnd))
	{
		CString str;
		str.Format(_T("生命：%d/%d(%d%%)，魔法：%d/%d(%d%%)。点击更多..."), m_wHP, m_wMaxHP, m_iHPPercent, m_wMana, m_wMaxMana, m_iManaPercent);
		m_wndTooltip.AddTool(this, str);
	}
}

BYTE CD2HPManaCtrl::GetPercent(WORD wVal, WORD wMaxVal)
{
	if (wVal == 0 || wMaxVal == 0)
		return 0;

	return max(1, min(100, (BYTE)((double)wVal * 100.0 / (double)wMaxVal)));
}

void CD2HPManaCtrl::CalcRects()
{
	if (!::IsWindow(m_hWnd))
		return;

	CRect rect;
	GetClientRect(&rect);
	m_rcHP = rect;
	m_rcHP.DeflateRect(0, 0, 0, rect.Height() / 2);
	m_rcMana = rect;
	m_rcMana.DeflateRect(0, rect.Height() / 2, 0, 0);
}

void CD2HPManaCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(0, SS_NOTIFY);
	CStatic::PreSubclassWindow();
	m_wndTooltip.Create(this);
}

BOOL CD2HPManaCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_wndTooltip.RelayEvent(pMsg);
	return CStatic::PreTranslateMessage(pMsg);
}

void CD2HPManaCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CStatic::OnLButtonDown(nFlags, point);
	if (!m_pStats || !m_bStarted)
		return;

	if (!::IsWindow(m_statsDlg.m_hWnd))
		m_statsDlg.Create(this);

	m_statsDlg.ShowWindow(SW_HIDE);

	POINT pt;
	::GetCursorPos(&pt);
	
	CRect rc;
	m_statsDlg.GetWindowRect(&rc);
	
	CRect rc2(pt, CSize(1, 1));
	rc2.InflateRect(rc.Width() / 2, rc.Height() / 2);
	m_statsDlg.MoveWindow(&rc2);
	m_statsDlg.ShowWindow(SW_SHOW);	
}


void CD2HPManaCtrl::HideStatsWnd()
{
	if (::IsWindow(m_statsDlg.m_hWnd))
		m_statsDlg.ShowWindow(SW_HIDE);
}

void CD2HPManaCtrl::SetPlayerNameClass(LPCTSTR lpszName, LPCTSTR lpszClass)
{
	m_statsDlg.SetPlayerNameClass(lpszName, lpszClass);
}
