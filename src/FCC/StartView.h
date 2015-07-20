#pragma once
#include "FCCDoc.h"
#define TAG_PROJECT 1;
#define TAG_FINGERPRINT 2;
#define TAG_TRAIN 3;
#define TAG_PREDICT 4;

class CTag : public CObject
{
public:
	CTag();
	virtual ~CTag();
	CRect rect_Region;
	int nTagType;
	CString str_Title;
	CString str_Info;
	int nStatus;
	CBitmap bitmap_green;
	CBitmap bitmap_yellow;
	CBitmap bitmap_red;
	HANDLE icon_Title;
	bool bSelected;
	void DrawTag(CDC* pDC);
	virtual int CheckStatus(CFCCDoc* pDoc);
	virtual void GetHelpString(CString& str);
	virtual void ExecuteCommand(void);
};

// CStartView view

class CStartView : public CScrollView
{
	DECLARE_DYNCREATE(CStartView)

protected:
	CStartView();           // protected constructor used by dynamic creation
	virtual ~CStartView();

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

	DECLARE_MESSAGE_MAP()
	CObList list_Tag;
	CString str_InfoBar;
	CRect rect_InfoBar;
	CSize margin_offset;
	bool bShowHand;
	HANDLE icon_help;
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
protected:
	
};


