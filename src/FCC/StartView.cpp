// StartView.cpp : implementation file
//

#include "stdafx.h"
#include "FCC.h"
#include "StartView.h"
#include "FCCDoc.h"
#define BANNER_HEIGHT 50
#define TAG_TOP 100
#define TAG_MARGIN 50

// CStartView

IMPLEMENT_DYNCREATE(CStartView, CScrollView)

CStartView::CStartView()
{
	bShowHand=false;
	CTag* pTag;
	for(int i=1;i<=4;i++)
	{
		pTag=new CTag;
		pTag->nTagType=i;
		pTag->str_Title.LoadStringA(115+i);
		pTag->str_Info.LoadStringA(215+i);
		pTag->rect_Region.SetRect(TAG_MARGIN+(151+TAG_MARGIN)*(i-1),TAG_TOP,TAG_MARGIN+(151+TAG_MARGIN)*(i-1)+151,317);
		pTag->icon_Title=::LoadImage(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDI_PROJECT+i-1),IMAGE_ICON,150,150,0);
		list_Tag.AddTail(pTag);
	}
	icon_help=::LoadImage(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDI_HELP),IMAGE_ICON,48,48,0);
}

CStartView::~CStartView()
{
	DestroyIcon((HICON)icon_help);
	while(!list_Tag.IsEmpty())
		delete list_Tag.RemoveTail();
}


BEGIN_MESSAGE_MAP(CStartView, CScrollView)
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CStartView drawing

void CStartView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = TAG_MARGIN*5+151*4;
	sizeTotal.cy = TAG_TOP+217+20+100;
	SetScrollSizes(MM_TEXT, sizeTotal);
	str_InfoBar="Welcome to Fingerprint-based Compound Classifier!#Tutorial and context-sensitive help is available by pressing F1.#Click tags above to start or manage your project.#";

}

void CStartView::OnDraw(CDC* ppDC)
{
	CDocument* pDoc = GetDocument();
	int count=list_Tag.GetCount();
	int i;
	CPoint curpos;
	CString tmpstr;
	CDC memDC;
	CBitmap memBitmap;
	CDC* pDC=&memDC;
	memDC.CreateCompatibleDC(ppDC);
	memBitmap.CreateCompatibleBitmap(ppDC,TAG_MARGIN*(count+1)+151*count,TAG_TOP+317);
	CBitmap* pOldBitmap=pDC->SelectObject(&memBitmap);
	CRect rect;
	GetClientRect(rect);
	

	pDC->SetBkMode(TRANSPARENT);
	//Draw the Welcome Banner
	pDC->FillSolidRect(0,0,TAG_MARGIN*(count+1)+151*count,BANNER_HEIGHT,RGB(190,224,255));
	CFont font_Welcome;
	font_Welcome.CreateFontA(40,0,0,0,FW_BOLD,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,"Times New Roman");
	CFont* pOldFont=pDC->SelectObject(&font_Welcome);
	pDC->SetTextColor(RGB(255,255,255));
	pDC->TextOutA(20,5,"WELCOME");
	pDC->SetTextColor(RGB(0,0,55));
	pDC->SelectObject(pOldFont);
	font_Welcome.DeleteObject();

	pDC->FillSolidRect(0,BANNER_HEIGHT,TAG_MARGIN*(count+1)+151*count,TAG_TOP-BANNER_HEIGHT,RGB(255,255,255));
	//Draw the option tags by calling CTag member function
	POSITION pos;
	CTag* pTag;
	pos=list_Tag.GetHeadPosition();
	while(pos)
	{
		pTag=(CTag*)list_Tag.GetNext(pos);
		pTag->DrawTag(pDC);
	}
	for(int i=0;i<=count;i++)
		pDC->FillSolidRect(i*(TAG_MARGIN+151),TAG_TOP,TAG_MARGIN,217,RGB(255,255,255));
	pDC->FillSolidRect(0,TAG_TOP+217,TAG_MARGIN*(count+1)+151*count,20,RGB(255,255,255));
	//Draw the infomation bar and output related text
	pDC->FillSolidRect(0,TAG_TOP+217+20,TAG_MARGIN*(count+1)+151*count,80,RGB(215,215,255));
	i=0;curpos.x=80;curpos.y=TAG_TOP+217+30;
	::DrawIconEx(pDC->GetSafeHdc(),16,TAG_TOP+217+36,(HICON)icon_help,48,48,NULL,NULL,DI_NORMAL);
	font_Welcome.CreateFontA(13,0,0,0,FW_BOLD,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,"Arial");
	pOldFont=pDC->SelectObject(&font_Welcome);
	while(i>=0)
	{
		tmpstr=str_InfoBar.Mid(i,str_InfoBar.Find('#',i)-i);
		pDC->TextOutA(curpos.x,curpos.y,tmpstr);
		curpos.y+=20;
		i=str_InfoBar.Find('#',i);
		if(i>=0)
			i++;
	}
	pDC->SelectObject(pOldFont);
	font_Welcome.DeleteObject();
	//Position the buffer bitmap at the center and calculate the offset set
	if(rect.Width()<=TAG_MARGIN*(count+1)+151*count && rect.Height()<=TAG_TOP+317)
	{
		ppDC->BitBlt(0,0,TAG_MARGIN*(count+1)+151*count,TAG_TOP+317,pDC,0,0,SRCCOPY);
		rect_InfoBar.SetRect(TAG_MARGIN,TAG_TOP+217+20,TAG_MARGIN*(count+1)+151*count,TAG_TOP+317);
		margin_offset.cx=0;
		margin_offset.cy=0;
	}
	if(rect.Width()>TAG_MARGIN*(count+1)+151*count && rect.Height()<=TAG_TOP+317)
	{
		int margin=(rect.Width()-(TAG_MARGIN*(count+1)+151*count))/2;
		ppDC->BitBlt(margin,0,TAG_MARGIN*(count+1)+151*count,TAG_TOP+317,pDC,0,0,SRCCOPY);
		ppDC->FillSolidRect(0,0,margin,rect.Height(),RGB(255,255,255));
		ppDC->FillSolidRect(rect.Width()-margin-1,0,margin+1,rect.Height(),RGB(255,255,255));
		rect_InfoBar.SetRect(margin+TAG_MARGIN,TAG_TOP+217+20,margin+TAG_MARGIN*(count+1)+151*count,TAG_TOP+317);
		margin_offset.cx=margin;
		margin_offset.cy=0;

	}
	if(rect.Width()<=TAG_MARGIN*(count+1)+151*count && rect.Height()>TAG_TOP+317)
	{
		int margin=(rect.Height()-(TAG_TOP+317))/2;
		ppDC->BitBlt(0,margin,TAG_MARGIN*(count+1)+151*count,TAG_TOP+317,pDC,0,0,SRCCOPY);
		ppDC->FillSolidRect(0,0,rect.Width(),margin,RGB(255,255,255));
		ppDC->FillSolidRect(0,margin+TAG_TOP+317-1,rect.Width(),margin+1,RGB(255,255,255));
		rect_InfoBar.SetRect(TAG_MARGIN,margin+TAG_TOP+217+20,TAG_MARGIN*(count+1)+151*count,margin+TAG_TOP+317);
		margin_offset.cx=0;
		margin_offset.cy=margin;
	}
	if(rect.Width()>=TAG_MARGIN*(count+1)+151*count && rect.Height()>=TAG_TOP+317)
	{
		int topmargin=(rect.Height()-(TAG_TOP+317))/2;
		int leftmargin=(rect.Width()-(TAG_MARGIN*(count+1)+151*count))/2;
		ppDC->BitBlt(leftmargin,topmargin,TAG_MARGIN*(count+1)+151*count,TAG_TOP+317,pDC,0,0,SRCCOPY);
		ppDC->FillSolidRect(0,0,rect.Width(),topmargin,RGB(255,255,255));
		ppDC->FillSolidRect(0,0,leftmargin,rect.Width(),RGB(255,255,255));
		ppDC->FillSolidRect(0,rect.Height()-topmargin-1,rect.Width(),topmargin+1,RGB(255,255,255));
		ppDC->FillSolidRect(rect.Width()-leftmargin-1,0,leftmargin+1,rect.Height(),RGB(255,255,255));
		rect_InfoBar.SetRect(leftmargin+TAG_MARGIN,topmargin+TAG_TOP+217+20,leftmargin+TAG_MARGIN*(count+1)+151*count,topmargin+TAG_TOP+317);
		margin_offset.cx=leftmargin;
		margin_offset.cy=topmargin;
	}
	
	

	pDC->SelectObject(pOldBitmap);
	pDC->DeleteDC();


}

BOOL CStartView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return true;
	//return CScrollView::OnEraseBkgnd(pDC);
}

void CStartView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	OnPrepareDC(&dc);
	CPoint m_point=point;
	dc.DPtoLP(&m_point);
	CRgn rgn;
	bShowHand=false;
	POSITION pos=list_Tag.GetHeadPosition();
	while(pos)
	{
		CTag* pTag=(CTag*)list_Tag.GetNext(pos);
		rgn.CreateRectRgn(pTag->rect_Region.left+margin_offset.cx,pTag->rect_Region.top+margin_offset.cy,
			pTag->rect_Region.right+margin_offset.cx,pTag->rect_Region.bottom+margin_offset.cy);
		if(rgn.PtInRegion(m_point))
		{
			bShowHand=true;
			if(pTag->bSelected==false)
			{
				pTag->bSelected=true;
				pTag->GetHelpString(str_InfoBar);
				InvalidateRect(rect_InfoBar,0);
			}
		}
		else
			pTag->bSelected=false;
		rgn.DeleteObject();
	}
	rgn.CreateRectRgnIndirect(rect_InfoBar);
	if(rgn.PtInRegion(m_point))
	{
		bShowHand=true;
	}
	rgn.DeleteObject();
	CScrollView::OnMouseMove(nFlags, point);
}

void CStartView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	OnPrepareDC(&dc);
	CPoint m_point=point;
	dc.DPtoLP(&m_point);
	CRgn rgn;
	POSITION pos=list_Tag.GetHeadPosition();
	CTag* pTargetTag=NULL;
	while(pos)
	{
		CTag* pTag=(CTag*)list_Tag.GetNext(pos);
		rgn.CreateRectRgn(pTag->rect_Region.left+margin_offset.cx,pTag->rect_Region.top+margin_offset.cy,
			pTag->rect_Region.right+margin_offset.cx,pTag->rect_Region.bottom+margin_offset.cy);
		if(rgn.PtInRegion(m_point))
		{
			pTargetTag=pTag;
			break;
		}
		rgn.DeleteObject();
	}
	rgn.DeleteObject();
	rgn.CreateRectRgnIndirect(rect_InfoBar);
	if(rgn.PtInRegion(m_point))
	{
		AfxMessageBox("help. to be finished.");
	}
	rgn.DeleteObject();

	CScrollView::OnLButtonDown(nFlags, point);
	if(pTargetTag)
		pTargetTag->ExecuteCommand();
}

BOOL CStartView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default

	//return CScrollView::OnSetCursor(pWnd, nHitTest, message);
	if(bShowHand)
		::SetCursor(::LoadCursor(NULL,IDC_HAND));
	else
		::SetCursor(::LoadCursor(NULL,IDC_ARROW));
	
	return true;
}

void CStartView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: Add your specialized code here and/or call the base class
	POSITION pos=list_Tag.GetHeadPosition();
	CTag* pTag;
	CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
	while(pos)
	{
		pTag=(CTag*)list_Tag.GetNext(pos);
		pTag->CheckStatus(pDoc);
	}
	Invalidate(0);
}


// CStartView diagnostics

#ifdef _DEBUG
void CStartView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CStartView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CStartView message handlers

//CTag Implementation

CTag::CTag()
{
	bitmap_green.LoadBitmapA(IDB_GREENTAG);
	bitmap_yellow.LoadBitmapA(IDB_YELLOWTAG);
	bitmap_red.LoadBitmapA(IDB_REDTAG);
	bSelected=false;
	nStatus=3;
}

CTag::~CTag()
{
	bitmap_green.DeleteObject();
	bitmap_yellow.DeleteObject();
	bitmap_red.DeleteObject();
	DestroyIcon((HICON)icon_Title);
}

void CTag::DrawTag(CDC* pDC)
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap;
	if(nStatus==1)
		pOldBitmap=memDC.SelectObject(&bitmap_green);
	if(nStatus==2)
		pOldBitmap=memDC.SelectObject(&bitmap_yellow);
	if(nStatus==3)
		pOldBitmap=memDC.SelectObject(&bitmap_red);
	pDC->BitBlt(rect_Region.left,rect_Region.top,151,217,&memDC,0,0,SRCCOPY);
	::DrawIconEx(pDC->GetSafeHdc(),rect_Region.left+(rect_Region.Width()-80)/2,rect_Region.top-40,(HICON)icon_Title,80,80,NULL,NULL,DI_NORMAL);
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	pDC->TextOutA(rect_Region.left+(rect_Region.Width()-pDC->GetTextExtent(str_Title).cx)/2,rect_Region.top+40+5,str_Title);
	if(nStatus==1)
		pDC->TextOutA(rect_Region.left+(rect_Region.Width()-pDC->GetTextExtent("Complete").cx)/2,rect_Region.top+40+35,"Complete");
	if(nStatus==2)
		pDC->TextOutA(rect_Region.left+(rect_Region.Width()-pDC->GetTextExtent("Partially Complete").cx)/2,rect_Region.top+40+35,"Partially Complete");
	if(nStatus==3)
		pDC->TextOutA(rect_Region.left+(rect_Region.Width()-pDC->GetTextExtent("Incomplete").cx)/2,rect_Region.top+40+35,"Incomplete");
	CPoint curpos(rect_Region.left+10,rect_Region.top+40+35+35);
	char tmpchar;
	for(int i=0;i<str_Info.GetLength();i++)
	{
		tmpchar=str_Info[i];
		if(curpos.x>rect_Region.right-10 || tmpchar=='#')
		{
			curpos.x=rect_Region.left+10;
			curpos.y+=pDC->GetTextExtent(&tmpchar,1).cy;
		}
		if(tmpchar=='#')
			continue;
		pDC->TextOutA(curpos.x,curpos.y,&tmpchar,1);
		curpos.x+=pDC->GetTextExtent(&tmpchar,1).cx;
		
	}
}

int CTag::CheckStatus(CFCCDoc* pDoc)
{
	if(nTagType==1)
	{
		nStatus=1;
		if(pDoc->strProjectTitle=="Untitled Project" || pDoc->strPositiveSet=="Positive Dataset" || pDoc->strNegativeSet=="Negative Dataset")
			nStatus=2;
		if(pDoc->strProjectTitle=="Untitled Project" && pDoc->strPositiveSet=="Positive Dataset" && pDoc->strNegativeSet=="Negative Dataset")
			nStatus=3;
		if(pDoc->strProjectTitle=="" || pDoc->strPositiveSet=="" || pDoc->strNegativeSet=="")
			nStatus=2;
		if(pDoc->strProjectTitle=="" && pDoc->strPositiveSet=="" && pDoc->strNegativeSet=="")
			nStatus=3;

	}
	if(nTagType==2)
	{
		nStatus=1;
		if(pDoc->list_CompoundPositive.GetCount()==0 || pDoc->list_CompoundNegative.GetCount()==0 || pDoc->list_CompoundTest.GetCount()==0)
			nStatus=2;
		if(pDoc->list_CompoundPositive.GetCount()==0 && pDoc->list_CompoundNegative.GetCount()==0 && pDoc->list_CompoundTest.GetCount()==0)
			nStatus=3;
	}
	if(nTagType==3)
	{
		nStatus=1;
		if(pDoc->list_Classifier.GetCount()==0)
			nStatus=3;
	}
	if(nTagType==4)
	{
		nStatus=1;
		int count=0;
		CCompound* pCompound;
		POSITION pos;
		pos=pDoc->list_CompoundTest.GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)pDoc->list_CompoundTest.GetNext(pos);
			if(pCompound->yhat==0.0)
				count++;
		}
		if(count>0)
			nStatus=2;
		if(count==pDoc->list_CompoundTest.GetCount())
			nStatus=3;
	}
	return 0;
}

void CTag::GetHelpString(CString& str)
{
	if(nTagType==1)
	{
		if(nStatus==3)
			str="Name your project title to distinguish from others.#Specify the names of two groups of compounds, for example, active or inactive. It will be the label of prediction.#You can always save your work session and continue your project next time.#";
		if(nStatus==2)
			str="Name your project title and two groups of compounds.#The value of some fields still remains as the default.#";
		if(nStatus==1)
			str="You have already updated project profile.#You can make changes at any time by clicking \"Project\" tag or go to \"Edit\".#Remember to save your work.#";
	}
	if(nTagType==2)
	{
		if(nStatus==3)
			str="Load labeled compounds with fingerprints as training data set. It will be used to develop a predictive model.#Once a model is developed, prediction can be made on the compounds in testing data set.#";
		if(nStatus==2)
			str="Model development requires compounds from two groups.#You can still carry training without testing data set.#Once a model is developed, prediction can be made on the compounds in testing data set.#";
		if(nStatus==1)
			str="You have already loaded training and testing data sets.#You can additively load more compounds or clean the existing data set.#You can view and edit your testing data set by View-Prediction.#";

	}
	if(nTagType==3)
	{
		if(nStatus==3)
			str="Develop a predictive model automatically on specified training sets.#View and edit developed model or export the whole model to a file.#";
		if(nStatus==1)
			str="A predictive model is available.#You can still continue your previous training.#Use cross-validation to find optimal parameters.#";
	}
	if(nTagType==4)
	{
		str="Make prediction according to developed model.#View and edit your testing data set.Copy selected items into clipboard or export prediction to a file.#";
	}

}

void CTag::ExecuteCommand(void)
{
	if(nTagType==1)
		AfxGetApp()->m_pMainWnd->SendMessage(WM_COMMAND,ID_EDIT_EDITPROJECTTITLE);
	if(nTagType==2)
		AfxGetApp()->m_pMainWnd->SendMessage(WM_COMMAND,ID_FILE_LOADFINGERPRINTS);
	if(nTagType==3)
	{
		if(nStatus==1)
			AfxGetApp()->m_pMainWnd->SendMessage(WM_COMMAND,ID_VIEW_TRAININGERROR);
		else
			AfxGetApp()->m_pMainWnd->SendMessage(WM_COMMAND,ID_ADABOOST_MODELTRAINING);
	}
	if(nTagType==4)
	{
		if(nStatus==1)
			AfxGetApp()->m_pMainWnd->SendMessage(WM_COMMAND,ID_VIEW_PREDICTION);
		else
			AfxGetApp()->m_pMainWnd->SendMessage(WM_COMMAND,ID_ADABOOST_PREDICTING);
	}
}
