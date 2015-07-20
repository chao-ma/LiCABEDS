#pragma once


// CCButton

class CCButton : public CButton
{
	DECLARE_DYNAMIC(CCButton)

public:
	CCButton();
	virtual ~CCButton();
	COLORREF color;
protected:
	DECLARE_MESSAGE_MAP()

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


