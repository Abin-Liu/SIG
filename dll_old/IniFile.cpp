// IniFile.cpp: implementation of the CIniFile class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h" // include if used in MFC applications
#include <stdio.h>
#include <stdlib.h>
#include "IniFile.h"
#include <fstream.h>
#include <string.h>

#define DEF_STR_LEN	256 // temporary string length
#define DEF_MAX_LEN	4096 // max length a key value string can have

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIniFile::CIniFile(): m_pszPathName(NULL)
{}

CIniFile::CIniFile(LPCTSTR lpPathName): m_pszPathName(NULL)
{
	SetPathName(lpPathName);
}

CIniFile::~CIniFile()
{
	if (m_pszPathName != NULL)
		delete [] m_pszPathName;
}

BOOL CIniFile::SetPathName(LPCTSTR lpPathName)
{
	if (m_pszPathName != NULL)
	{
		delete [] m_pszPathName;
		m_pszPathName = NULL;
	}

	if (lpPathName != NULL)
	{
		m_pszPathName = new TCHAR[_tcslen(lpPathName) + sizeof(TCHAR)];
		_tcscpy(m_pszPathName, lpPathName);
		return TRUE;
	}

	return FALSE;
}

int CIniFile::GetPathName(LPSTR lpBuffer, DWORD nLength) const
{
	if (m_pszPathName == NULL)
		return 0; // no path name available

	DWORD n = _tcslen(m_pszPathName);

	_tcsncpy(lpBuffer, m_pszPathName, nLength);
	return n; // normal
}

DWORD CIniFile::GetProfileString(LPCTSTR lpSection, LPCTSTR lpEntry, LPCTSTR lpDefault, LPSTR lpBuffer, DWORD nLength) const
{
	return ::GetPrivateProfileString(lpSection, lpEntry, lpDefault, lpBuffer, nLength, m_pszPathName);
}

int CIniFile::GetProfileInt(LPCTSTR lpSection, LPCTSTR lpEntry, int nDefault) const
{
	TCHAR szDefault[DEF_STR_LEN + sizeof(TCHAR)] = _T("");
	_stprintf(szDefault, _T("%d"), nDefault); // convert default value

	TCHAR szData[DEF_STR_LEN + sizeof(TCHAR)] = _T("");
	GetProfileString(lpSection, lpEntry, szDefault, szData, DEF_STR_LEN); // get the string form value

	return _ttoi(szData);
}

BOOL CIniFile::GetProfileBool(LPCTSTR lpSection, LPCTSTR lpEntry, BOOL bDefault) const
{
	int nRes = GetProfileInt(lpSection, lpEntry, bDefault ? 1 : 0);
	return (nRes != 0);
}

double CIniFile::GetProfileDbl(LPCTSTR lpSection, LPCTSTR lpEntry, double fDefault) const
{
	TCHAR szDefault[DEF_STR_LEN + sizeof(TCHAR)] = _T("");
	_stprintf(szDefault, _T("%f"), fDefault); // convert default value

	TCHAR szData[DEF_STR_LEN + sizeof(TCHAR)] = _T("");
	GetProfileString(lpSection, lpEntry, szDefault, szData, DEF_STR_LEN); // get the string form value

	return _tcstod(szData, NULL);
}

BOOL CIniFile::WriteProfileString(LPCTSTR lpSection, LPCTSTR lpEntry, LPCTSTR lpValue) const
{
	return ::WritePrivateProfileString(lpSection, lpEntry, lpValue, m_pszPathName);
}

BOOL CIniFile::WriteProfileInt(LPCTSTR lpSection, LPCTSTR lpEntry, int nValue) const
{
	TCHAR szValue[DEF_STR_LEN + sizeof(TCHAR)] = _T("");
	_stprintf(szValue, _T("%d"), nValue);
	return WriteProfileString(lpSection, lpEntry, szValue);
}

BOOL CIniFile::WriteProfileDbl(LPCTSTR lpSection, LPCTSTR lpEntry, double fValue) const
{
	TCHAR szValue[DEF_STR_LEN + sizeof(TCHAR)] = _T("");
	_stprintf(szValue, _T("%f"), fValue);
	return WriteProfileString(lpSection, lpEntry, szValue);
}

BOOL CIniFile::WriteProfileBool(LPCTSTR lpSection, LPCTSTR lpEntry, BOOL bValue) const
{
	return WriteProfileInt(lpSection, lpEntry, bValue ? 1 : 0);
}

DWORD CIniFile::GetProfileSection(LPCTSTR lpSection, LPTSTR lpBuffer, DWORD nLength, TCHAR cDelimiter /*='\0'*/) const
{
	DWORD dwLen = ::GetPrivateProfileSection(lpSection, lpBuffer, nLength, m_pszPathName);
	
	// By default, delimiter is '\0' (WIN32 API)
	// Need to replace with new delimiters
	if (cDelimiter != _T('\0'))
	{
		for (DWORD i = 0; i < dwLen; i++)
		{
			if (lpBuffer[i] == _T('\0'))
				lpBuffer[i] = cDelimiter;
		}
	}

	return dwLen;
}

DWORD CIniFile::GetProfileSectionNames(LPTSTR lpBuffer, DWORD nLength, TCHAR cDelimiter /*='\0'*/) const
{
	DWORD dwLen = ::GetPrivateProfileSectionNames(lpBuffer, nLength, m_pszPathName);
	
	// By default, delimiter is '\0' (WIN32 API)
	// Need to replace with new delimiters
	if (cDelimiter != _T('\0'))
	{
		for (DWORD i = 0; i < dwLen; i++)
		{
			if (lpBuffer[i] == _T('\0'))
				lpBuffer[i] = cDelimiter;
		}
	}

	return dwLen;
}

BOOL CIniFile::IncreaseProfileInt(LPCTSTR lpSection, LPCTSTR lpEntry, int nIncrease) const
{
	int nVal = GetProfileInt(lpSection, lpEntry, 0);
	nVal += nIncrease;
	return WriteProfileInt(lpSection, lpEntry, nVal);
}

BOOL CIniFile::AppendProfileString(LPCTSTR lpSection, LPCTSTR lpEntry, LPCTSTR lpString) const
{
	TCHAR szValue[DEF_MAX_LEN] = _T("");
	DWORD dwCurLen = GetProfileString(lpSection, lpEntry, _T(""), szValue, DEF_MAX_LEN);
	if (dwCurLen + _tcslen(lpString) > DEF_MAX_LEN) // will overflow
		return FALSE;

	_stprintf(szValue, _T("%s%s"), szValue, lpString);
	return WriteProfileString(lpSection, lpEntry, szValue);
}

BOOL CIniFile::RemoveProfileSection(LPCTSTR lpSection) const
{
	// basically, read line by line, copy the lines from orginal
	// ini file to a new file, with specified section contents ignored,
	// then replace the original ini file by the new file.

	// open original ini file, and create a new file
	ifstream origIni;
	ofstream tempFile;

	// temp file name
	TCHAR sTempName[_MAX_PATH] = _T("");
	_stprintf(sTempName, _T("%s.tmp"), m_pszPathName);

	origIni.open(m_pszPathName);
	tempFile.open(sTempName);

	// open fail
	if (origIni == NULL || tempFile == NULL)
		return FALSE;

	if (origIni.eof())
		return FALSE;

	// temp string line
	TCHAR* pszTemp = new TCHAR[DEF_MAX_LEN + sizeof(TCHAR)];

	BOOL bModified = FALSE;

	// read line by line
	while (!origIni.eof())
	{
		pszTemp[0] = _T('\0');
		origIni.getline(pszTemp, DEF_MAX_LEN);

		if (!FindSection(lpSection, pszTemp))
		{
			tempFile << pszTemp << _T('\n');
			continue;
		}

		// section found
		// find and ignore the specified entry within
		bModified = TRUE;
		while (!origIni.eof())
		{
			pszTemp[0] = _T('\0');
			origIni.getline(pszTemp, DEF_MAX_LEN);

			// ignore all lines until next section reached
			if (IsSection(pszTemp, NULL))
			{
				tempFile << _T('\n') << pszTemp << _T('\n');
				break;
			}
		}
	}

	delete [] pszTemp;
	origIni.close();
	tempFile.close();

	if (bModified)
	{
		::CopyFile(sTempName, m_pszPathName, FALSE);
	}
	::DeleteFile(sTempName);
	return bModified;
}

BOOL CIniFile::IsSection(LPCTSTR lpString, LPTSTR lpSection)
{
	if (lpString == NULL)
		return FALSE;

	int nStart = 0;
	int nEnd = _tcslen(lpString) - 1;

	while (lpString[nStart] == _T(' '))
		nStart++;

	while (lpString[nEnd] == _T(' ') && nEnd >= 0)
		nEnd--;

	if (nEnd - nStart < 2
		|| lpString[nStart] != _T('[')
		|| lpString[nEnd] != _T(']'))
		return FALSE;

	// now the remain stuff is "[*]"
	
	// strip the left '[' and right']'
	nStart++;
	nEnd--;

	// remove white spaces
	while (lpString[nStart] == _T(' ') && nStart <= nEnd)
		nStart++;

	while (lpString[nEnd] == _T(' ') && nEnd >= nStart)
		nEnd--;

	if (nEnd < nStart)
		return FALSE; // there's nothing inside the square brackets

	//Now it's a section for sure, and we need to extract the
	// section name and store it in lpSection
	if (lpSection != NULL)
	{
		for (int i = nStart, j = 0; i <= nEnd; i++, j++)
			lpSection[j] = lpString[i];
		lpSection[j] = _T('\0');
	}

	return TRUE;
}

BOOL CIniFile::RemoveProfileEntry(LPCTSTR lpSection, LPCTSTR lpEntry) const
{
	// basically, read line by line, copy the lines from orginal
	// ini file to a new file, with specified entry lines ignored,
	// then replace the original ini file by the new file.

	// open original ini file, and create a new file
	ifstream origIni;
	ofstream tempFile;

	// temp file name
	TCHAR sTempName[_MAX_PATH] = _T("");
	_stprintf(sTempName, _T("%s.tmp"), m_pszPathName);

	origIni.open(m_pszPathName);
	tempFile.open(sTempName);

	// open fail
	if (origIni == NULL || tempFile == NULL)
		return FALSE;

	if (origIni.eof())
		return FALSE;

	// temp string line
	TCHAR* pszTemp = new TCHAR[DEF_MAX_LEN + sizeof(TCHAR)];

	// string to hold section names
	TCHAR szSection[256] = _T("");

	BOOL bModified = FALSE;

	// read line by line
	while (!origIni.eof())
	{
		pszTemp[0] = _T('\0');
		origIni.getline(pszTemp, DEF_MAX_LEN);

		// is it the section?
		if (!FindSection(lpSection, pszTemp))
		{
			tempFile << pszTemp << _T('\n');
			continue;
		}

		tempFile << pszTemp << _T('\n');

		// section found
		// now find the entry
		while (!origIni.eof())
		{
			pszTemp[0] = _T('\0');
			origIni.getline(pszTemp, DEF_MAX_LEN);

			// ignore all lines until next section reached
			if (FindEntry(lpEntry, pszTemp))
			{
				bModified = TRUE;
				break;
			}
			else
			{
				tempFile << _T('\n') << pszTemp << _T('\n');
			}
		}
	}

	delete [] pszTemp;
	origIni.close();
	tempFile.close();

	if (bModified)
	{
		::CopyFile(sTempName, m_pszPathName, FALSE);
	}
	::DeleteFile(sTempName);
	return bModified;
}

BOOL CIniFile::FindSection(LPCTSTR lpSection, LPCTSTR lpString)
{
	TCHAR szTemp[256] = _T("");
	// is it a section?
	if (!IsSection(lpString, szTemp))
		return FALSE;

	// compare the section with lpSection
	return (_tcsicmp(lpSection, szTemp) == 0);
}

BOOL CIniFile::IsEntry(LPCTSTR lpString, LPTSTR lpParsedResult)
{
	if (lpString == NULL)
		return FALSE;

	if (IsSection(lpString, NULL))
		return FALSE;

	// an entry string must contain a '='
	TCHAR* p = _tcschr(lpString, _T('='));
	if (p == NULL)
		return FALSE;
	
	int nEnd = p - lpString - 1;

	// remove left side white spaces
	int nStart = 0;
	while (lpString[nStart] == _T(' '))
		nStart++;

	// remove right-side white spaces
	while (lpString[nEnd] == _T(' ') && nEnd >= nStart)
		nEnd--;

	// still contents?
	if (nEnd < nStart)
		return FALSE;

	if (lpParsedResult != NULL)
	{
		for (int i = nStart, j = 0; i <= nEnd; i++, j++)
			lpParsedResult[j] = lpString[i];
		lpParsedResult[j] = _T('\0');
	}

	return TRUE;
}

BOOL CIniFile::FindEntry(LPCTSTR lpEntry, LPCTSTR lpString)
{
	TCHAR szTemp[256] = _T("");
	if (!IsEntry(lpString, szTemp))
		return FALSE;

	// compare the entry name with lpEntry
	return (_tcsicmp(lpEntry, szTemp) == 0);
}
