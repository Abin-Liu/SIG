#if !defined(AFX_STATISTICSDLG_H__7450EE05_6C52_483F_955D_5D1DEBA9C09B__INCLUDED_)
#define AFX_STATISTICSDLG_H__7450EE05_6C52_483F_955D_5D1DEBA9C09B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatisticsDlg dialog
#include "sigstats.h"

class CStatisticsDlg : public CDialog
{
// Construction
public:
	CStatisticsDlg(const CSigStats* pStats, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStatisticsDlg)
	enum { IDD = IDD_STATISTICSDLG };
	CString	m_sStartTime;
	CString	m_sCrashCount;
	CString	m_sCreateSuccess;
	CString	m_sEndTime;
	CString	m_sInGameAvg;
	CString	m_sInGameMax;
	CString	m_sInGameMin;
	CString	m_sTotalTime;
	CString	m_sTotalInGameTime;
	CString	m_sCreateTotal;
	CString	m_sInGameCur;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatisticsDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void LoadData();

	// Generated message map functions
	//{{AFX_MSG(CStatisticsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	const CSigStats* m_pStats;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICSDLG_H__7450EE05_6C52_483F_955D_5D1DEBA9C09B__INCLUDED_)
