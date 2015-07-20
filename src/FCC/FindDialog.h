#pragma once
#include "afxwin.h"
#define GOTOPAGE 1
#define GOTOLINE 2
#define FINDPREV 3
#define FINDNEXT 4


// CFindDialog dialog

class CFindDialog : public CDialog
{
	DECLARE_DYNAMIC(CFindDialog)

public:
	CFindDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFindDialog();
	bool page;
	bool next;

// Dialog Data
	enum { IDD = IDD_FINDDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedFindprev();
	CEdit m_edit;
	afx_msg void OnBnClickedFindnext();
	CEdit m_num;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	virtual BOOL OnInitDialog();
	CString strMessage;
	int nIndex;
	CView* m_pView;
	afx_msg void OnBnClickedButton3();
};
