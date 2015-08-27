
// DeepVoid.h : main header file for the DeepVoid application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CDeepVoidApp:
// See DeepVoid.cpp for the implementation of this class
//
class CMainFrame;
class CImageDoc;
class CDeepVoidApp : public CWinAppEx
{
public:
	CDeepVoidApp();

// attributes
public:
	CMultiDocTemplate * m_pTemplateImgDoc;
	vector <cam_data> m_vCams;	// zhaokunz, 20140302, all the views to be handled
	CMainFrame * m_pMainFrame;	// zhaokunz, 20140302, direct mainframe pointer, no need to convert pointer type
	vector <CImageDoc *> m_vPImgCocs;

	ULONG_PTR m_gdiplusToken; // zhaokunz, 20140226, add GDI+

	CString m_pathStereoImg0;
	CString m_pathStereoImg1;
	CString m_pathStereoParam0;
	CString m_pathStereoParam1;
	int m_nPixDownSample;
	int m_idxStereoMethod;
	int m_idxLayout;
	int m_idxSGMImplementation;
	
	int m_idxDSItype;
	int m_minDisparity;
	int m_maxDisparity;
	double m_P1_ZZKSGM;
	double m_P2_ZZKSGM;
	int m_idxPaths_ZZKSGM;
	int m_idxSubpixel_ZZKSGM;
	int m_threshConstCheck_ZZKSGM;

	int m_idxSetZeroDisparity;
	int m_idxRectifyScaling;

	// parameters for Zhu's method
	int m_idxSimilarityMeasure_Zhu;
	int m_wndSize_Zhu;
	double m_maxNormAng_Zhu;
	int m_nPatchMatchIter_Zhu;
	double m_deFactor_Zhu;
	int m_nRandSamp_Zhu;
	double m_imgUncertainty_Zhu;

// operations
public:
	CDocument * CreateDocument(CMultiDocTemplate * pTemplate, CFrameWnd * pWnd = NULL);

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileAddimages();
	afx_msg void OnFileOpenimages();
//	afx_msg void OnFun();
	afx_msg void OnFullrun();
	afx_msg void OnSfm();
	afx_msg void OnDense();
	afx_msg void OnMysgm();
	afx_msg void OnDepthcheck();
	afx_msg void OnTestdata();
	afx_msg void OnPropvisi();
	afx_msg void OnRunfixedvisi();
	afx_msg void OnNewpatchmatch();
	afx_msg void OnStrechatestdata();
	afx_msg void OnZhoudata();
	afx_msg void OnFeaturematching();
	afx_msg void OnXudata();
	afx_msg void OnStereo();
	afx_msg void OnCalibsimu();
};

extern CDeepVoidApp theApp;
