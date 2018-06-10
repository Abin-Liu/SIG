#include "stdafx.h"
#include "StrSplit.h"

int StrSplit(const CString &str, CStringArray& arr, const CString& sDelimitior)
{
	int nStart = 0, nEnd = 0;
	arr.RemoveAll();

	while (nEnd < str.GetLength())
	{
		// determine the paragraph ("xxx,xxx,xxx;")
		nEnd = str.Find(sDelimitior, nStart);
		if( nEnd == -1 )
		{
			// reached the end of string
			nEnd = str.GetLength();
		}

		CString s = str.Mid(nStart, nEnd - nStart);
		if (!s.IsEmpty())
			arr.Add(s);

		nStart = nEnd + sDelimitior.GetLength();
	}	

	return arr.GetSize();
}

int StrSplit(const CString &str, CStringArray& arr, TCHAR chDelimitior)
{
	int nStart = 0, nEnd = 0;
	arr.RemoveAll();

	while (nEnd < str.GetLength())
	{
		// determine the paragraph ("xxx,xxx,xxx;")
		nEnd = str.Find(chDelimitior, nStart);
		if( nEnd == -1 )
		{
			// reached the end of string
			nEnd = str.GetLength();
		}

		CString s = str.Mid(nStart, nEnd - nStart);
		if (!s.IsEmpty())
			arr.Add(s);

		nStart = nEnd + 1;
	}	

	return arr.GetSize();
}

BOOL GetParameter(LPCTSTR lpParamLine, LPCTSTR lpParamName, CString& sParamValue)
{
	sParamValue.Empty();
	CString sLine(lpParamLine), sName(lpParamName);
	sLine.TrimLeft();
	sLine.TrimRight();
	if (sLine.IsEmpty())
		return FALSE;

	sName.TrimLeft();
	sName.TrimRight();
	
	sLine.MakeLower();
	sName.MakeLower();

	sLine += _T(" ");
	sName += _T(" ");

	int nNameLen = sName.GetLength();
	int nPos = sLine.Find(sName);
	if (nPos == -1)
	{
		nPos = sLine.Find(CString(_T("-")) + sName);
		if (nPos != -1)
			nNameLen++;
	}

	if (nPos == -1)
	{
		nPos = sLine.Find(CString(_T("/")) + sName);
		if (nPos != -1)
			nNameLen++;
	}

	if (nPos == -1)
		return FALSE;

	sLine = sLine.Mid(nPos + nNameLen);
	sLine.TrimLeft();

	if (sLine[0] == _T('\"'))
	{
		// multiple segments
		sLine.Delete(0);
		int nQuote = sLine.Find(_T('\"'));
		if (nQuote == -1)
			sParamValue = sLine;
		else
			sParamValue = sLine.Left(nQuote);

	}
	else
	{
		// single segment
		int nSpace = sLine.Find(_T(' '));
		if (nSpace == -1)
			sParamValue = sLine;
		else
			sParamValue = sLine.Left(nSpace);
	}
	
	return TRUE;
}
