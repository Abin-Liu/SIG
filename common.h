#ifndef __COMMON_H__
#define __COMMON_H__

#include <windows.h>
#include <tchar.h>

#define	APP_ID _T("{B6556CCD-9CC5-4B70-8071-7E688C193794}") // application uuid
#define FLAG_THISAPP 0x6FE30D4F // Application flag identity for IPC

#define MAX_MSGSTR		64 // maximum message string length
#define MAX_GAMENAMEPWD	16 // maximum gamename/pwd string length

// memory position definitions
#define MEM_SIG_HWND		0 // the control window handle entry
#define MEM_SIG_ANTIIDLE	(MEM_SIG_HWND + 4) // Anti-idle switch
#define MEM_SIG_STOP		(MEM_SIG_ANTIIDLE + 4) // control stop flag
#define MEM_SIG_PAUSE		(MEM_SIG_STOP + 4) // control pause flag
#define MEM_SIG_MSGSTR		(MEM_SIG_PAUSE + 4) // message string entry
#define MEM_SIG_NEXTGAME	(MEM_SIG_MSGSTR + MAX_MSGSTR) // next game type (0=create, 1=join)
#define MEM_SIG_NEXTNAME	(MEM_SIG_NEXTGAME + 4) // next game name
#define MEM_SIG_NEXTPWD		(MEM_SIG_NEXTNAME + MAX_GAMENAMEPWD) // next game password
#define MEM_SIG_END			(MEM_SIG_NEXTPWD + MAX_GAMENAMEPWD) // the end of shared-memory block

#define MEM_ALLOC_SIZE	MEM_SIG_END // memory allocate size

// message definitions
#define WM_SIG				(WM_APP + 1000)
#define WM_SIG_CREATE		WM_SIG + 102 // sent upon requesting a game-recreatiom
#define WM_SIG_LOADED		WM_SIG + 103 // sent upon sig.d2h loading
#define WM_SIG_UNLOADED		WM_SIG + 104 // sent upon sig.d2h unloading
#define WM_SIG_PAUSE		WM_SIG + 105 // sent upon pause
#define WM_SIG_RESUME		WM_SIG + 106 // sent upon resume
#define WM_SIG_HEARTBEAT	WM_SIG + 107 // sent upon 0x8f packet receive
#define WM_SIG_MSGSTR		WM_SIG + 108 // notify the ctrl wnd that a msg string arrived
#define WM_SIG_STOP			WM_SIG + 109 // completely stop SIG control
#define WM_SIG_PKTRCV		WM_SIG + 110 // sent upon packets receiving
#define WM_SIG_PKTSND		WM_SIG + 111 // sent upon packets sending
#define WM_SIG_ANTIIDLE		WM_SIG + 112 // sent upon anti-idle performing
#define WM_SIG_QUITD2		WM_SIG + 114 // quit d2
#define WM_SIG_SHOWD2		WM_SIG + 115 // show Diablo II window
#define WM_SIG_HIDED2		WM_SIG + 116 // hide Diablo II window
#define WM_SIG_NOTIFY		WM_SIG + 117 // notify message
#define WM_SIG_WHISPER		WM_SIG + 118 // received a whisper message from game
#define WM_SIG_KEY			WM_SIG + 119 // send a key stroke to d2 window
#define WM_SIG_UNKNOWN		WM_SIG + 120 // place holder

#endif