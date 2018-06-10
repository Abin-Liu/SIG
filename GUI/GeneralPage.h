#if !defined(AFX_GENERALPAGE_H__A529E0EC_DD51_4214_9F79_7B55D3C8EE46__INCLUDED_)
#define AFX_GENERALPAGE_H__A529E0EC_DD51_4214_9F79_7B55D3C8EE46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GeneralPage.h : header file
//

#include "D2ConfigData.h"
#include "BtnST.h"
#include "EditEx.h"
#include "BrowseCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CGeneralPage dialog

class CGeneralPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CGeneralPage)

// Construction
public:
	void ApplySettings(CD2ConfigData& data) const;
	void LoadSettings(const CD2ConfigData& data);
	CGeneralPage();
	~CGeneralPage();

	CString m_sD2Executable;
	CString m_sProxyPath;

// Dialog Data
	//{{AFX_DATA(CGeneralPage)
	enum { IDD = IDD_PAGE_GENERAL };
	CEditEx	m_wndMaxD2Proc;
	CEditEx	m_wndParameters;
	CBrowseCtrl	m_wndD2ExePath;
	CEditEx	m_wndAutoCloseLen;
	BOOL	m_bAutoStart;
	COleDateTime	m_AutoCloseDate;
	BOOL	m_bCloseWindows;
	DWORD	m_dwAutoCloseLen;
	int		m_nAutoCloseType;
	COleDateTime	m_AutoCloseTime;
	CString	m_sParameters;
	UINT	m_nMaxD2Proc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CGeneralPage)
	public:
	virtual BOOL OnSetActive();
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CGeneralPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnAutoclose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void CheckStats();
	BOOL m_bPageInitialized;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENERALPAGE_H__A529E0EC_DD51_4214_9F79_7B55D3C8EE46__INCLUDED_)
