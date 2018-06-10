#include "stdafx.h"
#include "GUID.h"
#include "stdlib.h"
#include <time.h>

const TCHAR HEXLETTERS[] = _T("0123456789ABCDEF");

CString CreateFakeGUIDStr()
{
	TCHAR s1[9] = _T("");
	TCHAR s2[5] = _T("");
	TCHAR s3[5] = _T("");
	TCHAR s4[5] = _T("");
	TCHAR s5[13] = _T("");

	int i = 0;
	for (i = 0; i < 8; i++)
		s1[i] = HEXLETTERS[(rand() % 16)];
	for (i = 0; i < 4; i++)
		s2[i] = HEXLETTERS[(rand() % 16)];
	for (i = 0; i < 4; i++)
		s3[i] = HEXLETTERS[(rand() % 16)];
	for (i = 0; i < 4; i++)
		s4[i] = HEXLETTERS[(rand() % 16)];
	for (i = 0; i < 12; i++)
		s5[i] = HEXLETTERS[(rand() % 16)];

	CString str;
	str.Format(_T("%s_%s_%s_%s_%s"), s1, s2, s3, s4, s5);
	return str;
}

CString CreateGUIDStr()
{
	/* Not working on Win9x...
	UUID id;
	::UuidCreateSequential(&id);

	unsigned char **p = new unsigned char*;
	::UuidToString(&id, p);

	CString sid(*p);
	delete p;

	sid.MakeUpper();
	if (sid.GetLength() > 0 && sid.GetAt(0) != '{')
	{
		sid = CString('{') + sid + '}';
	}

	return sid;
	*/

	return CreateFakeGUIDStr();
}