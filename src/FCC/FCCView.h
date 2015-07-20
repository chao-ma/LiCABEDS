// FCCView.h : interface of the CFCCView class
//


#pragma once
#include "FCCDoc.h"

class CFCCView : public CScrollView
{
protected: // create from serialization only
	CFCCView();
	DECLARE_DYNCREATE(CFCCView)

// Attributes
public:
	CFCCDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//For Painting
	CPen gridpen;//the GDI object to paint the grid
	CPen errorpen;//the GDI object to paint the error curve
	CBrush errorbrush;//the GDI object to paint the error legend
	CPen cvpen;//the GDI object to paint the cross-validation curve
	CBrush cvbrush;//the GDI object to paint the cross-validation legend
	CFont gridfont;//the font of numbers above the grids
	CFont legendfont;//the font to show the legend
	CBitmap memBitmap;//the memory buffer where the curve is painted. It will copied to the screening when OnDraw() is called
	bool bCreateBitmap;//indicate whether the bitmap should be deleted before recreation
	//Display Preference
	int NumGridVer;// how many grids to draw vertically
	int NumGridHor;//how many grids to draw horizonally
	int Height;//
	float ratioHor;//the zoom ratio in horizonal direction
	float ratioVer;//the zoom ratio in vertical direction
	bool bShowGrid;// indicate whether to draw the grids in the window
	int linewidthTrain;//how width the training curve is
	int linestyleTrain;//what is the pen style(dot dash..) of training curve
	COLORREF colorTrain;// the color of training curve
	int linewidthCV;//how width the cv curve is
	int linestyleCV;//what is the pen style(dot dash..) of cv curve
	COLORREF colorCV;// the color of cv curve
	bool bMouseTrack;//indicate whether the coordinate will be shown. It is the dotted crossing line moving with the mouse
	//for mouse track function
	CPoint ptOldMousePos;//record the position of mouse previously
	//for Mouse click and drag operation
	int selectLeft;//the point to start with
	int selectRight;//the point to end
	int selectLeftMost;//the left most point the mouse ever reached 
	int selectRightMost;//the right most point the mouse ever reached
	bool bLButtonDown;//indicate if the mouse left button is down
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CFCCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	LRESULT OnPaintNewClassifier(WPARAM wParam, LPARAM lParam);
	LRESULT OnInitiatePaint(WPARAM wParam, LPARAM lParam);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnViewFittowindowsize();
	afx_msg void OnViewHorizonalzoomout();
	afx_msg void OnViewHorizonalzoomin();
	afx_msg void OnViewVerticalzoomin();
	afx_msg void OnViewVerticalzoomout();
	afx_msg void OnViewDisplayoption();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnViewShowcoordinates();
	afx_msg void OnUpdateViewFittowindowsize(CCmdUI *pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	void RedrawBitmap(void);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnUpdateViewHorizonalzoomin(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewHorizonalzoomout(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewVerticalzoomin(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewVerticalzoomout(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewShowcoordinates(CCmdUI *pCmdUI);
protected:
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
public:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	afx_msg void OnEditCopy();
	afx_msg void OnEditSelectall();
	int SaveDisplayOption(void);
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};

#ifndef _DEBUG  // debug version in FCCView.cpp
inline CFCCDoc* CFCCView::GetDocument() const
   { return reinterpret_cast<CFCCDoc*>(m_pDocument); }
#endif

