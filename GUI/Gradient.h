//////////////////////////////////////////////////////////////////////
//	Gradient.h
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

#ifndef __GRADIENG_H__
#define __GRADIENG_H__

// The gradient data struct
typedef struct tagGradientData
{
	COLORREF startColor;	// The starting color
	COLORREF endColor;		// The ending color		
	WORD wCount;			// Number of colors to be generated
} GRADIENTDATA, *LPGRADIENTDATA;
typedef const tagGradientData* LPCGRADIENTDATA;

// The gradient class definition
class CGradient  
{
public:	
	
	///////////////////////////////////////////////////////////////////////////////
	// Constructors & Destructor
	///////////////////////////////////////////////////////////////////////////////
	
	CGradient();
	CGradient(LPCGRADIENTDATA lpData);
	virtual ~CGradient();

	///////////////////////////////////////////////////////////////////////////////
	//	Gradient Filling
	///////////////////////////////////////////////////////////////////////////////

	// Filling methods definition
	enum {  FILL_HORIZONTAL = 0, // Fill horizontally, gradient from left to right
			FILL_VERTICAL, // Fill vertically, gradient from top to bottom				
			FILL_BEVEL_NWSE,  // Fill bevelling, gradient from north-west to south-east
			FILL_BEVEL_SWNE,  // Fill bevelling, gradient from south-west to north-east
			FILL_RADIAL_OUTIN, // Fill radial, gradient from outside to inside	
			FILL_RADIAL_INOUT, // Fill radial, gradient from inside to outside
			FILL_SPIN_CLOCK, // Fill spinning, clock-wise
			FILL_SPIN_ANTICLOCK  // Fill spin, anti clock-wise
	}; 

	// Filling a gradient rectangle, using the specified method
	BOOL FillGradientRect(LPRECT lpRect, CDC* pDC, int nMethod, BOOL bFullFill = TRUE, BOOL bSwing = TRUE) const;

	///////////////////////////////////////////////////////////////////////////////
	//	Gradient Data Access
	///////////////////////////////////////////////////////////////////////////////
	
	BOOL SetData(LPCGRADIENTDATA lpData); // Set gradient data and regenerate colors
	void GetData(LPGRADIENTDATA lpData) const; // Retrieve the gradient data
	BOOL Reverse(); // Reverse the starting color and the ending color, regenerate gradient colors if needed
	BOOL GetGradientColor(int nIndex, COLORREF& color) const; // Retrieve a generated gradient color
	void Cleanup(); // Delete all generated color and reset all data

	///////////////////////////////////////////////////////////////////////////////
	//	Gradient Data Access
	///////////////////////////////////////////////////////////////////////////////
	BOOL IsValid() const; // Check for wether the object is valid
	BOOL IsGradient() const; // TRUE if the starting color and the ending color are different
	
protected:
	void __FillRectSpin(long x, long y, long cx, long cy, CDC *pDC, BOOL bClockwise, BOOL bFullfill, BOOL bSwing) const;

	// Helper functions
	void __FillRectHoriVert(long x, long y, long cx, long cy, CDC* pDC, BOOL bHorizontal, BOOL bFullfill, BOOL bSwing) const;
	void __FillRectRadial(long x, long y, long cx, long cy, CDC *pDC, BOOL bOutIn, BOOL bFullfill, BOOL bSwing) const;
	void __FillRectBevel(long x, long y, long cx, long cy, CDC *pDC, BOOL bNWSE, BOOL bFullfill, BOOL bSwing) const;

	COLORREF __GetColorWarp(int nIndex, BOOL bSwing) const;
	BOOL __CalculateGradientColors(); // Regenerate gradient colors
	
	// Member data
	GRADIENTDATA m_data; // The gradient data
	COLORREF* m_pColors; // Generated gradient colors
};

#endif // __GRADIENG_H__