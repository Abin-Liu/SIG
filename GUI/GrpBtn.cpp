// GrpBtn.cpp: implementation of the CGrpBtn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIG.h"
#include "GrpBtn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrpBtn::CGrpBtn()
{
	m_nCurSel = -1;
}

CGrpBtn::~CGrpBtn()
{

}

void CGrpBtn::RemoveAll()
{
	m_abtns.RemoveAll();
}

void CGrpBtn::Add(CButtonST *pBtn)
{
	m_abtns.Add(pBtn);
}

CButtonST* CGrpBtn::Remove(const CButtonST *pBtn)
{
	CButtonST* p = NULL;
	for (int i = 0; i < m_abtns.GetSize(); i++)
	{
		if (m_abtns[i] == pBtn)
		{
			p = m_abtns[i];
			m_abtns.RemoveAt(i);
			break;
		}
	}

	return p;
}

void CGrpBtn::SetCheck(int nIndex, BOOL bChangeIcon, BOOL bForceUpdate)
{
	if (nIndex < 0 || nIndex >= m_abtns.GetSize())
	{
		m_nCurSel = -1;
		for (int i = 0; i < m_abtns.GetSize(); i++)
		{
			m_abtns[i]->SetCheck(FALSE);

			if (bChangeIcon)
				m_abtns[i]->SetIcon(m_nIconResUnchecked);
		}
	}
	else
	{
		if (!bForceUpdate && m_nCurSel == nIndex)
		{
			m_abtns[m_nCurSel]->SetCheck(TRUE);
		}
		else
		{
			for (int i = 0; i < m_abtns.GetSize(); i++)
			{
				if (i == nIndex)
				{
					m_abtns[i]->SetCheck(TRUE);
					if (bChangeIcon)
						m_abtns[i]->SetIcon(m_nIconResChecked);
				}
				else
				{
					m_abtns[i]->SetCheck(FALSE);
					if (bChangeIcon)
						m_abtns[i]->SetIcon(m_nIconResUnchecked);
				}
			}
			m_nCurSel = nIndex;
		}
	}	
}

int CGrpBtn::GetCheck() const
{
	return m_nCurSel;
}

void CGrpBtn::SetIconRes(UINT nChecked, UINT nUnchecked)
{
	m_nIconResChecked = nChecked;
	m_nIconResUnchecked = nUnchecked;
}
