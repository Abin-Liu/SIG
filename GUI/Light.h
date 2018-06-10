#if !defined(AFX_LIGHT_H__641F71EE_8173_4DFB_9A32_0109AD07B932__INCLUDED_)
#define AFX_LIGHT_H__641F71EE_8173_4DFB_9A32_0109AD07B932__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Light.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLight window

enum { LIGHT_GRAY = 0, LIGHT_RED, LIGHT_GREEN, LIGHT_YELLOW };
enum { LIGHT_UNKNOWN = 0, LIGHT_LC, LIGHT_HB, LIGHT_RV, LIGHT_ST, LIGHT_AI };

class CLight : public CStatic
{
// Construction
public:
	CLight();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLight)
	//}}AFX_VIRTUAL

// Implementation
public:
	void AdjustWindow(CPoint pos);
	int GetType() const;
	void SetType(int nType);
	void SetColor(int nColor);
	void Flash(UINT nTimes = 1);
	void TurnOn(BOOL bOn);
	virtual ~CLight();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLight)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	void DrawText(CDC* pDC) const;
	void DrawLight(CDC* pDC, int nColor) const;
	COLORREF m_bkColor;
	int m_nColor;
	BOOL m_bOn;
	RECT m_rcLight;
	UINT m_nFlash;
	BOOL m_bFlashOn;
	int m_nType;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIGHT_H__641F71EE_8173_4DFB_9A32_0109AD07B932__INCLUDED_)
