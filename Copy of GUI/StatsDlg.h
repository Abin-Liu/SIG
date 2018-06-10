#if !defined(AFX_STATSDLG_H__820724BC_85AD_4EB3_B4D6_1B91FB613958__INCLUDED_)
#define AFX_STATSDLG_H__820724BC_85AD_4EB3_B4D6_1B91FB613958__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatsDlg.h : header file
#include "Resource.h"
//
#include "D2PlayerStats.h"
#include "Label.h"

/////////////////////////////////////////////////////////////////////////////
// CStatsDlg dialog

class CStatsDlg : public CDialog
{
// Construction
public:
	void SetPlayerNameClass(LPCTSTR lpszName, LPCTSTR lpszClass);
	void SetStatsPtr(CD2PlayerStats* pStats);
	CStatsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStatsDlg)
	enum { IDD = IDD_STATSDLG };
	CLabel	m_wndLabel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatsDlg)
	public:
	virtual BOOL Create(CWnd* pParentWnd);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStatsDlg)
	afx_msg void OnPaint();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void SetText();
	CD2PlayerStats* m_pStats;
	CString m_sPlayerName;
	CString m_sClass;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATSDLG_H__820724BC_85AD_4EB3_B4D6_1B91FB613958__INCLUDED_)
