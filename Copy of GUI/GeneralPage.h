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

// Dialog Data
	//{{AFX_DATA(CGeneralPage)
	enum { IDD = IDD_PAGE_GENERAL };
	CButton	m_wndNoPlugin;
	CEdit	m_wndPdirPath;
	CEditEx	m_wndAutoCloseLen;
	CEditEx	m_wndMpqFile;
	CEditEx	m_wndD2Exe;
	CButtonST	m_wndBrowse1;
	CString	m_sD2Executable;
	BOOL	m_bLQ;
	BOOL	m_bNS;
	BOOL	m_bRes800;
	BOOL	m_bUseLocale;
	int		m_nLocaleType;
	BOOL	m_bUseMpq;
	CString	m_sMpqFile;
	BOOL	m_bAutoStart;
	COleDateTime	m_AutoCloseDate;
	BOOL	m_bCloseWindows;
	DWORD	m_dwAutoCloseLen;
	int		m_nAutoCloseType;
	COleDateTime	m_AutoCloseTime;
	BOOL	m_bNoPlugin;
	BOOL	m_bPdir;
	CString	m_sPdirPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CGeneralPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CGeneralPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowse1();
	afx_msg void OnLocale();
	afx_msg void OnMpq();
	afx_msg void OnAutoclose();
	afx_msg void OnNoplugin();
	afx_msg void OnPdir();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void CheckStats();
	HICON m_hBrowse;
	BOOL m_bPageInitialized;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENERALPAGE_H__A529E0EC_DD51_4214_9F79_7B55D3C8EE46__INCLUDED_)
