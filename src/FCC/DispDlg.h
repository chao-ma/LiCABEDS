#pragma once
#include "afxwin.h"
#include "CButton.h"
#include "afxcmn.h"
#include "FCCView.h"
// CDispDlg dialog

class CDispDlg : public CDialog
{
	DECLARE_DYNAMIC(CDispDlg)

public:
	CDispDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDispDlg();

// Dialog Data
	enum { IDD = IDD_DISPSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox linewidthTrain;
	CComboBox linetypeTrain;
	CCButton colorTrain;
	CComboBox linewidthCV;
	CComboBox linetypeCV;
	CCButton colorCV;
	CComboBox VerticalRatio;
	CComboBox HorizonalRatio;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();

	CSpinButtonCtrl verSpin;
	CSpinButtonCtrl horSpin;

	CFCCView* pView;
	CButton showGrid;
	CEdit NumGridVer;
	CEdit NumGridHor;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnCbnSelchangeCombo4();
};
