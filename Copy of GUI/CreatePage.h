#if !defined(AFX_CREATEPAGE_H__AB48A476_D99E_4D6D_9245_C4923FBCE296__INCLUDED_)
#define AFX_CREATEPAGE_H__AB48A476_D99E_4D6D_9245_C4923FBCE296__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CreatePage.h : header file
//
#include "D2ConfigData.h"
#include "EditEx.h"
#include "btnst.h"

/////////////////////////////////////////////////////////////////////////////
// CCreatePage dialog

class CCreatePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCreatePage)

// Construction
public:
	BOOL IsAntiIdleEnabled() const;
	void ApplySettings(CD2ConfigData& data) const;
	void LoadSettings(const CD2ConfigData& data);
	CCreatePage();
	~CCreatePage();

// Dialog Data
	//{{AFX_DATA(CCreatePage)
	enum { IDD = IDD_PAGE_CREATE };
	CEditEx	m_wndFixedPwn;
	CButtonST	m_wndDic;
	CEditEx	m_wndInGameLimitLen;
	CEditEx	m_wndRetry;
	CEditEx	m_wndPrefix;
	CEditEx	m_wndMinPwdLen;
	CEditEx	m_wndMaxPwdLen;
	CEditEx	m_wndChannelName;
	DWORD	m_dwMaxPwdLen;
	DWORD	m_dwMinPwdLen;
	CString	m_sPrefix;
	CString	m_sChannelName;
	int		m_nDifficulty;
	BOOL	m_bAntiIdle;
	int		m_nNameType;
	int		m_nRetry;
	BOOL	m_bInGameLimit;
	DWORD	m_dwInGameLimitLength;
	int		m_nUseFixedPwd;
	CString	m_sFixedPwd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCreatePage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCreatePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnDic();
	afx_msg void OnNametype();
	afx_msg void OnIngamelimit();
	afx_msg void OnUsefixpwd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void CheckStats();
	BOOL m_bPageInitialized;
	BOOL m_bRandDic;
	CStringArray m_aLines;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREATEPAGE_H__AB48A476_D99E_4D6D_9245_C4923FBCE296__INCLUDED_)
