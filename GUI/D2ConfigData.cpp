#include "stdafx.h"
#include "Sig.h"
#include "D2ConfigData.h"
#include "Ini.h"
#include "Encrypt.h"
#include "strsplit.h"
#include "MD5Checksum.h"
#include "LoaderEx.h"
#include "guid.h"
#include "OSInfo.h"

// game creation related
#define LETTERNUM	36
#define CHANNEL_LETTER	15
#define NAME_LETTER	15
#define PWD_LETTER	15
const TCHAR LETTERS[] = _T("abcdefghijklmnopqrstuvwxyz1234567890");

// Game name policy
enum { NAME_RANDOM = 0, NAME_PREFIX, NAME_DICTIONARY };

CD2ConfigData::CD2ConfigData()
{
	srand(time(NULL));
	m_nMaxD2Proc = 4;
	m_dwLaunchDelay = DEF_DELAY_LAUNCH;
	m_dwLoginDelay = DEF_DELAY_LOGIN;
	m_dwCharSelDelay = DEF_DELAY_CHARSEL;
	m_dwJoinDelay = DEF_DELAY_JOIN;
	m_dwCreateDelayMin = DEF_DELAY_CREATEMIN;
	m_dwCreateDelayMax = DEF_DELAY_CREATEMAX;
	m_dwErrorDelay = DEF_DELAY_ERROR;
	m_dwCreateBtnDelay = DEF_DELAY_CREATEBTN;

	m_nCharPosition = 1;
	m_bD2JSPLoaded = FALSE;

	m_dwMinPwdLen = 1;
	m_dwMaxPwdLen = 5;
	m_nDifficulty = 0;
	m_bAntiIdle = FALSE;
	m_nGameNameType = NAME_RANDOM;

	m_bUseD2JSP = FALSE;
	m_bAutoLoadScripts = FALSE;

	// mule-mode related
	m_nCurMuleAccount = -1;
	m_bMuleModeEnabled = FALSE;
	m_pCurMule = NULL;

	m_bCloseD2 = FALSE;
	m_bCloseGUI = TRUE;
	m_nCurLine = -1;

	m_nJoinRetry = 3;
	m_nMuleCharStayLength = 10000;

	m_bInGameLimit = FALSE;
	m_dwInGameLimitLen = INT_MAX;

	m_dwMinConnDelay = DEF_DELAY_MINCONN;

	m_autoCloseDate = COleDateTime::GetCurrentTime();
	m_dwAutoCloseLen = 1;
	m_bCloseWindows = FALSE;
	m_nAutoCloseType = CLOSE_NONE;
	m_bAutoStart = FALSE;
	m_nCurDicIdx = 0;
	m_bRandDic = TRUE;

	m_bUseFixedPwd = FALSE;
	m_sFixedPwd = _T("123");

	m_sD2Title = MakeD2WndTitle();
	m_dwD2ProcID = 0;
}

CString CD2ConfigData::EncryptPwd() const
{
	return CD2Account::EncryptString(m_sPassword, ((CSIGApp*)AfxGetApp())->m_sUID);
}

CString CD2ConfigData::DecryptPwd(LPCTSTR lpSrc) const
{
	return CD2Account::DecryptString(lpSrc, ((CSIGApp*)AfxGetApp())->m_sUID);
}

void CD2ConfigData::SaveData(LPCTSTR lpSigIni,
							 LPCTSTR lpD2HackitIni,
							 LPCTSTR lpAccountsIni,
							 LPCTSTR lpD2GatewayIni,
							 LPCTSTR lpDictionary) const
{
	CIni ini(lpSigIni);

	ini.WriteString(_T("General"), _T("D2 Executable"), m_sD2Executable);
	ini.WriteString(_T("General"), _T("Parameters"), m_sParamLine);
	ini.WriteUInt(_T("General"), _T("Max Processes"), m_nMaxD2Proc);
	ini.WriteBool(_T("General"), _T("Auto Start"), m_bAutoStart);
	ini.WriteInt(_T("General"), _T("Auto Close Type"), m_nAutoCloseType);
	ini.WriteString(_T("General"), _T("Auto Close Date"), m_autoCloseDate.Format());
	ini.WriteInt(_T("General"), _T("Auto Close Span"), m_dwAutoCloseLen);
	ini.WriteBool(_T("General"), _T("Auto Close Windows"), m_bCloseWindows);

	ini.WriteInt(_T("Delay"), _T("Launch"), m_dwLaunchDelay);
	ini.WriteInt(_T("Delay"), _T("Login"), m_dwLoginDelay);
	ini.WriteInt(_T("Delay"), _T("Char Selection"), m_dwCharSelDelay);
	ini.WriteInt(_T("Delay"), _T("Join"), m_dwJoinDelay);
	ini.WriteInt(_T("Delay"), _T("Create Min"), m_dwCreateDelayMin);
	ini.WriteInt(_T("Delay"), _T("Create Max"), m_dwCreateDelayMax);
	ini.WriteInt(_T("Delay"), _T("Error Clean"), m_dwErrorDelay);
	ini.WriteInt(_T("Delay"), _T("Create Button"), m_dwCreateBtnDelay);
	ini.WriteInt(_T("Delay"), _T("Min Connect"), m_dwMinConnDelay);

	ini.WriteString(_T("Login"), _T("BnetIP"), m_sBnetIP);
	ini.WriteString(_T("Login"), _T("Sub Realm"), m_sSubRealm);
	CRealmInfo::ForceDefaultRealm(m_sBnetIP, m_sSubRealm);
	ini.WriteInt(_T("Login"), _T("Char Position"), m_nCharPosition);
	ini.WriteString(_T("Login"), _T("Account"), m_sAccount);
	ini.WriteString(_T("Login"), _T("Password"), EncryptPwd());
	
	ini.WriteBool(_T("Create"), _T("Anti Idle"), m_bAntiIdle);
	ini.WriteString(_T("Create"), _T("Channel Name"), m_sChannelName);
	ini.WriteBool(_T("Create"), _T("Use Fixed Pwd"), m_bUseFixedPwd);
	ini.WriteString(_T("Create"), _T("Fixed Pwd"), m_sFixedPwd);
	ini.WriteString(_T("Create"), _T("Prefix"), m_sPrefix);
	ini.WriteInt(_T("Create"), _T("Min Pwd Length"), m_dwMinPwdLen);
	ini.WriteInt(_T("Create"), _T("Max Pwd Length"), m_dwMaxPwdLen);
	ini.WriteInt(_T("Create"), _T("Game Difficulty"), m_nDifficulty);
	ini.WriteInt(_T("Create"), _T("Naming Method"), m_nGameNameType);
	ini.WriteInt(_T("Create"), _T("Join Retry"), m_nJoinRetry);
	ini.WriteInt(_T("Create"), _T("MCS Length"), m_nMuleCharStayLength);
	ini.WriteBool(_T("Create"), _T("Game Limit"), m_bInGameLimit);
	ini.WriteInt(_T("Create"), _T("Game Limit Len"), m_dwInGameLimitLen);
	ini.WriteBool(_T("Create"), _T("Rand Dictionary"), m_bRandDic);

	ini.WriteBool(_T("D2JSP"), _T("Attach"), m_bUseD2JSP);
	ini.WriteBool(_T("D2JSP"), _T("Autoload Scripts"), m_bAutoLoadScripts);

	UpdateD2HackitIni(lpD2HackitIni);	
	SaveAccounts(lpAccountsIni);
	m_ri.SavePrivateRealms(lpD2GatewayIni);
	SaveDictionary(m_aNames, GetDictionary());
}

void CD2ConfigData::LoadData(LPCTSTR lpSigIni,
							 LPCTSTR lpD2HackitIni,
							 LPCTSTR lpAccountsIni,
							 LPCTSTR lpD2GatewayIni,
							 LPCTSTR lpDictionary)
{
	CIni ini(lpSigIni);

	m_sD2Executable = ini.GetString(_T("General"), _T("D2 Executable"), _T(""));
	m_sParamLine = ini.GetString(_T("General"), _T("Parameters"), DEF_PARAM);
	m_nMaxD2Proc = ini.GetUInt(_T("General"), _T("Max Processes"), 4);
	m_nAutoCloseType = ini.GetInt(_T("General"), _T("Auto Close Type"), CLOSE_NONE);
	m_autoCloseDate.ParseDateTime(ini.GetString(_T("General"), _T("Auto Close Date"), _T("")));
	m_dwAutoCloseLen = ini.GetInt(_T("General"), _T("Auto Close Span"), 24);
	m_bCloseWindows = ini.GetBool(_T("General"), _T("Auto Close Windows"), FALSE);
	m_bAutoStart = ini.GetBool(_T("General"), _T("Auto Start"), FALSE);

	if ((m_autoCloseDate.GetStatus() == COleDateTime::invalid
		|| m_autoCloseDate <= COleDateTime::GetCurrentTime())
		&& m_nAutoCloseType == CLOSE_BY_DATE)
	{
		m_nAutoCloseType = CLOSE_NONE;
		m_autoCloseDate = COleDateTime::GetCurrentTime();
	}

	if (m_dwAutoCloseLen < 1)
		m_dwAutoCloseLen = 0xfffffffe;

	m_dwLaunchDelay = ini.GetInt(_T("Delay"), _T("Launch"), DEF_DELAY_LAUNCH);
	m_dwLoginDelay = ini.GetInt(_T("Delay"), _T("Login"), DEF_DELAY_LOGIN);
	m_dwCharSelDelay = ini.GetInt(_T("Delay"), _T("Char Selection"), DEF_DELAY_CHARSEL);
	m_dwJoinDelay = ini.GetInt(_T("Delay"), _T("Join"), DEF_DELAY_JOIN);
	m_dwCreateDelayMin = ini.GetInt(_T("Delay"), _T("Create Min"), DEF_DELAY_CREATEMIN);
	m_dwCreateDelayMax = ini.GetInt(_T("Delay"), _T("Create Max"), DEF_DELAY_CREATEMAX);
	CheckCreateDelays();
	m_dwErrorDelay = ini.GetInt(_T("Delay"), _T("Error Clean"), DEF_DELAY_ERROR);
	m_dwCreateBtnDelay = ini.GetInt(_T("Delay"), _T("Create Button"), DEF_DELAY_CREATEBTN);
	m_dwMinConnDelay = ini.GetInt(_T("Delay"), _T("Min Connect"), DEF_DELAY_MINCONN);

	m_sBnetIP = ini.GetString(_T("Login"), _T("BnetIP"), _T(""));
	m_sSubRealm = ini.GetString(_T("Login"), _T("Sub Realm"), _T(""));
	m_nCharPosition = ini.GetInt(_T("Login"), _T("Char Position"), 1);
	m_sAccount = ini.GetString(_T("Login"), _T("Account"), _T(""));
	m_sPassword = DecryptPwd(ini.GetString(_T("Login"), _T("Password"), _T("")));
	m_bAntiIdle = ini.GetBool(_T("Create"), _T("Anti Idle"), FALSE);
	m_sChannelName = ini.GetString(_T("Create"), _T("Channel Name"), _T(""));
	m_bUseFixedPwd = ini.GetBool(_T("Create"), _T("Use Fixed Pwd"), FALSE);
	m_sFixedPwd = ini.GetString(_T("Create"), _T("Fixed Pwd"), _T("123"));
	m_sPrefix = ini.GetString(_T("Create"), _T("Prefix"), _T(""));
	m_dwMinPwdLen = ini.GetInt(_T("Create"), _T("Min Pwd Length"), 1);
	m_dwMaxPwdLen = ini.GetInt(_T("Create"), _T("Max Pwd Length"), 5);
	m_nDifficulty = ini.GetInt(_T("Create"), _T("Game Difficulty"), 0);
	m_nGameNameType = ini.GetInt(_T("Create"), _T("Naming Method"), NAME_RANDOM);
	m_nJoinRetry = ini.GetInt(_T("Create"), _T("Join Retry"), 3);
	m_nMuleCharStayLength = ini.GetInt(_T("Create"), _T("MCS Length"), 10);
	m_bInGameLimit = ini.GetBool(_T("Create"), _T("Game Limit"), FALSE);
	m_dwInGameLimitLen = ini.GetInt(_T("Create"), _T("Game Limit Len"), INT_MAX);
	m_bRandDic = ini.GetBool(_T("Create"), _T("Rand Dictionary"), TRUE);
	
	m_bUseD2JSP = ini.GetBool(_T("D2JSP"), _T("Attach"), FALSE);
	m_bAutoLoadScripts = ini.GetBool(_T("D2JSP"), _T("Autoload Scripts"), FALSE);

	ini.SetPathName(lpD2HackitIni);
	m_aD2HModules.RemoveAll();
	ini.GetArray(_T("Misc"), _T("Autoload"), &m_aD2HModules, _T(","));
	for (int i = m_aD2HModules.GetSize() - 1; i >= 0 ; i--)
	{
		if (m_aD2HModules[i].CompareNoCase(_T("sig")) == 0
			|| m_aD2HModules[i].CompareNoCase(_T("d2jsp")) == 0)
			m_aD2HModules.RemoveAt(i);
	}

	LoadAccounts(lpAccountsIni);
	m_ri.InitPrivateRealms(lpD2GatewayIni);
	LoadDictionary(m_aNames, lpDictionary);

	if (m_nGameNameType == NAME_DICTIONARY && m_aNames.GetSize() < 2)
		m_nGameNameType = NAME_RANDOM;
}

CString CD2ConfigData::GetD2Path() const
{
	CString sPath = m_sD2Executable;
	if (sPath.Find('\\') != -1)
	{
		for (int i = sPath.GetLength() - 1; i >= 0; i--)
		{
			TCHAR ch = sPath[i];
			sPath.Delete(i);
			if (ch == '\\')
				break;	
		}
	}
	else
	{
		sPath = _T("");
	}

	return sPath;
}

CString CD2ConfigData::GenerateGameName()
{
	CString sName;
	switch (m_nGameNameType)
	{
	case NAME_PREFIX:
		sName = GenPrefixName();
		break;

	case NAME_DICTIONARY:
		sName = GenDicName(m_bRandDic);
		break;

	default:
		sName = GenRandName();
		break;
	}

	return sName.IsEmpty() ? GenRandName() : sName;
}

CString CD2ConfigData::GenerateGamePwd() const
{
	if (m_bUseFixedPwd)
		return m_sFixedPwd;
	
	CString sPwd;

	DWORD dwMin = min(m_dwMinPwdLen, m_dwMaxPwdLen);
	DWORD dwMax = max(m_dwMinPwdLen, m_dwMaxPwdLen);

	int nPwdLen = dwMin + (rand() % (1 + dwMax - dwMin));
	TCHAR *szPwd = new TCHAR[nPwdLen + 1];
	for (int j = 0; j < nPwdLen; j++)
		szPwd[j] = LETTERS[rand() % LETTERNUM];
	szPwd[nPwdLen] = _T('\0');
	sPwd = szPwd;
	delete [] szPwd;

	return sPwd;
}

BOOL CD2ConfigData::LaunchDiablo2()
{
	m_bD2JSPLoaded = FALSE;
	if (m_bMuleModeEnabled && m_pCurMule != NULL)
	{
		CRealmInfo::ForceDefaultRealm(m_pCurMule->GetBNetIP(), m_pCurMule->GetSubRealm());
		CRealmInfo::SetDefaultAccount(m_pCurMule->GetName());
	}
	else
	{
		CRealmInfo::ForceDefaultRealm(m_sBnetIP, m_sSubRealm);
		CRealmInfo::SetDefaultAccount(m_sAccount);
	}
	CRealmInfo::DisableSkipToOpen(TRUE);

	::CreateDirectory(GetD2JspBackupDir(), NULL);
	BackupD2JspFile(GetD2JspDir() + _T("\\autod2jsp.exe"));
	BackupD2JspFile(GetD2JspDir() + _T("\\autod2jsp.ini"));
	BackupD2JspFile(GetD2JspDir() + _T("\\autod2jsp_readme.txt"));
	BackupD2JspFile(GetD2JspScriptsDir() + _T("\\autostarter.d2j"));
	BackupD2JspFile(GetD2JspScriptsDir() + _T("\\starter.d2j"));	

	::DeleteFile(GetD2Path() + _T("\\bncache.dat"));	

	/*
	STARTUPINFO si = { 0 }; 
	PROCESS_INFORMATION pi = { 0 };
	
    si.cb = sizeof(si); 
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_MINIMIZE;
	//*/

	TCHAR szParam[1024] = _T("");
	MakeParamLine(szParam, 1023);

    //::CreateProcess(m_sD2Executable, szParam, NULL,NULL,FALSE, 0, NULL, GetD2Path(), &si, &pi);

	m_dwD2ProcID = 0;	
	HANDLE h = ::ShellExecute(NULL, _T("open"), m_sD2Executable, szParam, GetD2Path(), SW_MINIMIZE);

	return ((long)h > 32L);
	//m_dwD2ProcID = pi.dwProcessId;
	//return pi.dwProcessId;
}

BOOL CD2ConfigData::CloseDiablo2()
{
	// terminate diablo ii window
	COSInfo::CloseWnd(FindD2HWnd(), 10000);
	COSInfo::KillProcess(m_dwD2ProcID, 0);
	m_dwD2ProcID = 0;
	return TRUE;
}

BOOL CD2ConfigData::LaunchD2Hackit() const
{	
	CFileFind ff;
	if (!ff.FindFile(m_sD2Executable))
		return FALSE;

	ff.FindNextFile();
	const CString D2FILE = ff.GetFileName();

	HWND hwnd = FindD2HWnd();
	if (hwnd == NULL)
		return FALSE;
	
	if (IsD2HackitLoaded(hwnd, D2FILE))
	{
		UnloadD2Hackit(hwnd, D2FILE, GetHackDir() + _T("\\D2Hackit.dll"));
		Sleep(5000);
	}

	return LoadD2Hackit(hwnd, D2FILE, GetHackDir() + _T("\\D2Hackit.dll"));
/*
	CString sParams;
	sParams.Format(_T("-d2wnd %08x -d2exe \"%s\""), (DWORD)m_hD2Wnd, ff.GetFileName());

	HINSTANCE h = ShellExecute(NULL,
				_T("open"),
				lpFileName,
				sParams,
				lpPath,
				SW_HIDE);
	return ((long)h > 32L);
	*/
}

void CD2ConfigData::UpdateD2HackitIni(LPCTSTR lpD2HackitIni) const
{
	CString sLine(_T("sig"));
	for (int i = 0; i < m_aD2HModules.GetSize(); i++)
	{
		if (!m_aD2HModules[i].IsEmpty()
			&& m_aD2HModules[i].CompareNoCase(_T("d2jsp")) != 0
			&& m_aD2HModules[i].CompareNoCase(_T("sig")) != 0)
		{
			sLine += _T(',');
			sLine += m_aD2HModules[i];
		}
	}

	CIni ini(lpD2HackitIni);
	ini.WriteString(_T("Misc"), _T("Autoload"), sLine);
}

void CD2ConfigData::StartMuleMode()
{	
	m_nCurMuleAccount = -1;
	for (int i = 0; i < m_aMules.GetSize(); i++)
		m_aMules[i].SetActiveChar(0);
	m_bMuleModeEnabled = TRUE;
}

void CD2ConfigData::StopMuleMode()
{
	m_nCurMuleAccount = -2;
	m_bMuleModeEnabled = FALSE;
}

BOOL CD2ConfigData::_GetNextMuleAccount(CString& sAccountName, CString& sPassword)
{
	sAccountName.Empty();
	sPassword.Empty();
	m_nCurMuleAccount++;
	const BOOL bRes = m_nCurMuleAccount >= 0 && m_nCurMuleAccount < m_aMules.GetSize();
	
	if (bRes)
	{
		m_pCurMule = &m_aMules[m_nCurMuleAccount];
		sAccountName = m_pCurMule->GetName();
		sPassword = m_pCurMule->GetPassword();
		m_pCurMule->SetActiveChar(0);
		CRealmInfo::ForceDefaultRealm(m_pCurMule->GetBNetIP(), m_pCurMule->GetSubRealm());
		CRealmInfo::SetDefaultAccount(m_pCurMule->GetName());
	}

	return bRes;	
}

int CD2ConfigData::_GetNextMuleChar()
{
	if (m_pCurMule == NULL || !m_pCurMule->HasMoreChar() || !m_pCurMule->IsEnabled())
		return -1;

	m_pCurMule->MoveToNextChar();
	return m_pCurMule->GetActiveChar();
}

BOOL CD2ConfigData::SaveAccounts(LPCTSTR lpFile) const
{
	CFile file;

	if (!file.Open(lpFile, CFile::modeWrite | CFile::modeCreate))
	{
		return FALSE;
	}
	
	CArchive ar(&file, CArchive::store);
	try
	{
		ar << m_aMules.GetSize();
	}
	catch (CArchiveException* perr)
	{
		perr->Delete();
		ar.Close();
		file.Close();
		return FALSE;
	}

	BOOL bSucceeded = TRUE;
	for (int i = 0; i < m_aMules.GetSize(); i++)
	{
		if (!m_aMules[i].Serialize(ar))
		{
			bSucceeded = FALSE;
			break;
		}
	}

	ar.Close();
	file.Close();
	return bSucceeded;
}

BOOL CD2ConfigData::LoadAccounts(LPCTSTR lpFile)
{
	m_aMules.RemoveAll();
	CFile file;

	if (!file.Open(lpFile, CFile::modeRead))// file not exist
	{
		return TRUE;
	}
	
	int nCount = 0;
	CArchive ar(&file, CArchive::load);
	try
	{
		ar >> nCount;
	}
	catch (CArchiveException* perr)
	{
		perr->Delete();
		nCount = 0;
	}

	BOOL bSucceeded = TRUE;
	if (nCount > 0)
	{
		m_aMules.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
		{
			if (!m_aMules[i].Serialize(ar))
			{
				bSucceeded = FALSE;
				break;
			}
		}
	}

	ar.Close();
	file.Close();
	return bSucceeded;
}

/*
CString CD2ConfigData::GetUniqD2Title() const
{
	CString s;
	s.Format(_T("%s_%s"), CreateFakeGUIDStr(), g_sAppDir);
	s.Replace(_T('\\'), _T('_'));
	s.Replace(_T(':'), _T('_'));
	return s;
}
*/

CString CD2ConfigData::GenRandName() const
{
	int nNameLen = (rand() % 9) + 4;
	TCHAR szName[32] = _T("");

	for (int i = 0; i < nNameLen; i++)
		szName[i] = LETTERS[rand() % LETTERNUM];

	szName[nNameLen] = _T('\0');
	return CString(szName);
}

CString CD2ConfigData::GenPrefixName() const
{
	// we make 1/100 chance to change namenum serial
	if (m_sPrefix.IsEmpty())
		return GenRandName();

	static int nNameNum = 0;
	CString sName;
	if ((rand() % 100) == 0 || nNameNum >= 9999 || nNameNum < 1)
	{
		int n = nNameNum;

		while (abs(n - nNameNum) < 10)
		{
			n = rand() % 1000; // new serial
		}

		nNameNum = n;
	}
	else
	{
		nNameNum++;
	}

	CString sPrefix = m_sPrefix.GetLength() < 10 ? m_sPrefix : m_sPrefix.Left(9);

	if ((rand() % 4) == 0)
		sName.Format(_T("%s %d"), sPrefix, nNameNum);
	else
		sName.Format(_T("%s%d"), sPrefix, nNameNum);

	return sName;
}

int CD2ConfigData::GetNextMule(CString &sAccount, CString &sPassword, int& nCharPosition)
{
	nCharPosition = _GetNextMuleChar();
	BOOL bAccountSwitched = FALSE;

	while (nCharPosition < 1 || nCharPosition > 8)
	{	
		if (_GetNextMuleAccount(sAccount, sPassword))
		{
			bAccountSwitched = TRUE;
			nCharPosition = _GetNextMuleChar();
		}			
		else
		{
			return MULE_NO_MORE;
		}
	}

	if (nCharPosition < 1)
	{
		return MULE_NO_MORE;
	}
	else
	{
		return bAccountSwitched ? MULE_SWITCH_ACCOUNT : MULE_SAME_ACCOUNT;
	}
}

void CD2ConfigData::UpdateMuleCharCount(int nCount)
{
	if (m_pCurMule != NULL)
		m_pCurMule->SetCharCount(nCount);
}

CString CD2ConfigData::GetD2Title() const
{
	return m_sD2Title;
}

void CD2ConfigData::DisableCurrentMule()
{
	if (m_pCurMule != NULL)
	{
		m_pCurMule->Enable(FALSE);
		m_nCharPosition = 65535;
	}
}

int CD2ConfigData::LoadDictionary(CStringArray& arr, LPCTSTR lpFile)
{
	arr.RemoveAll();

	CStdioFile file;

	if (!file.Open(lpFile, CFile::modeRead))
		return 0;

	CString sLine;
	while (file.ReadString(sLine))
	{
		sLine.TrimLeft();
		sLine.TrimRight();
		if (!sLine.IsEmpty() && !IsDicNameExist(arr, sLine))
			arr.Add(sLine);
	}

	file.Close();
	return arr.GetSize();
}

BOOL CD2ConfigData::SaveDictionary(const CStringArray& arr, LPCTSTR lpFile)
{
	CStdioFile file;

	if (!file.Open(lpFile, CFile::modeWrite | CFile::modeCreate))
		return FALSE;

	for (int i = 0; i < arr.GetSize(); i++)
		file.WriteString(arr[i] + _T("\n"));

	file.Close();
	return TRUE;	
}

CString CD2ConfigData::GenDicName(BOOL bRandPick)
{
	CString str;
	if (m_aNames.GetSize() < 4)
		bRandPick = FALSE;

	if (m_aNames.GetSize() < 2)
		return str;

	if (bRandPick)
	{
		// randomly pick a name		
		const int IDX = rand() % (m_aNames.GetSize() - 3);
		str = m_aNames[IDX];
		m_aNames.RemoveAt(IDX);
		m_aNames.Add(str);
	}
	else
	{
		// circle through all names
		if (m_nCurDicIdx < 0 || m_nCurDicIdx >= m_aNames.GetSize())
			m_nCurDicIdx %= m_aNames.GetSize();

		if (m_nCurDicIdx >= 0 && m_nCurDicIdx < m_aNames.GetSize())
			str = m_aNames[m_nCurDicIdx++];
		
	}
	return str;
}

BOOL CD2ConfigData::IsDicNameExist(CStringArray& arr, LPCTSTR lpName)
{
	for (int i = 0; i < arr.GetSize(); i++)
	{
		if (arr[i].CompareNoCase(lpName) == 0)
			return TRUE;
	}

	return FALSE;
}

void CD2ConfigData::CheckCreateDelays()
{
	DWORD dwMin = m_dwCreateDelayMin;
	DWORD dwMax = m_dwCreateDelayMax;

	m_dwCreateDelayMin = min(dwMin, dwMax);
	m_dwCreateDelayMax = max(dwMin, dwMax);
}

DWORD CD2ConfigData::GetCreateDelay() const
{
	if (m_dwCreateDelayMax == m_dwCreateDelayMin)
		return m_dwCreateDelayMin;

	int n = abs((int)m_dwCreateDelayMax - (int)m_dwCreateDelayMin);
	return (rand() % n) + m_dwCreateDelayMin;
}

BOOL CD2ConfigData::LoadD2JSP()
{
	if (m_bD2JSPLoaded)
		return TRUE;

	CString sParam;
	sParam.Format(_T("-title \"%s\" -install"), m_sD2Title);
	HANDLE h = ShellExecute(NULL,
				_T("open"),
				_T("D2JSP.exe"),
				sParam,
				GetD2JspDir(),
				SW_MINIMIZE);

	m_bD2JSPLoaded = ((long)h > 32L);
	return m_bD2JSPLoaded;
}

void CD2ConfigData::BackupD2JspFile(LPCTSTR lpszFile)
{
	if (lpszFile == NULL)
		return;

	CFileFind ff;
	if (ff.FindFile(lpszFile))
	{
		ff.FindNextFile();
		
		::DeleteFile(GetD2JspBackupDir() + _T("\\") + ff.GetFileName());
		::CopyFile(lpszFile, GetD2JspBackupDir() + _T("\\") + ff.GetFileName(), FALSE);
		::DeleteFile(lpszFile);
	}
}

CString CD2ConfigData::GetD2ExeFileTitle() const
{
	CFileFind ff;
	BOOL b = ff.FindFile(m_sD2Executable);
	if (!b)
		return _T("");

	ff.FindNextFile();
	return ff.GetFileTitle();
}

CString CD2ConfigData::GetD2Executable() const
{
	return m_sD2Executable;
}

BOOL CD2ConfigData::LeaveGame() const
{
	HWND hwnd = FindD2HWnd();
	DWORD dwProcID = GetD2ProcID(hwnd);

	if (!hwnd || !dwProcID)
		return FALSE;

	HANDLE hProc = ::OpenProcess(PROCESS_ALL_ACCESS, 0, dwProcID);
	if (hProc == NULL)
		return FALSE;

	DWORD dw0 = 0;
	DWORD dwDummy;
	DWORD oldprot;
	VirtualProtect(LPVOID(0x6FAA0000+0x000D4538), 4, PAGE_READWRITE, &oldprot);	
	BOOL bOK = WriteProcessMemory(hProc, LPVOID(0x6FAA0000+0x000D4538), &dw0, 4, &dwDummy);
	VirtualProtect(LPVOID(0x6FAA0000+0x000D4538), 4, oldprot, &dwDummy);
	::CloseHandle(hProc);

	if (!bOK)
		return FALSE;

	::SendMessage(hwnd, WM_CLOSE, 0, 0);
	return TRUE;
}

BOOL CD2ConfigData::IsD2HackitLoadedAlready() const
{
	CFileFind ff;
	if (!ff.FindFile(m_sD2Executable))
		return FALSE;

	ff.FindNextFile();
	
	return IsD2HackitLoaded(FindD2HWnd(), ff.GetFileName());
}

CString CD2ConfigData::MakeD2WndTitle() const
{
	CString str;
	str.Format(_T("%s - [%s]"), g_sAppDir, CMD5Checksum::GetStringMD5(CreateFakeGUIDStr() + g_sAppDir));
	str.Replace(_T('\\'), _T('/'));
	return str;
}

HWND CD2ConfigData::FindD2HWnd() const
{
	return ::FindWindow(_T("Diablo II"), m_sD2Title);
}

DWORD CD2ConfigData::GetD2ProcID(HWND hwnd) const
{
	if (hwnd == NULL)
		hwnd = FindD2HWnd();
	DWORD dwProcID = 0;
	::GetWindowThreadProcessId(hwnd, &dwProcID);
	return dwProcID;
}

BOOL CD2ConfigData::CleanD2Hackit() const
{
	CFileFind ff;
	if (!ff.FindFile(m_sD2Executable))
		return FALSE;

	HWND hwnd = FindD2HWnd();

	ff.FindNextFile();
	if (!IsD2HackitLoaded(hwnd, ff.GetFileName()))
		return FALSE;

	return UnloadD2Hackit(hwnd, ff.GetFileName(), GetHackDir() + _T("\\D2Hackit.dll"));
}

BOOL CD2ConfigData::IsD2JspLoaded() const
{
	return m_bD2JSPLoaded;
}

CString CD2ConfigData::GetD2FullCmdLine()
{
	TCHAR szParams[1024] = _T("");
	MakeParamLine(szParams, 1023);
	return m_sD2Executable + _T(' ') + szParams;
}

void CD2ConfigData::MakeParamLine(LPTSTR lpBuffer, int nMax)
{
	if (lpBuffer == NULL || nMax <= 0)
		return;

	CString str = m_sParamLine;
	str.MakeLower();

	if (str.Find(_T("-w ")) == -1)
		m_sParamLine += _T(" -w");

	if (str.Find(_T("-skiptobnet")) == -1)
		m_sParamLine += _T(" -skiptobnet");

	_sntprintf(lpBuffer, nMax, _T(" %s -title \"%s\""), m_sParamLine, m_sD2Title);
}	

void CD2ConfigData::SetD2WndFound(HWND hWnd)
{
	::GetWindowThreadProcessId(hWnd, &m_dwD2ProcID);
}
