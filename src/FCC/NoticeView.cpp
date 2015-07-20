// NoticeView.cpp : implementation file
//

#include "stdafx.h"
#include "FCC.h"
#include "NoticeView.h"
#include "FCCDoc.h"

// CNoticeView

IMPLEMENT_DYNCREATE(CNoticeView, CScrollView)

CNoticeView::CNoticeView()
{
	messageBitmap.LoadBitmapA(IDB_MESSAGE);
	runningBitmap.LoadBitmapA(IDB_RUNNING);
	warningBitmap.LoadBitmapA(IDB_WARNING);
	titleBrush.CreateSolidBrush(RGB(150,200,250));
	titlePen.CreatePen(PS_SOLID,1,RGB(213,233,253));
	titleFont.CreateFont(
   18,                        // nHeight
   0,                         // nWidth
   0,                         // nEscapement
   0,                         // nOrientation
   FW_BOLD,                 // nWeight
   FALSE,                     // bItalic
   FALSE,                     // bUnderline
   0,                         // cStrikeOut
   ANSI_CHARSET,              // nCharSet
   OUT_DEFAULT_PRECIS,        // nOutPrecision
   CLIP_DEFAULT_PRECIS,       // nClipPrecision
   DEFAULT_QUALITY,           // nQuality
   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
   "Times New Roman");                 // lpszFacename
	projectBrush.CreateSolidBrush(RGB(190,210,255));
	messageBrush.CreateSolidBrush(RGB(204,229,255));
	projectFont.CreateFontA(15,0,0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,"Times New Roman");

}

CNoticeView::~CNoticeView()
{
	titlePen.DeleteObject();
	titleBrush.DeleteObject();
	titleFont.DeleteObject();
	projectBrush.DeleteObject();
	messageBrush.DeleteObject();
	projectFont.DeleteObject();
	messageBitmap.DeleteObject();
	runningBitmap.DeleteObject();
}


BEGIN_MESSAGE_MAP(CNoticeView, CScrollView)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CNoticeView drawing

void CNoticeView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 0;
	SetScrollSizes(MM_TEXT, sizeTotal);

}

void CNoticeView::OnDraw(CDC* pDC)
{
	CFCCDoc* pDoc =(CFCCDoc*) GetDocument();
	CBrush* pOldBrush;
	CPen* pOldPen;
	CFont* pOldFont;
	CRect clientRect;
	CString tmpstr;
	int width,maxwidth=0;CSize textSize;
	pDC->SetBkMode(TRANSPARENT);
	GetClientRect(clientRect);
	//Painting of the title region
	pOldPen=pDC->SelectObject(&titlePen);
	pOldBrush=pDC->SelectObject(&titleBrush);
	pDC->Rectangle(-1,-1,clientRect.right-clientRect.left+1000,30);
	pDC->SetTextColor(RGB(0,0,150));
	pOldFont=pDC->SelectObject(&titleFont);
	pDC->TextOutA(20,5,"Message Board");
	//Painting of the project information
	pDC->SelectObject(&projectBrush);
	pDC->Rectangle(-1,30,clientRect.right-clientRect.left+1000,100);
	pDC->SelectObject(&projectFont);
	pDC->SetTextColor(RGB(0,0,96));
	pDC->TextOutA(10,35,"Project Summary");
	width=20;
	pDC->SetTextColor(RGB(0,0,158));pDC->TextOutA(width,50," Project Title: ");textSize=pDC->GetTextExtent("Project Title: ");width+=textSize.cx+10;
	pDC->SetTextColor(RGB(0,0,96));pDC->TextOutA(width,50,pDoc->strProjectTitle);textSize=pDC->GetTextExtent(pDoc->strProjectTitle);width+=textSize.cx+20;
	pDC->SetTextColor(RGB(0,0,158));pDC->TextOutA(width,50," Name of Positive Training Set: ");textSize=pDC->GetTextExtent("Name of Positive Training Set: ");width+=textSize.cx+10;
	pDC->SetTextColor(RGB(0,0,96));pDC->TextOutA(width,50,pDoc->strPositiveSet);textSize=pDC->GetTextExtent(pDoc->strPositiveSet);width+=textSize.cx+20;
	pDC->SetTextColor(RGB(0,0,158));pDC->TextOutA(width,50," Name of Negative Training Set: ");textSize=pDC->GetTextExtent("Name of Negative Training Set: ");width+=textSize.cx+10;
	pDC->SetTextColor(RGB(0,0,96));pDC->TextOutA(width,50,pDoc->strNegativeSet);textSize=pDC->GetTextExtent(pDoc->strNegativeSet);width+=textSize.cx+20;
	if(width>maxwidth)
		maxwidth=width;
	width=20;
	tmpstr.Format("%d",pDoc->nFpLen);
	pDC->SetTextColor(RGB(0,0,158));pDC->TextOutA(width,65," Fingerprint Length: ");textSize=pDC->GetTextExtent("Fingerprint Length: ");width+=textSize.cx+10;
	pDC->SetTextColor(RGB(0,0,96));pDC->TextOutA(width,65,tmpstr);textSize=pDC->GetTextExtent(tmpstr);width+=textSize.cx+20;
	tmpstr.Format("%d",pDoc->list_Classifier.GetCount());
	pDC->SetTextColor(RGB(0,0,158));pDC->TextOutA(width,65," Number of Weak Classifiers: ");textSize=pDC->GetTextExtent(" Number of Weak Classifiers: ");width+=textSize.cx+10;
	pDC->SetTextColor(RGB(0,0,96));pDC->TextOutA(width,65,tmpstr);textSize=pDC->GetTextExtent(tmpstr);width+=textSize.cx+20;
	if(width>maxwidth)
		maxwidth=width;
	width=20;
	tmpstr.Format(" Size of %s:",pDoc->strPositiveSet);
	pDC->SetTextColor(RGB(0,0,158));pDC->TextOutA(width,80,tmpstr);textSize=pDC->GetTextExtent(tmpstr);width+=textSize.cx+10;
	tmpstr.Format("%d",pDoc->list_CompoundPositive.GetCount());
	pDC->SetTextColor(RGB(0,0,96));pDC->TextOutA(width,80,tmpstr);textSize=pDC->GetTextExtent(tmpstr);width+=textSize.cx+20;
	tmpstr.Format(" Size of %s:",pDoc->strNegativeSet);
	pDC->SetTextColor(RGB(0,0,158));pDC->TextOutA(width,80,tmpstr);textSize=pDC->GetTextExtent(tmpstr);width+=textSize.cx+10;
	tmpstr.Format("%d",pDoc->list_CompoundNegative.GetCount());
	pDC->SetTextColor(RGB(0,0,96));pDC->TextOutA(width,80,tmpstr);textSize=pDC->GetTextExtent(tmpstr);width+=textSize.cx+20;
	tmpstr.Format(" Size of Testing Set");
	pDC->SetTextColor(RGB(0,0,158));pDC->TextOutA(width,80,tmpstr);textSize=pDC->GetTextExtent(tmpstr);width+=textSize.cx+10;
	tmpstr.Format("%d",pDoc->list_CompoundTest.GetCount());
	pDC->SetTextColor(RGB(0,0,96));pDC->TextOutA(width,80,tmpstr);textSize=pDC->GetTextExtent(tmpstr);width+=textSize.cx+20;
	if(width>maxwidth)
		maxwidth=width;
	//Painting of the message block
	pDC->SelectObject(&messageBrush);
	CDC memDC;
	CBitmap* pOldBitmap;
	memDC.CreateCompatibleDC(pDC);
	pDC->Rectangle(-1,101,clientRect.right-clientRect.left+1000,133);
	if(pDoc->nMsgCode==1)
		pOldBitmap=memDC.SelectObject(&messageBitmap);
	if(pDoc->nMsgCode==2)
		pOldBitmap=memDC.SelectObject(&runningBitmap);
	if(pDoc->nMsgCode==3)
		pOldBitmap=memDC.SelectObject(&warningBitmap);
	pDC->BitBlt(0,101,32,32,&memDC,0,0,SRCCOPY);
	pDC->TextOutA(35,105,pDoc->strMessage);textSize=pDC->GetTextExtent(pDoc->strMessage);width=35+textSize.cx;
	if(width>maxwidth)
		maxwidth=width;

	memDC.SelectObject(pOldBitmap);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldFont);

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx=maxwidth;
	sizeTotal.cy=130;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CNoticeView diagnostics

#ifdef _DEBUG
void CNoticeView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CNoticeView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CNoticeView message handlers

BOOL CNoticeView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
	// TODO: Add your specialized code here and/or call the base class
	Invalidate();
	return CScrollView::OnScroll(nScrollCode, nPos, bDoScroll);
}

BOOL CNoticeView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	//return true;
	return CScrollView::OnEraseBkgnd(pDC);
}
