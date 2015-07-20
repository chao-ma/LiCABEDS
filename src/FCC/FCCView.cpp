// FCCView.cpp : implementation of the CFCCView class
//

#include "stdafx.h"
#include "FCC.h"

#include "FCCDoc.h"
#include "FCCView.h"
#include "DispDlg.h"
#include "Shlwapi.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
extern CEvent draw_event;

// CFCCView

IMPLEMENT_DYNCREATE(CFCCView, CScrollView)

BEGIN_MESSAGE_MAP(CFCCView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_MESSAGE(WM_REALTIMEDRAW,&CFCCView::OnPaintNewClassifier)
	ON_MESSAGE(WM_INITIATEPAINT,&CFCCView::OnInitiatePaint)
	ON_WM_VSCROLL()
	ON_COMMAND(ID_VIEW_FITTOWINDOWSIZE, &CFCCView::OnViewFittowindowsize)
	ON_COMMAND(ID_VIEW_HORIZONALZOOMOUT, &CFCCView::OnViewHorizonalzoomout)
	ON_COMMAND(ID_VIEW_HORIZONALZOOMIN, &CFCCView::OnViewHorizonalzoomin)
	ON_COMMAND(ID_VIEW_VERTICALZOOMIN, &CFCCView::OnViewVerticalzoomin)
	ON_COMMAND(ID_VIEW_VERTICALZOOMOUT, &CFCCView::OnViewVerticalzoomout)
	ON_COMMAND(ID_VIEW_DISPLAYOPTION, &CFCCView::OnViewDisplayoption)
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_VIEW_SHOWCOORDINATES, &CFCCView::OnViewShowcoordinates)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FITTOWINDOWSIZE, &CFCCView::OnUpdateViewFittowindowsize)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_UPDATE_COMMAND_UI(ID_VIEW_HORIZONALZOOMIN, &CFCCView::OnUpdateViewHorizonalzoomin)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HORIZONALZOOMOUT, &CFCCView::OnUpdateViewHorizonalzoomout)
	ON_UPDATE_COMMAND_UI(ID_VIEW_VERTICALZOOMIN, &CFCCView::OnUpdateViewVerticalzoomin)
	ON_UPDATE_COMMAND_UI(ID_VIEW_VERTICALZOOMOUT, &CFCCView::OnUpdateViewVerticalzoomout)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWCOORDINATES, &CFCCView::OnUpdateViewShowcoordinates)
	ON_COMMAND(ID_EDIT_COPY, &CFCCView::OnEditCopy)
	ON_COMMAND(ID_EDIT_SELECTALL, &CFCCView::OnEditSelectall)
END_MESSAGE_MAP()

// CFCCView construction/destruction

CFCCView::CFCCView()
{
	// TODO: add construction code here
	gridpen.CreatePen(PS_DOT,0,RGB(0,184,164));
	errorpen.CreatePen(PS_SOLID,0,RGB(0,0,160));
	errorbrush.CreateSolidBrush(RGB(0,0,160));
	cvpen.CreatePen(PS_SOLID,0,RGB(160,16,0));
	cvbrush.CreateSolidBrush(RGB(160,16,0));
	gridfont.CreateFontA(12,0,0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,"Times New Roman");
	legendfont.CreateFontA(14,0,0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,"Times New Roman");
	NumGridVer=10;
	NumGridHor=4;
	bShowGrid=true;
	bLButtonDown=false;
	bCreateBitmap=false;
	ptOldMousePos.SetPoint(0,0);
}

CFCCView::~CFCCView()
{
	gridpen.DeleteObject();
	errorpen.DeleteObject();
	errorbrush.DeleteObject();
	cvpen.DeleteObject();
	cvbrush.DeleteObject();
	gridfont.DeleteObject();
	legendfont.DeleteObject();
	if(bCreateBitmap)
		memBitmap.DeleteObject();
}

BOOL CFCCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CFCCView drawing

void CFCCView::OnDraw(CDC* pDC)
{
	CFCCDoc* pDoc = GetDocument();
	//ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if(pDoc->NumClassifier>0)
	{
		//CRect clientRect;
		CString tmpstr;
		//GetClientRect(clientRect);
		//Preparing the paint properties
		CSize sizeTotal;
		sizeTotal.cx = (long)(20+pDoc->NumClassifier*ratioHor);
		sizeTotal.cy = (long)(20+(Height-20)*ratioVer);
		SetScrollSizes(MM_TEXT, sizeTotal);
		//Copy the memory bitmap into the dc to save drawing time
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		CBitmap* pOldBitmap=memDC.SelectObject(&memBitmap);
		
		//ppDC->StretchBlt(0,0,sizeTotal.cx,sizeTotal.cy,pDC,0,0,pDoc->NumClassifier,1000,SRCCOPY);
		//code for drawing the selection
		if(abs(selectLeft-selectRight)>0)
		{
			memDC.InvertRect(CRect(selectLeft-20,0,selectRight-20,sizeTotal.cy-20));
			//TRACE("%d  %d\n",selectRight,selectLeft);
		}
		pDC->BitBlt(20,20,sizeTotal.cx,sizeTotal.cy,&memDC,0,0,SRCCOPY);
		if(abs(selectLeft-selectRight)>0)
			memDC.InvertRect(CRect(selectLeft-20,0,selectRight-20,sizeTotal.cy-20));
		
		memDC.SelectObject(pOldBitmap);
		memDC.DeleteDC();
		//Paint the title
		pDC->SetBkMode(TRANSPARENT);
		CPen* pOldPen=pDC->SelectObject(&errorpen);
		CFont* pOldFont=pDC->SelectObject(&legendfont);
		pDC->SelectObject(&errorbrush);
		pDC->FillSolidRect(CRect(3,3,57,17),colorTrain);
		pDC->TextOutA(65,5,"Training Error");
		if(pDoc->cv)
		{
			pDC->SelectObject(&cvpen);
			pDC->SelectObject(&cvbrush);
			//pDC->Rectangle(150,3,204,17);
			pDC->FillSolidRect(CRect(150,3,204,17),colorCV);
			pDC->TextOutA(214,5,"Cross-Validation Error");
		}
		//Paint the grid
		if(bShowGrid)
		{
			pDC->SelectObject(&gridpen);
			pDC->SelectObject(&gridfont);
			
			for(int i=1;i<NumGridVer;i++)
			{
				pDC->MoveTo(20+i*pDoc->NumClassifier/NumGridVer*ratioHor,20);
				pDC->LineTo(20+i*pDoc->NumClassifier/NumGridVer*ratioHor,20+(Height-20)*ratioVer);
				tmpstr.Format("%d",i*pDoc->NumClassifier/NumGridVer);
				pDC->TextOutA(20+i*pDoc->NumClassifier/NumGridVer*ratioHor,20,tmpstr);
			}
			for(int i=1;i<NumGridHor;i++)
			{
				pDC->MoveTo(20,20+(Height-20)*ratioVer-ratioVer*(Height-20)*(1-(float)i/NumGridHor));
				pDC->LineTo(20+pDoc->NumClassifier*ratioHor,20+(Height-20)*ratioVer-ratioVer*(Height-20)*(1-(float)i/NumGridHor));
				tmpstr.Format(".%d",100*(NumGridHor-i)/NumGridHor);
				pDC->TextOutA(0,20+(Height-20)*ratioVer-ratioVer*(Height-20)*(1-(float)i/NumGridHor),tmpstr);
			}
		}

		
		//draw the mouse coordinate
		CPen trackPen(PS_DOT,1,RGB(255,128,64));
		pDC->SelectObject(&trackPen);
		if(bMouseTrack)
		{
			pDC->MoveTo(20,ptOldMousePos.y);
			pDC->LineTo(sizeTotal.cx,ptOldMousePos.y);
			pDC->MoveTo(ptOldMousePos.x,20);
			pDC->LineTo(ptOldMousePos.x,sizeTotal.cy);
		}
		//Clean up
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldFont);
		pDC->SelectStockObject(BLACK_BRUSH);
		
		
	}
	// TODO: add draw code for native data here
}

void CFCCView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 0;
	SetScrollSizes(MM_TEXT, sizeTotal);
	ratioVer=1.0;
	ratioHor=1.0;
	selectLeft=0;
	selectRight=0;
	Height=500;
	char pathName[MAX_PATH];
	CString tmpstr;
	::SHGetSpecialFolderPathA(GetSafeHwnd(),pathName,CSIDL_APPDATA ,0);
	tmpstr=pathName;
	tmpstr+="\\FCC\\fcc";
	bMouseTrack=false;
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_VIEW_SHOWCOORDINATES,bMouseTrack?MF_CHECKED:MF_UNCHECKED);
	if(PathFileExists(tmpstr))
	{
		CFile hFile;
		if(hFile.Open(tmpstr,CFile::modeRead))
		{
			CArchive ar(&hFile,CArchive::load);
			ar>>linewidthTrain;
			ar>>linestyleTrain;
			ar>>colorTrain;
			errorpen.DeleteObject();
			errorpen.CreatePen(linestyleTrain,linewidthTrain,colorTrain);
			errorbrush.DeleteObject();
			errorbrush.CreateSolidBrush(colorTrain);
			ar>>linewidthCV;
			ar>>linestyleCV;
			ar>>colorCV;
			cvpen.DeleteObject();
			cvpen.CreatePen(linestyleCV,linewidthCV,colorCV);
			cvbrush.DeleteObject();
			cvbrush.CreateSolidBrush(colorCV);
			ar>>NumGridVer;
			ar>>NumGridHor;
			ar>>bShowGrid;
			ar>>ratioVer;
			ar>>ratioHor;
			ar.Close();
			hFile.Close();
		}
	}
	RedrawBitmap();
	selectLeft=0;selectRight=0;
	TRACE("OnInitialUpdate called\n");
}


// CFCCView printing

BOOL CFCCView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	pInfo->SetMinPage(1);
	pInfo->SetMaxPage(1);
	
	return DoPreparePrinting(pInfo);
}

void CFCCView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	TRACE("Begin Printing\n");
	pDC->SetMapMode(MM_LOENGLISH);
	CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
	pDoc->MessageCenter(IDS_MESSAGE_STARTPRINT);
	
	// TODO: add extra initialization before printing
}

void CFCCView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: Add your specialized code here and/or call the base class
	
	CScrollView::OnPrepareDC(pDC, pInfo);
	if(pDC->IsPrinting())
	{
		TRACE("Setup Maping Mode\n");
		pDC->SetMapMode(MM_LOENGLISH);
	}
	
}

void CFCCView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: Add your specialized code here and/or call the base class
	
	/*if(pDC->IsPrinting())
	{
		pDC->SetMapMode(MM_LOENGLISH);
	}*/
	CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
	CPen* pOldPen;
	CPen printGridPen(PS_DOT,1,RGB(208,208,208));
	CPen printErrorPen(PS_SOLID,1,RGB(0,0,0));
	CPen printCVPen(PS_SOLID,2,RGB(0,0,0));
	CString tmpstr;
	//Calculate one-inch margin and corresponding drawing region rectangle
	TRACE("cx:%d cy:%d x1 %d y1 %d x2 %d y2 %d\n",pInfo->m_rectDraw.Width(),pInfo->m_rectDraw.Height(),pInfo->m_rectDraw.left,pInfo->m_rectDraw.top,pInfo->m_rectDraw.right,pInfo->m_rectDraw.bottom);
	TRACE("Physical Offset x:%d y:%d\n",pDC->GetDeviceCaps(PHYSICALOFFSETX),pDC->GetDeviceCaps(PHYSICALOFFSETY));
	CSize margin(pDC->GetDeviceCaps(PHYSICALOFFSETX),pDC->GetDeviceCaps(PHYSICALOFFSETY));
	pDC->DPtoLP(&margin);
	TRACE("Margin in logical unite %d %d\n",margin.cx,margin.cy);
	pInfo->m_rectDraw.SetRect(100-margin.cx,margin.cy-100,pInfo->m_rectDraw.right-100+margin.cx,pInfo->m_rectDraw.bottom+100-margin.cy);
	if(abs(pInfo->m_rectDraw.Height())>pInfo->m_rectDraw.Width())
		pInfo->m_rectDraw.SetRect(pInfo->m_rectDraw.left,pInfo->m_rectDraw.top,pInfo->m_rectDraw.right,pInfo->m_rectDraw.bottom/2);
	TRACE("cx:%d cy:%d x1 %d y1 %d x2 %d y2 %d\n",pInfo->m_rectDraw.Width(),pInfo->m_rectDraw.Height(),pInfo->m_rectDraw.left,pInfo->m_rectDraw.top,pInfo->m_rectDraw.right,pInfo->m_rectDraw.bottom);
	pDC->SetBkMode(TRANSPARENT);
	//Draw the legend
	int offsetx=0,offsety=20;
	pDC->TextOutA(pInfo->m_rectDraw.left+50,pInfo->m_rectDraw.top," Training Error ");
	offsety=pDC->GetTextExtent(" Training Error ").cy;
	pDC->MoveTo(pInfo->m_rectDraw.left,pInfo->m_rectDraw.top-offsety/2);
	pDC->LineTo(pInfo->m_rectDraw.left+50,pInfo->m_rectDraw.top-offsety/2);
	if(pDoc->cv)
	{
		pDC->MoveTo(pInfo->m_rectDraw.left+50+pDC->GetTextExtent(" Training Error ").cx,pInfo->m_rectDraw.top-offsety/2);
		pOldPen=pDC->SelectObject(&printCVPen);
		pDC->LineTo(pInfo->m_rectDraw.left+100+pDC->GetTextExtent(" Training Error ").cx,pInfo->m_rectDraw.top-offsety/2);
		pDC->SelectObject(pOldPen);
		pDC->TextOutA(pInfo->m_rectDraw.left+100+pDC->GetTextExtent(" Training Error ").cx,pInfo->m_rectDraw.top," Cross-Validation Error ");
	}
	pInfo->m_rectDraw.top=pInfo->m_rectDraw.top-offsety;
	//Draw the y axis label
	int i;
	for(i=1;i<NumGridVer;i++)
	{
		tmpstr.Format(".%d",100*i/NumGridVer);
		if(pDC->GetTextExtent(tmpstr).cx>offsetx)
			offsetx=pDC->GetTextExtent(tmpstr).cx;
		pDC->TextOutA(pInfo->m_rectDraw.left,pInfo->m_rectDraw.bottom+abs(pInfo->m_rectDraw.Height())*i/NumGridVer,tmpstr);
	}
	pInfo->m_rectDraw.left=pInfo->m_rectDraw.left+offsetx;
	//Draw the x axis label
	for(i=1;i<NumGridHor;i++)
	{
		tmpstr.Format("%d",pDoc->NumClassifier*i/NumGridHor);
		offsety=pDC->GetTextExtent(tmpstr).cy;
		pDC->TextOutA(pInfo->m_rectDraw.left+pInfo->m_rectDraw.Width()*i/NumGridHor,pInfo->m_rectDraw.bottom+offsety,tmpstr);
	}
	pInfo->m_rectDraw.bottom=pInfo->m_rectDraw.bottom+offsety;
	//Drawing the grids
	if(bShowGrid)
	{
		pOldPen=pDC->SelectObject(&printGridPen);
		for(i=1;i<NumGridVer;i++)
		{
			pDC->MoveTo(pInfo->m_rectDraw.left,pInfo->m_rectDraw.bottom+abs(pInfo->m_rectDraw.Height())*i/NumGridVer);
			pDC->LineTo(pInfo->m_rectDraw.right,pInfo->m_rectDraw.bottom+abs(pInfo->m_rectDraw.Height())*i/NumGridVer);
		}
		for(i=1;i<NumGridHor;i++)
		{
			pDC->MoveTo(pInfo->m_rectDraw.left+pInfo->m_rectDraw.Width()*i/NumGridHor,pInfo->m_rectDraw.bottom);
			pDC->LineTo(pInfo->m_rectDraw.left+pInfo->m_rectDraw.Width()*i/NumGridHor,pInfo->m_rectDraw.top);
		}
		pDC->SelectObject(pOldPen);
	}
	//Draw the axis
	pDC->MoveTo(pInfo->m_rectDraw.left,pInfo->m_rectDraw.bottom);
	pDC->LineTo(pInfo->m_rectDraw.right,pInfo->m_rectDraw.bottom);
	for(i=1;i<NumGridVer;i++)
	{
		pDC->MoveTo(pInfo->m_rectDraw.left,pInfo->m_rectDraw.bottom+abs(pInfo->m_rectDraw.Height())*i/NumGridVer);
		pDC->LineTo(pInfo->m_rectDraw.left+5,pInfo->m_rectDraw.bottom+abs(pInfo->m_rectDraw.Height())*i/NumGridVer);
	}
	pDC->MoveTo(pInfo->m_rectDraw.left,pInfo->m_rectDraw.bottom);
	pDC->LineTo(pInfo->m_rectDraw.left,pInfo->m_rectDraw.top);
	for(i=1;i<NumGridHor;i++)
	{
		pDC->MoveTo(pInfo->m_rectDraw.left+pInfo->m_rectDraw.Width()*i/NumGridHor,pInfo->m_rectDraw.bottom);
		pDC->LineTo(pInfo->m_rectDraw.left+pInfo->m_rectDraw.Width()*i/NumGridHor,pInfo->m_rectDraw.bottom+5);
	}
	//Drawing the curves
	float ratioX,ratioY;
	int count=0;
	CClassifier* pClassifier;
	POSITION pos;
	ratioX=(float)pInfo->m_rectDraw.Width()/pDoc->NumClassifier;
	ratioY=(float)abs(pInfo->m_rectDraw.Height());
	pDC->MoveTo(pInfo->m_rectDraw.left,pInfo->m_rectDraw.bottom);
	pos=pDoc->list_Classifier.GetHeadPosition();
	while(pos)
	{
		pClassifier=(CClassifier*)(pDoc->list_Classifier.GetNext(pos));
		pDC->LineTo(pInfo->m_rectDraw.left+count*ratioX,pInfo->m_rectDraw.bottom+pClassifier->trainingerror*ratioY);
		count++;
	}
	if(pDoc->cv)
	{
		pOldPen=pDC->SelectObject(&printCVPen);
		count=0;
		pos=pDoc->list_Classifier.GetHeadPosition();
		if(pos)
		{
			pClassifier=(CClassifier*)pDoc->list_Classifier.GetHead();
			if(pDoc->bSmoothCV)
				pDC->MoveTo(pInfo->m_rectDraw.left,pInfo->m_rectDraw.bottom+pClassifier->SmoothCVError*ratioY);
			else
				pDC->MoveTo(pInfo->m_rectDraw.left,pInfo->m_rectDraw.bottom+pClassifier->cverror*ratioY);
		}
		while(pos)
		{
			pClassifier=(CClassifier*)(pDoc->list_Classifier.GetNext(pos));
			if(pDoc->bSmoothCV)
				pDC->LineTo(pInfo->m_rectDraw.left+count*ratioX,pInfo->m_rectDraw.bottom+pClassifier->SmoothCVError*ratioY);
			else
				pDC->LineTo(pInfo->m_rectDraw.left+count*ratioX,pInfo->m_rectDraw.bottom+pClassifier->cverror*ratioY);
			count++;
		}
		pDC->SelectObject(pOldPen);
	}
	//CScrollView::OnPrint(pDC, pInfo);
}


void CFCCView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	//TRACE("End Printing\n");
	CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
	pDoc->MessageCenter(IDS_MESSAGE_ENDPRINT);
	// TODO: add cleanup after printing
}


// CFCCView diagnostics

#ifdef _DEBUG
void CFCCView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CFCCView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CFCCDoc* CFCCView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFCCDoc)));
	return (CFCCDoc*)m_pDocument;
}
#endif //_DEBUG


// CFCCView message handlers
LRESULT CFCCView::OnInitiatePaint(WPARAM wParam, LPARAM lParam)
{
	Invalidate();
	RedrawBitmap();
	return LRESULT();
}
LRESULT CFCCView::OnPaintNewClassifier(WPARAM wParam, LPARAM lParam)
{
	CClientDC clientdc(this);
	CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
	CBitmap* pOldBitmap;
	CDC dc;
	dc.CreateCompatibleDC(&clientdc);
	pOldBitmap=dc.SelectObject(&memBitmap);
	int count=(int)wParam;
	//if(pDoc->list_Classifier.GetCount()>1)
	if(count>1)
	{
		CClassifier* pClassifier;
		dc.SelectObject(&errorpen);
		//pClassifier=(CClassifier*)(pDoc->list_Classifier.GetAt(pDoc->list_Classifier.FindIndex(pDoc->list_Classifier.GetCount()-2)));
		pClassifier=(CClassifier*)(pDoc->list_Classifier.GetAt(pDoc->list_Classifier.FindIndex(count-2)));
		//dc.MoveTo((pDoc->list_Classifier.GetCount()-2)*ratioHor,(Height-20)*ratioVer-(Height-20)*ratioVer*pClassifier->trainingerror);
		dc.MoveTo((count-2)*ratioHor,(Height-20)*ratioVer-(Height-20)*ratioVer*pClassifier->trainingerror);
		//pClassifier=(CClassifier*)(pDoc->list_Classifier.GetAt(pDoc->list_Classifier.FindIndex(pDoc->list_Classifier.GetCount()-1)));
		pClassifier=(CClassifier*)(pDoc->list_Classifier.GetAt(pDoc->list_Classifier.FindIndex(count-1)));
		//dc.LineTo((pDoc->list_Classifier.GetCount()-1)*ratioHor,(Height-20)*ratioVer-(Height-20)*ratioVer*pClassifier->trainingerror);
		dc.LineTo((count-1)*ratioHor,(Height-20)*ratioVer-(Height-20)*ratioVer*pClassifier->trainingerror);
		if(pDoc->cv)
		{
			dc.SelectObject(&cvpen);
			//pClassifier=(CClassifier*)(pDoc->list_Classifier.GetAt(pDoc->list_Classifier.FindIndex(pDoc->list_Classifier.GetCount()-2)));
			pClassifier=(CClassifier*)(pDoc->list_Classifier.GetAt(pDoc->list_Classifier.FindIndex(count-2)));
			//dc.MoveTo((pDoc->list_Classifier.GetCount()-2)*ratioHor,(Height-20)*ratioVer-(Height-20)*ratioVer*pClassifier->cverror);
			dc.MoveTo((count-2)*ratioHor,(Height-20)*ratioVer-(Height-20)*ratioVer*pClassifier->cverror);
			//pClassifier=(CClassifier*)(pDoc->list_Classifier.GetAt(pDoc->list_Classifier.FindIndex(pDoc->list_Classifier.GetCount()-1)));
			pClassifier=(CClassifier*)(pDoc->list_Classifier.GetAt(pDoc->list_Classifier.FindIndex(count-1)));
			//dc.LineTo((pDoc->list_Classifier.GetCount()-1)*ratioHor,(Height-20)*ratioVer-(Height-20)*ratioVer*pClassifier->cverror);
			dc.LineTo((count-1)*ratioHor,(Height-20)*ratioVer-(Height-20)*ratioVer*pClassifier->cverror);
		}
		dc.SelectStockObject(WHITE_PEN);
	}

	dc.SelectObject(pOldBitmap);
	draw_event.SetEvent();
	dc.DeleteDC();
	OnPrepareDC(&clientdc);
	//CRect updateRect(20+(pDoc->list_Classifier.GetCount()-2)*ratioHor,0,20+(pDoc->list_Classifier.GetCount()-1)*ratioHor,20+(Height-20)*ratioVer);
	CRect updateRect(20+(count-2)*ratioHor,0,20+(count-1)*ratioHor,20+(Height-20)*ratioVer);
	clientdc.LPtoDP(updateRect);
	InvalidateRect(updateRect,0);
	return LRESULT();
}

void CFCCView::RedrawBitmap(void)
{
	CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
	CClassifier* pClassifier;
	POSITION posa,posb;
	CClientDC clientdc(this);
	CDC dc;
	dc.CreateCompatibleDC(&clientdc);
	if(bCreateBitmap)
		memBitmap.DeleteObject();
	bool res=memBitmap.CreateCompatibleBitmap(&clientdc,pDoc->NumClassifier*ratioHor,(Height-20)*ratioVer);
	int count=0;
	while(res==false && count<20)
	{
		ratioHor=ratioHor/1.1;
		ratioVer=ratioVer/1.1;
		count++;
		memBitmap.DeleteObject();
		res=memBitmap.CreateCompatibleBitmap(&clientdc,pDoc->NumClassifier*ratioHor,(Height-20)*ratioVer);
	}
	if(res==false)
	{
		pDoc->MessageCenter(IDS_MESSAGE_NOMEMORY);
	}
	if(res==true && count>0)
	{
		pDoc->MessageCenter(IDS_MESSAGE_SMALLZOOMRATIO);
	}	
	bCreateBitmap=true;
	CDC* pDC=&dc;
	CBitmap* pOldBitmap=dc.SelectObject(&memBitmap);
	dc.FillSolidRect(0,0,pDoc->NumClassifier*ratioHor,(Height-20)*ratioVer,RGB(255,255,255));
	posb=pDoc->list_Classifier.GetHeadPosition();
	if(posb)
	{
		posa=posb;
		pDoc->list_Classifier.GetNext(posb);
		int count=0;
		pDC->SelectObject(&errorpen);
		while(posa && posb)
		{
			pClassifier=(CClassifier*)pDoc->list_Classifier.GetAt(posa);
			pDC->MoveTo(count*ratioHor,(Height-20)*ratioVer-ratioVer*(Height-20)*pClassifier->trainingerror);
			pClassifier=(CClassifier*)pDoc->list_Classifier.GetAt(posb);
			pDC->LineTo((count+1)*ratioHor,(Height-20)*ratioVer-ratioVer*(Height-20)*pClassifier->trainingerror);
			count++;
			posa=posb;
			pDoc->list_Classifier.GetNext(posb);
		}
	}
	if(pDoc->cv)
	{
		posb=pDoc->list_Classifier.GetHeadPosition();
		if(posb)
		{
			posa=posb;
			pDoc->list_Classifier.GetNext(posb);
			int count=0;
			pDC->SelectObject(&cvpen);
			while(posa && posb)
			{
				pClassifier=(CClassifier*)pDoc->list_Classifier.GetAt(posa);
				if(pDoc->bSmoothCV)
					pDC->MoveTo(count*ratioHor,(Height-20)*ratioVer-ratioVer*(Height-20)*pClassifier->SmoothCVError);
				else
					pDC->MoveTo(count*ratioHor,(Height-20)*ratioVer-ratioVer*(Height-20)*pClassifier->cverror);
				pClassifier=(CClassifier*)pDoc->list_Classifier.GetAt(posb);
				if(pDoc->bSmoothCV)
					pDC->LineTo((count+1)*ratioHor,(Height-20)*ratioVer-ratioVer*(Height-20)*pClassifier->SmoothCVError);
				else
					pDC->LineTo((count+1)*ratioHor,(Height-20)*ratioVer-ratioVer*(Height-20)*pClassifier->cverror);
				count++;
				posa=posb;
				pDoc->list_Classifier.GetNext(posb);
			}
		}
	}
	dc.SelectObject(pOldBitmap);
	dc.SelectStockObject(WHITE_PEN);
	dc.DeleteDC();
	selectLeft=selectRight=0;
}

void CFCCView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	//Invalidate();
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CFCCView::OnViewFittowindowsize()
{
	// TODO: Add your command handler code here
	CRect clientRect;
	GetClientRect(clientRect);
	CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
	Height=clientRect.Height()-18;
	ratioVer=1.0;
	ratioHor=(float)(clientRect.right-clientRect.left-20)/pDoc->NumClassifier;
	RedrawBitmap();
	Invalidate();
	SaveDisplayOption();
	
}

void CFCCView::OnViewHorizonalzoomout()
{
	// TODO: Add your command handler code here
	ratioHor/=1.1;
	RedrawBitmap();
	Invalidate();
	SaveDisplayOption();
}

void CFCCView::OnViewHorizonalzoomin()
{
	// TODO: Add your command handler code here
	ratioHor*=1.1;
	RedrawBitmap();
	Invalidate();
	SaveDisplayOption();
}

void CFCCView::OnViewVerticalzoomin()
{
	// TODO: Add your command handler code here
	ratioVer+=0.1;
	RedrawBitmap();
	Invalidate();
	SaveDisplayOption();
}

void CFCCView::OnViewVerticalzoomout()
{
	// TODO: Add your command handler code here
	ratioVer-=0.1;
	RedrawBitmap();
	Invalidate();
	SaveDisplayOption();
}

int CFCCView::SaveDisplayOption(void)
{
	CString tmpstr;
	char pathName[MAX_PATH];
	::SHGetSpecialFolderPathA(NULL,pathName,CSIDL_APPDATA ,0);
	tmpstr=pathName;
	tmpstr+="\\FCC\\fcc";
	CFile hFile;
	if(hFile.Open(tmpstr,CFile::modeCreate|CFile::modeWrite))
	{
		CArchive ar(&hFile,CArchive::store);
		ar<<linewidthTrain;
		ar<<linestyleTrain;
		ar<<colorTrain;//save training display option
		ar<<linewidthCV;
		ar<<linestyleCV;
		ar<<colorCV;//save cv display option
		ar<<NumGridVer;
		ar<<NumGridHor;
		ar<<bShowGrid;//save grid display option
		ar<<ratioVer;
		ar<<ratioHor;//save zoom ratio
		ar.Close();
		hFile.Close();
	}
	else
		return 0;
	return 1;
}

void CFCCView::OnViewDisplayoption()
{
	// TODO: Add your command handler code here
	CDispDlg dlg;
	dlg.pView=this;
	if(dlg.DoModal()==IDOK)
	{
		errorpen.DeleteObject();
		errorpen.CreatePen(linestyleTrain,linewidthTrain,colorTrain);
		errorbrush.DeleteObject();
		errorbrush.CreateSolidBrush(colorTrain);
		cvpen.DeleteObject();
		cvpen.CreatePen(linestyleCV,linewidthCV,colorCV);
		cvbrush.DeleteObject();
		cvbrush.CreateSolidBrush(colorCV);
		RedrawBitmap();
		Invalidate();
		SaveDisplayOption();		
	}
}

BOOL CFCCView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	//TRACE("%d\n",zDelta);
	if(nFlags==MK_SHIFT)
	{
		if(zDelta<0)
			SendMessage(WM_VSCROLL,SB_LINEDOWN,0);
		else
			SendMessage(WM_VSCROLL,SB_LINEUP,0);
	}
	else
	{
		if(zDelta>0)
			SendMessage(WM_HSCROLL,SB_LINELEFT,0);
		else
			SendMessage(WM_HSCROLL,SB_LINERIGHT,0);
	}
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

void CFCCView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(bMouseTrack)
	{
		CClientDC dc(this);
		OnPrepareDC(&dc);
		CPoint mPoint(point);
		dc.DPtoLP(&mPoint);
		int x;float y;
		x=(mPoint.x-20)/ratioHor;
		y=((Height-20)*ratioVer+20-mPoint.y)/((Height-20)*ratioVer);
		CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
		if(!pDoc->list_Classifier.IsEmpty() && x>=0 && x<pDoc->list_Classifier.GetCount() && y>=0.0 && y<=1.0)
		{
			CString tmpstr,tailstr;
			CClassifier* pClassifier;
			pClassifier=(CClassifier*)pDoc->list_Classifier.GetAt(pDoc->list_Classifier.FindIndex(x));
			if(pDoc->cv)
			{
				if(pDoc->bSmoothCV)
					tmpstr.Format("Num:%d T Err:%.3f CV Err %.3f;  ",x,pClassifier->trainingerror,pClassifier->SmoothCVError);
				else
					tmpstr.Format("Num:%d T Err:%.3f Smoothed CV Err %.3f;  ",x,pClassifier->trainingerror,pClassifier->cverror);
			}
			else
				tmpstr.Format("Num:%d T Err:%.3f;  ",x,pClassifier->trainingerror);
			int tmp,i=0;
			tmp=pClassifier->bit;
			while(tmp>0)
			{
				tmp=tmp/10;
				i++;
			}
			for(;i<6;i++)
				tmpstr+="0";
			tailstr.Format("%d %d -> 1",pClassifier->bit,pClassifier->value);
			tmpstr=tmpstr+tailstr;
			((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_wndStatusBar.SetPaneText(1,tmpstr);
			dc.LPtoDP(&ptOldMousePos);
			InvalidateRect(CRect(0,ptOldMousePos.y-1,21+(pDoc->NumClassifier)*ratioHor,ptOldMousePos.y+1),0);
			InvalidateRect(CRect(ptOldMousePos.x-1,0,ptOldMousePos.x+1,21+(Height-20)*ratioVer),0);
			dc.DPtoLP(&ptOldMousePos);
			ptOldMousePos=mPoint;
			if(pDoc->cv)
			{
				if(pDoc->bSmoothCV)
					ptOldMousePos.y=20+(Height-20)*ratioVer-ratioVer*(Height-20)*pClassifier->SmoothCVError;
				else
					ptOldMousePos.y=20+(Height-20)*ratioVer-ratioVer*(Height-20)*pClassifier->cverror;
			}
			else
				ptOldMousePos.y=20+(Height-20)*ratioVer-ratioVer*(Height-20)*pClassifier->trainingerror;
			dc.LPtoDP(&ptOldMousePos);
			InvalidateRect(CRect(0,ptOldMousePos.y-1,21+(pDoc->NumClassifier)*ratioHor,ptOldMousePos.y+1),1);
			InvalidateRect(CRect(ptOldMousePos.x-1,0,ptOldMousePos.x+1,21+(Height-20)*ratioVer),1);
			dc.DPtoLP(&ptOldMousePos);
			
		}
	}
	if(bLButtonDown)
	{
		CClientDC dc(this);
		OnPrepareDC(&dc);
		CPoint mPoint;
		mPoint=point;
		dc.DPtoLP(&mPoint);
		CFCCDoc* pDoc;
		pDoc=(CFCCDoc*)GetDocument();
		if(mPoint.x>=20 && mPoint.x<(pDoc->list_Classifier.GetCount()*ratioHor+20) && mPoint.y>20 && mPoint.y<20+(Height-20)*ratioVer)
		{
			selectRight=mPoint.x;
			if(selectLeft<selectLeftMost)
				selectLeftMost=selectLeft;
			if(selectRight<selectLeftMost)
				selectLeftMost=selectRight;
			if(selectLeft>selectRightMost)
				selectRightMost=selectLeft;
			if(selectRight>selectRightMost)
				selectRightMost=selectRight;
			CRect rect(selectLeftMost,0,selectRightMost,20+(Height-20)*ratioVer);
			dc.LPtoDP(rect);
			InvalidateRect(rect,0);
		}

		//Get the selected classifiers
		POSITION pos;
		pos=pDoc->list_Classifier.GetHeadPosition();
		int index=0;
		int count=0;
		float meanTrainingError=0;
		float meanCVError=0;
		CClassifier* pClassifier;
		while(pos)
		{
			pClassifier=(CClassifier*)(pDoc->list_Classifier.GetNext(pos));
			if((index*ratioHor+20>=selectLeft && index*ratioHor+20<=selectRight)||
				(index*ratioHor+20<=selectLeft && index*ratioHor+20>=selectRight))
			{
				meanTrainingError+=pClassifier->trainingerror;
				meanCVError+=pClassifier->cverror;
				count++;
			}
			index++;
		}
		if(count>0)
		{
			if(pDoc->cv)
				pDoc->strMessage.Format("%d classifier(s) selected. Average training error: %.3f;Average cv error %.3f",count,meanTrainingError/count,meanCVError/count);
			else
				pDoc->strMessage.Format("%d classifier(s) selected. Average training error: %.3f",count,meanTrainingError/count);
			pDoc->MessageCenter(IDS_MESSAGE_SELECTCLASSIFIER);
		}

	}
	CScrollView::OnMouseMove(nFlags, point);
}

void CFCCView::OnViewShowcoordinates()
{
	// TODO: Add your command handler code here
	bMouseTrack=!bMouseTrack;
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_VIEW_SHOWCOORDINATES,bMouseTrack?MF_CHECKED:MF_UNCHECKED);
	if(!bMouseTrack)
		Invalidate(0);
}

void CFCCView::OnUpdateViewFittowindowsize(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable((CFCCDoc*)GetDocument()->NumClassifier>0);
}

void CFCCView::OnEditCopy()
{
	// TODO: Add your command handler code here
	if(!OpenClipboard())
		return;
	EmptyClipboard();
	int index=0;
	CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
	POSITION pos=pDoc->list_Classifier.GetHeadPosition();
	CClassifier* pClassifier;
	CString tmpstr;
	CString strContainer;
	strContainer="";
	while(pos)
	{
		pClassifier=(CClassifier*)pDoc->list_Classifier.GetNext(pos);
		if((index*ratioHor+20>=selectLeft && index*ratioHor+20<=selectRight)||
				(index*ratioHor+20<=selectLeft && index*ratioHor+20>=selectRight))
		{
			tmpstr.Format("id %d\t",index);
			strContainer+=tmpstr;
			tmpstr.Format("bit %d==",pClassifier->bit);
			strContainer+=tmpstr;
			tmpstr.Format("%d->1\t",pClassifier->value);
			strContainer+=tmpstr;
			tmpstr.Format("weight %f\t",pClassifier->weight);
			strContainer+=tmpstr;
			tmpstr.Format("T Error %f\t",pClassifier->trainingerror);
			strContainer+=tmpstr;
			if(pDoc->cv)
			{
				tmpstr.Format("CV Error %f\t",pClassifier->cverror);
				strContainer+=tmpstr;
			}
			tmpstr.Format("\n",pClassifier->trainingerror);
			strContainer+=tmpstr;
			/*pClassifier->
			meanTrainingError+=pClassifier->trainingerror;
			meanCVError+=pClassifier->cverror;*/
		}
		index++;
	}
	TRACE("%s",strContainer);
	HGLOBAL hglbCopy;
	LPTSTR  lptstrCopy;
	hglbCopy=::GlobalAlloc(GMEM_MOVEABLE,(strContainer.GetLength()+1)*sizeof(char));
	if(hglbCopy==NULL)
	{
		CloseClipboard();
		return;
	}
	lptstrCopy=(LPTSTR)::GlobalLock(hglbCopy);
	memcpy(lptstrCopy,(LPCTSTR)strContainer,strContainer.GetLength()*sizeof(char));
	lptstrCopy[strContainer.GetLength()*sizeof(char)]=0;
	::GlobalUnlock(hglbCopy);
	SetClipboardData(CF_TEXT, hglbCopy);
	CloseClipboard();

}

void CFCCView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	SetCapture();
	CClientDC dc(this);
	OnPrepareDC(&dc);
	CPoint mPoint;
	mPoint=point;
	dc.DPtoLP(&mPoint);
	CFCCDoc* pDoc;
	pDoc=(CFCCDoc*)GetDocument();
	CRect rect;
	//if(selectLeft>selectRight)
	//	rect.SetRect(selectLeft+10,0,selectRight-10,20+(Height-20)*ratioVer);
	//else
	rect.SetRect(selectLeftMost-10,0,selectRightMost+10,20+(Height-20)*ratioVer);
	dc.LPtoDP(&rect);
	InvalidateRect(rect,0);
	//Invalidate(0);
	if(mPoint.x>=20 && mPoint.x<(pDoc->list_Classifier.GetCount()*ratioHor+20) && mPoint.y>20 && mPoint.y<20+(Height-20)*ratioVer)
	{
		selectLeft=mPoint.x;
		selectRight=mPoint.x;
		selectRightMost=selectLeftMost=selectLeft;
		bLButtonDown=true;
	}
	CScrollView::OnLButtonDown(nFlags, point);
}

void CFCCView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	ReleaseCapture();
	if(bLButtonDown)
	{
		ReleaseCapture();
		bLButtonDown=false;
		CClientDC dc(this);
		OnPrepareDC(&dc);
		CPoint mPoint;
		mPoint=point;
		dc.DPtoLP(&mPoint);
		CFCCDoc* pDoc;
		pDoc=(CFCCDoc*)GetDocument();
		if(mPoint.x>=20 && mPoint.x<(pDoc->list_Classifier.GetCount()*ratioHor+20) && mPoint.y>20 && mPoint.y<20+(Height-20)*ratioVer)
		{
			selectRight=mPoint.x;
		}
	}
	/*else
	{
		selectLeft=0;
		selectRight=0;
	}*/
	CScrollView::OnLButtonUp(nFlags, point);
}

void CFCCView::OnRButtonDown(UINT nFlags, CPoint point)
{
	
	// TODO: Add your message handler code here and/or call default
	CScrollView::OnRButtonDown(nFlags, point);
	CMenu m_menu;
	CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
	CPoint mPoint=point;
	ClientToScreen(&mPoint);
	
	m_menu.LoadMenuA(IDR_FCCVIEWMENU);
	
	if(!pDoc->NumClassifier>0)
	{
		m_menu.GetSubMenu(0)->EnableMenuItem(ID_VIEW_FITTOWINDOWSIZE,MF_GRAYED);
		m_menu.GetSubMenu(0)->EnableMenuItem(ID_VIEW_VERTICALZOOMIN,MF_GRAYED);
		m_menu.GetSubMenu(0)->EnableMenuItem(ID_VIEW_VERTICALZOOMOUT,MF_GRAYED);
		m_menu.GetSubMenu(0)->EnableMenuItem(ID_VIEW_HORIZONALZOOMIN,MF_GRAYED);
		m_menu.GetSubMenu(0)->EnableMenuItem(ID_VIEW_HORIZONALZOOMOUT,MF_GRAYED);
		m_menu.GetSubMenu(0)->EnableMenuItem(ID_VIEW_SHOWCOORDINATES,MF_GRAYED);
	}
	if(bMouseTrack)
		m_menu.GetSubMenu(0)->CheckMenuItem(ID_VIEW_SHOWCOORDINATES,MF_CHECKED);
	m_menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN,mPoint.x,mPoint.y,this);
	
}

void CFCCView::OnUpdateViewHorizonalzoomin(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable((CFCCDoc*)GetDocument()->NumClassifier>0);
}

void CFCCView::OnUpdateViewHorizonalzoomout(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((CFCCDoc*)GetDocument()->NumClassifier>0);
	// TODO: Add your command update UI handler code here
}

void CFCCView::OnUpdateViewVerticalzoomin(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable((CFCCDoc*)GetDocument()->NumClassifier>0);
}

void CFCCView::OnUpdateViewVerticalzoomout(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable((CFCCDoc*)GetDocument()->NumClassifier>0);
}

void CFCCView::OnUpdateViewShowcoordinates(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable((CFCCDoc*)GetDocument()->NumClassifier>0);
}

void CFCCView::OnEditSelectall()
{
	// TODO: Add your command handler code here
	selectLeft=20;
	selectRight=20+ratioHor*((CFCCDoc*)GetDocument())->list_Classifier.GetCount();
	selectLeftMost=selectLeft;
	selectRightMost=selectRight;
	Invalidate();
}

void CFCCView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: Add your specialized code here and/or call the base class
	/*Invalidate();
	this->UpdateWindow();*/
}
