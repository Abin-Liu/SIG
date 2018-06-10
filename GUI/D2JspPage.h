#if !defined(AFX_D2JSPPAGE_H__117107E6_9A84_4B1E_80B2_C24189F19C6B__INCLUDED_)
#define AFX_D2JSPPAGE_H__117107E6_9A84_4B1E_80B2_C24189F19C6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// D2JspPage.h : header file
//
#include "BtnST.h"
#include "D2ConfigData.h"
#include "ReportCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CD2JspPage dialog

class CD2JspPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CD2JspPage)

// Construction
public:
	void ApplySettings(CD2ConfigData& data) const;
	void LoadSettings(const CD2ConfigData& data);
	CD2JspPage();
	~CD2JspPage();

// Dialog Data
	//{{AFX_DATA(CD2JspPage)
	enum { IDD = IDD_PAGE_D2JSP };
	CReportCtrl	m_wndScripts;
	int		m_nLoadType;
	BOOL	m_bUseD2JSP;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CD2JspPage)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void CheckAutoloadScripts();
	void CheckStats();
	// Generated message map functions
	//{{AFX_MSG(CD2JspPage)
	afx_msg void OnAutoload();
	afx_msg void OnManualload();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUsed2jsp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void ListAllAvailableScripts();
	void FindScriptsInFolder(LPCTSTR lpFolder, LPCTSTR lpExclude = NULL);
	BOOL UpdateAutoloadScripts();
	BOOL m_bPageInitialized;
	CStringArray m_aScripts;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_D2JSPPAGE_H__117107E6_9A84_4B1E_80B2_C24189F19C6B__INCLUDED_)
