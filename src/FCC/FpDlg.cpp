// FpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FCC.h"
#include "FpDlg.h"


// CFpDlg dialog

IMPLEMENT_DYNAMIC(CFpDlg, CDialog)

CFpDlg::CFpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFpDlg::IDD, pParent)
{
	clean=false;
}

CFpDlg::~CFpDlg()
{
}


// CFpDlg message handlers

BOOL CFpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	positive=(CButton*)GetDlgItem(IDC_RADIO1);
	positive->SetCheck(1);
	negative=(CButton*)GetDlgItem(IDC_RADIO2);
	test=(CButton*)GetDlgItem(IDC_RADIO3);
	if(strPositive.GetLength()>50)
		strPositive=strPositive.Left(50)+"...";
	if(strNegative.GetLength()>50)
		strNegative=strNegative.Left(50)+"...";
	positive->SetWindowTextA(strPositive);
	negative->SetWindowTextA(strNegative);
	test=(CButton*)GetDlgItem(IDC_RADIO3);
	dataset=1;
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
BEGIN_MESSAGE_MAP(CFpDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO1, &CFpDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CFpDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CFpDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_CHECK1, &CFpDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON1, &CFpDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CFpDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void CFpDlg::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here
	positive->SetCheck(1);
	negative->SetCheck(0);
	test->SetCheck(0);
	dataset=1;
}

void CFpDlg::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
	positive->SetCheck(0);
	negative->SetCheck(1);
	test->SetCheck(0);
	dataset=2;
}

void CFpDlg::OnBnClickedRadio3()
{
	// TODO: Add your control notification handler code here
	positive->SetCheck(0);
	negative->SetCheck(0);
	test->SetCheck(1);
	dataset=3;
}

void CFpDlg::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
	clean=!clean;
	TRACE("%d\n",clean);
}

void CFpDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
   char szFilters[]="Compound Fingerprint Files (*.fp)|*.fp|All Files (*.*)|*.*||";

   CFileDialog fileDlg (TRUE, "fp", "*.fp",OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);
   
   // Display the file dialog. When user clicks OK, fileDlg.DoModal() 
   // returns IDOK.
   if( fileDlg.DoModal ()==IDOK )
   {
      pathName = fileDlg.GetPathName();
      fileName = fileDlg.GetFileTitle ();
	  GetDlgItem(IDC_EDIT1)->SetWindowTextA(pathName);
   }

}

void CFpDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT1)->GetWindowTextA(pathName);
	OnOK();
}
