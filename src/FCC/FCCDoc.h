// FCCDoc.h : interface of the CFCCDoc class
//


#pragma once
#define SORT_BY_ID 1
#define SORT_BY_VALUE 2
#define SORT_BY_NAME 3
#define DESCEND 0
#define ASCEND 1



// CCompound command target
UINT AdaboostTrain(LPVOID pParam);
class CCompound : public CObject
{
public:
	DECLARE_DYNCREATE(CCompound)
	CCompound();
	virtual ~CCompound();
	CString strCompoundName;
	bool* pFingerprint;
	bool bSelect;
	double weight;
	int label;
	int id;
	int predict;
	double yhat;
	virtual void Serialize(CArchive& ar,int length);
};



class CClassifier : public CObject
{
public:
	DECLARE_DYNCREATE(CClassifier)
	CClassifier();
	virtual ~CClassifier();
	int bit;
	int value;
	double weight;
	virtual void Serialize(CArchive& ar);
	float trainingerror;
	float cverror;
	float SmoothCVError;
};





class CFCCDoc : public CDocument
{
protected: // create from serialization only
	CFCCDoc();
	DECLARE_DYNCREATE(CFCCDoc)
	bool firstRun;
	bool cv;
	int iti;
	int NumClassifier;
	

// Attributes
public:
	CObList list_CompoundPositive;
	CObList list_CompoundNegative;
	CObList list_CompoundTest;
	CObList list_CompoundCV;
	CObList list_Classifier;
	int nFpLen;
	CString strPositiveSet;
	CString strNegativeSet;
	CString strProjectTitle;
	int nMsgCode;
	CString strMessage;
	float fRejectBoundary;
	int version;
	int nHelpIndex;

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CFCCDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnFileOpen();
	virtual void DeleteContents();
	int LoadFingerprint(CObList& list_Compound, int label, CString fileName,bool clean);
	int Classifier(int bit, int value, CObList& list_Compound);
	int ViewFingerprint(CObList& list_Compound);
	afx_msg void OnAdaboostModeltraining();
	afx_msg void OnAdaboostPredicting();
	int MakePrediction(CObList& list_Compound,int nCount);
	afx_msg void OnEditEditprojecttitle();
	void MessageCenter(int MessageCode);
	afx_msg void OnFileLoadfingerprints();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	int MergeSort(int field, int direction, CObList& list);
	afx_msg void OnEditStoptraining();
	afx_msg void OnUpdateFileOpen(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileLoadfingerprints(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditStoptraining(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAdaboostPredicting(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAdaboostModeltraining(CCmdUI *pCmdUI);
	afx_msg void OnEditExport();
protected:
	virtual BOOL SaveModified();
public:
	afx_msg void OnHelpContents();

	//version 2 variables and functions
	bool bSmoothCV;
	afx_msg void OnEditparameteropt();
};
