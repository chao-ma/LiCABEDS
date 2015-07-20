// NewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FCC.h"
#include "NewDlg.h"


// CNewDlg dialog

IMPLEMENT_DYNAMIC(CNewDlg, CDialog)

CNewDlg::CNewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewDlg::IDD, pParent)
{
	title="Untitled Project";
	positive="Positive Dataset";
	negative="Negative Dataset";
	bModify=false;
}

CNewDlg::~CNewDlg()
{
}

void CNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_title);
	DDX_Control(pDX, IDC_EDIT3, m_positive);
	DDX_Control(pDX, IDC_EDIT4, m_negative);
}


BEGIN_MESSAGE_MAP(CNewDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CNewDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CNewDlg message handlers

BOOL CNewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_title.SetWindowTextA(title);
	m_positive.SetWindowTextA(positive);
	m_negative.SetWindowTextA(negative);
	if(bModify)
		SetWindowTextA("Edit Project");
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CNewDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	m_title.GetWindowTextA(title);
	m_positive.GetWindowTextA(positive);
	m_negative.GetWindowTextA(negative);
	if(title.GetLength()==0)
		title="Untitled Project";
	if(positive.GetLength()==0)
		positive="Positive Dataset";
	if(negative.GetLength()==0)
		negative="Negative Dataset";
	OnOK();
}
