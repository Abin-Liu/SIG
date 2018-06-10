#ifndef __AUTOITFUNCS_H__
#define __AUTOITFUNCS_H__

#include "MsgSender.h"

class CAutoFunc
{
public:
	void CancelQueue();
	void JoinGame(LPCTSTR lpName, LPCTSTR lpPwd, DWORD dwOpenDelay = 2000, DWORD dwSwitchDelay = 200);
	BOOL SendSysKey(UINT nKeyCode);
	void CleanDlg();
	void CancelConnect();
	void SendEscape();
	void SetKeyDelay(DWORD dwDelay);
	void ConnectToBNet();
	CAutoFunc();
	
	void SetTargetHWnd(HWND hWnd);
	static POINT GetD2WndCursorPos(HWND hD2Wnd);

	// Out-game functions
	void Login(LPCTSTR lpAccount, LPCTSTR lpPassword);
	void SelectChar(int nCharPosition);
	void JoinChannel(LPCTSTR lpChannelName, LPCTSTR lpAwayMsg);
	void CreateGame(LPCTSTR lpName, LPCTSTR lpPwd, int nDifficulty = 0, DWORD dwOpenDelay = 2000, DWORD dwSwitchDelay = 200);

protected:
	
	// static functions for multi-threading calls
	static UINT __CreateGame(LPVOID lpParam);
	static UINT __JoinGame(LPVOID lpParam);
	static UINT __Login(LPVOID lpParam);
	static UINT __ConnectToBNet(LPVOID lpParam);
	static UINT __JoinChannel(LPVOID lpParam);

protected:
	
	CMsgSender m_sender;
	HWND m_hD2Wnd;
	BOOL m_bTrayed;
};

#endif