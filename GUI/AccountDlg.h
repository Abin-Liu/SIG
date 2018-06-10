#if !defined(AFX_ACCOUNTDLG_H__47F2AF4D_DB74_4780_9276_D1BF139C763E__INCLUDED_)
#define AFX_ACCOUNTDLG_H__47F2AF4D_DB74_4780_9276_D1BF139C763E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AccountDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAccountDlg dialog

#include "RealmInfo.h"
#include "EditEx.h"

class CAccountDlg : public CDialog
{
// Construction
public:
	void SetCharCount(int nCharCount);
	int GetCharCount() const;
	void SetSubRealm(LPCTSTR lpSubRealm);
	void SetBNetIP(LPCTSTR lpBnetIP);
	void SetPassword(LPCTSTR lpPassword);
	void SetName(LPCTSTR lpName);
	CString GetSubRealm() const;
	CString GetBNetIP() const;
	CString GetPassword() const;
	CString GetName() const;
	CAccountDlg(const CRealmInfo* pri, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAccountDlg)
	enum { IDD = IDD_DLG_ACCOUNT };
	CEditEx	m_wndRepeatPassword;
	CEditEx	m_wndPreferred;
	CEditEx	m_wndPassword;
	CEditEx	m_wndBNetIP;
	CEditEx	m_wndAccount;
	CComboBox	m_wndRealms;
	CString	m_sName;
	int		m_nCharCount;
	CString	m_sPassword;
	CString	m_sRepeatPassword;
	CString	m_sBnetIP;
	CString	m_sSubRealm;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAccountDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAccountDlg)
	afx_msg void OnRealmedit();
	afx_msg void OnRealmrefresh();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeRealms();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void ListRealms();
	const CRealmInfo* m_pri;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCOUNTDLG_H__47F2AF4D_DB74_4780_9276_D1BF139C763E__INCLUDED_)
