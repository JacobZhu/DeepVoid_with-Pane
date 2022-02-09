
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

	std::vector<cv::Mat> m_imgsOriginal; // 20200719�����ǰ�ѡ���ͼ�񶼶����ڴ�
	std::vector<cv::Mat> m_imgsProcessed;// 20200719���������������ԭͼ�Ѿ������仯��ͼ���ȷ�˵������ȡ���������������ͼ

	std::vector<CString> m_vPathImgs;	// 20220129����������ͼ��������·��
	std::vector<CString> m_vNameImgs;	// 20220129����������ͼ���ļ����ƣ�����β׺��
	CString m_pathDirOut;				// 20220129���������ļ��е�·��
	CString m_nameDirOut;				// 20220129���������ļ�������

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
	viz::WCloud m_ptcloud = viz::WCloud(Mat(1, 1, CV_64FC3));

	// 20200630
	SfM_ZZK::PointCloud m_mapPointCloud; // 20220127��map<trackID, CloudPoint>������SfM֮���γɵġ�ȫ�֡�ϡ����ơ�
//	SfM_ZZK::MultiTracks m_mapTracks; // 20220127��map<trackID, map<imgID, <featID, bInlier>>>����������������ٺ��γɵġ�ȫ�֡������켣������SfM֮�����е�bInlier�ḳ��ֵ��
	SfM_ZZK::MultiTracksWithFlags m_mapTracks; // 20220202�������ݽṹmap<trackID, map<imgID, <featID, vectorFlags>>>����������������ٺ��γɵġ�ȫ�֡������켣������SfM֮�����е�bInlier�ḳ��ֵ��
	SfM_ZZK::PairWise_F_matches_pWrdPts m_mapPairwiseFMatchesWrdPts; // 20200622, {<i, j>, <<F, matches>, wrdpts>}

	// 20161029, the calibration of the camera used to do MVS
	double m_fx, m_fy, m_s, m_cx, m_cy;
	double m_k1, m_k2, m_k3, m_k4, m_k5;
	int m_distType;
	CString m_pathImageCalibration;

	CString m_strOut;
	vector<CString> m_vImgPaths;
	vector<vector<int>> m_vIdxSupports;
	vector<CloudPoint> m_cloud_old;

	BOOL m_bNoneImages;
	BOOL m_bSfMFeatsReady;	// 20220127��ÿ��ͼ�Ƿ���ȡ����ͼ��������ÿ�������Ƿ��������������������������Ƿ��ܹ���ʼ����feature tracking�ˡ�
	BOOL m_bTracksReady;	// 20220127���Ƿ������feature tracking�����������Ƿ��ܹ���ʼ����SfM�ˡ�
	BOOL m_b3DReady_sparse;	// 20220127���Ƿ������ϡ����ά�ؽ������������Ƿ��ܹ���ʼ������ά��ʾ�ˡ�
	BOOL m_b3DReady_dense;	// 20220127���Ƿ�������ܼ���ά�ؽ���
	BOOL m_b3DViewOn;		// 20220127���Ƿ��ڽ�����ά������ʾ�����������Ƿ��ܹ�������ά��ͼ��ͼ�ˡ�

	// parameters for sift feature extraction
	int m_nfeaturesSift;
	int m_nOctaveLayersSift;
	double m_contrastThresholdSift;
	double m_edgeThresholdSift;
	double m_sigmaSift;

	// parameters for FAST feature extraction
	double m_sizeFast;
	int m_thresholdFast;
	bool m_nonmaxSuppressionFast;
	int m_typeFast;

	// ���������߶Ⱥͷ�������Ĳ���
	double m_thresholdOffset;
	int m_rMax;

	int m_nSfMFeatures;
	int m_nPrptFeatures;

//	SfM_ZZK::PairWise_F_matches_pWrdPts m_mapPairwiseFMatchesWrdPts; // 20200622, {<i, j>, <<F, matches>, wrdpts>}
//	SfM_ZZK::MultiTracks m_mapTracks;

	// ����ͼ����ƥ����Ҫ�Ĳ�������
	bool m_fmbOptim;					// input:	whether optimize F using Golden Standard algorithm or not
	double m_fmThreshRatioTest;			// input:	the ratio threshold for ratio test
	double m_fmThreshMinInlierRatio;	// input:	the allowed minimum ratio of inliers
	double m_fmThreshP2L;				// input:	the distance threshold between point and epiline, used in RANSAC stage
	double m_fmThreshConf;				// input:	specifying a desirable level of confidence (probability) that the estimated matrix is correct
	int m_fmMaxIter;					// input:	the maximum number of iterations
	double m_fmxEps;					// input:	threshold
	double m_fmfEps;					// input:	threshold
	int m_prptTh;

	// SfM��Ҫ�Ĳ�������
	double m_threshRpjErrRO;			// ��Զ���ʱҪ�õ�����ͶӰ�в���ֵ
	double m_optsLM[5];					// Levenberg-Marquartz������Ҫ��һЩ����
	double m_threshRpjErrInlier;		// EO��BA�������ж��Ƿ��ڵ����ͶӰ�в���ֵ
	double m_threshRatioInlier;			// EO�����������С�ڵ㼯ռ��
	int m_methodRO;						// ָ��RO�������͡�0:PIRO; 1:extract [R|t] from essential matrix, just like recoverPose() from opencv
	double m_threshMeanAngRO;			// RO��Ҫ�õ������ƽ���������ֵ
	int m_nMinInilier;					// ���ٵ��иø���ͼ��۲⵽�õ�Żᱻ���
	int m_nMaxIter;						// ����������
	bool m_bRefineImgPts;				// �Ƿ��Ż��������
	int m_nFlagPerImgPt;				// һ�������켣��ÿ�����Ԥ����ٸ���־λ��һ������ 1 ����־λ����ָ������㵱ǰ�Ƿ��ж�Ϊ�ڵ�
	int m_wndSizeImgptRefine;			// �������ƥ���Ż�ʱ���õĴ��ڴ�С��һ��Ϊ������
	double m_xEpsMPGC; 					// input: threshold
	double m_fEpsMPGC; 					// input: threshold

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
	afx_msg void OnFeaturesExtractsift();
	afx_msg void OnFeaturesExtractfast();
	afx_msg void OnFeaturesSiftfast();
	afx_msg void OnFeaturesDeleteall();
	afx_msg void On0settingsFeatures();
	afx_msg void On0settings2featurematching();
	afx_msg void On2featurematching();
	afx_msg void On1featuresGenfeaturesforsfm();
	afx_msg void OnUpdateFeaturesExtractsift(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFeaturesExtractfast(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFeaturesSiftfast(CCmdUI *pCmdUI);
	afx_msg void OnUpdate1featuresGenfeaturesforsfm(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFeaturesDeleteall(CCmdUI *pCmdUI);
	afx_msg void OnUpdate2featurematching(CCmdUI *pCmdUI);
	afx_msg void On1featuresManual();
	afx_msg void OnUpdate1featuresManual(CCmdUI *pCmdUI);
	afx_msg void OnCapture3dview();
	afx_msg void OnUpdate3dview(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCapture3dview(CCmdUI *pCmdUI);
	afx_msg void On3sfmIncrementalsfm();
	afx_msg void OnUpdate3sfmIncrementalsfm(CCmdUI *pCmdUI);
	afx_msg void OnUpdate3sfmGlobalsfm(CCmdUI *pCmdUI);
};

extern CDeepVoidApp theApp;

extern HCURSOR hCursorArrow;
extern HCURSOR hCursorCross;
extern HCURSOR hCursorGrab;