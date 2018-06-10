#if !defined(AFX_REALMDLG_H__47EF441B_3016_44CF_AE72_4A2BF9D25A70__INCLUDED_)
#define AFX_REALMDLG_H__47EF441B_3016_44CF_AE72_4A2BF9D25A70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RealmDlg.h : header file
//
#include "EditEx.h"
/////////////////////////////////////////////////////////////////////////////
// CRealmDlg dialog

class CRealmDlg : public CDialog
{
// Construction
public:
	CString GetPreferred() const;
	CString GetBnetIP() const;
	CString GetRealmName() const;
	void SetRealmInfo(LPCTSTR lpName, LPCTSTR lpBnetIP, LPCTSTR lpPreferred);
	CRealmDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRealmDlg)
	enum { IDD = IDD_REALMDLG };
	CEditEx	m_wndPreferred;
	CEditEx	m_wndName;
	CEditEx	m_wndBnetIP;
	CString	m_sBnetIP;
	CString	m_sName;
	CString	m_sPreferred;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRealmDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRealmDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REALMDLG_H__47EF441B_3016_44CF_AE72_4A2BF9D25A70__INCLUDED_)
