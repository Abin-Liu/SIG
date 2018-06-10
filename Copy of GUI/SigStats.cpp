// SigStats.cpp: implementation of the CSigStats class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIG.h"
#include "SigStats.h"
#include "ini.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSigStats::CSigStats(LPCTSTR lpFile)
{
	m_sFile = lpFile;
	Initialize(FALSE);
}

CSigStats::~CSigStats()
{

}

void CSigStats::UpdateStartTime(BOOL bStopped)
{
	if (bStopped)
	{
		m_stopTime = COleDateTime::GetCurrentTime();
	}
	else
	{
		m_stopTime.m_status = COleDateTime::invalid;
	}
}

void CSigStats::Initialize(BOOL bDelFile)
{
	if (bDelFile)
		::DeleteFile(m_sFile);

	m_startTime = COleDateTime::GetCurrentTime();
	m_stopTime.m_status = COleDateTime::invalid;
	m_dwTotalStartTime = 0;
	m_dwTotalInGameTime = 0;
	m_dwMinInGameTime = 0;
	m_dwMaxInGameTime = 0;
	//m_dwTotalConnect = 0;
	//m_dwSuccessConnect = 0;
	m_dwTotalLogin = 0;
	m_dwSuccessLogin = 0;
	m_dwTotalCreate = 0;
	m_dwSuccessCreate = 0;
	m_dwTotalCrash = 0;
	m_dwCurGameTime = 0;
	m_dwPrevGameTime = 0;
}

void CSigStats::SaveFile() const
{
	::DeleteFile(m_sFile);
	CIni ini(m_sFile);
	ini.WriteString(_T("Default"), _T("Start Time"), m_startTime.Format());
	if (m_stopTime.m_status == COleDateTime::valid)
		ini.WriteString(_T("Default"), _T("End Time"), m_stopTime.Format());
	ini.WriteInt(_T("Default"), _T("Run Time"), m_dwTotalStartTime);
	ini.WriteInt(_T("Default"), _T("In Game"), m_dwTotalInGameTime);
	ini.WriteInt(_T("Default"), _T("Max In Game"), m_dwMaxInGameTime);
	ini.WriteInt(_T("Default"), _T("Min In Game"), m_dwMinInGameTime);
	//ini.WriteInt(_T("Default"), _T("Connect"), m_dwTotalConnect);
	//ini.WriteInt(_T("Default"), _T("Success Connect"), m_dwSuccessConnect);
	ini.WriteInt(_T("Default"), _T("Login"), m_dwTotalLogin);
	ini.WriteInt(_T("Default"), _T("Success Login"), m_dwSuccessLogin);
	ini.WriteInt(_T("Default"), _T("Create"), m_dwTotalCreate);
	ini.WriteInt(_T("Default"), _T("Success Create"), m_dwSuccessCreate);
	ini.WriteInt(_T("Default"), _T("Crash"), m_dwTotalCrash);
}

void CSigStats::LoadFile()
{
	CIni ini(m_sFile);
	m_startTime.ParseDateTime(ini.GetString(_T("Default"), _T("Start Time"), _T("")));
	m_stopTime.ParseDateTime(ini.GetString(_T("Default"), _T("End Time"), _T("")));
	m_dwTotalStartTime = ini.GetInt(_T("Default"), _T("Run Time"), 0);
	m_dwTotalInGameTime = ini.GetInt(_T("Default"), _T("In Game"), 0);
	m_dwMaxInGameTime = ini.GetInt(_T("Default"), _T("Max In Game"), 0);
	m_dwMinInGameTime = ini.GetInt(_T("Default"), _T("Min In Game"), 0);
	//m_dwTotalConnect = ini.GetInt(_T("Default"), _T("Connect"), 0);
	//m_dwSuccessConnect = ini.GetInt(_T("Default"), _T("Success Connect"), 0);
	m_dwTotalLogin = ini.GetInt(_T("Default"), _T("Login"), 0);
	m_dwSuccessLogin = ini.GetInt(_T("Default"), _T("Success Login"), 0);
	m_dwTotalCreate = ini.GetInt(_T("Default"), _T("Create"), 0);
	m_dwSuccessCreate = ini.GetInt(_T("Default"), _T("Success Create"), 0);
	m_dwTotalCrash = ini.GetInt(_T("Default"), _T("Crash"), 0);
}

void CSigStats::UpdateInGameTime(DWORD dwAdd)
{
	m_dwTotalInGameTime += dwAdd;
	m_dwCurGameTime += dwAdd;
	m_dwMaxInGameTime = max(m_dwMaxInGameTime, m_dwCurGameTime);
	if (m_dwSuccessCreate < 2) // the first game
	{
		m_dwMinInGameTime = m_dwCurGameTime;
	}
	if (m_dwSuccessCreate == 2) // the second game
	{
		if (m_dwPrevGameTime < m_dwCurGameTime)
			m_dwMinInGameTime = m_dwPrevGameTime;
	}
	else
	{		
	}			
}

void CSigStats::RecordNewGame()
{
	m_dwSuccessCreate++;
	m_dwCurGameTime = 0;
}

DWORD CSigStats::GetAvgInGameTime() const
{
	return m_dwSuccessCreate == 0 ? 0 : m_dwTotalInGameTime / m_dwSuccessCreate;
}

void CSigStats::RecordGameLeave()
{
	m_dwPrevGameTime = m_dwCurGameTime;
	m_dwMinInGameTime = min(m_dwMinInGameTime, m_dwCurGameTime);
}

double CSigStats::GetCreateRate() const
{
	return m_dwTotalCreate == 0 ? 0.0 : (double)m_dwSuccessCreate / (double)m_dwTotalCreate;
}

double CSigStats::GetInGameRate() const
{
	return m_dwTotalStartTime == 0 ? 0.0 : (double)m_dwTotalInGameTime / (double)m_dwTotalStartTime;
}

void CSigStats::ContinueStats()
{
	m_stopTime.m_status = COleDateTime::invalid;
	CIni ini(m_sFile);
	ini.WriteString(_T("Default"), _T("End Time"), _T(""));
}

DWORD CSigStats::GetCurrentGameTime() const
{
	return m_dwCurGameTime;
}
