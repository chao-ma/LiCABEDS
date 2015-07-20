#pragma once
#include "afxwin.h"


// CFpDlg dialog

class CFpDlg : public CDialog
{
	DECLARE_DYNAMIC(CFpDlg)

public:
	CFpDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFpDlg();
	CString fileName;
	CString pathName;
	int dataset;
	bool clean;
	CString strPositive;
	CString strNegative;


// Dialog Data
	enum { IDD = IDD_FPDLG };
	CButton* positive;
	CButton* negative;
	CButton* test;
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
};
