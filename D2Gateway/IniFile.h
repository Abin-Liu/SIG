///////////////////////////////////////////////////////////////////
//                 IniFile.h (MFC)
//
// "CIniFile" is a simple API wrap class used for ini file access.
// The purpose of this class is to make ini file access more
// convenient than direct API calls.
//
// Also, a few methods are added to allow reading double values from
// ini file, and allow int/double values to be written into ini file,
// those functionalities were not available in direct API calls.
//
// 7/08/2002    Bin Liu
// All rights reserved.
//
// Update history:
//
// 7/08/2002 -- Initial release. (Bin Liu)
// 7/14/2002 -- Added "IncreaseProfileInt" and "AppendProfileString"
// 9/02/2002 -- Added "removeProfileSection" and "RemoveProfileEntry"
// 2/09/2003 -- The class has been made unicode-compliant 
//
///////////////////////////////////////////////////////////////////

#ifndef __CINIFILE_H__
#define __CINIFILE_H__

#include <windows.h>
#include <afxtempl.h>
#include <tchar.h>

class CIniFile  
{
public:

	//-----------------------------------------------------------
	//    Constructors/destructor
	//-----------------------------------------------------------
	CIniFile(); // Default constructor
	CIniFile(LPCTSTR lpPathName); // Construct with a given file name
	virtual ~CIniFile();

	//-----------------------------------------------------------
	//    Ini file path and name specification
	//-----------------------------------------------------------
	BOOL SetPathName(LPCTSTR lpPathName); // Specify a new file name
	const CString& GetPathName() const; // Retrieve current file name

	//------------------------------------------------------------
	//    Retrieve data from ini file
	//------------------------------------------------------------
	CString GetProfileString(LPCTSTR lpSection, LPCTSTR lpEntry, LPCTSTR lpDefault) const;
	double GetProfileDbl(LPCTSTR lpSection, LPCTSTR lpEntry, double fDefault) const;
	int GetProfileInt(LPCTSTR lpSection, LPCTSTR lpEntry, int nDefault) const;
	BOOL GetProfileBool(LPCTSTR lpSection, LPCTSTR lpEntry, BOOL bDefault) const;

	//------------------------------------------------------------
	//    Write data into ini file
	//------------------------------------------------------------
	BOOL WriteProfileString(LPCTSTR lpSection, LPCTSTR lpEntry, LPCTSTR lpValue);
	BOOL WriteProfileInt(LPCTSTR lpSection, LPCTSTR lpEntry, int nValue);
	BOOL WriteProfileBool(LPCTSTR lpSection, LPCTSTR lpEntry, BOOL bValue);
	BOOL WriteProfileDbl(LPCTSTR lpSection, LPCTSTR lpEntry, double fValue);
	BOOL AppendProfileString(LPCTSTR lpSection, LPCTSTR lpEntry, LPCTSTR lpString);
	BOOL IncreaseProfileInt(LPCTSTR lpSection, LPCTSTR lpEntry, int nIncrease = 1);
	
	//------------------------------------------------------------
	//    List all sections/keys of the ini file
	//------------------------------------------------------------
	DWORD GetProfileSectionNames(CStringArray& aList) const;
	DWORD GetProfileSection(LPCTSTR lpSection, CStringArray& aList) const;
	
	//------------------------------------------------------------
	//    Delete a section/entry from the ini file
	//------------------------------------------------------------
	BOOL RemoveProfileSection(LPCTSTR lpSection) const;
	BOOL RemoveProfileEntry(LPCTSTR lpSection, LPCTSTR lpEntry) const;

protected:
	// Helper functions, used for searching a specified section/entry in ini file
	static BOOL FindEntry(LPCTSTR lpEntry, LPCTSTR lpString);
	static BOOL IsEntry(LPCTSTR lpString, CString& sParsedResult);
	static BOOL FindSection(LPCTSTR lpSection, LPCTSTR lpString);
	static BOOL IsSection(LPCTSTR lpString, CString& sSection);

	// the only member data of this class
	CString m_sPathName; // The ini file path and name
};

#endif