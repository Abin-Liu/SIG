#if !defined(AFX_DICEDITDLG_H__CE79065C_4883_4D20_B69E_115234398DC4__INCLUDED_)
#define AFX_DICEDITDLG_H__CE79065C_4883_4D20_B69E_115234398DC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DicEditDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDicEditDlg dialog
#define DIC_MINLINES	10

class CDicEditDlg : public CDialog
{
// Construction
public:
	BOOL IsRandDic() const;
	void GetLines(CStringArray& a) const;
	CDicEditDlg(CStringArray* pLines, BOOL bRandDic, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDicEditDlg)
	enum { IDD = IDD_DICEDITDLG };
	CString	m_sContents;
	int		m_nDicUse;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDicEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDicEditDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	int FindDupeString(const CStringArray& a, LPCTSTR lpsz) const;
	const CStringArray* m_pLines;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DICEDITDLG_H__CE79065C_4883_4D20_B69E_115234398DC4__INCLUDED_)
