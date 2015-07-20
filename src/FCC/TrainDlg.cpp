// TrainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FCC.h"
#include "TrainDlg.h"


// CTrainDlg dialog

IMPLEMENT_DYNAMIC(CTrainDlg, CDialog)

CTrainDlg::CTrainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTrainDlg::IDD, pParent)
{
	iti=100;
	fCVProportion=0.1;
}

CTrainDlg::~CTrainDlg()
{
}

void CTrainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_iti);
}


BEGIN_MESSAGE_MAP(CTrainDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK2, &CTrainDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK1, &CTrainDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDOK, &CTrainDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO1, &CTrainDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CTrainDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()


// CTrainDlg message handlers

BOOL CTrainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_iti.SetWindowTextA("100");
	equalweight=(CButton*)GetDlgItem(IDC_RADIO1);
	balancedweight=(CButton*)GetDlgItem(IDC_RADIO2);
	equalweight->SetCheck(1);
	bequalweight=true;
	cv=false;
	continuetraining=false;
	
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTrainDlg::OnBnClickedCheck2()
{
	// TODO: Add your control notification handler code here
	cv=!cv;
	((CComboBox*)GetDlgItem(IDC_COMBO1))->EnableWindow(cv);
	if(((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel()==CB_ERR)
		((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(1);
}

void CTrainDlg::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
	continuetraining=!continuetraining;
}

void CTrainDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString tmpstr;
	m_iti.GetWindowTextA(tmpstr);
	iti=atoi(tmpstr);
	int sel=((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
	if(sel<0)
		sel=0;
	fCVProportion=0.05*(sel+1);
	OnOK();
}

void CTrainDlg::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here
	bequalweight=true;
}

void CTrainDlg::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
	bequalweight=false;
}
