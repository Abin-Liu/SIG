#if !defined(AFX_EXITDLG_H__F835E577_9597_4591_A3D1_7443A391D8BE__INCLUDED_)
#define AFX_EXITDLG_H__F835E577_9597_4591_A3D1_7443A391D8BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExitDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExitDlg dialog

class CExitDlg : public CDialog
{
// Construction
public:
	void Initialize(BOOL bCloseD2, BOOL bCloseGUI);
	BOOL IsCloseGUI() const;
	BOOL IsCloseDiabloII() const;
	CExitDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CExitDlg)
	enum { IDD = IDD_EXIT_DLG };
	BOOL	m_bCloseD2;
	BOOL	m_bCloseGUI;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExitDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXITDLG_H__F835E577_9597_4591_A3D1_7443A391D8BE__INCLUDED_)
