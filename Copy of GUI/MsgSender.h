// MsgSender.h: interface for the CMsgSender class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGSENDER_H__EC928348_5926_4675_9B53_C2ACAAC29B30__INCLUDED_)
#define AFX_MSGSENDER_H__EC928348_5926_4675_9B53_C2ACAAC29B30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

class CMsgSender  
{
public:
	void SetKeyDelay(DWORD dwKeyDelay);
	BOOL SendSysKey(int nKeyCode);
	BOOL LDblClick(int x, int y);
	BOOL SendChar(TCHAR ch);
	BOOL SendString(LPCTSTR lpString);
	BOOL RightClick(int x, int y);
	static DWORD TransPosition(int x, int y);
	BOOL LeftClick(int x, int y);
	BOOL MouseMove(int x, int y);
	HWND GetTargetHWnd() const;
	HWND SetTargetHWnd(HWND hWnd);
	CMsgSender();
	virtual ~CMsgSender();


protected:
	HWND m_hTarget;
	DWORD m_dwKeyDelay;
};

#endif // !defined(AFX_MSGSENDER_H__EC928348_5926_4675_9B53_C2ACAAC29B30__INCLUDED_)
