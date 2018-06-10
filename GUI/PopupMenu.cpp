#include "stdafx.h"

void CmdRouteMenu(CWnd* pWnd,CMenu* pPopupMenu)
{
	CCmdUI state;
	state.m_pMenu = pPopupMenu;
	state.m_pParentMenu = pPopupMenu;
	state.m_nIndexMax = pPopupMenu->GetMenuItemCount();

	for (state.m_nIndex = 0; 
	     state.m_nIndex < state.m_nIndexMax; 
	     state.m_nIndex++) 
	{
		state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);

		// menu separator or invalid cmd - ignore it
		if (state.m_nID == 0) continue; 

		if (state.m_nID == (UINT)-1)
		{
			// possibly a popup menu, route to child menu if so
			CMenu* pSub=pPopupMenu->GetSubMenu(state.m_nIndex);
			if(pSub) CmdRouteMenu(pWnd,pSub);
		}
		else 
		{
			// normal menu item, Auto disable if command is 
			// _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(pWnd, FALSE);
		}
	}
}

BOOL PopupMenu(CMenu* pPopupMenu, CWnd* pParentWnd, UINT nFlags, int x, int y, LPCRECT lpRect = NULL)
{
	CmdRouteMenu(pParentWnd, pPopupMenu);
	return pPopupMenu->TrackPopupMenu(nFlags, x, y, pParentWnd, lpRect);
}