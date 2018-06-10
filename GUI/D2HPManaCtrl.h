#if !defined(AFX_D2HPMANACTRL_H__7E3EACDC_3EAB_4A9B_A240_4EA43EFB6544__INCLUDED_)
#define AFX_D2HPMANACTRL_H__7E3EACDC_3EAB_4A9B_A240_4EA43EFB6544__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// D2HPManaCtrl.h : header file
//

#include "D2PlayerStats.h"
#include "StatsDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CD2HPManaCtrl window

class CD2HPManaCtrl : public CStatic
{
// Construction
public:
	CD2HPManaCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CD2HPManaCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetPlayerNameClass(LPCTSTR lpszName, LPCTSTR lpszClass);
	void HideStatsWnd();
	void Stop();
	BOOL Start(DWORD dwInterval);
	void SetStatsPtr(CD2PlayerStats* pStats);
	virtual ~CD2HPManaCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CD2HPManaCtrl)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	void CalcRects();
	static BYTE GetPercent(WORD wVal, WORD wMaxVal);
	void Update(WORD wHP, WORD wMaxHP, WORD wMana, WORD wMaxMana);
	CD2PlayerStats* m_pStats;
	BOOL m_bStarted;

	WORD m_wHP;
	WORD m_wMaxHP;
	BYTE m_iHPPercent;
	WORD m_wMana;
	WORD m_wMaxMana;
	BYTE m_iManaPercent;

	CRect m_rcHP;
	CRect m_rcMana;
	CToolTipCtrl m_wndTooltip;
	CStatsDlg m_statsDlg;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_D2HPMANACTRL_H__7E3EACDC_3EAB_4A9B_A240_4EA43EFB6544__INCLUDED_)
