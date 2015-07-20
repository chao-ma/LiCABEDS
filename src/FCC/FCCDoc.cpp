// FCCDoc.cpp : implementation of the CFCCDoc class
//

#include "stdafx.h"
#include "FCC.h"
#include "FCCDoc.h"
#include "math.h"
#include "NewDlg.h"
#include "NoticeView.h"
#include "FCCView.h"
#include "FpDlg.h"
#include "TrainDlg.h"
#include "PredDlg.h"
#include "ExpDlg.h"
#include "StartView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CEvent draw_event;
CEvent eventThreadExit;
bool bThreadExit;
bool bAdaboostRunning;
int ReleaseVersion;
// CFCCDoc

IMPLEMENT_DYNCREATE(CFCCDoc, CDocument)

BEGIN_MESSAGE_MAP(CFCCDoc, CDocument)
//	ON_COMMAND(ID_FILE_OPEN, &CFCCDoc::OnFileOpen)
	ON_COMMAND(ID_ADABOOST_MODELTRAINING, &CFCCDoc::OnAdaboostModeltraining)
	ON_COMMAND(ID_ADABOOST_PREDICTING, &CFCCDoc::OnAdaboostPredicting)
	ON_COMMAND(ID_EDIT_EDITPROJECTTITLE, &CFCCDoc::OnEditEditprojecttitle)
	ON_COMMAND(ID_FILE_LOADFINGERPRINTS, &CFCCDoc::OnFileLoadfingerprints)
	ON_COMMAND(ID_EDIT_STOPTRAINING, &CFCCDoc::OnEditStoptraining)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, &CFCCDoc::OnUpdateFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &CFCCDoc::OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, &CFCCDoc::OnUpdateFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_LOADFINGERPRINTS, &CFCCDoc::OnUpdateFileLoadfingerprints)
	ON_UPDATE_COMMAND_UI(ID_EDIT_STOPTRAINING, &CFCCDoc::OnUpdateEditStoptraining)
	ON_UPDATE_COMMAND_UI(ID_ADABOOST_PREDICTING, &CFCCDoc::OnUpdateAdaboostPredicting)
	ON_UPDATE_COMMAND_UI(ID_ADABOOST_MODELTRAINING, &CFCCDoc::OnUpdateAdaboostModeltraining)
	ON_COMMAND(ID_EDIT_EXPORT, &CFCCDoc::OnEditExport)
	ON_COMMAND(ID_HELP_CONTENTS, &CFCCDoc::OnHelpContents)
	ON_COMMAND(ID_EditParameterOpt, &CFCCDoc::OnEditparameteropt)
END_MESSAGE_MAP()


// CFCCDoc construction/destruction

CFCCDoc::CFCCDoc()
{
	// TODO: add one-time construction code here
	nFpLen=0;
	strPositiveSet="Positive Dataset";
	strNegativeSet="Negative Dataset";
	strProjectTitle="Untitled Project";
	nMsgCode=IDS_INDICATOR_MESSAGE;
	firstRun=true;
	NumClassifier=0;
	cv=false;
	bAdaboostRunning=false;
	fRejectBoundary=0.0;
	//record the version for each release
	version=1;
	ReleaseVersion=version;
	bSmoothCV=false;
}

CFCCDoc::~CFCCDoc()
{
}

BOOL CFCCDoc::OnNewDocument()
{
	
	if(!firstRun)
	{
		//CNewDlg dlg;
		//if(dlg.DoModal()==IDOK)
		//{
			if (!CDocument::OnNewDocument())
				return FALSE;
			nFpLen=0;
			// TODO: add reinitialization code here
			// (SDI documents will reuse this document)
			strPositiveSet="Positive Dataset";
			strNegativeSet="Negative Dataset";
			strProjectTitle="Untitled Project";
			SetTitle("Untitled Project");
			MessageCenter(IDS_MESSAGE_NEWDOCUMENT);
			AfxGetApp()->m_pMainWnd->SendMessage(WM_COMMAND,ID_VIEW_STARTUPPAGE);
			//UpdateAllViews(NULL);
			POSITION pos=this->GetFirstViewPosition();
			while(pos)
			{
				CView* pView=(CView*)this->GetNextView(pos);
				//TRACE("%s %s %d\n ",pView->GetRuntimeClass()->m_lpszClassName,"CNoticeView",pView->GetRuntimeClass()->m_lpszClassName=="CNoticeView");
				if(pView->GetRuntimeClass()->m_lpszClassName=="CNoticeView")
				{
					//UpdateAllViews(pView);
					pView->Invalidate();
				}
			}
			return TRUE;
		//}
	}
	else
	{
		if (!CDocument::OnNewDocument())
				return FALSE;
		nFpLen=0;
		// TODO: add reinitialization code here
		// (SDI documents will reuse this document)
		SetTitle("Untitled Project");
		firstRun=false;
		MessageCenter(IDS_MESSAGE_WELCOME);
		return TRUE;
	}
	
}




// CFCCDoc serialization

void CFCCDoc::Serialize(CArchive& ar)
{
	int count,i;
	POSITION pos;
	CCompound* pCompound;
	CClassifier* pClassifier;
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		ar<<version;
		ar<<strProjectTitle;
		ar<<strPositiveSet;
		ar<<strNegativeSet;
		ar<<nFpLen;
		ar<<NumClassifier;
		ar<<fRejectBoundary;
		ar<<cv;
		count=list_CompoundPositive.GetCount();
		ar<<count;
		pos=list_CompoundPositive.GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)list_CompoundPositive.GetNext(pos);
			pCompound->Serialize(ar,nFpLen);
		}
		count=list_CompoundNegative.GetCount();
		ar<<count;
		pos=list_CompoundNegative.GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)list_CompoundNegative.GetNext(pos);
			pCompound->Serialize(ar,nFpLen);
		}
		count=list_CompoundTest.GetCount();
		ar<<count;
		pos=list_CompoundTest.GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)list_CompoundTest.GetNext(pos);
			pCompound->Serialize(ar,nFpLen);
		}
		count=list_CompoundCV.GetCount();
		ar<<count;
		pos=list_CompoundCV.GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)list_CompoundCV.GetNext(pos);
			pCompound->Serialize(ar,nFpLen);
		}

		count=list_Classifier.GetCount();
		ar<<count;
		pos=list_Classifier.GetHeadPosition();
		while(pos)
		{
			pClassifier=(CClassifier*)list_Classifier.GetNext(pos);
			pClassifier->Serialize(ar);
		}
		MessageCenter(IDS_MESSAGE_SAVE);
		POSITION pos=this->GetFirstViewPosition();
		while(pos)
		{
			CView* pView=(CView*)this->GetNextView(pos);
			//TRACE("%s %s %d\n ",pView->GetRuntimeClass()->m_lpszClassName,"CNoticeView",pView->GetRuntimeClass()->m_lpszClassName=="CNoticeView");
			if(pView->GetRuntimeClass()->m_lpszClassName=="CNoticeView")
			{
				UpdateAllViews(pView);
				pView->Invalidate();
			}
		}
	}
	else
	{
		// TODO: add loading code here
		ar>>version;
		ar>>strProjectTitle;
		ar>>strPositiveSet;
		ar>>strNegativeSet;
		ar>>nFpLen;
		ar>>NumClassifier;
		ar>>fRejectBoundary;
		ar>>cv;
		ar>>count;
		for(i=0;i<count;i++)
		{
			pCompound=new CCompound;
			pCompound->Serialize(ar,nFpLen);
			list_CompoundPositive.AddTail(pCompound);
		}
		ar>>count;
		for(i=0;i<count;i++)
		{
			pCompound=new CCompound;
			pCompound->Serialize(ar,nFpLen);
			list_CompoundNegative.AddTail(pCompound);
		}
		ar>>count;
		for(i=0;i<count;i++)
		{
			pCompound=new CCompound;
			pCompound->Serialize(ar,nFpLen);
			list_CompoundTest.AddTail(pCompound);
		}
		ar>>count;
		for(i=0;i<count;i++)
		{
			pCompound=new CCompound;
			pCompound->Serialize(ar,nFpLen);
			list_CompoundCV.AddTail(pCompound);
		}
		ar>>count;
		for(i=0;i<count;i++)
		{
			pClassifier=new CClassifier;
			pClassifier->Serialize(ar);
			list_Classifier.AddTail(pClassifier);
		}
		TRACE("Restore code called\n");
		MessageCenter(IDS_MESSAGE_RESTORE);
		POSITION pos=this->GetFirstViewPosition();
		while(pos)
		{
			CView* pView=(CView*)this->GetNextView(pos);
			//TRACE("%s %s %d\n ",pView->GetRuntimeClass()->m_lpszClassName,"CNoticeView",pView->GetRuntimeClass()->m_lpszClassName=="CNoticeView");
			if(pView->GetRuntimeClass()->m_lpszClassName=="CNoticeView")
			{
				UpdateAllViews(pView);
				pView->Invalidate();
			}
		}

	}
}


// CFCCDoc diagnostics

#ifdef _DEBUG
void CFCCDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFCCDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CFCCDoc commands

//void CFCCDoc::OnFileOpen()
//{
//	
//	
//}
// FCCDoc.cpp : implementation file
//

// CCompound
IMPLEMENT_DYNCREATE(CCompound, CObject)
CCompound::CCompound()
{
	pFingerprint=NULL;
	weight=0;
	yhat=0;
	bSelect=false;
}

CCompound::~CCompound()
{
	if(pFingerprint)
		delete pFingerprint;
}


// CCompound member functions

void CFCCDoc::DeleteContents()
{
	// TODO: Add your specialized code here and/or call the base class
	//TRACE("DeleteContents called.\n");
	if(bAdaboostRunning)
	{
		bThreadExit=true;
		::WaitForSingleObject(eventThreadExit,INFINITE);
	}
	nFpLen=0;
	NumClassifier=0;
	cv=false;
	while(!list_CompoundPositive.IsEmpty())
	{
		CCompound* pCompound=(CCompound*)list_CompoundPositive.RemoveTail();
		delete pCompound;
	}
	while(!list_CompoundNegative.IsEmpty())
	{
		CCompound* pCompound=(CCompound*)list_CompoundNegative.RemoveTail();
		delete pCompound;
	}
	while(!list_CompoundTest.IsEmpty())
	{
		CCompound* pCompound=(CCompound*)list_CompoundTest.RemoveTail();
		delete pCompound;
	}
	while(!list_CompoundCV.IsEmpty())
	{
		CCompound* pCompound=(CCompound*)list_CompoundCV.RemoveTail();
		delete pCompound;
	}
	while(!list_Classifier.IsEmpty())
	{
		CClassifier* pClassifier=(CClassifier*)list_Classifier.RemoveTail();
		delete pClassifier;
	}
	CDocument::DeleteContents();
}

int CFCCDoc::LoadFingerprint(CObList& list_Compound, int label, CString fileName,bool clean)
{
	while(clean && !list_Compound.IsEmpty())
	{
		CCompound* pCompound=(CCompound*)list_Compound.RemoveTail();
		delete pCompound;
	}
	CFile m_file;
	int res=1;
	int id=list_Compound.GetCount()+1;
	m_file.Open(fileName,CFile::modeRead);
	if(m_file.m_hFile==CFile::hFileNull)
	{
		//AfxMessageBox("Can't open the specified file.");
		return IDS_MESSAGE_FAILOPENFILE;
	}
	else
	{
		int buffsize=512;int readlength=0;
		CString tmpstr;
		char* buffer=NULL;
		int maxinterval=0;
		int pos;
		ULONGLONG lower=0,upper=0;
		buffer=(char*)malloc(sizeof(char)*buffsize+1);
		ULONGLONG filelength=m_file.GetLength();
		ULONGLONG fileposition=0;
		while(fileposition<filelength)
		{
			if((fileposition+buffsize)<filelength)
			{
				m_file.Read(buffer,buffsize);
				tmpstr=buffer;
				tmpstr=tmpstr.Left(buffsize);
				fileposition+=buffsize;
				readlength=buffsize;
			}
			else
			{
				readlength=filelength-fileposition;
				m_file.Read(buffer,buffsize);
				tmpstr=buffer;
				tmpstr=tmpstr.Left(readlength);
				fileposition+=readlength;
			}
			//TRACE("%s",tmpstr);
			pos=0;
			//TRACE("%d \n",fileposition);
			while(tmpstr.Find(10,pos)>=0)
			{
				pos=tmpstr.Find(10,pos);
				upper=pos+fileposition-readlength;
				//TRACE("char return position %d \n",pos);
				if(upper-lower>maxinterval)
					maxinterval=upper-lower;
				lower=upper;
				pos++;
			}
		}
		//TRACE("max interval %d file length%d\n",maxinterval,filelength);
		delete buffer;
		if(maxinterval<1024*4)
			buffsize=1024*4;
		else
			buffsize=maxinterval*2+1;
		buffer=(char*)malloc(sizeof(char)*buffsize);
		//TRACE("buffer size%d\n",buffsize);
		if(buffer)
		{
			m_file.SeekToBegin();
			fileposition=0;
			CString strParse;
			CString strFeature;
			int i,nStrLen,index;
			while(fileposition<filelength)
			{
				if(fileposition+buffsize<filelength)
				{
					m_file.Read(buffer,buffsize);
					readlength=buffsize;
				}
				else
				{
					ZeroMemory(buffer,buffsize);
					m_file.Read(buffer,filelength-fileposition);
					readlength=filelength-fileposition;
				}
				strParse=buffer;
				//TRACE("strParse length %d\n",strParse.GetLength());
				strParse=strParse.Left(readlength);
				int nupper=0;int nlower=0;
				nupper=strParse.Find(10,nlower);
				while(nupper>=0)
				{
					strFeature=strParse.Mid(nlower,nupper-nlower+1);
					if(this->nFpLen==0)
					{
						int i=strFeature.Find(' ');
						for(;i<strFeature.GetLength() && i>=0;i++)
							if(strFeature[i]=='1' || strFeature[i]=='0')
								nFpLen++;
						//TRACE("fp length %d\n",nFpLen);
					}
					if(nFpLen<=0)
					{
						//AfxMessageBox("No fingerprint information detected!");
						delete buffer;
						m_file.Close();
						return IDS_MESSAGE_INCORRECTFORMAT;
						//res=0;
					}
					else
					{		
						nStrLen=strFeature.GetLength();
						CCompound* pCompound=new CCompound();
						pCompound->id=id;
						id++;
						list_Compound.AddTail(pCompound);
						i=strFeature.Find(' ');
						pCompound->strCompoundName=strFeature.Left(i);
						pCompound->pFingerprint=(bool*)malloc(sizeof(bool)*nFpLen);
						pCompound->label=label;
						index=0;
						for(;i<nStrLen && i>=0;i++)
						{
							if(strFeature[i]=='0')
							{
								if(index>=nFpLen)
								{
									TRACE("index %d\n",index);
									//AfxMessageBox("The length of fingerprint is not consistent!");
									delete buffer;
									m_file.Close();
									return IDS_MESSAGE_INCONSISTENTLENGTH;
									//index=0;
								}
								pCompound->pFingerprint[index]=0;
								index++;
							}
							if(strFeature[i]=='1')
							{
								if(index>=nFpLen)
								{
									TRACE("index %d\n",index);
									delete buffer;
									m_file.Close();
									return IDS_MESSAGE_INCONSISTENTLENGTH;
									
								}
								pCompound->pFingerprint[index]=1;
								index++;
							}
							
						}
					}
					nlower=nupper+1;
					nupper=strParse.Find(10,nlower);
					//TRACE("upper %d\n",nupper);
				}
				if(nlower==0)
				{
					delete buffer;
					m_file.Close();
					return IDS_MESSAGE_FATALFILEREADING;

				}
				fileposition+=nlower;
				//TRACE("file position %d\n",fileposition);
				m_file.Seek(fileposition,CFile::begin);
			}

			delete buffer;
		}
		else
		{
			m_file.Close();
			return IDS_MESSAGE_MEMORY;
		}
		m_file.Close();
	}
	return IDS_MESSAGE_FPREADINGSUCC;
}

int CFCCDoc::Classifier(int bit, int value, CObList& list_Compound)
{
	if(bit<0 || bit>=nFpLen)
		return 0;
	CCompound* pCompound;
	POSITION pos;
	pos=list_Compound.GetHeadPosition();
	while(pos)
	{
		pCompound=(CCompound*)list_Compound.GetNext(pos);
		if(pCompound->pFingerprint[bit]==value)
			pCompound->predict=1;
		else
			pCompound->predict=-1;
	}
	return 1;
}
// FCCDoc.cpp : implementation file
//


// CClassifier
IMPLEMENT_DYNCREATE(CClassifier, CObject)

CClassifier::CClassifier()
{
	trainingerror=0;
	cverror=0;
}

CClassifier::~CClassifier()
{
}


// CClassifier member functions

int CFCCDoc::ViewFingerprint(CObList& list_Compound)
{
	POSITION pos;
	CString tmpstr;
	pos=list_Compound.GetHeadPosition();
	while(pos)
	{
		CCompound* pCompound=(CCompound*)(list_Compound.GetNext(pos));
		TRACE("%s %d %f | ",pCompound->strCompoundName,pCompound->label,pCompound->weight);
		tmpstr="";
		for(int i=0;i<nFpLen;i++)
		{
			if(pCompound->pFingerprint[i])
				tmpstr+="1";
			else
				tmpstr+="0";
		}
		TRACE("%s\n",tmpstr);
	}

	return 0;
}

void CFCCDoc::OnAdaboostModeltraining()
{
	/*int res=LoadFingerprint(list_CompoundPositive,1,"E:\\GLIDE\\1\\maccs_agonist_train.fp",1);
	TRACE("Opening positive training data: %d\n",res);
	res=LoadFingerprint(list_CompoundNegative,-1,"e:\\glide\\1\\maccs_antagonist_train.fp",1);
	TRACE("Opening positive testing data: %d\n",res);*/

	CTrainDlg dlg;
	if(list_CompoundPositive.GetCount()==0 || list_CompoundNegative.GetCount()==0)
	{
		MessageCenter(IDS_MESSAGE_NOTRAININGDATA);
		return;
	}
	if(dlg.DoModal()==IDOK && dlg.iti>0)
	{
		
		CCompound* pCompound;
		POSITION pos;
		iti=dlg.iti;
		if(list_Classifier.IsEmpty())
			dlg.continuetraining=false;
		if(dlg.continuetraining==false)
		{
			while(!list_CompoundCV.IsEmpty())
			{
				pCompound=(CCompound*)list_CompoundCV.GetHead();
				if(pCompound->label>0)
					list_CompoundPositive.AddTail(pCompound);
				else
					list_CompoundNegative.AddTail(pCompound);
				list_CompoundCV.RemoveHead();
			}
			
			CClassifier* pClassifier;
			cv=dlg.cv;
			while(!list_Classifier.IsEmpty())
			{
				pClassifier=(CClassifier*)list_Classifier.RemoveTail();
				delete pClassifier;
			}
			if(cv)
			{
				int i,count;
				//count=list_CompoundNegative.GetCount()-(int)(list_CompoundNegative.GetCount()*0.9);
				count=list_CompoundNegative.GetCount()*dlg.fCVProportion;
				//TRACE("%d\n",list_CompoundNegative.GetCount()-(int)(list_CompoundNegative.GetCount()*0.9));
				//TRACE("CompoundNegative %d %d\n",list_CompoundNegative.GetCount(),count);
				for(i=0;i<count;i++)
				{
					pCompound=(CCompound*)list_CompoundNegative.RemoveTail();
					pCompound->yhat=0;
					list_CompoundCV.AddHead(pCompound);
				}
				//count=list_CompoundPositive.GetCount()-(int)(list_CompoundPositive.GetCount()*0.9);
				count=list_CompoundPositive.GetCount()*dlg.fCVProportion;
				//TRACE("%d\n",list_CompoundPositive.GetCount()-(int)(list_CompoundPositive.GetCount()*0.9));
				for(i=0;i<count;i++)
				{
					pCompound=(CCompound*)list_CompoundPositive.RemoveTail();
					pCompound->yhat=0;
					list_CompoundCV.AddHead(pCompound);
				}
				//TRACE("list_CompoundPositive %d %d\n",list_CompoundPositive.GetCount(),count);
				
			}
			int count;
			if(dlg.bequalweight)
				count=list_CompoundPositive.GetCount()+list_CompoundNegative.GetCount();
			else
				count=list_CompoundPositive.GetCount();
			pos=list_CompoundPositive.GetHeadPosition();
			while(pos)
			{
				pCompound=(CCompound*)(list_CompoundPositive).GetNext(pos);
				pCompound->weight=1.0/count;
				pCompound->yhat=0;
			}
			if(dlg.bequalweight)
				count=list_CompoundPositive.GetCount()+list_CompoundNegative.GetCount();
			else
				count=list_CompoundNegative.GetCount();
			pos=list_CompoundNegative.GetHeadPosition();
			while(pos)
			{
				pCompound=(CCompound*)(list_CompoundNegative).GetNext(pos);
				pCompound->weight=1.0/count;
				pCompound->yhat=0;
			}
		}
		MessageCenter(IDS_MESSAGE_TRAINING);
		AfxGetApp()->m_pMainWnd->SendMessageA(WM_COMMAND,ID_VIEW_TRAININGERROR);
		UpdateAllViews(NULL);
		SetModifiedFlag(true);
		NumClassifier=list_Classifier.GetCount()+iti;
		pos=this->GetFirstViewPosition();
		while(pos)
		{
			CView* pView=(CView*)this->GetNextView(pos);
			
			if(pView->GetRuntimeClass()->m_lpszClassName=="CFCCView")
			{
				pView->SendMessage(WM_INITIATEPAINT);
			}
		}
		bThreadExit=false;
		CWinThread* pThread=AfxBeginThread(AdaboostTrain,this);
		
	}

	// TODO: Add your command handler code here
}

void CFCCDoc::OnAdaboostPredicting()
{
	// TODO: Add your command handler code here
	int res;
	CPredDlg dlg;
	dlg.nTotal=list_Classifier.GetCount();
	dlg.fBoundary=0.0;
	//res=LoadFingerprint(list_CompoundTest,0,"e:\\glide\\1\\maccs_agonist_test.fp",1);
	if(dlg.DoModal()==IDOK)
	{
		fRejectBoundary=dlg.fBoundary;
		TRACE("num of classifiers %d\n",dlg.nCount);
		res=MakePrediction(list_CompoundTest,dlg.nCount);
		AfxGetApp()->m_pMainWnd->SendMessageA(WM_COMMAND,ID_VIEW_PREDICTION);
		SetModifiedFlag(1);
	}
	//res=LoadFingerprint(list_CompoundTest,0,"e:\\glide\\1\\maccs_antagonist_test.fp",1);
	//res=MakePrediction(list_CompoundTest);
}

void CFCCDoc::OnEditparameteropt()
{
	// TODO: Add your command handler code here
	//this->OnOpenDocument("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\CB2 Selectivity\\1\\Untitled Project.FCC");
	int nSmoothWinLength;
	nSmoothWinLength=20;
	CObList listCoverage;
	POSITION posa,posb,posc;
	CClassifier* pClassifier;
	CClassifier* pTmpClassifier;
	int counter=0;
	int index=0;
	posa=list_Classifier.GetHeadPosition();
	posb=posa;
	float minSmoothCVError=1.0;
	int optimalTrainingStep;
	while(posb && counter<nSmoothWinLength/2)
	{
		pClassifier=(CClassifier*)list_Classifier.GetNext(posb);
		listCoverage.AddTail(pClassifier);
		counter++;
	}
	TRACE("%d\n",counter);

	while(posa)
	{
		pClassifier=(CClassifier*)list_Classifier.GetNext(posa);
		index++;
		if(posb)
		{
			pTmpClassifier=(CClassifier*)list_Classifier.GetNext(posb);
			listCoverage.AddTail(pTmpClassifier);
			if(listCoverage.GetCount()>nSmoothWinLength)
				listCoverage.RemoveHead();
		}
		else
		{
			if(list_Classifier.GetCount()>nSmoothWinLength)
			{
				listCoverage.RemoveHead();
			}
			if(list_Classifier.GetCount()<=nSmoothWinLength && list_Classifier.GetCount()>nSmoothWinLength/2)
			{
				if(index>nSmoothWinLength/2)
					listCoverage.RemoveHead();
			}
			
		}
		//TRACE("%d\n",listCoverage.GetCount());
		pClassifier->SmoothCVError=0;
		posc=listCoverage.GetHeadPosition();
		while(posc)
		{
				pTmpClassifier=(CClassifier*)listCoverage.GetNext(posc);
				pClassifier->SmoothCVError+=pTmpClassifier->cverror;
		}
		pClassifier->SmoothCVError=pClassifier->SmoothCVError/listCoverage.GetCount();
		if(pClassifier->SmoothCVError<minSmoothCVError)
		{
			minSmoothCVError=pClassifier->SmoothCVError;
			optimalTrainingStep=index;
		}
		//TRACE("%d %f\n",index,pClassifier->SmoothCVError);
	}
	listCoverage.RemoveAll();
	bSmoothCV=true;
	this->strMessage.Format("The lowest smoothed cross-validation error: %f, which corresponds to %d training steps.",minSmoothCVError,optimalTrainingStep);
	MessageCenter(IDS_MESSAGE_OPTIMALSTEP);
	
	//UpdateAllViews(NULL);
	posa=this->GetFirstViewPosition();
	while(posa)
	{
		CView* pView=(CView*)this->GetNextView(posa);
		if(pView->GetRuntimeClass()->m_lpszClassName=="CFCCView")
		{
			((CFCCView*)pView)->RedrawBitmap();
			pView->Invalidate();
			pView->UpdateWindow();
		}
	}
}

int CFCCDoc::MakePrediction(CObList& list_Compound,int nCount)
{
	POSITION pos;
	CCompound* pCompound;
	CClassifier* pClassifier;
	int nPositive=0,nNegative=0;
	int counter=0;
	pos=list_Compound.GetHeadPosition();
	while(pos )
	{
		counter=0;
		pCompound=(CCompound*)list_Compound.GetNext(pos);
		pCompound->yhat=0;
		POSITION posa=list_Classifier.GetHeadPosition();
		while(posa && counter<nCount)
		{
			counter++;
			pClassifier=(CClassifier*)list_Classifier.GetNext(posa);
			if(pCompound->pFingerprint[pClassifier->bit]==pClassifier->value)
				pCompound->yhat+=pClassifier->weight;
			else
				pCompound->yhat-=pClassifier->weight;
		}
		//TRACE("%s %f\n",pCompound->strCompoundName,pCompound->yhat);
		if(pCompound->yhat>=0)
			nPositive++;
		else
			nNegative++;
		
	}
	//TRACE("#of positive %d # of negative %d\n",nPositive,nNegative);

	return 0;
}

void CCompound::Serialize(CArchive& ar,int length)
{
	int i;
	if (ar.IsStoring())
	{	// storing code
		ar<<label;
		ar<<predict;
		ar<<strCompoundName;
		ar<<weight;
		ar<<yhat;
		ar<<id;
		for(i=0;i<length;i++)
			ar<<pFingerprint[i];
	}
	else
	{	// loading code
		ar>>label;
		ar>>predict;
		ar>>strCompoundName;
		ar>>weight;
		ar>>yhat;
		ar>>id;
		pFingerprint=(bool*)malloc(sizeof(bool)*length);
		for(i=0;i<length;i++)
			ar>>pFingerprint[i];
	}
}

void CClassifier::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar<<bit;
		ar<<value;
		ar<<weight;
		ar<<trainingerror;
		ar<<cverror;
	}
	else
	{	// loading code
		ar>>bit;
		ar>>value;
		ar>>weight;
		ar>>trainingerror;
		ar>>cverror;
	}
}

void CFCCDoc::OnEditEditprojecttitle()
{
	// TODO: Add your command handler code here
	CNewDlg dlg;
	dlg.title=strProjectTitle;
	dlg.positive=strPositiveSet;
	dlg.negative=strNegativeSet;
	dlg.bModify=true;
	if(dlg.DoModal()==IDOK)
	{
		strProjectTitle=dlg.title;
		strPositiveSet=dlg.positive;
		strNegativeSet=dlg.negative;
		MessageCenter(IDS_MESSAGE_EDITDOCUMENT);
		SetTitle(dlg.title);
		POSITION pos=this->GetFirstViewPosition();
		while(pos)
		{
			CView* pView=(CView*)this->GetNextView(pos);
			//if(pView->GetRuntimeClass()->m_lpszClassName=="CNoticeView")
			{
				
				UpdateAllViews(pView);
				pView->Invalidate();
			}
		}
		SetModifiedFlag(true);
	}
}

void CFCCDoc::OnEditExport()
{
	// TODO: Add your command handler code here
	CExpDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		CFile hFile;
		hFile.Open(dlg.pathName,CFile::modeCreate|CFile::modeWrite);
		if(hFile.m_hFile==CFile::hFileNull)
		{
			MessageCenter(IDS_MESSAGE_FAILOPENFILE);
		}
		else
		{
			CString tmpstr;
			if(dlg.bExportModel)
			{
				tmpstr.Format("Totally %d weak classifiers:\r\nID\tbit\tvalue\tweight\r\n",list_Classifier.GetCount());
				hFile.Write(tmpstr,tmpstr.GetLength());
				CClassifier* pClassifier;
				POSITION pos=list_Classifier.GetHeadPosition();
				int index=0;
				while(pos)
				{
					pClassifier=(CClassifier*)list_Classifier.GetNext(pos);
					tmpstr.Format("%d\t%d\t%d\t%f\r\n",index,pClassifier->bit,pClassifier->value,pClassifier->weight);
					index++;
					hFile.Write(tmpstr,tmpstr.GetLength());
				}
				MessageCenter(IDS_MESSAGE_EXPORTMODEL);
			}
			else
			{
				tmpstr.Format("Totally %d testing samples:\r\nID\tname\tclass\traw value\r\n",list_CompoundTest.GetCount());
				CString predclass;
				hFile.Write(tmpstr,tmpstr.GetLength());
				CCompound* pCompound;
				POSITION pos=list_CompoundTest.GetHeadPosition();
				while(pos)
				{
					pCompound=(CCompound*)list_CompoundTest.GetNext(pos);
					if(pCompound->yhat>abs(fRejectBoundary))
						predclass=strPositiveSet;
					if(pCompound->yhat<(-1)*abs(fRejectBoundary))
						predclass=strNegativeSet;
					if(pCompound->yhat<=abs(fRejectBoundary)&&pCompound->yhat>=(-1)*abs(fRejectBoundary))
						predclass="Unknown";
					tmpstr.Format("%d\t%s\t%s\t%f\r\n",pCompound->id,pCompound->strCompoundName,predclass,pCompound->yhat);
					hFile.Write(tmpstr,tmpstr.GetLength());
				}
				MessageCenter(IDS_MESSAGE_EXPORTPREDICTION);
			}
			hFile.Close();
		}
	}
}

void CFCCDoc::MessageCenter(int MessageCode)
{
	if(MessageCode!=IDS_MESSAGE_CUSTOM)
		nHelpIndex=MessageCode;
	int nPositive=0;
	int nNegative=0;
	int nUnknown=0;
	POSITION pos;
	CString tmpstr;
	switch(MessageCode)
	{
	case IDS_MESSAGE_NEWDOCUMENT:
		nMsgCode=1;
		strMessage="A new project is created and initialized.Please load compound fingerprints and set up model training.";
		break;
	case IDS_MESSAGE_EDITDOCUMENT:
		nMsgCode=1;
		strMessage="The property of the project is updated.";
		break;
	case IDS_MESSAGE_WELCOME:
		nMsgCode=3;
		strMessage="Program initialized!";
		break;
	case IDS_MESSAGE_SAVE:
		nMsgCode=1;
		strMessage="The workspace has been saved.";
		break;
	case IDS_MESSAGE_RESTORE:
		nMsgCode=1;
		strMessage="Previously saved workspace restored.";
		break;
	case IDS_MESSAGE_FAILOPENFILE:
		nMsgCode=3;
		strMessage="Unable to open the specified file. Make sure that the file exists and is not opened by another program.";
		break;
	case IDS_MESSAGE_MEMORY:
		nMsgCode=3;
		strMessage="Unable to allocate enough system memory. Maybe the fingerprint is too long.";
		break;
	case IDS_MESSAGE_INCORRECTFORMAT:
		nMsgCode=3;
		strMessage="Program can't detect fingerprint for certain compounds.Make sure the format is correct";
		break;
	case IDS_MESSAGE_INCONSISTENTLENGTH:
		nMsgCode=3;
		strMessage="The fingerprint length is inconsistent for different compounds.";
		break;
	case IDS_MESSAGE_FATALFILEREADING:
		nMsgCode=3;
		strMessage="The program can't understand the file format.";
		break;
	case IDS_MESSAGE_FPREADINGSUCC:
		nMsgCode=1;
		strMessage="Compound fingerprints loaded successfully.";
		break;
	case IDS_MESSAGE_TRAINING:
		nMsgCode=2;
		strMessage="Adaboost training is running!";
		break;
	case IDS_MESSAGE_TRAINFINISHED:
		nMsgCode=1;
		strMessage="Adaboost training is finished successfully!";
		break;
	case IDS_MESSAGE_NOMEMORY:
		nMsgCode=3;
		strMessage="Not enough memory to paint the training error. Please reset the zoom ratio.";
		break;
	case IDS_MESSAGE_SMALLZOOMRATIO:
		nMsgCode=3;
		strMessage="A smaller zoom ratio is set due to the limit of system memory";
		break;
	case IDS_MESSAGE_SELECTCLASSIFIER:
		nMsgCode=1;
		break;
	case IDS_MESSAGE_STARTPRINT:
		nMsgCode=1;
		strMessage="Printing job is undergoing......";
		break;
	case IDS_MESSAGE_ENDPRINT:
		nMsgCode=1;
		strMessage="Printing job is finished successfully!";
		break;
	case IDS_MESSAGE_NOTRAININGDATA:
		nMsgCode=3;
		strMessage="Model training requires both positive and negative data sets. Please load labeled compound fingerprints.";
		break;
	case IDS_MESSAGE_STOPTRAINING:
		nMsgCode=1;
		strMessage="Model training is stopped by user. You can still continue the training later.";
		break;
	case IDS_MESSAGE_EXPORTMODEL:
		nMsgCode=1;
		strMessage="The whole predictive model is saved into file.";
		break;
	case IDS_MESSAGE_EXPORTPREDICTION:
		nMsgCode=1;
		strMessage="The prediction is saved into file.";
		break;
	case IDS_MESSAGE_COPY:
		nMsgCode=1;
		break;
	case IDS_MESSAGE_PASTE:
		nMsgCode=1;
		break;
	case IDS_MESSAGE_NOITEMSELECTED:
		nMsgCode=3;
		strMessage="No items are selected.";
		break;
	case IDS_MESSAGE_FINDCOMPOUND:
		nMsgCode=1;
		break;
	case IDS_MESSAGE_CUSTOM:
		nMsgCode=1;
		break;
	case IDS_MESSAGE_FITWINDOW:
		nMsgCode=1;
		break;
	case IDS_MESSAGE_OPTIMALSTEP:
		nMsgCode=1;
		break;
	case IDS_MESSAGE_MAKEPREDICTION:
		nMsgCode=1;
		nPositive=0;
		nNegative=0;
		nUnknown=0;
		pos=this->list_CompoundTest.GetHeadPosition();
		while(pos)
		{
			CCompound* pCompound=(CCompound*)list_CompoundTest.GetNext(pos);
			if(pCompound->yhat>abs(fRejectBoundary))
				nPositive++;
			if(pCompound->yhat<(-1)*abs(fRejectBoundary))
				nNegative++;
			if(pCompound->yhat<=abs(fRejectBoundary) && pCompound->yhat>=(-1)*abs(fRejectBoundary))
				nUnknown++;
		}
		if(nPositive>1)
			tmpstr.Format("%d  %ss; ",nPositive,strPositiveSet);
		else
			tmpstr.Format("%d  %s; ",nPositive,strPositiveSet);
		strMessage=tmpstr;
		if(nNegative>1)
			tmpstr.Format("%d  %ss; ",nNegative,strNegativeSet);
		else
			tmpstr.Format("%d  %s; ",nNegative,strNegativeSet);
		strMessage+=tmpstr;
		if(nUnknown>1)
			tmpstr.Format("%d  Unknowns",nUnknown);
		else
			tmpstr.Format("%d  Unknown",nUnknown);
		strMessage+=tmpstr;
		break;
	default:
		nMsgCode=3;
		strMessage="Unknown error occurs.";
	}
	pos=this->GetFirstViewPosition();
	while(pos)
	{
		CView* pView=(CView*)this->GetNextView(pos);
		if(pView->GetRuntimeClass()->m_lpszClassName=="CNoticeView")
		{				
			pView->Invalidate(0);
		}
		if(pView->GetRuntimeClass()->m_lpszClassName=="CStartView")
		{				
			((CStartView*)pView)->OnUpdate(0,0,0);
		}
	}
	
}

void CFCCDoc::OnFileLoadfingerprints()
{
	// TODO: Add your command handler code here
	CFpDlg dlg;
	dlg.strPositive=strPositiveSet;
	dlg.strNegative=strNegativeSet;
	int res;
	if(dlg.DoModal()==IDOK)
	{
		TRACE("%d\n",dlg.dataset);
		if(dlg.dataset==1)
			res=LoadFingerprint(this->list_CompoundPositive,1,dlg.pathName,dlg.clean);
		if(dlg.dataset==2)
			res=LoadFingerprint(this->list_CompoundNegative,-1,dlg.pathName,dlg.clean);
		if(dlg.dataset==3)
			res=LoadFingerprint(this->list_CompoundTest,0,dlg.pathName,dlg.clean);
		MessageCenter(res);
		if(res==IDS_MESSAGE_FPREADINGSUCC)
			SetModifiedFlag(true);
		POSITION pos=this->GetFirstViewPosition();
		while(pos)
		{
			CView* pView=(CView*)this->GetNextView(pos);
			//TRACE("%s %s %d\n ",pView->GetRuntimeClass()->m_lpszClassName,"CNoticeView",pView->GetRuntimeClass()->m_lpszClassName=="CNoticeView");
			if(pView->GetRuntimeClass()->m_lpszClassName=="CNoticeView")
			{
				UpdateAllViews(pView);
				pView->Invalidate();
			}
		}
		AfxGetApp()->m_pMainWnd->SendMessageA(WM_COMMAND,ID_VIEW_STARTUPPAGE);
	}
}

void CFCCDoc::OnEditStoptraining()
{
	// TODO: Add your command handler code here
	bThreadExit=true;
	draw_event.SetEvent();
	::WaitForSingleObject(eventThreadExit,INFINITE);
	NumClassifier=list_Classifier.GetCount();
	POSITION pos=GetFirstViewPosition();
	while(pos)
	{
		CView* pview=GetNextView(pos);
		pview->SendMessageA(WM_INITIATEPAINT);
		pview->Invalidate();
		pview->UpdateWindow();
	}
	MessageCenter(IDS_MESSAGE_STOPTRAINING);
}

UINT __cdecl AdaboostTrain(LPVOID pParam)
{
	bAdaboostRunning=true;
	CFCCDoc* pDoc=(CFCCDoc*)pParam;
	POSITION pos;
	CCompound* pCompound;
	CClassifier* pClassifier;
	int i,j,k;double besterror; double error;double sumweight;
	
	draw_event.SetEvent();
	for(k=0;k<pDoc->iti && bThreadExit==false;k++)
	{
		pClassifier=new CClassifier;
		//pDoc->list_Classifier.AddTail(pClassifier);
		pClassifier->bit=0;
		pClassifier->value=1;
		pDoc->Classifier(pClassifier->bit,pClassifier->value,pDoc->list_CompoundPositive);
		pDoc->Classifier(pClassifier->bit,pClassifier->value,pDoc->list_CompoundNegative);
		besterror=0;
		pos=pDoc->list_CompoundPositive.GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)(pDoc->list_CompoundPositive).GetNext(pos);
			if(pCompound->label*pCompound->predict<0)
				besterror+=pCompound->weight;
		}
		pos=(pDoc->list_CompoundNegative).GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)(pDoc->list_CompoundNegative).GetNext(pos);
			if(pCompound->label*pCompound->predict<0)
				besterror+=pCompound->weight;
		}
		for(i=0;i<pDoc->nFpLen;i++)
		{
			for(j=0;j<=1;j++)
			{
				pDoc->Classifier(i,j,pDoc->list_CompoundPositive);
				pDoc->Classifier(i,j,pDoc->list_CompoundNegative);
				error=0;
				pos=pDoc->list_CompoundPositive.GetHeadPosition();
				while(pos)
				{
					pCompound=(CCompound*)(pDoc->list_CompoundPositive).GetNext(pos);
					if(pCompound->label*pCompound->predict<0)
						error+=pCompound->weight;
				}
				pos=(pDoc->list_CompoundNegative).GetHeadPosition();
				while(pos)
				{
					pCompound=(CCompound*)(pDoc->list_CompoundNegative).GetNext(pos);
					if(pCompound->label*pCompound->predict<0)
						error+=pCompound->weight;
				}
				if(error<besterror)
				{
					pClassifier->bit=i;
					pClassifier->value=j;
					besterror=error;
				}
			}
		}
		pDoc->Classifier(pClassifier->bit,pClassifier->value,pDoc->list_CompoundPositive);
		pDoc->Classifier(pClassifier->bit,pClassifier->value,pDoc->list_CompoundNegative);
		error=0;sumweight=0;
		pos=(pDoc->list_CompoundPositive).GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)(pDoc->list_CompoundPositive).GetNext(pos);
			if(pCompound->label*pCompound->predict<0)
				error+=pCompound->weight;
			sumweight+=pCompound->weight;
		}
		pos=pDoc->list_CompoundNegative.GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)(pDoc->list_CompoundNegative).GetNext(pos);
			if(pCompound->label*pCompound->predict<0)
				error+=pCompound->weight;
			sumweight+=pCompound->weight;
		}
		error=error/sumweight;
		pClassifier->weight=log((1-error)/error);
		pos=pDoc->list_CompoundPositive.GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)(pDoc->list_CompoundPositive).GetNext(pos);
			if(pCompound->label*pCompound->predict<0)
				pCompound->weight=pCompound->weight*exp(pClassifier->weight);
			pCompound->yhat+=pCompound->predict*pClassifier->weight;
			if(pCompound->yhat<0)
				pClassifier->trainingerror=pClassifier->trainingerror+1;
		}
		pos=pDoc->list_CompoundNegative.GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)(pDoc->list_CompoundNegative).GetNext(pos);
			if(pCompound->label*pCompound->predict<0)
				pCompound->weight=pCompound->weight*exp(pClassifier->weight);
			pCompound->yhat+=pCompound->predict*pClassifier->weight;
			if(pCompound->yhat>0)
				pClassifier->trainingerror=pClassifier->trainingerror+1;
		}
		pClassifier->trainingerror=pClassifier->trainingerror/(pDoc->list_CompoundPositive.GetCount()+pDoc->list_CompoundNegative.GetCount());
		//TRACE("%d bit %d  value %d weight %f error %f\n",k,pClassifier->bit,pClassifier->value,pClassifier->weight,pClassifier->trainingerror);
		pDoc->Classifier(pClassifier->bit,pClassifier->value,pDoc->list_CompoundCV);
		pos=pDoc->list_CompoundCV.GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)(pDoc->list_CompoundCV).GetNext(pos);
			pCompound->yhat+=pCompound->predict*pClassifier->weight;
			if(pCompound->yhat*pCompound->label<0)
				pClassifier->cverror=pClassifier->cverror+1;
		}
		if(pDoc->list_CompoundCV.IsEmpty())
			pClassifier->cverror=0;
		else
			pClassifier->cverror=pClassifier->cverror/pDoc->list_CompoundCV.GetCount();
		//TRACE("cv error %f\n",pClassifier->cverror);
		pDoc->list_Classifier.AddTail(pClassifier);
		CView* pMainView=NULL;
		pos=pDoc->GetFirstViewPosition();
		while(pos)
		{
			CView* pView=(CView*)pDoc->GetNextView(pos);
			//TRACE("%s %s %d\n ",pView->GetRuntimeClass()->m_lpszClassName,"CFCCView",pView->GetRuntimeClass()->m_lpszClassName=="CFCCView");
			if(pView->GetRuntimeClass()->m_lpszClassName=="CFCCView")
			{
				//pDoc->UpdateAllViews(NULL);
				pMainView=pView;
			}
		}
		if(pMainView)
		{
			::WaitForSingleObject(draw_event,5000);
			draw_event.ResetEvent();
			//pMainView->SendMessageA(WM_REALTIMEDRAW);
			pMainView->PostMessageA(WM_REALTIMEDRAW,pDoc->list_Classifier.GetCount());
		}
		else
		{
			//AfxMessageBox("lost track of main view");
		}
	}
	pDoc->MessageCenter(IDS_MESSAGE_TRAINFINISHED);
	pos=pDoc->GetFirstViewPosition();
	while(pos)
	{
		CView* pView=(CView*)pDoc->GetNextView(pos);
		//TRACE("%s %s %d\n ",pView->GetRuntimeClass()->m_lpszClassName,"CNoticeView",pView->GetRuntimeClass()->m_lpszClassName=="CNoticeView");

		if(pView->GetRuntimeClass()->m_lpszClassName=="CNoticeView")
		{
			//pDoc->UpdateAllViews(NULL);
			pView->Invalidate();
		}
	}
	eventThreadExit.SetEvent();
	
	//restore the menu bar status
	for(i=1;i<=4;i++)
		AfxGetApp()->m_pMainWnd->GetMenu()->GetSubMenu(0)->EnableMenuItem(i,MF_ENABLED|MF_BYPOSITION);
	AfxGetApp()->m_pMainWnd->GetMenu()->GetSubMenu(1)->EnableMenuItem(0,MF_ENABLED|MF_BYPOSITION);
	AfxGetApp()->m_pMainWnd->GetMenu()->GetSubMenu(1)->EnableMenuItem(1,MF_GRAYED|MF_BYPOSITION);
	AfxGetApp()->m_pMainWnd->GetMenu()->GetSubMenu(1)->EnableMenuItem(2,MF_ENABLED|MF_BYPOSITION);

	bAdaboostRunning=false;
	return 0;
}

BOOL CFCCDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	this->firstRun=false;
	TRACE("open document\n");
	if (!CDocument::OnOpenDocument(lpszPathName))
	{
		return FALSE;
	}
	AfxGetApp()->m_pMainWnd->SendMessageA(WM_COMMAND,ID_VIEW_STARTUPPAGE);
	return TRUE;
}

int CFCCDoc::MergeSort(int field, int direction, CObList& list)
{
	CObList listLeft;
	CObList listRight;
	CObject* pCompound;
	CCompound* pCompoundLeft;
	CCompound* pCompoundRight;
	int nTotalCount=list.GetCount();
	int count=0;
	if(nTotalCount<=1)
		return 0;
	if(field!=SORT_BY_ID && field!=SORT_BY_NAME && field!=SORT_BY_VALUE )
	{
		TRACE("Sorting: Wrong Field Parameter %d\n",field);
		return 0;
	}
	if(direction!=DESCEND && direction!= ASCEND)
	{
		TRACE("Sorting: Wrong Direction Parameter %d\n",direction);
		return 0;
	}
	while(count<nTotalCount)
	{
		pCompound=list.RemoveHead();
		if(count<nTotalCount/2)
			listLeft.AddTail(pCompound);
		else
			listRight.AddTail(pCompound);
		count++;
	}
	MergeSort(field,direction,listLeft);
	MergeSort(field,direction,listRight);
	while(!listLeft.IsEmpty() || !listRight.IsEmpty())
	{
		if(listLeft.IsEmpty())
		{
			pCompound=listRight.RemoveHead();
			list.AddTail(pCompound);
			continue;
		}
		if(listRight.IsEmpty())
		{
			pCompound=listLeft.RemoveHead();
			list.AddTail(pCompound);
			continue;
		}
		pCompoundLeft=(CCompound*)listLeft.GetHead();
		pCompoundRight=(CCompound*)listRight.GetHead();
		if(field==SORT_BY_NAME && direction==ASCEND)
		{
			if(pCompoundLeft->strCompoundName < pCompoundRight->strCompoundName)
			{
				pCompound=listLeft.RemoveHead();
				list.AddTail(pCompound);
			}
			else
			{
				pCompound=listRight.RemoveHead();
				list.AddTail(pCompound);
			}
		}
		if(field==SORT_BY_NAME && direction==DESCEND)
		{
			if(pCompoundLeft->strCompoundName > pCompoundRight->strCompoundName)
			{
				pCompound=listLeft.RemoveHead();
				list.AddTail(pCompound);
			}
			else
			{
				pCompound=listRight.RemoveHead();
				list.AddTail(pCompound);
			}
		}
		if(field==SORT_BY_ID && direction==ASCEND)
		{
			if(pCompoundLeft->id < pCompoundRight->id)
			{
				pCompound=listLeft.RemoveHead();
				list.AddTail(pCompound);
			}
			else
			{
				pCompound=listRight.RemoveHead();
				list.AddTail(pCompound);
			}
		}
		if(field==SORT_BY_ID && direction==DESCEND)
		{
			if(pCompoundLeft->id > pCompoundRight->id)
			{
				pCompound=listLeft.RemoveHead();
				list.AddTail(pCompound);
			}
			else
			{
				pCompound=listRight.RemoveHead();
				list.AddTail(pCompound);
			}
		}
		if(field==SORT_BY_VALUE && direction==ASCEND)
		{
			if(pCompoundLeft->yhat < pCompoundRight->yhat)
			{
				pCompound=listLeft.RemoveHead();
				list.AddTail(pCompound);
			}
			else
			{
				pCompound=listRight.RemoveHead();
				list.AddTail(pCompound);
			}
		}
		if(field==SORT_BY_VALUE && direction==DESCEND)
		{
			if(pCompoundLeft->yhat > pCompoundRight->yhat)
			{
				pCompound=listLeft.RemoveHead();
				list.AddTail(pCompound);
			}
			else
			{
				pCompound=listRight.RemoveHead();
				list.AddTail(pCompound);
			}
		}
	}
	return 1;
}

void CFCCDoc::OnUpdateFileOpen(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!bAdaboostRunning);
}

void CFCCDoc::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!bAdaboostRunning);
}

void CFCCDoc::OnUpdateFileSaveAs(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!bAdaboostRunning);
}

void CFCCDoc::OnUpdateFileLoadfingerprints(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!bAdaboostRunning);
}

void CFCCDoc::OnUpdateEditStoptraining(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(bAdaboostRunning);
}

void CFCCDoc::OnUpdateAdaboostPredicting(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!bAdaboostRunning);
}

void CFCCDoc::OnUpdateAdaboostModeltraining(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!bAdaboostRunning);
}

BOOL CFCCDoc::SaveModified()
{
	// TODO: Add your specialized code here and/or call the base class
	if(bAdaboostRunning)
	{
		int res=AfxMessageBox("FCC is developing a predictive model. The workspace cannot be saved now. Are you sure you want to exit?",MB_OKCANCEL|MB_ICONQUESTION	);
		if(res==IDOK)
		{
			SetModifiedFlag(false);
			OnEditStoptraining();
			return CDocument::SaveModified();
		}
		else
			return false;
	}
	return CDocument::SaveModified();
}

void CFCCDoc::OnHelpContents()
{
	// TODO: Add your command handler code here
	//The following code is for Glide Data Analysis to study the effect  of M on testing errors.
	/*int array[]={2,4,8,16,32,64,128,256,512,1024,2048,4096,8192};
	int errorcount[13];
	int totalcount;
	
	CString fingerprint;
	int batch;
	
	fingerprint="Molprint";
	batch=10;
	CString tmpstr;

	tmpstr.Format("C:\\Documents and Settings\\Chao\\My Documents\\Glide\\%d\\%d %s balanced weight.FCC",batch,batch,fingerprint);
	this->OnOpenDocument(tmpstr);
	POSITION pos;
	CClassifier* pClassifier;
	CCompound * pCompound;
	TRACE("Training Error\n");
	for(int i=0;i<13;i++)
	{
		pos=list_Classifier.FindIndex(array[i]-1);
		if(pos)
		{
			pClassifier=(CClassifier*)list_Classifier.GetAt(pos);
			TRACE("%f\n",pClassifier->trainingerror);
		}
	}
	TRACE("Testing Error\n");
	this->fRejectBoundary = 0.0f;
	if(fingerprint=="Molprint")
		fingerprint="Molprint2d";
	tmpstr.Format("C:\\Documents and Settings\\Chao\\My Documents\\Glide\\%d\\%s_agonist_test.fp",batch,fingerprint);

	LoadFingerprint(this->list_CompoundTest,0,tmpstr,true);
	totalcount=list_CompoundTest.GetCount();
	for(int i=0;i<13;i++)
		errorcount[i]=0;
	for(int i=0;i<13;i++)
	{
		MakePrediction(list_CompoundTest,array[i]);
		pos=list_CompoundTest.GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)list_CompoundTest.GetNext(pos);
			if(pCompound->yhat<0)
				errorcount[i]++;
		}
		//TRACE("%d\n",errorcount[i]);
	}

	tmpstr.Format("C:\\Documents and Settings\\Chao\\My Documents\\Glide\\%d\\%s_antagonist_test.fp",batch,fingerprint);

	LoadFingerprint(this->list_CompoundTest,0,tmpstr,true);
	totalcount+=list_CompoundTest.GetCount();
	for(int i=0;i<13;i++)
	{
		MakePrediction(list_CompoundTest,array[i]);
		pos=list_CompoundTest.GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)list_CompoundTest.GetNext(pos);
			if(pCompound->yhat >= 0)
				errorcount[i]++;
		}
		TRACE("%f\n",1.0*errorcount[i]/totalcount);
	}*/
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//The following code is to automatically training and testing CB selective models
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	/*CCompound* pCompound;
	POSITION pos;

	int batchNum=1;
	iti=1000;
	CString FpName="Unityfs";
	CString FpFileName;
	CString ProjectFileName;

	for(batchNum=1;batchNum<21;batchNum++)
	{
		OnNewDocument();
		strProjectTitle.Format("%d %s",batchNum, FpName);
		strPositiveSet="Selective";
		strNegativeSet="Non-Selective";

		ProjectFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\CB2 Selectivity\\%d\\%d %s.FCC",batchNum,batchNum,FpName);
		FpFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\CB2 Selectivity\\%d\\%s CB2 Selective Train.fp",batchNum,FpName);
		LoadFingerprint(list_CompoundPositive,1,FpFileName,0);
		FpFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\CB2 Selectivity\\%d\\%s CB2 Non-Selective Train.fp",batchNum,FpName);
		LoadFingerprint(list_CompoundNegative,-1,FpFileName,0);

			
		int count;
			count=list_CompoundPositive.GetCount()+list_CompoundNegative.GetCount();
		pos=list_CompoundPositive.GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)(list_CompoundPositive).GetNext(pos);
			pCompound->weight=1.0/count;
			pCompound->yhat=0;
		}
		pos=list_CompoundNegative.GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)(list_CompoundNegative).GetNext(pos);
			pCompound->weight=1.0/count;
			pCompound->yhat=0;
		}
			
		MessageCenter(IDS_MESSAGE_TRAINING);
		AfxGetApp()->m_pMainWnd->SendMessageA(WM_COMMAND,ID_VIEW_TRAININGERROR);
		UpdateAllViews(NULL);
		SetModifiedFlag(true);
		NumClassifier=list_Classifier.GetCount()+iti;
		pos=this->GetFirstViewPosition();
		while(pos)
		{
			CView* pView=(CView*)this->GetNextView(pos);
			
			if(pView->GetRuntimeClass()->m_lpszClassName=="CFCCView")
			{
				pView->SendMessage(WM_INITIATEPAINT);
			}
		}
		bThreadExit=false;
		bAdaboostRunning=true;
		CWinThread* pThread=AfxBeginThread(AdaboostTrain,this);
		MSG message;
		while(bAdaboostRunning)
		{
			if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
				::TranslateMessage(&message);
				::DispatchMessage(&message);
			}
		}
		OnSaveDocument(ProjectFileName);

		FpFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\CB2 Selectivity\\%d\\%s CB2 Selective Test.fp",batchNum,FpName);
		LoadFingerprint(list_CompoundTest,0,FpFileName,1);

		fRejectBoundary=0.0;
		//TRACE("num of classifiers %d\n",iti);
		MakePrediction(list_CompoundTest,iti);
		AfxGetApp()->m_pMainWnd->SendMessageA(WM_COMMAND,ID_VIEW_PREDICTION);

		pos=list_CompoundTest.GetHeadPosition();
		int nPositive=0;
		int nNegative=0;
		while(pos )
		{
			int counter=0;
			pCompound=(CCompound*)list_CompoundTest.GetNext(pos);
			pCompound->yhat=0;
			POSITION posa=list_Classifier.GetHeadPosition();
			while(posa && counter<iti)
			{
				counter++;
				CClassifier* pClassifier=(CClassifier*)list_Classifier.GetNext(posa);
				if(pCompound->pFingerprint[pClassifier->bit]==pClassifier->value)
					pCompound->yhat+=pClassifier->weight;
				else
					pCompound->yhat-=pClassifier->weight;
			}
			if(pCompound->yhat>=0)
				nPositive++;
			else
				nNegative++;
		}
		TRACE("Batch Number %d  : Selective   %d  Predicted as Selective ;      Misses    %d\n",batchNum,nPositive,nNegative);

		FpFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\CB2 Selectivity\\%d\\%s CB2 Non-Selective Test.fp",batchNum,FpName);
		LoadFingerprint(list_CompoundTest,0,FpFileName,1);

		fRejectBoundary=0.0;
		//TRACE("num of classifiers %d\n",iti);
		MakePrediction(list_CompoundTest,iti);
		AfxGetApp()->m_pMainWnd->SendMessageA(WM_COMMAND,ID_VIEW_PREDICTION);

		pos=list_CompoundTest.GetHeadPosition();
		nPositive=0;
		nNegative=0;
		while(pos )
		{
			int counter=0;
			pCompound=(CCompound*)list_CompoundTest.GetNext(pos);
			pCompound->yhat=0;
			POSITION posa=list_Classifier.GetHeadPosition();
			while(posa && counter<iti)
			{
				counter++;
				CClassifier* pClassifier=(CClassifier*)list_Classifier.GetNext(posa);
				if(pCompound->pFingerprint[pClassifier->bit]==pClassifier->value)
					pCompound->yhat+=pClassifier->weight;
				else
					pCompound->yhat-=pClassifier->weight;
			}
			if(pCompound->yhat>=0)
				nPositive++;
			else
				nNegative++;
		}
		TRACE("Batch Number %d  : Non-Selective   %d  Predicted as Selective ;       %d   Correct ones\n",batchNum, nPositive,nNegative);
	}*/
	

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Automatic cross-validation, model training and make prediction
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	CCompound* pCompound;
	POSITION pos;

	int batchNum=1;
	int minsteps=100;
	int maxsteps=2000;
	CString FpName="fp2";
	CString FpFileName;
	CString ProjectFileName;
	CString className;
	CString logFileName;
	CString logstring;
	className="CB2";
	
	CFile logfile;
	logFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\%s Selectivity\\logfile.txt",className);
	logfile.Open(logFileName,CFile::modeCreate|CFile::modeWrite);

	for(batchNum=1;batchNum<21;batchNum++)
	{
		OnNewDocument();
		iti=maxsteps;
		strProjectTitle.Format("%d %s",batchNum, FpName);
		strPositiveSet="Selective";
		strNegativeSet="Non-Selective";
		

		ProjectFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\%s Selectivity\\%d\\%d %s cv.FCC",className,batchNum,batchNum,FpName);
		FpFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\%s Selectivity\\%d\\%s %s Selective Train.fp",className,batchNum,FpName,className);
		LoadFingerprint(list_CompoundPositive,1,FpFileName,0);
		FpFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\%s Selectivity\\%d\\%s %s Non-Selective Train.fp",className,batchNum,FpName,className);
		LoadFingerprint(list_CompoundNegative,-1,FpFileName,0);
			
		int count;
		cv=true;
		if(cv)
			{
				count=list_CompoundNegative.GetCount()*0.3;
				for(int i=0;i<count;i++)
				{
					pCompound=(CCompound*)list_CompoundNegative.RemoveTail();
					pCompound->yhat=0;
					list_CompoundCV.AddHead(pCompound);
				}
				count=list_CompoundPositive.GetCount()*0.3;
				for(int i=0;i<count;i++)
				{
					pCompound=(CCompound*)list_CompoundPositive.RemoveTail();
					pCompound->yhat=0;
					list_CompoundCV.AddHead(pCompound);
				}
			}

		count=list_CompoundPositive.GetCount()+list_CompoundNegative.GetCount();
		pos=list_CompoundPositive.GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)(list_CompoundPositive).GetNext(pos);
			pCompound->weight=1.0/count;
			pCompound->yhat=0;
		}
		pos=list_CompoundNegative.GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)(list_CompoundNegative).GetNext(pos);
			pCompound->weight=1.0/count;
			pCompound->yhat=0;
		}
			
		MessageCenter(IDS_MESSAGE_TRAINING);
		AfxGetApp()->m_pMainWnd->SendMessageA(WM_COMMAND,ID_VIEW_TRAININGERROR);
		UpdateAllViews(NULL);
		SetModifiedFlag(true);
		NumClassifier=list_Classifier.GetCount()+iti;
		pos=this->GetFirstViewPosition();
		while(pos)
		{
			CView* pView=(CView*)this->GetNextView(pos);
			
			if(pView->GetRuntimeClass()->m_lpszClassName=="CFCCView")
			{
				pView->SendMessage(WM_INITIATEPAINT);
			}
		}
		bThreadExit=false;
		bAdaboostRunning=true;
		CWinThread* pThread=AfxBeginThread(AdaboostTrain,this);
		MSG message;
		while(bAdaboostRunning)
		{
			if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
				::TranslateMessage(&message);
				::DispatchMessage(&message);
			}
		}

		int nSmoothWinLength;
		nSmoothWinLength=20;
		CObList listCoverage;
		POSITION posa,posb,posc;
		CClassifier* pClassifier;
		CClassifier* pTmpClassifier;
		int counter=0;
		int index=0;
		posa=list_Classifier.GetHeadPosition();
		posb=posa;
		float minSmoothCVError=1.0;
		int optimalTrainingStep;
		while(posb && counter<nSmoothWinLength/2)
		{
			pClassifier=(CClassifier*)list_Classifier.GetNext(posb);
			listCoverage.AddTail(pClassifier);
			counter++;
		}

		while(posa)
		{
			pClassifier=(CClassifier*)list_Classifier.GetNext(posa);
			index++;
			if(posb)
			{
				pTmpClassifier=(CClassifier*)list_Classifier.GetNext(posb);
				listCoverage.AddTail(pTmpClassifier);
				if(listCoverage.GetCount()>nSmoothWinLength)
					listCoverage.RemoveHead();
			}
			else
			{
				if(list_Classifier.GetCount()>nSmoothWinLength)
				{
					listCoverage.RemoveHead();
				}
				if(list_Classifier.GetCount()<=nSmoothWinLength && list_Classifier.GetCount()>nSmoothWinLength/2)
				{
					if(index>nSmoothWinLength/2)
						listCoverage.RemoveHead();
				}
				
			}
			pClassifier->SmoothCVError=0;
			posc=listCoverage.GetHeadPosition();
			while(posc)
			{
					pTmpClassifier=(CClassifier*)listCoverage.GetNext(posc);
					pClassifier->SmoothCVError+=pTmpClassifier->cverror;
			}
			pClassifier->SmoothCVError=pClassifier->SmoothCVError/listCoverage.GetCount();
			if(pClassifier->SmoothCVError<minSmoothCVError && index>=minsteps)
			{
				minSmoothCVError=pClassifier->SmoothCVError;
				optimalTrainingStep=index;
			}
		}
		listCoverage.RemoveAll();
		bSmoothCV=true;
		this->strMessage.Format("The lowest smoothed cross-validation error: %f, which corresponds to %d training steps.",minSmoothCVError,optimalTrainingStep);
		MessageCenter(IDS_MESSAGE_OPTIMALSTEP);
		
		posa=this->GetFirstViewPosition();
		while(posa)
		{
			CView* pView=(CView*)this->GetNextView(posa);
			if(pView->GetRuntimeClass()->m_lpszClassName=="CFCCView")
			{
				((CFCCView*)pView)->RedrawBitmap();
				pView->Invalidate();
				pView->UpdateWindow();
			}
		}
		cv=false;
		//AfxMessageBox("cv done");
		OnNewDocument();
		iti=optimalTrainingStep;
		TRACE("Batch Number %d  Optimal Training Steps: %d\n",batchNum, iti);
		strProjectTitle.Format("%d %s",batchNum, FpName);
		strPositiveSet="Selective";
		strNegativeSet="Non-Selective";

		ProjectFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\%s Selectivity\\%d\\%d %s cv.FCC",className,batchNum,batchNum,FpName);
		FpFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\%s Selectivity\\%d\\%s %s Selective Train.fp",className,batchNum,FpName,className);
		LoadFingerprint(list_CompoundPositive,1,FpFileName,0);
		FpFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\%s Selectivity\\%d\\%s %s Non-Selective Train.fp",className,batchNum,FpName,className);
		LoadFingerprint(list_CompoundNegative,-1,FpFileName,0);
			
		count=list_CompoundPositive.GetCount()+list_CompoundNegative.GetCount();
		pos=list_CompoundPositive.GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)(list_CompoundPositive).GetNext(pos);
			pCompound->weight=1.0/count;
			pCompound->yhat=0;
		}
		pos=list_CompoundNegative.GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)(list_CompoundNegative).GetNext(pos);
			pCompound->weight=1.0/count;
			pCompound->yhat=0;
		}
			
		MessageCenter(IDS_MESSAGE_TRAINING);
		AfxGetApp()->m_pMainWnd->SendMessageA(WM_COMMAND,ID_VIEW_TRAININGERROR);
		UpdateAllViews(NULL);
		SetModifiedFlag(true);
		NumClassifier=list_Classifier.GetCount()+iti;
		pos=this->GetFirstViewPosition();
		while(pos)
		{
			CView* pView=(CView*)this->GetNextView(pos);
			
			if(pView->GetRuntimeClass()->m_lpszClassName=="CFCCView")
			{
				pView->SendMessage(WM_INITIATEPAINT);
			}
		}
		bThreadExit=false;
		bAdaboostRunning=true;
		pThread=AfxBeginThread(AdaboostTrain,this);
		while(bAdaboostRunning)
		{
			if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
				::TranslateMessage(&message);
				::DispatchMessage(&message);
			}
		}
		OnSaveDocument(ProjectFileName);

		FpFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\%s Selectivity\\%d\\%s %s Selective Test.fp",className,batchNum,FpName,className);
		LoadFingerprint(list_CompoundTest,0,FpFileName,1);

		fRejectBoundary=0.0;
		//TRACE("num of classifiers %d\n",iti);
		MakePrediction(list_CompoundTest,iti);
		AfxGetApp()->m_pMainWnd->SendMessageA(WM_COMMAND,ID_VIEW_PREDICTION);

		pos=list_CompoundTest.GetHeadPosition();
		int nPositive=0;
		int nNegative=0;
		while(pos )
		{
			int counter=0;
			pCompound=(CCompound*)list_CompoundTest.GetNext(pos);
			pCompound->yhat=0;
			POSITION posa=list_Classifier.GetHeadPosition();
			while(posa && counter<iti)
			{
				counter++;
				CClassifier* pClassifier=(CClassifier*)list_Classifier.GetNext(posa);
				if(pCompound->pFingerprint[pClassifier->bit]==pClassifier->value)
					pCompound->yhat+=pClassifier->weight;
				else
					pCompound->yhat-=pClassifier->weight;
			}
			if(pCompound->yhat>=0)
				nPositive++;
			else
				nNegative++;
		}
		TRACE("Batch Number %d  : Selective   %d  Predicted as Selective ;      Misses    %d\n",batchNum,nPositive,nNegative);
		logstring.Format("%d\t%d\t%d\t",batchNum,optimalTrainingStep,nPositive);
		logfile.Write(logstring,logstring.GetLength());
		

		FpFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\%s Selectivity\\%d\\%s %s Non-Selective Test.fp",className,batchNum,FpName,className);
		LoadFingerprint(list_CompoundTest,0,FpFileName,1);

		fRejectBoundary=0.0;
		//TRACE("num of classifiers %d\n",iti);
		MakePrediction(list_CompoundTest,iti);
		AfxGetApp()->m_pMainWnd->SendMessageA(WM_COMMAND,ID_VIEW_PREDICTION);

		pos=list_CompoundTest.GetHeadPosition();
		nPositive=0;
		nNegative=0;
		while(pos )
		{
			int counter=0;
			pCompound=(CCompound*)list_CompoundTest.GetNext(pos);
			pCompound->yhat=0;
			POSITION posa=list_Classifier.GetHeadPosition();
			while(posa && counter<iti)
			{
				counter++;
				CClassifier* pClassifier=(CClassifier*)list_Classifier.GetNext(posa);
				if(pCompound->pFingerprint[pClassifier->bit]==pClassifier->value)
					pCompound->yhat+=pClassifier->weight;
				else
					pCompound->yhat-=pClassifier->weight;
			}
			if(pCompound->yhat>=0)
				nPositive++;
			else
				nNegative++;
		}
		TRACE("Batch Number %d  : Non-Selective   %d  Predicted as Selective ;       %d   Correct ones\n",batchNum, nPositive,nNegative);
		logstring.Format("%d\r\n",nPositive);
		logfile.Write(logstring,logstring.GetLength());
	}
	logfile.Close();*/
	


//////////////////////////////////////////////////////////////////
//automatic k-fold cross-validation, model training and prediction
//////////////////////////////////////////////////////////////////
	/*CCompound* pCompound;
	POSITION pos;

	int batchNum=1;
	int minsteps=50;
	int maxsteps=200;
	int kfold=5;
	CString FpName="MACCSfs";
	CString FpFileName;
	CString ProjectFileName;
	float* CVErrorAgg;
	CString popupmsg;
	CString tmpstr;
	CVErrorAgg=(float*)malloc(sizeof(float)*maxsteps);

	for(batchNum=1;batchNum<21;batchNum++)
	{
		OnNewDocument();
		iti=maxsteps;
		strProjectTitle.Format("%d %s",batchNum, FpName);
		strPositiveSet="Selective";
		strNegativeSet="Non-Selective";

		ProjectFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\CB2 Selectivity\\%d\\%d %s cv.FCC",batchNum,batchNum,FpName);
		FpFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\CB2 Selectivity\\%d\\%s CB2 Selective Train.fp",batchNum,FpName);
		LoadFingerprint(list_CompoundPositive,1,FpFileName,0);
		FpFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\CB2 Selectivity\\%d\\%s CB2 Non-Selective Train.fp",batchNum,FpName);
		LoadFingerprint(list_CompoundNegative,-1,FpFileName,0);
		for(int i=0;i<maxsteps;i++)
			CVErrorAgg[i]=0;
			
		int count;
		cv=true;
		for(int fold=0;fold<kfold;fold++)
		{
			if(cv)
			{
				count=list_CompoundNegative.GetCount()*1.0/kfold;
				for(int i=0;i<count;i++)
				{
					pCompound=(CCompound*)list_CompoundNegative.RemoveTail();
					pCompound->yhat=0;
					list_CompoundCV.AddHead(pCompound);
				}
				count=list_CompoundPositive.GetCount()*1.0/kfold;
				for(int i=0;i<count;i++)
				{
					pCompound=(CCompound*)list_CompoundPositive.RemoveTail();
					pCompound->yhat=0;
					list_CompoundCV.AddHead(pCompound);
				}
			}

			count=list_CompoundPositive.GetCount()+list_CompoundNegative.GetCount();
			pos=list_CompoundPositive.GetHeadPosition();
			while(pos)
			{
				pCompound=(CCompound*)(list_CompoundPositive).GetNext(pos);
				pCompound->weight=1.0/count;
				pCompound->yhat=0;
			}
			pos=list_CompoundNegative.GetHeadPosition();
			while(pos)
			{
				pCompound=(CCompound*)(list_CompoundNegative).GetNext(pos);
				pCompound->weight=1.0/count;
				pCompound->yhat=0;
			}
				
			MessageCenter(IDS_MESSAGE_TRAINING);
			AfxGetApp()->m_pMainWnd->SendMessageA(WM_COMMAND,ID_VIEW_TRAININGERROR);
			UpdateAllViews(NULL);
			SetModifiedFlag(true);
			NumClassifier=list_Classifier.GetCount()+iti;
			pos=this->GetFirstViewPosition();
			while(pos)
			{
				CView* pView=(CView*)this->GetNextView(pos);
				
				if(pView->GetRuntimeClass()->m_lpszClassName=="CFCCView")
				{
					pView->SendMessage(WM_INITIATEPAINT);
				}
			}
			bThreadExit=false;
			bAdaboostRunning=true;
			CWinThread* pThread=AfxBeginThread(AdaboostTrain,this);
			MSG message;
			while(bAdaboostRunning)
			{
				if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
					::TranslateMessage(&message);
					::DispatchMessage(&message);
				}
			}
			while(!list_CompoundCV.IsEmpty())
			{
				pCompound=(CCompound*)list_CompoundCV.RemoveTail();
				if(pCompound->label>0)
					list_CompoundPositive.AddHead(pCompound);
				else
					list_CompoundNegative.AddHead(pCompound);
			}
			count=0;
			while(!list_Classifier.IsEmpty())
			{
				CClassifier* pClassifier=(CClassifier*)list_Classifier.RemoveHead();
				if(count<maxsteps)
				{
					CVErrorAgg[count]+=pClassifier->cverror;
					count++;
				}
				delete pClassifier;
			}
			//AfxMessageBox("one fold is done.");
		}
	//	for(int i=0;i<maxsteps;i++)
	//		TRACE("%d\tCVErrorAgg[i]=%f\n",i,CVErrorAgg[i]);
		
		iti=maxsteps;
		
		strProjectTitle.Format("%d %s",batchNum, FpName);
		strPositiveSet="Selective";
		strNegativeSet="Non-Selective";

		count=list_CompoundPositive.GetCount()+list_CompoundNegative.GetCount();
		pos=list_CompoundPositive.GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)(list_CompoundPositive).GetNext(pos);
			pCompound->weight=1.0/count;
			pCompound->yhat=0;
		}
		pos=list_CompoundNegative.GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)(list_CompoundNegative).GetNext(pos);
			pCompound->weight=1.0/count;
			pCompound->yhat=0;
		}
			
		MessageCenter(IDS_MESSAGE_TRAINING);
		AfxGetApp()->m_pMainWnd->SendMessageA(WM_COMMAND,ID_VIEW_TRAININGERROR);
		UpdateAllViews(NULL);
		SetModifiedFlag(true);
		NumClassifier=list_Classifier.GetCount()+iti;
		pos=this->GetFirstViewPosition();
		while(pos)
		{
			CView* pView=(CView*)this->GetNextView(pos);
			
			if(pView->GetRuntimeClass()->m_lpszClassName=="CFCCView")
			{
				pView->SendMessage(WM_INITIATEPAINT);
			}
		}
		bThreadExit=false;
		bAdaboostRunning=true;
		AfxBeginThread(AdaboostTrain,this);
		MSG message;
		while(bAdaboostRunning)
		{
			if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
				::TranslateMessage(&message);
				::DispatchMessage(&message);
			}
		}
		pos=list_Classifier.GetHeadPosition();
		count=0;
		while(pos)
		{
			CClassifier* pClassifier=(CClassifier*)list_Classifier.GetNext(pos);
			pClassifier->cverror=CVErrorAgg[count]/kfold;
			count++;
		}

		
		int nSmoothWinLength;
		nSmoothWinLength=20;
		CObList listCoverage;
		POSITION posa,posb,posc;
		CClassifier* pClassifier;
		CClassifier* pTmpClassifier;
		int counter=0;
		int index=0;
		posa=list_Classifier.GetHeadPosition();
		posb=posa;
		float minSmoothCVError=1.0;
		int optimalTrainingStep;
		while(posb && counter<nSmoothWinLength/2)
		{
			pClassifier=(CClassifier*)list_Classifier.GetNext(posb);
			listCoverage.AddTail(pClassifier);
			counter++;
		}

		while(posa)
		{
			pClassifier=(CClassifier*)list_Classifier.GetNext(posa);
			index++;
			if(posb)
			{
				pTmpClassifier=(CClassifier*)list_Classifier.GetNext(posb);
				listCoverage.AddTail(pTmpClassifier);
				if(listCoverage.GetCount()>nSmoothWinLength)
					listCoverage.RemoveHead();
			}
			else
			{
				if(list_Classifier.GetCount()>nSmoothWinLength)
				{
					listCoverage.RemoveHead();
				}
				if(list_Classifier.GetCount()<=nSmoothWinLength && list_Classifier.GetCount()>nSmoothWinLength/2)
				{
					if(index>nSmoothWinLength/2)
						listCoverage.RemoveHead();
				}
			}
			pClassifier->SmoothCVError=0;
			posc=listCoverage.GetHeadPosition();
			while(posc)
			{
					pTmpClassifier=(CClassifier*)listCoverage.GetNext(posc);
					pClassifier->SmoothCVError+=pTmpClassifier->cverror;
			}
			pClassifier->SmoothCVError=pClassifier->SmoothCVError/listCoverage.GetCount();
			if(pClassifier->SmoothCVError<minSmoothCVError && index>=minsteps)
			{
				minSmoothCVError=pClassifier->SmoothCVError;
				optimalTrainingStep=index;
			}
		}
		listCoverage.RemoveAll();
		bSmoothCV=true;
		this->strMessage.Format("The lowest smoothed cross-validation error: %f, which corresponds to %d training steps.",minSmoothCVError,optimalTrainingStep);
		MessageCenter(IDS_MESSAGE_OPTIMALSTEP);
		
		posa=this->GetFirstViewPosition();
		while(posa)
		{
			CView* pView=(CView*)this->GetNextView(posa);
			if(pView->GetRuntimeClass()->m_lpszClassName=="CFCCView")
			{
				((CFCCView*)pView)->RedrawBitmap();
				pView->Invalidate();
				pView->UpdateWindow();
			}
		}
		TRACE("Batch Number %d  Optimal Training Steps: %d\n",batchNum, optimalTrainingStep);
		popupmsg.Format("Batch Number %d  Optimal Training Steps: %d\n",batchNum, optimalTrainingStep);


		FpFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\CB2 Selectivity\\%d\\%s CB2 Selective Test.fp",batchNum,FpName);
		LoadFingerprint(list_CompoundTest,0,FpFileName,1);

		fRejectBoundary=0.0;
		//TRACE("num of classifiers %d\n",iti);
		MakePrediction(list_CompoundTest,optimalTrainingStep);
		//AfxGetApp()->m_pMainWnd->SendMessageA(WM_COMMAND,ID_VIEW_PREDICTION);

		pos=list_CompoundTest.GetHeadPosition();
		int nPositive=0;
		int nNegative=0;
		while(pos )
		{
			int counter=0;
			pCompound=(CCompound*)list_CompoundTest.GetNext(pos);
			pCompound->yhat=0;
			POSITION posa=list_Classifier.GetHeadPosition();
			while(posa && counter<optimalTrainingStep)
			{
				counter++;
				CClassifier* pClassifier=(CClassifier*)list_Classifier.GetNext(posa);
				if(pCompound->pFingerprint[pClassifier->bit]==pClassifier->value)
					pCompound->yhat+=pClassifier->weight;
				else
					pCompound->yhat-=pClassifier->weight;
			}
			if(pCompound->yhat>=0)
				nPositive++;
			else
				nNegative++;
		}
		TRACE("Batch Number %d  : Selective   %d  Predicted as Selective ;      Misses    %d\n",batchNum,nPositive,nNegative);
		tmpstr.Format("Batch Number %d  : Selective   %d  Predicted as Selective ;      Misses    %d\n",batchNum,nPositive,nNegative);
		popupmsg+=tmpstr;


		FpFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\CB2 Selectivity\\%d\\%s CB2 Non-Selective Test.fp",batchNum,FpName);
		LoadFingerprint(list_CompoundTest,0,FpFileName,1);

		fRejectBoundary=0.0;
		//TRACE("num of classifiers %d\n",iti);
		MakePrediction(list_CompoundTest,optimalTrainingStep);
		//AfxGetApp()->m_pMainWnd->SendMessageA(WM_COMMAND,ID_VIEW_PREDICTION);

		pos=list_CompoundTest.GetHeadPosition();
		nPositive=0;
		nNegative=0;
		while(pos )
		{
			int counter=0;
			pCompound=(CCompound*)list_CompoundTest.GetNext(pos);
			pCompound->yhat=0;
			POSITION posa=list_Classifier.GetHeadPosition();
			while(posa && counter<optimalTrainingStep)
			{
				counter++;
				CClassifier* pClassifier=(CClassifier*)list_Classifier.GetNext(posa);
				if(pCompound->pFingerprint[pClassifier->bit]==pClassifier->value)
					pCompound->yhat+=pClassifier->weight;
				else
					pCompound->yhat-=pClassifier->weight;
			}
			if(pCompound->yhat>=0)
				nPositive++;
			else
				nNegative++;
		}
		TRACE("Batch Number %d  : Non-Selective   %d  Predicted as Selective ;       %d   Correct ones\n",batchNum, nPositive,nNegative);
		tmpstr.Format("Batch Number %d  : Non-Selective   %d  Predicted as Selective ;       %d   Correct ones\n",batchNum, nPositive,nNegative);
		popupmsg+=tmpstr;
		AfxMessageBox(popupmsg);

		
	}
	delete CVErrorAgg;
	*/

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//The following code is to automatically training and testing CB selective models with different numbers of ensemble decision stumps
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////////////
	//The following section tests purity and recall rate versus training steps, using CB1 CB2 selective datasets and four types of fingerprints
	////////////////////////////////////////////////////////////////////////////
	/*
	CCompound* pCompound;
	POSITION pos;

	int batchNum=1;
	iti=1000;
	CString FpName="Unity";//Molprint 3L,Maccs,FP2,Unity
	CString tag("CB1");
	int Steps[]={10,25,50,75,100,150,200,250,300,400,500,600,700,800,900,1000};//This is for FP2 and Unity
	//int Steps[]={10,15,20,25,30,40,50,60,80,100,125,150,200,250,300};//This is for Maccs
	//int Steps[]={10,25,50,75,100,150,200,250,300,400,500,600,700,800,900,1000,1200,1400,1600,1800,2000};//This is for Molprint 2D
	CString FpFileName;
	CString ProjectFileName;
	TRACE("\nSteps\t");
	for(int k=0;k<sizeof(Steps)/sizeof(int);k++)
		TRACE("%d\t",Steps[k]);
	TRACE("\n");
	for(batchNum=1;batchNum<21;batchNum++)
	{
		OnNewDocument();
		strProjectTitle.Format("%d %s",batchNum, FpName);
		strPositiveSet="Selective";
		strNegativeSet="Non-Selective";

		ProjectFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\%s Selectivity\\%d\\%d %s.FCC",tag,batchNum,batchNum,FpName);
		FpFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\%s Selectivity\\%d\\%s %s Selective Train.fp",tag,batchNum,FpName,tag);
		LoadFingerprint(list_CompoundPositive,1,FpFileName,0);
		FpFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\%s Selectivity\\%d\\%s %s Non-Selective Train.fp",tag,batchNum,FpName,tag);
		LoadFingerprint(list_CompoundNegative,-1,FpFileName,0);
			
		int count;
			count=list_CompoundPositive.GetCount()+list_CompoundNegative.GetCount();
		pos=list_CompoundPositive.GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)(list_CompoundPositive).GetNext(pos);
			pCompound->weight=1.0/count;
			pCompound->yhat=0;
		}
		pos=list_CompoundNegative.GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)(list_CompoundNegative).GetNext(pos);
			pCompound->weight=1.0/count;
			pCompound->yhat=0;
		}
			
		MessageCenter(IDS_MESSAGE_TRAINING);
		AfxGetApp()->m_pMainWnd->SendMessageA(WM_COMMAND,ID_VIEW_TRAININGERROR);
		UpdateAllViews(NULL);
		SetModifiedFlag(true);
		NumClassifier=list_Classifier.GetCount()+iti;
		pos=this->GetFirstViewPosition();
		while(pos)
		{
			CView* pView=(CView*)this->GetNextView(pos);
			
			if(pView->GetRuntimeClass()->m_lpszClassName=="CFCCView")
			{
				pView->SendMessage(WM_INITIATEPAINT);
			}
		}
		bThreadExit=false;
		bAdaboostRunning=true;
		CWinThread* pThread=AfxBeginThread(AdaboostTrain,this);
		MSG message;
		while(bAdaboostRunning)
		{
			if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
				::TranslateMessage(&message);
				::DispatchMessage(&message);
			}
		}
		//OnSaveDocument(ProjectFileName);
		
		
		TRACE("\nBatch Number %d\n",batchNum);
		FpFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\%s Selectivity\\%d\\%s %s Selective Test.fp",tag,batchNum,FpName,tag);
		LoadFingerprint(list_CompoundTest,0,FpFileName,1);
		
		TRACE("True Positives:\t");
		for(int k=0;k<sizeof(Steps)/sizeof(int);k++)
		{
			fRejectBoundary=0.0;
			//TRACE("num of classifiers %d\n",iti);
			//AfxGetApp()->m_pMainWnd->SendMessageA(WM_COMMAND,ID_VIEW_PREDICTION);

			pos=list_CompoundTest.GetHeadPosition();
			int nPositive=0;
			int nNegative=0;
			while(pos )
			{
				int counter=0;
				pCompound=(CCompound*)list_CompoundTest.GetNext(pos);
				pCompound->yhat=0;
				POSITION posa=list_Classifier.GetHeadPosition();
				while(posa && counter<Steps[k])
				{
					counter++;
					CClassifier* pClassifier=(CClassifier*)list_Classifier.GetNext(posa);
					if(pCompound->pFingerprint[pClassifier->bit]==pClassifier->value)
						pCompound->yhat+=pClassifier->weight;
					else
						pCompound->yhat-=pClassifier->weight;
				}
				if(pCompound->yhat>=0)
					nPositive++;
				else
					nNegative++;
			}
			TRACE("%d\t",nPositive);
		}
		TRACE("\n");
		FpFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\%s Selectivity\\%d\\%s %s Non-Selective Test.fp",tag,batchNum,FpName,tag);
		LoadFingerprint(list_CompoundTest,0,FpFileName,1);
		TRACE("False Positives:\t");
		for(int k=0;k<sizeof(Steps)/sizeof(int);k++)
		{
			fRejectBoundary=0.0;
			//TRACE("num of classifiers %d\n",iti);
			//MakePrediction(list_CompoundTest,Steps[k]);
			//AfxGetApp()->m_pMainWnd->SendMessageA(WM_COMMAND,ID_VIEW_PREDICTION);

			pos=list_CompoundTest.GetHeadPosition();
			int nPositive=0;
			int nNegative=0;
			while(pos )
			{
				int counter=0;
				pCompound=(CCompound*)list_CompoundTest.GetNext(pos);
				pCompound->yhat=0;
				POSITION posa=list_Classifier.GetHeadPosition();
				while(posa && counter<Steps[k])
				{
					counter++;
					CClassifier* pClassifier=(CClassifier*)list_Classifier.GetNext(posa);
					if(pCompound->pFingerprint[pClassifier->bit]==pClassifier->value)
						pCompound->yhat+=pClassifier->weight;
					else
						pCompound->yhat-=pClassifier->weight;
				}
				if(pCompound->yhat>=0)
					nPositive++;
				else
					nNegative++;
			}
			TRACE("%d\t",nPositive);
		}
	}
	*/
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//The following code is to automatically training and testing CB selective models. Predictions are exported to files for plotting ROC curve.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	CCompound* pCompound;
	POSITION pos;

	int batchNum=1;
	iti=400;
	CString FpName="Maccs";
	CString FpFileName;
	CString ProjectFileName;
	CString CBTag;
	CBTag="CB2";
	CFile SummaryFile;
	CFile PredictionFile;
	CString SummaryFileName;
	CString PredictionFileName;
	CString OutputString;
	SummaryFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\%s Selectivity\\Summary %s.txt",CBTag,FpName);
	SummaryFile.Open(SummaryFileName,CFile::modeCreate|CFile::modeWrite);
	if(SummaryFile.m_hFile==CFile::hFileNull)
		AfxMessageBox("Failed to create summary text file");
	for(batchNum=1;batchNum<21;batchNum++)
	{
		OnNewDocument();
		strProjectTitle.Format("%d %s",batchNum, FpName);
		strPositiveSet="Selective";
		strNegativeSet="Non-Selective";

		ProjectFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\%s Selectivity\\%d\\%d %s.FCC",CBTag,batchNum,batchNum,FpName);
		FpFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\%s Selectivity\\%d\\%s %s Selective Train.fp",CBTag,batchNum,FpName,CBTag);
		LoadFingerprint(list_CompoundPositive,1,FpFileName,0);
		FpFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\%s Selectivity\\%d\\%s %s Non-Selective Train.fp",CBTag,batchNum,FpName,CBTag);
		LoadFingerprint(list_CompoundNegative,-1,FpFileName,0);

			
		int count;
			count=list_CompoundPositive.GetCount()+list_CompoundNegative.GetCount();
		pos=list_CompoundPositive.GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)(list_CompoundPositive).GetNext(pos);
			pCompound->weight=1.0/count;
			pCompound->yhat=0;
		}
		pos=list_CompoundNegative.GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)(list_CompoundNegative).GetNext(pos);
			pCompound->weight=1.0/count;
			pCompound->yhat=0;
		}
			
		MessageCenter(IDS_MESSAGE_TRAINING);
		AfxGetApp()->m_pMainWnd->SendMessageA(WM_COMMAND,ID_VIEW_TRAININGERROR);
		UpdateAllViews(NULL);
		SetModifiedFlag(true);
		NumClassifier=list_Classifier.GetCount()+iti;
		pos=this->GetFirstViewPosition();
		while(pos)
		{
			CView* pView=(CView*)this->GetNextView(pos);
			
			if(pView->GetRuntimeClass()->m_lpszClassName=="CFCCView")
			{
				pView->SendMessage(WM_INITIATEPAINT);
			}
		}
		bThreadExit=false;
		bAdaboostRunning=true;
		CWinThread* pThread=AfxBeginThread(AdaboostTrain,this);
		MSG message;
		while(bAdaboostRunning)
		{
			if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
				::TranslateMessage(&message);
				::DispatchMessage(&message);
			}
		}
		//OnSaveDocument(ProjectFileName);

		FpFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\%s Selectivity\\%d\\%s %s Selective Test.fp",CBTag,batchNum,FpName,CBTag);
		LoadFingerprint(list_CompoundTest,0,FpFileName,1);

		fRejectBoundary=0.0;
		//TRACE("num of classifiers %d\n",iti);
		MakePrediction(list_CompoundTest,iti);
		AfxGetApp()->m_pMainWnd->SendMessageA(WM_COMMAND,ID_VIEW_PREDICTION);

		pos=list_CompoundTest.GetHeadPosition();
		int nPositive=0;
		int nNegative=0;
		while(pos )
		{
			int counter=0;
			pCompound=(CCompound*)list_CompoundTest.GetNext(pos);
			pCompound->yhat=0;
			POSITION posa=list_Classifier.GetHeadPosition();
			while(posa && counter<iti)
			{
				counter++;
				CClassifier* pClassifier=(CClassifier*)list_Classifier.GetNext(posa);
				if(pCompound->pFingerprint[pClassifier->bit]==pClassifier->value)
					pCompound->yhat+=pClassifier->weight;
				else
					pCompound->yhat-=pClassifier->weight;
			}
			if(pCompound->yhat>=0)
				nPositive++;
			else
				nNegative++;
		}
		OutputString.Format("%d\t",nPositive);
		SummaryFile.Write(OutputString,OutputString.GetLength());

		PredictionFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\%s Selectivity\\Decision Boundary\\%d %s %s Selective.txt",CBTag,batchNum,FpName,CBTag);
		PredictionFile.Open(PredictionFileName,CFile::modeCreate|CFile::modeWrite);
		pos=list_CompoundTest.GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)list_CompoundTest.GetNext(pos);
			OutputString.Format("%d\t%s\t%f\r\n",pCompound->id,pCompound->strCompoundName,pCompound->yhat);
			PredictionFile.Write(OutputString,OutputString.GetLength());
		}
		PredictionFile.Close();
		TRACE("Batch Number %d  : Selective   %d  Predicted as Selective ;      Misses    %d\n",batchNum,nPositive,nNegative);

		FpFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\%s Selectivity\\%d\\%s %s Non-Selective Test.fp",CBTag,batchNum,FpName,CBTag);
		LoadFingerprint(list_CompoundTest,0,FpFileName,1);

		fRejectBoundary=0.0;
		//TRACE("num of classifiers %d\n",iti);
		MakePrediction(list_CompoundTest,iti);
		AfxGetApp()->m_pMainWnd->SendMessageA(WM_COMMAND,ID_VIEW_PREDICTION);

		pos=list_CompoundTest.GetHeadPosition();
		nPositive=0;
		nNegative=0;
		while(pos )
		{
			int counter=0;
			pCompound=(CCompound*)list_CompoundTest.GetNext(pos);
			pCompound->yhat=0;
			POSITION posa=list_Classifier.GetHeadPosition();
			while(posa && counter<iti)
			{
				counter++;
				CClassifier* pClassifier=(CClassifier*)list_Classifier.GetNext(posa);
				if(pCompound->pFingerprint[pClassifier->bit]==pClassifier->value)
					pCompound->yhat+=pClassifier->weight;
				else
					pCompound->yhat-=pClassifier->weight;
			}
			if(pCompound->yhat>=0)
				nPositive++;
			else
				nNegative++;
		}
		OutputString.Format("%d\n",nPositive);
		SummaryFile.Write(OutputString,OutputString.GetLength());

		PredictionFileName.Format("C:\\Documents and Settings\\Chao\\My Documents\\Selectivity\\%s Selectivity\\Decision Boundary\\%d %s %s Non-Selective.txt",CBTag,batchNum,FpName,CBTag);
		PredictionFile.Open(PredictionFileName,CFile::modeCreate|CFile::modeWrite);
		pos=list_CompoundTest.GetHeadPosition();
		while(pos)
		{
			pCompound=(CCompound*)list_CompoundTest.GetNext(pos);
			OutputString.Format("%d\t%s\t%f\r\n",pCompound->id,pCompound->strCompoundName,pCompound->yhat);
			PredictionFile.Write(OutputString,OutputString.GetLength());
		}
		PredictionFile.Close();
		TRACE("Batch Number %d  : Non-Selective   %d  Predicted as Selective ;       %d   Correct ones\n",batchNum, nPositive,nNegative);
	}
	SummaryFile.Close();

}
