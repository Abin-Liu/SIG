#if !defined(AFX_DESCDLG_H__FE029610_4287_495F_B894_6BAEEA7AB2BA__INCLUDED_)
#define AFX_DESCDLG_H__FE029610_4287_495F_B894_6BAEEA7AB2BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DescDlg.h : header file
//

#include "label.h"

/////////////////////////////////////////////////////////////////////////////
// CDescDlg dialog

class CDescDlg : public CDialog
{
// Construction
public:
	void SetHeader(LPCTSTR lpsz);
	void SetText(LPCTSTR lpsz);
	CDescDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDescDlg)
	enum { IDD = IDD_DESCDLG };
	CLabel	m_wndContents;
	CLabel	m_wndHead;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDescDlg)
	public:
	virtual BOOL Create(CWnd* pParentWnd);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDescDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString m_sHeader;
	CString m_sContents;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESCDLG_H__FE029610_4287_495F_B894_6BAEEA7AB2BA__INCLUDED_)
