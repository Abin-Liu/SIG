// SingleSelChkList.cpp : implementation file
//

#include "stdafx.h"
#include "D2Gateway.h"
#include "SingleSelChkList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSingleSelChkList

CSingleSelChkList::CSingleSelChkList()
{
}

CSingleSelChkList::~CSingleSelChkList()
{
}


BEGIN_MESSAGE_MAP(CSingleSelChkList, CListCtrl)
	//{{AFX_MSG_MAP(CSingleSelChkList)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSingleSelChkList message handlers

void CSingleSelChkList::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	SetExtendedStyle(LVS_EX_CHECKBOXES);
	CListCtrl::PreSubclassWindow();
}

void CSingleSelChkList::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	const int nCurSel = HitTest(point);
	BOOL bOrig = GetCheck(nCurSel);

	CListCtrl::OnLButtonDown(nFlags, point);

	if (nCurSel >= 0)
	{
		BOOL bNow = GetCheck(nCurSel);
		if (bNow)
		{
			for (int i = 0; i < GetItemCount(); i++)
			{
				if (i != nCurSel && GetCheck(i))
					SetCheck(i, FALSE);
			}			
		}

		SetItemState(nCurSel, LVIS_SELECTED, LVIS_SELECTED);

		if (bOrig != bNow)
			GetParent()->PostMessage(WM_SSCL_ITEMCHECKED, (WPARAM)0, (LPARAM)0);
	}
}

void CSingleSelChkList::UnCheckAll()
{
	const int COUNT = GetItemCount();
	for (int i = 0; i < COUNT; i++)
		SetCheck(i, FALSE);
}

void CSingleSelChkList::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	const int nCurSel = HitTest(point);
	BOOL bOrig = GetCheck(nCurSel);

	CListCtrl::OnRButtonDown(nFlags, point);

	if (nCurSel >= 0)
	{
		BOOL bNow = GetCheck(nCurSel);
		if (bNow)
		{
			for (int i = 0; i < GetItemCount(); i++)
			{
				if (i != nCurSel && GetCheck(i))
					SetCheck(i, FALSE);
			}			
		}

		SetItemState(nCurSel, LVIS_SELECTED, LVIS_SELECTED);

		if (bOrig != bNow)
			GetParent()->PostMessage(WM_SSCL_ITEMCHECKED, (WPARAM)0, (LPARAM)0);
	}
}

void CSingleSelChkList::OnMButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	const int nCurSel = HitTest(point);
	BOOL bOrig = GetCheck(nCurSel);

	CListCtrl::OnMButtonDown(nFlags, point);

	if (nCurSel >= 0)
	{
		BOOL bNow = GetCheck(nCurSel);
		if (bNow)
		{
			for (int i = 0; i < GetItemCount(); i++)
			{
				if (i != nCurSel && GetCheck(i))
					SetCheck(i, FALSE);
			}			
		}

		SetItemState(nCurSel, LVIS_SELECTED, LVIS_SELECTED);

		if (bOrig != bNow)
			GetParent()->PostMessage(WM_SSCL_ITEMCHECKED, (WPARAM)0, (LPARAM)0);
	}
}
