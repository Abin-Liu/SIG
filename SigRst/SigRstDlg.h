// SigRstDlg.h : header file
//

#if !defined(AFX_SIGRSTDLG_H__3EF65465_E5D4_4B8C_AFE4_DEA2E142E582__INCLUDED_)
#define AFX_SIGRSTDLG_H__3EF65465_E5D4_4B8C_AFE4_DEA2E142E582__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSigRstDlg dialog
#include "btnst.h"

class CSigRstDlg : public CDialog
{
// Construction
public:
	CSigRstDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSigRstDlg)
	enum { IDD = IDD_SIGRST_DIALOG };
	CButtonST	m_wndCancel;
	CString	m_sMsg;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSigRstDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSigRstDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	int m_nRemain;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIGRSTDLG_H__3EF65465_E5D4_4B8C_AFE4_DEA2E142E582__INCLUDED_)
