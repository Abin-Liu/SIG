// D2Account.h: interface for the CD2Account class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_D2ACCOUNT_H__8E2BD48D_8B28_4113_BC08_845F2F32B96A__INCLUDED_)
#define AFX_D2ACCOUNT_H__8E2BD48D_8B28_4113_BC08_845F2F32B96A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CD2Account  
{
public:
	void SetActiveChar(int nChar);
	BOOL IsEnabled() const;
	void Enable(BOOL bEnable = TRUE);
	BOOL Serialize(CArchive& ar);
	BOOL IsOnSameRealm(const CD2Account& rhs) const;
	int GetActiveChar() const;
	int GetCharCount() const;
	CString GetSubRealm() const;
	CString GetBNetIP() const;
	CString GetPassword() const;
	CString GetName() const;
	BOOL HasMoreChar() const;
	void MoveToNextChar();
	void SetCharCount(int nCharCount);
	void SetSubRealm(LPCTSTR lpSubRealm);
	void SetBNetIP(LPCTSTR lpBNetIP);
	void SetPassword(LPCTSTR lpPassword);
	void SetName(LPCTSTR lpName);
	const CD2Account& operator= (const CD2Account& rhs);
	BOOL operator== (const CD2Account& rhs) const;
	CD2Account(const CD2Account& rhs);
	CD2Account(LPCTSTR lpName, LPCTSTR lpPassword, LPCTSTR lpBNetIP, LPCTSTR lpSubRealm, int nCharCount = 8, int nActiveChar = 0, BOOL bEnabled = TRUE);
	CD2Account();
	virtual ~CD2Account();

	static CString DecryptString(LPCTSTR lpszStr, LPCTSTR lpszPassword);
	static CString EncryptString(LPCTSTR lpsz, LPCTSTR lpszPassword);

protected:
	
	CString m_sName;
	CString m_sPassword;
	CString m_sBNetIP;
	CString m_sSubRealm;
	int m_nCharCount;
	int m_nActiveChar;
	BOOL m_bEnabled;
};

#endif // !defined(AFX_D2ACCOUNT_H__8E2BD48D_8B28_4113_BC08_845F2F32B96A__INCLUDED_)
