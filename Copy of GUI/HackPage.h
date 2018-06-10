#if !defined(AFX_HACKPAGE_H__7A891F6F_1F5D_45D6_AFF7_4148A99EE447__INCLUDED_)
#define AFX_HACKPAGE_H__7A891F6F_1F5D_45D6_AFF7_4148A99EE447__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HackPage.h : header file
//
#include "D2ConfigData.h"

/////////////////////////////////////////////////////////////////////////////
// CHackPage dialog

class CHackPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CHackPage)

// Construction
public:
	void ApplySettings(CD2ConfigData& data) const;
	void LoadSettings(const CD2ConfigData &data);
	CHackPage();
	~CHackPage();

// Dialog Data
	//{{AFX_DATA(CHackPage)
	enum { IDD = IDD_PAGE_HACKS };
	CListCtrl	m_wndList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CHackPage)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CHackPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL IsInIgnoreList(const CString& sName) const;
	BOOL m_bPageInitialized;
	CStringArray m_aAutoLoads;

	void UpdateAutoLoadModuleNames();
	void GetAutoLoadModuleNames();
	void GetAvailableModuleNames();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HACKPAGE_H__7A891F6F_1F5D_45D6_AFF7_4148A99EE447__INCLUDED_)
