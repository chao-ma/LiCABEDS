// CButton.cpp : implementation file
//

#include "stdafx.h"
#include "FCC.h"
#include "CButton.h"


// CCButton

IMPLEMENT_DYNAMIC(CCButton, CButton)

CCButton::CCButton()
{

}

CCButton::~CCButton()
{
}


BEGIN_MESSAGE_MAP(CCButton, CButton)
	ON_WM_DRAWITEM()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void CCButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC dc;
	RECT rect;
	dc.Attach(lpDrawItemStruct->hDC);
	rect=lpDrawItemStruct->rcItem;
	dc.Draw3dRect(&rect,RGB(255,255,255),RGB(0,0,0)); 
    dc.FillSolidRect(&rect,color);//Here you can define the required color to appear on the Button.
    UINT state=lpDrawItemStruct->itemState;  //This defines the state of the Push button either pressed or not. 
    if((state & ODS_SELECTED))
	{
		dc.DrawEdge(&rect,EDGE_SUNKEN,BF_RECT);
	}
	else
	{
		dc.DrawEdge(&rect,EDGE_RAISED,BF_RECT);
	}

	dc.SetBkColor(color);   //Setting the Text Background color
	dc.SetTextColor(RGB(255,0,0));     //Setting the Text Color

	dc.Detach();  // Detach the Button DC        
	//AfxMessageBox("called");
	// TODO:  Add your code to draw the specified item
}

void CCButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CColorDialog dlg;
	if(dlg.DoModal()==IDOK)
	{
		color=dlg.GetColor();
		Invalidate();
		UpdateWindow();
	}
	CButton::OnLButtonUp(nFlags, point);
}
