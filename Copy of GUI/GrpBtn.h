// GrpBtn.h: interface for the CGrpBtn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRPBTN_H__F3D232BF_AEB5_441E_BF85_09A37E50340A__INCLUDED_)
#define AFX_GRPBTN_H__F3D232BF_AEB5_441E_BF85_09A37E50340A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>
#include "btnst.h"

class CGrpBtn  
{
public:
	void SetIconRes(UINT nChecked, UINT nUnchecked);
	int GetCheck() const;
	void SetCheck(int nIndex, BOOL bChangeIcon = TRUE, BOOL bForceUpdate = FALSE);
	CButtonST* Remove(const CButtonST* pBtn);
	void Add(CButtonST* pBtn);
	void RemoveAll();
	CGrpBtn();
	virtual ~CGrpBtn();

protected:
	CArray<CButtonST*, CButtonST*> m_abtns;
	UINT m_nIconResChecked;
	UINT m_nIconResUnchecked;
	int m_nCurSel;
};

#endif // !defined(AFX_GRPBTN_H__F3D232BF_AEB5_441E_BF85_09A37E50340A__INCLUDED_)
