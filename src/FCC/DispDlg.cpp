// DispDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FCC.h"
#include "DispDlg.h"


// CDispDlg dialog

IMPLEMENT_DYNAMIC(CDispDlg, CDialog)

CDispDlg::CDispDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDispDlg::IDD, pParent)
{

}

CDispDlg::~CDispDlg()
{
}

void CDispDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, linewidthTrain);
	DDX_Control(pDX, IDC_COMBO2, linetypeTrain);
	DDX_Control(pDX, IDC_COMBO3, linewidthCV);
	DDX_Control(pDX, IDC_COMBO4, linetypeCV);
	DDX_Control(pDX, IDC_COMBO5, VerticalRatio);
	DDX_Control(pDX, IDC_COMBO6, HorizonalRatio);
	DDX_Control(pDX, IDC_SPIN1, verSpin);
	DDX_Control(pDX, IDC_SPIN2, horSpin);
	DDX_Control(pDX, IDC_CHECK1, showGrid);
	DDX_Control(pDX, IDC_EDIT1, NumGridVer);
	DDX_Control(pDX, IDC_EDIT2, NumGridHor);
}


BEGIN_MESSAGE_MAP(CDispDlg, CDialog)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDOK, &CDispDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDispDlg::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDispDlg::OnCbnSelchangeCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDispDlg::OnCbnSelchangeCombo3)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDispDlg::OnCbnSelchangeCombo4)
END_MESSAGE_MAP()


// CDispDlg message handlers

BOOL CDispDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	colorTrain.color=pView->colorTrain;
	colorCV.color=pView->colorCV;
	colorTrain.Create(NULL,WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,CRect(360,26,380,46),this,IDC_BUTTON1);
	colorCV.Create(NULL,WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,CRect(360,80,380,100),this,IDC_BUTTON2);
	// TODO:  Add extra initialization here
	int i;
	CString tmpstr;
	for(i=1;i<6;i++)
	{
		tmpstr.Format("%d",i);
		linewidthTrain.AddString(tmpstr);
		linewidthCV.AddString(tmpstr);
	}
	linetypeTrain.AddString("Solid Line");
	linetypeTrain.AddString("Dashed Line");
	linetypeTrain.AddString("Dotted Line");
	linetypeCV.AddString("Solid Line");
	linetypeCV.AddString("Dashed Line");
	linetypeCV.AddString("Dotted Line");
	
	linewidthTrain.SetCurSel(pView->linewidthTrain-1);
	linewidthCV.SetCurSel(pView->linewidthCV-1);
	linetypeTrain.SetCurSel(pView->linestyleTrain);
	linetypeCV.SetCurSel(pView->linestyleCV);

	showGrid.SetCheck(1);
	tmpstr.Format("%d",pView->NumGridVer);
	NumGridVer.SetWindowTextA(tmpstr);
	tmpstr.Format("%d",pView->NumGridHor);
	NumGridHor.SetWindowTextA(tmpstr);
	
	verSpin.SetRange32(1,100);
	verSpin.SetPos32(pView->NumGridVer);
	horSpin.SetRange32(1,50);
	horSpin.SetPos32(pView->NumGridHor);

	for(i=25;i<201;i=i+25)
	{
		tmpstr.Format("%d %%",i);
		VerticalRatio.AddString(tmpstr);
		HorizonalRatio.AddString(tmpstr);
	}
	for(i=300;i<1601;i=i+100)
	{
		tmpstr.Format("%d %%",i);
		VerticalRatio.AddString(tmpstr);
		HorizonalRatio.AddString(tmpstr);
	}
	tmpstr.Format("%d %%",(int)(pView->ratioVer*100));
	VerticalRatio.SetWindowTextA(tmpstr);

	tmpstr.Format("%d %%",(int)(pView->ratioHor*100));
	HorizonalRatio.SetWindowTextA(tmpstr);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDispDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	if(pScrollBar->GetDlgCtrlID()==IDC_SPIN1)
	{
		CString tmpstr;
		tmpstr.Format("%d",verSpin.GetPos32());
		NumGridVer.SetWindowTextA(tmpstr);
		//TRACE("%d\n",verSpin.GetPos32());
	}
	if(pScrollBar->GetDlgCtrlID()==IDC_SPIN2)
	{
		CString tmpstr;
		tmpstr.Format("%d",horSpin.GetPos32());
		NumGridHor.SetWindowTextA(tmpstr);
		//TRACE("%d\n",verSpin.GetPos32());
	}
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CDispDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	pView->linewidthTrain=linewidthTrain.GetCurSel()+1;
	pView->linestyleTrain=linetypeTrain.GetCurSel();
	pView->colorTrain=colorTrain.color;
	pView->linewidthCV=linewidthCV.GetCurSel()+1;
	pView->linestyleCV=linetypeCV.GetCurSel();
	pView->colorCV=colorCV.color;
	pView->NumGridVer=verSpin.GetPos32();
	pView->NumGridHor=horSpin.GetPos32();
	pView->bShowGrid=showGrid.GetCheck();

	CString tmpstr;
	VerticalRatio.GetWindowTextA(tmpstr);
	pView->ratioVer=atof(tmpstr)/100>0?atof(tmpstr)/100:1.0;
	HorizonalRatio.GetWindowTextA(tmpstr);
	pView->ratioHor=atof(tmpstr)/100>0?atof(tmpstr)/100:1.0;

	OnOK();
}

void CDispDlg::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
	if(linewidthTrain.GetCurSel()>0)
		linetypeTrain.SetCurSel(0);
}

void CDispDlg::OnCbnSelchangeCombo2()
{
	// TODO: Add your control notification handler code here
	if(linetypeTrain.GetCurSel()>0)
		linewidthTrain.SetCurSel(0);
}

void CDispDlg::OnCbnSelchangeCombo3()
{
	// TODO: Add your control notification handler code here
	if(linewidthCV.GetCurSel()>0)
		linetypeCV.SetCurSel(0);
}

void CDispDlg::OnCbnSelchangeCombo4()
{
	// TODO: Add your control notification handler code here
	if(linetypeCV.GetCurSel()>0)
		linewidthCV.SetCurSel(0);
}
