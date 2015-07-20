#pragma once
#include "afxwin.h"


// CPredDlg dialog

class CPredDlg : public CDialog
{
	DECLARE_DYNAMIC(CPredDlg)

public:
	CPredDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPredDlg();
	int nCount;
	float fBoundary;
	bool bChecked;
	int nTotal;

// Dialog Data
	enum { IDD = IDD_PREDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit m_Boundary;
	CEdit m_Count;
	CButton m_CheckBox;
	afx_msg void OnBnClickedCheck1();
protected:
	virtual void OnOK();
};
