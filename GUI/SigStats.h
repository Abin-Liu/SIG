// SigStats.h: interface for the CSigStats class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIGSTATS_H__089060ED_7624_48EE_B6B6_1F608C038596__INCLUDED_)
#define AFX_SIGSTATS_H__089060ED_7624_48EE_B6B6_1F608C038596__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSigStats  
{
public:
	DWORD GetCurrentGameTime() const;
	void ContinueStats();
	double GetInGameRate() const;
	double GetCreateRate() const;
	void RecordGameLeave();
	DWORD GetAvgInGameTime() const;
	void RecordNewGame();
	void UpdateInGameTime(DWORD dwAdd);
	void LoadFile();
	void SaveFile() const;
	void Initialize(BOOL bDelFile);
	void UpdateStartTime(BOOL bStopped);
	CSigStats(LPCTSTR lpFile);
	virtual ~CSigStats();

	COleDateTime m_startTime;
	COleDateTime m_stopTime;
	DWORD m_dwTotalStartTime;	
	DWORD m_dwTotalInGameTime;
	DWORD m_dwMinInGameTime;
	DWORD m_dwMaxInGameTime;
	//DWORD m_dwTotalConnect;
	//DWORD m_dwSuccessConnect;
	DWORD m_dwTotalLogin;
	DWORD m_dwSuccessLogin;
	DWORD m_dwTotalCreate;
	DWORD m_dwSuccessCreate;
	DWORD m_dwTotalCrash;
	DWORD m_dwCurGameTime;

private:	
	DWORD m_dwPrevGameTime;
	CString m_sFile;
};

#endif // !defined(AFX_SIGSTATS_H__089060ED_7624_48EE_B6B6_1F608C038596__INCLUDED_)
