// RealmInfo1.cpp: implementation of the CRealmInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RealmInfo.h"
#include "IniFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const HKEY RI_REG_ROOT = HKEY_CURRENT_USER;
const CString RI_REG_KEY = _T("Software\\Blizzard Entertainment\\Diablo II");
const CString RI_REG_VAL_BNETIP = _T("bnetip");
const CString RI_REG_VAL_PREFERRED = _T("Preferred Realm");
const CString RI_REG_VAL_SKIPTOOPEN = _T("Skip To Open");

// official bnet ip's
LPCTSTR OFFICIAL_IP[] = { _T("uswest.battle.net"),
						  //_T("63.241.83.8"),
						  _T("useast.battle.net"),
						  //_T("63.240.202.128"),
						  _T("asia.battle.net"),
						  //_T("211.233.0.60"),
						  _T("europe.battle.net"),
						  //_T("213.248.106.204"),
						  NULL };

CD2Realm::CD2Realm()
{
	m_sName = _T("");
	m_sBnetIP = _T("");
	m_sPreferred = _T("");
}

CD2Realm::CD2Realm(const CD2Realm& rhs)
{
	m_sName = rhs.m_sName;
	m_sBnetIP = rhs.m_sBnetIP;
	m_sPreferred = rhs.m_sPreferred;
}

CD2Realm::CD2Realm(LPCTSTR lpName, LPCTSTR lpBnetIP, LPCTSTR lpPreferred)
{
	m_sName = lpName;
	m_sBnetIP = lpBnetIP;
	m_sPreferred = lpPreferred == NULL ? lpName : lpPreferred;
}

BOOL CD2Realm::operator==(const CD2Realm& rhs) const
{
	return m_sName.CompareNoCase(rhs.m_sName) == 0;
}

const CD2Realm& CD2Realm::operator=(const CD2Realm& rhs)
{
	m_sName = rhs.m_sName;
	m_sBnetIP = rhs.m_sBnetIP;
	m_sPreferred = rhs.m_sPreferred;
	return *this;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRealmInfo::CRealmInfo()
{
	_InitOfficialRealms();
}

CRealmInfo::CRealmInfo(const CRealmInfo &rhs)
{
	m_aOfficial.Copy(rhs.m_aOfficial);
	m_aPrivate.Copy(rhs.m_aPrivate);
}

CRealmInfo::~CRealmInfo()
{
}

const CRealmInfo& CRealmInfo::operator=(const CRealmInfo &rhs)
{
	m_aOfficial.Copy(rhs.m_aOfficial);
	m_aPrivate.Copy(rhs.m_aPrivate);
	return *this;
}

void CRealmInfo::_InitOfficialRealms()
{
	m_aOfficial.RemoveAll();
	m_aOfficial.Add(CD2Realm(_T("Asia"), _T("asia.battle.net"), _T("Asia")));
	m_aOfficial.Add(CD2Realm(_T("Asia2"), _T("asia.battle.net"), _T("Asia2")));
	m_aOfficial.Add(CD2Realm(_T("Asia3"), _T("asia.battle.net"), _T("Asia3")));

	m_aOfficial.Add(CD2Realm(_T("USEast"), _T("useast.battle.net"), _T("USEast")));
	m_aOfficial.Add(CD2Realm(_T("USWest"), _T("uswest.battle.net"), _T("USWest")));

	m_aOfficial.Add(CD2Realm(_T("Europe"), _T("europe.battle.net"), _T("Europe")));
}

void CRealmInfo::InitPrivateRealms(LPCTSTR lpIniFile)
{
	m_aPrivate.RemoveAll();
	
	CIniFile ini(lpIniFile);
	CStringArray aNames;
	ini.GetProfileSectionNames(aNames);

	for (int i = 0; i < aNames.GetSize(); i++)
	{
		aNames[i].TrimLeft();
		aNames[i].TrimRight();
		if (!aNames[i].IsEmpty())
		{
			CString sBnetIP = ini.GetProfileString(aNames[i], _T("BnetIP"), _T(""));
			CString sPreferred = ini.GetProfileString(aNames[i], _T("Sub Realm"), aNames[i]);
			if (!sBnetIP.IsEmpty())
			{
				m_aPrivate.Add(CD2Realm(aNames[i], sBnetIP, sPreferred));
			}
		}
	}

	TCHAR szBnetIP[256] = _T("");
	TCHAR szPreferred[256] = _T("");
	if (GetDefaultRealm(szBnetIP, szPreferred))
	{
		CString sName;
		if (!_FindRealmName(szBnetIP, szPreferred, sName))
		{
			sName = _tcslen(szPreferred) == 0 ? szBnetIP : szPreferred;
			AddRealm(sName, szBnetIP, szPreferred);
		}
	}
}

BOOL CRealmInfo::IsOfficialRealm(LPCTSTR lpName) const
{
	return _FindInOfficial(lpName) != -1;
}

BOOL CRealmInfo::IsRealmExist(LPCTSTR lpName) const
{
	return IsOfficialRealm(lpName) || IsPrivateRealm(lpName);
}

BOOL CRealmInfo::IsPrivateRealm(LPCTSTR lpName) const
{
	return _FindInPrivate(lpName) != -1;
}

BOOL CRealmInfo::_FindRealmName(LPCTSTR lpBnetIP, LPCTSTR lpPreferred, CString& sName) const
{
	for (int i = 0; i < m_aOfficial.GetSize(); i++)
	{
		if (m_aOfficial[i].m_sBnetIP.CompareNoCase(lpBnetIP) == 0
			&& m_aOfficial[i].m_sPreferred.CompareNoCase(lpPreferred) == 0)
		{
			sName = m_aOfficial[i].m_sName;
			return TRUE;
		}
	}

	for (int j = 0; j < m_aPrivate.GetSize(); j++)
	{
		if (m_aPrivate[j].m_sBnetIP.CompareNoCase(lpBnetIP) == 0
			&& m_aPrivate[j].m_sPreferred.CompareNoCase(lpPreferred) == 0)
		{
			sName = m_aPrivate[j].m_sName;
			return TRUE;
		}
	}

	return FALSE;
}

int CRealmInfo::AddRealm(LPCTSTR lpName, LPCTSTR lpBnetIP, LPCTSTR lpPreferred)
{
	if (IsRealmExist(lpName))
		return RI_ERR_ALREADYEXIST;

	m_aPrivate.Add(CD2Realm(lpName, lpBnetIP, lpPreferred));
	return RI_ERR_OK;
}

int CRealmInfo::RemoveRealm(LPCTSTR lpName)
{
	if (IsOfficialRealm(lpName))
		return RI_ERR_BUILTINREALM;

	for (int i = 0; i < m_aPrivate.GetSize(); i++)
	{
		if (m_aPrivate[i].m_sName.CompareNoCase(lpName) == 0)
		{
			m_aPrivate.RemoveAt(i);
			return RI_ERR_OK;
		}
	}

	return RI_ERR_NOTFOUND;
}

void CRealmInfo::SavePrivateRealms(LPCTSTR lpIniFile) const
{
	// clean the existing ini, if any
	CFile file;
	
	if (file.Open(lpIniFile, CFile::modeCreate | CFile::modeWrite))
		file.Close();
	
	CIniFile ini(lpIniFile);
	for (int i = 0; i < m_aPrivate.GetSize(); i++)
	{
		ini.WriteProfileString(m_aPrivate[i].m_sName, _T("BnetIP"), m_aPrivate[i].m_sBnetIP);
		ini.WriteProfileString(m_aPrivate[i].m_sName, _T("Sub Realm"), m_aPrivate[i].m_sPreferred);
	}
}

int CRealmInfo::EditRealm(LPCTSTR lpOldName, LPCTSTR lpNewName, LPCTSTR lpBnetIP, LPCTSTR lpPreferred)
{
	if (IsOfficialRealm(lpOldName))
		return RI_ERR_BUILTINREALM;

	if (IsRealmExist(lpNewName))
		return RI_ERR_ALREADYEXIST;
	
	for (int i = 0; i < m_aPrivate.GetSize(); i++)
	{
		if (m_aPrivate[i].m_sName.CompareNoCase(lpOldName) == 0)
		{
			m_aPrivate[i].m_sName = lpNewName;
			m_aPrivate[i].m_sBnetIP = lpBnetIP;
			m_aPrivate[i].m_sPreferred = lpPreferred;

			return RI_ERR_OK;
		}
	}

	return RI_ERR_NOTFOUND;
}

int CRealmInfo::_Import(LPCTSTR lpFile, CString& sName, CString& sBnetIP, CString& sPreferred)
{
	// get the file title
	sName.Empty();
	sBnetIP.Empty();
	sPreferred.Empty();

	CString sFile(lpFile);
	sFile.TrimLeft();
	sFile.TrimRight();
	
	if (sFile.IsEmpty() || sFile[sFile.GetLength() - 1] == _T('\\'))
		return RI_ERR_FILE;

	const int nSlash = sFile.ReverseFind(_T('\\'));
	const int nDot = sFile.ReverseFind(_T('.'));
	const int nLen = nDot - nSlash;

	if (nLen == 0)
	{
		// no slash, no dot, so just get the whole thing
		sName = sFile;
	}
	else if (nLen < 0)
	{
		// has slash, no dot, get right portion
		sName = sFile.Mid(nSlash + 1);
	}
	else
	{
		// has both, get mid portion
		sName = sFile.Mid(nSlash + 1, nLen - 1);
	}

	if (IsRealmExist(sName))
		return RI_ERR_ALREADYEXIST;

	CIniFile ini(sFile);
	sBnetIP = ini.GetProfileString(_T("HKEY_CURRENT_USER\\Software\\Blizzard Entertainment\\Diablo II"),
		_T("\"BnetIP\""), _T(""));

	if (sBnetIP.IsEmpty())
		sBnetIP = ini.GetProfileString(_T("HKEY_LOCAL_MACHINE\\Software\\Blizzard Entertainment\\Diablo II"),
		_T("\"BnetIP\""), _T(""));
	
	sBnetIP.TrimLeft(_T("\""));
	sBnetIP.TrimRight(_T("\""));
	sBnetIP.TrimLeft();
	sBnetIP.TrimRight();

	if (sBnetIP.IsEmpty())
		return RI_ERR_FILE;

	sPreferred = ini.GetProfileString(_T("HKEY_CURRENT_USER\\Software\\Blizzard Entertainment\\Diablo II"),
		_T("\"Preferred Realm\""), _T(""));

	if (sPreferred.IsEmpty())
		sPreferred = ini.GetProfileString(_T("HKEY_LOCAL_MACHINE\\Software\\Blizzard Entertainment\\Diablo II"),
		_T("\"Preferred Realm\""), _T(""));

	sPreferred.TrimLeft(_T("\""));
	sPreferred.TrimRight(_T("\""));
	sPreferred.TrimLeft();
	sPreferred.TrimRight();
	if (sPreferred.IsEmpty())
		sPreferred = sName;

	m_aPrivate.Add(CD2Realm(sName, sBnetIP, sPreferred));
	return RI_ERR_OK;
}

int CRealmInfo::Export(LPCTSTR lpFile, LPCTSTR lpName) const
{
	CString sBnetIP, sPreferred;

	if (!_FindRealmDetail(lpName, sBnetIP, sPreferred))
		return RI_ERR_NOTFOUND;

	CStdioFile file;
	if (!file.Open(lpFile, CFile::modeWrite | CFile::modeCreate))
		return RI_ERR_FILE;

	file.WriteString(_T("REGEDIT4\n"));
	file.WriteString(_T("\n"));
	file.WriteString(_T("[HKEY_CURRENT_USER\\Software\\Blizzard Entertainment\\Diablo II]\n"));

	CString sLine;

	if (!IsOfficialRealm(lpName))
	{
		sLine.Format(_T("\"BnetIP\"=\"%s\"\n"), sBnetIP);
		file.WriteString(sLine);
	}

	sLine.Format(_T("\"Preferred Realm\"=\"%s\""), sPreferred);
	file.WriteString(sLine);

	file.Close();
	return RI_ERR_OK;
}

int CRealmInfo::_FindInPrivate(LPCTSTR lpName) const
{
	for (int i = 0; i < m_aPrivate.GetSize(); i++)
	{
		if (m_aPrivate[i].m_sName.CompareNoCase(lpName) == 0)
			return i;
	}

	return -1;
}

int CRealmInfo::_FindInOfficial(LPCTSTR lpName) const
{
	for (int i = 0; i < m_aOfficial.GetSize(); i++)
	{
		if (m_aOfficial[i].m_sName.CompareNoCase(lpName) == 0)
			return i;
	}

	return -1;
}

BOOL CRealmInfo::_FindRealmDetail(LPCTSTR lpName, CString &sBnetIP, CString &sPreferred) const
{
	BOOL bOfficial = TRUE;
	int nIndex = _FindInOfficial(lpName);
	sBnetIP.Empty();
	sPreferred.Empty();

	if (nIndex == -1)
	{
		bOfficial = FALSE;
		nIndex = _FindInPrivate(lpName);
	}

	if (nIndex == -1)
		return FALSE;

	if (bOfficial)
	{
		sBnetIP = m_aOfficial[nIndex].m_sBnetIP;
		sPreferred = m_aOfficial[nIndex].m_sPreferred;
	}
	else
	{
		sBnetIP = m_aPrivate[nIndex].m_sBnetIP;
		sPreferred = m_aPrivate[nIndex].m_sPreferred;
	}

	return TRUE;
}

BOOL CRealmInfo::DisableSkipToOpen(BOOL bDisable /*=TRUE*/)
{
	HKEY hKey = NULL;
	if (ERROR_SUCCESS != ::RegOpenKeyEx(RI_REG_ROOT, RI_REG_KEY, 0, KEY_SET_VALUE, &hKey))
		return FALSE;

	DWORD dwVal = bDisable ? 0 : 1;
	long lRes = ::RegSetValueEx(hKey, RI_REG_VAL_SKIPTOOPEN, 0, REG_DWORD, (LPBYTE)&dwVal, sizeof(DWORD));
	::RegCloseKey(hKey);

	return lRes == ERROR_SUCCESS;
}

BOOL CRealmInfo::GetDefaultRealm(LPTSTR szBnetIP, LPTSTR szPreferred)
{
	szBnetIP[0] = _T('\0');
	HKEY hKey = NULL;
	if (ERROR_SUCCESS != ::RegOpenKeyEx(RI_REG_ROOT, RI_REG_KEY, 0, KEY_QUERY_VALUE, &hKey))
		return FALSE;

	_tcsncpy(szBnetIP, _T(""), 1);
	_tcsncpy(szPreferred, _T(""), 1);
	DWORD dwBnetIP = 255 * sizeof(TCHAR), dwPreferred = 255 * sizeof(TCHAR);
	DWORD dwType = REG_SZ;

	::RegQueryValueEx(hKey, RI_REG_VAL_BNETIP, 0, &dwType, (LPBYTE)szBnetIP, &dwBnetIP);
	::RegQueryValueEx(hKey, RI_REG_VAL_PREFERRED, 0, &dwType, (LPBYTE)szPreferred, &dwPreferred);
	::RegCloseKey(hKey);

	if (_tcslen(szBnetIP) == 0)
	{
		_GetOfficialBNetIP(szBnetIP, szPreferred);
	}
	return TRUE;
}

BOOL CRealmInfo::SetDefaultRealm(LPCTSTR lpBnetIP, LPCTSTR lpPreferred)
{
	if (lpBnetIP == NULL || _tcslen(lpBnetIP) == 0)
		return FALSE;

	BOOL bRes = TRUE;
	const BOOL bOfficial = _IsOfficialIP(lpBnetIP);
	if (bOfficial)
	{
		bRes &= BNetIPCleanup(lpBnetIP);
		bRes &= _SetMultiSz(lpBnetIP);
	}

	HKEY hKey = NULL;
	if (ERROR_SUCCESS != ::RegOpenKeyEx(RI_REG_ROOT, RI_REG_KEY, 0, KEY_SET_VALUE, &hKey))
		return FALSE;	

	if (!bOfficial)
		bRes &= (ERROR_SUCCESS == ::RegSetValueEx(hKey, RI_REG_VAL_BNETIP, 0, REG_SZ, (LPBYTE)lpBnetIP, _tcslen(lpBnetIP) * sizeof(TCHAR)));

	if (lpPreferred != NULL && _tcslen(lpPreferred) > 0)
		bRes &= (ERROR_SUCCESS == ::RegSetValueEx(hKey, RI_REG_VAL_PREFERRED, 0, REG_SZ, (LPBYTE)lpPreferred, _tcslen(lpPreferred) * sizeof(TCHAR)));

	::RegCloseKey(hKey);
	return bRes;
}

int CRealmInfo::GetRealmCount(int nOpt) const
{
	if (nOpt == RI_LIST_OFFICIALONLY)
	{
		return m_aOfficial.GetSize();
	}
	else if (nOpt == RI_LIST_PRIVATEONLY)
	{
		return m_aPrivate.GetSize();
	}
	else
	{
		return m_aOfficial.GetSize() + m_aPrivate.GetSize();
	}
}

BOOL CRealmInfo::GetRealmByIndex(int nIndex, LPTSTR lpName, LPTSTR lpBnetIP, LPTSTR lpPreferred, int nOpt) const
{
	const int OFFICIALCOUNT = m_aOfficial.GetSize();
	const int PRIVATECOUNT = m_aPrivate.GetSize();

	if (nOpt == RI_LIST_OFFICIALONLY)
	{
		if (nIndex >= OFFICIALCOUNT)
			return FALSE;

		_tcscpy(lpName, m_aOfficial[nIndex].m_sName);
		_tcscpy(lpBnetIP, m_aOfficial[nIndex].m_sBnetIP);
		_tcscpy(lpPreferred, m_aOfficial[nIndex].m_sPreferred);
		return TRUE;
	}
	else if (nOpt == RI_LIST_PRIVATEONLY)
	{
		if (nIndex >= PRIVATECOUNT)
			return FALSE;
		_tcscpy(lpName, m_aPrivate[nIndex].m_sName);
		_tcscpy(lpBnetIP, m_aPrivate[nIndex].m_sBnetIP);
		_tcscpy(lpPreferred, m_aPrivate[nIndex].m_sPreferred);
		return TRUE;
	}
	else
	{
		if (nIndex < OFFICIALCOUNT)
		{
			_tcscpy(lpName, m_aOfficial[nIndex].m_sName);
			_tcscpy(lpBnetIP, m_aOfficial[nIndex].m_sBnetIP);
			_tcscpy(lpPreferred, m_aOfficial[nIndex].m_sPreferred);
			return TRUE;
		}
		else if (nIndex < OFFICIALCOUNT + PRIVATECOUNT)
		{
			_tcscpy(lpName, m_aPrivate[nIndex - OFFICIALCOUNT].m_sName);
			_tcscpy(lpBnetIP, m_aPrivate[nIndex - OFFICIALCOUNT].m_sBnetIP);
			_tcscpy(lpPreferred, m_aPrivate[nIndex - OFFICIALCOUNT].m_sPreferred);
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

BOOL CRealmInfo::FindRealmName(LPCTSTR lpBnetIP, LPCTSTR lpPreferred, LPTSTR lpName) const
{
	CString sName;
	BOOL bRes = _FindRealmName(lpBnetIP, lpPreferred, sName);
	if (bRes)
		_tcscpy(lpName, sName);
	return bRes;
}

BOOL CRealmInfo::FindRealmDetail(LPCTSTR lpName, LPTSTR lpBnetIP, LPTSTR lpPreferred) const
{
	CString sBnetIP, sPreferred;
	BOOL bRes = _FindRealmDetail(lpName, sBnetIP, sPreferred);
	if (bRes)
	{
		_tcscpy(lpBnetIP, sBnetIP);
		_tcscpy(lpPreferred, sPreferred);
	}
	return bRes;
}

int CRealmInfo::Import(LPCTSTR lpFile, LPTSTR lpName, LPTSTR lpBnetIP, LPTSTR lpPreferred)
{
	CString sName, sBnetIP, sPreferred;
	int nRes = _Import(lpFile, sName, sBnetIP, sPreferred);

	if (nRes == RI_ERR_OK)
	{
		_tcscpy(lpName, sName);
		_tcscpy(lpBnetIP, sBnetIP);
		_tcscpy(lpPreferred, sPreferred);
	}

	return nRes;
}

BOOL CRealmInfo::IsSkipToOpenDisabled(BOOL& bDisabled)
{
	HKEY hKey = NULL;
	if (ERROR_SUCCESS != ::RegOpenKeyEx(RI_REG_ROOT, RI_REG_KEY, 0, KEY_QUERY_VALUE, &hKey))
		return FALSE;

	DWORD dwValue = 0;
	DWORD dwType = REG_DWORD;
	DWORD dwSize = sizeof(DWORD);
	const long lRes = ::RegQueryValueEx(hKey, RI_REG_VAL_SKIPTOOPEN, 0, &dwType, (LPBYTE)&dwValue, &dwSize);
	::RegCloseKey(hKey);

	bDisabled = (dwValue == 0);

	return lRes == ERROR_SUCCESS;
}

BOOL CRealmInfo::_IsOfficialIP(LPCTSTR lpBNetIP)
{
	CString sIP(lpBNetIP);
	for (int i = 0; OFFICIAL_IP[i] != NULL; i++)
	{
		if (sIP.CompareNoCase(OFFICIAL_IP[i]) == 0)
			return TRUE;
	}

	return FALSE;
}

BOOL CRealmInfo::_SetMultiSz(LPCTSTR lpBNetIP)
{
	CStringArray arr;
	HKEY hKey = NULL;
	if (ERROR_SUCCESS != ::RegOpenKeyEx(HKEY_CURRENT_USER,
		_T("Software\\Battle.net\\Configuration"),
		0,
		KEY_QUERY_VALUE | KEY_SET_VALUE,
		&hKey))
		return FALSE;

	TCHAR szBuf[1025] = _T("");
	TCHAR* p = szBuf;
	DWORD dwLen = 1024 * sizeof(TCHAR);
	DWORD dwType = REG_MULTI_SZ;

	::RegQueryValueEx(hKey, _T("Diablo II Battle.net gateways"), NULL, &dwType, (LPBYTE)szBuf, &dwLen);

	while (p[0] != _T('\0'))
	{
		arr.Add(p);
		p += _tcslen(p) + 1;
	}

	p = szBuf + _tcslen(szBuf) + 1;
	const int IDX = _FindMatchIPFromMultiSz(arr, lpBNetIP);
	BOOL bRes = FALSE;
	if (p[0] != _T('\0') && IDX > 0)
	{
		_stprintf(p, _T("%02d"), IDX);
		bRes = (ERROR_SUCCESS == ::RegSetValueEx(hKey, _T("Diablo II Battle.net gateways"), NULL, REG_MULTI_SZ, (const BYTE*)szBuf, dwLen));
	}

	::RegCloseKey(hKey);

	return bRes;
}

BOOL CRealmInfo::BNetIPCleanup(LPCTSTR lpBNetIP)
{
	if (!_IsOfficialIP(lpBNetIP))
		return TRUE;

	HKEY hKey = NULL;
	if (ERROR_SUCCESS != ::RegOpenKeyEx(RI_REG_ROOT, RI_REG_KEY, 0, KEY_SET_VALUE, &hKey))
		return FALSE;

	BOOL bRes = (ERROR_SUCCESS == ::RegDeleteValue(hKey, RI_REG_VAL_BNETIP));

	::RegCloseKey(hKey);

	return bRes;
}

int CRealmInfo::_FindMatchIPFromMultiSz(const CStringArray &arr, LPCTSTR lpBNetIP)
{
	for (int i = 0; i < arr.GetSize(); i++)
	{
		if (arr[i].CompareNoCase(lpBNetIP) == 0)
		{
			return (i - 2) / 3 + 1;
		}
	}

	return -1;
}

BOOL CRealmInfo::_GetOfficialBNetIP(LPTSTR lpBNetIP, LPCTSTR lpPreferred)
{
	CString sPreferred(lpPreferred);
	sPreferred.MakeLower();

	if (sPreferred == _T("asia") || sPreferred == _T("asia2") || sPreferred == _T("asia3"))
	{
		_tcscpy(lpBNetIP, _T("asia.battle.net"));
	}
	else if (sPreferred == _T("useast"))
	{
		_tcscpy(lpBNetIP, _T("useast.battle.net"));
	}
	else if (sPreferred == _T("uswest"))
	{
		_tcscpy(lpBNetIP, _T("uswest.battle.net"));
	}
	else if (sPreferred == _T("europe"))
	{
		_tcscpy(lpBNetIP, _T("europe.battle.net"));
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CRealmInfo::SetDefaultAccount(LPCTSTR lpAccount)
{
	HKEY hKey = NULL;
	if (ERROR_SUCCESS != ::RegOpenKeyEx(RI_REG_ROOT, RI_REG_KEY, 0, KEY_SET_VALUE, &hKey))
		return FALSE;	

	BOOL bRes = (ERROR_SUCCESS == ::RegSetValueEx(hKey, _T("Last BNet"), 0, REG_SZ, (LPBYTE)lpAccount, _tcslen(lpAccount) * sizeof(TCHAR)));

	::RegCloseKey(hKey);
	return bRes;
}

BOOL CRealmInfo::ForceDefaultRealm(LPCTSTR lpBnetIP, LPCTSTR lpPreferred)
{
	if (lpBnetIP == NULL || _tcslen(lpBnetIP) == 0)
		return FALSE;

	HKEY hKey = NULL;
	if (ERROR_SUCCESS != ::RegOpenKeyEx(RI_REG_ROOT, RI_REG_KEY, 0, KEY_SET_VALUE, &hKey))
		return FALSE;

	BOOL bRes = (ERROR_SUCCESS == ::RegSetValueEx(hKey, RI_REG_VAL_BNETIP, 0, REG_SZ, (LPBYTE)lpBnetIP, _tcslen(lpBnetIP) * sizeof(TCHAR)));

	if (lpPreferred != NULL && _tcslen(lpPreferred) > 0)
		bRes &= (ERROR_SUCCESS == ::RegSetValueEx(hKey, RI_REG_VAL_PREFERRED, 0, REG_SZ, (LPBYTE)lpPreferred, _tcslen(lpPreferred) * sizeof(TCHAR)));

	::RegCloseKey(hKey);

	return bRes;
}

BOOL CRealmInfo::GetDefaultAccount(LPTSTR lpBuffer)
{
	HKEY hKey = NULL;
	if (ERROR_SUCCESS != ::RegOpenKeyEx(RI_REG_ROOT, RI_REG_KEY, 0, KEY_QUERY_VALUE, &hKey))
		return FALSE;	

	DWORD dwType = REG_SZ;
	DWORD nLen = 17 * sizeof(TCHAR);
	BOOL bRes = (ERROR_SUCCESS == ::RegQueryValueEx(hKey, _T("Last BNet"), NULL, &dwType, (LPBYTE)lpBuffer, &nLen));

	::RegCloseKey(hKey);
	return bRes;
}

BOOL CRealmInfo::IsDefaultAccountSame(LPCTSTR lpAccount)
{
	TCHAR sz[64] = _T("");
	GetDefaultAccount(sz);
	return _tcsicmp(sz, lpAccount) == 0;
}




