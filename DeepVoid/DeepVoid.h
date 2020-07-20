
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

	std::vector<cv::Mat> m_imgsOriginal; // 20200719，还是把选择的图像都读进内存
	std::vector<cv::Mat> m_imgsProcessed;// 20200719，这是内容相较于原图已经发生变化的图，比方说画了提取的特征点在上面的图

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

	viz::Viz3d m_wnd3d; // 20160403, 3D visualizer window
	viz::WCloud m_ptcloud = viz::WCloud(Mat(1,1,CV_64FC3));

	// 20200630
	SfM_ZZK::PointCloud m_map_pointcloud;
	SfM_ZZK::MultiTracks m_map_tracks;

	// 20161029, the calibration of the camera used to do MVS
	double m_fx, m_fy, m_s, m_cx, m_cy;
	double m_k1, m_k2, m_k3, m_k4, m_k5;
	int m_distType;
	CString m_pathImageCalibration;

	CString m_strOut;
	vector<CString> m_vImgPaths;
	vector<vector<int>> m_vIdxSupports;
	vector<CloudPoint> m_cloud_old;

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
	afx_msg void OnDsbasimu();
	afx_msg void OnSfmSimu();
	afx_msg void OnFeaturetrackingexp();
	afx_msg void OnSfmexp();
	afx_msg void OnDsbasimuNoise();
	afx_msg void OnDsbasimuObjpts();
	afx_msg void OnDsbasimuImgnum();
	afx_msg void OnDsbasimuObsnum();
	afx_msg void On3dreconstructionParametersettings();
	afx_msg void On3dreconstructionNetworkorientationandsparsereconstruction();
	afx_msg void On3dreconstructionDensereconstruction();
	afx_msg void On3dview();
};

extern CDeepVoidApp theApp;

extern HCURSOR hCursorArrow;
extern HCURSOR hCursorCross;
extern HCURSOR hCursorGrab;