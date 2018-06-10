////////////////////////////////////////////////////////////////////////
//
// RealmInfo.h
//
// Class definition of CRealmInfo class, which is used for manipulating
// Diablo II official/private realm information.
//
// Please be advised that this file and all of its related compiled modules
// are provided "as is", with no expressed or implied warranty. The author
// holds no responsibility to any loss or damage to user's data that are
// caused by using this file or any of its compiled modules. Used them at
// your own risk.
//
// Copyright (c) Abin (abinn32@163.com)
// All rights reserved.
//
////////////////////////////////////////////////////////////////////////

#ifndef	__REALMINFO_H__
#define __REALMINFO_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

enum { RI_ERR_OK = 0, RI_ERR_NOTFOUND, RI_ERR_ALREADYEXIST, RI_ERR_BUILTINREALM, RI_ERR_FILE, RI_ERR_UNKNOWN };
enum { RI_LIST_ALL = 0, RI_LIST_OFFICIALONLY, RI_LIST_PRIVATEONLY };

class CD2Realm; // forward declaration

// class CRealmInfo, manipulates realm info
class AFX_EXT_CLASS CRealmInfo  
{
public:	
	
	// construction
	CRealmInfo();	
	CRealmInfo(const CRealmInfo& rhs);
	virtual ~CRealmInfo();

	// static functions
	static BOOL GetDefaultRealm(LPTSTR szBnetIP, LPTSTR szPreferred);
	static BOOL SetDefaultRealm(LPCTSTR lpBnetIP, LPCTSTR lpPreferred = NULL);	
	static BOOL IsSkipToOpenDisabled(BOOL& bDisabled);
	static BOOL DisableSkipToOpen(BOOL bDisable = TRUE);
	static BOOL IsDefaultAccountSame(LPCTSTR lpAccount);
	static BOOL GetDefaultAccount(LPTSTR lpBuffer);
	static BOOL ForceDefaultRealm(LPCTSTR lpBnetIP, LPCTSTR lpPreferred);
	static BOOL SetDefaultAccount(LPCTSTR lpAccount);
	static BOOL BNetIPCleanup(LPCTSTR lpBNetIP);

	// operator overloadings
	const CRealmInfo& operator=(const CRealmInfo& rhs);

	// realm info accessors
	BOOL FindRealmName(LPCTSTR lpBnetIP, LPCTSTR lpPreferred, LPTSTR lpName) const;
	BOOL FindRealmDetail(LPCTSTR lpName, LPTSTR lpBnetIP, LPTSTR lpPreferred) const;
	BOOL GetRealmByIndex(int nIndex, LPTSTR lpName, LPTSTR lpBnetIP, LPTSTR lpPreferred, int nOpt = RI_LIST_ALL) const;
	int GetRealmCount(int nOpt = RI_LIST_ALL) const;
	BOOL IsOfficialRealm(LPCTSTR lpName) const;	
	BOOL IsPrivateRealm(LPCTSTR lpName) const;
	BOOL IsRealmExist(LPCTSTR lpName) const;

	// operations
	int AddRealm(LPCTSTR lpName, LPCTSTR lpBnetIP, LPCTSTR lpPreferred);
	int RemoveRealm(LPCTSTR lpName);
	int EditRealm(LPCTSTR lpOldName, LPCTSTR lpNewName, LPCTSTR lpBnetIP, LPCTSTR lpPreferred);
	int Import(LPCTSTR lpFile, LPTSTR lpName, LPTSTR lpBnetIP, LPTSTR lpPreferred);
	int Export(LPCTSTR lpFile, LPCTSTR lpName) const;
	void InitPrivateRealms(LPCTSTR lpIniFile);
	void SavePrivateRealms(LPCTSTR lpIniFile) const;	

protected:	
	
	// protected methods
	static BOOL _SetMultiSz(LPCTSTR lpBNetIP);
	static BOOL _GetOfficialBNetIP(LPTSTR lpBNetIP, LPCTSTR lpPreferred);
	static int _FindMatchIPFromMultiSz(const CStringArray& arr, LPCTSTR lpBNetIP);
	static BOOL _IsOfficialIP(LPCTSTR lpBNetIP);
	 
	BOOL _FindRealmName(LPCTSTR lpBnetIP, LPCTSTR lpPreferred, CString& sName) const;
	BOOL _FindRealmDetail(LPCTSTR lpName, CString& sBnetIP, CString& sPreferred) const;
	int _Import(LPCTSTR lpFile, CString& sName, CString& sBnetIP, CString& sPreferred);
	int _FindInOfficial(LPCTSTR lpName) const;
	int _FindInPrivate(LPCTSTR lpName) const;
	void _InitOfficialRealms();

	// data
	CArray<CD2Realm, const CD2Realm&> m_aOfficial;
	CArray<CD2Realm, const CD2Realm&> m_aPrivate;
};

// class CD2Realm, stores basic realm info, this class should not be used externally
class CD2Realm
{
	friend class CRealmInfo;
public:
	CD2Realm();
	CD2Realm(const CD2Realm& rhs);
	CD2Realm(LPCTSTR lpName, LPCTSTR lpBnetIP, LPCTSTR lpPreferred = NULL);

	BOOL operator==(const CD2Realm& rhs) const;
	const CD2Realm& operator=(const CD2Realm& rhs);

protected:
	CString m_sName;
	CString m_sBnetIP;
	CString m_sPreferred;
};

#endif // __REALMINFO_H__
