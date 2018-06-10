#if !defined(AFX_SINGLESELCHKLIST_H__13224C4D_CEAF_44F1_8A31_2E051A700B9B__INCLUDED_)
#define AFX_SINGLESELCHKLIST_H__13224C4D_CEAF_44F1_8A31_2E051A700B9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SingleSelChkList.h : header file
//

#define WM_SSCL_ITEMCHECKED	(WM_USER + 100)

/////////////////////////////////////////////////////////////////////////////
// CSingleSelChkList window

class CSingleSelChkList : public CListCtrl
{
// Construction
public:
	CSingleSelChkList();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSingleSelChkList)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void UnCheckAll();
	virtual ~CSingleSelChkList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSingleSelChkList)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SINGLESELCHKLIST_H__13224C4D_CEAF_44F1_8A31_2E051A700B9B__INCLUDED_)
