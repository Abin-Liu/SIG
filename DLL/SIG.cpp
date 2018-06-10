//////////////////////////////////////////////////////////////////////
// ClientImplementation.cpp
// -------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
// Sig.cpp
//
// The implementation file of SIG in-game controlling module. This module
// captures and interacts with Diablo II in-game packets/events and 
// communicates with the GUI via shared memory.
//
// Copyright (c) 2003
// Abin (abinn32@163.com)
// http://www.bridgeport.edu/~binliu/d2x/
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include ".\\External\\D2Hackit.h"
#include ".\\External\\D2Client.h"
#include "..\\common.h"
#include "SharedMem.h"

char ConfigPath[_MAX_PATH] = ""; // Dll path
FUNCTIONENTRYPOINTS	*server = NULL;

CSharedMem g_mem;
DWORD g_dwAntiIdleTick = 0;
BOOL g_bStopped = TRUE;
BOOL g_bPaused = FALSE;

//////////////////////////////////////////////////////////////////////
// CLIENTINFO
//////////////////////////////////////////////////////////////////////
DWORD   ModuleVersion=MAKELONG(0, 2);
char    ModuleAuthor[]="Abin";
char    ModuleWebsite[]="www.wxjindu.com/abin/";
char    ModuleDescription[]="SIG - Stay-in-Game control module";
char    ModuleEmail[]="abinn32@yahoo.com";

//////////////////////////////////////////////////////////////////////
// MODULECOMMANDSTRUCT ModuleCommands[]
// -------------------------------------------------------------------
// To add your own commands, just add them to this list like:
//
//	{
//		"name"			// Name of the command
//		pFunction		// The function that handles command
//		"usage"			// Help text for the command
//	}
// 
// ...where pFunction is the name of the function that handles the
// command. The function must be declared as...
//
// BOOL PRIVATE OnGameCommandName(char** argv, int argc)
//
// ...and return TRUE on success or FALSE to show usage of the command.
//
// The "usage" line can contain "\n" as linebreaks and should be in 
// the following format for consistency:
//
// "commandname <required parameter> [optional parameter]ÿc0 Description"
//
//
// To use your commands in the game, just type 
// .modulename <command> [arguments]
//////////////////////////////////////////////////////////////////////

BOOL PRIVATE OnGameCommandResume(char** argv, int argc);
BOOL PRIVATE OnGameCommandPause(char** argv, int argc);
BOOL PRIVATE OnGameCommandLeave(char** argv, int argc);
BOOL PRIVATE OnGameCommandQuitD2(char** argv, int argc);
BOOL PRIVATE OnGameCommandPostString(char** argv, int argc);
BOOL PRIVATE OnGameCommandStop(char** argv, int argc);
BOOL PRIVATE OnGameCommandShowD2(char** argv, int argc);
BOOL PRIVATE OnGameCommandHideD2(char** argv, int argc);
BOOL PRIVATE OnGameCommandKey(char** argv, int argc);
BOOL PRIVATE OnGameCommandNextGame(char** argv, int argc);
BOOL PRIVATE OnGameCommandNextName(char** argv, int argc);
BOOL PRIVATE OnGameCommandNextPassword(char** argv, int argc);
BOOL PRIVATE OnGameCommandDebug(char** argv, int argc);

MODULECOMMANDSTRUCT ModuleCommands[]=
{
	{
		"resume",
		OnGameCommandResume,
		"Enable SIG control.\n",
	},

	{
		"pause",
		OnGameCommandPause,
		"Disable SIG control.\n",
	},

	{
		"leave",
		OnGameCommandLeave,
		"Leave current game.\n",
	},

	{
		"quitd2",
		OnGameCommandQuitD2,
		"Quit Diablo II game.\n",
	},

	{
		"stop",
		OnGameCommandStop,
		"Stop SIG control.\n",
	},

	{
		"poststring",
		OnGameCommandPostString,
		"Post a string to the control window.\n",
	},

	{
		"showd2",
		OnGameCommandShowD2,
		"Restore Diablo II window. \n",
	},

	{
		"hided2",
		OnGameCommandHideD2,
		"Minimize Diablo II window. \n",
	},

	{
		"key",
		OnGameCommandKey,
		"Send a key-stroke to Diablo II window. \n",
	},

	{
		"nextgame",
		OnGameCommandNextGame,
		"Specify next game stats(create/join). \n",
	},

	{
		"nextname",
		OnGameCommandNextName,
		"Specify next game name. \n",
	},

	{
		"nextpassword",
		OnGameCommandNextPassword,
		"Specify next game password. \n",
	},

	{
		"debug",
		OnGameCommandDebug,
		"debug.\n",
	},

	{	// The 'help' command is implemented in ClientCore.cpp and 
		// should not need to be changed.
		"help",
		OnGameCommandHelp,
		"helpÿc0 List commands available in this module.\n"
		"<command> helpÿc0 Shows detailed help for <command> in this module."
	},
	// Add your own commands here
	{NULL}	// No more commands
};

BOOL SendOverHeadMsg(LPCTSTR lpMsg)
{
	int nSize = strlen(lpMsg);
	if (nSize == 0)
		return TRUE;

	int aLen = nSize + 6;
	BYTE* aPacket = new BYTE[aLen];
	aPacket[0] = 0x14;
	memset(aPacket + 1, 0, 2);
	memset(aPacket + 3 + nSize, 0, 3);
	for (int i = 0; i < nSize; i++)
	{
		memcpy(aPacket + 3 + i, lpMsg + i, 1);
	}
	server->GameSendPacketToServer(aPacket, aLen);
	delete [] aPacket;
	return TRUE;
}

BOOL SendSoundToServer(WORD sndID)
{
	BYTE packet[3];
	packet[0] = 0x3f;
	memcpy(&packet[1],&sndID,2);
	server->GameSendPacketToServer(packet,3);

	return true;
}

BOOL SendAntiIdle()
{
	DWORD dwTick = ::GetTickCount();
	if (dwTick % 2)
	{
		return SendOverHeadMsg("Anti-idle");
	}
	else
	{
		// send a random sound
		BYTE i = BYTE(dwTick % 0x08) + 0x19;
		return SendSoundToServer(i);
	}
}

BOOL IsJustStopped()
{
	BOOL bOrig = g_bStopped;
	g_bStopped = g_mem.GetLong(MEM_SIG_STOP) == FLAG_THISAPP;
	return !bOrig && g_bStopped; // originally not stopped, but now it is.
}

int IsJustPausedResumed()
{
	BOOL bOrig = g_bPaused;
	g_bPaused = g_mem.GetLong(MEM_SIG_PAUSE) == FLAG_THISAPP;
	if (!bOrig && g_bPaused)
	{
		// originally not paused, but now it is.
		return 1; // just paused
	}
	else if (bOrig && !g_bPaused)
	{
		// originally paused, but now it is not
		return 2; // just resumed
	}
	else
	{
		return 0;
	}
}

// D2JSP 0.31 has disabled in-game command line processing, I hence decided to do it
// through parsing messages characters said.
/*
UINT TranslateSaidMsg(BYTE* aPacket, int aLen, LPSTR lpBuffer, int nMaxBufferSize)
{
	if (aLen < 11 || lpBuffer == NULL || nMaxBufferSize < 1)
		return 0;

	char szHead[65] = "";
	strncpy(szHead, (LPCSTR)aPacket + 3, 4);

	// first portion must be "sig "
	if (stricmp(szHead, "sig ") != 0)
		return 0;

	// now need to find next space start from index 7

	strncpy(szHead, (LPCSTR)aPacket + 7, 64);
	int i = 0;
	for (i = 0; i < 64; i++)
	{
		if (szHead[i] == ' ')
		{
			szHead[i] = 0;
			break;
		}
	}

	if (stricmp(szHead, "pause") == 0)
	{
		//server->GamePrintInfo("pause");
		return WM_SIG_PAUSE;
	}
	else if (stricmp(szHead, "resume") == 0)
	{
		//server->GamePrintInfo("resume");
		return WM_SIG_RESUME;
	}
	else if (stricmp(szHead, "stop") == 0)
	{
		//server->GamePrintInfo("stop");
		return WM_SIG_STOP;
	}
	else if (stricmp(szHead, "leave") == 0)
	{
		//server->GamePrintInfo("leave");
		return WM_SIG_GAMELEAVE;
	}
	else if (stricmp(szHead, "quitd2") == 0)
	{
		//server->GamePrintInfo("quitd2");
		return WM_SIG_QUITD2;
	}
	else if (stricmp(szHead, "showd2") == 0)
	{
		//server->GamePrintInfo("showd2");
		return WM_SIG_SHOWD2;
	}
	else if (stricmp(szHead, "hided2") == 0)
	{
		//server->GamePrintInfo("hided2");
		return WM_SIG_HIDED2;
	}
	else if (stricmp(szHead, "key") == 0)
	{
		//server->GamePrintInfo("hided2");
		strncpy(lpBuffer, (LPCSTR)aPacket + 11, nMaxBufferSize);

		int nStrLen = strlen(lpBuffer);

		for (i = nStrLen - 1; i >= 0; i--)
		{
			if (lpBuffer[i] == 0x20) // white space
				lpBuffer[i] = 0; // truncate it
			else
				break;
		}

		return WM_SIG_KEY;
	}
	else if (stricmp(szHead, "poststring") == 0)
	{
		//server->GamePrintInfo("poststring");
		strncpy(lpBuffer, (LPCSTR)aPacket + 18, nMaxBufferSize);

		int nStrLen = strlen(lpBuffer);

		for (i = nStrLen - 1; i >= 0; i--)
		{
			if (lpBuffer[i] == 0x20) // white space
				lpBuffer[i] = 0; // truncate it
			else
				break;
		}

		// length verify
		if (strlen(lpBuffer) > MAX_MSGSTR)
			lpBuffer[MAX_MSGSTR] = '\0'; // exceed chars truncated, sorry


		return WM_SIG_MSGSTR;
	}
	else if (stricmp(szHead, "nextgame") == 0)
	{
		if (stricmp(szHead + 9, "create") == 0)
		{
			g_mem.SetLong(0, MEM_SIG_NEXTGAME);
			server->GamePrintInfo("ÿc7Next game: ÿc2Create");
			return WM_SIG_UNKNOWN;
		}
		else if (stricmp(szHead + 9, "join") == 0)
		{
			g_mem.SetLong(1, MEM_SIG_NEXTGAME);
			server->GamePrintInfo("ÿc7Next game: ÿc2Join");
			return WM_SIG_UNKNOWN;
		}
		else
		{
			g_mem.SetLong(0, MEM_SIG_NEXTGAME);
			g_mem.SetString("", MEM_SIG_NEXTNAME);
			server->GamePrintInfo("ÿc7Next game: reset to ÿc2Auto");
			return 0;
		}
	}
	else if (stricmp(szHead, "nextname") == 0)
	{
		g_mem.SetString(szHead + 9, MEM_SIG_NEXTNAME);
		if (strlen(szHead + 9) > 0)
		{
			server->GamePrintInfo("ÿc7Next game name specified.");
		}
		else
		{
			server->GamePrintInfo("ÿc7Next game name null, next game type reset to ÿc2Autoÿc7.");
		}
		return WM_SIG_UNKNOWN;
	}
	else if (stricmp(szHead, "nextpassword") == 0)
	{
		g_mem.SetString(szHead + 13, MEM_SIG_NEXTPWD);
		if (strlen(szHead + 13) > 0)
		{
			server->GamePrintInfo("ÿc7Next game password specified.");
		}
		else
		{
			server->GamePrintInfo("ÿc7Next game password null(this is OK).");
		}
		return WM_SIG_UNKNOWN;
	}
	else
	{
		// not a sig command
		//server->GamePrintInfo("non-sig command");
		return 0;
	}
}
*/
// Notifies the GUI using predefined messages.
BOOL NotifyCtrlWnd(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	int nResult;
	DWORD dw = g_mem.GetLong(MEM_SIG_HWND, &nResult);
	if (nResult > 0 && dw > 0)
	{
		::PostMessage((HWND)dw, nMsg, wParam, lParam);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void PostString(LPCTSTR lpString)
{
	g_mem.SetString(lpString, MEM_SIG_MSGSTR); // copy the string into memory
	NotifyCtrlWnd(WM_SIG_MSGSTR, (WPARAM)0, (LPARAM)0);
}

BOOL PRIVATE OnGameCommandDebug(char** argv, int argc)
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnClientStart
// -------------------------------------------------------------------
// Runs *once* when the module loads. 
// This function can be removed.
//
// Put any initialization code here.
//
// Return FALSE to prevent module from loading.
//////////////////////////////////////////////////////////////////////
BOOL EXPORT OnClientStart()
{
	char sMemID[321] = "";
	char szIni[_MAX_PATH] = "";
	sprintf(szIni, "%s\\SigMemory.ini", ConfigPath);

	::GetPrivateProfileString("Default", "Memory ID", "", sMemID, 320, szIni);
	
	if (strlen(sMemID) < 8)
	{
		server->GamePrintError("Memory ID: Invalid ID value.");
		return FALSE;
	}
	
	if (g_mem.Create(sMemID, MEM_ALLOC_SIZE))
	{
		NotifyCtrlWnd(WM_SIG_LOADED, (WPARAM)0, (LPARAM)0);
		return TRUE;
	}
	else
	{
		server->GamePrintError("Memory initialization failed.");
		return FALSE;
	}
	
}

//////////////////////////////////////////////////////////////////////
// OnClientStop
// -------------------------------------------------------------------
// Runs *once* before client is unloaded. 
// This function can be removed.
// 
// Put any cleanup code here.
//
// Return value should be TRUE.
//////////////////////////////////////////////////////////////////////
BOOL EXPORT OnClientStop()
{
	NotifyCtrlWnd(WM_SIG_UNLOADED, (WPARAM)0, (LPARAM)0);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnGamePacketBeforeSent
// -------------------------------------------------------------------
// Executes before packets are sent from the game to the server.
// 
// If you rewrite the packet, you must make sure to return the length
// of the new packet!
//
// NOTE: This event also works in single player games.
//////////////////////////////////////////////////////////////////////
DWORD EXPORT OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
{
	NotifyCtrlWnd(WM_SIG_PKTSND, (WPARAM)(aPacket[0]), (LPARAM)aLen);

	// we do not need to anti-idle if we are moving/casting/attacking
	if (aPacket[0] <= 0x11 && aPacket[0] != 0x0b)
	{
		g_dwAntiIdleTick = 0; // reset time ticker for anti-idle
	}

	if (aPacket[0] == 0x6c)
		NotifyCtrlWnd(WM_SIG_HEARTBEAT, (WPARAM)0, (LPARAM)0);

	// I no longer take care of D2JSP, don't translating said messages anymore
	/*
	if (aLen > 6
		&& aPacket[0] == 0x15
		&& aPacket[1] == 0x01
		&& aPacket[2] == 0x00)
	{
		UINT nKeyCode = 0;
		char sz[65] = "";
		const UINT MSGTYPE = TranslateSaidMsg(aPacket, aLen, sz, 64);
		BOOL bIsSigCmd = TRUE;
		switch (MSGTYPE)
		{
		case WM_SIG_PAUSE:
			OnGameCommandPause(NULL, 0);
			break;
		
		case WM_SIG_RESUME:
			OnGameCommandResume(NULL, 0);
			break;
		
		case WM_SIG_STOP:
			OnGameCommandStop(NULL, 0);
			break;

		case WM_SIG_GAMELEAVE:
			OnGameCommandLeave(NULL, 0);
			break;
			
		case WM_SIG_QUITD2:
			OnGameCommandQuitD2(NULL, 0);
			break;
		
		case WM_SIG_SHOWD2:
			OnGameCommandShowD2(NULL, 0);
			break;
		
		case WM_SIG_HIDED2:
			OnGameCommandHideD2(NULL, 0);
			break;
		
		case WM_SIG_MSGSTR:
			PostString(sz);
			break;

		case WM_SIG_KEY:
			nKeyCode = strtol(sz, NULL, 16);
			if (nKeyCode == 0)
			{
				server->GamePrintError("Syntax: sig key <key-code>");
			}
			else
			{					
				NotifyCtrlWnd(WM_SIG_KEY, (WPARAM)nKeyCode, (LPARAM)0);
			}	
			break;

		case WM_SIG_UNKNOWN:
			break;

		default:
			bIsSigCmd = FALSE;
			break;				
		}

		return bIsSigCmd ? 0 : aLen;
	}
	*/
	
	return aLen;
}

//////////////////////////////////////////////////////////////////////
// OnGamePacketBeforeReceived
// -------------------------------------------------------------------
// Executes before packets are received to the game from the server.
// 
// If you rewrite the packet, you must make sure to return the length
// of the new packet!
//
// NOTE: This event also works in single player games.
//////////////////////////////////////////////////////////////////////
DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{
	NotifyCtrlWnd(WM_SIG_PKTRCV, (WPARAM)(aPacket[0]), (LPARAM)aLen);

	if (aPacket[0] == 0x8f)
		NotifyCtrlWnd(WM_SIG_HEARTBEAT, (WPARAM)1, (LPARAM)0);	

	return aLen;
}


//////////////////////////////////////////////////////////////////////
// OnGameTimerTick
// -------------------------------------------------------------------
// This gets executed apx. every 1/10th of a second when in a game.
//
// You can use this to create custom timers.
//////////////////////////////////////////////////////////////////////
DWORD EXPORT OnGameTimerTick(void)
{
	//static nRemain = 0;
/*
	if (!g_bReady)
	{
		if (thisgame != NULL && thisgame->player != NULL
			&& thisgame->player->PlayerPositionX > 0 && thisgame->player->PlayerPositionY > 0)
		{
			g_bReady = TRUE;
			NotifyCtrlWnd(WM_SIG_GAMEJOIN, 
						(WPARAM)(thisgame->player->CharacterClass),
						(LPARAM)(thisgame->player->Act));
		}
	}
*/
	if (IsJustStopped())
		server->GamePrintInfo("ÿc7S.I.G. control stopped.");

	if (g_bStopped)
		return 0;

	const int nPausedResumed = IsJustPausedResumed();

	if (nPausedResumed == 1)
		server->GamePrintInfo("ÿc7S.I.G. control paused.");
	else if (nPausedResumed == 2)
		server->GamePrintInfo("ÿc7S.I.G. control resumed.");

	//if (g_bReady && !g_bPaused)
	//	g_dwInGameTime += 100;

	// is max allowed in-game time exceeded?
	/*
	const DWORD dwMaxStay = g_mem.GetLong(MEM_SIG_MAXSTAY);

	if (dwMaxStay > 0 && !g_bPaused)
	{
		if ((g_dwInGameTime / 1000) > dwMaxStay)
		{
			server->GamePrintInfo("Maximum in-game time elapsed.");
			server->GameCommandLine("saveandexit");
			g_dwInGameTime -= min(5000, g_dwInGameTime); // to avoid multiple exit message sent
		}
		else if ((g_dwInGameTime / 1000) + 20 >= dwMaxStay)
		{
			if ((dwMaxStay - (g_dwInGameTime / 1000) != nRemain)
				&& (dwMaxStay - (g_dwInGameTime / 1000)) % 5 == 0)
			{
				nRemain = dwMaxStay - (g_dwInGameTime / 1000); 
				char sz[64] = "";
				sprintf(sz, "ÿc7Game exiting in ÿc1%dÿc7 seconds.", nRemain);
				server->GamePrintInfo(sz);
			}
		}
	}
	*/

	if (g_mem.GetBool(MEM_SIG_ANTIIDLE))
	{
		g_dwAntiIdleTick += 100;
		if (g_dwAntiIdleTick > 15000) // anti-idle every 15 seconds
		{
			g_dwAntiIdleTick = 0;
			SendAntiIdle();
			NotifyCtrlWnd(WM_SIG_ANTIIDLE, (WPARAM)0, (LPARAM)0);
		}
	}

	return 0;
}

BOOL PRIVATE OnGameCommandShowD2(char** argv, int argc)
{
	if (g_bStopped)
		return TRUE;

	NotifyCtrlWnd(WM_SIG_SHOWD2, (WPARAM)0, (LPARAM)0);
	return TRUE;
}

BOOL PRIVATE OnGameCommandHideD2(char** argv, int argc)
{
	if (g_bStopped)
		return TRUE;

	NotifyCtrlWnd(WM_SIG_HIDED2, (WPARAM)0, (LPARAM)0);
	return TRUE;
}

BOOL PRIVATE OnGameCommandResume(char** argv, int argc)
{
	if (g_bStopped)
		return TRUE;

	g_bPaused = FALSE;
	g_mem.SetLong(0, MEM_SIG_PAUSE);
	NotifyCtrlWnd(WM_SIG_RESUME, (WPARAM)0, (LPARAM)0);
	server->GamePrintInfo("ÿc7S.I.G. control resumed.");
	return TRUE;
}

BOOL PRIVATE OnGameCommandPause(char** argv, int argc)
{
	if (g_bStopped)
		return TRUE;

	g_bPaused = TRUE;
	g_mem.SetLong(FLAG_THISAPP, MEM_SIG_PAUSE);
	NotifyCtrlWnd(WM_SIG_PAUSE, (WPARAM)0, (LPARAM)0);
	server->GamePrintInfo("ÿc7S.I.G. control paused.");
	return TRUE;
}

BOOL PRIVATE OnGameCommandLeave(char** argv, int argc)
{
	server->GameCommandLine("saveandexit");
	return TRUE;
}

BOOL PRIVATE OnGameCommandQuitD2(char** argv, int argc)
{
	if (!NotifyCtrlWnd(WM_SIG_QUITD2, (WPARAM)0, (LPARAM)0))
		::ExitProcess(0);
	
	return TRUE;
}

BOOL PRIVATE OnGameCommandStop(char** argv, int argc)
{
	g_bStopped = TRUE;
	g_mem.SetLong(FLAG_THISAPP, MEM_SIG_STOP);
	NotifyCtrlWnd(WM_SIG_STOP, (WPARAM)0, (LPARAM)0);
	server->GamePrintInfo("ÿc7S.I.G. control stopped.");
	return TRUE;
}

BOOL PRIVATE OnGameCommandKey(char** argv, int argc)
{
	if (argc < 3)
	{
		server->GamePrintError("Syntax: sig key <key-code>");
	}
	else
	{
		BYTE iKeyCode = (BYTE)strtoul(argv[2], NULL, 16);
		//::PostMessage(server->FindMyD2WndHandle(), WM_KEYDOWN, (WPARAM)iKeyCode, (LPARAM)0);
		NotifyCtrlWnd(WM_SIG_KEY, (WPARAM)iKeyCode, (LPARAM)0);
	}	
	
	return TRUE;
}

BOOL PRIVATE OnGameCommandNextGame(char** argv, int argc)
{
	if (argc < 3)
	{
		server->GamePrintError("Syntax: sig nextgame <create/join>");
	}
	else
	{
		if (stricmp(argv[2], "create") == 0)
		{
			g_mem.SetLong(0, MEM_SIG_NEXTGAME);
			server->GamePrintInfo("ÿc7Next game: ÿc2Create");
		}
		else if (stricmp(argv[2], "join") == 0)
		{
			g_mem.SetLong(1, MEM_SIG_NEXTGAME);
			server->GamePrintInfo("ÿc7Next game: ÿc2Join");
		}
		else
		{
			g_mem.SetLong(0, MEM_SIG_NEXTGAME);
			g_mem.SetString("", MEM_SIG_NEXTNAME);
			server->GamePrintInfo("ÿc7Next game: reset to ÿc2Auto");
		}
	}	
	
	return TRUE;
}

BOOL PRIVATE OnGameCommandNextName(char** argv, int argc)
{
	char sz[256] = "";
	for (int i = 2; i < argc; i++)
	{
		sprintf(sz, "%s%s ", sz, argv[i]);
	}
	g_mem.SetString(sz, MEM_SIG_NEXTNAME);

	if (strlen(sz) > 0)
	{
		server->GamePrintInfo("ÿc7Next game name specified.");
	}
	else
	{
		server->GamePrintInfo("ÿc7Next game name null, next game type reset to ÿc2Auto.");
	}
	
	return TRUE;
}

BOOL PRIVATE OnGameCommandNextPassword(char** argv, int argc)
{
	char sz[256] = "";
	for (int i = 2; i < argc; i++)
	{
		sprintf(sz, "%s%s ", sz, argv[i]);
	}
	g_mem.SetString(sz, MEM_SIG_NEXTPWD);
	if (strlen(sz) > 0)
	{
		server->GamePrintInfo("ÿc7Next game password specified.");
	}
	else
	{
		server->GamePrintInfo("ÿc7Next game password null(this is OK).");
	}
	return TRUE;
}

BOOL PRIVATE OnGameCommandPostString(char** argv, int argc)
{
	int i, nLen = 0;
	for (i = 2; i < argc; i++)
		nLen += (strlen(argv[i]) + 1); // including the white spaces

	if (nLen > 0)
	{
		// there are some contents...
		char* psz = new char[nLen + 1];
		memset(psz, NULL, nLen + 1);

		for (i = 2; i < argc; i++)
			sprintf(psz, "%s%s ", psz, argv[i]);

		 // remove the tail space
		psz[nLen - 1] = '\0';
		nLen--;

		// length verify
		if (nLen > MAX_MSGSTR)
			psz[MAX_MSGSTR] = '\0'; // exceed chars truncated, sorry

		PostString(psz);
		delete [] psz;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// stuff below here is from ClientCore.cpp
//////////////////////////////////////////////////////////////////////
// Dll entry/exit
//////////////////////////////////////////////////////////////////////
BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	BOOL hResult = TRUE;
	char *t;
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:

			// Create server struct
			server = new FUNCTIONENTRYPOINTS;

			// Bind exported functions from server
			HMODULE hModule;
			hModule = (HMODULE)GetModuleHandle("D2HackIt");

			// Macros make this look a lot nicer :)
			BIND_TO_SERVER(GamePrintInfo);
			BIND_TO_SERVER(GamePrintError);
			BIND_TO_SERVER(GamePrintVerbose);
			BIND_TO_SERVER(GamePrintString);
			BIND_TO_SERVER(GameCommandLine);
			BIND_TO_SERVER(GameSendPacketToServer);
			BIND_TO_SERVER(GameSendPacketToGame);
			BIND_TO_SERVER(GetFingerprint);
			BIND_TO_SERVER(Intercept);
			BIND_TO_SERVER(GetHackProfileString);
			BIND_TO_SERVER(SetHackProfileString);
			BIND_TO_SERVER(GetThisgameStruct);

			// Get plugin path
			t = ConfigPath;
			if (GetModuleFileName((HINSTANCE)hModule, t, _MAX_PATH)) {
				int p=strlen(ConfigPath);
				while (p) {
						if (ConfigPath[p] == '\\')
							{ ConfigPath[p] = 0; p=0;}
						else
					p--;
				}
			}

			// initiate client
			// hResult = OnClientStart();
			break;

		case DLL_PROCESS_DETACH:
			// kill client

			// hResult = OnClientStop();

			delete server;
			break;
    }
    return hResult;
}

//////////////////////////////////////////////////////////////////////
// Stubfunctions for 'property get' functions.
//////////////////////////////////////////////////////////////////////
LPCSTR	EXPORT GetModuleAuthor()		{return ModuleAuthor;}
LPCSTR	EXPORT GetModuleWebsite()		{return ModuleWebsite;}
DWORD	EXPORT GetModuleVersion()		{return ModuleVersion;}
LPCSTR	EXPORT GetModuleEmail()			{return ModuleEmail;}
LPCSTR	EXPORT GetModuleDescription()	{return ModuleDescription;}

//////////////////////////////////////////////////////////////////////
// OnClientCommandLine
// -------------------------------------------------------------------
// The modules own extension of the command line interface. Any custom
// commands you add are parsed here.
//
// Return value should be TRUE, but it is not used at this
// time.
//
// Arguments when we get here:
// argv[0]			Name of module
// argv[1]			Name of command (If supplied)
// argv[2 ... n]	The rest
//
// Syntax in the game: .<module> <arguments>
//////////////////////////////////////////////////////////////////////
BOOL EXPORT OnGameCommandLine(char* argv[], int argc)
{
	// Check if user supplied anything at all, if not assume help...
	if (argc==1)
		argv[argc++]="help";


	MODULECOMMANDSTRUCT* mcs=ModuleCommands;

	while (mcs->szName) {
		if (!stricmp(mcs->szName, argv[1]))
			break;
		mcs++;
	}

	char *p,*t,*msg,*fMsg;
	fMsg=new char[256];
	//
	// Is this a built-in function ?
	if (mcs->szName) {
		//
		// If functions returns false, show usage help
		if (!mcs->pFunc(argv, argc)) {
			t=new char[strlen(mcs->szUsage)+1];
			server->GamePrintInfo("Usage:");
			sprintf((char*)t, "%s", mcs->szUsage);
			if (strlen((char*)t))
			{
				msg=p=t;
				while (*p != 0) {
					if (*p == '\n')
					{
						*(p++) = 0;
						sprintf(fMsg, "ÿc4.%s %s", argv[0], msg);
						server->GamePrintInfo((char*)fMsg);
					if (*p != 0)
						msg = p;
					} else
						p++;
					}
				sprintf(fMsg, "ÿc4.%s %s", argv[0], msg);
				server->GamePrintInfo((char*)fMsg);
			}
			delete t;
		}
	}
	else {
	// Unknown command, show catch-all help phraze.
	t=new char[128];
	sprintf(t, "Unknown command ÿc4'%s %s'ÿc0 - try ÿc4'.%s help'ÿc0 to get help.",
		argv[0], argv[1], argv[0]);
	server->GamePrintError(t);
	delete t;
	}
	delete fMsg;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// OnGameCommandHelp
// -------------------------------------------------------------------
// Our default help function.
//
// Syntax in the game: .<module> <arguments>
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE OnGameCommandHelp(char** argv, int argc)
{
	// If command line is longer than 2, show usage for 'help'
	if (argc>2) return FALSE;

	char t[1024];
	sprintf(t, "Available commands for %s:", argv[0]);
	server->GamePrintInfo(t);

	// Loop through ModuleCommands[] and print their names
	for (int i=0; ModuleCommands[i].szName != NULL; i++)
	{
		sprintf(t, "ÿc4.%s %s", argv[0], ModuleCommands[i].szName);
		server->GamePrintInfo(t);
	}

	sprintf(t, "For help on a specific command, type ÿc4.%s <command> help", argv[0]);
	server->GamePrintInfo(t);
	return TRUE;
}

