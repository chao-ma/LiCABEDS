#pragma once
#include "afxwin.h"


// CNewDlg dialog

class CNewDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewDlg)

public:
	CNewDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewDlg();

// Dialog Data
	enum { IDD = IDD_NEWDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_title;
	CEdit m_positive;
	CEdit m_negative;
	CString title;
	CString positive;
	CString negative;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	bool bModify;
};
