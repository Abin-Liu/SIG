#if !defined(AFX_DELAYPAGE_H__EE7F51E0_A3E9_47B5_AD44_44280D54854F__INCLUDED_)
#define AFX_DELAYPAGE_H__EE7F51E0_A3E9_47B5_AD44_44280D54854F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DelayPage.h : header file
//
#include "D2ConfigData.h"
#include "EditEx.h"
#include "DescDlg.h"
#include "btnst.h"


/////////////////////////////////////////////////////////////////////////////
// CDelayPage dialog

class CDelayPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CDelayPage)

// Construction
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ApplySettings(CD2ConfigData& data) const;
	void LoadSettings(const CD2ConfigData& data);
	CDelayPage();
	~CDelayPage();

// Dialog Data
	//{{AFX_DATA(CDelayPage)
	enum { IDD = IDD_PAGE_DELAYS };
	CEditEx	m_wndCreateDelayMax;
	CEditEx	m_wndCreateDelayMin;
	CEditEx	m_wndMinConnDelay;
	CButtonST	m_wndHelpAbs;
	CButtonST	m_wndHelMax;
	CEditEx	m_wndLogindelay;
	CEditEx	m_wndLaunchDelay;
	CEditEx	m_wndJoinDelay;
	CEditEx	m_wndErrordelay;
	CEditEx	m_wndCreateBtnDelay;
	CEditEx	m_wndCharSelDelay;
	DWORD	m_dwCharSelDelay;
	DWORD	m_dwErrorDelay;
	DWORD	m_dwJoinDelay;
	DWORD	m_dwLaunchDelay;
	DWORD	m_dwLoginDelay;
	DWORD	m_dwCreateBtnDelay;
	DWORD	m_dwMinConndelay;
	DWORD	m_dwCreateDelayMin;
	DWORD	m_dwCreateDelayMax;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDelayPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDelayPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnDefault();
	afx_msg void OnDestroy();
	afx_msg void OnHelpmaxwait();
	afx_msg void OnHelpabswait();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CToolTipCtrl m_tooltip;
	BOOL m_bPageInitialized;
	CDescDlg* m_pDescDlg;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DELAYPAGE_H__EE7F51E0_A3E9_47B5_AD44_44280D54854F__INCLUDED_)
