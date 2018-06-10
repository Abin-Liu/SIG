// MulePage.cpp : implementation file
//

#include "stdafx.h"
#include "SIG.h"
#include "MulePage.h"
#include "AccountDlg.h"
#include "ArrayEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMulePage property page

IMPLEMENT_DYNCREATE(CMulePage, CPropertyPage)

CMulePage::CMulePage() : CPropertyPage(CMulePage::IDD)
{
	//{{AFX_DATA_INIT(CMulePage)
	m_nMuleCharStayLength = 10000;
	//}}AFX_DATA_INIT
	m_bPageInitialized = FALSE;
}

CMulePage::~CMulePage()
{
}

void CMulePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMulePage)
	DDX_Control(pDX, IDC_EDIT, m_wndedit);
	DDX_Control(pDX, IDC_ADD, m_wndAdd);
	DDX_Control(pDX, IDC_REMOVE, m_wndremove);
	DDX_Control(pDX, IDC_MOVEDOWN, m_wndMoveDown);
	DDX_Control(pDX, IDC_MOVEUP, m_wndMoveUp);
	DDX_Control(pDX, IDC_MULECHARSTAYLENGTH, m_wndMuleCharStayLength);
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	DDX_Text(pDX, IDC_MULECHARSTAYLENGTH, m_nMuleCharStayLength);
	DDV_MinMaxInt(pDX, m_nMuleCharStayLength, 5, 999999);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMulePage, CPropertyPage)
	//{{AFX_MSG_MAP(CMulePage)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_MOVEUP, OnMoveup)
	ON_BN_CLICKED(IDC_MOVEDOWN, OnMovedown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMulePage message handlers

BOOL CMulePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndMoveUp.SetIcon(IDI_UP);
	m_wndMoveDown.SetIcon(IDI_DOWN);
	m_wndAdd.SetIcon(IDI_NEW);
	m_wndedit.SetIcon(IDI_EDITDIC);
	m_wndremove.SetIcon(IDI_REMOVE);
	
	m_wndList.SetCheckboxeStyle(RC_CHKBOX_NORMAL);
	m_wndList.SetColumnHeader(_T("账号名, 80; 国度IP, 90; 分支国度, 70; 角色数量, 70"));
	m_bPageInitialized = TRUE;
	ListAccounts();
	// CG: The following block was added by the ToolTips component.
	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);

		// TODO: Use one of the following forms to add controls:
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), "<text>");
		m_tooltip.AddTool(GetDlgItem(IDC_ADD), _T("添加新账号"));
		m_tooltip.AddTool(GetDlgItem(IDC_EDIT), _T("更改账号内容"));
		m_tooltip.AddTool(GetDlgItem(IDC_REMOVE), _T("从列表中移除账号"));
		m_tooltip.AddTool(GetDlgItem(IDC_MOVEUP), _T("上移(使账号被优先登录)"));
		m_tooltip.AddTool(GetDlgItem(IDC_MOVEDOWN), _T("下移(使账号被推迟登录)"));
		m_tooltip.AddTool(GetDlgItem(IDC_LIST1), _T("点击账号名前的选取框以启用/禁用账号，点击表头进行排序"));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMulePage::ApplySettings(CD2ConfigData &data) const
{
	if (m_bPageInitialized)
	{
		data.m_aMules.Copy(m_aAccounts);
		data.m_nMuleCharStayLength = m_nMuleCharStayLength;
	}
}

void CMulePage::LoadSettings(const CD2ConfigData &data)
{
	m_ri = data.m_ri;
	m_aAccounts.Copy(data.m_aMules);
	m_nMuleCharStayLength = data.m_nMuleCharStayLength;
}

void CMulePage::ListAccounts()
{
	for (int i = 0; i < m_aAccounts.GetSize(); i++)
	{
		CString sCharCount;
		sCharCount.Format(_T("%d"), m_aAccounts[i].GetCharCount());
		int n = m_wndList.InsertItemEx(INT_MAX, m_aAccounts[i].GetName(),
								  m_aAccounts[i].GetBNetIP(),
								  m_aAccounts[i].GetSubRealm(),
								  sCharCount);

		m_wndList.SetItemData(n, DWORD(new CString(m_aAccounts[i].GetPassword())));
		m_wndList.SetCheck(n, m_aAccounts[i].IsEnabled());
	}
}

void CMulePage::OnAdd() 
{
	// TODO: Add your control notification handler code here
	CAccountDlg dlg(&m_ri);
	if (dlg.DoModal() == IDOK)
	{
		CString sCharCount;
		sCharCount.Format(_T("%d"), dlg.GetCharCount());
		const int n = m_wndList.InsertItemEx(INT_MAX, dlg.GetName(), dlg.GetBNetIP(), dlg.GetSubRealm(), sCharCount);
		m_wndList.SetItemData(n, DWORD(new CString(dlg.GetPassword())));
		m_wndList.SetCheck(n, TRUE);
	}
}

void CMulePage::OnEdit() 
{
	// TODO: Add your control notification handler code here
	const int IDX = m_wndList.GetFirstItem(RC_ITEM_SELECTED);
	if (IDX < 0)
		return;

	CAccountDlg dlg(&m_ri);
	dlg.SetName(m_wndList.GetItemText(IDX, 0));
	CString* p = (CString*)(m_wndList.GetItemData(IDX));
	dlg.SetPassword(*p);
	dlg.SetBNetIP(m_wndList.GetItemText(IDX, 1));
	dlg.SetSubRealm(m_wndList.GetItemText(IDX, 2));
	dlg.SetCharCount(_ttoi(m_wndList.GetItemText(IDX, 3)));

	if (dlg.DoModal() == IDOK)
	{
		CString sCharCount;
		sCharCount.Format(_T("%d"), dlg.GetCharCount());
		m_wndList.SetItemText(IDX, 0, dlg.GetName());
		m_wndList.SetItemText(IDX, 1, dlg.GetBNetIP());
		m_wndList.SetItemText(IDX, 2, dlg.GetSubRealm());
		m_wndList.SetItemText(IDX, 3, sCharCount);
		CString* p = (CString*)(m_wndList.GetItemData(IDX));
		if (p != NULL)
			*p = dlg.GetPassword();
	//	}
	//	else
	//	{
		//	MessageBox(_T("账号已存在。"), _T("重复数据"), MB_ICONEXCLAMATION);
	//	}
	}
}

void CMulePage::OnRemove() 
{
	// TODO: Add your control notification handler code here
	for (int i = m_wndList.GetItemCount() - 1; i >= 0; i--)
	{
		if (m_wndList.ExamItemStates(i, RC_ITEM_SELECTED))
		{
			CString* p = (CString*)(m_wndList.GetItemData(i));
			if (p != NULL)
			{
				delete p;
				m_wndList.SetItemData(i, NULL);
			}

			m_wndList.DeleteItem(i, FALSE);
		}
	}
}

void CMulePage::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnEdit();
	*pResult = 0;
}

void CMulePage::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	const int n = m_wndList.GetSelectedCount();
	m_wndMoveUp.EnableWindow(n == 1);
	m_wndMoveDown.EnableWindow(n == 1);
	GetDlgItem(IDC_EDIT)->EnableWindow(n > 0);
	GetDlgItem(IDC_REMOVE)->EnableWindow(n > 0);
	*pResult = 0;
}

void CMulePage::OnDestroy() 
{
	// TODO: Add your message handler code here
	for (int i = m_wndList.GetItemCount() - 1; i >= 0; i--)
	{
		CString* p = (CString*)(m_wndList.GetItemData(i));
		if (p != NULL)
		{
			delete p;
			m_wndList.SetItemData(i, NULL);
		}
	}

	CPropertyPage::OnDestroy();
}

void CMulePage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_aAccounts.RemoveAll();
	m_aAccounts.SetSize(m_wndList.GetItemCount());
	for (int i = 0; i < m_wndList.GetItemCount(); i++)
	{
		m_aAccounts[i].SetName(m_wndList.GetItemText(i, 0));
		CString* p = (CString*)(m_wndList.GetItemData(i));
		if (p != NULL)
			m_aAccounts[i].SetPassword(*p);
		m_aAccounts[i].SetBNetIP(m_wndList.GetItemText(i, 1));
		m_aAccounts[i].SetSubRealm(m_wndList.GetItemText(i, 2));
		m_aAccounts[i].SetCharCount(_ttoi(m_wndList.GetItemText(i, 3)));
		m_aAccounts[i].Enable(m_wndList.GetCheck(i));
	}

	CPropertyPage::OnOK();
}

void CMulePage::OnMoveup() 
{
	// TODO: Add your control notification handler code here
	if (m_wndList.GetSelectedCount() != 1)
		return;

	m_wndList.MoveUp(m_wndList.GetFirstItem(RC_ITEM_SELECTED));
}

void CMulePage::OnMovedown() 
{
	// TODO: Add your control notification handler code here
	if (m_wndList.GetSelectedCount() != 1)
		return;

	m_wndList.MoveDown(m_wndList.GetFirstItem(RC_ITEM_SELECTED));
	
}

BOOL CMulePage::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CPropertyPage::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}

BOOL CMulePage::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	g_nActivePage = PAGE_MULEMODE;
	return CPropertyPage::OnSetActive();
}
