#pragma once
#include "afxwin.h"


// CTrainDlg dialog

class CTrainDlg : public CDialog
{
	DECLARE_DYNAMIC(CTrainDlg)

public:
	CTrainDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTrainDlg();
	CButton* equalweight;
	CButton* balancedweight;
	float fCVProportion;
	bool cv;
	bool continuetraining;
	bool bequalweight;
	int iti;


// Dialog Data
	enum { IDD = IDD_TRAINDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit m_iti;
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
};
