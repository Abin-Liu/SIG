// IniFile.cpp: implementation of the CIniFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h" // include if used in MFC applications
#include <stdio.h>
#include <stdlib.h>
#include <fstream.h>
#include "IniFile.h"
#include <string.h>

#define DEF_STR_LEN	1024
#define DEF_MAX_LEN	4096

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIniFile::CIniFile()
{}

CIniFile::CIniFile(LPCTSTR lpPathName)
{
	SetPathName(lpPathName);
}

CIniFile::~CIniFile()
{}

BOOL CIniFile::SetPathName(LPCTSTR lpPathName)
{
	if (lpPathName != NULL)
	{
		m_sPathName = lpPathName;
		return TRUE;
	}

	return FALSE;
}

const CString& CIniFile::GetPathName() const
{
	return m_sPathName;
}

CString CIniFile::GetProfileString(LPCTSTR lpSection, LPCTSTR lpEntry, LPCTSTR lpDefault) const
{
	TCHAR szBuffer[DEF_STR_LEN + sizeof(TCHAR)] = _T("");
	::GetPrivateProfileString(lpSection, lpEntry, lpDefault, szBuffer, DEF_STR_LEN, m_sPathName);
	return CString(szBuffer);
}

int CIniFile::GetProfileInt(LPCTSTR lpSection, LPCTSTR lpEntry, int nDefault) const
{
	CString sData = GetProfileString(lpSection, lpEntry, _T("")); // get the string form value

	if (sData.IsEmpty())
		return nDefault;
	else
		return _ttoi(sData);
}

BOOL CIniFile::GetProfileBool(LPCTSTR lpSection, LPCTSTR lpEntry, BOOL bDefault) const
{
	int nDefault = bDefault ? 1 : 0;
	return (GetProfileInt(lpSection, lpEntry, nDefault) != 0);
}

double CIniFile::GetProfileDbl(LPCTSTR lpSection, LPCTSTR lpEntry, double fDefault) const
{
	CString sData = GetProfileString(lpSection, lpEntry, _T("")); // get the string form value

	if (sData.IsEmpty())
		return fDefault;
	else
		return _tcstod(sData, NULL);
}

BOOL CIniFile::WriteProfileString(LPCTSTR lpSection, LPCTSTR lpEntry, LPCTSTR lpValue)
{
	return ::WritePrivateProfileString(lpSection, lpEntry, lpValue, m_sPathName);
}

BOOL CIniFile::WriteProfileInt(LPCTSTR lpSection, LPCTSTR lpEntry, int nValue)
{
	CString sVal;
	sVal.Format(_T("%d"), nValue);
	return WriteProfileString(lpSection, lpEntry, sVal);
}

BOOL CIniFile::WriteProfileBool(LPCTSTR lpSection, LPCTSTR lpEntry, BOOL bValue)
{
	return WriteProfileInt(lpSection, lpEntry, bValue ? 1 : 0);
}

BOOL CIniFile::WriteProfileDbl(LPCTSTR lpSection, LPCTSTR lpEntry, double fValue)
{
	CString sVal;
	sVal.Format(_T("%f"), fValue);
	return WriteProfileString(lpSection, lpEntry, sVal);
}

DWORD CIniFile::GetProfileSection(LPCTSTR lpSection, CStringArray& aList) const
{
	DWORD seg = DEF_STR_LEN + sizeof(TCHAR);
	TCHAR* pszBuffer = new TCHAR[seg];
	DWORD dwLen = ::GetPrivateProfileSection(lpSection, pszBuffer, seg, m_sPathName);

	while (dwLen > seg - 3 * sizeof(TCHAR))
	{
		seg += DEF_STR_LEN;
		delete [] pszBuffer;
		pszBuffer = new TCHAR[seg];
		dwLen = ::GetPrivateProfileSection(lpSection, pszBuffer, seg, m_sPathName);
	}
	
	// By default, delimiter is '\0' (WIN32 API)
	// Replace them by '\n'
	for (DWORD i = 0; i < dwLen; i++)
	{
		if (pszBuffer[i] == _T('\0'))
			pszBuffer[i] = _T('\n');
	}

	CString sAll(pszBuffer);
	delete [] pszBuffer;
	pszBuffer = NULL;

	aList.RemoveAll();

	int nCount = 0;

	CString str;
	int nPosPrev = 0;
	int nPosNew = 0;
	BOOL bContinue = TRUE;

	int n = 0;

	while (bContinue)
	{
		nPosNew = sAll.Find(_T('\n'), nPosPrev);
		if (nPosNew != -1)
		{
			str = sAll.Mid(nPosPrev, nPosNew - nPosPrev);
		}
		else
		{
			str = sAll.Mid(nPosPrev);
			bContinue = FALSE;
		}

		str.TrimLeft();
		str.TrimRight();

		if (!str.IsEmpty())
		{
			aList.Add(str);
		}

		nPosPrev = nPosNew + 1;

		n++;
	}

	return aList.GetSize();
}

DWORD CIniFile::GetProfileSectionNames(CStringArray& aList) const
{
	DWORD nLen = DEF_STR_LEN + 1;
	TCHAR* pszBuffer = new TCHAR[nLen];
	DWORD dwLen = ::GetPrivateProfileSectionNames(pszBuffer, nLen, m_sPathName);

	while (dwLen >= nLen - 5)
	{
		nLen += DEF_STR_LEN;
		delete [] pszBuffer;
		pszBuffer = new TCHAR[nLen];
		dwLen = ::GetPrivateProfileSectionNames(pszBuffer, nLen, m_sPathName);
	}
	
	// By default, delimiter is '\0' (WIN32 API)
	// Replace them by '\n'
	for (DWORD i = 0; i < dwLen; i++)
	{
		if (pszBuffer[i] == _T('\0'))
			pszBuffer[i] = _T('\n');
	}

	CString sAll(pszBuffer);
	delete [] pszBuffer;
	pszBuffer = NULL;

	aList.RemoveAll();

	int nCount = 0;

	CString str;
	int nPosPrev = 0;
	int nPosNew = 0;
	BOOL bContinue = TRUE;

	int n = 0;

	while (bContinue)
	{
		nPosNew = sAll.Find(_T('\n'), nPosPrev);
		if (nPosNew != -1)
		{
			str = sAll.Mid(nPosPrev, nPosNew - nPosPrev);
		}
		else
		{
			str = sAll.Mid(nPosPrev);
			bContinue = FALSE;
		}

		str.TrimLeft();
		str.TrimRight();

		if (!str.IsEmpty())
		{
			aList.Add(str);
		}

		nPosPrev = nPosNew + 1;

		n++;
	}

	return aList.GetSize();
}

BOOL CIniFile::IncreaseProfileInt(LPCTSTR lpSection, LPCTSTR lpEntry, int nIncrease)
{
	int nVal = GetProfileInt(lpSection, lpEntry, 0);
	nVal += nIncrease;
	return WriteProfileInt(lpSection, lpEntry, nVal);
}

BOOL CIniFile::AppendProfileString(LPCTSTR lpSection, LPCTSTR lpEntry, LPCTSTR lpString)
{
	CString sVal = GetProfileString(lpSection, lpEntry, _T(""));
	sVal += lpString;
	return WriteProfileString(lpSection, lpEntry, sVal);
}

BOOL CIniFile::RemoveProfileSection(LPCTSTR lpSection) const
{
	// basically, read line by line, copy the lines from orginal
	// ini file to a new file, with specified section contents ignored,
	// then replace the original ini file by the new file.

	// open original ini file, and create a new file
	CStdioFile origIni;
	CStdioFile tempFile;

	// temp file name
	TCHAR sTempName[_MAX_PATH] = _T("");
	_stprintf(sTempName, _T("%s.tmp"), m_sPathName);

	if (!origIni.Open(m_sPathName, CFile::modeRead) || !tempFile.Open(sTempName, CFile::modeCreate))
		return FALSE;

	CString sTemp;
	BOOL bModified = FALSE;

	// read line by line
	while (origIni.ReadString(sTemp))
	{
		if (!FindSection(lpSection, sTemp))
		{
			tempFile.WriteString(sTemp + _T("\n"));
			continue;
		}

		// section found
		// find and ignore the specified entry within
		bModified = TRUE;
		CString sDummy;
		while (origIni.ReadString(sTemp))
		{
			// ignore all lines until next section reached
			if (IsSection(sTemp, sDummy))
			{
				tempFile.WriteString(sTemp + _T("\n"));
				break;
			}
		}
	}

	origIni.Close();
	tempFile.Close();

	if (bModified)
	{
		::CopyFile(sTempName, m_sPathName, FALSE);
	}
	::DeleteFile(sTempName);
	return bModified;
}

BOOL CIniFile::RemoveProfileEntry(LPCTSTR lpSection, LPCTSTR lpEntry) const
{
	// basically, read line by line, copy the lines from orginal
	// ini file to a new file, with specified entry lines ignored,
	// then replace the original ini file by the new file.

	// open original ini file, and create a new file
	CStdioFile origIni;
	CStdioFile tempFile;

	// temp file name
	TCHAR sTempName[_MAX_PATH] = _T("");
	_stprintf(sTempName, _T("%s.tmp"), m_sPathName);

	if (!origIni.Open(m_sPathName, CFile::modeRead) || !tempFile.Open(sTempName, CFile::modeCreate))
		return FALSE;

	// temp string line
	CString sTemp;
	CString sDummy;

	// string to hold section names
	TCHAR szSection[256] = _T("");

	BOOL bModified = FALSE;

	// read line by line
	while (origIni.ReadString(sTemp))
	{
		// is it the section?
		if (!FindSection(lpSection, sTemp))
		{
			tempFile.WriteString(sTemp + _T("\n"));
			continue;
		}

		// section found

		// write back section name
		tempFile.WriteString(sTemp + _T("\n"));

		// now try to find the entry
		while (origIni.ReadString(sTemp))
		{

			if (IsSection(sTemp, sDummy)) // reached another section, so entry does not exist
			{
				origIni.Close();
				tempFile.Close();
				::DeleteFile(sTempName);
				return FALSE;
			}
			else if(FindEntry(lpEntry, sTemp))
			{
				bModified = TRUE;
				break;
			}
			else
			{
				tempFile.WriteString(sTemp + _T("\n"));
			}
		}
	}

	origIni.Close();
	tempFile.Close();

	if (bModified)
	{
		::CopyFile(sTempName, m_sPathName, FALSE);
	}
	::DeleteFile(sTempName);
	return bModified;
}

BOOL CIniFile::IsSection(LPCTSTR lpString, CString& sSection)
{
	CString str(lpString);

	// remove white spaces
	str.TrimLeft();
	str.TrimRight();

	// not match the section format
	if (str.GetLength() < 2 || str[0] != _T('[') || str[str.GetLength() - 1] !=_T(']'))
		return FALSE;

	// now the remain stuff is "[*]"
	
	// strip the left '[' and right']'
	str.Delete(str.GetLength() - 1);
	str.Delete(0);

	// remove white spaces
	str.TrimLeft();
	str.TrimRight();

	//Now it's a section for sure, and we need to extract the
	// section name and store it in lpSection
	sSection = str;
	return TRUE;
}

BOOL CIniFile::FindSection(LPCTSTR lpSection, LPCTSTR lpString)
{
	CString s;
	// is it a section?
	if (!IsSection(lpString, s))
		return FALSE;

	// compare the section with lpSection
	return (s.CompareNoCase(lpSection) == 0);
}

BOOL CIniFile::IsEntry(LPCTSTR lpString, CString& sParsedResult)
{
	CString str(lpString);
	CString sDummy;

	// remove white spaces
	str.TrimLeft();
	str.TrimRight();

	// must be at least 2 characters after triming
	if (str.GetLength() < 2)
		return FALSE;

	// must not be in a section's format ([*])
	if (IsSection(lpString, sDummy))
		return FALSE;

	// an entry string must contain a '=', and the '=' must not be the first character
	int nEq = str.Find(_T('='));
	if (nEq < 1)
		return FALSE;

	sParsedResult = str.Left(nEq);
	return TRUE;
}

BOOL CIniFile::FindEntry(LPCTSTR lpEntry, LPCTSTR lpString)
{
	CString sEntry;
	if (!IsEntry(lpString, sEntry))
		return FALSE;

	// compare the entry name with lpEntry
	return (sEntry.CompareNoCase(lpEntry) == 0);
}
