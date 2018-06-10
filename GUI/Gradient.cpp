//////////////////////////////////////////////////////////////////////
//	Gradient.cpp
//
// A class that helps maintaining gradient color calculation and
// rectangle filling using gradient colors.
//
// Written by Abin (abinn32 @yahoo.com)
//
// History:
//
// May 20th, 2004 - Initial release.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Gradient.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGradient::CGradient()
{
	m_pColors = NULL;
	Cleanup();
}

CGradient::CGradient(LPCGRADIENTDATA lpData)
{
	m_pColors = NULL;
	Cleanup();
	SetData(lpData);
}

CGradient::~CGradient()
{
	if (m_pColors != NULL)
		delete [] m_pColors;
}

BOOL CGradient::IsValid() const
{
	return m_data.wCount != 0 && m_pColors != NULL;
}

BOOL CGradient::SetData(LPCGRADIENTDATA lpData)
{
	if (lpData == NULL || lpData->wCount == 0)
		return FALSE;

	if (::memcmp(&m_data, lpData, sizeof(GRADIENTDATA)) == 0)
		return TRUE;

	::memcpy(&m_data, lpData, sizeof(GRADIENTDATA));
	return __CalculateGradientColors(); // data changed, regenerate colors
}

BOOL CGradient::__CalculateGradientColors()
{
	if (m_data.wCount == 0)
		return FALSE;

	if (m_pColors != NULL)
	{
		delete [] m_pColors;
		m_pColors = NULL;
	}

	m_pColors = new COLORREF[m_data.wCount];
	if (m_pColors == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (m_data.startColor == m_data.endColor)
	{
		// not gradient
		for (WORD i = 0; i < m_data.wCount; i++)
			m_pColors[i] = m_data.startColor;
	}
	else
	{
		const double R_SEG = ((double)GetRValue(m_data.endColor) - (double)GetRValue(m_data.startColor)) / (double)m_data.wCount;
		const double G_SEG = ((double)GetGValue(m_data.endColor) - (double)GetGValue(m_data.startColor)) / (double)m_data.wCount;
		const double B_SEG = ((double)GetBValue(m_data.endColor) - (double)GetBValue(m_data.startColor)) / (double)m_data.wCount;

		double fRVal = (double)GetRValue(m_data.startColor);
		double fGVal = (double)GetGValue(m_data.startColor);
		double fBVal = (double)GetBValue(m_data.startColor);	

		for (WORD i = 0; i < m_data.wCount - 1; i++)
		{
			COLORREF color = RGB((BYTE)fRVal, (BYTE)fGVal, (BYTE)fBVal);
			m_pColors[i] = RGB((BYTE)fRVal, (BYTE)fGVal, (BYTE)fBVal);
			fRVal += R_SEG;
			fGVal += G_SEG;
			fBVal += B_SEG;
		}
		m_pColors[m_data.wCount - 1] = m_data.endColor; // the end color, no matter what
	}	

	return TRUE;
}

BOOL CGradient::IsGradient() const
{
	return m_data.wCount > 1 && m_data.startColor != m_data.endColor;
}

void CGradient::GetData(LPGRADIENTDATA lpData) const
{
	if (lpData != NULL)
		::memcpy(lpData, &m_data, sizeof(GRADIENTDATA));
}

void CGradient::Cleanup()
{
	if (m_pColors != NULL)
	{
		delete [] m_pColors;
		m_pColors = NULL;
	}
	::memset(&m_data, 0, sizeof(GRADIENTDATA));
}

BOOL CGradient::GetGradientColor(int nIndex, COLORREF &color) const
{
	if (nIndex < 0 || nIndex >= (int)m_data.wCount)
		return FALSE;

	color = m_pColors[nIndex];
	return TRUE;
}

BOOL CGradient::FillGradientRect(LPRECT lpRect, CDC* pDC, int nMethod, BOOL bFullFill, BOOL bSwing) const
{
	if (!IsValid() || lpRect == NULL || pDC == NULL || pDC->GetSafeHdc() == NULL)
		return FALSE;

	// validate rect
	CRect rect(lpRect);
	rect.NormalizeRect();	
	if (rect.Width() < 1 || rect.Height() < 1)
		return TRUE;

	if (nMethod == FILL_HORIZONTAL)
		__FillRectHoriVert(rect.left, rect.top, rect.Width(), rect.Height(), pDC, TRUE, bFullFill, bSwing);
	else if (nMethod == FILL_VERTICAL)
		__FillRectHoriVert(rect.left, rect.top, rect.Width(), rect.Height(), pDC, FALSE, bFullFill, bSwing);
	else if (nMethod == FILL_RADIAL_OUTIN)
		__FillRectRadial(rect.left, rect.top, rect.Width(), rect.Height(), pDC, FALSE, bFullFill, bSwing);
	else if (nMethod == FILL_RADIAL_INOUT)
		__FillRectRadial(rect.left, rect.top, rect.Width(), rect.Height(), pDC, TRUE, bFullFill, bSwing);
	else if (nMethod == FILL_BEVEL_NWSE)
		__FillRectBevel(rect.left, rect.top, rect.Width(), rect.Height(), pDC, TRUE, bFullFill, bSwing);
	else if (nMethod == FILL_BEVEL_SWNE)
		__FillRectBevel(rect.left, rect.top, rect.Width(), rect.Height(), pDC, FALSE, bFullFill, bSwing);
	else if (nMethod == FILL_SPIN_CLOCK)
		__FillRectSpin(rect.left, rect.top, rect.Width(), rect.Height(), pDC, TRUE, bFullFill, bSwing);
	else if (nMethod == FILL_SPIN_ANTICLOCK)
		__FillRectSpin(rect.left, rect.top, rect.Width(), rect.Height(), pDC, FALSE, bFullFill, bSwing);
	else
		return FALSE; // unrecognized filling method

	return TRUE;
}

void CGradient::__FillRectHoriVert(long x, long y, long cx, long cy, CDC *pDC, BOOL bHorizontal, BOOL bFullfill, BOOL bSwing) const
{
	ASSERT(IsValid() && pDC != NULL && pDC->GetSafeHdc() != NULL);
	long lCount = bHorizontal ? cx : cy;
	if (!bFullfill)
		lCount = min(lCount, (long)m_data.wCount);
	if (lCount < 1)
		return;

	const long RIGHT = x + cx;
	const long BOTTOM = y + cy;
	
	if (!IsGradient())
	{
		if (bHorizontal)
			pDC->FillSolidRect(x, y, lCount, cy, m_pColors[0]);
		else
			pDC->FillSolidRect(x, y, cx, lCount, m_pColors[0]);
	}
	else
	{
		COLORREF colorPrev = 0;
		CPen pen;
		CPen* pOld = pDC->GetCurrentPen();

		for (long i = 0; i < lCount; i++)
		{
			// recreate pen only if this color is different than the previous color 
			COLORREF color = __GetColorWarp(i, bSwing);
			if (i == 0 || color != colorPrev)
			{
				colorPrev = color;
				pDC->SelectObject(pOld);
				pen.DeleteObject();
				pen.CreatePen(PS_SOLID, 1, color);
				pDC->SelectObject(&pen);
			}					
			
			// draw a line using this color
			if (bHorizontal)
			{
				pDC->MoveTo(x, y);
				pDC->LineTo(x, BOTTOM);
				x++;
			}
			else
			{
				pDC->MoveTo(x, y);
				pDC->LineTo(RIGHT, y);
				y++;
			}
		}
		
		pDC->SelectObject(pOld);
		pen.DeleteObject();
	}
}

COLORREF CGradient::__GetColorWarp(int nIndex, BOOL bSwing) const
{
	if (!IsValid())
		return 0;	

	if (!IsGradient())
		return m_pColors[0];

	if (nIndex < 0)
	{
		nIndex = -nIndex;
		
		if (!bSwing)
		{
			nIndex %= (int)m_data.wCount;
			nIndex = (int)m_data.wCount - nIndex;
			nIndex %= (int)m_data.wCount;
		}
	}

	if (nIndex >= (int)m_data.wCount)
	{
		if (bSwing)		
		{
			int nTimes = nIndex / ((int)m_data.wCount - 1);
			int nMod = nIndex % ((int)m_data.wCount - 1);

			if (nTimes % 2)
				nIndex = (int)m_data.wCount - nMod - 1; // warp back
			else
				nIndex = nMod; // warp forth
		}
		else
		{
			nIndex %= (int)m_data.wCount - 1;
		}
	}

	nIndex %= (int)m_data.wCount; // ensure safety
	return m_pColors[nIndex];
}

void CGradient::__FillRectBevel(long x, long y, long cx, long cy, CDC *pDC, BOOL bNWSE, BOOL bFullfill, BOOL bSwing) const
{
	ASSERT(IsValid() && pDC != NULL && pDC->GetSafeHdc() != NULL);
	
	long lCount = cx + cy;
	if (!bFullfill)
		lCount = min(lCount, (long)m_data.wCount);
	if (lCount < 1)
		return;

	const long RIGHT = x + cx;
	const long BOTTOM = y + cy;
	CPoint pt1, pt2;
	pt1.x = x;
	pt1.y = bNWSE ? y : y + cy;	
	pt2 = pt1;
	
	COLORREF colorPrev = 0;
	CPen pen;
	CPen* pOld = pDC->GetCurrentPen();

	for (long i = 0; i < lCount; i++)
	{
		// recreate pen only if this color is different than the previous color 
		COLORREF color = __GetColorWarp(i, bSwing);
		if (i == 0 || color != colorPrev)
		{
			colorPrev = color;
			pDC->SelectObject(pOld);
			pen.DeleteObject();
			pen.CreatePen(PS_SOLID, 1, color);
			pDC->SelectObject(&pen);
		}
		
		pDC->MoveTo(pt1);
		pDC->LineTo(pt2);
			
		// update the points
		if (bNWSE)
		{
			if (pt1.x < RIGHT)
				pt1.x++;
			else
				pt1.y++;

			if (pt2.y < BOTTOM)
				pt2.y++;
			else
				pt2.x++;
		}
		else
		{
			if (pt1.y > y)
				pt1.y--;
			else
				pt1.x++;

			if (pt2.x < RIGHT)
				pt2.x++;
			else
				pt2.y--;
		}
	}
		
	pDC->SelectObject(pOld);
	pen.DeleteObject();	
}

void CGradient::__FillRectRadial(long x, long y, long cx, long cy, CDC *pDC, BOOL bOutIn, BOOL bFullfill, BOOL bSwing) const
{
	ASSERT(IsValid() && pDC != NULL && pDC->GetSafeHdc() != NULL);
	long lCount = min(cx, cy);
	lCount = (lCount % 2) ? lCount / 2 + 1 : lCount / 2;
	if (!bFullfill)
		lCount = min(lCount, (long)m_data.wCount);
	if (lCount < 1)
		return;
	
	COLORREF colorPrev = 0;
	CPen pen;
	CPen* pOld = pDC->GetCurrentPen();
	long lIndex = bOutIn ? 0 : lCount - 1;
	
	for (long i = 0; i < lCount; i++)
	{
		// recreate pen only if this color is different than the previous color 
		COLORREF color = __GetColorWarp(lIndex, bSwing);
		if (i == 0 || color != colorPrev)
		{
			colorPrev = color;
			pDC->SelectObject(pOld);
			pen.DeleteObject();
			pen.CreatePen(PS_SOLID, 1, color);
			pDC->SelectObject(&pen);
		}

		pDC->Draw3dRect(x, y, cx, cy, color, color);

		if (cx > 2)
		{
			x++;
			cx -= 2;
		}

		if (cy > 2)
		{
			y++;
			cy -= 2;
		}

		if (bOutIn)
			lIndex++;
		else
			lIndex--;
	}

	pDC->SelectObject(pOld);
	pen.DeleteObject();	
}

void CGradient::__FillRectSpin(long x, long y, long cx, long cy, CDC *pDC, BOOL bClockwise, BOOL bFullfill, BOOL bSwing) const
{
	ASSERT(IsValid() && pDC != NULL && pDC->GetSafeHdc() != NULL);
	long lCount = (cx + cy) * 2;
	if (!bFullfill)
		lCount = min(lCount, (long)m_data.wCount);
	if (lCount < 1)
		return;

	const long RIGHT = x + cx;
	const long BOTTOM = y + cy;
	
	CPoint pt(x, y);
	const CPoint CENTER = CPoint(x + cx / 2, y + cy / 2);
	COLORREF colorPrev = 0;
	CPen pen;
	CPen* pOld = pDC->GetCurrentPen();

	for (long i = 0; i < lCount; i++)
	{
		COLORREF color = __GetColorWarp(i, bSwing);
		if (i == 0 || color != colorPrev)
		{
			colorPrev = color;
			pDC->SelectObject(pOld);
			pen.DeleteObject();
			pen.CreatePen(PS_SOLID, 1, color);
			pDC->SelectObject(&pen);
		}

		pDC->MoveTo(pt);
		pDC->LineTo(CENTER);

		if (bClockwise)
		{
			if (pt.x < RIGHT && pt.y == y) // top
			{
				pt.x++;
			}
			else if (pt.x == RIGHT && pt.y < BOTTOM) // right
			{
				pt.y++;
			}
			else if (pt.x > x && pt.y == BOTTOM) // bottom
			{
				pt.x--;
			}
			else if (pt.x == x && pt.y > y) // right
			{
				pt.y--;
			}
		}
		else
		{
			if (pt.x > x && pt.y == y) // top
			{
				pt.x--;
			}
			else if (pt.x == RIGHT && pt.y > y) // right
			{
				pt.y--;
			}
			else if (pt.x < RIGHT && pt.y == BOTTOM) // bottom
			{
				pt.x++;
			}
			else if (pt.x == x && pt.y < BOTTOM) // right
			{
				pt.y++;
			}
		}
	}

	pDC->SetPixel(CENTER, colorPrev); // fill the center pixel
	pDC->SelectObject(pOld);
	pen.DeleteObject();	
}

BOOL CGradient::Reverse()
{
	if (!IsValid())
		return FALSE;

	COLORREF color = m_data.startColor;
	m_data.startColor = m_data.endColor;
	m_data.endColor = color;

	for (int i = 0; i < (int)m_data.wCount / 2; i++)
	{
		color = m_pColors[i];
		m_pColors[i] = m_pColors[m_data.wCount - i - 1];
		m_pColors[m_data.wCount - i - 1] = color;
	}
	return TRUE;
}
