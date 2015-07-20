// FindDialog.cpp : implementation file
//

#include "stdafx.h"
#include "FCC.h"
#include "FindDialog.h"


// CFindDialog dialog

IMPLEMENT_DYNAMIC(CFindDialog, CDialog)

CFindDialog::CFindDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFindDialog::IDD, pParent)
{
	m_pView=NULL;
}

CFindDialog::~CFindDialog()
{
}

void CFindDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	DDX_Control(pDX, IDC_EDIT2, m_num);
}


BEGIN_MESSAGE_MAP(CFindDialog, CDialog)
	ON_BN_CLICKED(IDC_FINDPREV, &CFindDialog::OnBnClickedFindprev)
	ON_BN_CLICKED(IDC_FINDNEXT, &CFindDialog::OnBnClickedFindnext)
	ON_BN_CLICKED(IDC_RADIO1, &CFindDialog::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CFindDialog::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_BUTTON3, &CFindDialog::OnBnClickedButton3)
END_MESSAGE_MAP()


// CFindDialog message handlers

void CFindDialog::OnBnClickedFindprev()
{
	// TODO: Add your control notification handler code here
	m_edit.GetWindowTextA(strMessage);
	m_pView->SendMessage(WM_FINDCOMPOUND,FINDPREV);
}

void CFindDialog::OnBnClickedFindnext()
{
	// TODO: Add your control notification handler code here
	m_edit.GetWindowTextA(strMessage);
	m_pView->SendMessage(WM_FINDCOMPOUND,FINDNEXT);
}

void CFindDialog::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(0);
	page=true;
}

void CFindDialog::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(1);
	page=false;
}

BOOL CFindDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_num.SetWindowTextA("1");
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(0);
	page=true;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CFindDialog::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	CString tmpstr;
	this->m_num.GetWindowTextA(tmpstr);
	nIndex=atoi(tmpstr);
	TRACE("sent %s\n",m_pView->GetRuntimeClass()->m_lpszClassName);
	
	if(((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck())
		m_pView->SendMessage(WM_FINDCOMPOUND,GOTOPAGE);
	else
		m_pView->SendMessage(WM_FINDCOMPOUND,GOTOLINE);
}
