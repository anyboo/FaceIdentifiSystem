#if !defined(AFX_CLRBUTTON_H__3611A4FC_CBDB_11D5_A183_B13329B34330__INCLUDED_)
#define AFX_CLRBUTTON_H__3611A4FC_CBDB_11D5_A183_B13329B34330__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClrButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CClrButton window

class CClrButton : public CButton
{
// Construction
public:
	CClrButton();

// Attributes
public:

// Operations
public:
	// UK version
	void SetColour(COLORREF text_colour, 
				   COLORREF background_colour);

	// US version
	void SetColor(COLORREF text_colour, 
				  COLORREF background_colour)
	{
		SetColour(text_colour, 
				  background_colour);		
	};

	// UK version
	void SetColour(COLORREF text_colour, 
				   COLORREF background_colour, 
				   COLORREF disabled_background_colour);

	// US version
	void SetColor(COLORREF text_colour, 
				  COLORREF background_colour, 
				  COLORREF disabled_background_colour)
	{
		SetColour(text_colour, 
				  background_colour, 
				  disabled_background_colour);
	};

	void ResetColour();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClrButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CClrButton();

private:
	COLORREF text_colour, background_colour, disabled_background_colour;
	COLORREF light, highlight, shadow, dark_shadow;

	// Generated message map functions
protected:
	//{{AFX_MSG(CClrButton)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	void DrawFrame(CDC *dc, CRect r, int state);
	void DrawFilledRect(CDC *dc, CRect r, COLORREF colour);
	void DrawLine(CDC *dc, long sx, long sy, long ex, long ey, COLORREF colour);
	void DrawButtonText(CDC *dc, CRect r, const char *buf, COLORREF text_colour);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLRBUTTON_H__3611A4FC_CBDB_11D5_A183_B13329B34330__INCLUDED_)
