// Logger.h: interface for the CLogger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGGER_H__4178EA00_0065_418E_A8D8_410112403F75__INCLUDED_)
#define AFX_LOGGER_H__4178EA00_0065_418E_A8D8_410112403F75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLogger  
{
public:
	void AddLineBreak();
	void SetCacheSize(DWORD dwCacheSize);
	BOOL Clean(BOOL bTruncateFile = TRUE);
	void Log(LPCTSTR lpText);
	BOOL Flush();
	CString GetFilePath() const;
	BOOL SetFilePath(LPCTSTR lpPath);
	CLogger();
	virtual ~CLogger();

private:
	CString m_sFile;
	DWORD m_dwCacheSize;
	CString m_sContents;
	BOOL m_bIsWriting;
	CString m_sTemp;
};

#endif // !defined(AFX_LOGGER_H__4178EA00_0065_418E_A8D8_410112403F75__INCLUDED_)
