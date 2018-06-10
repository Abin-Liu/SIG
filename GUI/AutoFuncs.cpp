#include "stdafx.h"
#include <windows.h>
#include "AutoFuncs.h"
#include "D2WndPosition.h"
#include "RealmInfo.h"

// parameters for creating a game
struct CREATEPARAM
{
	TCHAR szName[32];
	TCHAR szPwd[32];
	int nDifficulty;
	CMsgSender* pSender;
	DWORD dwOpenDelay;
	DWORD dwSwitchDelay;
};

// parameters for joining a game
struct JOINPARAM
{
	TCHAR szName[32];
	TCHAR szPwd[32];
	CMsgSender* pSender;
	DWORD dwOpenDelay;
	DWORD dwSwitchDelay;
};

// parameters for connecting to bnet
struct CONNECTPARAM
{
	CMsgSender* pSender;
};

// parameters for logging in
struct LOGINPARAM
{
	TCHAR szAccount[32];
	TCHAR szPassword[32];
	CMsgSender* pSender;
	BOOL bTrayed;
	HWND hD2Wnd;
};

// parameters for channel stuff
struct CHANNELPARAM
{
	CMsgSender* pSender;
	TCHAR szChannelName[129];
	TCHAR szMessage[129];
};

CREATEPARAM		g_createParam;
JOINPARAM		g_joinParam;
CONNECTPARAM	g_connectParam;
LOGINPARAM		g_loginParam;
CHANNELPARAM	g_channelParam;

POINT CAutoFunc::GetD2WndCursorPos(HWND hD2Wnd)
{
	CRect rcWnd, rcClient;
	::GetWindowRect(hD2Wnd, &rcWnd);
	::GetClientRect(hD2Wnd, &rcClient);
	const int cx = (rcWnd.Width() - rcClient.Width()) / 2; // border frame width
	const int cy = rcWnd.Height() - rcClient.Height() - cx; // title bar height
	POINT pt;
	::GetCursorPos(&pt);
	pt.x -= rcWnd.left + cx;
	pt.y -= rcWnd.top + cy;
	return pt;
}

UINT CAutoFunc::__CreateGame(LPVOID lpParam)
{
	POINT pt;

	pt = GetD2WndCursorPos(g_createParam.pSender->GetTargetHWnd());
	// location of the "Create" button
	g_createParam.pSender->LeftClick(CREATE_BUTTON.x, CREATE_BUTTON.y);
	g_createParam.pSender->MouseMove(pt.x, pt.y);

	Sleep(g_createParam.dwOpenDelay);

	pt = GetD2WndCursorPos(g_createParam.pSender->GetTargetHWnd());
	g_createParam.pSender->LeftClick(CREATE_BUTTON.x, CREATE_BUTTON.y);
	g_createParam.pSender->MouseMove(pt.x, pt.y);
	Sleep(500);

	// location of "game name" text box
	pt = GetD2WndCursorPos(g_createParam.pSender->GetTargetHWnd());
	g_createParam.pSender->LDblClick(CREATENAME_BOX.x, CREATENAME_BOX.y);
	g_createParam.pSender->MouseMove(pt.x, pt.y);
	Sleep(g_createParam.dwSwitchDelay);
	
	g_createParam.pSender->SendString(g_createParam.szName); // send game name	
	Sleep(g_createParam.dwSwitchDelay);

	if (_tcslen(g_createParam.szPwd) > 0)
	{
		g_createParam.pSender->SendSysKey(VK_TAB); // move to pwd editbox
		Sleep(g_createParam.dwSwitchDelay);

		g_createParam.pSender->SendString(g_createParam.szPwd);	// send pwd
		Sleep(g_createParam.dwSwitchDelay);
	}

	// select difficulties
	if (g_createParam.nDifficulty > 0 && g_createParam.nDifficulty <= 3)
	{
		pt = GetD2WndCursorPos(g_createParam.pSender->GetTargetHWnd());
		g_createParam.pSender->LeftClick(DIFFICULTIES[g_createParam.nDifficulty - 1].x, DIFFICULTIES[g_createParam.nDifficulty - 1].y);
		g_createParam.pSender->MouseMove(pt.x, pt.y);
		Sleep(g_createParam.dwSwitchDelay);
	}

	Sleep(1000);

	g_createParam.pSender->LeftClick(APPLY_CREATE.x, APPLY_CREATE.y);
	g_createParam.pSender->MouseMove(pt.x, pt.y); // done

	return 0;
}

UINT CAutoFunc::__Login(LPVOID lpParam)
{
	POINT pt;
	pt = GetD2WndCursorPos(g_loginParam.pSender->GetTargetHWnd());

	if (!CRealmInfo::IsDefaultAccountSame(g_loginParam.szAccount))
	{
		g_loginParam.pSender->LDblClick(ACCOUNTNAME_BOX.x, ACCOUNTNAME_BOX.y);
		g_loginParam.pSender->MouseMove(pt.x, pt.y);
		Sleep(1000);

		g_loginParam.pSender->LDblClick(ACCOUNTNAME_BOX.x, ACCOUNTNAME_BOX.y);
		g_loginParam.pSender->MouseMove(pt.x, pt.y);
		Sleep(500);

		g_loginParam.pSender->SendString(g_loginParam.szAccount);
		Sleep(200);

		g_loginParam.pSender->SendSysKey(VK_TAB);
		Sleep(500);
	}
	
	g_loginParam.pSender->SendString(g_loginParam.szPassword);
	Sleep(200);

	g_loginParam.pSender->LeftClick(APPLY_LOGIN.x, APPLY_LOGIN.y);
	g_loginParam.pSender->MouseMove(pt.x, pt.y); // done

	return 0;
}

UINT CAutoFunc::__ConnectToBNet(LPVOID lpParam)
{
	POINT pt;
	pt = GetD2WndCursorPos(g_connectParam.pSender->GetTargetHWnd());
	g_connectParam.pSender->LeftClick(BNET_BUTTON.x, BNET_BUTTON.y);
	g_connectParam.pSender->MouseMove(pt.x, pt.y);

	return 0;
}

UINT CAutoFunc::__JoinChannel(LPVOID lpParam)
{
	POINT pt;
	pt = GetD2WndCursorPos(g_channelParam.pSender->GetTargetHWnd());
	g_channelParam.pSender->LDblClick(CHAT_INPUT.x, CHAT_INPUT.y);
	g_channelParam.pSender->MouseMove(pt.x, pt.y);
	Sleep(500);
	CString str;
	if (g_channelParam.szMessage[0]!= _T('\0'))
	{
		str.Format(_T("/away %s"), g_channelParam.szMessage);
		g_channelParam.pSender->SendString(str);
		Sleep(100);
		g_channelParam.pSender->LeftClick(APPLY_CHAT.x, APPLY_CHAT.y);
		g_channelParam.pSender->MouseMove(pt.x, pt.y); // done
		str = _T("");
		Sleep(500);
	}

	if (g_channelParam.szChannelName[0] != _T('\0'))
	{
		str.Format(_T("/join %s"), g_channelParam.szChannelName);
		g_channelParam.pSender->SendString(str);
		Sleep(500);
		g_channelParam.pSender->LeftClick(APPLY_CHAT.x, APPLY_CHAT.y);
		g_channelParam.pSender->MouseMove(pt.x, pt.y); // done
	}

	return 0;
}

CAutoFunc::CAutoFunc()
{
	m_hD2Wnd = NULL;
	m_bTrayed = TRUE;
}

// Out-game functions
void CAutoFunc::Login(LPCTSTR lpAccount, LPCTSTR lpPassword)
{
	if (lpAccount == NULL || lpPassword == NULL)
		return;

	memset(&g_loginParam, NULL, sizeof(LOGINPARAM));
	_tcsncpy(g_loginParam.szAccount, lpAccount, 31);
	_tcsncpy(g_loginParam.szPassword, lpPassword, 31);
	g_loginParam.pSender = &m_sender;
	g_loginParam.bTrayed = m_bTrayed;
	g_loginParam.hD2Wnd = m_hD2Wnd;
	::AfxBeginThread(__Login, NULL);
}

void CAutoFunc::SelectChar(int nCharPosition)
{
	if (nCharPosition < 1 || nCharPosition > 8)
		nCharPosition = 1;

	POINT pt;
	pt = GetD2WndCursorPos(m_sender.GetTargetHWnd());
	m_sender.LDblClick(CHAR_POSITIONS[nCharPosition - 1].x,
		CHAR_POSITIONS[nCharPosition - 1].y);

	m_sender.MouseMove(pt.x, pt.y);
}

void CAutoFunc::JoinChannel(LPCTSTR lpChannelName, LPCTSTR lpAwayMsg)
{
	if (lpChannelName == NULL)
		return;

	memset(&g_channelParam, NULL, sizeof(CHANNELPARAM));
	_tcsncpy(g_channelParam.szChannelName, lpChannelName, 128);

	if (lpAwayMsg)
		_tcsncpy(g_channelParam.szMessage, lpAwayMsg, 128);

	g_channelParam.pSender = &m_sender;
	::AfxBeginThread(__JoinChannel, NULL);
}

void CAutoFunc::CreateGame(LPCTSTR lpName, LPCTSTR lpPwd, int nDifficulty, DWORD dwOpenDelay, DWORD dwSwitchDelay)
{
	if (lpName == NULL)
		return;

	memset(&g_createParam, NULL, sizeof(CREATEPARAM));
	_tcsncpy(g_createParam.szName, lpName, 31);

	if (lpPwd)
		_tcsncpy(g_createParam.szPwd, lpPwd, 31);

	g_createParam.nDifficulty = nDifficulty;
	g_createParam.pSender = &m_sender;
	g_createParam.dwOpenDelay = dwOpenDelay;
	g_createParam.dwSwitchDelay = dwSwitchDelay;
	::AfxBeginThread(__CreateGame, NULL);
}

void CAutoFunc::SetTargetHWnd(HWND hWnd)
{
	m_sender.SetTargetHWnd(hWnd);
	m_hD2Wnd = hWnd;
}

void CAutoFunc::ConnectToBNet()
{	
	::memset(&g_connectParam, NULL, sizeof(CONNECTPARAM));
	g_connectParam.pSender = &m_sender;
	::AfxBeginThread(__ConnectToBNet, NULL);
}

void CAutoFunc::SetKeyDelay(DWORD dwDelay)
{
	if (dwDelay >= 0 && dwDelay <= 500)
		m_sender.SetKeyDelay(dwDelay);
}

void CAutoFunc::SendEscape()
{
	m_sender.SendSysKey(VK_ESCAPE);
}

void CAutoFunc::CancelConnect()
{
	POINT pt = GetD2WndCursorPos(m_sender.GetTargetHWnd());
	m_sender.LeftClick(CANCEL_CONNECT.x, CANCEL_CONNECT.y);
	m_sender.MouseMove(pt.x, pt.y);
}

void CAutoFunc::CleanDlg()
{
	POINT pt = GetD2WndCursorPos(m_sender.GetTargetHWnd());
	m_sender.LeftClick(CLEAN_DLG.x, CLEAN_DLG.y);
	m_sender.MouseMove(pt.x, pt.y);
}

BOOL CAutoFunc::SendSysKey(UINT nKeyCode)
{
	return m_sender.SendSysKey(nKeyCode);
}

void CAutoFunc::JoinGame(LPCTSTR lpName, LPCTSTR lpPwd, DWORD dwOpenDelay, DWORD dwSwitchDelay)
{
	if (lpName == NULL)
		return;

	memset(&g_joinParam, NULL, sizeof(CREATEPARAM));
	_tcsncpy(g_joinParam.szName, lpName, 31);

	if (lpPwd)
		_tcsncpy(g_joinParam.szPwd, lpPwd, 31);

	g_joinParam.pSender = &m_sender;
	g_joinParam.dwOpenDelay = dwOpenDelay;
	g_joinParam.dwSwitchDelay = dwSwitchDelay;
	::AfxBeginThread(__JoinGame, NULL);
}

UINT CAutoFunc::__JoinGame(LPVOID lpParam)
{
	POINT pt;

	pt = GetD2WndCursorPos(g_joinParam.pSender->GetTargetHWnd());
	// location of the "Create" button
	g_joinParam.pSender->LeftClick(JOIN_BUTTON.x, JOIN_BUTTON.y);
	g_joinParam.pSender->MouseMove(pt.x, pt.y);

	Sleep(g_joinParam.dwOpenDelay);

	pt = GetD2WndCursorPos(g_joinParam.pSender->GetTargetHWnd());
	g_joinParam.pSender->LeftClick(JOIN_BUTTON.x, JOIN_BUTTON.y);
	g_joinParam.pSender->MouseMove(pt.x, pt.y);
	Sleep(500);

	// location of "game name" text box
	pt = GetD2WndCursorPos(g_joinParam.pSender->GetTargetHWnd());
	g_joinParam.pSender->LDblClick(JOINNAME_BOX.x, JOINNAME_BOX.y);
	g_joinParam.pSender->MouseMove(pt.x, pt.y);
	Sleep(g_joinParam.dwSwitchDelay);
	
	g_joinParam.pSender->SendString(g_joinParam.szName); // send game name	
	Sleep(g_joinParam.dwSwitchDelay);

	if (_tcslen(g_joinParam.szPwd) > 0)
	{
		g_joinParam.pSender->SendSysKey(VK_TAB); // move to pwd editbox
		Sleep(g_joinParam.dwSwitchDelay);

		g_joinParam.pSender->SendString(g_joinParam.szPwd);	// send pwd
		Sleep(g_joinParam.dwSwitchDelay);
	}	

	Sleep(500);

	g_joinParam.pSender->LeftClick(APPLY_CREATE.x, APPLY_CREATE.y);
	g_joinParam.pSender->MouseMove(pt.x, pt.y); // done

	return 0;
}

void CAutoFunc::CancelQueue()
{
	POINT pt = GetD2WndCursorPos(m_sender.GetTargetHWnd());
	m_sender.LeftClick(CANCEL_QUEUE.x, CANCEL_QUEUE.y);
	m_sender.MouseMove(pt.x, pt.y);
}
