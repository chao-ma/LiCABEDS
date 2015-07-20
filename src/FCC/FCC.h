// FCC.h : main header file for the FCC application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CFCCApp:
// See FCC.cpp for the implementation of this class
//

class CFCCApp : public CWinApp
{
public:
	CFCCApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
};

extern CFCCApp theApp;