#if !defined(AFX_LOGINPAGE_H__1FE97C1A_7209_494C_8DDE_C71448B3838E__INCLUDED_)
#define AFX_LOGINPAGE_H__1FE97C1A_7209_494C_8DDE_C71448B3838E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoginPage.h : header file
//
#include "D2ConfigData.h"
#include "realmInfo.h"
#include "btnst.h"
#include "GrpBtn.h"
#include "EditEx.h"

/////////////////////////////////////////////////////////////////////////////
// CLoginPage dialog

class CLoginPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CLoginPage)

// Construction
public:
	void ApplySettings(CD2ConfigData& data) const;
	void LoadSettings(const CD2ConfigData& data);
	CLoginPage();
	~CLoginPage();

// Dialog Data
	//{{AFX_DATA(CLoginPage)
	enum { IDD = IDD_PAGE_LOGIN };
	CEditEx	m_wndRepeatPassword;
	CEditEx	m_wndPreferred;
	CEditEx	m_wndPassword;
	CEditEx	m_wndBNetIP;
	CEditEx	m_wndAccount;
	CButtonST	m_wndChar8;
	CButtonST	m_wndChar7;
	CButtonST	m_wndChar6;
	CButtonST	m_wndChar5;
	CButtonST	m_wndChar4;
	CButtonST	m_wndChar3;
	CButtonST	m_wndChar2;
	CButtonST	m_wndChar1;
	CComboBox	m_wndRealms;
	CString	m_sAccount;
	CString	m_sPassword;
	CString	m_sRepeatPassword;
	CString	m_sBnetIP;
	CString	m_sSubRealm;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLoginPage)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CLoginPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnRealmedit();
	afx_msg void OnRealmrefresh();
	afx_msg void OnChar1();
	afx_msg void OnChar2();
	afx_msg void OnChar3();
	afx_msg void OnChar4();
	afx_msg void OnChar5();
	afx_msg void OnChar6();
	afx_msg void OnChar7();
	afx_msg void OnChar8();
	afx_msg void OnSelchangeRealms();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void ListRealms();
	BOOL m_bPageInitialized;
	CRealmInfo m_ri;
	CGrpBtn m_btns;
	int m_nCharPosition;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGINPAGE_H__1FE97C1A_7209_494C_8DDE_C71448B3838E__INCLUDED_)
