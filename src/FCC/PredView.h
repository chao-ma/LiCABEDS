#pragma once
#include "FindDialog.h"


// CNaviWnd

class CNaviWnd : public CWnd
{
	DECLARE_DYNAMIC(CNaviWnd)

public:
	CNaviWnd();
	virtual ~CNaviWnd();

protected:
	DECLARE_MESSAGE_MAP()
	CPen edgePen;
	CBrush bkBrush;

	CBitmap bitmap_LeftMost;
	CBitmap bitmap_Left;
	CBitmap bitmap_Right;
	CBitmap bitmap_RightMost;
	CBitmap bitmap_DropDown;
	CBitmap bitmap_ScrollUp;
	CBitmap bitmap_Check;
	CFont font;
	int posflag;
	bool LButtonDown;
public:
	afx_msg void OnPaint();
	int MouseTest(int x, int y);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	CEdit m_nRow;
	CEdit m_nCol;
	int nCurPage;
	int nTotalPage;
	bool bDropDown;
};


// CPredView view

class CPredView : public CScrollView
{
	DECLARE_DYNCREATE(CPredView)

protected:
	CPredView();           // protected constructor used by dynamic creation
	virtual ~CPredView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	LRESULT OnClickNavi(WPARAM wParam, LPARAM lParam);// When mouse click the navigation bar, this function is called.
													//It processes messages containing page numbers and etc
	LRESULT OnFindCompound(WPARAM wParam, LPARAM lParam);// "Find" dialog calls this function to find previous or next compound, goto certain page or go to a specific line. 
										//MouseLBuyttonDown also calls this function to select one or more items.
	DECLARE_MESSAGE_MAP()
	int nDecimal;// Controal how many digits to how for raw yhat value
	int nItemPage;// how many rows for each page
	int nCol;//how many colomns for each page
	int widthCompoundName;// the width to show compound name
	int widthClassName;//the width to show prediction name
	int widthYhat;// the width to show raw yhat value
	int widthID;// the width of compound id
	CObject* pCompound;//record the selected compound position in the list
	int SortAscend;// indicator sorting descend or ascend
	bool bOldMouseInCaption;// indicator the position of mouse. it may affect the shape of cursor
	HCURSOR cursorAscend;// the handle to ascending cursor
	HCURSOR cursorDescend;// the handle to descending cursor
	bool bHitCaption;// indicates where the mouse is. if false, it means arrow cursor should be shown

	CNaviWnd wnd_navi;// the window object for the navigation bar
	CFindDialog dlg;// the dialog object to receive information. It is mainly responsible for positioning one item
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEditFind();//pop up the "find" dialog
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);//either sort by field or select one or more item
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);//identify where the mouse curse is so that the proper cursor shape is shown
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);//  override to accomadate more than one type of mouse cursor
protected:
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);//these three are printing related functions
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
public:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
protected:
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	int printWidthName;
	int printWidthID;
	int printWidthClass;
	int printWidthValue;
	int printFieldMargin;
	int printColMargin;
	int printRowMargin;
	int printCol;
	int printRow;
public:
	afx_msg void OnFilePrint();
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnEditSelectall();
	afx_msg void OnUpdateEditSelectall(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditFind(CCmdUI *pCmdUI);
	afx_msg void OnCpredviewInverseselection();
	afx_msg void OnUpdateCpredviewInverseselection(CCmdUI *pCmdUI);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnUpdateFilePrintPreview(CCmdUI *pCmdUI);
};





