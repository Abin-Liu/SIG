// Logger.cpp: implementation of the CLogger class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Logger.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogger::CLogger()
{
	m_dwCacheSize = 1024; // default cache size
	m_bIsWriting = FALSE;
}

CLogger::~CLogger()
{
	Flush();
}

BOOL CLogger::SetFilePath(LPCTSTR lpPath)
{
	if (m_sFile.CompareNoCase(lpPath) == 0)
		return TRUE;

	Flush();
	m_sFile = lpPath;
	return Clean(TRUE);
}

CString CLogger::GetFilePath() const
{
	return m_sFile;
}

BOOL CLogger::Flush()
{
	if (m_sFile.IsEmpty())
		return FALSE;

	CStdioFile file;
	if (!file.Open(m_sFile, CFile::modeWrite | CFile::shareDenyWrite))
		return FALSE;

	m_bIsWriting = TRUE;
	file.SeekToEnd();
	file.WriteString(m_sContents);
	file.Flush();
	file.Close();

	m_sContents = m_sTemp;
	m_sTemp.Empty();
	m_bIsWriting = FALSE;
	return TRUE;
}

void CLogger::Log(LPCTSTR lpText)
{
	if (m_sFile.IsEmpty() || lpText == NULL || lpText[0] == _T('\0'))
		return;

	COleDateTime now = COleDateTime::GetCurrentTime();
	CString sLine;
	sLine.Format(_T("%d-%02d %02d:%02d:%02d %s\n"), now.GetMonth(), now.GetDay(), now.GetHour(), now.GetMinute(), now.GetSecond(), lpText);
	m_sContents += sLine;

	if (DWORD(m_sContents.GetLength()) > m_dwCacheSize)
		Flush();
}

BOOL CLogger::Clean(BOOL bTruncateFile)
{
	m_sContents.Empty();
	m_sTemp.Empty();

	if (bTruncateFile && !m_sFile.IsEmpty())
	{
		CFile file;
		if (!file.Open(m_sFile, CFile::modeWrite | CFile::modeCreate))
			return FALSE;

		file.Close();
	}

	return TRUE;
}

void CLogger::SetCacheSize(DWORD dwCacheSize)
{
	m_dwCacheSize = dwCacheSize;
}

void CLogger::AddLineBreak()
{
	if (m_bIsWriting)
		m_sTemp += _T("=========================================================\n");
	else
		m_sContents += _T("=========================================================\n");
}
