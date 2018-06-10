// Light.cpp : implementation file
//

#include "stdafx.h"
#include "SIG.h"
#include "Light.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COL_DARK		RGB(132, 130, 132)
#define COL_SHADOW		RGB(198, 195, 198)
#define COL_RED			RGB(255, 0, 0)
#define COL_GREEN		RGB(0, 255, 0)
#define COL_YELLOW		RGB(255, 255, 0)
#define COL_PINK		RGB(255, 0, 255)
#define COL_OLIVE		RGB(132, 130, 0)
#define COL_DARKGREEN	RGB(0, 130, 0)

#define LIGHT_FLASH_INTERVAL	100

/////////////////////////////////////////////////////////////////////////////
// CLight

CLight::CLight()
{
	m_bkColor = RGB(48, 48, 48);
	m_nColor = LIGHT_GRAY;
	m_bOn = FALSE;
	m_rcLight.left = 1;
	m_rcLight.top = 1;
	m_rcLight.right = 6;
	m_rcLight.bottom = 6;
	m_nFlash = 0;
	m_bFlashOn = FALSE;
	m_nType = LIGHT_UNKNOWN;	
}

CLight::~CLight()
{
}

BEGIN_MESSAGE_MAP(CLight, CStatic)
	//{{AFX_MSG_MAP(CLight)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLight message handlers

void CLight::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect(&rect);
	dc.FillSolidRect(&rect, m_bkColor);
	BOOL bOn = m_nFlash ? m_bFlashOn : m_bOn;
	int nColor = bOn ? m_nColor : LIGHT_GRAY;
	DrawLight(&dc, nColor);
	DrawText(&dc);
}

void CLight::DrawLight(CDC *pDC, int nColor) const
{
	if (pDC == NULL)
		return;

	COLORREF color1, color2, color3;
	switch (nColor)
	{
	case LIGHT_RED:
		color1 = COL_RED;
		color2 = COL_RED;
		color3 = COL_PINK;
		break;

	case LIGHT_GREEN:
		color1 = COL_DARKGREEN;
		color2 = COL_GREEN;
		color3 = COL_YELLOW;
		break;

	case LIGHT_YELLOW:
		color1 = COL_OLIVE;
		color2 = COL_YELLOW;
		color3 = COL_YELLOW;
		break;

	default:
		color1 = COL_DARK;
		color2 = COL_DARK;
		color3 = COL_SHADOW;
		break;
	}

	CPen pen1, pen2, pen3;
	pen1.CreatePen(PS_SOLID, 1, color1);
	pen2.CreatePen(PS_SOLID, 1, color2);
	pen3.CreatePen(PS_SOLID, 1, color3);
	CPen* pOld = pDC->SelectObject(&pen1);

	// draw 1
	pDC->MoveTo(2, 1);
	pDC->LineTo(4, 1);
	pDC->LineTo(5, 2);
	pDC->LineTo(1, 2);
	pDC->LineTo(1, 3);
	pDC->SetPixel(2, 2, RGB(255, 255, 255));

	// draw 2
	pDC->SelectObject(&pen2);
	pDC->LineTo(5, 3);
	pDC->LineTo(5, 4);
	pDC->LineTo(1, 4);
	pDC->LineTo(2, 5);

	// draw 3
	pDC->SelectObject(&pen3);
	pDC->LineTo(5, 5);

	pDC->SelectObject(pOld);
	pen1.DeleteObject();
	pen2.DeleteObject();
	pen3.DeleteObject();
}

void CLight::TurnOn(BOOL bOn)
{
	if (!!m_bOn != !!bOn)
	{
		m_bOn = bOn;
		if (::IsWindow(m_hWnd))
			RedrawWindow(&m_rcLight);
	}
}

void CLight::OnDestroy() 
{
	KillTimer(0);
	CStatic::OnDestroy();	
}

void CLight::Flash(UINT nTimes)
{
	BOOL bWasFlashing = m_nFlash;
	m_nFlash = (nTimes == -1) ? -1 : nTimes * 2;

	if (!bWasFlashing && m_nFlash)
	{
		SetTimer(0, LIGHT_FLASH_INTERVAL, NULL);
		return;
	}

	if (bWasFlashing && m_nFlash == 0)
	{
		KillTimer(0);
		if (::IsWindow(m_hWnd))
			RedrawWindow(&m_rcLight);
	}
}

void CLight::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_nFlash)
	{
		m_bFlashOn = !m_bFlashOn;
		RedrawWindow(&m_rcLight);
		if (m_nFlash != -1)
			m_nFlash--;
	}
	else
	{
		KillTimer(0);
		RedrawWindow(&m_rcLight);
	}
	CStatic::OnTimer(nIDEvent);
}

void CLight::SetColor(int nColor)
{
	if (m_nColor != nColor)
	{
		m_nColor = nColor;
		if (::IsWindow(m_hWnd) && m_nFlash == 0)
			RedrawWindow(&m_rcLight);
	}
}

void CLight::SetType(int nType)
{
	if (m_nType != nType)
	{
		m_nType = nType;
		if (::IsWindow(m_hWnd))
			RedrawWindow();
	}
}

int CLight::GetType() const
{
	return m_nType;
}

void CLight::DrawText(CDC *pDC) const
{
	if (pDC == NULL)
		return;

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(198, 195, 198));
	CPen* pOld = pDC->SelectObject(&pen);

	switch (m_nType)
	{
	case LIGHT_LC:
		pDC->MoveTo(0, 9);
		pDC->LineTo(0, 13);
		pDC->LineTo(3, 13);
		pDC->MoveTo(6, 9);
		pDC->LineTo(5, 9);
		pDC->LineTo(4, 10);
		pDC->LineTo(4, 12);
		pDC->LineTo(5, 13);
		pDC->LineTo(7, 13);
		break;
		
	case LIGHT_HB:
		pDC->MoveTo(0, 9);
		pDC->LineTo(0, 14);
		pDC->MoveTo(2, 9);
		pDC->LineTo(2, 14);
		pDC->MoveTo(0, 11);
		pDC->LineTo(2, 11);
		pDC->MoveTo(4, 9);
		pDC->LineTo(4, 13);
		pDC->LineTo(5, 13);
		pDC->LineTo(6, 12);
		pDC->LineTo(5, 11);
		pDC->LineTo(6, 10);
		pDC->LineTo(5, 9);
		pDC->LineTo(4, 9);
		break;
		
	case LIGHT_RV:
		pDC->MoveTo(0, 13);
		pDC->LineTo(0, 9);
		pDC->LineTo(2, 9);
		pDC->LineTo(2, 10);
		pDC->LineTo(1, 11);
		pDC->LineTo(2, 12);
		pDC->LineTo(2, 14);
		pDC->MoveTo(4, 9);
		pDC->LineTo(4, 11);
		pDC->LineTo(5, 12);
		pDC->LineTo(5, 14);
		pDC->MoveTo(6, 9);
		pDC->LineTo(6, 11);
		pDC->LineTo(5, 12);
		break;
		
	case LIGHT_ST:
		pDC->MoveTo(2, 9);
		pDC->LineTo(0, 9);
		pDC->LineTo(0, 11);
		pDC->LineTo(2, 11);
		pDC->LineTo(2, 13);
		pDC->LineTo(-1, 13);
		pDC->MoveTo(4, 9);
		pDC->LineTo(7, 9);
		pDC->MoveTo(5, 9);
		pDC->LineTo(5, 14);
		break;
		
	case LIGHT_AI:
		pDC->MoveTo(0, 13);
		pDC->LineTo(0, 10);
		pDC->LineTo(1, 9);
		pDC->LineTo(2, 10);
		pDC->LineTo(2, 14);
		pDC->MoveTo(1, 11);
		pDC->LineTo(2, 11);
		pDC->MoveTo(4, 9);
		pDC->LineTo(7, 9);
		pDC->MoveTo(4, 13);
		pDC->LineTo(7, 13);
		pDC->MoveTo(5, 10);
		pDC->LineTo(5, 13);		
		break;

	default:
		break;
	}
	pDC->SelectObject(pOld);
	pen.DeleteObject();
}

void CLight::AdjustWindow(CPoint pos)
{
	if (!::IsWindow(m_hWnd))
		return;

	CRect rect(pos, CSize(7, 14));
	MoveWindow(&rect);
}
