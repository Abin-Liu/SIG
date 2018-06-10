// SIGDlg.h : header file
//

#if !defined(AFX_SIGDLG_H__1F0CC41B_7EB8_4D13_8FD0_DCCCB8753061__INCLUDED_)
#define AFX_SIGDLG_H__1F0CC41B_7EB8_4D13_8FD0_DCCCB8753061__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSIGDlg dialog

#include "SharedMem.h"
#include "Label.h"
#include "d2ConfigData.h"
#include "Ticker.h"
#include "AutoFuncs.h"
#include <afxtempl.h>
#include "BtnST.h"
#include "SystemTray.h"
#include "Logger.h"
#include "sigstats.h"
#include "D2Spy.h"
#include "Gradient.h"
#include "D2HPManaCtrl.h"
#include "D2PlayerStats.h"
#include "ResPack.h"
#include "Guid.h"

class CSIGDlg : public CDialog
{
// Construction
public:
	static CString GetCharClassName(DWORD dwClass, BOOL bLongName = TRUE);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CSIGDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSIGDlg)
	enum { IDD = IDD_SIG_DIALOG };
	CD2HPManaCtrl	m_wndHPManaBar;
	CLabel	m_wndD2JSP;
	CButtonST	m_wndSelFile;
	CButtonST	m_wndViewFile;
	CButtonST	m_wndKillD2;
	CButtonST	m_wndStatistics;
	CButtonST	m_wndShowD2;
	CButtonST	m_wndMuleMode;
	CButtonST	m_wndLocation;
	CLabel	m_wndCharName;
	CButtonST	m_wndPacketsSnd;
	CButtonST	m_wndPacketsRcv;
	CButtonST	m_wndHeartBeat;
	CButtonST	m_wndAntiIdle;
	CButtonST	m_wndHelp;
	CButtonST	m_wndFile;
	CButtonST	m_wndExit;
	CButtonST	m_wndSettings;
	CButtonST	m_wndStop;
	CButtonST	m_wndStart;
	CLabel	m_wndLabel;
	CLabel	m_wndAppVer;
	CLabel	m_wndAppName;
	BOOL	m_bMuleModeEnabled;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSIGDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL SetForegroundWindowEx(HWND hwnd) const;
	void RestoreD2Window(BOOL bBringToTop = FALSE);
	BOOL UpdateFile(CResPack& rp, UINT nResID, LPCTSTR lpszFilePath);
	BOOL CheckFileMd5(LPCTSTR lpszFilePath, LPCTSTR lpszMd5);
	BOOL CheckD2LoaderMd5();
	void HideLightsForD2JSP();
	void TmrProc_Player();
	void CheckGameInOut();
	void OnGameJoin(BYTE iCharClass);
	void CleanNextGameStats();
	int GetNextGameStats(CString& sName, CString& sPwd);
	void ResetSig();
	void CleanLogs();
	BOOL CheckAndCloseErrors();
	void DoConfig(int nActivePage = 0);
	void MuleEnd();
	void MoveD2WndOut();
	void MoveD2WndIn();
	void MoveOn(int nNewState, DWORD dwDelay = 0);
	BOOL CreateSharedMem();
	void PrepareToCloseD2(DWORD dwWait = 5000);
	void PlayerMoving();
	void OnGameLeave(BOOL bRecord = TRUE);

	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSIGDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnStart();
	afx_msg void OnSettings();
	afx_msg void OnStop();
	afx_msg void OnHelpHelp();
	afx_msg void OnHelpAbout();
	afx_msg void OnPopupRestore();
	afx_msg void OnUpdateHelpHelp(CCmdUI* pCmdUI);
	virtual void OnCancel();
	afx_msg void OnMulemode();
	afx_msg void OnStatisticsViewdata();
	afx_msg void OnStatisticsViewlog();
	afx_msg void OnKilld2();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPopupStartpauseresume();
	afx_msg void OnUpdatePopupStartpauseresume(CCmdUI* pCmdUI);
	afx_msg void OnPopupStop();
	afx_msg void OnUpdatePopupStop(CCmdUI* pCmdUI);
	afx_msg void OnPopupKilld2();
	afx_msg void OnUpdatePopupKilld2(CCmdUI* pCmdUI);
	afx_msg void OnViewfile();
	afx_msg void OnSelfile();
	afx_msg void OnShowd2Active();
	afx_msg void OnShowd2Bkgnd();
	afx_msg void OnUpdateShowd2Active(CCmdUI* pCmdUI);
	//}}AFX_MSG

	afx_msg LRESULT OnSIGJoin(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSIGLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSIGCreate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSIGLoaded(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSIGUnloaded(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSIGPause(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSIGResume(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSIGHeartBeat(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSIGMsgString(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSIGStop(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSIGPktRcv(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSIGPktSnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSIGAntiIdle(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSIGLocation(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSIGQuitD2(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSIGNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSIGShowD2(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSIGHideD2(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSIGKey(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
	static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
	static int CheckAndKillxD2();
	void SIGPause(BOOL bPaused);
	void EnableBossMode(BOOL bEnable);
	void CheckSIGinINI();
	int IsFileMissing() const;
	void Terminate(BOOL bUnloadModules = TRUE);
	void TmrProc_CheckStats();
	void TmrProc_Main();
	void ShowInfo(LPCTSTR lpInfo, BOOL bLog = TRUE);
	CSharedMem m_mem;
	CD2ConfigData m_cfg;
	CTicker m_ticker;
	CAutoFunc m_auto;
	int m_nCurState;
	BOOL m_bSigLoaded;
	BOOL m_bGameJoined;
	DWORD m_dwCount;
	DWORD m_dwJoinFail;
	DWORD m_dwDHFail;
	BOOL m_bStarted;
	BOOL m_bPaused;
	BOOL m_bInitialized;
	DWORD m_dwCloseD2Fail;

	CStringArray m_aCriticalFiles;

	CDialog* m_pActiveWnd;
	BOOL m_bBossModeEnabled;
	CSystemTray m_trayIcon;
	CString m_sOrigInfo;

	DWORD m_dwPrevLocation;
	BOOL m_bMovingOn;
	BOOL m_bStartOver;

	// mule-mode related
	int m_nCurMuleChar;
	CString m_sCurMuleAccountName;
	CString m_sCurMulePassword;

	CRect m_rcOrigD2Wnd;

	CString m_sCurGamename;
	int m_nRecentCreateResult;
	CLogger m_logger;
	CString m_sMemoryID;
	int m_nAutoCloseRemain;

	// d2 window placement
	WINDOWPLACEMENT m_wp;
	BOOL m_bPlacementChanged;

	// auto login retry
	DWORD m_dwConnRetry;
	DWORD m_dwLoginFail;

	// app time
	DWORD m_dwAppTime;

	// statistics
	CSigStats m_stats;

	// view file
	CString m_sViewFile;

	CD2Spy m_spy;
	CGradient m_gradient;

	BOOL m_bUseD2JSP;
	CD2PlayerStats m_playerStats;

	// multiple label
//	CStringArray m_aInfo;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIGDLG_H__1F0CC41B_7EB8_4D13_8FD0_DCCCB8753061__INCLUDED_)
