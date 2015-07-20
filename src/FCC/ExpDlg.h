#pragma once


// CExpDlg dialog

class CExpDlg : public CDialog
{
	DECLARE_DYNAMIC(CExpDlg)

public:
	CExpDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CExpDlg();
	CString pathName;
	CString fileName;
	bool bExportModel;

// Dialog Data
	enum { IDD = IDD_EXPDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
