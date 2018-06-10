// DicEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SIG.h"
#include "DicEditDlg.h"
#include "StrSplit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDicEditDlg dialog


CDicEditDlg::CDicEditDlg(CStringArray* pLines, BOOL bRandDic, CWnd* pParent /*=NULL*/)
	: m_pLines(pLines), CDialog(CDicEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDicEditDlg)
	m_sContents = _T("");
	m_nDicUse = -1;
	//}}AFX_DATA_INIT
	m_nDicUse = bRandDic ? 0 : 1;
}


void CDicEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDicEditDlg)
	DDX_Text(pDX, IDC_EDIT1, m_sContents);
	DDX_Radio(pDX, IDC_RADIO1, m_nDicUse);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDicEditDlg, CDialog)
	//{{AFX_MSG_MAP(CDicEditDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDicEditDlg message handlers



BOOL CDicEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_pLines != NULL)
	{
		for (int i = 0; i < m_pLines->GetSize(); i++)
			m_sContents += m_pLines->GetAt(i) + _T("\r\n");
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDicEditDlg::GetLines(CStringArray &a) const
{
	a.RemoveAll();

	CStringArray aTemp;
	StrSplit(m_sContents, aTemp, _T('\n'));

	for (int i = 0; i < aTemp.GetSize(); i++)
	{
		aTemp[i].TrimLeft();
		aTemp[i].TrimRight();

		if (!aTemp[i].IsEmpty())
		{
			if (FindDupeString(a, aTemp[i]) == -1)
				a.Add(aTemp[i]);

		}			
	}
}

// finds and removes duplicated game names
int CDicEditDlg::FindDupeString(const CStringArray &a, LPCTSTR lpsz) const
{
	for (int i = 0; i < a.GetSize(); i++)
	{
		if (a[i].CompareNoCase(lpsz) == 0)
			return i;
	}

	return -1;
}

BOOL CDicEditDlg::IsRandDic() const
{
	return m_nDicUse == 0;
}
