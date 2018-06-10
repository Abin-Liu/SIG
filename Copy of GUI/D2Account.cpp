// D2Account.cpp: implementation of the CD2Account class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "D2Account.h"
#include "Encrypt.h"
#include "SIG.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CD2Account::CD2Account()
{
	m_nCharCount = 8;
	m_nActiveChar = 0;
	m_bEnabled = TRUE;
}

CD2Account::~CD2Account()
{

}

CD2Account::CD2Account(LPCTSTR lpName, LPCTSTR lpPassword, LPCTSTR lpBNetIP, LPCTSTR lpSubRealm, int nCharCount, int nActiveChar, BOOL bEnabled)
{
	m_sName = lpName;
	m_sPassword = lpPassword;
	m_sBNetIP = lpBNetIP;
	m_sSubRealm = lpSubRealm;
	m_nCharCount = nCharCount;
	m_nActiveChar = nActiveChar;
	m_bEnabled = bEnabled;
}

CD2Account::CD2Account(const CD2Account &rhs)
{
	m_sName = rhs.m_sName;
	m_sPassword = rhs.m_sPassword;
	m_sBNetIP = rhs.m_sBNetIP;
	m_sSubRealm = rhs.m_sSubRealm;
	m_nCharCount = rhs.m_nCharCount;
	m_nActiveChar = rhs.m_nActiveChar;
	m_bEnabled = rhs.m_bEnabled;
}

BOOL CD2Account::operator== (const CD2Account &rhs) const
{
	return (m_sName.CompareNoCase(rhs.m_sName) == 0
		&& m_sBNetIP.CompareNoCase(rhs.m_sBNetIP) == 0
		&& m_sSubRealm.CompareNoCase(rhs.m_sSubRealm) == 0);
}

const CD2Account& CD2Account::operator= (const CD2Account &rhs)
{
	m_sName = rhs.m_sName;
	m_sPassword = rhs.m_sPassword;
	m_sBNetIP = rhs.m_sBNetIP;
	m_sSubRealm = rhs.m_sSubRealm;
	m_nCharCount = rhs.m_nCharCount;
	m_nActiveChar = rhs.m_nActiveChar;
	m_bEnabled = rhs.m_bEnabled;
	return *this;
}

void CD2Account::SetName(LPCTSTR lpName)
{
	m_sName = lpName;
}

void CD2Account::SetPassword(LPCTSTR lpPassword)
{
	m_sPassword = lpPassword;
}

void CD2Account::SetBNetIP(LPCTSTR lpBNetIP)
{
	m_sBNetIP = lpBNetIP;
}

void CD2Account::SetSubRealm(LPCTSTR lpSubRealm)
{
	m_sSubRealm = lpSubRealm;
}

void CD2Account::SetCharCount(int nCharCount)
{
	if (nCharCount > 0 && nCharCount < 9)
		m_nCharCount = nCharCount;
}

void CD2Account::MoveToNextChar()
{
	if (HasMoreChar())
		m_nActiveChar++;
	else
		m_nActiveChar = 65535;
}

BOOL CD2Account::HasMoreChar() const
{
	return m_nActiveChar < m_nCharCount;
}

CString CD2Account::GetName() const
{
	return m_sName;
}

CString CD2Account::GetPassword() const
{
	return m_sPassword;
}

CString CD2Account::GetBNetIP() const
{
	return m_sBNetIP;
}

CString CD2Account::GetSubRealm() const
{
	return m_sSubRealm;
}

int CD2Account::GetCharCount() const
{
	return m_nCharCount;
}

int CD2Account::GetActiveChar() const
{
	return m_bEnabled ? m_nActiveChar : -1;
}

BOOL CD2Account::IsOnSameRealm(const CD2Account &rhs) const
{
	return m_sBNetIP.CompareNoCase(rhs.m_sBNetIP) == 0
		&& m_sSubRealm.CompareNoCase(rhs.m_sSubRealm) == 0;
}

BOOL CD2Account::Serialize(CArchive &ar)
{
	CString str;
	try
	{
		if (ar.IsLoading())
		{			
			ar >> m_sName >> str >> m_sBNetIP >> m_sSubRealm >> m_nCharCount >> m_bEnabled;
			m_sPassword = DecryptString(str, ((CSIGApp*)AfxGetApp())->m_sUID);
		}
		else
		{
			str = EncryptString(m_sPassword, ((CSIGApp*)AfxGetApp())->m_sUID);
			ar << m_sName << str << m_sBNetIP << m_sSubRealm << m_nCharCount << m_bEnabled;
		}
	}
	catch (CArchiveException* perr)
	{
		perr->Delete();
		m_sName.Empty();
		m_sPassword.Empty();
		m_sBNetIP.Empty();
		m_sSubRealm.Empty();
		m_nCharCount = 0;
		m_bEnabled = TRUE;
		return FALSE;
	}

	return TRUE;
}

CString CD2Account::EncryptString(LPCTSTR lpsz, LPCTSTR lpszPassword)
{
	CString str;
	const int LEN = GetEncryptLength(lpsz);
	if (LEN > 0)
	{
		TCHAR* psz = new TCHAR[LEN + 1];
		if (ERR_OK == Encrypt(lpsz, psz, LEN, lpszPassword))
			str = psz;
		delete [] psz;
		psz = NULL;
	}
	return str;
}

CString CD2Account::DecryptString(LPCTSTR lpszStr, LPCTSTR lpszPassword)
{
	CString str;
	WORD wLen = 0;
	if (ERR_OK == GetDecryptLength(lpszStr, lpszPassword, wLen))
	{
		TCHAR* psz = new TCHAR[wLen + 1];
		if (ERR_OK == Decrypt(lpszStr, psz, wLen, lpszPassword))
			str = psz;
		delete [] psz;
		psz = NULL;
	}
	return str;
}

void CD2Account::Enable(BOOL bEnable)
{
	m_bEnabled = bEnable;
}

BOOL CD2Account::IsEnabled() const
{
	return m_bEnabled;
}

void CD2Account::SetActiveChar(int nChar)
{
	if (nChar < 0)
		nChar = 0;

	if (nChar > 8)
		nChar = 8;

	m_nActiveChar = nChar;
}
