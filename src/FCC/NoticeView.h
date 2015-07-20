#pragma once



// CNoticeView view

class CNoticeView : public CScrollView
{
	DECLARE_DYNCREATE(CNoticeView)

protected:
	CNoticeView();           // protected constructor used by dynamic creation
	virtual ~CNoticeView();
	CBrush titleBrush;
	CPen titlePen;
	CFont titleFont;
	CBrush projectBrush;
	CBrush messageBrush;
	CFont projectFont;
	CBitmap messageBitmap;
	CBitmap runningBitmap;
	CBitmap warningBitmap;
	

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
public:
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


