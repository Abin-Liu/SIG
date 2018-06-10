// D2GatewayDlg.h : header file
//

#if !defined(AFX_D2GATEWAYDLG_H__99557062_25D8_47FE_BDEC_442ABEEDF917__INCLUDED_)
#define AFX_D2GATEWAYDLG_H__99557062_25D8_47FE_BDEC_442ABEEDF917__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SingleSelChkList.h"
#include "realmInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CD2GatewayDlg dialog

class CD2GatewayDlg : public CDialog
{
// Construction
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CD2GatewayDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CD2GatewayDlg)
	enum { IDD = IDD_D2GATEWAY_DIALOG };
	CSingleSelChkList	m_wndList;
	BOOL	m_bDisableSkipToOpen;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CD2GatewayDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CD2GatewayDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAdd();
	afx_msg void OnEdit();
	afx_msg void OnRemove();
	afx_msg void OnImport();
	afx_msg void OnExport();
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnCancel();
	afx_msg void OnApply();
	afx_msg void OnHelpHelp();
	afx_msg void OnHelpAbout();
	afx_msg void OnBatchimport();
	afx_msg void OnBatchexport();
	afx_msg void OnDisableskiptoopen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnSSCLChanged(WPARAM wParam, LPARAM lParam);

protected:
	void ListRealms(int nOpt = RI_LIST_ALL);
	CToolTipCtrl m_tooltip;
	void SetModified(BOOL bSet = TRUE);
	void UpdateCtrls();
	void CheckCurrentRealm();
	CRealmInfo m_ri;
	int m_nCurSel;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_D2GATEWAYDLG_H__99557062_25D8_47FE_BDEC_442ABEEDF917__INCLUDED_)
