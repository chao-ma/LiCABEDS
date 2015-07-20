// PredDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FCC.h"
#include "PredDlg.h"


// CPredDlg dialog

IMPLEMENT_DYNAMIC(CPredDlg, CDialog)

CPredDlg::CPredDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPredDlg::IDD, pParent)
{

}

CPredDlg::~CPredDlg()
{
}

void CPredDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_Boundary);
	DDX_Control(pDX, IDC_EDIT1, m_Count);
	DDX_Control(pDX, IDC_CHECK1, m_CheckBox);
}


BEGIN_MESSAGE_MAP(CPredDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK1, &CPredDlg::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CPredDlg message handlers

BOOL CPredDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CString tmpstr;
	tmpstr.Format("%.2f",fBoundary);
	m_Boundary.SetWindowTextA(tmpstr);
	tmpstr.Format("%d",nTotal);
	m_Count.SetWindowTextA(tmpstr);
	bChecked=true;
	m_CheckBox.SetCheck(1);
	m_Count.EnableWindow(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPredDlg::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
	bChecked=!bChecked;
	m_CheckBox.SetCheck(bChecked);
	if(bChecked)
	{
		CString tmpstr;
		tmpstr.Format("%d",nTotal);
		m_Count.SetWindowTextA(tmpstr);
	}
	m_Count.EnableWindow(!bChecked);
}

void CPredDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CString tmpstr;
	m_Count.GetWindowTextA(tmpstr);
	nCount=atoi(tmpstr);
	if(nCount>nTotal)
		nCount=nTotal;
	if(nCount<0)
		nCount=0;
	m_Boundary.GetWindowTextA(tmpstr);
	fBoundary=atof(tmpstr);
	if(fBoundary<0)
		fBoundary=(-1)*fBoundary;
	
	CDialog::OnOK();
}
