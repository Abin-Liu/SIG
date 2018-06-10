//////////////////////////////////////////////////////////////////////
// D2Spy.h
//
// A class for spying Diablo II game status.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#ifndef __D2SPY_H__
#define __D2SPY_H__

typedef struct playerinfostruct_t_110
{
	BYTE		UnitType;		// +0x00
	DWORD		CharacterClass;	// +0x04
	DWORD		unknown1;		// +0x08
	DWORD		PlayerID;		// +0x0c
	DWORD		PlayerLocation;	// +0x10 (defines location somehow in or out of town and maybe other locations (0x05=in town, 0x01=out of town)
	LPCSTR		PlayerName;		// +0x14 pointer to LPSZ player name
	BYTE		Act;			// +0x18
	BYTE		unknown2[0x73];	// +0x19
	WORD		PlayerPositionX;// +0x8c
	WORD		PlayerPositionY;// +0x8e
	
} PLAYERINFOSTRUCT, *PPLAYERINFOSTRUCT;

typedef struct tagPlayer
{
	BYTE iCharClass;
	DWORD dwPlayerID;
	DWORD dwPlayerLocation;
	TCHAR szPlayeName[16];
	BYTE iAct;
	WORD x;
	WORD y;
} PLAYER, *LPPLAYER;
typedef const tagPlayer* LPCPLAYER;

enum { CREATE_SUCCEED = 0,		// Game created successfully
	   CREATE_GAME_NOT_RUNNING, // Diablo II is not running
	   CREATE_FAILED,			// Failed to join game
	   CREATE_NAME_INVALID,		// Invalid game name (e.g. "fuck Blizzard" will result this)
	   CREATE_NAME_EXIST,		// Name already exists
	   CREATE_SERVER_DOWN,		// Server down
	   CREATE_NOT_YET,			// Game is still under creation, need to wait
	   CREATE_NOT_EXIST,		// Game not exists, cdkey banned?
	   CREATE_UNKNOWN			// Exception
};

class CD2Spy  
{
public:		
	
	CD2Spy();
	virtual ~CD2Spy();

	BOOL SetD2Wnd(HWND hWnd);

	DWORD IsInGame() const;
	LPCPLAYER GetPlayer() const;
	BOOL UpdatePlayerData();
	HANDLE GetD2ProcessHandle() const;
	int GetCreationResult() const;
	BOOL IsInChatRoom() const;
	DWORD GetCharCount() const;
	BOOL IsBNetLoginSuccessful() const;
	BOOL IsBNetConnectSuccessful() const;
	DWORD GetCreationQueue() const;

protected:
	BOOL ReadProcessMem(DWORD dwAddr, LPVOID lpBuffer, DWORD dwBufSize) const;
	DWORD m_dwProcessID;
	HANDLE m_hProcess;
	PLAYER m_player;
};

#endif // __D2SPY_H__