// D2Spy.cpp: implementation of the CD2Spy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIG.h"
#include "D2Spy.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Memory offsets for D2 1.10
// June, 24, 2004, Abin

#define OF_BNET_CONNECT		(0x6FF00000+0x0001D159)//0x6FF1D15C
#define OF_BNET_LOGIN		(0x6FA10000+0x0002B80C)//0x6FA02E5B
#define OF_CHAR_NUM			(0x6FA10000+0x0002CA64)//0x6FA3CA64
#define OF_CHANNEL			(0x6FF00000+0x0001D92C)//0x6F8FE263
#define OF_CREATE_RESULT	(0x6F9F0000+0x00012D00)//0x6FA02D00
#define OF_CREATE_QUEUE		(0x6F9F0000+0x00012CC4)
#define OF_PLAYERDATA		0x6FB283C6

// creation results
#define CR_SUCCEED			0x00
#define CR_FAILED           0x01
#define CR_INVALID_NAME     0x1e
#define CR_NAME_EXIST       0x1f
#define CR_SERVER_DOWN      0x20
#define CR_WAIT			    0x32

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CD2Spy::CD2Spy()
{
	m_dwProcessID = 0;
	m_hProcess = NULL;
	::memset(&m_player, 0, sizeof(PLAYER));
}

CD2Spy::~CD2Spy()
{
	::CloseHandle(m_hProcess);
}

BOOL CD2Spy::SetD2Wnd(HWND hWnd)
{
	m_dwProcessID = 0;
	::CloseHandle(m_hProcess);
	m_hProcess = NULL;

	if (hWnd)
		::GetWindowThreadProcessId(hWnd, &m_dwProcessID);
	if (m_dwProcessID == 0)
		return FALSE;

	m_hProcess = ::OpenProcess(PROCESS_VM_READ, FALSE, m_dwProcessID);
	return m_hProcess != NULL;
}

BOOL CD2Spy::ReadProcessMem(DWORD dwAddr, LPVOID lpBuffer, DWORD dwBufSize) const
{
	if (m_hProcess == NULL || lpBuffer == NULL || dwBufSize == 0)
		return FALSE;
	
	DWORD dwRead = 0;
	return ::ReadProcessMemory(m_hProcess, (LPCVOID)dwAddr, lpBuffer, dwBufSize, &dwRead)
		&& dwRead == dwBufSize;
}

BOOL CD2Spy::IsBNetConnectSuccessful() const
{
	DWORD dwRes = 0;
	return ReadProcessMem(OF_BNET_CONNECT, &dwRes, 4) ? dwRes : FALSE;
}

BOOL CD2Spy::IsBNetLoginSuccessful() const
{
	if (!IsBNetConnectSuccessful())
		return FALSE;

	DWORD dwRes = 0;
	return ReadProcessMem(OF_BNET_LOGIN, &dwRes, 4) ? dwRes : FALSE;
}

DWORD CD2Spy::GetCharCount() const
{
	if (!IsBNetConnectSuccessful())
		return 0;

	DWORD dwRes = 0;
	return ReadProcessMem(OF_CHAR_NUM, &dwRes, 4) ? dwRes : 0;
}

BOOL CD2Spy::IsInChatRoom() const
{
	DWORD dwRes = 0;
	return ReadProcessMem(OF_CHANNEL, &dwRes, 4) ? dwRes : FALSE;	
}

int CD2Spy::GetCreationResult() const
{
	BYTE iRes = 0;
	if (!ReadProcessMem(OF_CREATE_RESULT, &iRes, sizeof(BYTE)))
		return CREATE_NOT_YET;

	int nResult = CREATE_UNKNOWN;
	switch (iRes)
	{
	case CR_SUCCEED:
		nResult = CREATE_SUCCEED;
		break;

	case CR_FAILED:
		nResult = CREATE_FAILED;
		break;

	case CR_INVALID_NAME:
		nResult = CREATE_NAME_INVALID;
		break;

	case CR_NAME_EXIST:
		nResult = CREATE_NAME_EXIST;
		break;

	case CR_SERVER_DOWN:
		nResult = CREATE_SERVER_DOWN;
		break;

	case CR_WAIT:
		nResult = CREATE_NOT_YET;
		break;

	default:
		break;
	}

	return nResult;
}

HANDLE CD2Spy::GetD2ProcessHandle() const
{
	return m_hProcess;
}

BOOL CD2Spy::UpdatePlayerData()
{	
	DWORD dwAddr = IsInGame();
	if (dwAddr == 0)
		return FALSE;
	
	DWORD dwDummy = 0;
	PLAYERINFOSTRUCT pis = {0};
	if (!::ReadProcessMemory(m_hProcess, (LPCVOID)dwAddr, &pis, sizeof(PLAYERINFOSTRUCT), &dwDummy))
		return FALSE;

	::memset(&m_player, 0, sizeof(PLAYER));
	m_player.iCharClass = (BYTE)pis.CharacterClass;
	m_player.dwPlayerID = pis.PlayerID;
	m_player.dwPlayerLocation = pis.PlayerLocation;
	m_player.iAct = pis.Act;
	m_player.x = pis.PlayerPositionX;
	m_player.y = pis.PlayerPositionY;
	if (!::ReadProcessMemory(m_hProcess, (LPCVOID)pis.PlayerName, m_player.szPlayeName, 16, &dwDummy))
		return FALSE;

	return TRUE;
}

LPCPLAYER CD2Spy::GetPlayer() const
{
	return &m_player;
}

DWORD CD2Spy::IsInGame() const
{
	if (m_hProcess == NULL)
		return 0;

	DWORD dwDummy = 0;
	DWORD dwVal = 0;
	DWORD dwAddr = 0;
	
	if (!::ReadProcessMemory(m_hProcess, (LPCVOID)OF_PLAYERDATA, &dwVal, 4, &dwDummy))
		return 0;

	if (!::ReadProcessMemory(m_hProcess, (LPCVOID)dwVal, &dwAddr, 4, &dwDummy))
		return 0;

	return dwAddr;
}

DWORD CD2Spy::GetCreationQueue() const
{
	DWORD dwQueue = 0;
	if (!::ReadProcessMemory(m_hProcess, (LPCVOID)OF_CREATE_QUEUE, &dwQueue, 4, NULL))
		return 0;

	return dwQueue;
}
