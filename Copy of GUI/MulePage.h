#if !defined(AFX_MULEPAGE_H__2E022285_F4A7_45A5_8123_CFD61528A99F__INCLUDED_)
#define AFX_MULEPAGE_H__2E022285_F4A7_45A5_8123_CFD61528A99F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MulePage.h : header file
#include "ReportCtrl.h"
#include "D2Account.h"
#include "d2ConfigData.h"
#include "RealmInfo.h"
#include "EditEx.h"
#include "btnst.h"
//

/////////////////////////////////////////////////////////////////////////////
// CMulePage dialog

class CMulePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CMulePage)

// Construction
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void LoadSettings(const CD2ConfigData& data);
	void ApplySettings(CD2ConfigData& data) const;
	CMulePage();
	~CMulePage();

// Dialog Data
	//{{AFX_DATA(CMulePage)
	enum { IDD = IDD_PAGE_MULE };
	CButtonST	m_wndedit;
	CButtonST	m_wndAdd;
	CButtonST	m_wndremove;
	CButtonST	m_wndMoveDown;
	CButtonST	m_wndMoveUp;
	CEditEx	m_wndMuleCharStayLength;
	CReportCtrl	m_wndList;
	int		m_nMuleCharStayLength;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMulePage)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMulePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnEdit();
	afx_msg void OnRemove();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnMoveup();
	afx_msg void OnMovedown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	

protected:
	CToolTipCtrl m_tooltip;
	void ListAccounts();
	BOOL m_bPageInitialized;
	CArray<CD2Account, const CD2Account&> m_aAccounts;
	CRealmInfo m_ri;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULEPAGE_H__2E022285_F4A7_45A5_8123_CFD61528A99F__INCLUDED_)
