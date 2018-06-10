#ifndef __D2CONFIGDATA_H__
#define __D2CONFIGDATA_H__

#include <afxtempl.h>
#include "D2Account.h"
#include "realmInfo.h"

//default delay
#define DEF_DELAY_LAUNCH	30000
#define DEF_DELAY_LOGIN		15000
#define DEF_DELAY_CHARSEL	10000
#define DEF_DELAY_CHANNEL	1000
#define DEF_DELAY_JOIN		120000
#define DEF_DELAY_EXITAPP	8000
#define DEF_DELAY_ERROR		15000
#define DEF_DELAY_CREATEBTN	2000
#define DEF_DELAY_MINCONN	5000
#define DEF_DELAY_CREATEMIN	5000
#define DEF_DELAY_CREATEMAX	10000

// mule switch results
enum { MULE_NO_MORE = 0, MULE_SAME_ACCOUNT, MULE_SWITCH_ACCOUNT };

// Auto-close types
enum { CLOSE_NONE = 0, CLOSE_BY_DATE, CLOSE_BY_LEN };

class CD2ConfigData
{
public:
	BOOL FindD2HackitInPdir(CString& sFile) const;

	///////////////////////////////////////////////////////////////
	// Constructor & Destructor
	///////////////////////////////////////////////////////////////
	CD2ConfigData();

	///////////////////////////////////////////////////////////////
	// Diablo II Manipulation
	///////////////////////////////////////////////////////////////
	BOOL LaunchDiablo2();
	BOOL CloseDiablo2();
	HWND FindD2HWnd() const;
	DWORD GetD2ProcID(HWND hwnd = NULL) const;
	CString GetD2Executable() const;
	CString GetD2ExeFileTitle() const;
	CString GetD2Path() const;
	CString GetD2Title() const;	
	CString GetD2FullCmdLine() const;

	///////////////////////////////////////////////////////////////
	// D2Hackit Manipulation
	///////////////////////////////////////////////////////////////	
	BOOL LaunchD2Hackit() const;
	BOOL CleanD2Hackit() const;	
	BOOL IsD2HackitLoadedAlready() const;

	///////////////////////////////////////////////////////////////
	// D2Jsp Manipulation
	///////////////////////////////////////////////////////////////	
	BOOL LoadD2JSP();
	BOOL IsD2JspLoaded() const;

	///////////////////////////////////////////////////////////////
	// Game Manipulation
	///////////////////////////////////////////////////////////////	
	BOOL LeaveGame() const;
	DWORD GetCreateDelay() const;
	void CheckCreateDelays();
	CString GenerateGamePwd() const;
	CString GenerateGameName();
	static BOOL SaveDictionary(const CStringArray& arr, LPCTSTR lpFile);
	static int LoadDictionary(CStringArray& arr, LPCTSTR lpFile);
	static BOOL IsDicNameExist(CStringArray& arr, LPCTSTR lpName);

	///////////////////////////////////////////////////////////////
	// Mule Mode Related
	///////////////////////////////////////////////////////////////	
	void DisableCurrentMule();	
	void UpdateMuleCharCount(int nCount);
	int GetNextMule(CString& sAccount, CString& sPassword, int& nCharPosition);		
	void StopMuleMode();	
	void StartMuleMode();

	///////////////////////////////////////////////////////////////
	// Ini File Read & Write
	///////////////////////////////////////////////////////////////	
	void LoadData(LPCTSTR lpSigIni,
				  LPCTSTR lpD2HackitIni,
				  LPCTSTR lpAccountsIni,
				  LPCTSTR lpD2GatewayIni,
				  LPCTSTR lpDictionary);
	void SaveData(LPCTSTR lpSigIni,
				  LPCTSTR lpD2HackitIni,
				  LPCTSTR lpAccountsIni,
				  LPCTSTR lpD2GatewayIni,
				  LPCTSTR lpDictionary) const;


	///////////////////////////////////////////////////////////////////////////
	// Public Member Data (Would be way too inconvenient if set to non-public)
	///////////////////////////////////////////////////////////////////////////	
	CString m_sD2Executable;
	BOOL	m_bLQ;
	BOOL	m_bNS;
	BOOL	m_bRes800;
	BOOL m_bUseLocale;
	int m_nLocaleType;
	BOOL m_bUseMpq;
	CString m_sMpqFile;
	int m_nAutoCloseType;
	COleDateTime m_autoCloseDate;
	DWORD m_dwAutoCloseLen;
	BOOL m_bCloseWindows;
	BOOL m_bAutoStart;
	BOOL m_bNoPlugin;
	BOOL m_bPdir;
	CString m_sPdirPath;

	DWORD m_dwLaunchDelay;
	DWORD m_dwLoginDelay;
	DWORD m_dwCharSelDelay;
	DWORD m_dwJoinDelay;
	DWORD m_dwCreateDelayMin;
	DWORD m_dwCreateDelayMax;
	DWORD m_dwErrorDelay;
	DWORD m_dwCreateBtnDelay;
	DWORD m_dwMinConnDelay;

	CString m_sBnetIP;
	CString m_sSubRealm;
	int m_nCharPosition;
	CString m_sAccount;
	CString m_sPassword;
	BOOL m_bInGameLimit;
	DWORD m_dwInGameLimitLen;

	CString m_sChannelName;
	BOOL m_bUseFixedPwd;
	CString m_sFixedPwd;
	CString m_sPrefix;
	DWORD m_dwMinPwdLen;
	DWORD m_dwMaxPwdLen;
	int m_nDifficulty;
	BOOL m_bAntiIdle;
	int m_nGameNameType;

	CStringArray m_aNames;
	CStringArray m_aD2HModules;
	int m_nCurDicIdx;
	BOOL m_bRandDic;

	int m_nJoinRetry;
	int m_nMuleCharStayLength;

	BOOL m_bUseD2JSP;
	BOOL m_bAutoLoadScripts;	

	// account list for mule mode
	CArray<CD2Account, const CD2Account&> m_aMules;

	// realm info
	CRealmInfo m_ri;

	// exit
	BOOL m_bCloseD2;
	BOOL m_bCloseGUI;	

protected:	

	CString GenDicName(BOOL bRandPick);	
	void BackupD2JspFile(LPCTSTR lpszFile);
	int _GetNextMuleChar();	
	BOOL _GetNextMuleAccount(CString& sAccountName, CString& sPassword);
	CString GetParamLine() const;
	CString GenPrefixName() const;
	CString GenRandName() const;
	BOOL LoadAccounts(LPCTSTR lpFile);
	BOOL SaveAccounts(LPCTSTR lpFile) const;
	CString DecryptPwd(LPCTSTR lpSrc) const;
	CString EncryptPwd() const;
	void UpdateD2HackitIni(LPCTSTR lpD2HackitIni) const;
	CString MakeD2WndTitle() const;

	CString m_sD2Title;	
	int m_nCurMuleAccount;
	BOOL m_bMuleModeEnabled;
	CD2Account* m_pCurMule;
	int m_nCurLine;
	BOOL m_bD2JSPLoaded;
	DWORD m_dwD2ProcID;
};

#endif