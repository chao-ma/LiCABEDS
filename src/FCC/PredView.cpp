// PredView.cpp : implementation file
//

#include "stdafx.h"
#include "FCC.h"
#include "PredView.h"
#include "FCCDoc.h"
#include "math.h"
#include "afxdlgs.h"
// CPredView

IMPLEMENT_DYNCREATE(CPredView, CScrollView)

CPredView::CPredView()
{
	pCompound=NULL;
	SortAscend=ASCEND;
	bOldMouseInCaption=false;
	cursorAscend=AfxGetApp()->LoadCursorA(IDC_ASCEND);
	cursorDescend=AfxGetApp()->LoadCursorA(IDC_DESCEND);
	bHitCaption=false;
}

CPredView::~CPredView()
{
	::DeleteObject(cursorAscend);
	::DeleteObject(cursorDescend);
}


BEGIN_MESSAGE_MAP(CPredView, CScrollView)
	ON_MESSAGE(WM_NAVIBAR,&CPredView::OnClickNavi)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_COMMAND(ID_EDIT_FIND, &CPredView::OnEditFind)
	ON_MESSAGE(WM_FINDCOMPOUND,&CPredView::OnFindCompound)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	//ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT, &CPredView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	//ON_COMMAND(ID_FILE_PRINT, &CPredView::OnFilePrint)
	ON_COMMAND(ID_EDIT_COPY, &CPredView::OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CPredView::OnEditPaste)
	ON_COMMAND(ID_EDIT_CUT, &CPredView::OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CPredView::OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_SELECTALL, &CPredView::OnEditSelectall)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECTALL, &CPredView::OnUpdateEditSelectall)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, &CPredView::OnUpdateEditFind)
	ON_COMMAND(ID_CPREDVIEW_INVERSESELECTION, &CPredView::OnCpredviewInverseselection)
	ON_UPDATE_COMMAND_UI(ID_CPREDVIEW_INVERSESELECTION, &CPredView::OnUpdateCpredviewInverseselection)
	ON_WM_RBUTTONDOWN()
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CPredView::OnUpdateFilePrintPreview)
END_MESSAGE_MAP()


// CPredView drawing

void CPredView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	OnUpdate(NULL,0,0);
	
}

void CPredView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// TODO: Add your specialized code here and/or call the base class
	//OnInitialUpdate();
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 0;
	SetScrollSizes(MM_TEXT, sizeTotal);
	this->dlg.m_pView=this;
	pCompound=NULL;

	CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
	CCompound* pCompound;
	CString tmpstr;
	CString decimalstr;
	POSITION pos;
	int cx,i;
	float yhat;
	int integer,decimal;
	nDecimal=3;
	CClientDC dc(this);
	OnPrepareDC(&dc);
	widthCompoundName=0;
	widthClassName=0;
	widthYhat=0;
	widthID=0;
	pos=pDoc->list_CompoundTest.GetHeadPosition();
	while(pos)
	{
		pCompound=(CCompound*)(pDoc->list_CompoundTest.GetNext(pos));
		cx=dc.GetTextExtent(pCompound->strCompoundName).cx;
		if(cx>widthCompoundName)
			widthCompoundName=cx;
		tmpstr.Format("%d",pCompound->id);
		cx=dc.GetTextExtent(tmpstr).cx;
		if(cx>widthID)
			widthID=cx;

		yhat=pCompound->yhat;
		integer=abs((int)yhat);
		decimal=abs(yhat)*pow(10.0,(double)nDecimal)-integer*pow(10.0,(double)nDecimal);
		tmpstr.Format("%d.",integer);
		decimalstr.Format("%d",decimal);
		for(i=0;i<nDecimal-decimalstr.GetLength();i++)
			tmpstr=tmpstr+"0";
		tmpstr=tmpstr+decimalstr;
		if(yhat<0)
			tmpstr=CString("-")+tmpstr;

		cx=dc.GetTextExtent(tmpstr).cx;
		if(cx>widthYhat)
			widthYhat=cx;
	}
	widthClassName=dc.GetTextExtent(pDoc->strPositiveSet).cx>dc.GetTextExtent(pDoc->strNegativeSet).cx?
		dc.GetTextExtent(pDoc->strPositiveSet).cx:dc.GetTextExtent(pDoc->strNegativeSet).cx;
	if(dc.GetTextExtent("Unknown").cx>widthClassName)
		widthClassName=dc.GetTextExtent("Unknown").cx;
	if(dc.GetTextExtent("Compound Name").cx>widthCompoundName)
		widthCompoundName=dc.GetTextExtent("Compound Name").cx;
	if(dc.GetTextExtent("Prediction").cx>widthClassName)
		widthClassName=dc.GetTextExtent("Prediction").cx;
	if(dc.GetTextExtent("Raw Value").cx>widthYhat)
		widthYhat=dc.GetTextExtent("Raw Value").cx;
	if(dc.GetTextExtent("ID").cx>widthID)
		widthID=dc.GetTextExtent("ID").cx;
	//TRACE("%d %d %d\n",widthCompoundName,widthClassName,widthYhat);

	CRect rect;
	GetClientRect(rect);
	nCol=rect.Width()/(80+widthCompoundName+widthYhat+widthClassName+widthID);
	if(nCol<1)
		nCol=1;
	nItemPage=(rect.Height()-50)/20;
	if(nItemPage<10)
		nItemPage=10;
	wnd_navi.DestroyWindow();
	wnd_navi.bDropDown=false;
	wnd_navi.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,CRect(1,1,200,21),this,101);
	wnd_navi.m_nRow.DestroyWindow();
	wnd_navi.m_nCol.DestroyWindow();
	wnd_navi.m_nRow.Create(WS_CHILD|WS_VISIBLE,CRect(40,21,90,39),&wnd_navi,101);
	wnd_navi.m_nCol.Create(WS_CHILD|WS_VISIBLE,CRect(130,21,170,39),&wnd_navi,102);
	wnd_navi.nCurPage=1;
	wnd_navi.nTotalPage=pDoc->list_CompoundTest.GetCount()/(nCol*nItemPage)+1;
	wnd_navi.Invalidate();
	tmpstr.Format("%d",nItemPage);
	wnd_navi.m_nRow.SetWindowTextA(tmpstr);
	tmpstr.Format("%d",nCol);
	wnd_navi.m_nCol.SetWindowTextA(tmpstr);
	
}

void CPredView::OnUpdateFilePrintPreview(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(0);
}

void CPredView::OnDraw(CDC* pDC)
{
	// TODO: add draw code here
	CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
	CCompound* pCompound;
	CString tmpstr;
	CString decimalstr;
	POSITION pos;
	CPoint curpos;
	float yhat;
	int integer,decimal,i;
	int curCol=0;
	int initialoffsety=30;
	int initialoffsetx=60;
	curpos.x=initialoffsetx;
	curpos.y=initialoffsety;
	pDC->SetBkMode(TRANSPARENT);
	pos=pDoc->list_CompoundTest.GetHeadPosition();
	bool flag=true;// to show the interchanging color in the table
	for(i=1;i<wnd_navi.nCurPage;i++)
		for(int j=0;j<nItemPage*nCol;j++)
			pDoc->list_CompoundTest.GetNext(pos);
	while(pos && curCol<nCol)
	{
		pCompound=(CCompound*)(pDoc->list_CompoundTest.GetNext(pos));
		flag=!flag;
		if(curpos.y<=initialoffsety)
		{
			pDC->FillSolidRect(curpos.x-10,curpos.y-2,50+widthCompoundName+widthYhat+widthClassName+widthID,20,RGB(12,37,97));
			pDC->SetTextColor(RGB(225,225,255));
			pDC->TextOutA(curpos.x,curpos.y,"ID");
			curpos.x+=widthID+10;
			pDC->TextOutA(curpos.x,curpos.y,"Compound Name");
			curpos.x+=widthCompoundName+10;
			pDC->TextOutA(curpos.x,curpos.y,"Prediction");
			curpos.x+=widthClassName+10;
			pDC->TextOutA(curpos.x,curpos.y,"Raw Value");
			curpos.x=initialoffsetx+curCol*(80+widthCompoundName+widthYhat+widthClassName+widthID);
			curpos.y+=20;
		}
		if(flag)
		{
			pDC->FillSolidRect(curpos.x-10,curpos.y-2,50+widthCompoundName+widthYhat+widthClassName+widthID,20,RGB(186,228,255));
			pDC->SetTextColor(RGB(5,5,55));
		}
		else
		{
			pDC->FillSolidRect(curpos.x-10,curpos.y-2,50+widthCompoundName+widthYhat+widthClassName+widthID,20,RGB(220,240,255));
			pDC->SetTextColor(RGB(5,5,25));
		}
		if(pCompound->bSelect)
			pDC->Draw3dRect(curpos.x-10,curpos.y-2,50+widthCompoundName+widthYhat+widthClassName+widthID,20,RGB(220,0,0),RGB(186,0,0));
		tmpstr.Format("%d",pCompound->id);
		pDC->TextOutA(curpos.x,curpos.y,tmpstr);
		curpos.x+=widthID+10;
		yhat=pCompound->yhat;
		integer=abs((int)yhat);
		decimal=abs(yhat)*pow(10.0,(double)nDecimal)-integer*pow(10.0,(double)nDecimal);
		tmpstr.Format("%d.",integer);
		decimalstr.Format("%d",decimal);
		for(i=0;i<nDecimal-decimalstr.GetLength();i++)
			tmpstr=tmpstr+"0";
		if(yhat<0)
			tmpstr=CString("-")+tmpstr;
		tmpstr=tmpstr+decimalstr;
		pDC->TextOutA(curpos.x,curpos.y,pCompound->strCompoundName);
		curpos.x+=widthCompoundName+10;
		if(yhat>abs(pDoc->fRejectBoundary))
			pDC->TextOutA(curpos.x,curpos.y,pDoc->strPositiveSet);
		if(yhat<abs(pDoc->fRejectBoundary)*(-1))
			pDC->TextOutA(curpos.x,curpos.y,pDoc->strNegativeSet);
		if(yhat<=abs(pDoc->fRejectBoundary)&&yhat>=abs(pDoc->fRejectBoundary)*(-1))
			pDC->TextOutA(curpos.x,curpos.y,"Unknown");
		curpos.x+=widthClassName+10;
		pDC->TextOutA(curpos.x,curpos.y,tmpstr);
		curpos.y+=20;
		curpos.x=initialoffsetx+curCol*(80+widthCompoundName+widthYhat+widthClassName+widthID);
		if((curpos.y-initialoffsety)/20>nItemPage)
		{
			curpos.y=initialoffsety;
			curCol++;
			curpos.x=initialoffsetx+curCol*(80+widthCompoundName+widthYhat+widthClassName+widthID);
		}
	}
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = curpos.x;
	sizeTotal.cy = 50+nItemPage*20;
	SetScrollSizes(MM_TEXT, sizeTotal);
	//wnd_navi.SetWindowPos(this,1,1,200,21,SWP_NOSIZE);
}

void CPredView::OnFilePrint()
{
	CDC dc;
	CPrintDialog printDlg(false);
	if(printDlg.DoModal()==IDCANCEL)
		return;
	dc.Attach(printDlg.GetPrinterDC());
	dc.m_bPrinting=true;

	DOCINFO di;
	::ZeroMemory(&di,sizeof(DOCINFO));
	di.cbSize=sizeof(DOCINFO);
	di.lpszDocName="FCC Prediction";
	

	bool bPrintingOK=dc.StartDocA(&di);
	CPrintInfo info;
	info.m_rectDraw.SetRect(0,0,dc.GetDeviceCaps(HORZRES),dc.GetDeviceCaps(VERTRES));
	
	
	//OnPreparePrinting(&info);
	OnBeginPrinting(&dc,&info);
	OnPrint(&dc,&info);
	OnEndPrinting(&dc,&info);
	
	if(bPrintingOK)
		dc.EndDoc();
	else
		dc.AbortDoc();
	dc.Detach();
}

BOOL CPredView::OnPreparePrinting(CPrintInfo* pInfo)
{

	// TODO:  call DoPreparePrinting to invoke the Print dialog box
	bool res;
	CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
	/*pInfo->SetMinPage(1);
	pInfo->SetMaxPage(1);*/
	res=DoPreparePrinting(pInfo);
	
	return res;
	//return CScrollView::OnPreparePrinting(pInfo);
	//!!!!Don't call the CScrollViewCScrollView::OnPreparePrinting(pInfo);
	//It does nothing. Framework requires pInfo filled.
	
}

void CPredView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: Add your specialized code here and/or call the base class
	TRACE("Physical width %d height %d\n",pDC->GetDeviceCaps(PHYSICALWIDTH),pDC->GetDeviceCaps(PHYSICALHEIGHT));
	TRACE("Physical width %d height %d\n",pDC->GetDeviceCaps(PHYSICALOFFSETX),pDC->GetDeviceCaps(PHYSICALOFFSETY));
	TRACE("Resolution width %d height %d\n",pDC->GetDeviceCaps(LOGPIXELSX),pDC->GetDeviceCaps(LOGPIXELSY));
	CSize page((pDC->GetDeviceCaps(PHYSICALWIDTH)-pDC->GetDeviceCaps(PHYSICALOFFSETX)*2)*100/pDC->GetDeviceCaps(LOGPIXELSX),(pDC->GetDeviceCaps(PHYSICALHEIGHT)-pDC->GetDeviceCaps(PHYSICALOFFSETY)*2)*100/pDC->GetDeviceCaps(LOGPIXELSY));
	//CScrollView::OnBeginPrinting(pDC, pInfo);
	TRACE("page size in logic unit %d %d\n",page.cx,page.cy);
	//pInfo->m_rectDraw.top=0;
	//pInfo->m_rectDraw.bottom=(-1)*page.cy;
	printWidthName=0;
	printWidthID=0;
	printWidthClass=0;
	printWidthValue=0;
	printFieldMargin=10;
	printColMargin=20;
	printRowMargin=10;
	int nWidth;
	int nHeight;
	CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
	pDoc->MessageCenter(IDS_MESSAGE_STARTPRINT);
	POSITION pos;
	CString tmpstr;
	CCompound* pCompound;
	pDC->SetMapMode(MM_LOENGLISH);
	//Calculate the width of raw value column and id column... according to the data
	pos=pDoc->list_CompoundTest.GetHeadPosition();
	while(pos)
	{
		pCompound=(CCompound*)pDoc->list_CompoundTest.GetNext(pos);
		tmpstr.Format("%d",pCompound->id);
		nWidth=pDC->GetTextExtent(tmpstr).cx;
		if(nWidth>printWidthID)
			printWidthID=nWidth;
		nWidth=pDC->GetTextExtent(pCompound->strCompoundName).cx;
		if(nWidth>printWidthName)
			printWidthName=nWidth;
		tmpstr.Format("%.3f",pCompound->yhat);
		nWidth=pDC->GetTextExtent(tmpstr).cx;
		if(nWidth>printWidthValue)
			printWidthValue=nWidth;
	}
	//Calculate the width of column of prediction class
	nWidth=pDC->GetTextExtent(pDoc->strNegativeSet).cx;
	if(nWidth>printWidthClass)
		printWidthClass=nWidth;
	nWidth=pDC->GetTextExtent(pDoc->strPositiveSet).cx;
	if(nWidth>printWidthClass)
		printWidthClass=nWidth;
	nWidth=pDC->GetTextExtent("Unknown").cx;
	if(nWidth>printWidthClass)
		printWidthClass=nWidth;
	//readjust the columns according to the title
	nWidth=pDC->GetTextExtent("ID").cx;
	if(nWidth>printWidthID)
		printWidthID=nWidth;
	nWidth=pDC->GetTextExtent("Compound Name").cx;
	if(nWidth>printWidthName)
		printWidthName=nWidth;
	nWidth=pDC->GetTextExtent("Raw Value").cx;
	if(nWidth>printWidthValue)
		printWidthValue=nWidth;
	nWidth=pDC->GetTextExtent("Prediction").cx;
	if(nWidth>printWidthClass)
		printWidthClass=nWidth;
	nHeight=pDC->GetTextExtent("Raw Value").cy;
	printCol=(page.cx-printWidthName-printWidthID-printWidthClass-printWidthValue-printFieldMargin*3)/(printColMargin+printWidthName+printWidthID+printWidthClass+printWidthValue+printFieldMargin*3)+1;
	printRow=(page.cy-nHeight)/(nHeight+printRowMargin)+1;
	if(printCol<1)
		printCol=1;
	if(printRow<2)
		printRow=2;
	TRACE("Col: %d Row: %d\n",printCol,printRow);
	int nPageNum=(pDoc->list_CompoundTest.GetCount()-1)/((printRow-1)*printCol)+1;
	if(nPageNum<1)
		nPageNum=1;
	TRACE("page# %d\n",nPageNum);
	
	

}

void CPredView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: Add your specialized code here and/or call the base class

	CScrollView::OnPrepareDC(pDC, pInfo);
	if(pDC->IsPrinting())
		pDC->SetMapMode(MM_LOENGLISH);
}

void CPredView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: Add your specialized code here and/or call the base class
	int i,j;
	CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
	CCompound* pCompound;
	CString tmpstr;
	int nHeight=pDC->GetTextExtent("ID").cy;
	POSITION pos=pDoc->list_CompoundTest.GetHeadPosition();
	CPoint curpos(pInfo->m_rectDraw.left,pInfo->m_rectDraw.top);
	//int LeftMargin=(pInfo->m_rectDraw.Width()-(printWidthName+printWidthID+printWidthClass+printWidthValue+printFieldMargin*3+printColMargin)
	//	*printCol)/2;
	int LeftMargin=0;
	int pagenum=1;
	//pDC->StartDocA("FCC Printing Job");
	while(pos)
	{
		pDC->StartPage();
		pDoc->strMessage.Format("Printing page No. %d ....",pagenum++);
		pDoc->MessageCenter(IDS_MESSAGE_CUSTOM);
		for(i=0;i<printCol && pos;i++)
		{
			curpos.x=LeftMargin+pInfo->m_rectDraw.left+(printWidthName+printWidthID+printWidthClass+printWidthValue+printFieldMargin*3+printColMargin)*i;
			pDC->TextOutA(curpos.x,curpos.y,"ID");
			curpos.x+=printWidthID+printFieldMargin;
			pDC->TextOutA(curpos.x,curpos.y,"Compound Name");
			curpos.x+=printWidthName+printFieldMargin;
			pDC->TextOutA(curpos.x,curpos.y,"Prediction");
			curpos.x+=printWidthClass+printFieldMargin;
			pDC->TextOutA(curpos.x,curpos.y,"Raw Value");
			curpos.x+=printWidthValue+printFieldMargin;
			curpos.x=LeftMargin+pInfo->m_rectDraw.left+(printWidthName+printWidthID+printWidthClass+printWidthValue+printFieldMargin*3+printColMargin)*i;
			curpos.y=curpos.y-printRowMargin-nHeight;
			for(j=1;j<printRow && pos;j++)
			{
				pCompound=(CCompound*)pDoc->list_CompoundTest.GetNext(pos);
				tmpstr.Format("%d",pCompound->id);
				pDC->TextOutA(curpos.x,curpos.y,tmpstr);
				curpos.x+=printWidthID+printFieldMargin;
				pDC->TextOutA(curpos.x,curpos.y,pCompound->strCompoundName);
				curpos.x+=printWidthName+printFieldMargin;
				if(pCompound->yhat>abs(pDoc->fRejectBoundary))
					pDC->TextOutA(curpos.x,curpos.y,pDoc->strPositiveSet);
				if(pCompound->yhat<abs(pDoc->fRejectBoundary)*(-1))
					pDC->TextOutA(curpos.x,curpos.y,pDoc->strNegativeSet);
				if(pCompound->yhat>=abs(pDoc->fRejectBoundary)*(-1) && pCompound->yhat<=abs(pDoc->fRejectBoundary))
					pDC->TextOutA(curpos.x,curpos.y,"Unknown");
				curpos.x+=printWidthClass+printFieldMargin;
				tmpstr.Format("%.3f",pCompound->yhat);
				pDC->TextOutA(curpos.x,curpos.y,tmpstr);
				curpos.x=LeftMargin+pInfo->m_rectDraw.left+(printWidthName+printWidthID+printWidthClass+printWidthValue+printFieldMargin*3+printColMargin)*i;
				curpos.y=curpos.y-printRowMargin-nHeight;
			}
			curpos.y=pInfo->m_rectDraw.top;
		}
		pDC->EndPage();
		TRACE("Ending page\n");
	}
	//pDC->EndDoc();
	//CScrollView::OnPrint(pDC, pInfo);
}

void CPredView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: Add your specialized code here and/or call the base class
	CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
	pDoc->MessageCenter(IDS_MESSAGE_ENDPRINT);
	//CScrollView::OnEndPrinting(pDC, pInfo);
}

void CPredView::OnEditCopy()
{
	// TODO: Add your command handler code here
	CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
	if(!OpenClipboard())
	{
		pDoc->strMessage="Fail to open clipboard!";
		pDoc->MessageCenter(IDS_MESSAGE_CUSTOM);
		return;
	}
	EmptyClipboard();

	
	CString tmpstr;
	CString strContainer;
	POSITION pos;
	int count=0;
	int memlength=0;
	CCompound* pCompound;
	strContainer="";
	pos=pDoc->list_CompoundTest.GetHeadPosition();
	while(pos)
	{
		pCompound=(CCompound*)pDoc->list_CompoundTest.GetNext(pos);
		if(pCompound->bSelect)
		{
			tmpstr.Format("%d\t",pCompound->id);
			strContainer+=tmpstr;
			strContainer+=pCompound->strCompoundName;
			strContainer+="\t";
			if(pCompound->yhat>abs(pDoc->fRejectBoundary))
				strContainer+=pDoc->strPositiveSet;
			if(pCompound->yhat<abs(pDoc->fRejectBoundary)*(-1))
				strContainer+=pDoc->strNegativeSet;
			if(pCompound->yhat>=abs(pDoc->fRejectBoundary)*(-1) && pCompound->yhat<=abs(pDoc->fRejectBoundary))
				strContainer+="Unknown";
			tmpstr.Format("\t%.3f\n",pCompound->yhat);
			strContainer+=tmpstr;
			count++;
			memlength+=pCompound->strCompoundName.GetLength()*sizeof(char);
		}
	}
	HGLOBAL hglbCopy;
	LPTSTR  lptstrCopy;
	hglbCopy=::GlobalAlloc(GMEM_MOVEABLE,(strContainer.GetLength()+1)*sizeof(char));
	if(hglbCopy==NULL)
	{
		CloseClipboard();
		pDoc->MessageCenter(IDS_MESSAGE_MEMORY);
		return;
	}
	lptstrCopy=(LPTSTR)::GlobalLock(hglbCopy);
	memcpy(lptstrCopy,(LPCTSTR)strContainer,strContainer.GetLength()*sizeof(char));
	lptstrCopy[strContainer.GetLength()*sizeof(char)]=0;
	TRACE("%s",lptstrCopy);
	::GlobalUnlock(hglbCopy);
	SetClipboardData(CF_TEXT, hglbCopy);

	if(count>0)
	{
		UINT uFormat=RegisterClipboardFormat("FCC");
		memlength+=sizeof(int)*2+sizeof(int)*count+sizeof(bool)*pDoc->nFpLen*count;
		HGLOBAL hglbCustmCopy;
		hglbCustmCopy=::GlobalAlloc(GMEM_MOVEABLE,memlength);
		if(hglbCustmCopy==NULL)
		{
			CloseClipboard();
			return;
		}
		char* pData;
		pData=(char*)GlobalLock(hglbCustmCopy);
		//Clipboard serialization procedure
		//Fingerprint length-compound number-length of each compound name-compound name in string-fingerprints-next compound...
		int memoffset=0;
		int strlenCmpName;
		memcpy(pData,&(pDoc->nFpLen),sizeof(int));
		memoffset+=sizeof(int);
		memcpy(pData+memoffset,&count,sizeof(int));
		memoffset+=sizeof(int);
		pos=pDoc->list_CompoundTest.GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)pDoc->list_CompoundTest.GetNext(pos);
			if(pCompound->bSelect)
			{
				strlenCmpName=pCompound->strCompoundName.GetLength();
				memcpy(pData+memoffset,&strlenCmpName,sizeof(int));
				memoffset+=sizeof(int);
				memcpy(pData+memoffset,(LPCTSTR)pCompound->strCompoundName,sizeof(char)*strlenCmpName);
				memoffset+=sizeof(char)*strlenCmpName;
				memcpy(pData+memoffset,(LPCTSTR)pCompound->pFingerprint,sizeof(bool)*pDoc->nFpLen);
				memoffset+=sizeof(bool)*pDoc->nFpLen;
			}
		}
		GlobalUnlock(hglbCustmCopy);
		SetClipboardData(uFormat,hglbCustmCopy);
		if(count>1)
			pDoc->strMessage.Format("%d items are copied into clipboard. You can paste them into FCC or other text editors.",count);
		else
			pDoc->strMessage.Format("%d item is copied into clipboard. You can paste it into FCC or other text editors.",count);
		pDoc->MessageCenter(IDS_MESSAGE_COPY);
	}
	else
	{
		pDoc->MessageCenter(IDS_MESSAGE_NOITEMSELECTED);
	}
	CloseClipboard();

}

void CPredView::OnEditCut()
{
	// TODO: Add your command handler code here
	OnEditCopy();
	POSITION pos,posa;
	bool flag=false;
	CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
	CCompound* pCompound;
	pos=pDoc->list_CompoundTest.GetHeadPosition();
	while(pos)
	{
		pCompound=(CCompound*)pDoc->list_CompoundTest.GetAt(pos);
		if(pCompound->bSelect)
		{
			posa=pos;
			pDoc->list_CompoundTest.GetNext(pos);
			pDoc->list_CompoundTest.RemoveAt(posa);
			delete pCompound;
			flag=true;
			continue;
		}
		pDoc->list_CompoundTest.GetNext(pos);
	}
	if(flag)
	{
		OnUpdate(0,0,0);
		Invalidate();
		pDoc->SetModifiedFlag(true);
	}
	else
	{
		//pDoc->strMessage="No items selected.";
		pDoc->MessageCenter(IDS_MESSAGE_NOITEMSELECTED);
	}
}

void CPredView::OnEditPaste()
{
	// TODO: Add your command handler code here
	CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
	UINT uFormat=RegisterClipboardFormat("FCC");
	if(!IsClipboardFormatAvailable(uFormat))
		return;
	if(!OpenClipboard())
	{
		pDoc->strMessage="Fail to open clipboard!";
		pDoc->MessageCenter(IDS_MESSAGE_CUSTOM);
		return;
	}
	int count;
	int nFpLen;
	int i,strlenCmpName;
	int memoffset=0;
	int id=0;
	
	CCompound* pCompound;
	HGLOBAL hglbPaste;
	hglbPaste=GetClipboardData(uFormat);
	char* pData=(char*)GlobalLock(hglbPaste);
	memcpy(&nFpLen,pData+memoffset,sizeof(int));
	memoffset+=sizeof(int);
	memcpy(&count,pData+memoffset,sizeof(int));
	memoffset+=sizeof(int);
	if(nFpLen!=pDoc->nFpLen)
	{
		pDoc->MessageCenter(IDS_MESSAGE_INCONSISTENTLENGTH);
		return;
	}
	//Add Error Processing Function if The pasted fingerprint length and current project do not match
	POSITION pos=pDoc->list_CompoundTest.GetHeadPosition();
	while(pos)
	{
		pCompound=(CCompound*)pDoc->list_CompoundTest.GetNext(pos);
		if(pCompound->id>id)
			id=pCompound->id;
	}
	for(i=0;i<count;i++)
	{
		pCompound=new CCompound;
		memcpy(&strlenCmpName,pData+memoffset,sizeof(int));
		memoffset+=sizeof(int);
		char* pTmpStr=(char*)malloc(sizeof(char)*(strlenCmpName+1));
		memcpy(pTmpStr,pData+memoffset,sizeof(char)*strlenCmpName);
		memoffset+=sizeof(char)*strlenCmpName;
		pTmpStr[strlenCmpName]=NULL;
		pCompound->strCompoundName=pTmpStr;
		pCompound->id=++id;
		pCompound->pFingerprint=(bool*)malloc(sizeof(bool)*pDoc->nFpLen);
		memcpy(pCompound->pFingerprint,pData+memoffset,sizeof(char)*pDoc->nFpLen);
		memoffset+=sizeof(bool)*pDoc->nFpLen;
		delete pTmpStr;
		pDoc->list_CompoundTest.AddTail(pCompound);
	}
	GlobalUnlock(hglbPaste);
	CloseClipboard();
	if(count>0)
	{
		OnUpdate(NULL,0,0);
		Invalidate();
		pDoc->SetModifiedFlag(true);
		if(count>1)
			pDoc->strMessage.Format("%d items are pasted into testing data set.",count);
		else
			pDoc->strMessage.Format("%d item is pasted into testing data set.",count);
		pDoc->MessageCenter(IDS_MESSAGE_PASTE);
	}

}

void CPredView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	UINT uFormat=RegisterClipboardFormat("FCC");
	pCmdUI->Enable(IsClipboardFormatAvailable(uFormat));
}

void CPredView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
	wnd_navi.SetWindowPos(&CWnd::wndTop,1,1,0,0,SWP_NOSIZE|SWP_SHOWWINDOW);
}

void CPredView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
	wnd_navi.SetWindowPos(&CWnd::wndTop,1,1,0,0,SWP_NOSIZE|SWP_SHOWWINDOW);
}

LRESULT CPredView::OnClickNavi(WPARAM wParam, LPARAM lParam)
{
	TRACE("%d\n",wParam);
	if(wParam>=1 && wParam<=4)
		Invalidate();
	if(wParam==5)
	{
		if(wnd_navi.bDropDown==false)
			wnd_navi.SetWindowPos(&this->wndTop,0,0,200,21,SWP_NOMOVE);
		else
			wnd_navi.SetWindowPos(&this->wndTop,0,0,200,42,SWP_NOMOVE);
	}
	if(wParam==6)
	{
		CString tmpstr;
		CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
		wnd_navi.m_nCol.GetWindowTextA(tmpstr);
		nCol=atoi(tmpstr);
		if(nCol<1)
			nCol=1;
		wnd_navi.m_nRow.GetWindowTextA(tmpstr);
		nItemPage=atoi(tmpstr);
		if(nItemPage<10)
			nItemPage=10;
		wnd_navi.nCurPage=1;
		wnd_navi.nTotalPage=pDoc->list_CompoundTest.GetCount()/(nCol*nItemPage)+1;
		wnd_navi.Invalidate();
		tmpstr.Format("%d",nItemPage);
		wnd_navi.m_nRow.SetWindowTextA(tmpstr);
		tmpstr.Format("%d",nCol);
		wnd_navi.m_nCol.SetWindowTextA(tmpstr);
		Invalidate();	
	}
	return LRESULT();
}

void CPredView::OnEditFind()
{
	dlg.DestroyWindow();
	dlg.Create(IDD_FINDDIALOG,this);
	dlg.ShowWindow(SW_SHOW);
}

BOOL CPredView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default
	//return CScrollView::OnSetCursor(pWnd, nHitTest, message);
	return false;
}


void CPredView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CScrollView::OnMouseMove(nFlags, point);
	int i;
	CClientDC dc(this);
	OnPrepareDC(&dc);
	POINT m_point;
	CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
	m_point.x=point.x;
	m_point.y=point.y;
	dc.DPtoLP(&m_point);
	CRgn rgn;
	bHitCaption=false;
	for(i=0;i<nCol;i++)
	{
		
		if(wnd_navi.nCurPage<wnd_navi.nTotalPage || (wnd_navi.nCurPage-1)*nCol+i+1 <= (pDoc->list_CompoundTest.GetCount()-1)/nItemPage+1)
		{
			
			CPoint topleft(60+i*(80+widthCompoundName+widthYhat+widthClassName+widthID),30);
			CPoint bottomright(60+i*(80+widthCompoundName+widthYhat+widthClassName+widthID)+widthID,50);
			rgn.CreateRectRgn(topleft.x,topleft.y,bottomright.x,bottomright.y);
			if(rgn.PtInRegion(m_point))
			{
				//pDoc->MergeSort(SORT_BY_ID,SortAscend,pDoc->list_CompoundTest);
				bHitCaption=true;
			}
			rgn.DeleteObject();
			topleft.SetPoint(60+i*(80+widthCompoundName+widthYhat+widthClassName+widthID)+widthID+10,30);
			bottomright.SetPoint(60+i*(80+widthCompoundName+widthYhat+widthClassName+widthID)+widthID+widthCompoundName+10,50);
			rgn.CreateRectRgn(topleft.x,topleft.y,bottomright.x,bottomright.y);
			if(rgn.PtInRegion(m_point))
			{
				//pDoc->MergeSort(SORT_BY_NAME,SortAscend,pDoc->list_CompoundTest);
				bHitCaption=true;
			}
			rgn.DeleteObject();
			topleft.SetPoint(60+i*(80+widthCompoundName+widthYhat+widthClassName+widthID)+widthID+widthCompoundName+20,30);
			bottomright.SetPoint(60+i*(80+widthCompoundName+widthYhat+widthClassName+widthID)+widthID+widthCompoundName+widthClassName+widthYhat+30,50);
			rgn.CreateRectRgn(topleft.x,topleft.y,bottomright.x,bottomright.y);
			if(rgn.PtInRegion(m_point))
			{
				//pDoc->MergeSort(SORT_BY_VALUE,SortAscend,pDoc->list_CompoundTest);
				bHitCaption=true;
			}
			rgn.DeleteObject();
		}
	}
	if(bHitCaption)
	{
		if(bOldMouseInCaption==false && pDoc->list_CompoundTest.GetCount()>0)
		{
			if(SortAscend)
				::SetCursor(cursorAscend);
			else
				::SetCursor(cursorDescend);
			bOldMouseInCaption=true;
			//TRACE("hit caption change cursor\n");
		}
	}
	else
	{
		//if(bOldMouseInCaption==true)
		{
			::SetCursor(::LoadCursor(NULL,IDC_ARROW));
			bOldMouseInCaption=false;
			//TRACE("not hit caption change cursor\n");
		}
		
	}

	// TODO: Add your message handler code here
}


void CPredView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CMenu m_menu;
	CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
	CPoint mPoint=point;
	ClientToScreen(&mPoint);
	UINT uFormat=RegisterClipboardFormat("FCC");
	bool flag=IsClipboardFormatAvailable(uFormat);
	m_menu.LoadMenuA(IDR_FCCVIEWMENU);
	if(!pDoc->list_CompoundTest.GetCount()>0)
	{
		m_menu.GetSubMenu(1)->EnableMenuItem(ID_EDIT_SELECTALL,MF_GRAYED);
		m_menu.GetSubMenu(1)->EnableMenuItem(ID_CPREDVIEW_INVERSESELECTION,MF_GRAYED);
		m_menu.GetSubMenu(1)->EnableMenuItem(ID_EDIT_COPY,MF_GRAYED);
		m_menu.GetSubMenu(1)->EnableMenuItem(ID_EDIT_CUT,MF_GRAYED);
		m_menu.GetSubMenu(1)->EnableMenuItem(ID_EDIT_FIND,MF_GRAYED);
	}
	if(!flag)
		m_menu.GetSubMenu(1)->EnableMenuItem(ID_EDIT_PASTE,MF_GRAYED);
	m_menu.GetSubMenu(1)->TrackPopupMenu(TPM_LEFTALIGN,mPoint.x,mPoint.y,this);
	CScrollView::OnRButtonDown(nFlags, point);
}

void CPredView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	/*HCURSOR hCursor=AfxGetApp()->LoadCursorA(IDC_DESCEND);
	if(hCursor==NULL)
		TRACE("Invaidate cursor\n");
	SetCursor(hCursor);*/
	TRACE("%d\n",nFlags);
	CClientDC dc(this);
	OnPrepareDC(&dc);
	POINT m_point;
	CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
	m_point.x=point.x;
	m_point.y=point.y;
	dc.DPtoLP(&m_point);
	CRgn rgn;
	int i,j;
	int selCol=-1;
	int selRow=-1;
	for(i=0;i<nCol;i++)
	{
		bool bHitCaption=false;
		if(wnd_navi.nCurPage<wnd_navi.nTotalPage || (wnd_navi.nCurPage-1)*nCol+i+1 <= (pDoc->list_CompoundTest.GetCount()-1)/nItemPage+1)
		{
			
			CPoint topleft(60+i*(80+widthCompoundName+widthYhat+widthClassName+widthID),30);
			CPoint bottomright(60+i*(80+widthCompoundName+widthYhat+widthClassName+widthID)+widthID,50);
			rgn.CreateRectRgn(topleft.x,topleft.y,bottomright.x,bottomright.y);
			if(rgn.PtInRegion(m_point))
			{
				pDoc->MergeSort(SORT_BY_ID,SortAscend,pDoc->list_CompoundTest);
				bHitCaption=true;
			}
			rgn.DeleteObject();
			topleft.SetPoint(60+i*(80+widthCompoundName+widthYhat+widthClassName+widthID)+widthID+10,30);
			bottomright.SetPoint(60+i*(80+widthCompoundName+widthYhat+widthClassName+widthID)+widthID+widthCompoundName+10,50);
			rgn.CreateRectRgn(topleft.x,topleft.y,bottomright.x,bottomright.y);
			if(rgn.PtInRegion(m_point))
			{
				pDoc->MergeSort(SORT_BY_NAME,SortAscend,pDoc->list_CompoundTest);
				bHitCaption=true;
			}
			rgn.DeleteObject();
			topleft.SetPoint(60+i*(80+widthCompoundName+widthYhat+widthClassName+widthID)+widthID+widthCompoundName+20,30);
			bottomright.SetPoint(60+i*(80+widthCompoundName+widthYhat+widthClassName+widthID)+widthID+widthCompoundName+widthClassName+widthYhat+30,50);
			rgn.CreateRectRgn(topleft.x,topleft.y,bottomright.x,bottomright.y);
			if(rgn.PtInRegion(m_point))
			{
				pDoc->MergeSort(SORT_BY_VALUE,SortAscend,pDoc->list_CompoundTest);
				bHitCaption=true;
			}
			rgn.DeleteObject();
		}
		if(bHitCaption)
		{
			TRACE("LButton down caption\n");
			Invalidate();
			UpdateWindow();
			SortAscend=1-SortAscend;
			if(SortAscend)
				::SetCursor(cursorAscend);
			else
				::SetCursor(cursorDescend);
			bOldMouseInCaption=true;
			
		}
		for(j=0;j<nItemPage;j++)
		{
			rgn.CreateRectRgn(50+i*(80+widthCompoundName+widthYhat+widthClassName+widthID),50+20*j,
				20+(i+1)*(80+widthCompoundName+widthYhat+widthClassName+widthID),50+20*(j+1));
			if(rgn.PtInRegion(m_point))
			{
				selCol=i;
				selRow=j;
			}
			rgn.DeleteObject();
		}
	}
	if(selCol>=0 && selRow>=0)
	{
		dlg.nIndex=(wnd_navi.nCurPage-1)*(nCol*nItemPage)+selCol*nItemPage+selRow+1;
		if(nFlags&MK_CONTROL)
			SendMessage(WM_FINDCOMPOUND,GOTOLINE,2);
		else
			SendMessage(WM_FINDCOMPOUND,GOTOLINE,1);
	}
	else
	{
		if(nFlags!=MK_CONTROL)
		{
			POSITION pos;
			pos=pDoc->list_CompoundTest.GetHeadPosition();
			while(pos)
			{
				pCompound=(CCompound*)pDoc->list_CompoundTest.GetNext(pos);
				((CCompound*)pCompound)->bSelect=false;
			}
		}
		Invalidate(0);
	}
	CScrollView::OnLButtonDown(nFlags, point);
}

LRESULT CPredView::OnFindCompound(WPARAM wParam, LPARAM lParam)
{
	//lParam=0 means the message is sent from "Find" dialog lParam=1 means the message is sent from LButtonDown without CTRL pressed. 
	//lParam=2 means the message is sent from LBUttonDown and CTRL is pressed 
	CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
	if(pDoc->list_CompoundTest.IsEmpty())
		return LRESULT();
	bool flag=false;
	if(wParam==FINDNEXT)
	{
		POSITION pos;
		CCompound* pTmpCompound=NULL;
		bool bFound=false;
		pos=pDoc->list_CompoundTest.GetTailPosition();
		while(pos)
		{
			pTmpCompound=(CCompound*)pDoc->list_CompoundTest.GetAt(pos);
			if(pTmpCompound->bSelect)
				break;
			pDoc->list_CompoundTest.GetPrev(pos);
		}
		if(pos)
			pDoc->list_CompoundTest.GetNext(pos);
		if(!pos)
			pos=pDoc->list_CompoundTest.GetHeadPosition();
		while(pos)
		{
			pTmpCompound=(CCompound*)pDoc->list_CompoundTest.GetAt(pos);
			if(pTmpCompound->strCompoundName.Find(dlg.strMessage)>=0)
			{
				bFound=true;
				break;
			}
			pDoc->list_CompoundTest.GetNext(pos);
		}
		if(bFound)
		{
			int index=1;
			POSITION posa=pDoc->list_CompoundTest.GetHeadPosition();
			while(posa)
			{
				if(posa==pos)
				{
					break;
				}
				pDoc->list_CompoundTest.GetNext(posa);
				index++;
			}
			dlg.nIndex=index;
			flag=true;
			pDoc->strMessage="Matching item is found.";
			pDoc->MessageCenter(IDS_MESSAGE_FINDCOMPOUND);
		}
		else
		{
			pDoc->strMessage="No matching item is found.";
			pDoc->MessageCenter(IDS_MESSAGE_FINDCOMPOUND);
		}
	}
	if(wParam==FINDPREV)
	{
		POSITION pos;
		CCompound* pTmpCompound=NULL;
		bool bFound=false;
		pos=pDoc->list_CompoundTest.GetHeadPosition();
		while(pos)
		{
			pTmpCompound=(CCompound*)pDoc->list_CompoundTest.GetAt(pos);
			if(pTmpCompound->bSelect)
				break;
			pDoc->list_CompoundTest.GetNext(pos);
		}
		if(pos)
			pDoc->list_CompoundTest.GetPrev(pos);
		if(!pos)
			pos=pDoc->list_CompoundTest.GetHeadPosition();
		while(pos)
		{
			pTmpCompound=(CCompound*)pDoc->list_CompoundTest.GetAt(pos);
			if(pTmpCompound->strCompoundName.Find(dlg.strMessage)>=0)
			{
				bFound=true;
				break;
			}
			pDoc->list_CompoundTest.GetPrev(pos);
		}
		if(bFound)
		{
			int index=1;
			POSITION posa=pDoc->list_CompoundTest.GetHeadPosition();
			while(posa)
			{
				if(posa==pos)
				{
					break;
				}
				pDoc->list_CompoundTest.GetNext(posa);
				index++;
			}
			dlg.nIndex=index;
			flag=true;
			pDoc->strMessage="Matching item is found.";
			pDoc->MessageCenter(IDS_MESSAGE_FINDCOMPOUND);
		}
		else
		{
			pDoc->strMessage="No matching item is found.";
			pDoc->MessageCenter(IDS_MESSAGE_FINDCOMPOUND);
		}
	}
	if(wParam==GOTOPAGE)
	{
		if(dlg.nIndex<1)
			dlg.nIndex=1;
		if(dlg.nIndex>wnd_navi.nTotalPage)
			dlg.nIndex=wnd_navi.nTotalPage;
		wnd_navi.nCurPage=dlg.nIndex;
		wnd_navi.Invalidate();
		Invalidate();
		pDoc->strMessage.Format("Page No. %d",dlg.nIndex);
		pDoc->MessageCenter(IDS_MESSAGE_FINDCOMPOUND);
	}
	if(wParam==GOTOLINE || flag)
	{
		int curCol;
		int curRow;
		int subLine;
		POSITION pos;
		int nTotalLine=pDoc->list_CompoundTest.GetCount();
		if(dlg.nIndex<1)
			dlg.nIndex=1;
		if(dlg.nIndex>nTotalLine)
			dlg.nIndex=nTotalLine;
		wnd_navi.nCurPage=(dlg.nIndex-1)/(nCol*nItemPage)+1;
		subLine=dlg.nIndex-(wnd_navi.nCurPage-1)*(nCol*nItemPage);
		curCol=(subLine-1)/nItemPage+1;
		curRow=subLine-(curCol-1)*nItemPage;
		if(lParam==0)
		{
			ScrollToPosition(CPoint((curCol-1)*(80+widthCompoundName+widthYhat+widthClassName+widthID),(curRow-1)*20));
			OnHScroll(SB_LINELEFT,0,0);
			OnHScroll(SB_LINERIGHT,0,0);
			OnVScroll(SB_LINEUP,0,0);
			OnVScroll(SB_LINEDOWN,0,0);
			wnd_navi.SetWindowPos(&CWnd::wndTop,1,1,0,0,SWP_NOSIZE|SWP_SHOWWINDOW);
			wnd_navi.Invalidate();
			pDoc->strMessage.Format("Line No. %d",dlg.nIndex);
			pDoc->MessageCenter(IDS_MESSAGE_FINDCOMPOUND);
		}
		if(lParam==0 || lParam==1)
		{
			pos=pDoc->list_CompoundTest.GetHeadPosition();
			while(pos)
			{
				pCompound=pDoc->list_CompoundTest.GetNext(pos);
				((CCompound*)pCompound)->bSelect=false;
			}
		}
		/*pos=pDoc->list_CompoundTest.Find(pCompound);
		if(pos)
			((CCompound*)pCompound)->bSelect=false;*/
		pos=pDoc->list_CompoundTest.FindIndex(dlg.nIndex-1);
		pCompound=pDoc->list_CompoundTest.GetAt(pos);
		((CCompound*)pCompound)->bSelect=true;
		if(wnd_navi.nCurPage==wnd_navi.nTotalPage)
			Invalidate(1);
		else
			Invalidate(0);
	}
	
	return LRESULT();
}

void CPredView::OnUpdateEditFind(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
	pCmdUI->Enable(pDoc->list_CompoundTest.GetCount()>0);
}

void CPredView::OnEditSelectall()
{
	// TODO: Add your command handler code here
	CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
	POSITION pos=pDoc->list_CompoundTest.GetHeadPosition();
	CCompound* pCompound;
	while(pos)
	{
		pCompound=(CCompound*)pDoc->list_CompoundTest.GetNext(pos);
		pCompound->bSelect=true;
	}
	Invalidate();
}

void CPredView::OnUpdateEditSelectall(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
	pCmdUI->Enable(pDoc->list_CompoundTest.GetCount()>0);
}

void CPredView::OnCpredviewInverseselection()
{
	// TODO: Add your command handler code here
	CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
	POSITION pos=pDoc->list_CompoundTest.GetHeadPosition();
	CCompound* pCompound;
	while(pos)
	{
		pCompound=(CCompound*)pDoc->list_CompoundTest.GetNext(pos);
		pCompound->bSelect=!pCompound->bSelect;
	}
	Invalidate();
}

void CPredView::OnUpdateCpredviewInverseselection(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CFCCDoc* pDoc=(CFCCDoc*)GetDocument();
	pCmdUI->Enable(pDoc->list_CompoundTest.GetCount()>0);
}
// CPredView diagnostics

#ifdef _DEBUG
void CPredView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CPredView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG

// CPredView message handlers
// PredView.cpp : implementation file
//

#include "stdafx.h"
#include "FCC.h"
#include "PredView.h"


// CNaviWnd

IMPLEMENT_DYNAMIC(CNaviWnd, CWnd)

CNaviWnd::CNaviWnd()
{
	edgePen.CreatePen(PS_SOLID,1,RGB(200,200,255));
	bkBrush.CreateSolidBrush(RGB(218,234,255));
	bitmap_LeftMost.LoadBitmapA(IDB_LEFTMOST);
	bitmap_Left.LoadBitmapA(IDB_LEFT);
	bitmap_Right.LoadBitmapA(IDB_RIGHT);
	bitmap_RightMost.LoadBitmapA(IDB_RIGHTMOST);
	bitmap_DropDown.LoadBitmapA(IDB_DROPDOWN);
	bitmap_ScrollUp.LoadBitmapA(IDB_SCROLLUP);
	bitmap_Check.LoadBitmapA(IDB_CHECK);
	font.CreateFontA(14,0,0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,"Arial");

	posflag=0;
	LButtonDown=false;
	bDropDown=false;
}

CNaviWnd::~CNaviWnd()
{
	edgePen.DeleteObject();
	bkBrush.DeleteObject();
	bitmap_LeftMost.DeleteObject();
	bitmap_Left.DeleteObject();
	bitmap_Right.DeleteObject();
	bitmap_RightMost.DeleteObject();
	bitmap_DropDown.DeleteObject();
	bitmap_ScrollUp.DeleteObject();
	bitmap_Check.DeleteObject();
	font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CNaviWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CNaviWnd message handlers



void CNaviWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
	CRect rect;
	GetClientRect(rect);
	CBrush* pOldBrush=dc.SelectObject(&bkBrush);
	CPen* pOldPen=dc.SelectObject(&edgePen);
	
	dc.Rectangle(rect);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap* pOldBitmap=memDC.SelectObject(&bitmap_LeftMost);
	dc.BitBlt(2,2,16,16,&memDC,0,0,SRCCOPY);
	if(posflag==1 && !LButtonDown)//draw left-most button
		dc.Draw3dRect(1,1,18,18,RGB(255,255,255),RGB(158,174,245));
	if(posflag==1 && LButtonDown)//draw left-most button
		dc.Draw3dRect(1,1,18,18,RGB(158,174,245),RGB(255,255,255));

	memDC.SelectObject(&bitmap_Left);
	dc.BitBlt(19,2,16,16,&memDC,0,0,SRCCOPY);
	if(posflag==2 && !LButtonDown)// draw left button
		dc.Draw3dRect(18,1,18,18,RGB(255,255,255),RGB(158,174,245));
	if(posflag==2 && LButtonDown)// draw left button
		dc.Draw3dRect(18,1,18,18,RGB(158,174,245),RGB(255,255,255));

	memDC.SelectObject(&bitmap_Right);
	dc.BitBlt(36,2,16,16,&memDC,0,0,SRCCOPY);
	if(posflag==3 && !LButtonDown)// draw right button
		dc.Draw3dRect(35,1,18,18,RGB(255,255,255),RGB(158,174,245));
	if(posflag==3 && LButtonDown)// draw right button
		dc.Draw3dRect(35,1,18,18,RGB(158,174,245),RGB(255,255,255));

	memDC.SelectObject(&bitmap_RightMost);
	dc.BitBlt(53,2,16,16,&memDC,0,0,SRCCOPY);
	if(posflag==4&& !LButtonDown)// draw right-most button
		dc.Draw3dRect(52,1,18,18,RGB(255,255,255),RGB(158,174,245));
	if(posflag==4&& LButtonDown)// draw right-most button
		dc.Draw3dRect(52,1,18,18,RGB(158,174,245),RGB(255,255,255));
	
	if(bDropDown==false)
		memDC.SelectObject(&bitmap_DropDown);
	else
		memDC.SelectObject(&bitmap_ScrollUp);
	dc.BitBlt(180,8,15,10,&memDC,0,0,SRCCOPY);
	if(posflag==5&& !LButtonDown)//draw drop-down button
		dc.Draw3dRect(179,7,16,11,RGB(255,255,255),RGB(158,174,245));
	if(posflag==5&& LButtonDown)//draw drop-down button
		dc.Draw3dRect(179,7,16,11,RGB(158,174,245),RGB(255,255,255));
	//dc.StretchBlt(1,1,18,18,&memDC,0,0,100,73,SRCCOPY);
	dc.SetBkMode(TRANSPARENT);
	CFont* pOldFont=dc.SelectObject(&font);
	dc.TextOutA(2,23,"Rows");
	dc.TextOutA(102,23,"Cols");
	CString tmpstr;
	tmpstr.Format("%d / %d",nCurPage,nTotalPage);
	dc.TextOutA(100,4,tmpstr);
	memDC.SelectObject(&bitmap_Check);
	dc.BitBlt(178,20,20,20,&memDC,0,0,SRCCOPY);
	if(posflag==6&& !LButtonDown)// draw right-most button
		dc.Draw3dRect(178,20,20,20,RGB(255,255,255),RGB(158,174,245));
	if(posflag==6&& LButtonDown)// draw right-most button
		dc.Draw3dRect(178,20,20,20,RGB(158,174,245),RGB(255,255,255));

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();

	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldFont);
}

int CNaviWnd::MouseTest(int x, int y)
{
	int res=0;
	CRect rect;
	CRgn rgn;
	for(int i=1;i<5;i++)
	{
		rect.SetRect(2+(i-1)*17,2,2+i*17,18);
		rgn.CreateRectRgnIndirect(&rect);
		if(rgn.PtInRegion(x,y))
			res=i;
		rgn.DeleteObject();
	}
	rect.SetRect(180,8,195,18);
	rgn.CreateRectRgnIndirect(&rect);
		if(rgn.PtInRegion(x,y))
			res=5;
	rect.SetRect(178,20,1985,40);
	rgn.DeleteObject();
	rgn.CreateRectRgnIndirect(&rect);
		if(rgn.PtInRegion(x,y))
			res=6;
	return res;
}

void CNaviWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//TRACE("%d\n",MouseTest(point.x,point.y));
	CRect rect;
	GetClientRect(rect);
	if(posflag>0 && posflag<5)
		InvalidateRect(CRect((posflag-1)*18-2,1,posflag*18+1,20),0);
	if(posflag==5)
		InvalidateRect(CRect(179,1,200,20),0);
	if(posflag==6)
		InvalidateRect(CRect(177,19,200,41),0);
	posflag=MouseTest(point.x,point.y);
	if(posflag>0 && posflag<5)
		InvalidateRect(CRect((posflag-1)*18-2,1,posflag*18+1,20),0);
	if(posflag==5)
		InvalidateRect(CRect(179,1,200,20),0);
	if(posflag==6)
		InvalidateRect(CRect(177,19,200,41),0);
	CWnd::OnMouseMove(nFlags, point);
}

void CNaviWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	SetCapture();
	LButtonDown=true;
	CRect rect;
	GetClientRect(rect);
	if(posflag>0 && posflag<5)
		InvalidateRect(CRect((posflag-1)*18-2,1,posflag*18+1,20),0);
	if(posflag==5)
		InvalidateRect(CRect(179,1,200,20),0);
	if(posflag==6)
		InvalidateRect(CRect(177,19,200,41),0);
	posflag=MouseTest(point.x,point.y);
	if(posflag>0 && posflag<5)
		InvalidateRect(CRect((posflag-1)*18-2,1,posflag*18+1,20),0);
	if(posflag==5)
		InvalidateRect(CRect(179,1,200,20),0);
	if(posflag==6)
		InvalidateRect(CRect(177,19,200,41),0);
	CWnd::OnLButtonDown(nFlags, point);
}

void CNaviWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	ReleaseCapture();
	if(LButtonDown)
	{
		LButtonDown=false;
		CRect rect;
		GetClientRect(rect);
		if(posflag>0 && posflag<5)
			InvalidateRect(CRect((posflag-1)*18-2,1,posflag*18+1,20),0);
		if(posflag==5)
			InvalidateRect(CRect(179,1,200,20),0);
		if(posflag==6)
			InvalidateRect(CRect(177,19,200,41),0);
		posflag=MouseTest(point.x,point.y);
		if(posflag>0 && posflag<5)
			InvalidateRect(CRect((posflag-1)*18-2,1,posflag*18+1,20),0);
		if(posflag==5)
		{
			InvalidateRect(CRect(179,1,200,20),0);
			bDropDown=!bDropDown;
		}
		if(posflag==6)
			InvalidateRect(CRect(177,19,200,41),0);
		if(posflag==1)
			this->nCurPage=1;
		if(posflag==2)
			this->nCurPage--;
		if(nCurPage<1)
			nCurPage=1;
		if(posflag==3)
			this->nCurPage++;
		if(nCurPage>nTotalPage)
			nCurPage=nTotalPage;
		if(posflag==4)
			this->nCurPage=nTotalPage;
		InvalidateRect(CRect(100,1,180,20),0);
		GetParent()->SendMessage(WM_NAVIBAR,posflag,0);
	}
	CWnd::OnLButtonUp(nFlags, point);
}
