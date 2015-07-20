// ExpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FCC.h"
#include "ExpDlg.h"


// CExpDlg dialog

IMPLEMENT_DYNAMIC(CExpDlg, CDialog)

CExpDlg::CExpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExpDlg::IDD, pParent)
{

}

CExpDlg::~CExpDlg()
{
}

void CExpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CExpDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CExpDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CExpDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CExpDlg message handlers

void CExpDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	char szFilters[]="Text File (*.txt)|*.txt|All Files (*.*)|*.*||";

   CFileDialog fileDlg (FALSE, "txt", "*.txt",OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);
   
   // Display the file dialog. When user clicks OK, fileDlg.DoModal() 
   // returns IDOK.
   if( fileDlg.DoModal ()==IDOK )
   {
      pathName = fileDlg.GetPathName();
      fileName = fileDlg.GetFileTitle ();
	  GetDlgItem(IDC_EDIT1)->SetWindowTextA(pathName);
   }

}

BOOL CExpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	bExportModel=true;
	((CButton*)this->GetDlgItem(IDC_RADIO1))->SetCheck(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CExpDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if(((CButton*)this->GetDlgItem(IDC_RADIO1))->GetCheck())
		bExportModel=true;
	else
		bExportModel=false;
	GetDlgItem(IDC_EDIT1)->GetWindowTextA(pathName);
	OnOK();
}
