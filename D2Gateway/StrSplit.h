#ifndef __STRSPLIT_H__
#define __STRSPLIT_H__

int StrSplit(const CString &str, CStringArray& arr, const CString& sDelimitior);
int StrSplit(const CString &str, CStringArray& arr, TCHAR chDelimitior);
BOOL GetParameter(LPCTSTR lpParamLine, LPCTSTR lpParamName, CString& sParamValue);

#endif