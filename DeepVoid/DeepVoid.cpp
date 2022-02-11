
// DeepVoid.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "DeepVoid.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "DeepVoidDoc.h"
#include "DeepVoidView.h"

#include "ImageDoc.h"
#include "ImageView.h"

#include "DlgStereoFiles.h"
#include "DlgStereoMethods.h"
#include "DlgSGMSettings.h"
#include "DlgSGMZZKSettings.h"
#include "DlgZhuMethodSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HCURSOR hCursorArrow;
HCURSOR hCursorCross;
HCURSOR hCursorGrab;

// CDeepVoidApp

BEGIN_MESSAGE_MAP(CDeepVoidApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CDeepVoidApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_FILE_ADDIMAGES, &CDeepVoidApp::OnFileAddimages)
	ON_COMMAND(ID_FILE_OPENIMAGES, &CDeepVoidApp::OnFileOpenimages)
	/*ON_COMMAND(ID_FUN, &CDeepVoidApp::OnFun)*/
	ON_COMMAND(ID_FULLRUN, &CDeepVoidApp::OnFullrun)
	ON_COMMAND(ID_SFM, &CDeepVoidApp::OnSfm)
	ON_COMMAND(ID_DENSE, &CDeepVoidApp::OnDense)
	ON_COMMAND(ID_MYSGM, &CDeepVoidApp::OnMysgm)
	ON_COMMAND(ID_DEPTHCHECK, &CDeepVoidApp::OnDepthcheck)
	ON_COMMAND(ID_TESTDATA, &CDeepVoidApp::OnTestdata)
	ON_COMMAND(ID_PROPVISI, &CDeepVoidApp::OnPropvisi)
	ON_COMMAND(ID_RUNFIXEDVISI, &CDeepVoidApp::OnRunfixedvisi)
	ON_COMMAND(ID_NEWPATCHMATCH, &CDeepVoidApp::OnNewpatchmatch)
	ON_COMMAND(ID_STRECHATESTDATA, &CDeepVoidApp::OnStrechatestdata)
	ON_COMMAND(ID_ZHOUDATA, &CDeepVoidApp::OnZhoudata)
	ON_COMMAND(ID_FEATUREMATCHING, &CDeepVoidApp::OnFeaturematching)
	ON_COMMAND(ID_XUDATA, &CDeepVoidApp::OnXudata)
	ON_COMMAND(ID_STEREO, &CDeepVoidApp::OnStereo)
	ON_COMMAND(ID_CALIBSIMU, &CDeepVoidApp::OnCalibsimu)
	ON_COMMAND(ID_DSBASIMU, &CDeepVoidApp::OnDsbasimu)
	ON_COMMAND(ID_SFM_SIMU, &CDeepVoidApp::OnSfmSimu)
	ON_COMMAND(ID_FEATURETRACKINGEXP, &CDeepVoidApp::OnFeaturetrackingexp)
	ON_COMMAND(ID_SfMEXP, &CDeepVoidApp::OnSfmexp)
	ON_COMMAND(ID_DSBASIMU_NOISE, &CDeepVoidApp::OnDsbasimuNoise)
	ON_COMMAND(ID_DSBASIMU_OBJPTS, &CDeepVoidApp::OnDsbasimuObjpts)
	ON_COMMAND(ID_DSBASIMU_IMGNUM, &CDeepVoidApp::OnDsbasimuImgnum)
	ON_COMMAND(ID_DSBASIMU_OBSNUM, &CDeepVoidApp::OnDsbasimuObsnum)
	ON_COMMAND(ID_3DRECONSTRUCTION_PARAMETERSETTINGS, &CDeepVoidApp::On3dreconstructionParametersettings)
	ON_COMMAND(ID_3DRECONSTRUCTION_NETWORKORIENTATIONANDSPARSERECONSTRUCTION, &CDeepVoidApp::On3dreconstructionNetworkorientationandsparsereconstruction)
	ON_COMMAND(ID_3DRECONSTRUCTION_DENSERECONSTRUCTION, &CDeepVoidApp::On3dreconstructionDensereconstruction)
	ON_COMMAND(ID_3DVIEW, &CDeepVoidApp::On3dview)
	ON_COMMAND(ID_FEATURES_EXTRACTSIFT, &CDeepVoidApp::OnFeaturesExtractsift)
	ON_COMMAND(ID_FEATURES_EXTRACTFAST, &CDeepVoidApp::OnFeaturesExtractfast)
	ON_COMMAND(ID_FEATURES_SIFTFAST, &CDeepVoidApp::OnFeaturesSiftfast)
	ON_COMMAND(ID_FEATURES_DELETEALL, &CDeepVoidApp::OnFeaturesDeleteall)
	ON_COMMAND(ID_0SETTINGS_FEATURES, &CDeepVoidApp::On0settingsFeatures)
	ON_COMMAND(ID_0SETTINGS_2FEATUREMATCHING, &CDeepVoidApp::On0settings2featurematching)
	ON_COMMAND(ID_2FEATUREMATCHING, &CDeepVoidApp::On2featurematching)
	ON_COMMAND(ID_1FEATURES_GENFEATURESFORSFM, &CDeepVoidApp::On1featuresGenfeaturesforsfm)
	ON_UPDATE_COMMAND_UI(ID_FEATURES_EXTRACTSIFT, &CDeepVoidApp::OnUpdateFeaturesExtractsift)
	ON_UPDATE_COMMAND_UI(ID_FEATURES_EXTRACTFAST, &CDeepVoidApp::OnUpdateFeaturesExtractfast)
	ON_UPDATE_COMMAND_UI(ID_FEATURES_SIFTFAST, &CDeepVoidApp::OnUpdateFeaturesSiftfast)
	ON_UPDATE_COMMAND_UI(ID_1FEATURES_GENFEATURESFORSFM, &CDeepVoidApp::OnUpdate1featuresGenfeaturesforsfm)
	ON_UPDATE_COMMAND_UI(ID_FEATURES_DELETEALL, &CDeepVoidApp::OnUpdateFeaturesDeleteall)
	ON_UPDATE_COMMAND_UI(ID_2FEATUREMATCHING, &CDeepVoidApp::OnUpdate2featurematching)
	ON_COMMAND(ID_1FEATURES_MANUAL, &CDeepVoidApp::On1featuresManual)
	ON_UPDATE_COMMAND_UI(ID_1FEATURES_MANUAL, &CDeepVoidApp::OnUpdate1featuresManual)
	ON_COMMAND(ID_CAPTURE_3DVIEW, &CDeepVoidApp::OnCapture3dview)
	ON_UPDATE_COMMAND_UI(ID_3DVIEW, &CDeepVoidApp::OnUpdate3dview)
	ON_UPDATE_COMMAND_UI(ID_CAPTURE_3DVIEW, &CDeepVoidApp::OnUpdateCapture3dview)
	ON_COMMAND(ID_3SFM_INCREMENTALSFM, &CDeepVoidApp::On3sfmIncrementalsfm)
	ON_UPDATE_COMMAND_UI(ID_3SFM_INCREMENTALSFM, &CDeepVoidApp::OnUpdate3sfmIncrementalsfm)
	ON_UPDATE_COMMAND_UI(ID_3SFM_GLOBALSFM, &CDeepVoidApp::OnUpdate3sfmGlobalsfm)
	ON_COMMAND(ID_TEST_SBAANDDBSBACOMPARISON, &CDeepVoidApp::OnTestSbaanddbsbacomparison)
	ON_UPDATE_COMMAND_UI(ID_TEST_SBAANDDBSBACOMPARISON, &CDeepVoidApp::OnUpdateTestSbaanddbsbacomparison)
END_MESSAGE_MAP()


// CDeepVoidApp construction

CDeepVoidApp::CDeepVoidApp()
{
	m_bHiColorIcons = TRUE;

	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("DeepVoid.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

	m_pTemplateImgDoc = NULL;
	m_pMainFrame = NULL;

	m_pathStereoImg0 = "";
	m_pathStereoImg1 = "";
	m_pathStereoParam0 = "";
	m_pathStereoParam1 = "";
	m_idxStereoMethod = 0;
	m_idxLayout = 0;
	m_idxSGMImplementation = 0;
	m_nPixDownSample = 1;

	m_idxDSItype = 0;
	m_minDisparity = 1;
	m_maxDisparity = 10;
	m_P1_ZZKSGM = 15;
	m_P2_ZZKSGM = 100;
	m_idxPaths_ZZKSGM = 0;
	m_idxSubpixel_ZZKSGM = 0;
	m_threshConstCheck_ZZKSGM = 1;

	m_idxSetZeroDisparity = 0;
	m_idxRectifyScaling = 0;

	m_idxSimilarityMeasure_Zhu = 0;
	m_wndSize_Zhu = 5;
	m_maxNormAng_Zhu = 80;
	m_nPatchMatchIter_Zhu = 4;
	m_deFactor_Zhu = 0.25;
	m_nRandSamp_Zhu = 5;
	m_imgUncertainty_Zhu = 0.5;

	m_bNoneImages = TRUE;
	m_bSfMFeatsReady = FALSE;
	m_bTracksReady = FALSE;
	m_b3DReady_sparse = FALSE;
	m_b3DReady_dense = FALSE;
	m_b3DViewOn = FALSE;

//	m_wnd3d = viz::Viz3d("3D Window"); // give the 3D visualizer window a name

	m_fx = m_fy = m_s = m_cx = m_cy = 0;
	m_k1 = m_k2 = m_k3 = m_k4 = m_k5 = 0;
	m_distType = 1; // D.C.Brown's
	m_pathImageCalibration = "";

	m_strOut = "";
	m_pathDirOut = "";			// 20220129，默认是空的，如果选择了图像后才会有路径
	m_nameDirOut = "results";	// 20220129，这里名称一改就都改了

	// parameters for sift feature extraction
	m_nfeaturesSift = 0;	// The number of best features to retain. The features are ranked by their scores (measured in SIFT algorithm as the local contrast)
	m_nOctaveLayersSift = 3;	// The number of layers in each octave. 3 is the value used in D.Lowe paper.The number of octaves is computed automatically from the image resolution.
	m_contrastThresholdSift = 0.03/*0.01*//*0.04*/;	// The contrast threshold used to filter out weak features in semi-uniform (low - contrast) regions.The larger the threshold, the less features are produced by the detector.
	m_edgeThresholdSift = 10;	// The threshold used to filter out edge-like features. Note that the its meaning is different from the contrastThreshold, i.e.the larger the edgeThreshold, the less features are filtered out(more features are retained).
	m_sigmaSift = 1.6;	// The sigma of the Gaussian applied to the input image at the octave \#0. If your image is captured with a weak camera with soft lenses, you might want to reduce the number.

	// parameters for FAST feature extraction
	m_sizeFast = 15.0;
	m_thresholdFast = 20; // threshold on difference between intensity of the central pixel and pixels of a circle around this pixel.
	m_nonmaxSuppressionFast = true; // if true, non-maximum suppression is applied to detected corners (keypoints).
	m_typeFast = cv::FastFeatureDetector::TYPE_9_16; // one of the three neighborhoods as defined in the paper: FastFeatureDetector::TYPE_9_16, FastFeatureDetector::TYPE_7_12, FastFeatureDetector::TYPE_5_8

	// 计算特征尺度和方向所需的参数
	m_thresholdOffset = 1.0;
	m_rMax = /*3*/100;

	m_nSfMFeatures = /*2048*/8192;
	m_nPrptFeatures = 150;

	// 两视图特征匹配需要的参数设置
	m_fmbOptim = true;						// input:	whether optimize F using Golden Standard algorithm or not
	m_fmThreshRatioTest = 0.65/*0.3*/;		// input:	the ratio threshold for ratio test
	m_fmThreshMinInlierRatio = 0.5;			// input:	the allowed minimum ratio of inliers
	m_fmThreshP2L = 3.;						// input:	the distance threshold between point and epiline, used in RANSAC stage
	m_fmThreshConf = 0.99;					// input:	specifying a desirable level of confidence (probability) that the estimated matrix is correct
	m_fmMaxIter = 64/*10*/;					// input:	the maximum number of iterations
	m_fmxEps = 1.0E-8;						// input:	threshold
	m_fmfEps = 1.0E-6;						// input:	threshold
	m_prptTh = 1;

	// SfM需要的参数设置
	m_threshRpjErrRO = 1.0;					// 相对定向时要用到的重投影残差阈值
	m_optsLM[0] = 1.0E-6;					// levmar 优化方法中要用到的参数 u 的初始尺度因子，默认为 1.0E-3
	m_optsLM[1] = 1.0E-8;					// 当目标函数对各待优化参数的最大导数小于等于该值时优化结束，默认为 1.0E-12
	m_optsLM[2] = 1.0E-12;					// 当待优化参数 2 范数的变化量小于该阈值时优化结束，默认为 1.0E-12
	m_optsLM[3] = 1.0E-12;					// 当误差矢量的 2 范数小于该阈值时优化结束，默认为 1.0E-12
	m_optsLM[4] = 0;						// 当误差矢量的 2 范数的相对变化量小于该阈值时优化结束，默认为 0
	m_threshRpjErrInlier = 1.5;				// EO和BA中用于判断是否内点的重投影残差阈值
	m_threshRatioInlier = 0.15;				// EO中所允许的最小内点集占比
	m_methodRO = 0;							// 指定RO方法类型。0:PIRO; 1:extract [R|t] from essential matrix, just like recoverPose() from opencv
	m_threshMeanAngRO = 5.0;				// RO中要用到的物点平均交会角阈值
	m_nMinInilier = 2;						// 至少得有该个数图像观测到该点才会被输出
	m_nMaxIter = 128;						// 最大迭代次数
	m_bRefineImgPts = true;					// 是否优化像点坐标，默认是要进行优化
	m_nFlagPerImgPt = 2;					// 一条特征轨迹中每个像点预设多少个标志位，一般至少 1 个标志位用于指明该像点当前是否被判定为内点
	m_wndSizeImgptRefine = 5;				// 进行像点匹配优化时采用的窗口大小，一般为奇数哈
	m_xEpsMPGC = 1.0E-8;					// input: threshold
	m_fEpsMPGC = 1.0E-6;					// input: threshold
	m_idxRefImg = -100;						// SfM、SBA过程中的参考坐标系图像
}

// The one and only CDeepVoidApp object

CDeepVoidApp theApp;


// CDeepVoidApp initialization

BOOL CDeepVoidApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
// 	CMultiDocTemplate* pDocTemplate;
// 	pDocTemplate = new CMultiDocTemplate(IDR_DeepVoidTYPE,
// 		RUNTIME_CLASS(CDeepVoidDoc),
// 		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
// 		RUNTIME_CLASS(CDeepVoidView));
// 	if (!pDocTemplate)
// 		return FALSE;
// 	AddDocTemplate(pDocTemplate);

	m_pTemplateImgDoc = new CMultiDocTemplate(IDR_MAINFRAME,
		RUNTIME_CLASS(CImageDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CImageView));
	if (!m_pTemplateImgDoc)
		return FALSE;
	AddDocTemplate(m_pTemplateImgDoc);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	m_pMainFrame = pMainFrame;	// zhaokunz, 20140302, direct mainframe pointer, no need to convert pointer type
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing; // zhaokunz, 20140226, do not initial any doc template, only show IDR_MAINFRAME
	ParseCommandLine(cmdInfo);



	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	
	// zhaokunz, 20140226, add GDI+
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	hCursorArrow = LoadStandardCursor(IDC_ARROW);
	hCursorCross = LoadStandardCursor(IDC_CROSS);
	hCursorGrab = LoadStandardCursor(IDC_HAND);

	return TRUE;
}

int CDeepVoidApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	// zhaokunz, 20140226, shutdown GDI+
	GdiplusShutdown( m_gdiplusToken );

	return CWinAppEx::ExitInstance();
}

// CDeepVoidApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnFullrun();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	/*ON_COMMAND(ID_FULLRUN, &CAboutDlg::OnFullrun)*/
END_MESSAGE_MAP()

// App command to run the dialog
void CDeepVoidApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CDeepVoidApp customization load/save methods

void CDeepVoidApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CDeepVoidApp::LoadCustomState()
{
}

void CDeepVoidApp::SaveCustomState()
{
}

// CDeepVoidApp message handlers


CDocument * CDeepVoidApp::CreateDocument(CMultiDocTemplate * pTemplate, CFrameWnd * pWnd)
{
	CDocument * pDocument = NULL;

	if(pTemplate != NULL)
	{
		pDocument = pTemplate->CreateNewDocument();

		CFrameWndEx *pFrame = (CFrameWndEx *)pTemplate->CreateNewFrame(pDocument, pWnd);
		pTemplate->InitialUpdateFrame(pFrame, pDocument);
	}

	return	pDocument;
}

void CDeepVoidApp::OnFileOpenimages()
{
	// TODO: Add your command handler code here

	// 20151210
//	AfxSetResourceHandle(GetModuleHandle(NULL));

	// zhaokunz, 20140222
	CString filt = _T("Image(BMP;IMG;JPG;PCX;GIF;TIF;TGA;PGM;PBM;PPM;PNG;DIB)|*.BMP;*.IMG;*.JPG;*.PCX;*.GIF;*.TIF;*.TGA;*.PGM;*.PBM;*.PPM;*.PNG;*.DIB|(*.BMP)|*.BMP|(*.IMG)|*.IMG|(*.JPG)|*.JPG|(*.PCX)|*.PCX|(*.GIF)|*.GIF|(*.TIF)|*.TIF|(*.TGA)|*.TGA|(*.PGM)|*.PGM|(*.PBM)|*.PBM|(*.PPM)|*.PPM|(*.PNG)|*.PNG|(*.DIB)|*.DIB||");
	CFileDialog  dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, filt, NULL);
	dlgFile.m_ofn.Flags |= OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT; // enable selecting multiple files at the same time

	int nMaxFile = 50; // maximum amount of files to be selected at the same time;
	TCHAR * pBuffer = new TCHAR[MAX_PATH * nMaxFile];
	dlgFile.m_ofn.lpstrFile = pBuffer;
	dlgFile.m_ofn.nMaxFile = MAX_PATH * nMaxFile;
	dlgFile.m_ofn.lpstrFile[0] = '\0';

	if (dlgFile.DoModal() == IDOK)
	{
		// delete all thumbnail images first
		((CMainFrame *)m_pMainWnd)->m_wndImgThumbnailPane.m_wndImgListCtrl.InitImgListCtrl();

		CString strFilePath = _T("");
		POSITION pos = dlgFile.GetStartPosition();
		while (pos)
		{
			strFilePath = dlgFile.GetNextPathName(pos);

			if (m_pathDirOut == "")
			{
				m_pathDirOut = GetFolderPath(strFilePath) + m_nameDirOut + "\\";
			}

			((CMainFrame *)m_pMainWnd)->m_wndImgThumbnailPane.m_wndImgListCtrl.AddOneImage(strFilePath);
		}

		m_bNoneImages = FALSE; // 通知有图了，更新按键状态。
	}	

	delete [] pBuffer;



	// 20201116，直接调用SfM //////////////////////////////////////////////
//	OnSfm();

// 	int n_imgs = m_vCams.size();
// 
// 	m_vCams.clear();
// 
// 	int i;
// 	for (i = 0; i < n_imgs; i++)
// 	{
// 		cam_data cam;
// 		m_vCams.push_back(cam);
// 	}
// 
// 	AfxBeginThread(SfM, this, THREAD_PRIORITY_NORMAL);
	//////////////////////////////////////////////////////////////////////
}


void CDeepVoidApp::OnFileAddimages()
{
	// TODO: Add your command handler code here

	// 20151210
//	AfxSetResourceHandle(GetModuleHandle(NULL));

	// zhaokunz, 20140224
	CString filt = _T("Image(BMP;IMG;JPG;PCX;GIF;TIF;TGA;PGM;PBM;PPM;PNG;DIB)|*.BMP;*.IMG;*.JPG;*.PCX;*.GIF;*.TIF;*.TGA;*.PGM;*.PBM;*.PPM;*.PNG;*.DIB|(*.BMP)|*.BMP|(*.IMG)|*.IMG|(*.JPG)|*.JPG|(*.PCX)|*.PCX|(*.GIF)|*.GIF|(*.TIF)|*.TIF|(*.TGA)|*.TGA|(*.PGM)|*.PGM|(*.PBM)|*.PBM|(*.PPM)|*.PPM|(*.PNG)|*.PNG|(*.DIB)|*.DIB||");
	CFileDialog  dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, filt, NULL);
	dlgFile.m_ofn.Flags |= OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT; // enable selecting multiple files at the same time

	int nMaxFile = 50; // maximum amount of files to be selected at the same time;
	TCHAR * pBuffer = new TCHAR[MAX_PATH * nMaxFile];
	dlgFile.m_ofn.lpstrFile = pBuffer;
	dlgFile.m_ofn.nMaxFile = MAX_PATH * nMaxFile;
	dlgFile.m_ofn.lpstrFile[0] = '\0';

	if (dlgFile.DoModal() == IDOK)
	{
		CString strFilePath = _T("");
		POSITION pos = dlgFile.GetStartPosition();
		while (pos)
		{
			strFilePath = dlgFile.GetNextPathName(pos);

			if (m_pathDirOut == "")
			{
				m_pathDirOut = GetFolderPath(strFilePath) + m_nameDirOut + "\\";
			}

			((CMainFrame *)m_pMainWnd)->m_wndImgThumbnailPane.m_wndImgListCtrl.AddOneImage(strFilePath);
		}

		m_bNoneImages = FALSE; // 通知有图了，更新按键状态。
	}	

	delete [] pBuffer;
}


//void CDeepVoidApp::OnFun()
//{
//	// TODO: Add your command handler code here
//	((CMainFrame *)m_pMainWnd)->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(_T("Hello world!"));
//}


//void CAboutDlg::OnFullrun()
//{
//	// TODO: Add your command handler code here
//	
//}

UINT FullRun(LPVOID param)
{
	CDeepVoidApp * pApp = (CDeepVoidApp * )param;

	return TRUE;
}

void CDeepVoidApp::OnFullrun()
{
	// TODO: Add your command handler code here
	AfxBeginThread(FullRun, this, THREAD_PRIORITY_NORMAL);
}

UINT SfM(LPVOID param)
{
	CDeepVoidApp * pApp = (CDeepVoidApp * )param;

	int i,j,k,ii,jj;

	int nSubFeats = 150; // Changchang Wu 文章中该值是 100，考虑到 sift 特征点有不少是重的（存在不同的特征主方向），因此这里放宽到 150
	int nMaxFeats = /*4096*//*2048*/8192; // 参考 Changchang Wu 的文章

	CString strInfo;

	int nCam = pApp->m_vCams.size();

	// zhaokunz, 20140404, store every image into memory
	vector<Mat> vImages;

	vector<CString> vImgPaths;

	pApp->m_vImgPaths.clear(); // 20200630

	int imgWidth, imgHeight;

//	typedef std::pair<int, cv::KeyPoint> pair_idx_keypt;

	// first, extract all the features in each image
	for (i=0;i<nCam;i++)
	{
		// read corresponding image data into memory first
		char * pDir = (char *)pApp->m_pMainFrame->m_wndImgThumbnailPane.m_wndImgListCtrl.GetItemData(i);

		CString strDir;
		strDir.Format(_T("%s"), pDir);
		strDir.Trim();

		Mat img = imread(strDir.GetBuffer(), CV_LOAD_IMAGE_UNCHANGED);

		vImgPaths.push_back(strDir);

		pApp->m_vImgPaths.push_back(strDir);

		vImages.push_back(img); // store every image

		imgWidth = img.cols;
		imgHeight = img.rows;
			
		cam_data & cam = pApp->m_vCams[i];
//		cv::Mat & imgDraw = pApp->m_imgsProcessed[i];
//		const CImageDoc * pDoc = pApp->m_vPImgCocs[i];

		// reference: http://stackoverflow.com/questions/27533203/how-do-i-use-sift-in-opencv-3-0-with-c
		// create a feature class
		cv::Ptr<Feature2D> f2d = cv::xfeatures2d::SIFT::create(0, 3, 0.01);
//		cv::Ptr<Feature2D> f2d = cv::xfeatures2d::SURF::create();
//		cv::Ptr<Feature2D> f2d = cv::ORB::create();

		// detect features and compute descriptors
//		Mat mask;
//		f2d->detectAndCompute(img, mask, cam.m_feats.key_points, cam.m_feats.descriptors);

		vector<KeyPoint> keypts_sift, keypts_fast;

		// 先提取 sift 特征点
		f2d->detect(img, keypts_sift);

		// 按特征 size 从大到小对 sift 特征点进行排序
		sort(keypts_sift.begin(), keypts_sift.end(), [](const KeyPoint & a, const KeyPoint & b) {return a.size > b.size; });

		// 再提取 fast 角点特征
		// 20200706，先把图像转换成灰度图再提取FAST特征，因为opencv fast算子只能适用于灰度图（sift是彩色和灰度皆可），而lightroom处理完的图片导出时会被自动转为3通道图，难怪fast提取的特征总有偏移
		cv::Mat im_gray;
		if (img.channels() < 3)
		{
			im_gray = img.clone();
		}
		else
		{
			cv::cvtColor(img, im_gray, CV_RGB2GRAY);
		}
 
		cv::FAST(im_gray, keypts_fast, 10, true, cv::FastFeatureDetector::TYPE_9_16);

		// 按照 response 从大到小对 fast 特征点进行排序
		sort(keypts_fast.begin(), keypts_fast.end(), [](const KeyPoint & a, const KeyPoint & b) {return a.response > b.response; });

		// 把与已有 sift 特征点重叠的 fast 特征点给删除掉
		//for (auto iter = keypts_sift.begin(); iter != keypts_sift.end(); ++iter)
		//{
		//	auto iter_found = find_if(keypts_fast.begin(), keypts_fast.end(),
		//		[iter](const KeyPoint & a) {return sqrt((a.pt.x - iter->pt.x)*(a.pt.x - iter->pt.x) + (a.pt.y - iter->pt.y)*(a.pt.y - iter->pt.y)) < 1.0; });

		//	if (iter_found != keypts_fast.end())
		//	{
		//		keypts_fast.erase(iter_found);
		//	}
		//}

		// 生成特征描述向量
		cv::Mat descrps_sift, descrps_fast;
		f2d->compute(img, keypts_sift, descrps_sift);
		f2d->compute(img, keypts_fast, descrps_fast);

		// 暂时先合成个大的
		vector<KeyPoint> keypts_all = keypts_sift;
		keypts_all.insert(keypts_all.end(), keypts_fast.begin(), keypts_fast.end());

		cv::Mat descrps_all = descrps_sift.clone();
		descrps_all.push_back(descrps_fast);

		// 然后截取为最终的，并录入
		int nSize = keypts_all.size();
		int nSmaller = nSize < nMaxFeats ? nSize : nMaxFeats;
		cam.m_feats.key_points.insert(cam.m_feats.key_points.end(), keypts_all.begin(), keypts_all.begin() + nSmaller);
		cam.m_feats.descriptors = descrps_all.rowRange(cv::Range(0, nSmaller));

		cam.m_feats.type = Feature_SIFTFAST_SIFT;
		
		// 按类别画到图里去
		//int markerSize = 5;
		//int thickness = 1;
		//cv::LineTypes lineType = cv::LineTypes::LINE_4;
		//cv::Scalar color_fast = cv::Scalar(0, 255, 255);
		//cv::Scalar color_sift = cv::Scalar(0, 255, 0);
		//cv::Scalar color_manual = cv::Scalar(255, 255, 255);

		//for (auto iter = cam.m_feats.key_points.begin(); iter != cam.m_feats.key_points.end(); ++iter)
		//{
		//	if (fabs(iter->angle + 1) < 1.0E-12) // 目前来看只有 fast 特征点的 angle 恒为-1，sift 特征点角度的取值范围为0-360°
		//	{
		//		cv::drawMarker(imgDraw, iter->pt, color_fast, cv::MarkerTypes::MARKER_CROSS, markerSize, thickness, lineType);
		//		continue;
		//	}

		//	cv::drawMarker(imgDraw, iter->pt, color_sift, cv::MarkerTypes::MARKER_CROSS, markerSize, thickness, lineType);
		//}

		//// 把画了特征点的图像显示出来
		//if (pDoc)
		//{
		//	pDoc->m_pImgView->Invalidate(TRUE);
		//}		
		
		// 下面主要是为了将 sift 特征中重复位置但主方向不同的特征点编为统一的全局编号，并把每个特征点处的色彩值插值出来
		KeyPoint kpt_pre;
		kpt_pre.pt.x = -1000;	kpt_pre.pt.y = -1000;
		kpt_pre.size = -1000;
		int idx_imgPt;
		for (j = 0; j < cam.m_feats.key_points.size(); j++)
		{
			cam.m_feats.tracks.push_back(-1);

			KeyPoint kpt_cur = cam.m_feats.key_points[j];
			if (fabs(kpt_cur.pt.x - kpt_pre.pt.x) < 1.0E-12 && fabs(kpt_cur.pt.y - kpt_pre.pt.y) < 1.0E-12
				&& fabs(kpt_cur.size - kpt_pre.size) < 1.0E-12)
			{
				// indicating that current keypoint is identical to the previous keypoint
				cam.m_feats.idx_pt.push_back(idx_imgPt);
			} 
			else
			{
				cam.m_feats.idx_pt.push_back(j);
				idx_imgPt = j;
			}

			kpt_pre.pt.x = kpt_cur.pt.x;
			kpt_pre.pt.y = kpt_cur.pt.y;
			kpt_pre.size = kpt_cur.size;

			// 20150215, zhaokunz, 把该特征点的颜色信息插值出来
			uchar r,g,b;
			Vec3b rgb;
			if (BilinearInterp(img, kpt_cur.pt.x, kpt_cur.pt.y, r, g, b))
			{
				rgb[0]=b;
				rgb[1]=g;
				rgb[2]=r;	
			}
			cam.m_feats.rgbs.push_back(rgb);
		}

		// 20200703 ///////////////////////////////////////////////////////////////////
		// 生成子特征集，用于快速筛查匹配
		const Features & feats = cam.m_feats;
		Features & subFeats = cam.m_subFeats;

		if (nSubFeats < nSize)
		{
			subFeats.key_points.insert(subFeats.key_points.end(), keypts_all.begin(), keypts_all.begin() + nSubFeats);
			subFeats.descriptors = descrps_all.rowRange(cv::Range(0, nSubFeats));
			subFeats.idx_pt.insert(subFeats.idx_pt.end(), feats.idx_pt.begin(), feats.idx_pt.begin() + nSubFeats);
			subFeats.tracks.insert(subFeats.tracks.end(), feats.tracks.begin(), feats.tracks.begin() + nSubFeats);
			subFeats.rgbs.insert(subFeats.rgbs.end(), feats.rgbs.begin(), feats.rgbs.begin() + nSubFeats);
			subFeats.type = Feature_SIFTFAST_SIFT;
		}
		///////////////////////////////////////////////////////////////////////////////

		strInfo.Format("Image %03d extracted %07d features", i, pApp->m_vCams[i].m_feats.key_points.size());
		pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
	}

	for (i=0;i<pApp->m_vCams.size();i++)
	{
// 		pApp->m_vCams[i].fx = 707.244864;
// 		pApp->m_vCams[i].fy = 707.135373;
// 		pApp->m_vCams[i].s  = 0;
// 		pApp->m_vCams[i].cx = 200.696825;
// 		pApp->m_vCams[i].cy = 154.045817;
// 
// 		pApp->m_vCams[i].m_K(0,0) = 707.244864;
// 		pApp->m_vCams[i].m_K(1,1) = 707.135373;
// 		pApp->m_vCams[i].m_K(0,1) = 0;
// 		pApp->m_vCams[i].m_K(0,2) = 200.696825;
// 		pApp->m_vCams[i].m_K(1,2) = 154.045817;
// 		pApp->m_vCams[i].m_K(2,2) = 1;
// 		pApp->m_vCams[i].m_bCalibed = true;

// 		pApp->m_vCams[i].fx = 1414.489728;
// 		pApp->m_vCams[i].fy = 1414.270745;
// 		pApp->m_vCams[i].s  = 0;
// 		pApp->m_vCams[i].cx = 401.3936505;
// 		pApp->m_vCams[i].cy = 308.091633;
// 
// 		pApp->m_vCams[i].m_K(0,0) = 1414.489728;
// 		pApp->m_vCams[i].m_K(1,1) = 1414.270745;
// 		pApp->m_vCams[i].m_K(0,1) = 0;
// 		pApp->m_vCams[i].m_K(0,2) = 401.3936505;
// 		pApp->m_vCams[i].m_K(1,2) = 308.091633;
// 		pApp->m_vCams[i].m_K(2,2) = 1;
// 		pApp->m_vCams[i].m_bCalibed = true;

		// 20151125 沙盘重建
// 		pApp->m_vCams[i].fx = 557.072909185350450;
// 		pApp->m_vCams[i].fy = 557.404946420734060;
// 		pApp->m_vCams[i].s  = 0;
// 		pApp->m_vCams[i].cx = 397.172326465639740;
// 		pApp->m_vCams[i].cy = 272.495109535585640;
// 
// 		pApp->m_vCams[i].m_K(0,0) = 557.072909185350450;
// 		pApp->m_vCams[i].m_K(1,1) = 557.404946420734060;
// 		pApp->m_vCams[i].m_K(0,1) = 0;
// 		pApp->m_vCams[i].m_K(0,2) = 397.172326465639740;
// 		pApp->m_vCams[i].m_K(1,2) = 272.495109535585640;
// 		pApp->m_vCams[i].m_K(2,2) = 1;
// 		pApp->m_vCams[i].m_bCalibed = true;

		// 20151126 周博 UAV 数据 cx 2000.000000, cy 1335.000000, f 4091.504
// 		pApp->m_vCams[i].fx = 4091.504*0.2;
// 		pApp->m_vCams[i].fy = 4091.504*0.2;
// 		pApp->m_vCams[i].s  = 0;
// 		pApp->m_vCams[i].cx = 2000.000000*0.2;
// 		pApp->m_vCams[i].cy = 1335.000000*0.2;
// 
// 		pApp->m_vCams[i].m_K(0,0) = 4091.504*0.2;
// 		pApp->m_vCams[i].m_K(1,1) = 4091.504*0.2;
// 		pApp->m_vCams[i].m_K(0,1) = 0;
// 		pApp->m_vCams[i].m_K(0,2) = 2000.000000*0.2;
// 		pApp->m_vCams[i].m_K(1,2) = 1335.000000*0.2;
// 		pApp->m_vCams[i].m_K(2,2) = 1;
// 		pApp->m_vCams[i].m_bCalibed = true;

		// 20151206 橘子洲毛泽东像
// 		double fx = /*1100*/849.812399662762910;
// 		double fy = /*1100*/849.028798494762330;
// 		pApp->m_vCams[i].fx = fx;
// 		pApp->m_vCams[i].fy = fy;
// 		pApp->m_vCams[i].s  = 0;
// 		pApp->m_vCams[i].cx = 401.558250358447480;
// 		pApp->m_vCams[i].cy = 273.689303692577480;
// 
// 		pApp->m_vCams[i].m_K(0,0) = fx;
// 		pApp->m_vCams[i].m_K(1,1) = fy;
// 		pApp->m_vCams[i].m_K(0,1) = 0;
// 		pApp->m_vCams[i].m_K(0,2) = 401.558250358447480;
// 		pApp->m_vCams[i].m_K(1,2) = 273.689303692577480;
// 		pApp->m_vCams[i].m_K(2,2) = 1;
// 		pApp->m_vCams[i].m_bCalibed = true;

		// 20200519 涿州测量
//  		double f = /*7692.31*/2000.0/*2692.31*//*1700*/;
//  		pApp->m_vCams[i].fx = f;
//  		pApp->m_vCams[i].fy = f;
//  		pApp->m_vCams[i].s  = 0;
//  		pApp->m_vCams[i].cx = 501.5;
//  		pApp->m_vCams[i].cy = 500.5;
//  
//  		pApp->m_vCams[i].m_K(0, 0) = f;
//  		pApp->m_vCams[i].m_K(1, 1) = f;
//  		pApp->m_vCams[i].m_K(0, 1) = 0;
//  		pApp->m_vCams[i].m_K(0, 2) = 501.5;
//  		pApp->m_vCams[i].m_K(1, 2) = 500.5;
//  		pApp->m_vCams[i].m_K(2, 2) = 1;
//  		pApp->m_vCams[i].m_bCalibed = true;

		// 20200626 四时田园“马雕塑”
		// iphone se2 rear camera parameters
		double f = /*521.2902*/657.1836;
		pApp->m_vCams[i].fx = f;
		pApp->m_vCams[i].fy = f;
		pApp->m_vCams[i].s = 0;
		pApp->m_vCams[i].cx = /*299.5*/399.5;
		pApp->m_vCams[i].cy = /*399.5*/299.5;

		pApp->m_vCams[i].m_K(0, 0) = f;
		pApp->m_vCams[i].m_K(1, 1) = f;
		pApp->m_vCams[i].m_K(0, 1) = 0;
		pApp->m_vCams[i].m_K(0, 2) = /*299.5*/399.5;
		pApp->m_vCams[i].m_K(1, 2) = /*399.5*/299.5;
		pApp->m_vCams[i].m_K(2, 2) = 1;
		pApp->m_vCams[i].m_bCalibed = true;

		// 20150212
// 		pApp->m_vCams[i].fx = 1816.431947;
// 		pApp->m_vCams[i].fy = 1816.448252;
// 		pApp->m_vCams[i].s  = 0;
// 		pApp->m_vCams[i].cx = 959.502819;
// 		pApp->m_vCams[i].cy = 539.500942;

		// temple sparse paras 1520.400000 0.000000 302.320000 0.000000 1525.900000 246.870000
// 		pApp->m_vCams[i].fx = 1520.400000;
// 		pApp->m_vCams[i].fy = 1525.900000;
// 		pApp->m_vCams[i].s  = 0;
// 		pApp->m_vCams[i].cx = 302.320000;
// 		pApp->m_vCams[i].cy = 246.870000;
// 
// 		pApp->m_vCams[i].m_K(0,0) = 1520.400000;
// 		pApp->m_vCams[i].m_K(1,1) = 1525.900000;
// 		pApp->m_vCams[i].m_K(0,1) = 0;
// 		pApp->m_vCams[i].m_K(0,2) = 302.320000;
// 		pApp->m_vCams[i].m_K(1,2) = 246.870000;
// 		pApp->m_vCams[i].m_K(2,2) = 1;
// 		pApp->m_vCams[i].m_bCalibed = true;

		// with sock on
// 		pApp->m_vCams[i].fx = 1271.94;
// 		pApp->m_vCams[i].fy = 1271.07;
// 		pApp->m_vCams[i].s  = 0;
// 		pApp->m_vCams[i].cx = 636.91;
// 		pApp->m_vCams[i].cy = 365.85;

		// without sock on
// 		pApp->m_vCams[i].fx = 1287.83;
// 		pApp->m_vCams[i].fy = 1292.60;
// 		pApp->m_vCams[i].s  = 0;
// 		pApp->m_vCams[i].cx = 628.30;
// 		pApp->m_vCams[i].cy = 369.45;
// 
// 		pApp->m_vCams[i].m_K(0,0) = 1287.83;
// 		pApp->m_vCams[i].m_K(1,1) = 1292.60;
// 		pApp->m_vCams[i].m_K(0,1) = 0;
// 		pApp->m_vCams[i].m_K(0,2) = 628.30;
// 		pApp->m_vCams[i].m_K(1,2) = 369.45;
// 		pApp->m_vCams[i].m_K(2,2) = 1;
// 		pApp->m_vCams[i].m_bCalibed = true;

		// liu's data, pony
// 		pApp->m_vCams[i].fx = 2126.9165;
// 		pApp->m_vCams[i].fy = 2126.9079;
// 		pApp->m_vCams[i].s  = 0;
// 		pApp->m_vCams[i].cx = 1295.4475;
// 		pApp->m_vCams[i].cy = 727.5050;

// 		pApp->m_vCams[i].fx = 1414.489728;
// 		pApp->m_vCams[i].fy = 1414.270745;
// 		pApp->m_vCams[i].s  = 0;
// 		pApp->m_vCams[i].cx = 401.3936505;
// 		pApp->m_vCams[i].cy = 308.091633;

// 		pApp->m_vCams[i].fx = 3954.7557192818421754;
// 		pApp->m_vCams[i].fy = 3948.0083629740670403;
// 		pApp->m_vCams[i].s  = 0;
// 		pApp->m_vCams[i].cx = 1619.9232700857789951;
// 		pApp->m_vCams[i].cy = 1151.4558912976590364;

		memset(pApp->m_vCams[i].k, 0, 5*sizeof(double));
		pApp->m_vCams[i].dist_type = 1;
	}

	// remove distortions and set the distortion coefficients to be all zeros
	for (i=0;i<nCam;i++)
	{
		RemoveDistortion_DCBrown(pApp->m_vCams[i]);
		memset(pApp->m_vCams[i].k, 0, 5*sizeof(double));
	}

	double thresh_match = 5;
	double thresh_d2epiline = 3;
	double thresh_matchConf = 0.99;
	int thresh_n_matches = 20;

	double thresh_stdev_YZ = 0.2;
	double thresh_reproj_ro = 1.0;
	double thresh_pyerr = 0.01;
	double thresh_ang = 15;

	int nSptImgs_desired = 2; // each image will be associated with at most this number of support images

	if (nSptImgs_desired>(pApp->m_vCams.size()-1))
	{
		nSptImgs_desired = pApp->m_vCams.size()-1; // at most n-1 support images
	}

	double opts[5];
//	opts[0] = 1.0E-3;	// levmar 优化方法中要用到的参数 u 的初始尺度因子，默认为 1.0E-3
	opts[0] = 1.0E-6;	// levmar 优化方法中要用到的参数 u 的初始尺度因子，默认为 1.0E-3
	opts[1] = 1.0E-8;	// 当目标函数对各待优化参数的最大导数小于等于该值时优化结束，默认为 1.0E-12
	opts[2] = 1.0E-8;	// 当待优化参数 2 范数的变化量小于该阈值时优化结束，默认为 1.0E-12
	opts[3] = 1.0E-12;	// 当误差矢量的 2 范数小于该阈值时优化结束，默认为 1.0E-12
	opts[4] = 0;		// 当误差矢量的 2 范数的相对变化量小于该阈值时优化结束，默认为 0

	vector<vector<DMatch>> allMatches;
	vector<DMatch> matches;

//	SfM_ZZK::PairWiseMatches map_pairwise_matches; // 20150920, <(i,j), <DMatch>>
	SfM_ZZK::PairWise_F_matches_pWrdPts map_pairwise_F_matches_pWrdPts; // 20200622, {<i, j>, <<F, matches>, wrdpts>}

	FILE * fileMatches = fopen("E:\\all\\matches number.txt", "w");

	for (i=0;i<nCam;i++)
	{
		for (j=0;j<nCam;j++)
		{
			if (j==i)
			{
				vector<DMatch> nullMatch;
				allMatches.push_back(nullMatch);
				fprintf(fileMatches, "%d	", nullMatch.size());
				continue;
			}
			if (j<i)
			{
				vector<DMatch> match_ji = allMatches[j*nCam+i];
				for (k=0;k<match_ji.size();k++)
				{
					int tmp = match_ji[k].queryIdx;
					match_ji[k].queryIdx = match_ji[k].trainIdx;
					match_ji[k].trainIdx = tmp;
				}
				allMatches.push_back(match_ji);
				fprintf(fileMatches, "%d	", match_ji.size());
				continue;
			}

			// match all image pairs
// 			CalibFundamentalMat_RANSAC_Features_Original(pApp->m_vCams[i].m_feats, pApp->m_vCams[j].m_feats, matches,
// 				thresh_match, thresh_d2epiline, thresh_matchConf);

// 			CalibFundamentalMat_RANSAC_Features(pApp->m_vCams[i].m_feats, pApp->m_vCams[j].m_feats, matches,
// 				thresh_match, thresh_d2epiline, thresh_matchConf);

// 			CalibFundamentalMat_RANSAC_Features_idxImgPt(pApp->m_vCams[i].m_feats, pApp->m_vCams[j].m_feats, matches,
// 				thresh_match, thresh_d2epiline, thresh_matchConf);

			// 20150113, zhaokunz, new matching function
			Matx33d mF; 
			vector<Point3d> pWrdPts;
//			Get_F_Matches(pApp->m_vCams[i].m_feats,pApp->m_vCams[j].m_feats,mF,matches,thresh_d2epiline,thresh_matchConf); bool bSuc = true;
//			Get_F_Matches(vImages[i],vImages[j],pApp->m_vCams[i].m_feats,pApp->m_vCams[j].m_feats,mF,matches,thresh_d2epiline,thresh_matchConf); bool bSuc = true;
//			bool bSuc = Get_F_Matches_knn(pApp->m_vCams[i].m_feats, pApp->m_vCams[j].m_feats, mF, matches, true, 0.65, 0.5, thresh_d2epiline, thresh_matchConf, 64);

			bool bSuc = false;
//			bool bPreempSuc = false;

			// 20200629, “先发制人”特征匹配
			if (nSubFeats < pApp->m_vCams[i].m_feats.key_points.size())
			{
				if (PreemptiveFeatureMatching(pApp->m_vCams[i].m_subFeats, pApp->m_vCams[j].m_subFeats, 0.65, 1))
				{
					// 20200621, 同步输出射影重建物点坐标
					bSuc = Get_F_Matches_pWrdPts_knn(pApp->m_vCams[i].m_feats, pApp->m_vCams[j].m_feats, mF, matches, pWrdPts, true, 0.65, 0.5, thresh_d2epiline, thresh_matchConf, 64);
				}
			}
			else
			{
				bSuc = Get_F_Matches_pWrdPts_knn(pApp->m_vCams[i].m_feats, pApp->m_vCams[j].m_feats, mF, matches, pWrdPts, true, 0.65, 0.5, thresh_d2epiline, thresh_matchConf, 64);
			}			
			
			// 20151017，draw matches from each stage from inside the function
// 			bool bSuc = Get_F_Matches_knn(vImages[i],vImages[j],pApp->m_vCams[i].m_feats,pApp->m_vCams[j].m_feats,mF,matches,
// 				true,0.65,0.5,thresh_d2epiline,thresh_matchConf,64);

			if (!bSuc)		
			{
				vector<DMatch> nullMatch;
				allMatches.push_back(nullMatch);
				fprintf(fileMatches, "%d	", nullMatch.size());

//				strInfo.Format("matching between image %03d and %03d finished: %04d matches are found", i, j, nullMatch.size());
//				pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
			} 
			else
			{
				allMatches.push_back(matches);
				fprintf(fileMatches, "%d	", matches.size());

				strInfo.Format("matching between image %03d and %03d finished: %04d matches are found", i, j, matches.size());
				pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

//				map_pairwise_matches.insert(make_pair(make_pair(i,j), matches));
				map_pairwise_F_matches_pWrdPts.insert(make_pair(make_pair(i, j), make_pair(make_pair(mF, matches), pWrdPts)));

				Mat disp_matches;
// 				drawMatches(vImages[i], pApp->m_vCams[i].m_feats.key_points, vImages[j], pApp->m_vCams[j].m_feats.key_points, matches,
// 					disp_matches, Scalar(0,255,0), Scalar(0,0,255)/*,std::vector<char>(), 1*/);
				drawMatchesRefImg(vImages[i], pApp->m_vCams[i].m_feats.key_points, pApp->m_vCams[j].m_feats.key_points, matches,
					disp_matches);
				strInfo.Format("E:\\all\\matches between image %03d and %03d.bmp", i, j);
				imwrite(strInfo.GetBuffer(), disp_matches);
			}
		}
		fprintf(fileMatches, "\n");
	}
	fclose(fileMatches);


	// feature tracking
//	SfM_ZZK::MultiTracks map_tracks_init;
	SfM_ZZK::MultiTracksWithFlags map_tracks_init; // 20220202
//	SfM_ZZK::FindAllTracks_Olsson(map_pairwise_matches, map_tracks_init);
	SfM_ZZK::FindAllTracks_Olsson(map_pairwise_F_matches_pWrdPts, map_tracks_init); // 20200622

	// 确保特征轨迹从0开始依次计数
	// 并建立特征轨迹中包含的特征点至该特征轨迹的映射
//	SfM_ZZK::MultiTracks map_tracks;
	SfM_ZZK::MultiTracksWithFlags map_tracks; // 20220202
	int idx_count = 0;
	for (auto iter_track=map_tracks_init.begin();iter_track!=map_tracks_init.end();++iter_track)
	{
		map_tracks.insert(make_pair(idx_count,iter_track->second));

		// 建立该特征轨迹中包含的特征点至该特征轨迹的映射，通过 trackID 来索引
		for (auto iter_Ii=iter_track->second.begin();iter_Ii!=iter_track->second.end();++iter_Ii)
		{
			const int & I = iter_Ii->first; // image I
//			const int & i = iter_Ii->second; // feature i
			const int & i = iter_Ii->second.first; // feature i
			pApp->m_vCams[I].m_feats.tracks[i] = idx_count;
		}

		++idx_count;
	}

	// 统计特征轨迹直方图
	std::map<int,int> hist_track;
	SfM_ZZK::BuildTrackLengthHistogram(map_tracks, hist_track);
	int n_tracklength_more_than_1 = 0;
	for (auto iter_n=hist_track.begin();iter_n!=hist_track.end();++iter_n)
	{
		if (iter_n->first<2)
		{
			continue;
		}
		n_tracklength_more_than_1+=iter_n->second;
	}

	vector<vector<Point2i>> allTracks;
//	FindAllMatchingTracks(pApp->m_vCams, allMatches, allTracks);

// 	FILE * file = fopen("D:\\good tracks.txt", "w");
// 
// 	for (i=0;i<allTracks.size();i++)
// 	{
// 		vector<Point2i> track = allTracks[i];
// 
// 		for (j=0;j<track.size();j++)
// 		{
// 			fprintf(file, "(%02d,%04d)	", track[j].x, track[j].y);
// 		}
// 		fprintf(file, "\n");
// 	}
// 
// 	fclose(file);

	strInfo.Format("number of good tracks: %07d", n_tracklength_more_than_1);
	pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

	// relative orientation //////////////////////////////////////////////////////////////////////////
	CMatrix mRT; vector<CloudPoint> clouds;

	// 20151105 point cloud based on std::map
	SfM_ZZK::PointCloud map_pointcloud;

	vector<int> status(nCam);
	for (i=0;i<nCam;i++)
	{
		status[i] = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	vector<SfM_ZZK::pair_ij_k> pairs;
//	SfM_ZZK::RankImagePairs_TrackLengthSum(map_pairwise_matches, map_tracks, pairs);
	SfM_ZZK::RankImagePairs_TrackLengthSum(map_pairwise_F_matches_pWrdPts, map_tracks, pairs);

// 	vector<Point2i> pairs;
// 	FindPairObservingMostCommonTracks(pApp->m_vCams,allTracks,pairs);

	CString strFile;
	int idx_refimg;
	bool bSuc_RO = false;

//	for (k=0;k<pairs.size();k++)
	for (auto iter_pair=pairs.begin();iter_pair!=pairs.end();++iter_pair)
	{
// 		i = pairs[k].x;
// 		j = pairs[k].y;

		i = iter_pair->first.first;
		j = iter_pair->first.second;

		cam_data & cam_i = pApp->m_vCams[i];
		cam_data & cam_j = pApp->m_vCams[j];

		// 找到两者之间的特征匹配
//		auto iter_found = map_pairwise_matches.find(iter_pair->first);
		auto iter_found = map_pairwise_F_matches_pWrdPts.find(iter_pair->first);

		// 尝试做相对定向
// 		bool bSuc_RO = RelativeOrientation_Features_PIRO_givenMatches(cam_i, cam_j, i, j, iter_found->second,
// 			mRT, clouds, thresh_reproj_ro);

		Matx33d mR;
		Matx31d mt;
		SfM_ZZK::PointCloud map_pointcloud_tmp;

//		bSuc_RO = RelativeOrientation_Features_PIRO_givenMatches(cam_i, cam_j, i, j, /*iter_found->second*/iter_found->second.first.second,
//			mR, mt, map_pointcloud_tmp, thresh_reproj_ro/*, 0.1*/); // 20200519 交会角阈值放宽
		bSuc_RO = RelativeOrientation(cam_i, cam_j, iter_found->second.first.first, iter_found->second.first.second,
			mR, mt, map_pointcloud_tmp, 0, thresh_reproj_ro/*, 0.5*/); // 20200623 inspect

		if (bSuc_RO)
		{
			cam_i.m_R = Matx33d();
			cam_i.m_t = Matx31d();
			cam_i.m_R(0,0)=cam_i.m_R(1,1)=cam_i.m_R(2,2)=1;

			cam_j.m_R = mR;
			cam_j.m_t = mt;

			map_pointcloud = map_pointcloud_tmp; // 正式录用所有生成的物点

//			strInfo.Format("Relative orientation between image %03d and %03d finished, the rank is: %d, number of cloud points are %d", i, j, k, clouds.size());
			strInfo.Format("Relative orientation between image %03d and %03d finished, number of cloud points are %d", i, j, map_pointcloud.size());
			pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

			status[i] = 1;
			status[j] = 1;

			cam_i.m_bOriented = true;
			cam_j.m_bOriented = true;

			idx_refimg = i;

			// 将所有重建出来的像点暂时赋上都为内点
			for (auto iter_wrdpt = map_pointcloud.begin();iter_wrdpt!=map_pointcloud.end();++iter_wrdpt)
			{
				const int & trackID = iter_wrdpt->first;

				auto iter_found_track = map_tracks.find(trackID);

				auto iter_found_i = iter_found_track->second.find(i);
				auto iter_found_j = iter_found_track->second.find(j);

				iter_found_i->second.second[0] = 1; // 为内点
				iter_found_j->second.second[0] = 1; // 为内点
			}

			strFile.Format("point cloud after RO of images %03d and %03d.txt", i, j);

			break;
		}
	}

	// 20200607，如果没有任何像对相对定向成功那就直接退出
	if (!bSuc_RO)
	{
		theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(_T("Relative Orientation failed for all image pair candidates!"));
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////

	CString strOut;
	strOut.Format("E:\\all\\");

//	OutputPointCloud(strOut+strFile,pApp->m_vCams,clouds);

	vector<CloudPoint> cloud_old;

	SfM_ZZK::OutputPointCloud(strOut+strFile,map_pointcloud,pApp->m_vCams,map_tracks,cloud_old,2);

	double d_mean = MeanMinDistance_3D(cloud_old);
	strInfo.Format("average distance between cloud points is: %lf", d_mean);
	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

	vector<SfM_ZZK::pair_ij> imgRank;
//	RankImages_NumObjPts(pApp->m_vCams, clouds, status, imgRank);
	RankImages_NumObjPts(pApp->m_vCams, map_pointcloud, imgRank);

	// 20151103，全自动的增量式 SfM 循环 ////////////////////////////////////
	double thresh_inlier_rpjerr = 1.5;
	double thresh_inlier_ratio = 0.15;

	int n_pointcloud_size_old = map_pointcloud.size();

	bool bAllFail = false;
	while (imgRank.size()>0 && !bAllFail) // 还有图没有完成定向且并不是所有剩余图像都定向失败了就继续
	{
		bAllFail = true;
		vector<SfM_ZZK::pair_ij> imgRank_tmp = imgRank;

		for (auto iter_img=imgRank_tmp.begin();iter_img!=imgRank_tmp.end();++iter_img)
		{
			const int & I = iter_img->first; // image I

			cam_data & cam_I = pApp->m_vCams[I];

			// 1. 尝试 RANSAC 后方交会
			Matx33d mR;
			Matx31d mt;
			bool bEOSuc;
			try
			{
				bEOSuc = EO_PnP_RANSAC(cam_I, map_pointcloud, mR, mt, thresh_inlier_rpjerr, thresh_inlier_ratio);
			}
			catch (cv::Exception & e)
			{
				CString str;
				str = e.msg.c_str();
				str+="	error happened in RANSAC EO";
				AfxMessageBox(str);
			}
			
			if (!bEOSuc) // 如果后方交会失败就考虑排在后面的图像
			{
				continue;
			}

			// 到这里就说明后方交会成功了
			bAllFail = false;
			
			// 正式录入外参初值
			cam_I.m_R = mR;
			cam_I.m_t = mt;

			status[I] = 1;
			cam_I.m_bOriented = true;

			// 2. 稀疏光束法平差。只利用现有的物点，还没有利用新加入的图像前方交会新的物点
			theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(_T("SBA starts"));
			double info[10], rltUctt, uctt_f;
			try
			{
//				int nnn = optim_sba_levmar_XYZ_ext_rotvec(map_pointcloud, pApp->m_vCams, map_tracks, idx_refimg, thresh_inlier_rpjerr, 64, opts, info);
				// 20200607 同时优化所有图像共有的等效焦距 f
				int nnn = DeepVoid::optim_sba_levmar_f_XYZ_ext_rotvec_IRLS_Huber(map_pointcloud, pApp->m_vCams, map_tracks, rltUctt, uctt_f, idx_refimg, thresh_inlier_rpjerr, 64, opts, info);
			}
			catch (cv::Exception & e)
			{
				CString str;
				str = e.msg.c_str();
				str+="	error happened in SBA";
				AfxMessageBox(str);
			}
			strInfo.Format("SBA ends, point cloud size: %d, initial err: %lf, final err: %lf, iter: %04.0f, code: %01.0f, rltUctt(1): %f, uctt_f(1): %f",
				map_pointcloud.size(), info[0], info[1], info[3], info[4], rltUctt, uctt_f);
			theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

			strFile.Format("point cloud after successful EO of image %03d and bundle adjustment.txt", I);
			try
			{
				SfM_ZZK::OutputPointCloud(strOut+strFile,map_pointcloud,pApp->m_vCams,map_tracks,cloud_old,2/*3*//*1*/);	
			}
			catch (cv::Exception & e)
			{
				CString str;
				str = e.msg.c_str();
				str+="	error happened in OutputPointCloud";
				AfxMessageBox(str);
			}

			// 更新阈值
//			thresh_inlier_rpjerr = 3*info[1];

			// 3. 前方交会
			try
			{
				SfM_ZZK::Triangulation_AddOneImg(map_pointcloud,pApp->m_vCams,map_tracks,I,thresh_inlier_rpjerr);
//				SfM_ZZK::Triangulation_AllImgs(map_pointcloud,pApp->m_vCams,map_tracks,thresh_inlier_rpjerr);
			}
			catch (cv::Exception & e)
			{
				CString str;
				str = e.msg.c_str();
				str+="	error happened in Triangulation";
				AfxMessageBox(str);
			}
			

			// 4. 如果点云拓展了，就对剩下的图像重新进行排序
			int n_pointcloud_size_new = map_pointcloud.size();
			if (n_pointcloud_size_new != n_pointcloud_size_old)
			{
				try
				{
					RankImages_NumObjPts(pApp->m_vCams, map_pointcloud, imgRank);
				}
				catch (cv::Exception & e)
				{
					CString str;
					str = e.msg.c_str();
					str+="	error happened in RankImages";
					AfxMessageBox(str);
				}

				n_pointcloud_size_old = n_pointcloud_size_new;
				break;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////

	// 最后总的来一次稀疏光束法平差
	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(_T("Final SBA starts"));
	double info[10], rltUctt, uctt_f;
//	int nnn = optim_sba_levmar_XYZ_ext_rotvec(map_pointcloud, pApp->m_vCams, map_tracks, idx_refimg, thresh_inlier_rpjerr, /*1024*/64,opts, info);
	// 20200607 同时优化所有图像共有的等效焦距 f
	int nnn = DeepVoid::optim_sba_levmar_f_XYZ_ext_rotvec_IRLS_Huber(map_pointcloud, pApp->m_vCams, map_tracks, rltUctt, uctt_f, idx_refimg, thresh_inlier_rpjerr, 64, opts, info);
	strInfo.Format("Final SBA ends, point cloud size: %d, initial err: %lf, final err: %lf, iter: %04.0f, code: %01.0f, rltUctt(1): %f, uctt_f(1): %f",
		map_pointcloud.size(), info[0], info[1], info[3], info[4], rltUctt, uctt_f);
	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

	std::map<int,int> hist_cloudpoint_inlier;
	double length_average = SfM_ZZK::BuildCloudPointInlierHistogram(map_pointcloud,map_tracks,hist_cloudpoint_inlier);

	FILE * file = fopen("E:\\all\\hist.txt", "w");
	for (auto iter_hist=hist_cloudpoint_inlier.begin();iter_hist!=hist_cloudpoint_inlier.end();++iter_hist)
	{
		fprintf(file,"%d	%d\n", iter_hist->first, iter_hist->second);
	}
	fprintf(file,"%lf", length_average);
	fclose(file);

	// 输出点云
	strFile.Format("Final point cloud.txt");
	SfM_ZZK::OutputPointCloud(strOut+strFile,map_pointcloud,pApp->m_vCams,map_tracks,cloud_old,2/*3*//*1*/);

	// 输出像机定向
	// save all cameras' parameters
	for (i=0;i<nCam;i++)
	{
		CString strtmp;
		strFile.Format("cam%02d.txt", i);
		SaveCameraData(strOut+strFile, pApp->m_vCams[i]);
	}

	// 20200630
	pApp->m_mapPointCloud = map_pointcloud;
	pApp->m_mapTracks = map_tracks;

	// 20220127，用来表征完成了稀疏三维重建，使能三维显示
	pApp->m_b3DReady_sparse = TRUE;

	return TRUE;

	vector<vector<int>> vIdxSupports;
//	ScoreMatchingImages(map_pointcloud, pApp->m_vCams, map_pairwise_matches, map_tracks, vIdxSupports, nSptImgs_desired, 10);
	ScoreMatchingImages(map_pointcloud, pApp->m_vCams, map_pairwise_F_matches_pWrdPts, map_tracks, vIdxSupports, nSptImgs_desired, 10);

	// 20150206, zhaokunz, try dense reconstruction //////////////////////////
	vector<Mat> vDepths, vHxs, vHys, vScores, vVisis;

	int wndSize = 5;
//	int wndSize = 7;
	double thresh_norm = 80;
	int nPatchMatchIter = 2;
	double factor = 0.25;
	int nRandSamp = 5;
	double img_sigma = 0.5;

	vector<Mat> vSilhouettes;

// 	MVDE_package_final(strOut,vAllCams,vImgPaths,vSilhouettes,vIdxSupports,vClouds,vDepths,vHxs,vHys,vScores,vVisis,
// 		wndSize,thresh_norm,nPatchMatchIter,factor,nRandSamp,90,0.95,1.5,0.01,img_sigma,0.1/*0.01*/,50/*300*/,5,1.0E-8,1.0E-6,false);

	MVDE_package_150206(strOut,pApp->m_vCams,vImgPaths,vSilhouettes,vIdxSupports,cloud_old,vDepths,vHxs,vHys,vScores,vVisis,
		wndSize,thresh_norm,nPatchMatchIter,factor,nRandSamp,90,0.95,1.5,0.01,img_sigma,2,1,0.01,0.02,5);
	//////////////////////////////////////////////////////////////////////////

// 	int n_inliers_changed = SfM_ZZK::Triangulation_AllImgs(map_pointcloud,pApp->m_vCams,map_tracks,thresh_inlier_rpjerr/*3*info[1]*/);
// 
// 	strInfo.Format("n_inliers_changed: %d", n_inliers_changed);
// 	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

	return TRUE;

	vector<Point2i> candidates;
	GetReconstructedTrackNum(pApp->m_vCams, clouds, status, candidates);

	int count_sucEO = 0;

	bool bquit = false;

	while (!bquit)
	{
		for (i=0;i<candidates.size();i++)
		{
			Point2i cand = candidates[i];

			bool bSuc_EO = EO_PnP_RANSAC(pApp->m_vCams, idx_refimg, cand.x, clouds, allTracks, 1.5, 0.15);
			strInfo.Format("Exterior orientation of image %03d finished, code: %d, number of cloud points are %d", cand.x, bSuc_EO, clouds.size());
			theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

			if (!bSuc_EO) // failed
			{
				continue;
			}

			++count_sucEO;

			status[cand.x] = 1;

			theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(_T("SBA starts"));
			double info[10];

// 			int nnn = optim_sba_levmar_XYZ_ext_rotvec(clouds, pApp->m_vCams, idx_refimg, 1024, opts, info);
// 			double rrr = sqrt(info[1]/nnn);
// 			strInfo.Format("SBA ends, err: %lf, iter: %04.0f, code: %01.0f", rrr, info[5], info[6]);

			int nnn = optim_sba_levmar_XYZ_ext_rotvec_own(clouds, pApp->m_vCams, idx_refimg, 1024, opts, info);
			double rrr = info[1];
			strInfo.Format("SBA ends, err: %lf, iter: %04.0f, code: %01.0f", rrr, info[3], info[4]);

			theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

			d_mean = MeanMinDistance_3D(clouds);
			strInfo.Format("average distance between cloud points is: %lf", d_mean);
			theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

			strFile.Format("point cloud after %03d successful EO of image %03d.txt", count_sucEO, cand.x);
			OutputPointCloud(strOut+strFile,pApp->m_vCams,clouds);

			break;
		}

		vector<Point2i> candidates_new;
		GetReconstructedTrackNum(pApp->m_vCams, clouds, status, candidates_new);

		if (candidates_new==candidates)
		{
			bquit = true;
			continue;
		}

		candidates = candidates_new;
	}

	vector<vector<int>> clusters;
	Clustering_3D_byDistance(clouds, clusters, d_mean*10);

	vector<CloudPoint> clouds_no_outliers;

	int n_outliers = 0;

	/*FILE * */file = fopen("D:\\result_SfM_outliers_inliers.txt", "w");
	for (i=0;i<clusters.size();i++)
	{
		vector<int> cluster = clusters[i];

		if (cluster.size() < 3)
		{
			for (j=0;j<cluster.size();j++)
			{
				int R = 255;
				int G = 255;
				int B = 255;

				fprintf(file, "%lf;%lf;%lf;%d;%d;%d\n", clouds[cluster[j]].m_pt.x, clouds[cluster[j]].m_pt.y, clouds[cluster[j]].m_pt.z, R, G, B);

				n_outliers++;
			}
		} 
		else
		{
			for (j=0;j<cluster.size();j++)
			{
				clouds_no_outliers.push_back(clouds[cluster[j]]);

				int R = 128;
				int G = 128;
				int B = 0;

				fprintf(file, "%lf;%lf;%lf;%d;%d;%d\n", clouds[cluster[j]].m_pt.x, clouds[cluster[j]].m_pt.y, clouds[cluster[j]].m_pt.z, R, G, B);
			}
		}
	}
	fclose(file);

	strInfo.Format("found %04d outliers by clustering, ratio: %lf", n_outliers, n_outliers/(double)clouds.size());
	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

	// the final sba after a few obvious outliers being eliminated
	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(_T("Final SBA starts"));
//	double info[10];

// 	int nnn = optim_sba_levmar_XYZ_ext_rotvec(clouds_no_outliers, pApp->m_vCams, idx_refimg, 1024, opts, info);
// 	double rrr = sqrt(info[1]/nnn);
// 	strInfo.Format("Final SBA ends, err: %lf, iter: %04.0f, code: %01.0f", rrr, info[5], info[6]);

	nnn = optim_sba_levmar_XYZ_ext_rotvec_own(clouds_no_outliers, pApp->m_vCams, idx_refimg, 1024, opts, info);
	double rrr = info[1];
	strInfo.Format("Final SBA ends, err: %lf, iter: %04.0f, code: %01.0f", rrr, info[3], info[4]);

	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);



	// 20150204, zhaokunz, find several matching images for each image for dense matching
// 	vector<int> matchingImgs;
// 	RankMatchingImages(pApp->m_vCams, clouds_no_outliers, 0, matchingImgs);

	// 20150206, associate each input image with several support images
//	vector<vector<int>> vIdxSupports;

	for (auto iter=pApp->m_vCams.begin();iter!=pApp->m_vCams.end();++iter)
	{
		int idx = iter-pApp->m_vCams.begin();

		vector<Point2d> scores;
		ScoreMatchingImages(pApp->m_vCams, clouds_no_outliers, allMatches, idx, scores, 15);

		vector<int> idxSpts;

		if (scores.size()<1)
		{
			// null vector means that the image hasn't been oriented yet
			vIdxSupports.push_back(idxSpts);
			continue;
		}

		for (i=0;i<nSptImgs_desired;i++)
		{
			if (scores[i].y<=0.0)
			{
				break;
			}
			idxSpts.push_back((int)scores[i].x);
		}

		vIdxSupports.push_back(idxSpts);
	}
	//////////////////////////////////////////////////////////////////////////

	FillCloudPoints_RBG_RpjErr(pApp->m_vCams, clouds_no_outliers);

	file = fopen("D:\\result_SfM.txt", "w");
	for (i=0;i<clouds_no_outliers.size();i++)
	{
		double sumR = 0;
		double sumG = 0;
		double sumB = 0;
		for (j=0;j<clouds_no_outliers[i].m_vImgInfos.size();j++)
		{
			sumR += (double)clouds_no_outliers[i].m_vImgInfos[j].m_rgb.val[2];
			sumG += (double)clouds_no_outliers[i].m_vImgInfos[j].m_rgb.val[1];
			sumB += (double)clouds_no_outliers[i].m_vImgInfos[j].m_rgb.val[0];
		}
		int R = (int)sumR/clouds_no_outliers[i].m_vImgInfos.size();
		int G = (int)sumG/clouds_no_outliers[i].m_vImgInfos.size();
		int B = (int)sumB/clouds_no_outliers[i].m_vImgInfos.size();

// 		int R = 0;
// 		int G = 255;
// 		int B = 0;

		fprintf(file, "%lf;%lf;%lf;%d;%d;%d\n", clouds_no_outliers[i].m_pt.x, clouds_no_outliers[i].m_pt.y, clouds_no_outliers[i].m_pt.z, R, G, B);
	}
	fclose(file);

	return TRUE;

	// save all cameras' parameters
	for (i=0;i<nCam;i++)
	{
		CString strtmp;
		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\cam%02d.txt", i);
//		SaveCameraData(strtmp, pApp->m_vCams[i]);
	}

	// 20150206, zhaokunz, try dense reconstruction //////////////////////////
// 	vector<Mat> vDepths, vHxs, vHys, vScores, vVisis;
// 
// 	int wndSize = 5;
// //	int wndSize = 7;
// 	double thresh_norm = 80;
// 	int nPatchMatchIter = 2;
// 	double factor = 0.25;
// 	int nRandSamp = 5;
// 	double img_sigma = 0.5;
// 
// 	vector<Mat> vSilhouettes;

// 	MVDE_package_final(strOut,vAllCams,vImgPaths,vSilhouettes,vIdxSupports,vClouds,vDepths,vHxs,vHys,vScores,vVisis,
// 		wndSize,thresh_norm,nPatchMatchIter,factor,nRandSamp,90,0.95,1.5,0.01,img_sigma,0.1/*0.01*/,50/*300*/,5,1.0E-8,1.0E-6,false);

	MVDE_package_150206(strOut,pApp->m_vCams,vImgPaths,vSilhouettes,vIdxSupports,clouds_no_outliers,vDepths,vHxs,vHys,vScores,vVisis,
		wndSize,thresh_norm,nPatchMatchIter,factor,nRandSamp,90,0.95,1.5,0.01,img_sigma,2,1,0.01,0.02,5);
	//////////////////////////////////////////////////////////////////////////

	return TRUE;

	// 20140401 PatchMatch ////////////////////////////////////////////////////////////////////////////////////////////////
// 	vector<cam_data> vCamShort(pApp->m_vCams.begin(), pApp->m_vCams.begin()+2);
// 	pApp->m_vCams = vCamShort;

	// convert to gray images
	vector<Mat> vGrayImages;
	for (i=0;i<vImages.size();i++)
	{
		Mat im_gray;
		cvtColor(vImages[i], im_gray, CV_RGB2GRAY);
		vGrayImages.push_back(im_gray);
	}

	idx_refimg = 0;
//	int wndSize = 5;
	double thresh_ncc = 0.5;
//	double thresh_norm = 80;
	int mincam = 1;
	double minncc = thresh_ncc*mincam/(pApp->m_vCams.size()-1);
	double maxncc_minncc_1 = 1/(1-minncc);
//	double factor = 0.3;
//	int nRandSamp = 4;

	Matx33d mR0,mK0;
	Matx31d mt0;

	for (ii=0;ii<3;ii++)
	{
		for (jj=0;jj<3;jj++)
		{
			mR0(ii,jj) = pApp->m_vCams[idx_refimg].R[ii*3+jj];
		}
	}
	mK0(0,0) = pApp->m_vCams[idx_refimg].fx; mK0(0,1) = pApp->m_vCams[idx_refimg].s;  mK0(0,2) = pApp->m_vCams[idx_refimg].cx;
	mK0(1,1) = pApp->m_vCams[idx_refimg].fy; mK0(1,2) = pApp->m_vCams[idx_refimg].cy; mK0(2,2) = 1;
	mt0(0) = pApp->m_vCams[idx_refimg].t[0]; mt0(1) = pApp->m_vCams[idx_refimg].t[1]; mt0(2) = pApp->m_vCams[idx_refimg].t[2];

	vector<Mat> vRndFields(3);
	InitRndField(mK0, mR0, mt0, clouds_no_outliers, thresh_norm, imgWidth, imgHeight, vRndFields[0], vRndFields[1], vRndFields[2]);

	Mat mScores(imgHeight, imgWidth, CV_32FC1);
	//EvaluateOneParamSet(pApp->m_vCams, idx_refimg, vRndFields[0], vRndFields[1], vRndFields[2], mScores, wndSize, thresh_ncc);
	//EvaluateOneParamSet(pApp->m_vCams, vImages, idx_refimg, vRndFields[0], vRndFields[1], vRndFields[2], mScores, mincam, wndSize, thresh_ncc);

	double max_depth,min_depth,max_incre_x,min_incre_x,max_incre_y,min_incre_y,max_score,min_score;

	minMaxIdx(vRndFields[0], &min_depth, &max_depth);
	minMaxIdx(vRndFields[1], &min_incre_x, &max_incre_x);
	minMaxIdx(vRndFields[2], &min_incre_y, &max_incre_y);
	//minMaxIdx(mScores, &min_score, &max_score);

	vector<Mat> vRndFields_normed(3);

	Mat mScores_normed_color(mScores.rows, mScores.cols, CV_8UC3);

// 	vRndFields[0].convertTo(vRndFields_normed[0], CV_32FC1, 1/(max_depth-min_depth), -min_depth/(max_depth-min_depth));
// 	vRndFields[1].convertTo(vRndFields_normed[1], CV_32FC1, 1/(max_incre_x-min_incre_x), -min_incre_x/(max_incre_x-min_incre_x));
// 	vRndFields[2].convertTo(vRndFields_normed[2], CV_32FC1, 1/(max_incre_y-min_incre_y), -min_incre_y/(max_incre_y-min_incre_y));

	vRndFields[0].convertTo(vRndFields_normed[0], CV_8UC1, 255/(max_depth-min_depth), -255*min_depth/(max_depth-min_depth));
	vRndFields[1].convertTo(vRndFields_normed[1], CV_8UC1, 255/(max_incre_x-min_incre_x), -255*min_incre_x/(max_incre_x-min_incre_x));
	vRndFields[2].convertTo(vRndFields_normed[2], CV_8UC1, 255/(max_incre_y-min_incre_y), -255*min_incre_y/(max_incre_y-min_incre_y));

// 	for (i=0;i<mScores.rows;i++)
// 	{
// 		for (j=0;j<mScores.cols;j++)
// 		{
// 			if (mScores.at<float>(i,j)<0)
// 			{
// 				mScores_normed_color.at<Vec3b>(i,j).val[0] = 0;
// 				mScores_normed_color.at<Vec3b>(i,j).val[1] = 0;
// 				mScores_normed_color.at<Vec3b>(i,j).val[2] = 255;
// 			}
// 			else
// 			{
// 				mScores_normed_color.at<Vec3b>(i,j).val[0] = 0;
// 				mScores_normed_color.at<Vec3b>(i,j).val[1] = FTOI(255*(mScores.at<float>(i,j)-minncc)*maxncc_minncc_1);
// 				mScores_normed_color.at<Vec3b>(i,j).val[2] = 0;
// 			}
// 		}
// 	}
	
	imwrite("C:\\Users\\DeepVoid\\Desktop\\initial depth map.bmp", vRndFields_normed[0]);
	imwrite("C:\\Users\\DeepVoid\\Desktop\\initial hx map.bmp", vRndFields_normed[1]);
	imwrite("C:\\Users\\DeepVoid\\Desktop\\initial hy map.bmp", vRndFields_normed[2]);
	//imwrite("C:\\Users\\DeepVoid\\Desktop\\initial score map.bmp", mScores_normed_color);

	for (i=0;i<2;i++)
	{
		try
		{
			//Scan_Update_rndField(pApp->m_vCams, clouds_no_outliers, thresh_norm, idx_refimg, vRndFields[0], vRndFields[1], vRndFields[2], mScores, i, wndSize, thresh_ncc);
			//Scan_Update_rndField(pApp->m_vCams, vImages, clouds_no_outliers, thresh_norm, idx_refimg, vRndFields[0], vRndFields[1], vRndFields[2], mScores, i, wndSize, thresh_ncc);
			Scan_Update_rndField_original(pApp->m_vCams, /*vGrayImages*/vImages, clouds_no_outliers, thresh_norm, idx_refimg, vRndFields[0], vRndFields[1], vRndFields[2], mScores, i, mincam, factor, nRandSamp, wndSize, thresh_ncc);
			minMaxIdx(vRndFields[0], &min_depth, &max_depth);
			minMaxIdx(vRndFields[1], &min_incre_x, &max_incre_x);
			minMaxIdx(vRndFields[2], &min_incre_y, &max_incre_y);
			minMaxIdx(mScores, &min_score, &max_score);

			vRndFields[0].convertTo(vRndFields_normed[0], CV_8UC1, 255/(max_depth-min_depth), -255*min_depth/(max_depth-min_depth));
			vRndFields[1].convertTo(vRndFields_normed[1], CV_8UC1, 255/(max_incre_x-min_incre_x), -255*min_incre_x/(max_incre_x-min_incre_x));
			vRndFields[2].convertTo(vRndFields_normed[2], CV_8UC1, 255/(max_incre_y-min_incre_y), -255*min_incre_y/(max_incre_y-min_incre_y));
			//mScores.convertTo(mScores_normed, CV_8UC1, 255/(max_score-min_score), -255*min_score/(max_score-min_score));

			for (ii=0;ii<mScores.rows;ii++)
			{
				for (jj=0;jj<mScores.cols;jj++)
				{
					if (mScores.at<float>(ii,jj)<0)
					{
						mScores_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
						mScores_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
						mScores_normed_color.at<Vec3b>(ii,jj).val[2] = 255;
					}
					else
					{
						mScores_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
						mScores_normed_color.at<Vec3b>(ii,jj).val[1] = FTOI(255*(mScores.at<float>(ii,jj)-minncc)*maxncc_minncc_1);
						mScores_normed_color.at<Vec3b>(ii,jj).val[2] = 0;
					}
				}
			}

			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\depth map iteration %02d.bmp", i);
			imwrite(strInfo.GetBuffer(), vRndFields_normed[0]);
			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\hx map iteration %02d.bmp", i);
			imwrite(strInfo.GetBuffer(), vRndFields_normed[1]);
			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\hy map iteration %02d.bmp", i);
			imwrite(strInfo.GetBuffer(), vRndFields_normed[2]);
			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\score map iteration %02d.bmp", i);
			imwrite(strInfo.GetBuffer(), mScores_normed_color);

			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\cloud points iteration %02d.txt", i);
			WriteDepthMap(strInfo,pApp->m_vCams[idx_refimg],vImages[idx_refimg],vRndFields[0],mScores);
		}
		catch (cv::Exception & e)
		{
			CString str;
			str = e.msg.c_str();
			AfxMessageBox(str);
		}
	}

	FILE * file_depth = fopen("C:\\Users\\DeepVoid\\Desktop\\final depth map.txt", "w");
	FILE * file_hx = fopen("C:\\Users\\DeepVoid\\Desktop\\final hx map.txt", "w");
	FILE * file_hy = fopen("C:\\Users\\DeepVoid\\Desktop\\final hy map.txt", "w");
	FILE * file_score = fopen("C:\\Users\\DeepVoid\\Desktop\\final score map.txt", "w");
	for (i=0;i<vRndFields[0].rows;i++)
	{
		for (j=0;j<vRndFields[0].cols;j++)
		{
			double score = mScores.at<float>(i,j);
			if (score<0)
			{
				double tmp = -1;
				fprintf(file_depth, "%lf	", tmp);
				fprintf(file_hx, "%lf	", tmp);
				fprintf(file_hy, "%lf	", tmp);
				fprintf(file_score, "%lf	", tmp);
			} 
			else
			{
				fprintf(file_depth, "%lf	", vRndFields[0].at<float>(i,j));
				fprintf(file_hx, "%lf	", vRndFields[1].at<float>(i,j));
				fprintf(file_hy, "%lf	", vRndFields[2].at<float>(i,j));
				fprintf(file_score, "%lf	", mScores.at<float>(i,j));
			}
		}
		fprintf(file_depth, "\n");
		fprintf(file_hx, "\n");
		fprintf(file_hy, "\n");
		fprintf(file_score, "\n");
	}
	fclose(file_depth);
	fclose(file_hx);
	fclose(file_hy);
	fclose(file_score);
	
// 	int x_check = 190;
// 	int y_check = 130;
// 
// 	double depth_init = vRndFields[0].at<float>(y_check, x_check);
// 	double hx_init = vRndFields[1].at<float>(y_check, x_check);
// 	double hy_init = vRndFields[2].at<float>(y_check, x_check);
// 
// 	file = fopen("C:\\Users\\DeepVoid\\Desktop\\initial.txt", "w");
// 	fprintf(file, "%lf	%lf	%lf", depth_init, hx_init, hy_init);
// 	fclose(file);
// 
// 	vector<double> vScores;
// 	CheckOnePixel_givenOneParamSet(pApp->m_vCams,vImages,idx_refimg,x_check,y_check,depth_init,hx_init,hy_init,vScores,wndSize);
// 
// 	vector<int> vIdxValidCam;
// 	for (i=0;i<vScores.size();i++)
// 	{
// 		if (vScores[i]<0.5)
// 		{
// 			continue;
// 		}
// 		vIdxValidCam.push_back(i);
// 	}
// 	
// 
// 
// 	double depth_optim, hx_optim, hy_optim;
// 	optim_gn_drhxhyck(pApp->m_vCams, vImages, vIdxValidCam, idx_refimg, x_check, y_check, wndSize, wndSize, depth_init, hx_init, hy_init, depth_optim, hx_optim, hy_optim);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// 20140331, just for test
	// choose one of the reconstructed features in image 0
// 	vector<Point2i> vImgPts_in0;
// 	vector<Point3d> vWrdPts_in0;
// 	for (i=0;i<clouds_no_outliers.size();i++)
// 	{
// 		CloudPoint pt3d = clouds_no_outliers[i];
// 
// 		for (j=0;j<pt3d.m_vImgInfos.size();j++)
// 		{
// 			CloudPoint_ImgInfo imginfo = pt3d.m_vImgInfos[j];
// 			if (imginfo.m_idxImg == 0)
// 			{
// 				Point2i pt2d;
// 				pt2d.x = FTOI(pApp->m_vCams[0].m_feats.key_points[imginfo.m_idxImgPt].pt.x);
// 				pt2d.y = FTOI(pApp->m_vCams[0].m_feats.key_points[imginfo.m_idxImgPt].pt.y);
// 				vImgPts_in0.push_back(pt2d);
// 				vWrdPts_in0.push_back(pt3d.m_pt);
// 				break;
// 			}
// 		}
// 	}
// 
// 	Matx33d mR0,mK0;
// 	Matx31d mt0;
// 
// 	for (ii=0;ii<3;ii++)
// 	{
// 		for (jj=0;jj<3;jj++)
// 		{
// 			mR0(ii,jj) = pApp->m_vCams[0].R[ii*3+jj];
// 		}
// 	}
// 	mK0(0,0) = pApp->m_vCams[0].fx;	mK0(0,1) = pApp->m_vCams[0].s;  mK0(0,2) = pApp->m_vCams[0].cx;
// 	mK0(1,1) = pApp->m_vCams[0].fy; mK0(1,2) = pApp->m_vCams[0].cy; mK0(2,2) = 1;
// 	mt0(0) = pApp->m_vCams[0].t[0]; mt0(1) = pApp->m_vCams[0].t[1]; mt0(2) = pApp->m_vCams[0].t[2];
// 
// 	int idx_choosen = 0;
// 	Matx31d XYZ_known;
// 	XYZ_known(0) = vWrdPts_in0[idx_choosen].x;
// 	XYZ_known(1) = vWrdPts_in0[idx_choosen].y;
// 	XYZ_known(2) = vWrdPts_in0[idx_choosen].z;
// 	XYZ_known = mR0*XYZ_known+mt0;
// 
// 	int n_windowsize = 5;
// 	double hx = 0;
// 	double hy = 0;
// 
// 	Mat imgsmall0(n_windowsize, n_windowsize, CV_8UC3), imgsmalli(n_windowsize, n_windowsize, CV_8UC3);
// 
// 	vector<Point3d> vWrdPts;
// 
// 	char * pDir0 = (char *)pApp->m_pMainFrame->m_wndImgThumbnailPane.m_wndImgListCtrl.GetItemData(0);
// 
// 	CString strDir0;
// 	strDir0.Format(_T("%s"), pDir0);
// 	strDir0.Trim();
// 
// 	Mat img0 = imread(strDir0.GetBuffer(), CV_LOAD_IMAGE_UNCHANGED);
// 
// 	for (i=-(n_windowsize-1)/2;i<=(n_windowsize-1)/2;i++)
// 	{
// 		for (j=-(n_windowsize-1)/2;j<=(n_windowsize-1)/2;j++)
// 		{
// 			Matx31d XYZ = GetXYZ_givenDepth(mK0, mR0, mt0, vImgPts_in0[idx_choosen].x+j, vImgPts_in0[idx_choosen].y+i, XYZ_known(2)+hx*j+hy*i);
// 			Point3d pt3d;
// 			pt3d.x = XYZ(0);
// 			pt3d.y = XYZ(1);
// 			pt3d.z = XYZ(2);
// 			vWrdPts.push_back(pt3d);
// 
// 			imgsmall0.at<Vec3b>(i+(n_windowsize-1)/2, j+(n_windowsize-1)/2).val[0] = img0.at<Vec3b>(vImgPts_in0[idx_choosen].y+i,vImgPts_in0[idx_choosen].x+j).val[0];
// 			imgsmall0.at<Vec3b>(i+(n_windowsize-1)/2, j+(n_windowsize-1)/2).val[1] = img0.at<Vec3b>(vImgPts_in0[idx_choosen].y+i,vImgPts_in0[idx_choosen].x+j).val[1];
// 			imgsmall0.at<Vec3b>(i+(n_windowsize-1)/2, j+(n_windowsize-1)/2).val[2] = img0.at<Vec3b>(vImgPts_in0[idx_choosen].y+i,vImgPts_in0[idx_choosen].x+j).val[2];
// 		}
// 	}
// 
// 	CString pathPatch;
// 	pathPatch.Format("D:\\patches\\imgPatch00_%03d.bmp", idx_choosen);
// 	imwrite(pathPatch.GetBuffer(), imgsmall0);
// 
// 	for (i=1;i<nCam;i++)
// 	{
// 		// read corresponding image data into memory first
// 		char * pDir = (char *)pApp->m_pMainFrame->m_wndImgThumbnailPane.m_wndImgListCtrl.GetItemData(i);
// 
// 		CString strDir;
// 		strDir.Format(_T("%s"), pDir);
// 		strDir.Trim();
// 
// 		Mat img = imread(strDir.GetBuffer(), CV_LOAD_IMAGE_UNCHANGED);
// 
// 		cam_data cami = pApp->m_vCams[i];
// 
// 		for (ii=0;ii<3;ii++)
// 		{
// 			for (jj=0;jj<3;jj++)
// 			{
// 				mR0(ii,jj) = cami.R[ii*3+jj];
// 			}
// 		}
// 		mK0(0,0) = cami.fx;	mK0(0,1) = cami.s;  mK0(0,2) = cami.cx;
// 		mK0(1,1) = cami.fy; mK0(1,2) = cami.cy; mK0(2,2) = 1;
// 		mt0(0) = cami.t[0]; mt0(1) = cami.t[1]; mt0(2) = cami.t[2];
// 
// 		for (ii=0;ii<n_windowsize;ii++)
// 		{
// 			for (jj=0;jj<n_windowsize;jj++)
// 			{
// 				uchar r,g,b;
// 				BilinearInterp(mK0,mR0,mt0,img,vWrdPts[ii*n_windowsize+jj].x, vWrdPts[ii*n_windowsize+jj].y, vWrdPts[ii*n_windowsize+jj].z, r,g,b);
// 				imgsmalli.at<Vec3b>(ii, jj).val[0] = b;
// 				imgsmalli.at<Vec3b>(ii, jj).val[1] = g;
// 				imgsmalli.at<Vec3b>(ii, jj).val[2] = r;
// 			}
// 		}
// 
// 		/// Create the result matrix
// 		int result_cols =  imgsmalli.cols - imgsmall0.cols + 1;
// 		int result_rows = imgsmalli.rows - imgsmall0.rows + 1;
// 
// 		Mat result( result_cols, result_rows, CV_32FC1 );
// 
// 		/// Do the Matching and Normalize
// 		matchTemplate( imgsmall0, imgsmalli, result, CV_TM_SQDIFF_NORMED/*CV_TM_CCORR_NORMED*/ );
// 		
// 		pathPatch.Format("D:\\patches\\imgPatch%02d_%03d_%lf.bmp", i, idx_choosen, result.at<float>(0));
// 		imwrite(pathPatch.GetBuffer(), imgsmalli);
// 	}
// 	//////////////////////////////////////////////////////////////////////////
// 
	// find good image pairs for dense matching
	vector<Point2i> imgPairs;
	FindAllGoodImagePairs(pApp->m_vCams, imgPairs);

	int n_pairs = imgPairs.size();

	Matx33d mK1, mK2, mR1, mR2, mR12, mR1_rct, mR2_rct;
	Matx31d mt1, mt2, mt12, mc12;
	Matx34d mP1_new, mP2_new;
	Matx44d mQ;

	Matx<double, 5, 1> mDist1,mDist2;

	Mat rmap[2][2];

	for (i=0;i<n_pairs;i++)
	{
		Point2i pair = imgPairs[i];

		// idxCam1 is the index of the left image in a horizontal stereo or the top image in a vertical stereo
		// idxCam2 is the index of the right image in a horizontal stereo or the bottom image in a vertical stereo
		int idxCam1 = pair.x;
		int idxCam2 = pair.y;

		cam_data cam1 = pApp->m_vCams[idxCam1];
		cam_data cam2 = pApp->m_vCams[idxCam2];

		// read images into memory
		char * pDir = (char *)pApp->m_pMainFrame->m_wndImgThumbnailPane.m_wndImgListCtrl.GetItemData(idxCam1);
		CString strDir;
		strDir.Format(_T("%s"), pDir);
		strDir.Trim();
		Mat img1 = imread(strDir.GetBuffer(), CV_LOAD_IMAGE_UNCHANGED); // convert to grayscale image

		pDir = (char *)pApp->m_pMainFrame->m_wndImgThumbnailPane.m_wndImgListCtrl.GetItemData(idxCam2);
		strDir.Format(_T("%s"), pDir);
		strDir.Trim();
		Mat img2 = imread(strDir.GetBuffer(), CV_LOAD_IMAGE_UNCHANGED); // convert to grayscale image

		cv::Size size;
		size.width  = img1.cols;
		size.height = img1.rows;

		mK1(0,0) = cam1.fx;	mK1(1,1) = cam1.fy;
		mK1(0,2) = cam1.cx;	mK1(1,2) = cam1.cy;
		mK1(0,1) = cam1.s;	mK1(2,2) = 1;

		mK2(0,0) = cam2.fx;	mK2(1,1) = cam2.fy;
		mK2(0,2) = cam2.cx;	mK2(1,2) = cam2.cy;
		mK2(0,1) = cam2.s;	mK2(2,2) = 1;

		for (ii=0;ii<3;ii++)
		{
			for (jj=0;jj<3;jj++)
			{
				mR1(ii,jj) = cam1.R[ii*3+jj];
				mR2(ii,jj) = cam2.R[ii*3+jj];
			}
		}

		mt1(0)=cam1.t[0];	mt1(1)=cam1.t[1];	mt1(2)=cam1.t[2];	
		mt2(0)=cam2.t[0];	mt2(1)=cam2.t[1];	mt2(2)=cam2.t[2];	

		mR12 = mR2*mR1.t();
		mt12 = -mR12*mt1+mt2;
		mc12 = -mR12.t() * mt12; // C = -R't, the coordinates of image 2's optical center in image 1

		int flag_stereoRectify = CV_CALIB_ZERO_DISPARITY;
		double alpha_stereoRectify = 0;

		stereoRectify(mK1, mDist1, mK2, mDist2, size, mR12, mt12, mR1_rct, mR2_rct, mP1_new, mP2_new, mQ,
			flag_stereoRectify, alpha_stereoRectify);
	
		// determine it's a horizontal stereo or a vertical stereo
		bool isVerticalStereo = fabs(mP2_new(1, 3)) > fabs(mP2_new(0, 3));

		if (!isVerticalStereo)
		{
			// it's a horizontal stereo
			// then determine which image is on the left
			if (mc12(0) < 0)
			{
				// change the sequence to make sure the 1st image is the left one, and the 2nd image is the right one
				Swap_int(idxCam1, idxCam2);
				cam1 = pApp->m_vCams[idxCam1];
				cam2 = pApp->m_vCams[idxCam2];

				for (ii=0;ii<3;ii++)
				{
					for (jj=0;jj<3;jj++)
					{
						Swap_double(mK1(ii,jj),mK2(ii,jj));
						Swap_double(mR1(ii,jj),mR2(ii,jj));
					}
				}
				for (ii=0;ii<5;ii++)
				{
					Swap_double(mDist1(ii),mDist2(ii));
				}
				Swap_double(mt1(0),mt2(0)); Swap_double(mt1(1),mt2(1)); Swap_double(mt1(2),mt2(2));

				// reverse the images too
				cv::swap(img1, img2);

				mR12 = mR2*mR1.t();
				mt12 = -mR12*mt1+mt2;

				stereoRectify(mK1, mDist1, mK2, mDist2, size, mR12, mt12, mR1_rct, mR2_rct, mP1_new, mP2_new, mQ,
					flag_stereoRectify, alpha_stereoRectify);
			} 
		} 
		else
		{
			// it's a vertical stereo
			// then determine which image is on the top
			if (mc12(1) < 0)
			{
				// change the sequence to make sure the 1st image is the top one, and the 2nd image is the bottom one
				Swap_int(idxCam1, idxCam2);
				cam1 = pApp->m_vCams[idxCam1];
				cam2 = pApp->m_vCams[idxCam2];

				for (ii=0;ii<3;ii++)
				{
					for (jj=0;jj<3;jj++)
					{
						Swap_double(mK1(ii,jj),mK2(ii,jj));
						Swap_double(mR1(ii,jj),mR2(ii,jj));
					}
				}
				for (ii=0;ii<5;ii++)
				{
					Swap_double(mDist1(ii),mDist2(ii));
				}
				Swap_double(mt1(0),mt2(0)); Swap_double(mt1(1),mt2(1)); Swap_double(mt1(2),mt2(2));

				// reverse the images too
				cv::swap(img1, img2);

				mR12 = mR2*mR1.t();
				mt12 = -mR12*mt1+mt2;

				stereoRectify(mK1, mDist1, mK2, mDist2, size, mR12, mt12, mR1_rct, mR2_rct, mP1_new, mP2_new, mQ,
					flag_stereoRectify, alpha_stereoRectify);
			} 
		}

		// Computes the undistortion and rectification transformation map
		initUndistortRectifyMap(mK1, mDist1, mR1_rct, mP1_new, size, CV_32FC1, rmap[0][0], rmap[0][1]);
		initUndistortRectifyMap(mK2, mDist2, mR2_rct, mP2_new, size, CV_32FC1, rmap[1][0], rmap[1][1]);

		Mat rimg1,rimg2;
		remap(img1, rimg1, rmap[0][0], rmap[0][1], CV_INTER_LINEAR);
		remap(img2, rimg2, rmap[1][0], rmap[1][1], CV_INTER_LINEAR);

		img1.release();
		img2.release();

		Mat rimg1_gray, rimg2_gray;
		cvtColor(rimg1, rimg1_gray, CV_RGB2GRAY);
		cvtColor(rimg2, rimg2_gray, CV_RGB2GRAY);

// 		imwrite("D:\\rimg1.bmp", rimg1);
// 		imwrite("D:\\rimg2.bmp", rimg2);

		// find all the finally confirmed matches between these two image
		vector<Point2d> imgPtsMatch1, imgPtsMatch2;
		for (j=0;j<clouds_no_outliers.size();j++)
		{
			CloudPoint pt3d = clouds_no_outliers[j];

			bool bfoundCam1 = false;
			bool bfoundCam2 = false;
			int idxMatchImgPt1 = -1;
			int idxMatchImgPt2 = -1;
			for (k=0;k<pt3d.m_vImgInfos.size();k++)
			{
				if (!bfoundCam1 && pt3d.m_vImgInfos[k].m_idxImg == idxCam1)
				{
					idxMatchImgPt1 = pt3d.m_vImgInfos[k].m_idxImgPt;
					bfoundCam1 = true;
				}

				if (!bfoundCam2 && pt3d.m_vImgInfos[k].m_idxImg == idxCam2)
				{
					idxMatchImgPt2 = pt3d.m_vImgInfos[k].m_idxImgPt;
					bfoundCam2 = true;
				}

				if (bfoundCam1 && bfoundCam2)
				{
					Point2d pt2d;
					pt2d.x = cam1.m_feats.key_points[idxMatchImgPt1].pt.x;
					pt2d.y = cam1.m_feats.key_points[idxMatchImgPt1].pt.y;
					imgPtsMatch1.push_back(pt2d);
					pt2d.x = cam2.m_feats.key_points[idxMatchImgPt2].pt.x;
					pt2d.y = cam2.m_feats.key_points[idxMatchImgPt2].pt.y;
					imgPtsMatch2.push_back(pt2d);
					break;
				}
			}
		}

		vector<Point2d> imgPtsMatch1_rect, imgPtsMatch2_rect;

		undistortPoints(imgPtsMatch1, imgPtsMatch1_rect, mK1, mDist1, mR1_rct, mP1_new);
		undistortPoints(imgPtsMatch2, imgPtsMatch2_rect, mK2, mDist2, mR2_rct, mP2_new);
		
		int n_match = imgPtsMatch1.size();

		vector<Point3d> dispFeatures;

		// compute the min disparity and max disparity of feature matches
		bool bFirst = true;
		double minDisX, maxDisX, minDisY, maxDisY;
		for (j=0;j<n_match;j++)
		{
			double dx = imgPtsMatch1_rect[j].x - imgPtsMatch2_rect[j].x;
			double dy = imgPtsMatch1_rect[j].y - imgPtsMatch2_rect[j].y;

			Point3d pt3d;
			pt3d.x = FTOI(imgPtsMatch1_rect[j].x);
			pt3d.y = FTOI(imgPtsMatch1_rect[j].y);
			pt3d.z = dx;
			dispFeatures.push_back(pt3d);

			if (bFirst)
			{
				minDisX = maxDisX = dx;
				minDisY = maxDisY = dy;
				bFirst = false;
				continue;
			}

			if (dx<minDisX)
			{
				minDisX = dx;
			}
			if (dx>maxDisX)
			{
				maxDisX = dx;
			}
			if (dy<minDisY)
			{
				minDisY = dy;
			}
			if (dy>maxDisY)
			{
				maxDisY = dy;
			}
		}

		double minDisparity, maxDisparity;

		if (!isVerticalStereo)
		{
			minDisparity = minDisX;
			maxDisparity = maxDisX;
		} 
		else
		{
			minDisparity = minDisY;
			maxDisparity = maxDisY;
		}

		double disparityEx = 5;

		maxDisparity += disparityEx;

		if (minDisparity<0)
		{
			minDisparity -= disparityEx;
		}
		else
		{
			if (minDisparity-disparityEx<0)
			{
				minDisparity = 0;
			} 
			else
			{
				minDisparity -= disparityEx;
			}
		}

		int minDisp = floor(minDisparity);
		int maxDisp = ceil(maxDisparity);
		int nDisp = maxDisp-minDisp+1;
		nDisp = (nDisp + 15) & ~15; // make sure nDisp can be divisible by 16
		
		// setting SGBM
		/*StereoSGBM sgbm;

		sgbm.preFilterCap = 63;
		sgbm.SADWindowSize = 1;
		// int cn = rimg1.channels();
		int cn = rimg1_gray.channels();
		sgbm.P1 = 8*cn*sgbm.SADWindowSize*sgbm.SADWindowSize;
		sgbm.P2 = 32*cn*sgbm.SADWindowSize*sgbm.SADWindowSize;
		sgbm.minDisparity = minDisp;
		sgbm.numberOfDisparities = nDisp;
		sgbm.uniquenessRatio = 10;
		sgbm.speckleWindowSize = 100;
		sgbm.speckleRange = 32;
		sgbm.disp12MaxDiff = 1;
		sgbm.fullDP = true;

		Mat disp;

		if (!isVerticalStereo)
		{
//			sgbm(rimg1, rimg2, disp);
			sgbm(rimg1_gray, rimg2_gray, disp);
		}
		else
		{

		}
		
		Mat disp32f;
		disp.convertTo(disp32f, CV_32F, 1/16.);

		CString strFile;
		strFile.Format("D:\\result_SfM_dispDiff_pair%02d.txt", i);
		FILE* fp = fopen(strFile, "wt");
		for (j=0;j<dispFeatures.size();j++)
		{
			Point3d pt3d = dispFeatures[j];

			int x = int(pt3d.x);
			int y = int(pt3d.y);

			if (x<0 || y<0 || x>=disp32f.cols || y>=disp32f.rows)
			{
				continue;
			}

			double dispSGBM;

			dispSGBM = disp32f.at<float>(y, x);
			
			fprintf(fp, "%lf	%lf	%lf\n", pt3d.x, pt3d.y, pt3d.z-dispSGBM);
		}
		fclose(fp);
		
		Mat xyz;
		reprojectImageTo3D(disp32f, xyz, mQ, true);

		const double max_z = 1.0E+4;
		strFile.Format("D:\\result_SfM_dense_pair%02d.txt", i);
		fp = fopen(strFile, "wt");
		for(int y = 0; y < xyz.rows; y++)
		{
			for(int x = 0; x < xyz.cols; x++)
			{
				Vec3f point = xyz.at<Vec3f>(y, x);
				if(fabs(point[2] - max_z) < FLT_EPSILON || fabs(point[2]) > max_z) continue;

				Matx31d mX;
				mX(0) = point[0]; mX(1) = point[1]; mX(2) = point[2];
				Matx31d mWrdPt = mR1.t() * (mX - mt1);

				int R,G,B;
				if (rimg1.channels()==1)
				{
					Scalar pix = rimg1.at<uchar>(y, x);
					R = G = B = pix.val[0];
				}
				else
				{
					Vec3b pix = rimg1.at<Vec3b>(y, x);
					B = pix.val[0]; G = pix.val[1]; R = pix.val[2];
				}
				
				fprintf(fp, "%lf;%lf;%lf;%d;%d;%d\n", mWrdPt(0), mWrdPt(1), mWrdPt(2), R, G, B);
			}
		}
		fclose(fp);*/
	}
	
	return TRUE;
}

// 20220127，该函数负责在所有图像完成了特征提取、特征跟踪，已经拥有特征轨迹的基础上进行增量式SfM稀疏重建和图像定向
UINT SfM_incremental(LPVOID param)
{
	CDeepVoidApp * pApp = (CDeepVoidApp * )param;

	int nCam = pApp->m_vCams.size(); // 图像总数量
	
	CString strInfo;

	const CString & dirOut = pApp->m_pathDirOut; // 结果输出文件夹路径
	const std::vector<CString> & vImgNames = pApp->m_vNameImgs; // 所有图像的纯名称（不含路径和尾缀）

	// 20220128，在重建图像所在目录下新建结果输出文件夹（如果事先不存在的话）///
	int code = mkdir(dirOut); // code=0 说明成功新建；code=-1 说明文件夹已存在

	// 把要更新和要用的全局变量引用过来 //////////////////////////////////////
	SfM_ZZK::PointCloud & pointCloud = pApp->m_mapPointCloud;
	pointCloud.clear(); // 先清空一下，这是SfM环节的主要产出

//	SfM_ZZK::MultiTracks & tracks = pApp->m_mapTracks; // 20220127，在SfM环节对tracks做的修改仅在于为每个像点是否为内点的标志位赋上值
	SfM_ZZK::MultiTracksWithFlags & tracks = pApp->m_mapTracks; // 20220202，新数据结构

	const SfM_ZZK::PairWise_F_matches_pWrdPts & pairMatchInfos = pApp->m_mapPairwiseFMatchesWrdPts; // 20220128，<<i,j>, <<F,matches>, pWrdPts>>，两视图i和j间的所有匹配信息，包括基础矩阵F、所有匹配matches、射影重建物点坐标pWrdPts
	//////////////////////////////////////////////////////////////////////////

	// 对所有图像对进行排序
	vector<SfM_ZZK::pair_ij_k> pairs;
	SfM_ZZK::RankImagePairs_TrackLengthSum(pairMatchInfos, tracks, pairs);

	CString strFile;
	int idx_refimg;
	bool bSuc_RO = false;

	for (auto iter_pair = pairs.begin(); iter_pair != pairs.end(); ++iter_pair)
	{
		int i = iter_pair->first.first;
		int j = iter_pair->first.second;

		cam_data & cam_i = pApp->m_vCams[i];
		cam_data & cam_j = pApp->m_vCams[j];

		// 找到两者之间的特征匹配
		auto iter_found = pairMatchInfos.find(iter_pair->first);

		Matx33d mR;
		Matx31d mt;
		SfM_ZZK::PointCloud map_pointcloud_tmp;

		// 相对定向
		bSuc_RO = RelativeOrientation(cam_i, cam_j, iter_found->second.first.first, iter_found->second.first.second,
			mR, mt, map_pointcloud_tmp, pApp->m_methodRO, pApp->m_threshRpjErrRO); // 20200623 inspect

		if (bSuc_RO)
		{
			cam_i.m_R = Matx33d();
			cam_i.m_t = Matx31d();
			cam_i.m_R(0, 0) = cam_i.m_R(1, 1) = cam_i.m_R(2, 2) = 1;

			cam_j.m_R = mR;
			cam_j.m_t = mt;

			pointCloud = map_pointcloud_tmp; // 正式录用所有生成的物点

			strInfo.Format("RO between " + vImgNames[i] + " and " + vImgNames[j] + " succeeded: number of cloud points are %d", pointCloud.size());
			pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

			cam_i.m_bOriented = true;
			cam_j.m_bOriented = true;

			idx_refimg = i;

			pApp->m_idxRefImg = idx_refimg;	// 告诉全局变量哪个是参考坐标系图像

			// 将所有重建出来的像点暂时赋上都为内点
			for (auto iter_wrdpt = pointCloud.begin(); iter_wrdpt != pointCloud.end(); ++iter_wrdpt)
			{
				const int & trackID = iter_wrdpt->first;

				auto iter_found_track = tracks.find(trackID);

				auto iter_found_i = iter_found_track->second.find(i);
				auto iter_found_j = iter_found_track->second.find(j);

				iter_found_i->second.second[0] = 1; // 为内点
				iter_found_j->second.second[0] = 1; // 为内点
			}

			strFile = "point cloud after RO of " + vImgNames[i] + " and " + vImgNames[j] + ".txt";

			break;
		}
		else
		{
			strInfo = "RO between " + vImgNames[i] + " and " + vImgNames[j] + " failed!";
			pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
		}
	}

	// 20200607，如果没有任何像对相对定向成功那就直接退出
	if (!bSuc_RO)
	{
		theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(_T("Relative Orientation failed for all image pair candidates!"));
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	vector<CloudPoint> cloud_old;

	SfM_ZZK::OutputPointCloud(dirOut + strFile, pointCloud, pApp->m_vCams, tracks, cloud_old, pApp->m_nMinInilier);

	double d_mean = MeanMinDistance_3D(cloud_old);
	strInfo.Format("average distance between cloud points is: %lf", d_mean);
	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

	vector<SfM_ZZK::pair_ij> imgRank;
	RankImages_NumObjPts(pApp->m_vCams, pointCloud, imgRank);

	// 20151103，全自动的增量式 SfM 循环 ////////////////////////////////////
	int n_pointcloud_size_old = pointCloud.size();

	bool bAllFail = false;
	while (imgRank.size() > 0 && !bAllFail) // 还有图没有完成定向且并不是所有剩余图像都定向失败了就继续
	{
		bAllFail = true;
		vector<SfM_ZZK::pair_ij> imgRank_tmp = imgRank;

		for (auto iter_img=imgRank_tmp.begin();iter_img!=imgRank_tmp.end();++iter_img)
		{
			const int & I = iter_img->first; // image I

			cam_data & cam_I = pApp->m_vCams[I];

			// 1. 尝试 RANSAC 后方交会
			Matx33d mR;
			Matx31d mt;
			bool bEOSuc;
			try
			{
				bEOSuc = EO_PnP_RANSAC(cam_I, pointCloud, mR, mt, pApp->m_threshRpjErrInlier, pApp->m_threshRatioInlier);
			}
			catch (cv::Exception & e)
			{
				CString str;
				str = e.msg.c_str();
				str+="	error happened in RANSAC EO";
				AfxMessageBox(str);
			}
			
			if (!bEOSuc) // 如果后方交会失败就考虑排在后面的图像
			{
				strInfo = "EO of " + vImgNames[I] + " failed!";
				pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

				continue;
			}

			// 到这里就说明后方交会成功了
			bAllFail = false;
			
			// 正式录入外参初值
			cam_I.m_R = mR;
			cam_I.m_t = mt;

			cam_I.m_bOriented = true;

			// 2. 稀疏光束法平差。只利用现有的物点，还没有利用新加入的图像前方交会新的物点
			strInfo = "EO of " + vImgNames[I] + " succeeded";
			pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

			double info[10], rltUctt, uctt_f;
			try
			{
//				int nnn = optim_sba_levmar_XYZ_ext_rotvec(map_pointcloud, pApp->m_vCams, map_tracks, idx_refimg, thresh_inlier_rpjerr, 64, opts, info);
				// 20200607 同时优化所有图像共有的等效焦距 f
				int nnn = DeepVoid::optim_sba_levmar_f_XYZ_ext_rotvec_IRLS_Huber(pointCloud, pApp->m_vCams, tracks, rltUctt, uctt_f, idx_refimg,
					pApp->m_threshRpjErrInlier, pApp->m_nMaxIter, pApp->m_optsLM, info);
			}
			catch (cv::Exception & e)
			{
				CString str;
				str = e.msg.c_str();
				str+="	error happened in SBA";
				AfxMessageBox(str);
			}

			strInfo.Format("SBA ends, point cloud size: %d, initial err: %lf, final err: %lf, iter: %04.0f, code: %01.0f, rltUctt(1): %f, uctt_f(1): %f",
				pointCloud.size(), info[0], info[1], info[3], info[4], rltUctt, uctt_f);

			theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

			strFile = "point cloud after successful EO of " + vImgNames[I] + " and bundle adjustment.txt";

			try
			{
				SfM_ZZK::OutputPointCloud(dirOut + strFile, pointCloud, pApp->m_vCams, tracks, cloud_old, pApp->m_nMinInilier);
			}
			catch (cv::Exception & e)
			{
				CString str;
				str = e.msg.c_str();
				str+="	error happened in OutputPointCloud";
				AfxMessageBox(str);
			}

			// 3. 前方交会
			try
			{
				SfM_ZZK::Triangulation_AddOneImg(pointCloud, pApp->m_vCams, tracks, I, pApp->m_threshRpjErrInlier);
			}
			catch (cv::Exception & e)
			{
				CString str;
				str = e.msg.c_str();
				str+="	error happened in Triangulation";
				AfxMessageBox(str);
			}

			// 4. 如果点云拓展了，就对剩下的图像重新进行排序
			int n_pointcloud_size_new = pointCloud.size();

			int nPointsAdded = n_pointcloud_size_new - n_pointcloud_size_old;

			strInfo.Format("%d new points are triangulated with " + vImgNames[I], nPointsAdded);
			pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

			if (n_pointcloud_size_new != n_pointcloud_size_old)
			{
				try
				{
					RankImages_NumObjPts(pApp->m_vCams, pointCloud, imgRank);
				}
				catch (cv::Exception & e)
				{
					CString str;
					str = e.msg.c_str();
					str+="	error happened in RankImages";
					AfxMessageBox(str);
				}

				n_pointcloud_size_old = n_pointcloud_size_new;

				break;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////


	// 完成最后一次前方交会后，物点个数就固定了，再总的来一次稀疏光束法平差 ///////
	double info[10], rltUctt, uctt_f;
//	int nnn = optim_sba_levmar_XYZ_ext_rotvec(map_pointcloud, pApp->m_vCams, map_tracks, idx_refimg, thresh_inlier_rpjerr, /*1024*/64,opts, info);
	// 20200607 同时优化所有图像共有的等效焦距 f
	int nnn = DeepVoid::optim_sba_levmar_f_XYZ_ext_rotvec_IRLS_Huber(pointCloud, pApp->m_vCams, tracks, rltUctt, uctt_f, idx_refimg,
		pApp->m_threshRpjErrInlier, pApp->m_nMaxIter, pApp->m_optsLM, info);

	strInfo.Format("SBA ends, point cloud size: %d, initial err: %lf, final err: %lf, iter: %04.0f, code: %01.0f, rltUctt(1): %f, uctt_f(1): %f",
		pointCloud.size(), info[0], info[1], info[3], info[4], rltUctt, uctt_f);

	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
	//////////////////////////////////////////////////////////////////////////


	// 20220203，进行像点坐标匹配优化（减少同名像点于不同图像间的匹配误差）///////
	if (pApp->m_bRefineImgPts)
	{
		pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo("Image matching refinement starts");

		DeepVoid::RefineMatchingAccuracy(pointCloud, tracks, pApp->m_vCams, pApp->m_imgsOriginal, pApp->m_wndSizeImgptRefine, pApp->m_nMaxIter, pApp->m_xEpsMPGC, pApp->m_fEpsMPGC);

		pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo("Image matching refinement ends");

		// 这里应该再来一次 SBA，因为像点坐标变化了

		double info[10], rltUctt, uctt_f;
//		int nnn = optim_sba_levmar_XYZ_ext_rotvec(map_pointcloud, pApp->m_vCams, map_tracks, idx_refimg, thresh_inlier_rpjerr, /*1024*/64,opts, info);
		// 20200607 同时优化所有图像共有的等效焦距 f
		int nnn = DeepVoid::optim_sba_levmar_f_XYZ_ext_rotvec_IRLS_Huber(pointCloud, pApp->m_vCams, tracks, rltUctt, uctt_f, idx_refimg,
			pApp->m_threshRpjErrInlier, pApp->m_nMaxIter, pApp->m_optsLM, info);

		strInfo.Format("SBA ends, point cloud size: %d, initial err: %lf, final err: %lf, iter: %04.0f, code: %01.0f, rltUctt(1): %f, uctt_f(1): %f",
			pointCloud.size(), info[0], info[1], info[3], info[4], rltUctt, uctt_f);

		theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
	}
	//////////////////////////////////////////////////////////////////////////


	// 结果输出 //////////////////////////////////////////////////////////////////////////////
	std::map<int,int> hist_cloudpoint_inlier;
	double length_average = SfM_ZZK::BuildCloudPointInlierHistogram(pointCloud, tracks, hist_cloudpoint_inlier);

	FILE * file = fopen(dirOut + "hist.txt", "w");
	for (auto iter_hist = hist_cloudpoint_inlier.begin(); iter_hist != hist_cloudpoint_inlier.end(); ++iter_hist)
	{
		fprintf(file,"%d	%d\n", iter_hist->first, iter_hist->second);
	}
	fprintf(file,"%lf", length_average);
	fclose(file);

	// 输出点云
	strFile.Format("Final point cloud.txt");
	SfM_ZZK::OutputPointCloud(dirOut + strFile, pointCloud, pApp->m_vCams, tracks, cloud_old, pApp->m_nMinInilier);

	// 输出像机定向
	// save all cameras' parameters
	for (int i = 0; i < nCam; i++)
	{
		strFile = vImgNames[i] + "_param.txt";
		SaveCameraData(dirOut + strFile, pApp->m_vCams[i]);
	}
	/////////////////////////////////////////////////////////////////////////////////////////


	// 20220127，用来表征完成了稀疏三维重建，使能三维显示
	pApp->m_b3DReady_sparse = TRUE;

	return TRUE;
}


void CDeepVoidApp::OnSfm()
{
	// TODO: Add your command handler code here
	int n_imgs = m_vCams.size();

	m_vCams.clear();

	int i;
	for (i=0;i<n_imgs;i++)
	{
		cam_data cam;
		m_vCams.push_back(cam);
	}

	AfxBeginThread(SfM, this, THREAD_PRIORITY_NORMAL);
}

UINT Dense(LPVOID param)
{
	CDeepVoidApp * pApp = (CDeepVoidApp * )param;

	//////////////////////////////////////////////////////////////////////////
	int i,j,k,ii,jj,kk, iter;

	int nCam = pApp->m_vCams.size();

	vector<Mat> vImages;

	int imgWidth, imgHeight;

	//vector<Mat> vDepths_optimized, vHxs_optimized, vHys_optimized, vVisis_optimized;

	int wndSize = 5;
	int halfwnd = (wndSize-1)*0.5;
	int wndOptim = wndSize;
	double thresh_ncc = 0.6;
	double thresh_norm = 80;
	int mincam = 1;
	/*double minncc = thresh_ncc*mincam/(pApp->m_vCams.size()-1);*/
	double minncc = thresh_ncc;
	double maxncc_minncc_1 = 1/(1-minncc);
	double thresh_normdir = 0.1;

	for (kk=0;kk<nCam;kk++)
	{
		// read corresponding image data into memory first
		char * pDir = (char *)pApp->m_pMainFrame->m_wndImgThumbnailPane.m_wndImgListCtrl.GetItemData(kk);

		CString strDir;
		strDir.Format(_T("%s"), pDir);
		strDir.Trim();

		Mat img = imread(strDir.GetBuffer(), CV_LOAD_IMAGE_UNCHANGED);

		vImages.push_back(img); // store every image

		imgWidth = img.cols;
		imgHeight = img.rows;

		CString strtmp;
		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140515\\cam%02d.txt", kk);
		ReadCameraData(strtmp, pApp->m_vCams[kk]);
	}

	// first, extract all the features in each image
	for (kk=0;kk<nCam;kk++)
	{
		CString strtmp;
		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140519\\final PatchMatch depth map ref %02d.txt", kk);
		FILE * file_depth = fopen(strtmp, "r");
		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140519\\final PatchMatch hx map ref %02d.txt", kk);
		FILE * file_hx = fopen(strtmp, "r");
		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140519\\final PatchMatch hy map ref %02d.txt", kk);
		FILE * file_hy = fopen(strtmp, "r");
		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140519\\final PatchMatch score map ref %02d.txt", kk);
		FILE * file_score = fopen(strtmp, "r");
		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140519\\final PatchMatch visi map ref %02d.txt", kk);
		FILE * file_visi = fopen(strtmp, "r");

		Mat mDepth_init(imgHeight, imgWidth, CV_32FC1);
		Mat mhx_init(imgHeight, imgWidth, CV_32FC1);
		Mat mhy_init(imgHeight, imgWidth, CV_32FC1);
		Mat mScore(imgHeight, imgWidth, CV_32FC1);
		Mat mVisi_init(imgHeight, imgWidth, CV_8UC1);

		for (i=0;i<imgHeight;i++)
		{
			for (j=0;j<imgWidth;j++)
			{
				double d,hx,hy,score;
				int visi;
				fscanf(file_depth, "%lf	", &d);
				fscanf(file_hx, "%lf	", &hx);
				fscanf(file_hy, "%lf	", &hy);
				fscanf(file_score, "%lf	", &score);
				fscanf(file_visi, "%d	", &visi);

				mDepth_init.at<float>(i,j) = d;
				mhx_init.at<float>(i,j) = hx;
				mhy_init.at<float>(i,j) = hy;
				mScore.at<float>(i,j) = score;
				mVisi_init.at<uchar>(i,j) = (uchar)visi;
			}
		}
		fclose(file_depth);
		fclose(file_hx);
		fclose(file_hy);
		fclose(file_score);
		fclose(file_visi);

		Mat mDepth_optim(imgHeight, imgWidth, CV_32FC1, Scalar(-1));
		Mat mhx_optim(imgHeight, imgWidth, CV_32FC1, Scalar(-1));
		Mat mhy_optim(imgHeight, imgWidth, CV_32FC1, Scalar(-1));
		Mat mVisi_optim(imgHeight, imgWidth, CV_8UC1, Scalar(0));
		Mat mVisiN_optim(imgHeight, imgWidth, CV_8UC1, Scalar(0));

		double max_depth,min_depth,max_incre_x,min_incre_x,max_incre_y,min_incre_y,max_score,min_score;

		minMaxIdx(mDepth_init, &min_depth, &max_depth);
		minMaxIdx(mhx_init, &min_incre_x, &max_incre_x);
		minMaxIdx(mhy_init, &min_incre_y, &max_incre_y);

		double fx_ref = pApp->m_vCams[kk].fx;
		double fx_ref_1 = 1/fx_ref;
		double fy_ref = pApp->m_vCams[kk].fy;
		double fy_ref_1 = 1/fy_ref;
		double cx_ref = pApp->m_vCams[kk].cx;
		double cy_ref = pApp->m_vCams[kk].cy;

		for (i=0;i<imgHeight;i++)
		{
			CString strInfo;
			strInfo.Format("optimize row %04d", i);
			pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

			double nimgy = (i-cy_ref)*fy_ref_1;

			for (j=0;j<imgWidth;j++)
			{
				double nimgx = (j-cx_ref)*fx_ref_1;

				double depth_init = mDepth_init.at<float>(i,j);
				double hx_init = mhx_init.at<float>(i,j);
				double hy_init = mhy_init.at<float>(i,j);
				double score = mScore.at<float>(i,j);

				if (score<0)
				{
					mDepth_optim.at<float>(i,j) = mDepth_init.at<float>(i,j);
					mhx_optim.at<float>(i,j) = mhx_init.at<float>(i,j);
					mhy_optim.at<float>(i,j) = mhy_init.at<float>(i,j);
					continue;
				}

				int i_real, j_real;
				MakeSureNotOutBorder(j,i,j_real,i_real,halfwnd,imgWidth,imgHeight);

				uchar visi = mVisi_init.at<uchar>(i,j);
				vector<bool> vbools;
				InterpVisiVector_uchar(visi, vbools);

				bool bOptimFailed = false;

				for (iter=0;iter<4;iter++)
				{
					vector<int> vIdxValidCam;
					for (k=0;k<vImages.size();k++)
					{
						if (!vbools[k])
						{
							continue;
						}
						vIdxValidCam.push_back(k);
					}

					double depth_optim, hx_optim, hy_optim;
					if (optim_gn_drhxhyck(pApp->m_vCams, vImages, vIdxValidCam, kk, j_real, i_real, wndOptim, wndOptim,
						depth_init, hx_init, hy_init, depth_optim, hx_optim, hy_optim, false, 20, 1.0E-8, 1.0E-6) &&
						depth_optim>min_depth && depth_optim<max_depth
						//&& isvalid_hxhy(fx_ref,fy_ref,nimgx,nimgy,depth_optim,hx_optim,hy_optim,thresh_normdir)// hx_optim>min_incre_x && hx_optim<max_incre_x && hy_optim>min_incre_y && hy_optim<max_incre_y
						)
					{
// 						mDepth_optim.at<float>(i,j) = depth_optim;
// 						mhx_optim.at<float>(i,j) = hx_optim;
// 						mhy_optim.at<float>(i,j) = hy_optim;
						depth_init = depth_optim;
						hx_init = hx_optim;
						hy_init = hy_optim;
					}
					else
					{
// 						mDepth_optim.at<float>(i,j) = mDepth_init.at<float>(i,j);
// 						mhx_optim.at<float>(i,j) = mhx_init.at<float>(i,j);
// 						mhy_optim.at<float>(i,j) = mhy_init.at<float>(i,j);
// 						mScore.at<float>(i,j) = -1;
						bOptimFailed = true;
						break;
					}

					vector<double> vScores;
					CheckOnePixel_givenOneParamSet(pApp->m_vCams,vImages,kk,j_real,i_real,depth_init,hx_init,hy_init,vScores,wndSize);

					for (k=0;k<vImages.size();k++)
					{
						if (!vbools[k]) // was invisible
						{
							if (vScores[k]>/*0.8*/thresh_ncc)
							{
								vbools[k] = true; // set to visible
							}
						}
						else
						{
							if (vScores[k]<=thresh_ncc)
							{
								vbools[k] = false; // set to invisible
							}
						}
					}

					uchar visi_new = GetVisibilityVector_uchar(vbools);
					uchar visi_diff = hamdist(visi_new, visi);
					visi = visi_new;

					if (visi==0)
					{
						bOptimFailed = true;
						break;
					}

					if (visi_diff==0)
					{
						break;
					}
				}

				if (bOptimFailed)
				{
					mDepth_optim.at<float>(i,j) = mDepth_init.at<float>(i,j);
					mhx_optim.at<float>(i,j) = mhx_init.at<float>(i,j);
					mhy_optim.at<float>(i,j) = mhy_init.at<float>(i,j);
					mScore.at<float>(i,j) = -1;
				} 
				else
				{
					mDepth_optim.at<float>(i,j) = depth_init;
					mhx_optim.at<float>(i,j) = hx_init;
					mhy_optim.at<float>(i,j) = hy_init;
				}

				mVisi_optim.at<uchar>(i,j) = visi;
				int nnn = 0;
				for (iter=0;iter<vImages.size();iter++)
				{
					if (vbools[iter])
					{
						++nnn;
					}
				}
				mVisiN_optim.at<uchar>(i,j) = (uchar)nnn;
			}
		}

		CString strInfo;
		strInfo.Format("%lf	%lf	%lf	%lf	%lf	%lf", min_depth, max_depth, min_incre_x, max_incre_x, min_incre_y, max_incre_y);
		pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

		minMaxIdx(mDepth_optim, &min_depth, &max_depth);
		minMaxIdx(mhx_optim, &min_incre_x, &max_incre_x);
		minMaxIdx(mhy_optim, &min_incre_y, &max_incre_y);
		minMaxIdx(mScore, &min_score, &max_score);
		strInfo.Format("%lf	%lf	%lf	%lf	%lf	%lf", min_depth, max_depth, min_incre_x, max_incre_x, min_incre_y, max_incre_y);
		pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

		Mat mDepth_optim_normed, mhx_optim_normed, mhy_optim_normed;
		Mat mScores_normed_color(imgHeight, imgWidth, CV_8UC3), mVisiN_color(imgHeight, imgWidth, CV_8UC3);

		mDepth_optim.convertTo(mDepth_optim_normed, CV_8UC1, 255/(max_depth-min_depth), -255*min_depth/(max_depth-min_depth));
		mhx_optim.convertTo(mhx_optim_normed, CV_8UC1, 255/(max_incre_x-min_incre_x), -255*min_incre_x/(max_incre_x-min_incre_x));
		mhy_optim.convertTo(mhy_optim_normed, CV_8UC1, 255/(max_incre_y-min_incre_y), -255*min_incre_y/(max_incre_y-min_incre_y));

		for (ii=0;ii<imgHeight;ii++)
		{
			for (jj=0;jj<imgWidth;jj++)
			{
				if (mScore.at<float>(ii,jj)<0)
				{
					mScores_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
					mScores_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
					mScores_normed_color.at<Vec3b>(ii,jj).val[2] = 255;
				}
				else
				{
					mScores_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
					mScores_normed_color.at<Vec3b>(ii,jj).val[1] = FTOI(255*(mScore.at<float>(ii,jj)-minncc)*maxncc_minncc_1);
					mScores_normed_color.at<Vec3b>(ii,jj).val[2] = 0;
				}

				uchar nnn = mVisiN_optim.at<uchar>(ii,jj);

				if (nnn==0)
				{
					mVisiN_color.at<Vec3b>(ii,jj).val[0] = 0;
					mVisiN_color.at<Vec3b>(ii,jj).val[1] = 0;
					mVisiN_color.at<Vec3b>(ii,jj).val[2] = 255;
				}
				else if (nnn==1)
				{
					mVisiN_color.at<Vec3b>(ii,jj).val[0] = 0;
					mVisiN_color.at<Vec3b>(ii,jj).val[1] = 255;
					mVisiN_color.at<Vec3b>(ii,jj).val[2] = 255;
				}
				else if (nnn==2)
				{
					mVisiN_color.at<Vec3b>(ii,jj).val[0] = 255;
					mVisiN_color.at<Vec3b>(ii,jj).val[1] = 0;
					mVisiN_color.at<Vec3b>(ii,jj).val[2] = 255;
				}
				else if (nnn==3)
				{
					mVisiN_color.at<Vec3b>(ii,jj).val[0] = 0;
					mVisiN_color.at<Vec3b>(ii,jj).val[1] = 255;
					mVisiN_color.at<Vec3b>(ii,jj).val[2] = 0;
				}
				else
				{
					mVisiN_color.at<Vec3b>(ii,jj).val[0] = 255;
					mVisiN_color.at<Vec3b>(ii,jj).val[1] = 0;
					mVisiN_color.at<Vec3b>(ii,jj).val[2] = 0;
				}
			}
		}

		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\depth map optimized ref %02d.bmp", kk);
		imwrite(strtmp.GetBuffer(), mDepth_optim_normed);
		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\hx map optimized ref %02d.bmp", kk);
		imwrite(strtmp.GetBuffer(), mhx_optim_normed);
		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\hy map optimized ref %02d.bmp", kk);
		imwrite(strtmp.GetBuffer(), mhy_optim_normed);
		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\score map optimized ref %02d.bmp", kk);
		imwrite(strtmp.GetBuffer(), mScores_normed_color);
		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\visi map optimized ref %02d.bmp", kk);
		imwrite(strtmp.GetBuffer(), mVisi_optim);
		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\visiN map optimized ref %02d.bmp", kk);
		imwrite(strtmp.GetBuffer(), mVisiN_color);

		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\cloud points optimized ref %02d.txt", kk);
		WriteDepthMap(strtmp,pApp->m_vCams[kk],vImages[kk],mDepth_optim,mScore);

		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final optimized depth map ref %02d.txt", kk);
		file_depth = fopen(strtmp, "w");
		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final optimized hx map ref %02d.txt", kk);
		file_hx = fopen(strtmp, "w");
		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final optimized hy map ref %02d.txt", kk);
		file_hy = fopen(strtmp, "w");
		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final optimized score map ref %02d.txt", kk);
		file_score = fopen(strtmp, "w");
		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final optimized visi map ref %02d.txt", kk);
		file_visi = fopen(strtmp, "w");

		for (i=0;i<mDepth_optim.rows;i++)
		{
			for (j=0;j<mDepth_optim.cols;j++)
			{
				double score = mScore.at<float>(i,j);
				if (score<0)
				{
					double tmp = -1;
					fprintf(file_depth, "%.12f	", tmp);
					fprintf(file_hx, "%.12f	", tmp);
					fprintf(file_hy, "%.12f	", tmp);
					fprintf(file_score, "%.12f	", tmp);
				} 
				else
				{
					fprintf(file_depth, "%.12f	", mDepth_optim.at<float>(i,j));
					fprintf(file_hx, "%.12f	", mhx_optim.at<float>(i,j));
					fprintf(file_hy, "%.12f	", mhy_optim.at<float>(i,j));
					fprintf(file_score, "%.12f	", mScore.at<float>(i,j));
				}

				fprintf(file_visi, "%d	", mVisi_optim.at<uchar>(i,j));
			}
			fprintf(file_depth, "\n");
			fprintf(file_hx, "\n");
			fprintf(file_hy, "\n");
			fprintf(file_score, "\n");
			fprintf(file_visi, "\n");
		}
		fclose(file_depth);
		fclose(file_hx);
		fclose(file_hy);
		fclose(file_score);
		fclose(file_visi);

// 		vDepths_optimized.push_back(mDepth_optim);
// 		vHxs_optimized.push_back(mhx_optim);
// 		vHys_optimized.push_back(mhy_optim);
// 		/*vScores_init.push_back(mScore_init);*/
// 		vVisis_optimized.push_back(mVisi_optim);
	}
	//////////////////////////////////////////////////////////////////////////

	return TRUE;
}

void CDeepVoidApp::OnDense()
{
	// TODO: Add your command handler code here
// 	CString strInfo;
// 	double d;
// 	for (d= -5; d <= +5; d+= 0.125)
// 	{
// 		strInfo.Format("%.3f: %d %d %d %d %d %d\n", d, (int)floor(d), floor_fast_noshift(d), floor_fast(d), (int)ceil(d), ceil_fast_noshift(d), ceil_fast(d));
// 		theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 	}
// 
// 	d = -.000056;
// 	strInfo.Format("%.3f: %d %d %d %d %d %d\n", d, (int)floor(d), floor_fast_noshift(d), floor_fast(d), (int)ceil(d), ceil_fast_noshift(d), ceil_fast(d));
// 	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 
// 	d = .000056;
// 	strInfo.Format("%.3f: %d %d %d %d %d %d\n", d, (int)floor(d), floor_fast_noshift(d), floor_fast(d), (int)ceil(d), ceil_fast_noshift(d), ceil_fast(d));
// 	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 
// 	d = -4.000056;
// 	strInfo.Format("%.3f: %d %d %d %d %d %d\n", d, (int)floor(d), floor_fast_noshift(d), floor_fast(d), (int)ceil(d), ceil_fast_noshift(d), ceil_fast(d));
// 	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 
// 	d = 4.000056;
// 	strInfo.Format("%.3f: %d %d %d %d %d %d\n", d, (int)floor(d), floor_fast_noshift(d), floor_fast(d), (int)ceil(d), ceil_fast_noshift(d), ceil_fast(d));
// 	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 
// 	d = -1000.000000006;
// 	strInfo.Format("%.3f: %d %d %d %d %d %d\n", d, (int)floor(d), floor_fast_noshift(d), floor_fast(d), (int)ceil(d), ceil_fast_noshift(d), ceil_fast(d));
// 	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 
// 	d = 1000.000000006;
// 	strInfo.Format("%.3f: %d %d %d %d %d %d\n", d, (int)floor(d), floor_fast_noshift(d), floor_fast(d), (int)ceil(d), ceil_fast_noshift(d), ceil_fast(d));
// 	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 
// 	d = 1000.99999999999999;
// 	strInfo.Format("%.3f: %d %d %d %d %d %d\n", d, (int)floor(d), floor_fast_noshift(d), floor_fast(d), (int)ceil(d), ceil_fast_noshift(d), ceil_fast(d));
// 	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 
// 	int i,j;
// 
// 	int nJ = 10000;
// 
// 	double V[1000];
// 	for (i= 0; i < 1000; i++)
// 	{
// 		V[i]= (i - 500) / 10.;
// 	}
// 
// 	strInfo.Format("standard floor starts");
// 	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 	int S=0;
// 	for (j= 0; j < nJ; j++)
// 	{
// 		for (i= 0; i < 1000; i++)
// 		{
// 			S+= int(floor(V[i]));
// 		}
// 	}
// 	strInfo.Format("standard floor ends: %d", S);
// 	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 
// 	strInfo.Format("fast floor without shifting starts");
// 	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 	S=0;
// 	for (j= 0; j < nJ; j++)
// 	{
// 		for (i= 0; i < 1000; i++)
// 		{
// 			S+= floor_fast_noshift(V[i]);
// 		}
// 	}
// 	strInfo.Format("fast floor without shifting ends: %d", S);
// 	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 
// 	strInfo.Format("fast floor with shifting starts");
// 	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 	S=0;
// 	for (j= 0; j < nJ; j++)
// 	{
// 		for (i= 0; i < 1000; i++)
// 		{
// 			S+= floor_fast(V[i]);
// 		}
// 	}
// 	strInfo.Format("fast floor with shifting ends: %d", S);
// 	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 
// 	strInfo.Format("standard ceil starts");
// 	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 	S=0;
// 	for (j= 0; j < nJ; j++)
// 	{
// 		for (i= 0; i < 1000; i++)
// 		{
// 			S+= int(ceil(V[i]));
// 		}
// 	}
// 	strInfo.Format("standard ceil ends: %d", S);
// 	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 
// 	strInfo.Format("fast ceil without shifting starts");
// 	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 	S=0;
// 	for (j= 0; j < nJ; j++)
// 	{
// 		for (i= 0; i < 1000; i++)
// 		{
// 			S+= ceil_fast_noshift(V[i]);
// 		}
// 	}
// 	strInfo.Format("fast ceil without shifting ends: %d", S);
// 	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 
// 	strInfo.Format("fast ceil with shifting starts");
// 	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 	S=0;
// 	for (j= 0; j < nJ; j++)
// 	{
// 		for (i= 0; i < 1000; i++)
// 		{
// 			S+= ceil_fast(V[i]);
// 		}
// 	}
// 	strInfo.Format("fast ceil with shifting ends: %d", S);
// 	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);


// 	Matx31f mKx,mKy;
// 	Matx33f mDx,mDy;
// 
// 	try
// 	{
// 		getDerivKernels(mKx, mKy, 1, 0, CV_SCHARR, true, CV_32F);
// 		mDx = mKy*mKx.t();
// 		mDy = mDx.t();
// 	}
// 	catch (cv::Exception & e)
// 	{
// 		CString str;
// 		str = e.msg.c_str();
// 		AfxMessageBox(str);
// 	}
// 
// 	double aaa = 1+1;


// 	Mat img = imread("E:\\Test Data\\SET001_6\\SET001\\09\\Img001_09.bmp", CV_LOAD_IMAGE_UNCHANGED);
// 
// 	int patchWidth = 5;
// 	int patchHeight = 5;
// 	int half_w = (patchWidth-1)*0.5;
// 	int half_h = (patchHeight-1)*0.5;
// 
// 	double x_check = 190.15;
// 	double y_check = 130.15;
// 
// 	vector<Point2d> vImgPts;
// 	int i,j;
// 	for (i=-half_h;i<=half_h;i++)
// 	{
// 		for (j=-half_w;j<=half_w;j++)
// 		{
// 			Point2d pt2d;
// 			pt2d.x = x_check+j;
// 			pt2d.y = y_check+i;
// 			vImgPts.push_back(pt2d);
// 		}
// 	}
// 
// 	Mat mPatch, mGx, mGy;
// 	MPGC_get_patch_gx_gy_scharr(img, vImgPts, patchWidth, patchHeight, mPatch, mGx, mGy);

	AfxBeginThread(Dense, this, THREAD_PRIORITY_NORMAL);
}

UINT MySGM(LPVOID param)
{
	CDeepVoidApp * pApp = (CDeepVoidApp * )param;

	Mat imgb_original = imread("E:\\Test Data\\rectified stereo pair\\left_rectified1.png", CV_LOAD_IMAGE_UNCHANGED);
	Mat imgm_original = imread("E:\\Test Data\\rectified stereo pair\\right_rectified1.png", CV_LOAD_IMAGE_UNCHANGED);

	int imgHeight = imgb_original.rows;
	int imgWidth = imgb_original.cols;

	int n_sampPix = 2;
	imgWidth /= n_sampPix;
	imgHeight /= n_sampPix;

	int imgSize = imgHeight * imgWidth;

	BYTE ** pImgB = new BYTE * [imgHeight];
	pImgB[0] = new BYTE [imgSize];
	memset(pImgB[0], 0, imgSize * sizeof(BYTE));
	int i, j;
	for (i = 1; i < imgHeight; i++)
	{
		pImgB[i] = pImgB[i - 1] + imgWidth;
	}

	BYTE ** pImgM = new BYTE * [imgHeight];
	pImgM[0] = new BYTE [imgSize];
	memset(pImgM[0], 0, imgSize * sizeof(BYTE));
	for (i = 1; i < imgHeight; i++)
	{
		pImgM[i] = pImgM[i - 1] + imgWidth;
	}

	double ** pDI = new double * [imgHeight];
	pDI[0] = new double [imgSize];
	memset(pDI[0], 0, imgSize * sizeof(double));
	for (i = 1; i < imgHeight; i++)
	{
		pDI[i] = pDI[i - 1] + imgWidth;
	}

	for (i=0;i<imgHeight;i++)
	{
		for (j=0;j<imgWidth;j++)
		{
			Vec3b pix = imgb_original.at<Vec3b>(i*n_sampPix,j*n_sampPix);
			uchar b = pix.val[0];
			pImgB[i][j] = b;

			pix = imgm_original.at<Vec3b>(i*n_sampPix,j*n_sampPix);
			b = pix.val[0];
			pImgM[i][j] = b;
		}
	}

	// Start SGM for the base image
	SemiGlobalMatching(imgWidth, imgHeight, pImgB, pImgM, 30, 70, 15, 100, pDI, SGM_PATHS_8, 1, true);

	// locate depth discontinuities
	// 	int i_samp, j_samp;
	// 	for (i=0;i<imgb_original.m_pImg->height;i++)
	// 	{
	// 		for (j=0;j<imgb_original.m_pImg->width;j++)
	// 		{
	// 			i_samp = int(i/(double)n_sampPix+0.5);
	// 			j_samp = int(j/(double)n_sampPix+0.5);
	// 
	// 			if (i_samp>=imgb_gray.m_pImg->height||j_samp>=imgb_gray.m_pImg->width||pDI[i_samp][j_samp]<=0)
	// 			{
	// 				continue;
	// 			}
	// 			double cur_d = pDI[i_samp][j_samp];
	// 			bool bDis = false;
	// 			for (ii=-1;ii<=1;ii++)
	// 			{
	// 				int i_tmp = i_samp + ii;
	// 				if (i_tmp<0||i_tmp>=imgb_gray.m_pImg->height)
	// 				{
	// 					continue;
	// 				}
	// 				for (jj=-1;jj<=1;jj++)
	// 				{
	// 					int j_tmp = j_samp + jj;
	// 					if (j_tmp<0||j_tmp>=imgb_gray.m_pImg->width)
	// 					{
	// 						continue;
	// 					}
	// 					double d_tmp = pDI[i_tmp][j_tmp];
	// 					if (d_tmp<=0)
	// 					{
	// 						continue;
	// 					}
	// 					if (fabs(d_tmp-cur_d)>=1)
	// 					{
	// 						bDis=true;
	// 						break;
	// 					}
	// 				}
	// 				if (bDis)
	// 				{
	// 					break;
	// 				}
	// 			}
	// 			if (bDis)
	// 			{
	// 				CvScalar scl;
	// 				scl.val[0] = scl.val[1] = scl.val[2] = 0;
	// 				cvSet2D(imgb_original.m_pImg, i, j, scl);
	// 			}
	// 		}
	// 	}
	// 
	// 	imgb_original.Show("b");
	// 
	// 	int idx;
	// 	double vvv = 0;
	// 	double maxVal = FindArrayMax(pDI[0], imgb_gray.m_pImg->height * imgb_gray.m_pImg->width, idx);
	// 	double minVal = FindArrayMin(pDI[0], imgb_gray.m_pImg->height * imgb_gray.m_pImg->width, idx, &vvv);
	// 	CMatrix mVal(2,1);
	// 	mVal(1) = maxVal;
	// 	mVal(2) = minVal;
	// 	mVal.PrintMatrix2Screen();
	// 
	// 	// convert disparity image to real byte image
	// 	/*ConvertMtx2Image_int(imgb_gray.m_pImg->width, imgb_gray.m_pImg->height, pDI, pImgB);*/
	ConvertMtx2Image_double(imgWidth, imgHeight, pDI, pImgM);
	 
	Mat mDisparity(imgHeight, imgWidth, CV_8UC1);
	
	for (i=0;i<imgHeight;i++)
	{
		for (j=0;j<imgWidth;j++)
		{
			mDisparity.at<uchar>(i,j) = pImgM[i][j];
		}
	}
	imwrite("C:\\Users\\DeepVoid\\Desktop\\disparity.bmp", mDisparity);

	// set the baseline and focal length
	double b = 0.283401295273638;
	double ff = 1855/n_sampPix;

	double cx = 1296/n_sampPix;
	double cy = 1026.98084691563/n_sampPix;

	FILE * pfile = fopen("C:\\Users\\DeepVoid\\Desktop\\cloud.txt", "w");
	for (i=0;i<imgHeight;i++)
	{
		for (j=0;j<imgWidth;j++)
		{
			if (pDI[i][j] <= 0)
			{
				continue;
			}

			double b_d = b/pDI[i][j];
			double Z = b_d * ff;
			double X = b_d * (j - cx);
			double Y = b_d * (i - cy);

			fprintf(pfile, "%lf;%lf;%lf;%d;%d;%d\n",
				X, Y, Z, UCHAR(pImgB[i][j]), UCHAR(pImgB[i][j]), UCHAR(pImgB[i][j]));
		}
	}
	fclose(pfile);

	pfile = fopen("C:\\Users\\DeepVoid\\Desktop\\depth map.txt", "w");
	for (i=0;i<imgHeight;i++)
	{
		for (j=0;j<imgWidth;j++)
		{
			fprintf(pfile, "%lf	", pDI[i][j]);
		}
		fprintf(pfile, "\n");
	}
	fclose(pfile);

	// release memory
	delete [] pImgB[0];
	delete [] pImgB;

	delete [] pImgM[0];
	delete [] pImgM;

	delete [] pDI[0];
	delete [] pDI;

	return TRUE;
}

void CDeepVoidApp::OnMysgm()
{
	// TODO: Add your command handler code here
	AfxBeginThread(MySGM, this, THREAD_PRIORITY_NORMAL);
}

UINT DepthCheck(LPVOID param)
{
	CDeepVoidApp * pApp = (CDeepVoidApp * )param;

	//////////////////////////////////////////////////////////////////////////
	int i,j,k, ii,jj;

	int nCam = pApp->m_vCams.size();

	vector<Mat> vImages, vSilhouettes;
	vector<Mat> vDepths, vHxs, vHys, vDepths_final, vHxs_final, vHys_final, vQuality_final, vQuality_final_G;
	vector<Mat> vVisis;

	int imgWidth, imgHeight;

	// first, extract all the features in each image
	for (i=0;i<nCam;i++)
	{
		// read corresponding image data into memory first
		char * pDir = (char *)pApp->m_pMainFrame->m_wndImgThumbnailPane.m_wndImgListCtrl.GetItemData(i);

		CString strDir;
		strDir.Format(_T("%s"), pDir);
		strDir.Trim();

		Mat img = imread(strDir.GetBuffer(), CV_LOAD_IMAGE_UNCHANGED);

		imgWidth = img.cols;
		imgHeight = img.rows;

		vImages.push_back(img); // store every image

		strDir.Format("E:\\Test Data\\templeRing\\contours\\templeR%04d_seg.png", i+1);
		img = imread(strDir.GetBuffer(), CV_LOAD_IMAGE_GRAYSCALE);
		vSilhouettes.push_back(img);

		CString strtmp;
		/*strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\for depth consistency check\\set 01\\cam%02d.txt", i);*/
		/*strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\for depth consistency check\\templeSparseRing\\ncc 0.6 norm 80 factor 0.5 nrand 8 iter 4\\group 00-04\\cam%02d.txt", i);*/
		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140515\\cam%02d.txt", i);
		ReadCameraData(strtmp, pApp->m_vCams[i]);

		/*strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\for depth consistency check\\set 01\\ncc 0.5\\%d\\final optimized depth map.txt", i);*/
		/*strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\for depth consistency check\\templeSparseRing\\ncc 0.6 norm 80 factor 0.5 nrand 8 iter 4\\group 00-04\\%02d\\final optimized depth map.txt", i);*/
		/*strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140521 morning\\final optimized depth map fixed visi ref %02d.txt", i);*/
		/*strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140519 night\\final PatchMatch depth map ref %02d.txt", i);*/
		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140519 night\\final optimized depth map ref %02d.txt", i);
		FILE * file_depth = fopen(strtmp, "r");
		/*strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\for depth consistency check\\set 01\\ncc 0.5\\%d\\final optimized hx map.txt", i);*/
		/*strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\for depth consistency check\\templeSparseRing\\ncc 0.6 norm 80 factor 0.5 nrand 8 iter 4\\group 00-04\\%02d\\final optimized hx map.txt", i);*/
		/*strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140521 morning\\final optimized hx map fixed visi ref %02d.txt", i);*/
		/*strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140519 night\\final PatchMatch hx map ref %02d.txt", i);*/
		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140519 night\\final optimized hx map ref %02d.txt", i);
		FILE * file_hx = fopen(strtmp, "r");
		/*strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\for depth consistency check\\set 01\\ncc 0.5\\%d\\final optimized hy map.txt", i);*/
		/*strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\for depth consistency check\\templeSparseRing\\ncc 0.6 norm 80 factor 0.5 nrand 8 iter 4\\group 00-04\\%02d\\final optimized hy map.txt", i);*/
		/*strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140521 morning\\final optimized hy map fixed visi ref %02d.txt", i);*/
		/*strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140519 night\\final PatchMatch hy map ref %02d.txt", i);*/
		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140519 night\\final optimized hy map ref %02d.txt", i);
		FILE * file_hy = fopen(strtmp, "r");

		/*strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140521 morning\\visi map propagated ref %02d.txt", i);*/
		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140519 night\\final PatchMatch visi map ref %02d.txt", i);
		FILE * file_visi = fopen(strtmp, "r");

		Mat mDepth(imgHeight, imgWidth, CV_32FC1);
		Mat mhx(imgHeight, imgWidth, CV_32FC1);
		Mat mhy(imgHeight, imgWidth, CV_32FC1);
		Mat mVisi(imgHeight, imgWidth, CV_8UC1);

		for (ii=0;ii<imgHeight;ii++)
		{
			for (jj=0;jj<imgWidth;jj++)
			{
				double d,hx,hy,score;
				int visi;
				fscanf(file_depth, "%lf	", &d);
				fscanf(file_hx, "%lf	", &hx);
				fscanf(file_hy, "%lf	", &hy);
				fscanf(file_visi, "%d	", &visi);

				mDepth.at<float>(ii,jj) = d;
				mhx.at<float>(ii,jj) = hx;
				mhy.at<float>(ii,jj) = hy;
				mVisi.at<uchar>(ii,jj) = (uchar)visi;
			}
		}
		fclose(file_depth);
		fclose(file_hx);
		fclose(file_hy);
		fclose(file_visi);

		vDepths.push_back(mDepth);
		vHxs.push_back(mhx);
		vHys.push_back(mhy);
		vVisis.push_back(mVisi);
	}

	int wndSize = 5;
	double thresh_ncc = 0.6;
	double thresh_radio = 0.01;

	vector<Point2d> vMinMaxD, vMinMaxHx, vMinMaxHy, vMinMaxQ;

	// DepthConsistencyCheck(pApp->m_vCams, vImages, vDepths, vHxs, vHys, vDepths_final, vHxs_final, vHys_final, vMinMaxD, vMinMaxHx, vMinMaxHy, wndSize, thresh_ncc, thresh_radio);
	/*DepthConsistencyCheck_QualityEvaluation(pApp->m_vCams, vImages, vDepths, vHxs, vHys, vDepths_final, vHxs_final, vHys_final, vQuality_final, vMinMaxD, vMinMaxHx, vMinMaxHy, vMinMaxQ, wndSize, thresh_ncc, thresh_radio);*/
	DepthConsistencyCheck_QualityEvaluation_Goesele(pApp->m_vCams, vImages, vSilhouettes, vDepths, vHxs, vHys, vVisis, vDepths_final, vHxs_final, vHys_final, vQuality_final, vQuality_final_G, vMinMaxD, vMinMaxHx, vMinMaxHy, vMinMaxQ, wndSize, thresh_ncc, thresh_radio);

	//vector<Mat> vDepths_final_combined = vDepths_final;

	for (i=0;i<nCam;i++)
	{
		double mind = vMinMaxD[i].x; double maxd = vMinMaxD[i].y;
		double minhx = vMinMaxHx[i].x; double maxhx = vMinMaxHx[i].y;
		double minhy = vMinMaxHy[i].x; double maxhy = vMinMaxHy[i].y;
		double minq = vMinMaxQ[i].x; double maxq = vMinMaxQ[i].y;

		double maxd_mind_1 = 1/(maxd-mind);
		double maxhx_minhx_1 = 1/(maxhx-minhx);
		double maxhy_minhy_1 = 1/(maxhy-minhy);
		double maxq_minq_1 = 1/(maxq-minq);
		double threshratio_minq_1 = 1/(thresh_radio-minq);

		Mat mDepth_normed_color(imgHeight, imgWidth, CV_8UC3), mHx_normed_color(imgHeight, imgWidth, CV_8UC3), mHy_normed_color(imgHeight, imgWidth, CV_8UC3);
		Mat mQ_normed_color(imgHeight, imgWidth, CV_8UC3), mQ_interval_color(imgHeight, imgWidth, CV_8UC3);
		Mat mQ_normed_color_G(imgHeight, imgWidth, CV_8UC3), mQ_interval_color_G(imgHeight, imgWidth, CV_8UC3);

		for (ii=0;ii<imgHeight;ii++)
		{
			for (jj=0;jj<imgWidth;jj++)
			{
// 				if (vDepths_final_combined[i].at<float>(ii,jj)>0 && vQuality_final_G[i].at<float>(ii,jj)<0.9)
// 				{
// 					vDepths_final_combined[i].at<float>(ii,jj)=-1;
// 				}

				//////////////////////////////////////////////////////////////////////////
				if (vDepths_final[i].at<float>(ii,jj)<0)
				{
					mDepth_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
					mDepth_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
					mDepth_normed_color.at<Vec3b>(ii,jj).val[2] = 255;
				}
				else
				{
					int val = FTOI(255*(vDepths_final[i].at<float>(ii,jj)-mind)*maxd_mind_1);
					mDepth_normed_color.at<Vec3b>(ii,jj).val[0] = val;
					mDepth_normed_color.at<Vec3b>(ii,jj).val[1] = val;
					mDepth_normed_color.at<Vec3b>(ii,jj).val[2] = val;
				}

				//////////////////////////////////////////////////////////////////////////
				/*if (vHxs_final[i].at<float>(ii,jj)<0)*/
				if (fabs(vHxs_final[i].at<float>(ii,jj)+1)<1.0E-8)
				{
					mHx_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
					mHx_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
					mHx_normed_color.at<Vec3b>(ii,jj).val[2] = 255;
				}
				else
				{
					int val = FTOI(255*(vHxs_final[i].at<float>(ii,jj)-minhx)*maxhx_minhx_1);
					mHx_normed_color.at<Vec3b>(ii,jj).val[0] = val;
					mHx_normed_color.at<Vec3b>(ii,jj).val[1] = val;
					mHx_normed_color.at<Vec3b>(ii,jj).val[2] = val;
				}

				//////////////////////////////////////////////////////////////////////////
				/*if (vHys_final[i].at<float>(ii,jj)<0)*/
				if (fabs(vHys_final[i].at<float>(ii,jj)+1)<1.0E-8)
				{
					mHy_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
					mHy_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
					mHy_normed_color.at<Vec3b>(ii,jj).val[2] = 255;
				}
				else
				{
					int val = FTOI(255*(vHys_final[i].at<float>(ii,jj)-minhy)*maxhy_minhy_1);
					mHy_normed_color.at<Vec3b>(ii,jj).val[0] = val;
					mHy_normed_color.at<Vec3b>(ii,jj).val[1] = val;
					mHy_normed_color.at<Vec3b>(ii,jj).val[2] = val;
				}

				//////////////////////////////////////////////////////////////////////////
				if (fabs(vQuality_final[i].at<float>(ii,jj)+1)<1.0E-8)
				{
					mQ_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
					mQ_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
					mQ_normed_color.at<Vec3b>(ii,jj).val[2] = 255;

					mQ_interval_color.at<Vec3b>(ii,jj).val[0] = 0;
					mQ_interval_color.at<Vec3b>(ii,jj).val[1] = 0;
					mQ_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
				}
				else
				{
// 					if (vQuality_final[i].at<float>(ii,jj)>thresh_radio)
// 					{
// 						mQ_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
// 						mQ_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
// 						mQ_normed_color.at<Vec3b>(ii,jj).val[2] = 255;
// 					}
// 					else
// 					{
// 						int val = FTOI(255*(thresh_radio-vQuality_final[i].at<float>(ii,jj))*threshratio_minq_1);
// 						mQ_normed_color.at<Vec3b>(ii,jj).val[0] = val;
// 						mQ_normed_color.at<Vec3b>(ii,jj).val[1] = val;
// 						mQ_normed_color.at<Vec3b>(ii,jj).val[2] = val;
// 					}


					int val = FTOI(255*(maxq-vQuality_final[i].at<float>(ii,jj))*maxq_minq_1);
					mQ_normed_color.at<Vec3b>(ii,jj).val[0] = val;
					mQ_normed_color.at<Vec3b>(ii,jj).val[1] = val;
					mQ_normed_color.at<Vec3b>(ii,jj).val[2] = val;
					

					if (vQuality_final[i].at<float>(ii,jj)<=0.001)
					{
						mQ_interval_color.at<Vec3b>(ii,jj).val[0] = 255;
						mQ_interval_color.at<Vec3b>(ii,jj).val[1] = 0;
						mQ_interval_color.at<Vec3b>(ii,jj).val[2] = 0;
					}
					else if (vQuality_final[i].at<float>(ii,jj)<=0.005)
					{
						mQ_interval_color.at<Vec3b>(ii,jj).val[0] = 0;
						mQ_interval_color.at<Vec3b>(ii,jj).val[1] = 255;
						mQ_interval_color.at<Vec3b>(ii,jj).val[2] = 0;
					}
					else if (vQuality_final[i].at<float>(ii,jj)<=0.01)
					{
						mQ_interval_color.at<Vec3b>(ii,jj).val[0] = 0;
						mQ_interval_color.at<Vec3b>(ii,jj).val[1] = 255;
						mQ_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
					}
					else if (vQuality_final[i].at<float>(ii,jj)<=0.05)
					{
						mQ_interval_color.at<Vec3b>(ii,jj).val[0] = 255;
						mQ_interval_color.at<Vec3b>(ii,jj).val[1] = 0;
						mQ_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
					}
					else if (vQuality_final[i].at<float>(ii,jj)<=0.1)
					{
						mQ_interval_color.at<Vec3b>(ii,jj).val[0] = 255;
						mQ_interval_color.at<Vec3b>(ii,jj).val[1] = 255;
						mQ_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
					}
					else
					{
						mQ_interval_color.at<Vec3b>(ii,jj).val[0] = 0;
						mQ_interval_color.at<Vec3b>(ii,jj).val[1] = 0;
						mQ_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
					}
				}


				//////////////////////////////////////////////////////////////////////////

				if (vQuality_final_G[i].at<float>(ii,jj)<0)
				{
					mQ_normed_color_G.at<Vec3b>(ii,jj).val[0] = 0;
					mQ_normed_color_G.at<Vec3b>(ii,jj).val[1] = 0;
					mQ_normed_color_G.at<Vec3b>(ii,jj).val[2] = 255;
				}
				else
				{
					int val = FTOI(255*vQuality_final_G[i].at<float>(ii,jj));
					mQ_normed_color_G.at<Vec3b>(ii,jj).val[0] = val;
					mQ_normed_color_G.at<Vec3b>(ii,jj).val[1] = val;
					mQ_normed_color_G.at<Vec3b>(ii,jj).val[2] = val;
				}
			}
		}

// 		CString str;
// 		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\depth map %d checked.bmp", i);
// 		imwrite(str.GetBuffer(), mDepth_normed_color);
// 
// 		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\hx map %d checked.bmp", i);
// 		imwrite(str.GetBuffer(), mHx_normed_color);
// 
// 		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\hy map %d checked.bmp", i);
// 		imwrite(str.GetBuffer(), mHy_normed_color);
// 
// 		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\quality map A %d checked.bmp", i);
// 		imwrite(str.GetBuffer(), mQ_normed_color);
// 
// 		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\quality map B %d checked.bmp", i);
// 		imwrite(str.GetBuffer(), mQ_interval_color);
// 
// 		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\quality map C %d checked.bmp", i);
// 		imwrite(str.GetBuffer(), mQ_normed_color_G);
// 
// 		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\cloud points %d checked.txt", i);
// 		WriteDepthMap(str, pApp->m_vCams[i], vImages[i], vDepths_final[i]);
// 
// 		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\weighted cloud points %d checked.txt", i);
// 		WriteDepthMap(str, pApp->m_vCams[i], mQ_interval_color, vDepths_final[i]);


// 		str.Format("C:\\Users\\DeepVoid\\Desktop\\cloud points %d combine checked.txt", i);
// 		WriteDepthMap(str, pApp->m_vCams[i], vImages[i], vDepths_final_combined[i]);

// 		str.Format("C:\\Users\\DeepVoid\\Desktop\\weighted cloud points %d combine checked.txt", i);
// 		WriteDepthMap(str, pApp->m_vCams[i], mQ_interval_color, vDepths_final_combined[i]);
	}

	//////////////////////////////////////////////////////////////////////////

	return TRUE;
}

void CDeepVoidApp::OnDepthcheck()
{
	// TODO: Add your command handler code here
	AfxBeginThread(DepthCheck, this, THREAD_PRIORITY_NORMAL);
}

UINT Testdata(LPVOID param)
{
	CDeepVoidApp * pApp = (CDeepVoidApp * )param;

	int i,j,ii,jj,k,kk;

	CString strInfo;

	int imgWidth, imgHeight;

	// read in all the IO and EO of test images
	int nAllImgs;
	/*FILE * file = fopen("E:\\Test Data\\templeSparseRing\\templeSR_par.txt", "r");*/
	FILE * file = fopen("E:\\Test Data\\templeRing\\templeR_par.txt", "r");
	fscanf(file, "%d", &nAllImgs);

	vector<cam_data> vAllCams;
	vector<Mat> vAllImgs;

	for (i=0;i<nAllImgs;i++)
	{
		cam_data cam;

		double tmp;
		char strtmp[48];
		fscanf(file, "%s", strtmp);

		fscanf(file, "%lf", &cam.fx);	fscanf(file, "%lf", &cam.s);	fscanf(file, "%lf", &cam.cx);
		fscanf(file, "%lf", &tmp);		fscanf(file, "%lf", &cam.fy);	fscanf(file, "%lf", &cam.cy);
		fscanf(file, "%lf", &tmp);		fscanf(file, "%lf", &tmp);		fscanf(file, "%lf", &tmp);

		for (ii=0;ii<3;ii++)
		{
			for (jj=0;jj<3;jj++)
			{
				fscanf(file, "%lf", &cam.R[ii*3+jj]);
			}
		}

		for (j=0;j<3;j++)
		{
			fscanf(file, "%lf", &cam.t[j]);
		}

		memset(cam.k, 0, 5*sizeof(double));
		cam.dist_type = 1;

		vAllCams.push_back(cam);

		CString strDir;
		/*strDir.Format("E:\\Test Data\\templeSparseRing\\templeSR%04d.png", i+1);*/
		strDir.Format("E:\\Test Data\\templeRing\\templeR%04d.png", i+1);

		Mat img = imread(strDir.GetBuffer(), CV_LOAD_IMAGE_UNCHANGED);

		imgWidth = img.cols; imgHeight = img.rows;

		vAllImgs.push_back(img); // store every image
	}

	fclose(file);

// 	double bbox_xmin = -0.073568;	double bbox_xmax = 0.028855;
// 	double bbox_ymin = 0.021728;	double bbox_ymax = 0.181892;
// 	double bbox_zmin = -0.012445;	double bbox_zmax = 0.062736;

	double bbox_xmin = -0.023121;	double bbox_xmax = 0.078626;
	double bbox_ymin = -0.038009;	double bbox_ymax = 0.121636;
	double bbox_zmin = -0.091940;	double bbox_zmax = -0.017395;

	vector<CloudPoint> cloudpts_bbox;
	CloudPoint cloudpt;
	cloudpt.m_pt.x = bbox_xmin;	cloudpt.m_pt.y = bbox_ymin;	cloudpt.m_pt.z = bbox_zmin;	cloudpts_bbox.push_back(cloudpt);
	cloudpt.m_pt.x = bbox_xmin;	cloudpt.m_pt.y = bbox_ymin;	cloudpt.m_pt.z = bbox_zmax;	cloudpts_bbox.push_back(cloudpt);
	cloudpt.m_pt.x = bbox_xmin;	cloudpt.m_pt.y = bbox_ymax;	cloudpt.m_pt.z = bbox_zmin;	cloudpts_bbox.push_back(cloudpt);
	cloudpt.m_pt.x = bbox_xmin;	cloudpt.m_pt.y = bbox_ymax;	cloudpt.m_pt.z = bbox_zmax;	cloudpts_bbox.push_back(cloudpt);

	cloudpt.m_pt.x = bbox_xmax;	cloudpt.m_pt.y = bbox_ymax;	cloudpt.m_pt.z = bbox_zmax;	cloudpts_bbox.push_back(cloudpt);
	cloudpt.m_pt.x = bbox_xmax;	cloudpt.m_pt.y = bbox_ymax;	cloudpt.m_pt.z = bbox_zmin;	cloudpts_bbox.push_back(cloudpt);
	cloudpt.m_pt.x = bbox_xmax;	cloudpt.m_pt.y = bbox_ymin;	cloudpt.m_pt.z = bbox_zmax;	cloudpts_bbox.push_back(cloudpt);
	cloudpt.m_pt.x = bbox_xmax;	cloudpt.m_pt.y = bbox_ymin;	cloudpt.m_pt.z = bbox_zmin;	cloudpts_bbox.push_back(cloudpt);


	// 20140513 PatchMatch ////////////////////////////////////////////////////////////////////////////////////////////////
// 	vector<cam_data> vCamShort(vAllCams.begin()+12, vAllCams.begin()+17);
// 	vector<Mat> vImgShort(vAllImgs.begin()+12, vAllImgs.begin()+17);
	vector<cam_data> vCamShort(vAllCams.begin(), vAllCams.begin()+5);
	vector<Mat> vImgShort(vAllImgs.begin(), vAllImgs.begin()+5);
	vAllCams = vCamShort;
	vAllImgs = vImgShort;

	for (i=0;i<vAllCams.size();i++)
	{
		CString strtmp;
		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\cam%02d.txt", i);
		//SaveCameraData(strtmp, vAllCams[i]);
	}

	// convert to gray images
	vector<Mat> vGrayImages;
	for (i=0;i<vAllImgs.size();i++)
	{
		Mat im_gray;
		cvtColor(vAllImgs[i], im_gray, CV_RGB2GRAY);
		vGrayImages.push_back(im_gray);
	}

	// parameters
	int wndSize = 5;
	double thresh_ncc = 0.6;
	double thresh_norm = 80;
	int mincam = 1;
	/*double minncc = thresh_ncc*mincam/(vAllCams.size()-1);*/
	double minncc = thresh_ncc;
	double maxncc_minncc_1 = 1/(1-minncc);
	double factor = 0.5;
	int nRandSamp = 8;
	int nPatchMatchIter = 4;
	int halfwnd = (wndSize-1)*0.5;
	double thresh_normdir = 0.1;
	double thresh_radio = 0.01;
	double ratio_2ndto1st = 0.01;

	vector<Mat> vDepths, vHxs, vHys, vDepths_final, vHxs_final, vHys_final, vQuality_final, vVisis, vVisis_diff;

	for (k=0;k<vAllCams.size();k++)
	{
		int idx_refimg = k;

		Matx33d mR0,mK0;
		Matx31d mt0;

		for (ii=0;ii<3;ii++)
		{
			for (jj=0;jj<3;jj++)
			{
				mR0(ii,jj) = vAllCams[idx_refimg].R[ii*3+jj];
			}
		}
		mK0(0,0) = vAllCams[idx_refimg].fx; mK0(0,1) = vAllCams[idx_refimg].s;  mK0(0,2) = vAllCams[idx_refimg].cx;
		mK0(1,1) = vAllCams[idx_refimg].fy; mK0(1,2) = vAllCams[idx_refimg].cy; mK0(2,2) = 1;
		mt0(0) = vAllCams[idx_refimg].t[0]; mt0(1) = vAllCams[idx_refimg].t[1]; mt0(2) = vAllCams[idx_refimg].t[2];

		vector<Mat> vRndFields(3);
		InitRndField(mK0, mR0, mt0, cloudpts_bbox, thresh_norm, imgWidth, imgHeight, vRndFields[0], vRndFields[1], vRndFields[2]);

		Mat mScores(imgHeight, imgWidth, CV_32FC1); Mat mVisi(imgHeight, imgWidth, CV_8UC1); Mat mVisiN(imgHeight, imgWidth, CV_8UC1);

		double max_depth,min_depth,max_incre_x,min_incre_x,max_incre_y,min_incre_y,max_score,min_score;

		minMaxIdx(vRndFields[0], &min_depth, &max_depth);
		minMaxIdx(vRndFields[1], &min_incre_x, &max_incre_x);
		minMaxIdx(vRndFields[2], &min_incre_y, &max_incre_y);
		//minMaxIdx(mScores, &min_score, &max_score);

		vector<Mat> vRndFields_normed(3);

		Mat mScores_normed_color(mScores.rows, mScores.cols, CV_8UC3);
		Mat mVisiN_color(mScores.rows, mScores.cols, CV_8UC3);

		vRndFields[0].convertTo(vRndFields_normed[0], CV_8UC1, 255/(max_depth-min_depth), -255*min_depth/(max_depth-min_depth));
		vRndFields[1].convertTo(vRndFields_normed[1], CV_8UC1, 255/(max_incre_x-min_incre_x), -255*min_incre_x/(max_incre_x-min_incre_x));
		vRndFields[2].convertTo(vRndFields_normed[2], CV_8UC1, 255/(max_incre_y-min_incre_y), -255*min_incre_y/(max_incre_y-min_incre_y));

		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\initial depth map ref %02d.bmp", k);
		imwrite(strInfo.GetBuffer(), vRndFields_normed[0]);
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\initial hx map ref %02d.bmp", k);
		imwrite(strInfo.GetBuffer(), vRndFields_normed[1]);
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\initial hy map ref %02d.bmp", k);
		imwrite(strInfo.GetBuffer(), vRndFields_normed[2]);

		for (i=0;i<nPatchMatchIter;i++)
		{
			/*Scan_Update_rndField_original(vAllCams, / *vGrayImages* /vAllImgs, cloudpts_bbox, thresh_norm, idx_refimg, vRndFields[0], vRndFields[1], vRndFields[2], mScores, i, mincam, factor, nRandSamp, wndSize, thresh_ncc);*/
// 			Scan_Update_rndField_original_visi(vAllCams, /*vGrayImages*/vAllImgs, cloudpts_bbox, thresh_norm, idx_refimg, vRndFields[0], vRndFields[1], vRndFields[2],
// 				mScores, mVisi, mVisiN, i, mincam, factor, nRandSamp, wndSize, thresh_ncc);
			Scan_Update_rndField_original_visi_new(vAllCams, /*vGrayImages*/vAllImgs, cloudpts_bbox, thresh_norm, idx_refimg, vRndFields[0], vRndFields[1], vRndFields[2],
				mScores, mVisi, mVisiN, i, mincam, factor, nRandSamp, wndSize, thresh_ncc, ratio_2ndto1st);
			minMaxIdx(vRndFields[0], &min_depth, &max_depth);
			minMaxIdx(vRndFields[1], &min_incre_x, &max_incre_x);
			minMaxIdx(vRndFields[2], &min_incre_y, &max_incre_y);
			minMaxIdx(mScores, &min_score, &max_score);

			vRndFields[0].convertTo(vRndFields_normed[0], CV_8UC1, 255/(max_depth-min_depth), -255*min_depth/(max_depth-min_depth));
			vRndFields[1].convertTo(vRndFields_normed[1], CV_8UC1, 255/(max_incre_x-min_incre_x), -255*min_incre_x/(max_incre_x-min_incre_x));
			vRndFields[2].convertTo(vRndFields_normed[2], CV_8UC1, 255/(max_incre_y-min_incre_y), -255*min_incre_y/(max_incre_y-min_incre_y));

			for (ii=0;ii<mScores.rows;ii++)
			{
				for (jj=0;jj<mScores.cols;jj++)
				{
					if (mScores.at<float>(ii,jj)<0)
					{
						mScores_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
						mScores_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
						mScores_normed_color.at<Vec3b>(ii,jj).val[2] = 255;
					}
					else
					{
						mScores_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
						mScores_normed_color.at<Vec3b>(ii,jj).val[1] = FTOI(255*(mScores.at<float>(ii,jj)-minncc)*maxncc_minncc_1);
						mScores_normed_color.at<Vec3b>(ii,jj).val[2] = 0;
					}

					uchar nnn = mVisiN.at<uchar>(ii,jj);

					if (nnn==0)
					{
						mVisiN_color.at<Vec3b>(ii,jj).val[0] = 0;
						mVisiN_color.at<Vec3b>(ii,jj).val[1] = 0;
						mVisiN_color.at<Vec3b>(ii,jj).val[2] = 255;
					}
					else if (nnn==1)
					{
						mVisiN_color.at<Vec3b>(ii,jj).val[0] = 0;
						mVisiN_color.at<Vec3b>(ii,jj).val[1] = 255;
						mVisiN_color.at<Vec3b>(ii,jj).val[2] = 255;
					}
					else if (nnn==2)
					{
						mVisiN_color.at<Vec3b>(ii,jj).val[0] = 255;
						mVisiN_color.at<Vec3b>(ii,jj).val[1] = 0;
						mVisiN_color.at<Vec3b>(ii,jj).val[2] = 255;
					}
					else if (nnn==3)
					{
						mVisiN_color.at<Vec3b>(ii,jj).val[0] = 0;
						mVisiN_color.at<Vec3b>(ii,jj).val[1] = 255;
						mVisiN_color.at<Vec3b>(ii,jj).val[2] = 0;
					}
					else
					{
						mVisiN_color.at<Vec3b>(ii,jj).val[0] = 255;
						mVisiN_color.at<Vec3b>(ii,jj).val[1] = 0;
						mVisiN_color.at<Vec3b>(ii,jj).val[2] = 0;
					}
				}
			}

			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\depth map iteration %02d ref %02d.bmp", i, k);
			imwrite(strInfo.GetBuffer(), vRndFields_normed[0]);
			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\hx map iteration %02d ref %02d.bmp", i, k);
			imwrite(strInfo.GetBuffer(), vRndFields_normed[1]);
			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\hy map iteration %02d ref %02d.bmp", i, k);
			imwrite(strInfo.GetBuffer(), vRndFields_normed[2]);
			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\score map iteration %02d ref %02d.bmp", i, k);
			imwrite(strInfo.GetBuffer(), mScores_normed_color);
			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\visi map iteration %02d ref %02d.bmp", i, k);
			imwrite(strInfo.GetBuffer(), mVisi);
			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\visiN map iteration %02d ref %02d.bmp", i, k);
			imwrite(strInfo.GetBuffer(), mVisiN_color);

			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\cloud points iteration %02d ref %02d.txt", i, k);
			WriteDepthMap(strInfo,vAllCams[idx_refimg],vAllImgs[idx_refimg],vRndFields[0],mScores);
		}

// 		for (i=0;i<vRndFields[0].rows;i++)
// 		{
// 			for (j=0;j<vRndFields[0].cols;j++)
// 			{
// 				double score = mScores.at<float>(i,j);
// 				if (score<0)
// 				{
// 					vRndFields[0].at<float>(i,j) = -1;
// 					vRndFields[1].at<float>(i,j) = -1;
// 					vRndFields[2].at<float>(i,j) = -1;
// 					mScores.at<float>(i,j) = -1;
// 				}
// 			}
// 		}

		// after phase 1, i.e. PatchMatch, depth map and normal maps are without -1, but there are -1 in score map denoting invalid pixels
		// output the results of the last iteration of PatchMatch
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final PatchMatch depth map ref %02d.txt", k);
		FILE * file_depth = fopen(strInfo, "w");
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final PatchMatch hx map ref %02d.txt", k);
		FILE * file_hx = fopen(strInfo, "w");
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final PatchMatch hy map ref %02d.txt", k);
		FILE * file_hy = fopen(strInfo, "w");
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final PatchMatch score map ref %02d.txt", k);
		FILE * file_score = fopen(strInfo, "w");
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final PatchMatch visi map ref %02d.txt", k);
		FILE * file_visi = fopen(strInfo, "w");

		for (i=0;i<vRndFields[0].rows;i++)
		{
			for (j=0;j<vRndFields[0].cols;j++)
			{
				fprintf(file_depth, "%.12f	", vRndFields[0].at<float>(i,j));
				fprintf(file_hx, "%.12f	", vRndFields[1].at<float>(i,j));
				fprintf(file_hy, "%.12f	", vRndFields[2].at<float>(i,j));
				fprintf(file_score, "%.12f	", mScores.at<float>(i,j));
				fprintf(file_visi, "%d	", mVisi.at<uchar>(i,j));
			}
			fprintf(file_depth, "\n");
			fprintf(file_hx, "\n");
			fprintf(file_hy, "\n");
			fprintf(file_score, "\n");
			fprintf(file_visi, "\n");
		}
		fclose(file_depth);
		fclose(file_hx);
		fclose(file_hy);
		fclose(file_score);
		fclose(file_visi);

		// phase 2, MPGC //////////////////////////////////////////////////////////////////////////
		Mat mDepth_optim(imgHeight, imgWidth, CV_32FC1, Scalar(-1));
		Mat mhx_optim(imgHeight, imgWidth, CV_32FC1, Scalar(-1));
		Mat mhy_optim(imgHeight, imgWidth, CV_32FC1, Scalar(-1));
		

		//double max_depth,min_depth,max_incre_x,min_incre_x,max_incre_y,min_incre_y,max_score,min_score;

		minMaxIdx(vRndFields[0], &min_depth, &max_depth);
		minMaxIdx(vRndFields[1], &min_incre_x, &max_incre_x);
		minMaxIdx(vRndFields[2], &min_incre_y, &max_incre_y);

		double fx_ref = vAllCams[idx_refimg].fx;
		double fx_ref_1 = 1/fx_ref;
		double fy_ref = vAllCams[idx_refimg].fy;
		double fy_ref_1 = 1/fy_ref;
		double cx_ref = vAllCams[idx_refimg].cx;
		double cy_ref = vAllCams[idx_refimg].cy;

		for (i=0;i<imgHeight;i++)
		{
			CString strInfo;
			strInfo.Format("optimize row %04d", i);
			pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

			double nimgy = (i-cy_ref)*fy_ref_1;

			for (j=0;j<imgWidth;j++)
			{
				double nimgx = (j-cx_ref)*fx_ref_1;

				double depth_init = vRndFields[0].at<float>(i,j);
				double hx_init = vRndFields[1].at<float>(i,j);
				double hy_init = vRndFields[2].at<float>(i,j);
				double score = mScores.at<float>(i,j);

				if (score<0)
				{
					mDepth_optim.at<float>(i,j) = vRndFields[0].at<float>(i,j);
					mhx_optim.at<float>(i,j) = vRndFields[1].at<float>(i,j);
					mhy_optim.at<float>(i,j) = vRndFields[2].at<float>(i,j);
					continue;
				}

				int i_real, j_real;
				MakeSureNotOutBorder(j,i,j_real,i_real,halfwnd,imgWidth,imgHeight);

// 				vector<double> vScores;
// 				CheckOnePixel_givenOneParamSet(vAllCams,vAllImgs,idx_refimg,j_real,i_real,depth_init,hx_init,hy_init,vScores,wndSize);
// 
// 				vector<int> vIdxValidCam;
// 				for (kk=0;kk<vScores.size();kk++)
// 				{
// 					if (vScores[kk]<thresh_ncc)
// 					{
// 						continue;
// 					}
// 					vIdxValidCam.push_back(kk);
// 				}

				vector<bool> vScores;
				InterpVisiVector_uchar(mVisi.at<uchar>(i,j), vScores);

				vector<int> vIdxValidCam;
				for (kk=0;kk<vAllCams.size();kk++)
				{
					if (!vScores[kk])
					{
						continue;
					}
					vIdxValidCam.push_back(kk);
				}

				double depth_optim, hx_optim, hy_optim;
				if (optim_gn_drhxhyck(vAllCams, vAllImgs, vIdxValidCam, idx_refimg, j_real, i_real, wndSize, wndSize,
					depth_init, hx_init, hy_init, depth_optim, hx_optim, hy_optim, false, 20, 1.0E-8, 1.0E-6) &&
					depth_optim>min_depth && depth_optim<max_depth
					//&& isvalid_hxhy(fx_ref,fy_ref,nimgx,nimgy,depth_optim,hx_optim,hy_optim,thresh_normdir)// hx_optim>min_incre_x && hx_optim<max_incre_x && hy_optim>min_incre_y && hy_optim<max_incre_y
					)
				{
					mDepth_optim.at<float>(i,j) = depth_optim;
					mhx_optim.at<float>(i,j) = hx_optim;
					mhy_optim.at<float>(i,j) = hy_optim;
				}
				else
				{
					mDepth_optim.at<float>(i,j) = vRndFields[0].at<float>(i,j);
					mhx_optim.at<float>(i,j) = vRndFields[1].at<float>(i,j);
					mhy_optim.at<float>(i,j) = vRndFields[2].at<float>(i,j);
					mScores.at<float>(i,j) = -1;
				}
			}
		}

		CString strInfo;
		strInfo.Format("%lf	%lf	%lf	%lf	%lf	%lf", min_depth, max_depth, min_incre_x, max_incre_x, min_incre_y, max_incre_y);
		pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

		minMaxIdx(mDepth_optim, &min_depth, &max_depth);
		minMaxIdx(mhx_optim, &min_incre_x, &max_incre_x);
		minMaxIdx(mhy_optim, &min_incre_y, &max_incre_y);
		minMaxIdx(mScores, &min_score, &max_score);
		strInfo.Format("%lf	%lf	%lf	%lf	%lf	%lf", min_depth, max_depth, min_incre_x, max_incre_x, min_incre_y, max_incre_y);
		pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

		Mat mDepth_optim_normed, mhx_optim_normed, mhy_optim_normed;
		//Mat mScores_normed_color(imgHeight, imgWidth, CV_8UC3);

		mDepth_optim.convertTo(mDepth_optim_normed, CV_8UC1, 255/(max_depth-min_depth), -255*min_depth/(max_depth-min_depth));
		mhx_optim.convertTo(mhx_optim_normed, CV_8UC1, 255/(max_incre_x-min_incre_x), -255*min_incre_x/(max_incre_x-min_incre_x));
		mhy_optim.convertTo(mhy_optim_normed, CV_8UC1, 255/(max_incre_y-min_incre_y), -255*min_incre_y/(max_incre_y-min_incre_y));

		for (ii=0;ii<imgHeight;ii++)
		{
			for (jj=0;jj<imgWidth;jj++)
			{
				if (mScores.at<float>(ii,jj)<0)
				{
					mScores_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
					mScores_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
					mScores_normed_color.at<Vec3b>(ii,jj).val[2] = 255;
				}
				else
				{
					mScores_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
					mScores_normed_color.at<Vec3b>(ii,jj).val[1] = FTOI(255*(mScores.at<float>(ii,jj)-minncc)*maxncc_minncc_1);
					mScores_normed_color.at<Vec3b>(ii,jj).val[2] = 0;
				}
			}
		}
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\depth map optimized ref %02d.bmp", k);
		imwrite(strInfo.GetBuffer(), mDepth_optim_normed);
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\hx map optimized ref %02d.bmp", k);
		imwrite(strInfo.GetBuffer(), mhx_optim_normed);
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\hy map optimized ref %02d.bmp", k);
		imwrite(strInfo.GetBuffer(), mhy_optim_normed);
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\score map optimized ref %02d.bmp", k);
		imwrite(strInfo.GetBuffer(), mScores_normed_color);

		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\cloud points optimized ref %02d.txt", k);
		WriteDepthMap(strInfo,vAllCams[idx_refimg],vAllImgs[idx_refimg],mDepth_optim,mScores);

		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final optimized depth map ref %02d.txt", k);
		file_depth = fopen(strInfo, "w");
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final optimized hx map ref %02d.txt", k);
		file_hx = fopen(strInfo, "w");
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final optimized hy map ref %02d.txt", k);
		file_hy = fopen(strInfo, "w");
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final optimized score map ref %02d.txt", k);
		file_score = fopen(strInfo, "w");

		for (i=0;i<mDepth_optim.rows;i++)
		{
			for (j=0;j<mDepth_optim.cols;j++)
			{
				double score = mScores.at<float>(i,j);
				if (score<0)
				{
					double tmp = -1;
					fprintf(file_depth, "%.12f	", tmp);
					fprintf(file_hx, "%.12f	", tmp);
					fprintf(file_hy, "%.12f	", tmp);
					fprintf(file_score, "%.12f	", tmp);

					mDepth_optim.at<float>(i,j) = -1;
					mhx_optim.at<float>(i,j) = -1;
					mhy_optim.at<float>(i,j) = -1;
					mScores.at<float>(i,j) = -1;
				} 
				else
				{
					fprintf(file_depth, "%.12f	", mDepth_optim.at<float>(i,j));
					fprintf(file_hx, "%.12f	", mhx_optim.at<float>(i,j));
					fprintf(file_hy, "%.12f	", mhy_optim.at<float>(i,j));
					fprintf(file_score, "%.12f	", mScores.at<float>(i,j));
				}
			}
			fprintf(file_depth, "\n");
			fprintf(file_hx, "\n");
			fprintf(file_hy, "\n");
			fprintf(file_score, "\n");
		}
		fclose(file_depth);
		fclose(file_hx);
		fclose(file_hy);
		fclose(file_score);

		vDepths.push_back(mDepth_optim);
		vHxs.push_back(mhx_optim);
		vHys.push_back(mhy_optim);
		vVisis.push_back(mVisi);
		///////////////////////////////////////////////////////////////////////////////////////////
	}

	// after phase 2, i.e. MPGC, depth map and normal maps have -1 themselves denoting invalid pixels, thus score map is no longer used.
	
	// phase 3, depth check //////////////////////////////////////////////////////////////////////////////
	vector<Point2d> vMinMaxD, vMinMaxHx, vMinMaxHy, vMinMaxQ; 

	// DepthConsistencyCheck(pApp->m_vCams, vImages, vDepths, vHxs, vHys, vDepths_final, vHxs_final, vHys_final, vMinMaxD, vMinMaxHx, vMinMaxHy, wndSize, thresh_ncc, thresh_radio);
	/*DepthConsistencyCheck_QualityEvaluation(vAllCams, vAllImgs, vDepths, vHxs, vHys, vDepths_final, vHxs_final, vHys_final, vQuality_final, vMinMaxD, vMinMaxHx, vMinMaxHy, vMinMaxQ, wndSize, thresh_ncc, thresh_radio);*/
	DepthConsistencyCheck_QualityEvaluation_visi(vAllCams, vAllImgs, vDepths, vHxs, vHys, vVisis, vDepths_final, vHxs_final, vHys_final, vVisis_diff, vQuality_final,
		vMinMaxD, vMinMaxHx, vMinMaxHy, vMinMaxQ, wndSize, thresh_ncc, thresh_radio);

	for (i=0;i<vAllCams.size();i++)
	{
		double mind = vMinMaxD[i].x; double maxd = vMinMaxD[i].y;
		double minhx = vMinMaxHx[i].x; double maxhx = vMinMaxHx[i].y;
		double minhy = vMinMaxHy[i].x; double maxhy = vMinMaxHy[i].y;
		double minq = vMinMaxQ[i].x; double maxq = vMinMaxQ[i].y;

		double maxd_mind_1 = 1/(maxd-mind);
		double maxhx_minhx_1 = 1/(maxhx-minhx);
		double maxhy_minhy_1 = 1/(maxhy-minhy);
		double maxq_minq_1 = 1/(maxq-minq);
		double threshratio_minq_1 = 1/(thresh_radio-minq);

		Mat mDepth_normed_color(imgHeight, imgWidth, CV_8UC3), mHx_normed_color(imgHeight, imgWidth, CV_8UC3), mHy_normed_color(imgHeight, imgWidth, CV_8UC3);
		Mat mQ_normed_color(imgHeight, imgWidth, CV_8UC3), mQ_interval_color(imgHeight, imgWidth, CV_8UC3), mVisiDiff_interval_color(imgHeight, imgWidth, CV_8UC3);

		for (ii=0;ii<imgHeight;ii++)
		{
			for (jj=0;jj<imgWidth;jj++)
			{
				//////////////////////////////////////////////////////////////////////////
				if (vDepths_final[i].at<float>(ii,jj)<0)
				{
					mDepth_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
					mDepth_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
					mDepth_normed_color.at<Vec3b>(ii,jj).val[2] = 255;
				}
				else
				{
					int val = FTOI(255*(vDepths_final[i].at<float>(ii,jj)-mind)*maxd_mind_1);
					mDepth_normed_color.at<Vec3b>(ii,jj).val[0] = val;
					mDepth_normed_color.at<Vec3b>(ii,jj).val[1] = val;
					mDepth_normed_color.at<Vec3b>(ii,jj).val[2] = val;
				}

				//////////////////////////////////////////////////////////////////////////
				/*if (vHxs_final[i].at<float>(ii,jj)<0)*/
				if (fabs(vHxs_final[i].at<float>(ii,jj)+1)<1.0E-8)
				{
					mHx_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
					mHx_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
					mHx_normed_color.at<Vec3b>(ii,jj).val[2] = 255;
				}
				else
				{
					int val = FTOI(255*(vHxs_final[i].at<float>(ii,jj)-minhx)*maxhx_minhx_1);
					mHx_normed_color.at<Vec3b>(ii,jj).val[0] = val;
					mHx_normed_color.at<Vec3b>(ii,jj).val[1] = val;
					mHx_normed_color.at<Vec3b>(ii,jj).val[2] = val;
				}

				//////////////////////////////////////////////////////////////////////////
				/*if (vHys_final[i].at<float>(ii,jj)<0)*/
				if (fabs(vHys_final[i].at<float>(ii,jj)+1)<1.0E-8)
				{
					mHy_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
					mHy_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
					mHy_normed_color.at<Vec3b>(ii,jj).val[2] = 255;
				}
				else
				{
					int val = FTOI(255*(vHys_final[i].at<float>(ii,jj)-minhy)*maxhy_minhy_1);
					mHy_normed_color.at<Vec3b>(ii,jj).val[0] = val;
					mHy_normed_color.at<Vec3b>(ii,jj).val[1] = val;
					mHy_normed_color.at<Vec3b>(ii,jj).val[2] = val;
				}

				//////////////////////////////////////////////////////////////////////////
				if (fabs(vQuality_final[i].at<float>(ii,jj)+1)<1.0E-8)
				{
					mQ_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
					mQ_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
					mQ_normed_color.at<Vec3b>(ii,jj).val[2] = 255;

					mQ_interval_color.at<Vec3b>(ii,jj).val[0] = 0;
					mQ_interval_color.at<Vec3b>(ii,jj).val[1] = 0;
					mQ_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
				}
				else
				{
					// 					if (vQuality_final[i].at<float>(ii,jj)>thresh_radio)
					// 					{
					// 						mQ_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
					// 						mQ_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
					// 						mQ_normed_color.at<Vec3b>(ii,jj).val[2] = 255;
					// 					}
					// 					else
					// 					{
					// 						int val = FTOI(255*(thresh_radio-vQuality_final[i].at<float>(ii,jj))*threshratio_minq_1);
					// 						mQ_normed_color.at<Vec3b>(ii,jj).val[0] = val;
					// 						mQ_normed_color.at<Vec3b>(ii,jj).val[1] = val;
					// 						mQ_normed_color.at<Vec3b>(ii,jj).val[2] = val;
					// 					}


					int val = FTOI(255*(maxq-vQuality_final[i].at<float>(ii,jj))*maxq_minq_1);
					mQ_normed_color.at<Vec3b>(ii,jj).val[0] = val;
					mQ_normed_color.at<Vec3b>(ii,jj).val[1] = val;
					mQ_normed_color.at<Vec3b>(ii,jj).val[2] = val;


					if (vQuality_final[i].at<float>(ii,jj)<=0.001)
					{
						mQ_interval_color.at<Vec3b>(ii,jj).val[0] = 255;
						mQ_interval_color.at<Vec3b>(ii,jj).val[1] = 0;
						mQ_interval_color.at<Vec3b>(ii,jj).val[2] = 0;
					}
					else if (vQuality_final[i].at<float>(ii,jj)<=0.005)
					{
						mQ_interval_color.at<Vec3b>(ii,jj).val[0] = 0;
						mQ_interval_color.at<Vec3b>(ii,jj).val[1] = 255;
						mQ_interval_color.at<Vec3b>(ii,jj).val[2] = 0;
					}
					else if (vQuality_final[i].at<float>(ii,jj)<=0.01)
					{
						mQ_interval_color.at<Vec3b>(ii,jj).val[0] = 0;
						mQ_interval_color.at<Vec3b>(ii,jj).val[1] = 255;
						mQ_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
					}
					else if (vQuality_final[i].at<float>(ii,jj)<=0.05)
					{
						mQ_interval_color.at<Vec3b>(ii,jj).val[0] = 255;
						mQ_interval_color.at<Vec3b>(ii,jj).val[1] = 0;
						mQ_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
					}
					else if (vQuality_final[i].at<float>(ii,jj)<=0.1)
					{
						mQ_interval_color.at<Vec3b>(ii,jj).val[0] = 255;
						mQ_interval_color.at<Vec3b>(ii,jj).val[1] = 255;
						mQ_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
					}
					else
					{
						mQ_interval_color.at<Vec3b>(ii,jj).val[0] = 0;
						mQ_interval_color.at<Vec3b>(ii,jj).val[1] = 0;
						mQ_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
					}
				}

				//////////////////////////////////////////////////////////////////////////
				if (vVisis_diff[i].at<uchar>(ii,jj)==0)
				{
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[0] = 255;
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[1] = 0;
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[2] = 0;
				}
				else if (vVisis_diff[i].at<uchar>(ii,jj)==1)
				{
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[0] = 0;
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[1] = 255;
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[2] = 0;
				}
				else if (vVisis_diff[i].at<uchar>(ii,jj)==2)
				{
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[0] = 0;
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[1] = 255;
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
				}
				else if (vVisis_diff[i].at<uchar>(ii,jj)==3)
				{
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[0] = 255;
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[1] = 0;
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
				}
				else
				{
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[0] = 0;
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[1] = 0;
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
				}
			}
		}

		CString str;
		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\depth map %d checked.bmp", i);
		imwrite(str.GetBuffer(), mDepth_normed_color);

		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\hx map %d checked.bmp", i);
		imwrite(str.GetBuffer(), mHx_normed_color);

		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\hy map %d checked.bmp", i);
		imwrite(str.GetBuffer(), mHy_normed_color);

		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\quality map A %d checked.bmp", i);
		imwrite(str.GetBuffer(), mQ_normed_color);

		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\quality map B %d checked.bmp", i);
		imwrite(str.GetBuffer(), mQ_interval_color);

		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\visidiff map %d checked.bmp", i);
		imwrite(str.GetBuffer(), mVisiDiff_interval_color);

		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\cloud points %d checked.txt", i);
		WriteDepthMap(str, vAllCams[i], vAllImgs[i], vDepths_final[i]);

		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\weighted cloud points %d checked.txt", i);
		WriteDepthMap(str, vAllCams[i], mQ_interval_color, vDepths_final[i]);
	}
	

// 	Mat im_gray;
// 	cvtColor(vAllImgs[0], im_gray, CV_RGB2GRAY);
// 
// 	uchar val33_1 = GetCensusTransform_BitString_33(im_gray, 184, 124);
// 	uchar val33_2 = GetCensusTransform_BitString_33(im_gray, 189, 124);
// 	uchar diff1 = hamdist(val33_1, val33_2);
// 
// 	UINT val55_1 = GetCensusTransform_BitString_55(im_gray, 184, 124);
// 	UINT val55_2 = GetCensusTransform_BitString_55(im_gray, 189, 124);
// 	UINT diff2 = hamdist(val55_1, val55_2);
// 
// 	uint64 val97_1 = GetCensusTransform_BitString_97(im_gray, 184, 124);
// 	uint64 val97_2 = GetCensusTransform_BitString_97(im_gray, 189, 124);
// 	uint64 diff3 = hamdist(val97_1, val97_2);

	return TRUE;
}

void CDeepVoidApp::OnTestdata()
{
	// TODO: Add your command handler code here
	AfxBeginThread(Testdata, this, THREAD_PRIORITY_NORMAL);
}

UINT PropVisi(LPVOID param)
{
	CDeepVoidApp * pApp = (CDeepVoidApp * )param;

	//////////////////////////////////////////////////////////////////////////
	int i,j,k, ii,jj,kk;

	int nCam = pApp->m_vCams.size();

	vector<Mat> vImages, vSilhouettes;
	vector<Mat> vDepths, vHxs, vHys, vScores, vDepths_final, vHxs_final, vHys_final, vQuality_final, vQuality_final_G;
	vector<Mat> vVisis, vVisis_proped;

	vector<Matx33d> vKs,vRs,vKRs;
	vector<Matx31d> vts,vKts;

	int imgWidth, imgHeight;

	// first, extract all the features in each image
	for (i=0;i<nCam;i++)
	{
		// read corresponding image data into memory first
		char * pDir = (char *)pApp->m_pMainFrame->m_wndImgThumbnailPane.m_wndImgListCtrl.GetItemData(i);

		CString strDir;
		strDir.Format(_T("%s"), pDir);
		strDir.Trim();

		Mat img = imread(strDir.GetBuffer(), CV_LOAD_IMAGE_UNCHANGED);

		imgWidth = img.cols;
		imgHeight = img.rows;

		vImages.push_back(img); // store every image

		strDir.Format("E:\\Test Data\\templeRing\\contours\\templeR%04d_seg.png", i+1);
		img = imread(strDir.GetBuffer(), CV_LOAD_IMAGE_GRAYSCALE);
		vSilhouettes.push_back(img);

		CString strtmp;
		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140515\\cam%02d.txt", i);
		ReadCameraData(strtmp, pApp->m_vCams[i]);

		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140521 morning\\final PatchMatch depth map ref %02d.txt", i);
		FILE * file_depth = fopen(strtmp, "r");
		
		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140521 morning\\final PatchMatch hx map ref %02d.txt", i);
		FILE * file_hx = fopen(strtmp, "r");
		
		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140521 morning\\final PatchMatch hy map ref %02d.txt", i);
		FILE * file_hy = fopen(strtmp, "r");

		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140521 morning\\final PatchMatch score map ref %02d.txt", i);
		FILE * file_score = fopen(strtmp, "r");

		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140521 morning\\final PatchMatch visi map ref %02d.txt", i);
		FILE * file_visi = fopen(strtmp, "r");

		Mat mDepth(imgHeight, imgWidth, CV_32FC1);
		Mat mhx(imgHeight, imgWidth, CV_32FC1);
		Mat mhy(imgHeight, imgWidth, CV_32FC1);
		Mat mScore(imgHeight, imgWidth, CV_32FC1);
		Mat mVisi(imgHeight, imgWidth, CV_8UC1);

		for (ii=0;ii<imgHeight;ii++)
		{
			for (jj=0;jj<imgWidth;jj++)
			{
				double d,hx,hy,score;
				int visi;
				fscanf(file_depth, "%lf	", &d);
				fscanf(file_hx, "%lf	", &hx);
				fscanf(file_hy, "%lf	", &hy);
				fscanf(file_score, "%lf	", &score);
				fscanf(file_visi, "%d	", &visi);

				mDepth.at<float>(ii,jj) = d;
				mhx.at<float>(ii,jj) = hx;
				mhy.at<float>(ii,jj) = hy;
				mScore.at<float>(ii,jj) = score;
				mVisi.at<uchar>(ii,jj) = (uchar)visi;
			}
		}
		fclose(file_depth);
		fclose(file_hx);
		fclose(file_hy);
		fclose(file_score);
		fclose(file_visi);

		vDepths.push_back(mDepth);
		vHxs.push_back(mhx);
		vHys.push_back(mhy);
		vScores.push_back(mScore);
		vVisis.push_back(mVisi);
		vVisis_proped.push_back(mVisi);

		cam_data cam = pApp->m_vCams[i];

		Matx33d mK, mR;
		Matx31d mt;

		for (ii=0;ii<3;ii++)
		{
			for (jj=0;jj<3;jj++)
			{
				mR(ii,jj) = cam.R[ii*3+jj];
			}
		}
		mt(0)   = cam.t[0];	mt(1)   = cam.t[1];	mt(2)   = cam.t[2];	
		mK(0,0) = cam.fx;	mK(0,1) = cam.s;	mK(0,2) = cam.cx;
		mK(1,0) = 0;		mK(1,1) = cam.fy;	mK(1,2) = cam.cy;
		mK(2,0) = 0;		mK(2,1) = 0;		mK(2,2) = 1;

		vKs.push_back(mK);
		vRs.push_back(mR);
		vts.push_back(mt);
		vKRs.push_back(mK*mR);
		vKts.push_back(mK*mt);
	}

	double bbox_xmin = -0.023121;	double bbox_xmax = 0.078626;
	double bbox_ymin = -0.038009;	double bbox_ymax = 0.121636;
	double bbox_zmin = -0.091940;	double bbox_zmax = -0.017395;

	vector<CloudPoint> cloudpts_bbox;
	CloudPoint cloudpt;
	cloudpt.m_pt.x = bbox_xmin;	cloudpt.m_pt.y = bbox_ymin;	cloudpt.m_pt.z = bbox_zmin;	cloudpts_bbox.push_back(cloudpt);
	cloudpt.m_pt.x = bbox_xmin;	cloudpt.m_pt.y = bbox_ymin;	cloudpt.m_pt.z = bbox_zmax;	cloudpts_bbox.push_back(cloudpt);
	cloudpt.m_pt.x = bbox_xmin;	cloudpt.m_pt.y = bbox_ymax;	cloudpt.m_pt.z = bbox_zmin;	cloudpts_bbox.push_back(cloudpt);
	cloudpt.m_pt.x = bbox_xmin;	cloudpt.m_pt.y = bbox_ymax;	cloudpt.m_pt.z = bbox_zmax;	cloudpts_bbox.push_back(cloudpt);

	cloudpt.m_pt.x = bbox_xmax;	cloudpt.m_pt.y = bbox_ymax;	cloudpt.m_pt.z = bbox_zmax;	cloudpts_bbox.push_back(cloudpt);
	cloudpt.m_pt.x = bbox_xmax;	cloudpt.m_pt.y = bbox_ymax;	cloudpt.m_pt.z = bbox_zmin;	cloudpts_bbox.push_back(cloudpt);
	cloudpt.m_pt.x = bbox_xmax;	cloudpt.m_pt.y = bbox_ymin;	cloudpt.m_pt.z = bbox_zmax;	cloudpts_bbox.push_back(cloudpt);
	cloudpt.m_pt.x = bbox_xmax;	cloudpt.m_pt.y = bbox_ymin;	cloudpt.m_pt.z = bbox_zmin;	cloudpts_bbox.push_back(cloudpt);

	for (i=0;i<nCam;i++)
	{
		for (ii=0;ii<imgHeight;ii++)
		{
			CString strInfo;
			strInfo.Format("propagate row %04d of image %02d", ii, i);
			pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

			for (jj=0;jj<imgWidth;jj++)
			{
				double d0 = vDepths[i].at<float>(ii,jj);
				double hx0 = vHxs[i].at<float>(ii,jj);
				double hy0 = vHys[i].at<float>(ii,jj);
				double score = vScores[i].at<float>(ii,jj);

				if (score<0)
				{
					continue;
				}

				uchar visi_i = vVisis[i].at<uchar>(ii,jj);

				vector<bool> vbools_i;
				InterpVisiVector_uchar(visi_i, vbools_i);

				for (j=0;j<nCam;j++)
				{
					if (!vbools_i[j])
					{
						continue;
					}

					// compute corresponding XYZ
					Matx31d pt3d = GetXYZ_givenDepth(vKs[i], vRs[i], vts[i], jj, ii, d0);

					pt3d = vRs[j]*pt3d+vts[j];

					Matx31d imgpt = vKs[j]*pt3d;
					double zk_1 = 1/imgpt(2);
					double xk = imgpt(0)*zk_1;
					double yk = imgpt(1)*zk_1;

					if (xk < 0 || xk > imgWidth-1 || yk < 0 || yk > imgHeight-1)
					{
						continue;
					}

					int x_floor = floor(xk);	int y_floor = floor(yk);	
					int x_ceil = ceil(xk);		int y_ceil = ceil(yk);

					uchar visi_j_tl = vVisis[j].at<uchar>(y_floor,x_floor);
					uchar visi_j_tr = vVisis[j].at<uchar>(y_floor,x_ceil);
					uchar visi_j_bl = vVisis[j].at<uchar>(y_ceil,x_floor);
					uchar visi_j_br = vVisis[j].at<uchar>(y_ceil,x_ceil);
					vector<bool> vbools_j_tl, vbools_j_tr, vbools_j_bl, vbools_j_br;
					InterpVisiVector_uchar(visi_j_tl, vbools_j_tl);
					InterpVisiVector_uchar(visi_j_tr, vbools_j_tr);
					InterpVisiVector_uchar(visi_j_bl, vbools_j_bl);
					InterpVisiVector_uchar(visi_j_br, vbools_j_br);

					vbools_j_tl[i]=true;
					vbools_j_tr[i]=true;
					vbools_j_bl[i]=true;
					vbools_j_br[i]=true;

					visi_j_tl = GetVisibilityVector_uchar(vbools_j_tl);
					visi_j_tr = GetVisibilityVector_uchar(vbools_j_tr);
					visi_j_bl = GetVisibilityVector_uchar(vbools_j_bl);
					visi_j_br = GetVisibilityVector_uchar(vbools_j_br);
					vVisis_proped[j].at<uchar>(y_floor,x_floor) = visi_j_tl;
					vVisis_proped[j].at<uchar>(y_floor,x_ceil)  = visi_j_tr;
					vVisis_proped[j].at<uchar>(y_ceil,x_floor)  = visi_j_bl;
					vVisis_proped[j].at<uchar>(y_ceil,x_ceil)   = visi_j_br;
				}
			}
		}
	}

	for (i=0;i<nCam;i++)
	{
		Mat mVisiN_proped_color(imgHeight, imgWidth, CV_8UC3);

		CString strtmp;
		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\visi map propagated ref %02d.txt", i);
		FILE * file = fopen(strtmp, "w");

		for (ii=0;ii<imgHeight;ii++)
		{
			for (jj=0;jj<imgWidth;jj++)
			{
				uchar visi = vVisis_proped[i].at<uchar>(ii,jj);

				vector<bool> vbools; int nnn;
				InterpVisiVector_uchar(visi, vbools, &nnn);

				if (nnn==0)
				{
					mVisiN_proped_color.at<Vec3b>(ii,jj).val[0] = 0;
					mVisiN_proped_color.at<Vec3b>(ii,jj).val[1] = 0;
					mVisiN_proped_color.at<Vec3b>(ii,jj).val[2] = 255;
				}
				else if (nnn==1)
				{
					mVisiN_proped_color.at<Vec3b>(ii,jj).val[0] = 0;
					mVisiN_proped_color.at<Vec3b>(ii,jj).val[1] = 255;
					mVisiN_proped_color.at<Vec3b>(ii,jj).val[2] = 255;
				}
				else if (nnn==2)
				{
					mVisiN_proped_color.at<Vec3b>(ii,jj).val[0] = 255;
					mVisiN_proped_color.at<Vec3b>(ii,jj).val[1] = 0;
					mVisiN_proped_color.at<Vec3b>(ii,jj).val[2] = 255;
				}
				else if (nnn==3)
				{
					mVisiN_proped_color.at<Vec3b>(ii,jj).val[0] = 0;
					mVisiN_proped_color.at<Vec3b>(ii,jj).val[1] = 255;
					mVisiN_proped_color.at<Vec3b>(ii,jj).val[2] = 0;
				}
				else
				{
					mVisiN_proped_color.at<Vec3b>(ii,jj).val[0] = 255;
					mVisiN_proped_color.at<Vec3b>(ii,jj).val[1] = 0;
					mVisiN_proped_color.at<Vec3b>(ii,jj).val[2] = 0;
				}

				fprintf(file, "%d	", visi);
			}
			fprintf(file, "\n");
		}
		fclose(file);

		CString strInfo;
		
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\visi map propagated ref %02d.bmp", i);
		imwrite(strInfo.GetBuffer(), vVisis_proped[i]);
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\visiN map propagated ref %02d.bmp", i);
		imwrite(strInfo.GetBuffer(), mVisiN_proped_color);
	}

	// parameters
// 	int wndSize = 5;
// 	double thresh_ncc = 0.6;
// 	double thresh_norm = 80;
// 	int mincam = 1;
// 	/*double minncc = thresh_ncc*mincam/(vAllCams.size()-1);*/
// 	double minncc = thresh_ncc;
// 	double maxncc_minncc_1 = 1/(1-minncc);
// 	double factor = 0.5;
// 	int nRandSamp = 8;
// 	int nPatchMatchIter = 4;
// 	int halfwnd = (wndSize-1)*0.5;
// 	double thresh_normdir = 0.1;
// 	double thresh_radio = 0.01;
// 	double ratio_2ndto1st = 0.01;
// 
// 	/*vector<Mat> vDepths, vHxs, vHys, vDepths_final, vHxs_final, vHys_final, vQuality_final, vVisis, vVisis_diff;*/
// 
// 	for (k=0;k<pApp->m_vCams.size();k++)
// 	{
// 		CString strInfo;
// 
// 		int idx_refimg = k;
// 
// 		Matx33d mR0 = vRs[idx_refimg];
// 		Matx33d	mK0 = vKs[idx_refimg];
// 		Matx31d mt0 = vts[idx_refimg];
// 
// 		Mat mScores(imgHeight, imgWidth, CV_32FC1); Mat mVisi(imgHeight, imgWidth, CV_8UC1); Mat mVisiN(imgHeight, imgWidth, CV_8UC1);
// 
// 		double max_depth,min_depth,max_incre_x,min_incre_x,max_incre_y,min_incre_y,max_score,min_score;
// 
// 		minMaxIdx(vDepths[idx_refimg], &min_depth, &max_depth);
// 		minMaxIdx(vHxs[idx_refimg], &min_incre_x, &max_incre_x);
// 		minMaxIdx(vHys[idx_refimg], &min_incre_y, &max_incre_y);
// 		//minMaxIdx(mScores, &min_score, &max_score);
// 
// 		vector<Mat> vRndFields_normed(3);
// 
// 		Mat mScores_normed_color(mScores.rows, mScores.cols, CV_8UC3);
// 
// 		vDepths[idx_refimg].convertTo(vRndFields_normed[0], CV_8UC1, 255/(max_depth-min_depth), -255*min_depth/(max_depth-min_depth));
// 		vHxs[idx_refimg].convertTo(vRndFields_normed[1], CV_8UC1, 255/(max_incre_x-min_incre_x), -255*min_incre_x/(max_incre_x-min_incre_x));
// 		vHys[idx_refimg].convertTo(vRndFields_normed[2], CV_8UC1, 255/(max_incre_y-min_incre_y), -255*min_incre_y/(max_incre_y-min_incre_y));
// 
// 		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\initial depth map ref %02d.bmp", k);
// 		imwrite(strInfo.GetBuffer(), vRndFields_normed[0]);
// 		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\initial hx map ref %02d.bmp", k);
// 		imwrite(strInfo.GetBuffer(), vRndFields_normed[1]);
// 		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\initial hy map ref %02d.bmp", k);
// 		imwrite(strInfo.GetBuffer(), vRndFields_normed[2]);
// 
// 		for (i=0;i<nPatchMatchIter;i++)
// 		{
// 			Scan_Update_rndField_original_fixedvisi(pApp->m_vCams, vImages, cloudpts_bbox, thresh_norm, idx_refimg, vDepths[idx_refimg], vHxs[idx_refimg], vHys[idx_refimg], vScores[idx_refimg], 
// 				vVisis_proped[idx_refimg], i, mincam, factor, nRandSamp, wndSize);
// 
// 			minMaxIdx(vDepths[idx_refimg], &min_depth, &max_depth);
// 			minMaxIdx(vHxs[idx_refimg], &min_incre_x, &max_incre_x);
// 			minMaxIdx(vHys[idx_refimg], &min_incre_y, &max_incre_y);
// 			minMaxIdx(vScores[idx_refimg], &min_score, &max_score);
// 
// 			vDepths[idx_refimg].convertTo(vRndFields_normed[0], CV_8UC1, 255/(max_depth-min_depth), -255*min_depth/(max_depth-min_depth));
// 			vHxs[idx_refimg].convertTo(vRndFields_normed[1], CV_8UC1, 255/(max_incre_x-min_incre_x), -255*min_incre_x/(max_incre_x-min_incre_x));
// 			vHys[idx_refimg].convertTo(vRndFields_normed[2], CV_8UC1, 255/(max_incre_y-min_incre_y), -255*min_incre_y/(max_incre_y-min_incre_y));
// 
// 			for (ii=0;ii<mScores.rows;ii++)
// 			{
// 				for (jj=0;jj<mScores.cols;jj++)
// 				{
// 					if (mScores.at<float>(ii,jj)<0)
// 					{
// 						mScores_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
// 						mScores_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
// 						mScores_normed_color.at<Vec3b>(ii,jj).val[2] = 255;
// 					}
// 					else
// 					{
// 						mScores_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
// 						mScores_normed_color.at<Vec3b>(ii,jj).val[1] = FTOI(255*(mScores.at<float>(ii,jj)-minncc)*maxncc_minncc_1);
// 						mScores_normed_color.at<Vec3b>(ii,jj).val[2] = 0;
// 					}
// 				}
// 			}
// 
// 			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\depth map iteration %02d ref %02d.bmp", i, k);
// 			imwrite(strInfo.GetBuffer(), vRndFields_normed[0]);
// 			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\hx map iteration %02d ref %02d.bmp", i, k);
// 			imwrite(strInfo.GetBuffer(), vRndFields_normed[1]);
// 			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\hy map iteration %02d ref %02d.bmp", i, k);
// 			imwrite(strInfo.GetBuffer(), vRndFields_normed[2]);
// 			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\score map iteration %02d ref %02d.bmp", i, k);
// 			imwrite(strInfo.GetBuffer(), mScores_normed_color);
// 
// 			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\cloud points iteration %02d ref %02d.txt", i, k);
// 			WriteDepthMap(strInfo,pApp->m_vCams[idx_refimg],vImages[idx_refimg],vDepths[idx_refimg],vScores[idx_refimg]);
// 		}
// 
// 		// after phase 1, i.e. PatchMatch, depth map and normal maps are without -1, but there are -1 in score map denoting invalid pixels
// 		// output the results of the last iteration of PatchMatch
// 		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final PatchMatch depth map ref %02d.txt", k);
// 		FILE * file_depth = fopen(strInfo, "w");
// 		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final PatchMatch hx map ref %02d.txt", k);
// 		FILE * file_hx = fopen(strInfo, "w");
// 		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final PatchMatch hy map ref %02d.txt", k);
// 		FILE * file_hy = fopen(strInfo, "w");
// 		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final PatchMatch score map ref %02d.txt", k);
// 		FILE * file_score = fopen(strInfo, "w");
// 
// 		for (i=0;i<vDepths[idx_refimg].rows;i++)
// 		{
// 			for (j=0;j<vDepths[idx_refimg].cols;j++)
// 			{
// 				fprintf(file_depth, "%.12f	", vDepths[idx_refimg].at<float>(i,j));
// 				fprintf(file_hx, "%.12f	", vHxs[idx_refimg].at<float>(i,j));
// 				fprintf(file_hy, "%.12f	", vHys[idx_refimg].at<float>(i,j));
// 				fprintf(file_score, "%.12f	", vScores[idx_refimg].at<float>(i,j));
// 			}
// 			fprintf(file_depth, "\n");
// 			fprintf(file_hx, "\n");
// 			fprintf(file_hy, "\n");
// 			fprintf(file_score, "\n");
// 		}
// 		fclose(file_depth);
// 		fclose(file_hx);
// 		fclose(file_hy);
// 		fclose(file_score);

		// phase 2, MPGC //////////////////////////////////////////////////////////////////////////
// 		Mat mDepth_optim(imgHeight, imgWidth, CV_32FC1, Scalar(-1));
// 		Mat mhx_optim(imgHeight, imgWidth, CV_32FC1, Scalar(-1));
// 		Mat mhy_optim(imgHeight, imgWidth, CV_32FC1, Scalar(-1));
// 
// 		//double max_depth,min_depth,max_incre_x,min_incre_x,max_incre_y,min_incre_y,max_score,min_score;
// 		minMaxIdx(vDepths[idx_refimg], &min_depth, &max_depth);
// 		minMaxIdx(vHxs[idx_refimg], &min_incre_x, &max_incre_x);
// 		minMaxIdx(vHys[idx_refimg], &min_incre_y, &max_incre_y);
// 
// 		double fx_ref = pApp->m_vCams[idx_refimg].fx;
// 		double fx_ref_1 = 1/fx_ref;
// 		double fy_ref = pApp->m_vCams[idx_refimg].fy;
// 		double fy_ref_1 = 1/fy_ref;
// 		double cx_ref = pApp->m_vCams[idx_refimg].cx;
// 		double cy_ref = pApp->m_vCams[idx_refimg].cy;
// 
// 		for (i=0;i<imgHeight;i++)
// 		{
// 			CString strInfo;
// 			strInfo.Format("optimize row %04d", i);
// 			pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 
// 			double nimgy = (i-cy_ref)*fy_ref_1;
// 
// 			for (j=0;j<imgWidth;j++)
// 			{
// 				double nimgx = (j-cx_ref)*fx_ref_1;
// 
// 				double depth_init = vDepths[idx_refimg].at<float>(i,j);
// 				double hx_init = vHxs[idx_refimg].at<float>(i,j);
// 				double hy_init = vHys[idx_refimg].at<float>(i,j);
// 				double score = vScores[idx_refimg].at<float>(i,j);
// 
// 				if (score<0)
// 				{
// 					mDepth_optim.at<float>(i,j) = vDepths[idx_refimg].at<float>(i,j);
// 					mhx_optim.at<float>(i,j) = vHxs[idx_refimg].at<float>(i,j);
// 					mhy_optim.at<float>(i,j) = vHys[idx_refimg].at<float>(i,j);
// 					continue;
// 				}
// 
// 				int i_real, j_real;
// 				MakeSureNotOutBorder(j,i,j_real,i_real,halfwnd,imgWidth,imgHeight);
// 
// 				vector<bool> vScores;
// 				InterpVisiVector_uchar(vVisis_proped[idx_refimg].at<uchar>(i,j), vScores);
// 
// 				vector<int> vIdxValidCam;
// 				for (kk=0;kk<pApp->m_vCams.size();kk++)
// 				{
// 					if (!vScores[kk])
// 					{
// 						continue;
// 					}
// 					vIdxValidCam.push_back(kk);
// 				}
// 
// 				double depth_optim, hx_optim, hy_optim;
// 				if (optim_gn_drhxhyck(vAllCams, vAllImgs, vIdxValidCam, idx_refimg, j_real, i_real, wndSize, wndSize,
// 					depth_init, hx_init, hy_init, depth_optim, hx_optim, hy_optim, false, 20, 1.0E-8, 1.0E-6) &&
// 					depth_optim>min_depth && depth_optim<max_depth
// 					//&& isvalid_hxhy(fx_ref,fy_ref,nimgx,nimgy,depth_optim,hx_optim,hy_optim,thresh_normdir)// hx_optim>min_incre_x && hx_optim<max_incre_x && hy_optim>min_incre_y && hy_optim<max_incre_y
// 					)
// 				{
// 					mDepth_optim.at<float>(i,j) = depth_optim;
// 					mhx_optim.at<float>(i,j) = hx_optim;
// 					mhy_optim.at<float>(i,j) = hy_optim;
// 				}
// 				else
// 				{
// 					mDepth_optim.at<float>(i,j) = vRndFields[0].at<float>(i,j);
// 					mhx_optim.at<float>(i,j) = vRndFields[1].at<float>(i,j);
// 					mhy_optim.at<float>(i,j) = vRndFields[2].at<float>(i,j);
// 					mScores.at<float>(i,j) = -1;
// 				}
// 			}
// 		}
// 
// 		CString strInfo;
// 		strInfo.Format("%lf	%lf	%lf	%lf	%lf	%lf", min_depth, max_depth, min_incre_x, max_incre_x, min_incre_y, max_incre_y);
// 		pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 
// 		minMaxIdx(mDepth_optim, &min_depth, &max_depth);
// 		minMaxIdx(mhx_optim, &min_incre_x, &max_incre_x);
// 		minMaxIdx(mhy_optim, &min_incre_y, &max_incre_y);
// 		minMaxIdx(mScores, &min_score, &max_score);
// 		strInfo.Format("%lf	%lf	%lf	%lf	%lf	%lf", min_depth, max_depth, min_incre_x, max_incre_x, min_incre_y, max_incre_y);
// 		pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 
// 		Mat mDepth_optim_normed, mhx_optim_normed, mhy_optim_normed;
// 		//Mat mScores_normed_color(imgHeight, imgWidth, CV_8UC3);
// 
// 		mDepth_optim.convertTo(mDepth_optim_normed, CV_8UC1, 255/(max_depth-min_depth), -255*min_depth/(max_depth-min_depth));
// 		mhx_optim.convertTo(mhx_optim_normed, CV_8UC1, 255/(max_incre_x-min_incre_x), -255*min_incre_x/(max_incre_x-min_incre_x));
// 		mhy_optim.convertTo(mhy_optim_normed, CV_8UC1, 255/(max_incre_y-min_incre_y), -255*min_incre_y/(max_incre_y-min_incre_y));
// 
// 		for (ii=0;ii<imgHeight;ii++)
// 		{
// 			for (jj=0;jj<imgWidth;jj++)
// 			{
// 				if (mScores.at<float>(ii,jj)<0)
// 				{
// 					mScores_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
// 					mScores_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
// 					mScores_normed_color.at<Vec3b>(ii,jj).val[2] = 255;
// 				}
// 				else
// 				{
// 					mScores_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
// 					mScores_normed_color.at<Vec3b>(ii,jj).val[1] = FTOI(255*(mScores.at<float>(ii,jj)-minncc)*maxncc_minncc_1);
// 					mScores_normed_color.at<Vec3b>(ii,jj).val[2] = 0;
// 				}
// 			}
// 		}
// 		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\depth map optimized ref %02d.bmp", k);
// 		imwrite(strInfo.GetBuffer(), mDepth_optim_normed);
// 		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\hx map optimized ref %02d.bmp", k);
// 		imwrite(strInfo.GetBuffer(), mhx_optim_normed);
// 		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\hy map optimized ref %02d.bmp", k);
// 		imwrite(strInfo.GetBuffer(), mhy_optim_normed);
// 		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\score map optimized ref %02d.bmp", k);
// 		imwrite(strInfo.GetBuffer(), mScores_normed_color);
// 
// 		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\cloud points optimized ref %02d.txt", k);
// 		WriteDepthMap(strInfo,vAllCams[idx_refimg],vAllImgs[idx_refimg],mDepth_optim,mScores);
// 
// 		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final optimized depth map ref %02d.txt", k);
// 		file_depth = fopen(strInfo, "w");
// 		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final optimized hx map ref %02d.txt", k);
// 		file_hx = fopen(strInfo, "w");
// 		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final optimized hy map ref %02d.txt", k);
// 		file_hy = fopen(strInfo, "w");
// 		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final optimized score map ref %02d.txt", k);
// 		file_score = fopen(strInfo, "w");
// 
// 		for (i=0;i<mDepth_optim.rows;i++)
// 		{
// 			for (j=0;j<mDepth_optim.cols;j++)
// 			{
// 				double score = mScores.at<float>(i,j);
// 				if (score<0)
// 				{
// 					double tmp = -1;
// 					fprintf(file_depth, "%.12f	", tmp);
// 					fprintf(file_hx, "%.12f	", tmp);
// 					fprintf(file_hy, "%.12f	", tmp);
// 					fprintf(file_score, "%.12f	", tmp);
// 
// 					mDepth_optim.at<float>(i,j) = -1;
// 					mhx_optim.at<float>(i,j) = -1;
// 					mhy_optim.at<float>(i,j) = -1;
// 					mScores.at<float>(i,j) = -1;
// 				} 
// 				else
// 				{
// 					fprintf(file_depth, "%.12f	", mDepth_optim.at<float>(i,j));
// 					fprintf(file_hx, "%.12f	", mhx_optim.at<float>(i,j));
// 					fprintf(file_hy, "%.12f	", mhy_optim.at<float>(i,j));
// 					fprintf(file_score, "%.12f	", mScores.at<float>(i,j));
// 				}
// 			}
// 			fprintf(file_depth, "\n");
// 			fprintf(file_hx, "\n");
// 			fprintf(file_hy, "\n");
// 			fprintf(file_score, "\n");
// 		}
// 		fclose(file_depth);
// 		fclose(file_hx);
// 		fclose(file_hy);
// 		fclose(file_score);
// 
// 		vDepths.push_back(mDepth_optim);
// 		vHxs.push_back(mhx_optim);
// 		vHys.push_back(mhy_optim);
// 		vVisis.push_back(mVisi);
//		///////////////////////////////////////////////////////////////////////////////////////////
//	}

	// after phase 2, i.e. MPGC, depth map and normal maps have -1 themselves denoting invalid pixels, thus score map is no longer used.

	// phase 3, depth check //////////////////////////////////////////////////////////////////////////////
// 	vector<Point2d> vMinMaxD, vMinMaxHx, vMinMaxHy, vMinMaxQ; 
// 
// 	// DepthConsistencyCheck(pApp->m_vCams, vImages, vDepths, vHxs, vHys, vDepths_final, vHxs_final, vHys_final, vMinMaxD, vMinMaxHx, vMinMaxHy, wndSize, thresh_ncc, thresh_radio);
// 	/*DepthConsistencyCheck_QualityEvaluation(vAllCams, vAllImgs, vDepths, vHxs, vHys, vDepths_final, vHxs_final, vHys_final, vQuality_final, vMinMaxD, vMinMaxHx, vMinMaxHy, vMinMaxQ, wndSize, thresh_ncc, thresh_radio);*/
// 	DepthConsistencyCheck_QualityEvaluation_visi(vAllCams, vAllImgs, vDepths, vHxs, vHys, vVisis, vDepths_final, vHxs_final, vHys_final, vVisis_diff, vQuality_final,
// 		vMinMaxD, vMinMaxHx, vMinMaxHy, vMinMaxQ, wndSize, thresh_ncc, thresh_radio);
// 
// 	for (i=0;i<vAllCams.size();i++)
// 	{
// 		double mind = vMinMaxD[i].x; double maxd = vMinMaxD[i].y;
// 		double minhx = vMinMaxHx[i].x; double maxhx = vMinMaxHx[i].y;
// 		double minhy = vMinMaxHy[i].x; double maxhy = vMinMaxHy[i].y;
// 		double minq = vMinMaxQ[i].x; double maxq = vMinMaxQ[i].y;
// 
// 		double maxd_mind_1 = 1/(maxd-mind);
// 		double maxhx_minhx_1 = 1/(maxhx-minhx);
// 		double maxhy_minhy_1 = 1/(maxhy-minhy);
// 		double maxq_minq_1 = 1/(maxq-minq);
// 		double threshratio_minq_1 = 1/(thresh_radio-minq);
// 
// 		Mat mDepth_normed_color(imgHeight, imgWidth, CV_8UC3), mHx_normed_color(imgHeight, imgWidth, CV_8UC3), mHy_normed_color(imgHeight, imgWidth, CV_8UC3);
// 		Mat mQ_normed_color(imgHeight, imgWidth, CV_8UC3), mQ_interval_color(imgHeight, imgWidth, CV_8UC3), mVisiDiff_interval_color(imgHeight, imgWidth, CV_8UC3);
// 
// 		for (ii=0;ii<imgHeight;ii++)
// 		{
// 			for (jj=0;jj<imgWidth;jj++)
// 			{
// 				//////////////////////////////////////////////////////////////////////////
// 				if (vDepths_final[i].at<float>(ii,jj)<0)
// 				{
// 					mDepth_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
// 					mDepth_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
// 					mDepth_normed_color.at<Vec3b>(ii,jj).val[2] = 255;
// 				}
// 				else
// 				{
// 					int val = FTOI(255*(vDepths_final[i].at<float>(ii,jj)-mind)*maxd_mind_1);
// 					mDepth_normed_color.at<Vec3b>(ii,jj).val[0] = val;
// 					mDepth_normed_color.at<Vec3b>(ii,jj).val[1] = val;
// 					mDepth_normed_color.at<Vec3b>(ii,jj).val[2] = val;
// 				}
// 
// 				//////////////////////////////////////////////////////////////////////////
// 				/*if (vHxs_final[i].at<float>(ii,jj)<0)*/
// 				if (fabs(vHxs_final[i].at<float>(ii,jj)+1)<1.0E-8)
// 				{
// 					mHx_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
// 					mHx_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
// 					mHx_normed_color.at<Vec3b>(ii,jj).val[2] = 255;
// 				}
// 				else
// 				{
// 					int val = FTOI(255*(vHxs_final[i].at<float>(ii,jj)-minhx)*maxhx_minhx_1);
// 					mHx_normed_color.at<Vec3b>(ii,jj).val[0] = val;
// 					mHx_normed_color.at<Vec3b>(ii,jj).val[1] = val;
// 					mHx_normed_color.at<Vec3b>(ii,jj).val[2] = val;
// 				}
// 
// 				//////////////////////////////////////////////////////////////////////////
// 				/*if (vHys_final[i].at<float>(ii,jj)<0)*/
// 				if (fabs(vHys_final[i].at<float>(ii,jj)+1)<1.0E-8)
// 				{
// 					mHy_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
// 					mHy_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
// 					mHy_normed_color.at<Vec3b>(ii,jj).val[2] = 255;
// 				}
// 				else
// 				{
// 					int val = FTOI(255*(vHys_final[i].at<float>(ii,jj)-minhy)*maxhy_minhy_1);
// 					mHy_normed_color.at<Vec3b>(ii,jj).val[0] = val;
// 					mHy_normed_color.at<Vec3b>(ii,jj).val[1] = val;
// 					mHy_normed_color.at<Vec3b>(ii,jj).val[2] = val;
// 				}
// 
// 				//////////////////////////////////////////////////////////////////////////
// 				if (fabs(vQuality_final[i].at<float>(ii,jj)+1)<1.0E-8)
// 				{
// 					mQ_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
// 					mQ_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
// 					mQ_normed_color.at<Vec3b>(ii,jj).val[2] = 255;
// 
// 					mQ_interval_color.at<Vec3b>(ii,jj).val[0] = 0;
// 					mQ_interval_color.at<Vec3b>(ii,jj).val[1] = 0;
// 					mQ_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
// 				}
// 				else
// 				{
// 					// 					if (vQuality_final[i].at<float>(ii,jj)>thresh_radio)
// 					// 					{
// 					// 						mQ_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
// 					// 						mQ_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
// 					// 						mQ_normed_color.at<Vec3b>(ii,jj).val[2] = 255;
// 					// 					}
// 					// 					else
// 					// 					{
// 					// 						int val = FTOI(255*(thresh_radio-vQuality_final[i].at<float>(ii,jj))*threshratio_minq_1);
// 					// 						mQ_normed_color.at<Vec3b>(ii,jj).val[0] = val;
// 					// 						mQ_normed_color.at<Vec3b>(ii,jj).val[1] = val;
// 					// 						mQ_normed_color.at<Vec3b>(ii,jj).val[2] = val;
// 					// 					}
// 
// 
// 					int val = FTOI(255*(maxq-vQuality_final[i].at<float>(ii,jj))*maxq_minq_1);
// 					mQ_normed_color.at<Vec3b>(ii,jj).val[0] = val;
// 					mQ_normed_color.at<Vec3b>(ii,jj).val[1] = val;
// 					mQ_normed_color.at<Vec3b>(ii,jj).val[2] = val;
// 
// 
// 					if (vQuality_final[i].at<float>(ii,jj)<=0.001)
// 					{
// 						mQ_interval_color.at<Vec3b>(ii,jj).val[0] = 255;
// 						mQ_interval_color.at<Vec3b>(ii,jj).val[1] = 0;
// 						mQ_interval_color.at<Vec3b>(ii,jj).val[2] = 0;
// 					}
// 					else if (vQuality_final[i].at<float>(ii,jj)<=0.005)
// 					{
// 						mQ_interval_color.at<Vec3b>(ii,jj).val[0] = 0;
// 						mQ_interval_color.at<Vec3b>(ii,jj).val[1] = 255;
// 						mQ_interval_color.at<Vec3b>(ii,jj).val[2] = 0;
// 					}
// 					else if (vQuality_final[i].at<float>(ii,jj)<=0.01)
// 					{
// 						mQ_interval_color.at<Vec3b>(ii,jj).val[0] = 0;
// 						mQ_interval_color.at<Vec3b>(ii,jj).val[1] = 255;
// 						mQ_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
// 					}
// 					else if (vQuality_final[i].at<float>(ii,jj)<=0.05)
// 					{
// 						mQ_interval_color.at<Vec3b>(ii,jj).val[0] = 255;
// 						mQ_interval_color.at<Vec3b>(ii,jj).val[1] = 0;
// 						mQ_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
// 					}
// 					else if (vQuality_final[i].at<float>(ii,jj)<=0.1)
// 					{
// 						mQ_interval_color.at<Vec3b>(ii,jj).val[0] = 255;
// 						mQ_interval_color.at<Vec3b>(ii,jj).val[1] = 255;
// 						mQ_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
// 					}
// 					else
// 					{
// 						mQ_interval_color.at<Vec3b>(ii,jj).val[0] = 0;
// 						mQ_interval_color.at<Vec3b>(ii,jj).val[1] = 0;
// 						mQ_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
// 					}
// 				}
// 
// 				//////////////////////////////////////////////////////////////////////////
// 				if (vVisis_diff[i].at<uchar>(ii,jj)==0)
// 				{
// 					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[0] = 255;
// 					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[1] = 0;
// 					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[2] = 0;
// 				}
// 				else if (vVisis_diff[i].at<uchar>(ii,jj)==1)
// 				{
// 					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[0] = 0;
// 					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[1] = 255;
// 					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[2] = 0;
// 				}
// 				else if (vVisis_diff[i].at<uchar>(ii,jj)==2)
// 				{
// 					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[0] = 0;
// 					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[1] = 255;
// 					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
// 				}
// 				else if (vVisis_diff[i].at<uchar>(ii,jj)==3)
// 				{
// 					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[0] = 255;
// 					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[1] = 0;
// 					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
// 				}
// 				else
// 				{
// 					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[0] = 0;
// 					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[1] = 0;
// 					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
// 				}
// 			}
// 		}
// 
// 		CString str;
// 		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\depth map %d checked.bmp", i);
// 		imwrite(str.GetBuffer(), mDepth_normed_color);
// 
// 		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\hx map %d checked.bmp", i);
// 		imwrite(str.GetBuffer(), mHx_normed_color);
// 
// 		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\hy map %d checked.bmp", i);
// 		imwrite(str.GetBuffer(), mHy_normed_color);
// 
// 		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\quality map A %d checked.bmp", i);
// 		imwrite(str.GetBuffer(), mQ_normed_color);
// 
// 		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\quality map B %d checked.bmp", i);
// 		imwrite(str.GetBuffer(), mQ_interval_color);
// 
// 		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\visidiff map %d checked.bmp", i);
// 		imwrite(str.GetBuffer(), mVisiDiff_interval_color);
// 
// 		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\cloud points %d checked.txt", i);
// 		WriteDepthMap(str, vAllCams[i], vAllImgs[i], vDepths_final[i]);
// 
// 		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\weighted cloud points %d checked.txt", i);
// 		WriteDepthMap(str, vAllCams[i], mQ_interval_color, vDepths_final[i]);
// 	}
	//////////////////////////////////////////////////////////////////////////

	return TRUE;
}

void CDeepVoidApp::OnPropvisi()
{
	// TODO: Add your command handler code here
	AfxBeginThread(PropVisi, this, THREAD_PRIORITY_NORMAL);
}

UINT Runfixedvisi(LPVOID param)
{
	CDeepVoidApp * pApp = (CDeepVoidApp * )param;

	//////////////////////////////////////////////////////////////////////////
	int i,j,k, ii,jj,kk;

	int nCam = pApp->m_vCams.size();

	vector<Mat> vImages, vSilhouettes;
	vector<Mat> vDepths, vHxs, vHys, vScores;
	vector<Mat> vVisis;

	vector<Matx33d> vKs,vRs,vKRs;
	vector<Matx31d> vts,vKts;

	int imgWidth, imgHeight;

	// first, extract all the features in each image
	for (i=0;i<nCam;i++)
	{
		// read corresponding image data into memory first
		char * pDir = (char *)pApp->m_pMainFrame->m_wndImgThumbnailPane.m_wndImgListCtrl.GetItemData(i);

		CString strDir;
		strDir.Format(_T("%s"), pDir);
		strDir.Trim();

		Mat img = imread(strDir.GetBuffer(), CV_LOAD_IMAGE_UNCHANGED);

		imgWidth = img.cols;
		imgHeight = img.rows;

		vImages.push_back(img); // store every image

		strDir.Format("E:\\Test Data\\templeRing\\contours\\templeR%04d_seg.png", i+1);
		img = imread(strDir.GetBuffer(), CV_LOAD_IMAGE_GRAYSCALE);
		vSilhouettes.push_back(img);

		CString strtmp;
		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140515\\cam%02d.txt", i);
		ReadCameraData(strtmp, pApp->m_vCams[i]);

		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140521 morning\\final PatchMatch depth map ref %02d.txt", i);
		FILE * file_depth = fopen(strtmp, "r");

		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140521 morning\\final PatchMatch hx map ref %02d.txt", i);
		FILE * file_hx = fopen(strtmp, "r");

		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140521 morning\\final PatchMatch hy map ref %02d.txt", i);
		FILE * file_hy = fopen(strtmp, "r");

		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140521 morning\\final PatchMatch score map ref %02d.txt", i);
		FILE * file_score = fopen(strtmp, "r");

		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\01-05 20140521 morning\\visi map propagated ref %02d.txt", i);
		FILE * file_visi = fopen(strtmp, "r");

		Mat mDepth(imgHeight, imgWidth, CV_32FC1);
		Mat mhx(imgHeight, imgWidth, CV_32FC1);
		Mat mhy(imgHeight, imgWidth, CV_32FC1);
		Mat mScore(imgHeight, imgWidth, CV_32FC1);
		Mat mVisi(imgHeight, imgWidth, CV_8UC1);

		for (ii=0;ii<imgHeight;ii++)
		{
			for (jj=0;jj<imgWidth;jj++)
			{
				double d,hx,hy,score;
				int visi;
				fscanf(file_depth, "%lf	", &d);
				fscanf(file_hx, "%lf	", &hx);
				fscanf(file_hy, "%lf	", &hy);
				fscanf(file_score, "%lf	", &score);
				fscanf(file_visi, "%d	", &visi);

				mDepth.at<float>(ii,jj) = d;
				mhx.at<float>(ii,jj) = hx;
				mhy.at<float>(ii,jj) = hy;
				mScore.at<float>(ii,jj) = score;
				mVisi.at<uchar>(ii,jj) = (uchar)visi;
			}
		}
		fclose(file_depth);
		fclose(file_hx);
		fclose(file_hy);
		fclose(file_score);
		fclose(file_visi);

		vDepths.push_back(mDepth);
		vHxs.push_back(mhx);
		vHys.push_back(mhy);
		vScores.push_back(mScore);
		vVisis.push_back(mVisi);

		cam_data cam = pApp->m_vCams[i];

		Matx33d mK, mR;
		Matx31d mt;

		for (ii=0;ii<3;ii++)
		{
			for (jj=0;jj<3;jj++)
			{
				mR(ii,jj) = cam.R[ii*3+jj];
			}
		}
		mt(0)   = cam.t[0];	mt(1)   = cam.t[1];	mt(2)   = cam.t[2];	
		mK(0,0) = cam.fx;	mK(0,1) = cam.s;	mK(0,2) = cam.cx;
		mK(1,0) = 0;		mK(1,1) = cam.fy;	mK(1,2) = cam.cy;
		mK(2,0) = 0;		mK(2,1) = 0;		mK(2,2) = 1;

		vKs.push_back(mK);
		vRs.push_back(mR);
		vts.push_back(mt);
		vKRs.push_back(mK*mR);
		vKts.push_back(mK*mt);
	}

	double bbox_xmin = -0.023121;	double bbox_xmax = 0.078626;
	double bbox_ymin = -0.038009;	double bbox_ymax = 0.121636;
	double bbox_zmin = -0.091940;	double bbox_zmax = -0.017395;

	vector<CloudPoint> cloudpts_bbox;
	CloudPoint cloudpt;
	cloudpt.m_pt.x = bbox_xmin;	cloudpt.m_pt.y = bbox_ymin;	cloudpt.m_pt.z = bbox_zmin;	cloudpts_bbox.push_back(cloudpt);
	cloudpt.m_pt.x = bbox_xmin;	cloudpt.m_pt.y = bbox_ymin;	cloudpt.m_pt.z = bbox_zmax;	cloudpts_bbox.push_back(cloudpt);
	cloudpt.m_pt.x = bbox_xmin;	cloudpt.m_pt.y = bbox_ymax;	cloudpt.m_pt.z = bbox_zmin;	cloudpts_bbox.push_back(cloudpt);
	cloudpt.m_pt.x = bbox_xmin;	cloudpt.m_pt.y = bbox_ymax;	cloudpt.m_pt.z = bbox_zmax;	cloudpts_bbox.push_back(cloudpt);

	cloudpt.m_pt.x = bbox_xmax;	cloudpt.m_pt.y = bbox_ymax;	cloudpt.m_pt.z = bbox_zmax;	cloudpts_bbox.push_back(cloudpt);
	cloudpt.m_pt.x = bbox_xmax;	cloudpt.m_pt.y = bbox_ymax;	cloudpt.m_pt.z = bbox_zmin;	cloudpts_bbox.push_back(cloudpt);
	cloudpt.m_pt.x = bbox_xmax;	cloudpt.m_pt.y = bbox_ymin;	cloudpt.m_pt.z = bbox_zmax;	cloudpts_bbox.push_back(cloudpt);
	cloudpt.m_pt.x = bbox_xmax;	cloudpt.m_pt.y = bbox_ymin;	cloudpt.m_pt.z = bbox_zmin;	cloudpts_bbox.push_back(cloudpt);

	// parameters
	int wndSize = 5;
	double thresh_ncc = 0.6;
	double thresh_norm = 80;
	int mincam = 1;
	/*double minncc = thresh_ncc*mincam/(vAllCams.size()-1);*/
	double minncc = thresh_ncc;
	double maxncc_minncc_1 = 1/(1-minncc);
	double factor = 0.5;
	int nRandSamp = 8;
	int nPatchMatchIter = 4;
	int halfwnd = (wndSize-1)*0.5;
	double thresh_normdir = 0.1;
	double thresh_radio = 0.01;

	// vector<Mat> vDepths, vHxs, vHys, vDepths_final, vHxs_final, vHys_final, vQuality_final, vVisis, vVisis_diff;

	for (k=0;k<nCam;k++)
	{
		int idx_refimg = k;

		Matx33d mR0 = vRs[idx_refimg];
		Matx33d mK0 = vKs[idx_refimg];
		Matx31d mt0 = vts[idx_refimg];

		Mat mDepth = vDepths[idx_refimg];
		Mat mHx = vHxs[idx_refimg];
		Mat mHy = vHys[idx_refimg];
		Mat mScore = vScores[idx_refimg];
		Mat mVisi = vVisis[idx_refimg];

		double max_depth,min_depth,max_incre_x,min_incre_x,max_incre_y,min_incre_y,max_score,min_score;

		minMaxIdx(mDepth, &min_depth, &max_depth);
		minMaxIdx(mHx, &min_incre_x, &max_incre_x);
		minMaxIdx(mHy, &min_incre_y, &max_incre_y);

		vector<Mat> vRndFields_normed(3);

		Mat mScores_normed_color(mScore.rows, mScore.cols, CV_8UC3);

		mDepth.convertTo(vRndFields_normed[0], CV_8UC1, 255/(max_depth-min_depth), -255*min_depth/(max_depth-min_depth));
		mHx.convertTo(vRndFields_normed[1], CV_8UC1, 255/(max_incre_x-min_incre_x), -255*min_incre_x/(max_incre_x-min_incre_x));
		mHy.convertTo(vRndFields_normed[2], CV_8UC1, 255/(max_incre_y-min_incre_y), -255*min_incre_y/(max_incre_y-min_incre_y));

		CString strInfo;
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\initial depth map fixed visi ref %02d.bmp", k);
		imwrite(strInfo.GetBuffer(), vRndFields_normed[0]);
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\initial hx map fixed visi ref %02d.bmp", k);
		imwrite(strInfo.GetBuffer(), vRndFields_normed[1]);
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\initial hy map fixed visi ref %02d.bmp", k);
		imwrite(strInfo.GetBuffer(), vRndFields_normed[2]);

		for (i=0;i<nPatchMatchIter;i++)
		{
			Scan_Update_rndField_original_fixedvisi(pApp->m_vCams, vImages, cloudpts_bbox, thresh_norm, idx_refimg, mDepth, mHx, mHy,
				mScore, mVisi, i, mincam, factor, nRandSamp, wndSize);

			minMaxIdx(mDepth, &min_depth, &max_depth);
			minMaxIdx(mHx, &min_incre_x, &max_incre_x);
			minMaxIdx(mHy, &min_incre_y, &max_incre_y);
			minMaxIdx(mScore, &min_score, &max_score);

			mDepth.convertTo(vRndFields_normed[0], CV_8UC1, 255/(max_depth-min_depth), -255*min_depth/(max_depth-min_depth));
			mHx.convertTo(vRndFields_normed[1], CV_8UC1, 255/(max_incre_x-min_incre_x), -255*min_incre_x/(max_incre_x-min_incre_x));
			mHy.convertTo(vRndFields_normed[2], CV_8UC1, 255/(max_incre_y-min_incre_y), -255*min_incre_y/(max_incre_y-min_incre_y));

			for (ii=0;ii<mScore.rows;ii++)
			{
				for (jj=0;jj<mScore.cols;jj++)
				{
					if (mScore.at<float>(ii,jj)<0)
					{
						mScores_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
						mScores_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
						mScores_normed_color.at<Vec3b>(ii,jj).val[2] = 255;
					}
					else
					{
						mScores_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
						mScores_normed_color.at<Vec3b>(ii,jj).val[1] = FTOI(255*(mScore.at<float>(ii,jj)-minncc)*maxncc_minncc_1);
						mScores_normed_color.at<Vec3b>(ii,jj).val[2] = 0;
					}
				}
			}

			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\depth map fixed visi iteration %02d ref %02d.bmp", i, k);
			imwrite(strInfo.GetBuffer(), vRndFields_normed[0]);
			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\hx map fixed visi iteration %02d ref %02d.bmp", i, k);
			imwrite(strInfo.GetBuffer(), vRndFields_normed[1]);
			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\hy map fixed visi iteration %02d ref %02d.bmp", i, k);
			imwrite(strInfo.GetBuffer(), vRndFields_normed[2]);
			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\score map fixed visi iteration %02d ref %02d.bmp", i, k);
			imwrite(strInfo.GetBuffer(), mScores_normed_color);

			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\cloud points fixed visi iteration %02d ref %02d.txt", i, k);
			WriteDepthMap(strInfo,pApp->m_vCams[idx_refimg],vImages[idx_refimg],mDepth,mScore);
		}

		// after phase 1, i.e. PatchMatch, depth map and normal maps are without -1, but there are -1 in score map denoting invalid pixels
		// output the results of the last iteration of PatchMatch
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final PatchMatch depth map fixed visi ref %02d.txt", k);
		FILE * file_depth = fopen(strInfo, "w");
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final PatchMatch hx map fixed visi ref %02d.txt", k);
		FILE * file_hx = fopen(strInfo, "w");
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final PatchMatch hy map fixed visi ref %02d.txt", k);
		FILE * file_hy = fopen(strInfo, "w");
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final PatchMatch score map fixed visi ref %02d.txt", k);
		FILE * file_score = fopen(strInfo, "w");

		for (i=0;i<mDepth.rows;i++)
		{
			for (j=0;j<mDepth.cols;j++)
			{
				fprintf(file_depth, "%.12f	", mDepth.at<float>(i,j));
				fprintf(file_hx, "%.12f	", mHx.at<float>(i,j));
				fprintf(file_hy, "%.12f	", mHy.at<float>(i,j));
				fprintf(file_score, "%.12f	", mScore.at<float>(i,j));
			}
			fprintf(file_depth, "\n");
			fprintf(file_hx, "\n");
			fprintf(file_hy, "\n");
			fprintf(file_score, "\n");
		}
		fclose(file_depth);
		fclose(file_hx);
		fclose(file_hy);
		fclose(file_score);

		// phase 2, MPGC //////////////////////////////////////////////////////////////////////////
		Mat mDepth_optim(imgHeight, imgWidth, CV_32FC1, Scalar(-1));
		Mat mhx_optim(imgHeight, imgWidth, CV_32FC1, Scalar(-1));
		Mat mhy_optim(imgHeight, imgWidth, CV_32FC1, Scalar(-1));


		//double max_depth,min_depth,max_incre_x,min_incre_x,max_incre_y,min_incre_y,max_score,min_score;
		minMaxIdx(mDepth, &min_depth, &max_depth);
		minMaxIdx(mHx, &min_incre_x, &max_incre_x);
		minMaxIdx(mHy, &min_incre_y, &max_incre_y);

		double fx_ref = pApp->m_vCams[idx_refimg].fx;
		double fx_ref_1 = 1/fx_ref;
		double fy_ref = pApp->m_vCams[idx_refimg].fy;
		double fy_ref_1 = 1/fy_ref;
		double cx_ref = pApp->m_vCams[idx_refimg].cx;
		double cy_ref = pApp->m_vCams[idx_refimg].cy;

		for (i=0;i<imgHeight;i++)
		{
			CString strInfo;
			strInfo.Format("optimize row %04d", i);
			pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

			double nimgy = (i-cy_ref)*fy_ref_1;

			for (j=0;j<imgWidth;j++)
			{
				double nimgx = (j-cx_ref)*fx_ref_1;

				double depth_init = mDepth.at<float>(i,j);
				double hx_init = mHx.at<float>(i,j);
				double hy_init = mHy.at<float>(i,j);
				double score = mScore.at<float>(i,j);

				if (score<0)
				{
					mDepth_optim.at<float>(i,j) = depth_init;
					mhx_optim.at<float>(i,j) = hx_init;
					mhy_optim.at<float>(i,j) = hy_init;
					continue;
				}

				int i_real, j_real;
				MakeSureNotOutBorder(j,i,j_real,i_real,halfwnd,imgWidth,imgHeight);

				vector<bool> vScores;
				InterpVisiVector_uchar(mVisi.at<uchar>(i,j), vScores);

				vector<int> vIdxValidCam;
				for (kk=0;kk<nCam;kk++)
				{
					if (!vScores[kk])
					{
						continue;
					}
					vIdxValidCam.push_back(kk);
				}

				double depth_optim, hx_optim, hy_optim;
				if (optim_gn_drhxhyck(pApp->m_vCams, vImages, vIdxValidCam, idx_refimg, j_real, i_real, wndSize, wndSize,
					depth_init, hx_init, hy_init, depth_optim, hx_optim, hy_optim, false, 20, 1.0E-8, 1.0E-6) &&
					depth_optim>min_depth && depth_optim<max_depth
					//&& isvalid_hxhy(fx_ref,fy_ref,nimgx,nimgy,depth_optim,hx_optim,hy_optim,thresh_normdir)// hx_optim>min_incre_x && hx_optim<max_incre_x && hy_optim>min_incre_y && hy_optim<max_incre_y
					)
				{
					mDepth_optim.at<float>(i,j) = depth_optim;
					mhx_optim.at<float>(i,j) = hx_optim;
					mhy_optim.at<float>(i,j) = hy_optim;
				}
				else
				{
					mDepth_optim.at<float>(i,j) = depth_init;
					mhx_optim.at<float>(i,j) = hx_init;
					mhy_optim.at<float>(i,j) = hy_init;
					mScore.at<float>(i,j) = -1;
				}
			}
		}

		strInfo.Format("%lf	%lf	%lf	%lf	%lf	%lf", min_depth, max_depth, min_incre_x, max_incre_x, min_incre_y, max_incre_y);
		pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

		minMaxIdx(mDepth_optim, &min_depth, &max_depth);
		minMaxIdx(mhx_optim, &min_incre_x, &max_incre_x);
		minMaxIdx(mhy_optim, &min_incre_y, &max_incre_y);
		minMaxIdx(mScore, &min_score, &max_score);
		strInfo.Format("%lf	%lf	%lf	%lf	%lf	%lf", min_depth, max_depth, min_incre_x, max_incre_x, min_incre_y, max_incre_y);
		pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

		Mat mDepth_optim_normed, mhx_optim_normed, mhy_optim_normed;
		//Mat mScores_normed_color(imgHeight, imgWidth, CV_8UC3);

		mDepth_optim.convertTo(mDepth_optim_normed, CV_8UC1, 255/(max_depth-min_depth), -255*min_depth/(max_depth-min_depth));
		mhx_optim.convertTo(mhx_optim_normed, CV_8UC1, 255/(max_incre_x-min_incre_x), -255*min_incre_x/(max_incre_x-min_incre_x));
		mhy_optim.convertTo(mhy_optim_normed, CV_8UC1, 255/(max_incre_y-min_incre_y), -255*min_incre_y/(max_incre_y-min_incre_y));

		for (ii=0;ii<imgHeight;ii++)
		{
			for (jj=0;jj<imgWidth;jj++)
			{
				if (mScore.at<float>(ii,jj)<0)
				{
					mScores_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
					mScores_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
					mScores_normed_color.at<Vec3b>(ii,jj).val[2] = 255;
				}
				else
				{
					mScores_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
					mScores_normed_color.at<Vec3b>(ii,jj).val[1] = FTOI(255*(mScore.at<float>(ii,jj)-minncc)*maxncc_minncc_1);
					mScores_normed_color.at<Vec3b>(ii,jj).val[2] = 0;
				}
			}
		}
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\depth map fixed visi optimized ref %02d.bmp", k);
		imwrite(strInfo.GetBuffer(), mDepth_optim_normed);
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\hx map fixed visi optimized ref %02d.bmp", k);
		imwrite(strInfo.GetBuffer(), mhx_optim_normed);
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\hy map fixed visi optimized ref %02d.bmp", k);
		imwrite(strInfo.GetBuffer(), mhy_optim_normed);
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\score map fixed visi optimized ref %02d.bmp", k);
		imwrite(strInfo.GetBuffer(), mScores_normed_color);

		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\cloud points fixed visi optimized ref %02d.txt", k);
		WriteDepthMap(strInfo,pApp->m_vCams[idx_refimg],vImages[idx_refimg],mDepth_optim,mScore);

		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final optimized depth map fixed visi ref %02d.txt", k);
		file_depth = fopen(strInfo, "w");
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final optimized hx map fixed visi ref %02d.txt", k);
		file_hx = fopen(strInfo, "w");
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final optimized hy map fixed visi ref %02d.txt", k);
		file_hy = fopen(strInfo, "w");
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final optimized score map fixed visi ref %02d.txt", k);
		file_score = fopen(strInfo, "w");

		for (i=0;i<mDepth_optim.rows;i++)
		{
			for (j=0;j<mDepth_optim.cols;j++)
			{
				double score = mScore.at<float>(i,j);
				if (score<0)
				{
					double tmp = -1;
					fprintf(file_depth, "%.12f	", tmp);
					fprintf(file_hx, "%.12f	", tmp);
					fprintf(file_hy, "%.12f	", tmp);
					fprintf(file_score, "%.12f	", tmp);

					mDepth_optim.at<float>(i,j) = -1;
					mhx_optim.at<float>(i,j) = -1;
					mhy_optim.at<float>(i,j) = -1;
					mScore.at<float>(i,j) = -1;
				} 
				else
				{
					fprintf(file_depth, "%.12f	", mDepth_optim.at<float>(i,j));
					fprintf(file_hx, "%.12f	", mhx_optim.at<float>(i,j));
					fprintf(file_hy, "%.12f	", mhy_optim.at<float>(i,j));
					fprintf(file_score, "%.12f	", mScore.at<float>(i,j));
				}
			}
			fprintf(file_depth, "\n");
			fprintf(file_hx, "\n");
			fprintf(file_hy, "\n");
			fprintf(file_score, "\n");
		}
		fclose(file_depth);
		fclose(file_hx);
		fclose(file_hy);
		fclose(file_score);

		vDepths.push_back(mDepth_optim);
		vHxs.push_back(mhx_optim);
		vHys.push_back(mhy_optim);
		vVisis.push_back(mVisi);
		///////////////////////////////////////////////////////////////////////////////////////////
	}	

	return TRUE;
}

void CDeepVoidApp::OnRunfixedvisi()
{
	// TODO: Add your command handler code here
	AfxBeginThread(Runfixedvisi, this, THREAD_PRIORITY_NORMAL);
}

UINT NewPatchMatch(LPVOID param)
{
	CDeepVoidApp * pApp = (CDeepVoidApp * )param;

	int i,j,ii,jj,k,kk;

	CString strInfo;

	int imgWidth, imgHeight;

	// read in all the IO and EO of test images
	int nAllImgs;
//	FILE * file = fopen("E:\\Test Data\\templeSparseRing\\templeSR_par.txt", "r");
	FILE * file = fopen("E:\\Test Data\\templeRing\\templeR_par.txt", "r");
//	FILE * file = fopen("E:\\Test Data\\dinoSparseRing\\dinoSR_par.txt", "r");
//	FILE * file = fopen("E:\\Test Data\\dinoRing\\dinoR_par.txt", "r");
	
	fscanf(file, "%d", &nAllImgs);

	vector<cam_data> vAllCams;
	vector<Mat> vAllImgs, vSilhouettes;
	vector<CString> vPaths_imgs;

	for (i=0;i<nAllImgs;i++)
	{
		cam_data cam;

		double tmp;
		char strtmp[48];
		fscanf(file, "%s", strtmp);

		fscanf(file, "%lf", &cam.fx);	fscanf(file, "%lf", &cam.s);	fscanf(file, "%lf", &cam.cx);
		fscanf(file, "%lf", &tmp);		fscanf(file, "%lf", &cam.fy);	fscanf(file, "%lf", &cam.cy);
		fscanf(file, "%lf", &tmp);		fscanf(file, "%lf", &tmp);		fscanf(file, "%lf", &tmp);

		for (ii=0;ii<3;ii++)
		{
			for (jj=0;jj<3;jj++)
			{
				fscanf(file, "%lf", &cam.R[ii*3+jj]);
			}
		}

		for (j=0;j<3;j++)
		{
			fscanf(file, "%lf", &cam.t[j]);
		}

		memset(cam.k, 0, 5*sizeof(double));
		cam.dist_type = 1;

		vAllCams.push_back(cam);

		CString strDir;
//		strDir.Format("E:\\Test Data\\templeSparseRing\\templeSR%04d.png", i+1);
		strDir.Format("E:\\Test Data\\templeRing\\templeR%04d.png", i+1);
//		strDir.Format("E:\\Test Data\\dinoSparseRing\\dinoSR%04d.png", i+1);
//		strDir.Format("E:\\Test Data\\dinoRing\\dinoR%04d.png", i+1);

		vPaths_imgs.push_back(strDir);

		Mat img = imread(strDir.GetBuffer(), CV_LOAD_IMAGE_UNCHANGED);

		imgWidth = img.cols; imgHeight = img.rows;

		vAllImgs.push_back(img); // store every image

		// read in contour images
//		strDir.Format("E:\\Test Data\\templeSparseRing\\contours\\templeSR%04d_seg.png", i+1);
		strDir.Format("E:\\Test Data\\templeRing\\contours\\templeR%04d_seg.png", i+1);
//		strDir.Format("E:\\Test Data\\dinoSparseRing\\contours\\dinoSR%04d_seg.png", i+1);
//		strDir.Format("E:\\Test Data\\dinoRing\\contours\\dinoR%04d_seg.png", i+1);
		img = imread(strDir.GetBuffer(), CV_LOAD_IMAGE_GRAYSCALE);
		vSilhouettes.push_back(img);
	}

	fclose(file);

	// bounding box of temple sparse ring
// 	double bbox_xmin = -0.073568;	double bbox_xmax = 0.028855;
// 	double bbox_ymin = 0.021728;	double bbox_ymax = 0.181892;
// 	double bbox_zmin = -0.012445;	double bbox_zmax = 0.062736;

	// bounding box of temple ring
	double bbox_xmin = -0.023121;	double bbox_xmax = 0.078626;
	double bbox_ymin = -0.038009;	double bbox_ymax = 0.121636;
	double bbox_zmin = -0.091940;	double bbox_zmax = -0.017395;

	// bounding box of dino sparse ring
// 	double bbox_xmin = -0.061897;	double bbox_xmax = 0.010897;
// 	double bbox_ymin = -0.018874;	double bbox_ymax = 0.068227;
// 	double bbox_zmin = -0.057845;	double bbox_zmax = 0.015495;

	// bounding box of dino ring
// 	double bbox_xmin = -0.021897;	double bbox_xmax = 0.050897;
// 	double bbox_ymin = 0.021126;	double bbox_ymax = 0.108227;
// 	double bbox_zmin = -0.017845;	double bbox_zmax = 0.055495;

	vector<CloudPoint> cloudpts_bbox;
	CloudPoint cloudpt;
	cloudpt.m_pt.x = bbox_xmin;	cloudpt.m_pt.y = bbox_ymin;	cloudpt.m_pt.z = bbox_zmin;	cloudpts_bbox.push_back(cloudpt);
	cloudpt.m_pt.x = bbox_xmin;	cloudpt.m_pt.y = bbox_ymin;	cloudpt.m_pt.z = bbox_zmax;	cloudpts_bbox.push_back(cloudpt);
	cloudpt.m_pt.x = bbox_xmin;	cloudpt.m_pt.y = bbox_ymax;	cloudpt.m_pt.z = bbox_zmin;	cloudpts_bbox.push_back(cloudpt);
	cloudpt.m_pt.x = bbox_xmin;	cloudpt.m_pt.y = bbox_ymax;	cloudpt.m_pt.z = bbox_zmax;	cloudpts_bbox.push_back(cloudpt);

	cloudpt.m_pt.x = bbox_xmax;	cloudpt.m_pt.y = bbox_ymax;	cloudpt.m_pt.z = bbox_zmax;	cloudpts_bbox.push_back(cloudpt);
	cloudpt.m_pt.x = bbox_xmax;	cloudpt.m_pt.y = bbox_ymax;	cloudpt.m_pt.z = bbox_zmin;	cloudpts_bbox.push_back(cloudpt);
	cloudpt.m_pt.x = bbox_xmax;	cloudpt.m_pt.y = bbox_ymin;	cloudpt.m_pt.z = bbox_zmax;	cloudpts_bbox.push_back(cloudpt);
	cloudpt.m_pt.x = bbox_xmax;	cloudpt.m_pt.y = bbox_ymin;	cloudpt.m_pt.z = bbox_zmin;	cloudpts_bbox.push_back(cloudpt);


	// 20140513 PatchMatch ////////////////////////////////////////////////////////////////////////////////////////////////
// 	vector<cam_data> vCamShort(vAllCams.begin()+12, vAllCams.begin()+17);
// 	vector<Mat> vImgShort(vAllImgs.begin()+12, vAllImgs.begin()+17);
	vector<cam_data> vCamShort(vAllCams.begin(), vAllCams.begin()+5);
	vector<Mat> vImgShort(vAllImgs.begin(), vAllImgs.begin()+5);
// 	vector<cam_data> vCamShort(vAllCams.begin()+3, vAllCams.begin()+5);
// 	vector<Mat> vImgShort(vAllImgs.begin()+3, vAllImgs.begin()+5);

// 	vAllCams = vCamShort;
// 	vAllImgs = vImgShort;

	for (i=0;i<vAllCams.size();i++)
	{
		CString strtmp;
		strtmp.Format("C:\\Users\\DeepVoid\\Desktop\\all\\cam%02d.txt", i);
//		SaveCameraData(strtmp, vAllCams[i]);
	}

	// convert to gray images
	vector<Mat> vGrayImages;
	for (i=0;i<vAllImgs.size();i++)
	{
		Mat im_gray;
		cvtColor(vAllImgs[i], im_gray, CV_RGB2GRAY);
		vGrayImages.push_back(im_gray);
	}

	// parameters
	int wndSize = 5;
//	int wndSize = 7;
	double thresh_ncc = 0.6;
	double thresh_norm = 80;
	int mincam = 1;
	/*double minncc = thresh_ncc*mincam/(vAllCams.size()-1);*/
	double minncc = thresh_ncc;
	double maxncc_minncc_1 = 1/(1-minncc);
	double factor = 0.25;
	int nRandSamp = 5;
//	int nPatchMatchIter = 4;
	int nPatchMatchIter = 2;
	int halfwnd = (wndSize-1)*0.5;
	double thresh_normdir = 0.1;
	double thresh_radio = 0.01;
	double ratio_2ndto1st = 0.01;

	vector<Mat> vDepths, vHxs, vHys, vScores, vDepths_final, vHxs_final, vHys_final, vQuality_final, vVisis, vVisiNs, vVisis_diff;

// 	for (k=0;k<vAllCams.size()-1;k++)
// 	{
// 		Mat mDepth(imgHeight, imgWidth, CV_32FC1), mHx(imgHeight, imgWidth, CV_32FC1), mHy(imgHeight, imgWidth, CV_32FC1), mScore(imgHeight, imgWidth, CV_32FC1)/*, mVisi(imgHeight, imgWidth, CV_8UC1),  mVisiN(imgHeight, imgWidth, CV_8UC1)*/;
// 
// 		strInfo.Format("E:\\Test Data\\templeRing\\contours\\templeR%04d_seg.png", k+1);
// 		Mat img = imread(strInfo.GetBuffer(), CV_LOAD_IMAGE_GRAYSCALE);
// 		vSilhouettes.push_back(img);
// 
// 		strInfo.Format("D:\\all\\20140802 new hope\\img04\\constraint normal be smaller than 90\\final PatchMatch depth map ref %02d.txt", k);
// 		FILE * file_depth = fopen(strInfo, "r");
// 		strInfo.Format("D:\\all\\20140802 new hope\\img04\\constraint normal be smaller than 90\\final PatchMatch hx map ref %02d.txt", k);
// 		FILE * file_hx = fopen(strInfo, "r");
// 		strInfo.Format("D:\\all\\20140802 new hope\\img04\\constraint normal be smaller than 90\\final PatchMatch hy map ref %02d.txt", k);
// 		FILE * file_hy = fopen(strInfo, "r");
// 		strInfo.Format("D:\\all\\20140802 new hope\\img04\\constraint normal be smaller than 90\\final PatchMatch score map ref %02d.txt", k);
// 		FILE * file_score = fopen(strInfo, "r");
// // 		strInfo.Format("D:\\all\\20140802 new hope\\img00\\final PatchMatch visi map ref %02d.txt", k);
// // 		FILE * file_visi = fopen(strInfo, "r");
// // 		strInfo.Format("D:\\all\\20140802 new hope\\img00\\final PatchMatch visiN map ref %02d.txt", k);
// // 		FILE * file_visiN = fopen(strInfo, "r");
// 
// 		// at the same time evaluate all parameters
// 		for (i=0;i<imgHeight;i++)
// 		{
// 			for (j=0;j<imgWidth;j++)
// 			{
// 				double depth,hx,hy,score;
// 				int tmp;
// 				fscanf(file_depth, "%lf	", &depth);
// 				fscanf(file_hx, "%lf	", &hx);
// 				fscanf(file_hy, "%lf	", &hy);
// 				fscanf(file_score, "%lf	", &score);
// 
// 				mDepth.at<float>(i,j) = depth;
// 				mHx.at<float>(i,j) = hx;
// 				mHy.at<float>(i,j) = hy;
// 				mScore.at<float>(i,j) = score;
// 
// // 				fscanf(file_visi, "%d	", &tmp);
// // 				mVisi.at<uchar>(i,j) = (uchar)tmp;
// // 				fscanf(file_visiN, "%d	", &tmp);
// // 				mVisiN.at<uchar>(i,j) = (uchar)tmp;
// 			}
// 		}
// 		fclose(file_depth);
// 		fclose(file_hx);
// 		fclose(file_hy);
// 		fclose(file_score);
// // 		fclose(file_visi);
// // 		fclose(file_visiN);
// 
// 		vDepths.push_back(mDepth);
// 		vHxs.push_back(mHx);
// 		vHys.push_back(mHy);
// 		vScores.push_back(mScore);
// // 		vVisis.push_back(mVisi);
// // 		vVisiNs.push_back(mVisiN);
// 	}

	// 20140611 calculate the normal color map
// 	Mat mNormColor;
// 	int idididi = 4;
// 	GetNormColorField(vAllCams[idididi], vDepths[idididi], vHxs[idididi], vHys[idididi], mNormColor);
// 	imwrite("C:\\Users\\DeepVoid\\Desktop\\normcolor.bmp", mNormColor);

//	RemoveSmallDepthRegions_4(vAllCams[0], vAllImgs[0], vDepths[3], vScores[3], 80, vAllImgs[0].cols*vAllImgs[0].rows/400.0);

	vector<vector<int>> vIdxSupports; vector<vector<CloudPoint>> vClouds;
//	file = fopen("E:\\Test Data\\templeSparseRing\\support images.txt", "r");
//	file = fopen("E:\\Test Data\\templeRing\\support images.txt", "r");
	file = fopen("E:\\Test Data\\templeRing\\support images - without 180 roll.txt", "r");
//	file = fopen("E:\\Test Data\\dinoSparseRing\\support images.txt", "r");
//	file = fopen("E:\\Test Data\\dinoRing\\support images.txt", "r");
	for (i=0;i<vAllCams.size();i++)
	{
		vector<int> vIdxSpts;
		int tmp;
		for (j=0;j<4;j++)
//		for (j=0;j<2;j++)
		{
			fscanf(file, "%d	", &tmp);
			vIdxSpts.push_back(tmp);
		}
		vIdxSupports.push_back(vIdxSpts);
		vClouds.push_back(cloudpts_bbox);
	}
	fclose(file);

	CString strOut;
//	strOut.Format("D:\\all\\");
//	strOut.Format("D:\\all\\20140802 new hope\\ultimate fullrun 06 - dinoSparseR - with improved MPGC ensure unimodal and normal - memory efficient\\");
	strOut.Format("E:\\all\\");
//	strOut.Format("D:\\all\\Final experiment results\\temple sparse ring - 20141113\\");

	// 20141215 try pyramid
// 	MVDE_package_141209(strOut,vAllCams,vPaths_imgs,vSilhouettes,vIdxSupports,vClouds,vDepths,vHxs,vHys,vScores,vVisis,
// 		wndSize,thresh_norm,nPatchMatchIter,factor,nRandSamp,90,0.95,1.5,0.01,0.5,0.1,100,5);
// 	return TRUE;
	
//	MVDE_package_01(vAllCams,vAllImgs,vIdxSupports,cloudpts_bbox,vDepths,vHxs,vHys,vScores,vVisis,wndSize,thresh_norm,nPatchMatchIter,factor,nRandSamp,90,0.95,1,0.01,80,100,5);
//	MVDE_package_02(vAllCams,vAllImgs,vIdxSupports,cloudpts_bbox,vDepths,vHxs,vHys,vScores,vVisis,wndSize,thresh_norm,nPatchMatchIter,factor,nRandSamp,90,0.95,2.0,0.05,1,0.01,0.01,80,100,5);
	MVDE_package_final(strOut,vAllCams,vPaths_imgs,vSilhouettes,vIdxSupports,vClouds,vDepths,vHxs,vHys,vScores,vVisis,
		wndSize,thresh_norm,nPatchMatchIter,factor,nRandSamp,90,/*0.01*/0.95,1.5,0.01,0.5,0.01,100,5);
	return TRUE;

	//////////////////////////////////////////////////////////////////////////
	int idx_ref = 4;
	cam_data cam0 = vAllCams[idx_ref];
	Mat img0 = vAllImgs[idx_ref];
	vector<cam_data> vSupportCams;
	vector<Mat> vSupportImgs;
	for (i=0;i<vAllCams.size();i++)
	{
		if (i==idx_ref)
		{
			continue;
		}
		vSupportCams.push_back(vAllCams[i]);
		vSupportImgs.push_back(vAllImgs[i]);
	}

// 	PatchMatch_140802(cam0, vSupportCams, img0, vSupportImgs, cloudpts_bbox, vDepths, vHxs, vHys, vScores, wndSize, thresh_ncc, thresh_norm, nPatchMatchIter, mincam, factor, nRandSamp);
//	return TRUE;

//	ExtractVisibility_NCC_WTA(vAllCams[4], vAllImgs[4], vDepths, vHxs, vHys, vScores);

	for (k=0;k<vSupportCams.size();k++)
	{
		InvalidatePixels_byNormal(cam0, vSupportCams[k], vDepths[k], vHxs[k], vHys[k], vScores[k], k, 90);
	}

	Mat mSel, mVisi;
//	ExtractMostLikelyDepth_DP_withInvalids(cam0, img0, vDepths, vHxs, vHys, vScores, mSel);
	
	ExtractMostLikelyDepth_DP(cam0, img0, vDepths, vHxs, vHys, vScores, mSel);
	AugmentVisibility_basedonMostLikelyDepth(cam0, vSupportCams, img0, vDepths, vHxs, vHys, vScores, mSel, mVisi, 1);
	return TRUE;

	Mat mDepth(imgHeight, imgWidth, CV_32FC1), mHx(imgHeight, imgWidth, CV_32FC1), mHy(imgHeight, imgWidth, CV_32FC1), mScore(imgHeight, imgWidth, CV_32FC1);
	for (i=0;i<imgHeight;i++)
	{
		for (j=0;j<imgWidth;j++)
		{
			int idx = mSel.at<uchar>(i,j);
			mDepth.at<float>(i,j)=vDepths[idx].at<float>(i,j);
			mHx.at<float>(i,j)=vHxs[idx].at<float>(i,j);
			mHy.at<float>(i,j)=vHys[idx].at<float>(i,j);
			mScore.at<float>(i,j)=vScores[idx].at<float>(i,j);
		}
	}

	PatchMatch_140805(cam0,vSupportCams,img0,vSupportImgs,cloudpts_bbox,mDepth,mHx,mHy,mScore,mVisi,wndSize,thresh_ncc,thresh_norm,nPatchMatchIter,mincam,0.1,4);
	//////////////////////////////////////////////////////////////////////////




//	PatchMatch_withViewPropagation_140728(vAllCams, vAllImgs, cloudpts_bbox, vDepths, vHxs, vHys, vScores, vVisis, vVisiNs, wndSize, thresh_ncc, thresh_norm, nPatchMatchIter, mincam, factor, nRandSamp);

//	PatchMatch_withViewPropagation(vAllCams, vAllImgs, cloudpts_bbox, vDepths, vHxs, vHys, vScores, vVisis, vVisiNs, wndSize, thresh_ncc, thresh_norm, nPatchMatchIter, mincam, factor, nRandSamp);

//	DepthConsistencyCheck_QualityEvaluation_visi(vAllCams, vAllImgs, vSilhouettes, vDepths, vHxs, vHys, vVisis, vScores, vQuality_final, wndSize, thresh_radio);

//	DepthConsistencyCheck_QualityEvaluation_visi_20140731(vAllCams, vAllImgs, vSilhouettes, vDepths, vHxs, vHys, vVisis, vScores, wndSize, thresh_radio);

//	VisibilityUpdate(vAllCams, vDepths, vHxs, vHys, vScores, vVisis, vVisiNs, /*thresh_radio*/0.005);

//	PatchMatch_withViewPropagation_fixedVisi(vAllCams, vAllImgs, vDepths, vHxs, vHys, vScores, vVisis, vVisiNs, wndSize, thresh_ncc, thresh_norm, 75, 2, 1, 0.1, 4);

// 	int x_test = 468;
// 	int y_test = 383;
// 	int y_test = 276;
// 
// 	int idx_test = 4;
// 
// 	double depth_init = vDepths[idx_test].at<float>(y_test,x_test);
// 	double hx_init = vHxs[idx_test].at<float>(y_test,x_test);
// 	double hy_init = vHys[idx_test].at<float>(y_test,x_test);
// 	double score = vScores[idx_test].at<float>(y_test,x_test);
// 
// 	vector<bool> vbools;
// 	InterpVisiVector_uchar(vVisis[idx_test].at<uchar>(y_test,x_test), vbools);
// 
// 	vector<int> vIdxValidCam;
// 	for (kk=0;kk<vAllCams.size();kk++)
// 	{
// 		if (!vbools[kk])
// 		{
// 			continue;
// 		}
// 		vIdxValidCam.push_back(kk);
// 	}
// 
// 	double depth_optim, hx_optim, hy_optim;
// 
//  	optim_gn_drhxhyck_new(vAllCams, vAllImgs, vIdxValidCam, idx_test, x_test, y_test, wndSize, wndSize,
//  		depth_init, hx_init, hy_init, depth_optim, hx_optim, hy_optim, true, 20, 1.0E-8, 1.0E-6);
// 	optim_gn_drhxhyck_NCCcontrolled(vAllCams, vAllImgs, vIdxValidCam, idx_test, x_test, y_test, wndSize, wndSize,
// 		depth_init, hx_init, hy_init, depth_optim, hx_optim, hy_optim, 20, 1.0E-8, 1.0E-6);

// 	for (k=0;k<vAllCams.size();k++)
// 	{
// 		strInfo.Format("D:\\all\\point cloud ref %02d.txt", k);
// 		OutputPointCloud(strInfo, vAllCams[k], vAllImgs[k], vDepths[k], vHxs[k], vHys[k], vScores[k]);
// 	}
//	VisibilityUpdate(vAllCams, vAllImgs, vDepths, vHxs, vHys, vScores, vVisis, vVisiNs, wndSize, thresh_ncc, thresh_radio);

//	MPGC(vAllCams, vAllImgs, vDepths, vHxs, vHys, vScores, vVisis, vVisiNs, wndSize, false, 20, 1.0E-8, 1.0E-6);

//	DepthConsistencyCheck_QualityEvaluation_visi(vAllCams, vAllImgs, vSilhouettes, vDepths, vHxs, vHys, vVisis, vScores, vQuality_final, wndSize, thresh_radio);

	return TRUE;

	for (k=0;k<vAllCams.size();k++)
	{
		int idx_refimg = k;

		Matx33d mR0,mK0;
		Matx31d mt0;

		for (ii=0;ii<3;ii++)
		{
			for (jj=0;jj<3;jj++)
			{
				mR0(ii,jj) = vAllCams[idx_refimg].R[ii*3+jj];
			}
		}
		mK0(0,0) = vAllCams[idx_refimg].fx; mK0(0,1) = vAllCams[idx_refimg].s;  mK0(0,2) = vAllCams[idx_refimg].cx;
		mK0(1,1) = vAllCams[idx_refimg].fy; mK0(1,2) = vAllCams[idx_refimg].cy; mK0(2,2) = 1;
		mt0(0) = vAllCams[idx_refimg].t[0]; mt0(1) = vAllCams[idx_refimg].t[1]; mt0(2) = vAllCams[idx_refimg].t[2];

		vector<Mat> vRndFields(3);
		InitRndField(mK0, mR0, mt0, cloudpts_bbox, thresh_norm, imgWidth, imgHeight, vRndFields[0], vRndFields[1], vRndFields[2]);

		Mat mScores(imgHeight, imgWidth, CV_32FC1); Mat mVisi(imgHeight, imgWidth, CV_8UC1); Mat mVisiN(imgHeight, imgWidth, CV_8UC1);

		double max_depth,min_depth,max_incre_x,min_incre_x,max_incre_y,min_incre_y,max_score,min_score;

		minMaxIdx(vRndFields[0], &min_depth, &max_depth);
		minMaxIdx(vRndFields[1], &min_incre_x, &max_incre_x);
		minMaxIdx(vRndFields[2], &min_incre_y, &max_incre_y);
		//minMaxIdx(mScores, &min_score, &max_score);

		vector<Mat> vRndFields_normed(3);

		Mat mScores_normed_color(mScores.rows, mScores.cols, CV_8UC3);
		Mat mVisiN_color(mScores.rows, mScores.cols, CV_8UC3);

		vRndFields[0].convertTo(vRndFields_normed[0], CV_8UC1, 255/(max_depth-min_depth), -255*min_depth/(max_depth-min_depth));
		vRndFields[1].convertTo(vRndFields_normed[1], CV_8UC1, 255/(max_incre_x-min_incre_x), -255*min_incre_x/(max_incre_x-min_incre_x));
		vRndFields[2].convertTo(vRndFields_normed[2], CV_8UC1, 255/(max_incre_y-min_incre_y), -255*min_incre_y/(max_incre_y-min_incre_y));

		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\initial depth map ref %02d.bmp", k);
		imwrite(strInfo.GetBuffer(), vRndFields_normed[0]);
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\initial hx map ref %02d.bmp", k);
		imwrite(strInfo.GetBuffer(), vRndFields_normed[1]);
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\initial hy map ref %02d.bmp", k);
		imwrite(strInfo.GetBuffer(), vRndFields_normed[2]);

		for (i=0;i<nPatchMatchIter;i++)
		{
			/*Scan_Update_rndField_original(vAllCams, / *vGrayImages* /vAllImgs, cloudpts_bbox, thresh_norm, idx_refimg, vRndFields[0], vRndFields[1], vRndFields[2], mScores, i, mincam, factor, nRandSamp, wndSize, thresh_ncc);*/
			// 			Scan_Update_rndField_original_visi(vAllCams, /*vGrayImages*/vAllImgs, cloudpts_bbox, thresh_norm, idx_refimg, vRndFields[0], vRndFields[1], vRndFields[2],
			// 				mScores, mVisi, mVisiN, i, mincam, factor, nRandSamp, wndSize, thresh_ncc);
			Scan_Update_rndField_original_visi_new(vAllCams, /*vGrayImages*/vAllImgs, cloudpts_bbox, thresh_norm, idx_refimg, vRndFields[0], vRndFields[1], vRndFields[2],
				mScores, mVisi, mVisiN, i, mincam, factor, nRandSamp, wndSize, thresh_ncc, ratio_2ndto1st);
			minMaxIdx(vRndFields[0], &min_depth, &max_depth);
			minMaxIdx(vRndFields[1], &min_incre_x, &max_incre_x);
			minMaxIdx(vRndFields[2], &min_incre_y, &max_incre_y);
			minMaxIdx(mScores, &min_score, &max_score);

			vRndFields[0].convertTo(vRndFields_normed[0], CV_8UC1, 255/(max_depth-min_depth), -255*min_depth/(max_depth-min_depth));
			vRndFields[1].convertTo(vRndFields_normed[1], CV_8UC1, 255/(max_incre_x-min_incre_x), -255*min_incre_x/(max_incre_x-min_incre_x));
			vRndFields[2].convertTo(vRndFields_normed[2], CV_8UC1, 255/(max_incre_y-min_incre_y), -255*min_incre_y/(max_incre_y-min_incre_y));

			for (ii=0;ii<mScores.rows;ii++)
			{
				for (jj=0;jj<mScores.cols;jj++)
				{
					if (mScores.at<float>(ii,jj)<0)
					{
						mScores_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
						mScores_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
						mScores_normed_color.at<Vec3b>(ii,jj).val[2] = 255;
					}
					else
					{
						mScores_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
						mScores_normed_color.at<Vec3b>(ii,jj).val[1] = FTOI(255*(mScores.at<float>(ii,jj)-minncc)*maxncc_minncc_1);
						mScores_normed_color.at<Vec3b>(ii,jj).val[2] = 0;
					}

					uchar nnn = mVisiN.at<uchar>(ii,jj);

					if (nnn==0)
					{
						mVisiN_color.at<Vec3b>(ii,jj).val[0] = 0;
						mVisiN_color.at<Vec3b>(ii,jj).val[1] = 0;
						mVisiN_color.at<Vec3b>(ii,jj).val[2] = 255;
					}
					else if (nnn==1)
					{
						mVisiN_color.at<Vec3b>(ii,jj).val[0] = 0;
						mVisiN_color.at<Vec3b>(ii,jj).val[1] = 255;
						mVisiN_color.at<Vec3b>(ii,jj).val[2] = 255;
					}
					else if (nnn==2)
					{
						mVisiN_color.at<Vec3b>(ii,jj).val[0] = 255;
						mVisiN_color.at<Vec3b>(ii,jj).val[1] = 0;
						mVisiN_color.at<Vec3b>(ii,jj).val[2] = 255;
					}
					else if (nnn==3)
					{
						mVisiN_color.at<Vec3b>(ii,jj).val[0] = 0;
						mVisiN_color.at<Vec3b>(ii,jj).val[1] = 255;
						mVisiN_color.at<Vec3b>(ii,jj).val[2] = 0;
					}
					else
					{
						mVisiN_color.at<Vec3b>(ii,jj).val[0] = 255;
						mVisiN_color.at<Vec3b>(ii,jj).val[1] = 0;
						mVisiN_color.at<Vec3b>(ii,jj).val[2] = 0;
					}
				}
			}

			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\depth map iteration %02d ref %02d.bmp", i, k);
			imwrite(strInfo.GetBuffer(), vRndFields_normed[0]);
			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\hx map iteration %02d ref %02d.bmp", i, k);
			imwrite(strInfo.GetBuffer(), vRndFields_normed[1]);
			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\hy map iteration %02d ref %02d.bmp", i, k);
			imwrite(strInfo.GetBuffer(), vRndFields_normed[2]);
			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\score map iteration %02d ref %02d.bmp", i, k);
			imwrite(strInfo.GetBuffer(), mScores_normed_color);
			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\visi map iteration %02d ref %02d.bmp", i, k);
			imwrite(strInfo.GetBuffer(), mVisi);
			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\visiN map iteration %02d ref %02d.bmp", i, k);
			imwrite(strInfo.GetBuffer(), mVisiN_color);

			strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\cloud points iteration %02d ref %02d.txt", i, k);
			WriteDepthMap(strInfo,vAllCams[idx_refimg],vAllImgs[idx_refimg],vRndFields[0],mScores);
		}

		// 		for (i=0;i<vRndFields[0].rows;i++)
		// 		{
		// 			for (j=0;j<vRndFields[0].cols;j++)
		// 			{
		// 				double score = mScores.at<float>(i,j);
		// 				if (score<0)
		// 				{
		// 					vRndFields[0].at<float>(i,j) = -1;
		// 					vRndFields[1].at<float>(i,j) = -1;
		// 					vRndFields[2].at<float>(i,j) = -1;
		// 					mScores.at<float>(i,j) = -1;
		// 				}
		// 			}
		// 		}

		// after phase 1, i.e. PatchMatch, depth map and normal maps are without -1, but there are -1 in score map denoting invalid pixels
		// output the results of the last iteration of PatchMatch
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final PatchMatch depth map ref %02d.txt", k);
		FILE * file_depth = fopen(strInfo, "w");
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final PatchMatch hx map ref %02d.txt", k);
		FILE * file_hx = fopen(strInfo, "w");
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final PatchMatch hy map ref %02d.txt", k);
		FILE * file_hy = fopen(strInfo, "w");
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final PatchMatch score map ref %02d.txt", k);
		FILE * file_score = fopen(strInfo, "w");
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final PatchMatch visi map ref %02d.txt", k);
		FILE * file_visi = fopen(strInfo, "w");

		for (i=0;i<vRndFields[0].rows;i++)
		{
			for (j=0;j<vRndFields[0].cols;j++)
			{
				fprintf(file_depth, "%.12f	", vRndFields[0].at<float>(i,j));
				fprintf(file_hx, "%.12f	", vRndFields[1].at<float>(i,j));
				fprintf(file_hy, "%.12f	", vRndFields[2].at<float>(i,j));
				fprintf(file_score, "%.12f	", mScores.at<float>(i,j));
				fprintf(file_visi, "%d	", mVisi.at<uchar>(i,j));
			}
			fprintf(file_depth, "\n");
			fprintf(file_hx, "\n");
			fprintf(file_hy, "\n");
			fprintf(file_score, "\n");
			fprintf(file_visi, "\n");
		}
		fclose(file_depth);
		fclose(file_hx);
		fclose(file_hy);
		fclose(file_score);
		fclose(file_visi);

		// phase 2, MPGC //////////////////////////////////////////////////////////////////////////
		Mat mDepth_optim(imgHeight, imgWidth, CV_32FC1, Scalar(-1));
		Mat mhx_optim(imgHeight, imgWidth, CV_32FC1, Scalar(-1));
		Mat mhy_optim(imgHeight, imgWidth, CV_32FC1, Scalar(-1));


		//double max_depth,min_depth,max_incre_x,min_incre_x,max_incre_y,min_incre_y,max_score,min_score;

		minMaxIdx(vRndFields[0], &min_depth, &max_depth);
		minMaxIdx(vRndFields[1], &min_incre_x, &max_incre_x);
		minMaxIdx(vRndFields[2], &min_incre_y, &max_incre_y);

		double fx_ref = vAllCams[idx_refimg].fx;
		double fx_ref_1 = 1/fx_ref;
		double fy_ref = vAllCams[idx_refimg].fy;
		double fy_ref_1 = 1/fy_ref;
		double cx_ref = vAllCams[idx_refimg].cx;
		double cy_ref = vAllCams[idx_refimg].cy;

		for (i=0;i<imgHeight;i++)
		{
			CString strInfo;
			strInfo.Format("optimize row %04d", i);
			pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

			double nimgy = (i-cy_ref)*fy_ref_1;

			for (j=0;j<imgWidth;j++)
			{
				double nimgx = (j-cx_ref)*fx_ref_1;

				double depth_init = vRndFields[0].at<float>(i,j);
				double hx_init = vRndFields[1].at<float>(i,j);
				double hy_init = vRndFields[2].at<float>(i,j);
				double score = mScores.at<float>(i,j);

				if (score<0)
				{
					mDepth_optim.at<float>(i,j) = vRndFields[0].at<float>(i,j);
					mhx_optim.at<float>(i,j) = vRndFields[1].at<float>(i,j);
					mhy_optim.at<float>(i,j) = vRndFields[2].at<float>(i,j);
					continue;
				}

				int i_real, j_real;
				MakeSureNotOutBorder(j,i,j_real,i_real,halfwnd,imgWidth,imgHeight);

				// 				vector<double> vScores;
				// 				CheckOnePixel_givenOneParamSet(vAllCams,vAllImgs,idx_refimg,j_real,i_real,depth_init,hx_init,hy_init,vScores,wndSize);
				// 
				// 				vector<int> vIdxValidCam;
				// 				for (kk=0;kk<vScores.size();kk++)
				// 				{
				// 					if (vScores[kk]<thresh_ncc)
				// 					{
				// 						continue;
				// 					}
				// 					vIdxValidCam.push_back(kk);
				// 				}

				vector<bool> vScores;
				InterpVisiVector_uchar(mVisi.at<uchar>(i,j), vScores);

				vector<int> vIdxValidCam;
				for (kk=0;kk<vAllCams.size();kk++)
				{
					if (!vScores[kk])
					{
						continue;
					}
					vIdxValidCam.push_back(kk);
				}

				double depth_optim, hx_optim, hy_optim;
				if (optim_gn_drhxhyck(vAllCams, vAllImgs, vIdxValidCam, idx_refimg, j_real, i_real, wndSize, wndSize,
					depth_init, hx_init, hy_init, depth_optim, hx_optim, hy_optim, false, 20, 1.0E-8, 1.0E-6) &&
					depth_optim>min_depth && depth_optim<max_depth
					//&& isvalid_hxhy(fx_ref,fy_ref,nimgx,nimgy,depth_optim,hx_optim,hy_optim,thresh_normdir)// hx_optim>min_incre_x && hx_optim<max_incre_x && hy_optim>min_incre_y && hy_optim<max_incre_y
					)
				{
					mDepth_optim.at<float>(i,j) = depth_optim;
					mhx_optim.at<float>(i,j) = hx_optim;
					mhy_optim.at<float>(i,j) = hy_optim;
				}
				else
				{
					mDepth_optim.at<float>(i,j) = vRndFields[0].at<float>(i,j);
					mhx_optim.at<float>(i,j) = vRndFields[1].at<float>(i,j);
					mhy_optim.at<float>(i,j) = vRndFields[2].at<float>(i,j);
					mScores.at<float>(i,j) = -1;
				}
			}
		}

		CString strInfo;
		strInfo.Format("%lf	%lf	%lf	%lf	%lf	%lf", min_depth, max_depth, min_incre_x, max_incre_x, min_incre_y, max_incre_y);
		pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

		minMaxIdx(mDepth_optim, &min_depth, &max_depth);
		minMaxIdx(mhx_optim, &min_incre_x, &max_incre_x);
		minMaxIdx(mhy_optim, &min_incre_y, &max_incre_y);
		minMaxIdx(mScores, &min_score, &max_score);
		strInfo.Format("%lf	%lf	%lf	%lf	%lf	%lf", min_depth, max_depth, min_incre_x, max_incre_x, min_incre_y, max_incre_y);
		pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

		Mat mDepth_optim_normed, mhx_optim_normed, mhy_optim_normed;
		//Mat mScores_normed_color(imgHeight, imgWidth, CV_8UC3);

		mDepth_optim.convertTo(mDepth_optim_normed, CV_8UC1, 255/(max_depth-min_depth), -255*min_depth/(max_depth-min_depth));
		mhx_optim.convertTo(mhx_optim_normed, CV_8UC1, 255/(max_incre_x-min_incre_x), -255*min_incre_x/(max_incre_x-min_incre_x));
		mhy_optim.convertTo(mhy_optim_normed, CV_8UC1, 255/(max_incre_y-min_incre_y), -255*min_incre_y/(max_incre_y-min_incre_y));

		for (ii=0;ii<imgHeight;ii++)
		{
			for (jj=0;jj<imgWidth;jj++)
			{
				if (mScores.at<float>(ii,jj)<0)
				{
					mScores_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
					mScores_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
					mScores_normed_color.at<Vec3b>(ii,jj).val[2] = 255;
				}
				else
				{
					mScores_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
					mScores_normed_color.at<Vec3b>(ii,jj).val[1] = FTOI(255*(mScores.at<float>(ii,jj)-minncc)*maxncc_minncc_1);
					mScores_normed_color.at<Vec3b>(ii,jj).val[2] = 0;
				}
			}
		}
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\depth map optimized ref %02d.bmp", k);
		imwrite(strInfo.GetBuffer(), mDepth_optim_normed);
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\hx map optimized ref %02d.bmp", k);
		imwrite(strInfo.GetBuffer(), mhx_optim_normed);
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\hy map optimized ref %02d.bmp", k);
		imwrite(strInfo.GetBuffer(), mhy_optim_normed);
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\score map optimized ref %02d.bmp", k);
		imwrite(strInfo.GetBuffer(), mScores_normed_color);

		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\cloud points optimized ref %02d.txt", k);
		WriteDepthMap(strInfo,vAllCams[idx_refimg],vAllImgs[idx_refimg],mDepth_optim,mScores);

		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final optimized depth map ref %02d.txt", k);
		file_depth = fopen(strInfo, "w");
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final optimized hx map ref %02d.txt", k);
		file_hx = fopen(strInfo, "w");
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final optimized hy map ref %02d.txt", k);
		file_hy = fopen(strInfo, "w");
		strInfo.Format("C:\\Users\\DeepVoid\\Desktop\\all\\final optimized score map ref %02d.txt", k);
		file_score = fopen(strInfo, "w");

		for (i=0;i<mDepth_optim.rows;i++)
		{
			for (j=0;j<mDepth_optim.cols;j++)
			{
				double score = mScores.at<float>(i,j);
				if (score<0)
				{
					double tmp = -1;
					fprintf(file_depth, "%.12f	", tmp);
					fprintf(file_hx, "%.12f	", tmp);
					fprintf(file_hy, "%.12f	", tmp);
					fprintf(file_score, "%.12f	", tmp);

					mDepth_optim.at<float>(i,j) = -1;
					mhx_optim.at<float>(i,j) = -1;
					mhy_optim.at<float>(i,j) = -1;
					mScores.at<float>(i,j) = -1;
				} 
				else
				{
					fprintf(file_depth, "%.12f	", mDepth_optim.at<float>(i,j));
					fprintf(file_hx, "%.12f	", mhx_optim.at<float>(i,j));
					fprintf(file_hy, "%.12f	", mhy_optim.at<float>(i,j));
					fprintf(file_score, "%.12f	", mScores.at<float>(i,j));
				}
			}
			fprintf(file_depth, "\n");
			fprintf(file_hx, "\n");
			fprintf(file_hy, "\n");
			fprintf(file_score, "\n");
		}
		fclose(file_depth);
		fclose(file_hx);
		fclose(file_hy);
		fclose(file_score);

		vDepths.push_back(mDepth_optim);
		vHxs.push_back(mhx_optim);
		vHys.push_back(mhy_optim);
		vVisis.push_back(mVisi);
		///////////////////////////////////////////////////////////////////////////////////////////
	}

	// after phase 2, i.e. MPGC, depth map and normal maps have -1 themselves denoting invalid pixels, thus score map is no longer used.

	// phase 3, depth check //////////////////////////////////////////////////////////////////////////////
	vector<Point2d> vMinMaxD, vMinMaxHx, vMinMaxHy, vMinMaxQ; 

	// DepthConsistencyCheck(pApp->m_vCams, vImages, vDepths, vHxs, vHys, vDepths_final, vHxs_final, vHys_final, vMinMaxD, vMinMaxHx, vMinMaxHy, wndSize, thresh_ncc, thresh_radio);
	/*DepthConsistencyCheck_QualityEvaluation(vAllCams, vAllImgs, vDepths, vHxs, vHys, vDepths_final, vHxs_final, vHys_final, vQuality_final, vMinMaxD, vMinMaxHx, vMinMaxHy, vMinMaxQ, wndSize, thresh_ncc, thresh_radio);*/
	DepthConsistencyCheck_QualityEvaluation_visi(vAllCams, vAllImgs, vDepths, vHxs, vHys, vVisis, vDepths_final, vHxs_final, vHys_final, vVisis_diff, vQuality_final,
		vMinMaxD, vMinMaxHx, vMinMaxHy, vMinMaxQ, wndSize, thresh_ncc, thresh_radio);

	for (i=0;i<vAllCams.size();i++)
	{
		double mind = vMinMaxD[i].x; double maxd = vMinMaxD[i].y;
		double minhx = vMinMaxHx[i].x; double maxhx = vMinMaxHx[i].y;
		double minhy = vMinMaxHy[i].x; double maxhy = vMinMaxHy[i].y;
		double minq = vMinMaxQ[i].x; double maxq = vMinMaxQ[i].y;

		double maxd_mind_1 = 1/(maxd-mind);
		double maxhx_minhx_1 = 1/(maxhx-minhx);
		double maxhy_minhy_1 = 1/(maxhy-minhy);
		double maxq_minq_1 = 1/(maxq-minq);
		double threshratio_minq_1 = 1/(thresh_radio-minq);

		Mat mDepth_normed_color(imgHeight, imgWidth, CV_8UC3), mHx_normed_color(imgHeight, imgWidth, CV_8UC3), mHy_normed_color(imgHeight, imgWidth, CV_8UC3);
		Mat mQ_normed_color(imgHeight, imgWidth, CV_8UC3), mQ_interval_color(imgHeight, imgWidth, CV_8UC3), mVisiDiff_interval_color(imgHeight, imgWidth, CV_8UC3);

		for (ii=0;ii<imgHeight;ii++)
		{
			for (jj=0;jj<imgWidth;jj++)
			{
				//////////////////////////////////////////////////////////////////////////
				if (vDepths_final[i].at<float>(ii,jj)<0)
				{
					mDepth_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
					mDepth_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
					mDepth_normed_color.at<Vec3b>(ii,jj).val[2] = 255;
				}
				else
				{
					int val = FTOI(255*(vDepths_final[i].at<float>(ii,jj)-mind)*maxd_mind_1);
					mDepth_normed_color.at<Vec3b>(ii,jj).val[0] = val;
					mDepth_normed_color.at<Vec3b>(ii,jj).val[1] = val;
					mDepth_normed_color.at<Vec3b>(ii,jj).val[2] = val;
				}

				//////////////////////////////////////////////////////////////////////////
				/*if (vHxs_final[i].at<float>(ii,jj)<0)*/
				if (fabs(vHxs_final[i].at<float>(ii,jj)+1)<1.0E-8)
				{
					mHx_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
					mHx_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
					mHx_normed_color.at<Vec3b>(ii,jj).val[2] = 255;
				}
				else
				{
					int val = FTOI(255*(vHxs_final[i].at<float>(ii,jj)-minhx)*maxhx_minhx_1);
					mHx_normed_color.at<Vec3b>(ii,jj).val[0] = val;
					mHx_normed_color.at<Vec3b>(ii,jj).val[1] = val;
					mHx_normed_color.at<Vec3b>(ii,jj).val[2] = val;
				}

				//////////////////////////////////////////////////////////////////////////
				/*if (vHys_final[i].at<float>(ii,jj)<0)*/
				if (fabs(vHys_final[i].at<float>(ii,jj)+1)<1.0E-8)
				{
					mHy_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
					mHy_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
					mHy_normed_color.at<Vec3b>(ii,jj).val[2] = 255;
				}
				else
				{
					int val = FTOI(255*(vHys_final[i].at<float>(ii,jj)-minhy)*maxhy_minhy_1);
					mHy_normed_color.at<Vec3b>(ii,jj).val[0] = val;
					mHy_normed_color.at<Vec3b>(ii,jj).val[1] = val;
					mHy_normed_color.at<Vec3b>(ii,jj).val[2] = val;
				}

				//////////////////////////////////////////////////////////////////////////
				if (fabs(vQuality_final[i].at<float>(ii,jj)+1)<1.0E-8)
				{
					mQ_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
					mQ_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
					mQ_normed_color.at<Vec3b>(ii,jj).val[2] = 255;

					mQ_interval_color.at<Vec3b>(ii,jj).val[0] = 0;
					mQ_interval_color.at<Vec3b>(ii,jj).val[1] = 0;
					mQ_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
				}
				else
				{
					// 					if (vQuality_final[i].at<float>(ii,jj)>thresh_radio)
					// 					{
					// 						mQ_normed_color.at<Vec3b>(ii,jj).val[0] = 0;
					// 						mQ_normed_color.at<Vec3b>(ii,jj).val[1] = 0;
					// 						mQ_normed_color.at<Vec3b>(ii,jj).val[2] = 255;
					// 					}
					// 					else
					// 					{
					// 						int val = FTOI(255*(thresh_radio-vQuality_final[i].at<float>(ii,jj))*threshratio_minq_1);
					// 						mQ_normed_color.at<Vec3b>(ii,jj).val[0] = val;
					// 						mQ_normed_color.at<Vec3b>(ii,jj).val[1] = val;
					// 						mQ_normed_color.at<Vec3b>(ii,jj).val[2] = val;
					// 					}


					int val = FTOI(255*(maxq-vQuality_final[i].at<float>(ii,jj))*maxq_minq_1);
					mQ_normed_color.at<Vec3b>(ii,jj).val[0] = val;
					mQ_normed_color.at<Vec3b>(ii,jj).val[1] = val;
					mQ_normed_color.at<Vec3b>(ii,jj).val[2] = val;


					if (vQuality_final[i].at<float>(ii,jj)<=0.001)
					{
						mQ_interval_color.at<Vec3b>(ii,jj).val[0] = 255;
						mQ_interval_color.at<Vec3b>(ii,jj).val[1] = 0;
						mQ_interval_color.at<Vec3b>(ii,jj).val[2] = 0;
					}
					else if (vQuality_final[i].at<float>(ii,jj)<=0.005)
					{
						mQ_interval_color.at<Vec3b>(ii,jj).val[0] = 0;
						mQ_interval_color.at<Vec3b>(ii,jj).val[1] = 255;
						mQ_interval_color.at<Vec3b>(ii,jj).val[2] = 0;
					}
					else if (vQuality_final[i].at<float>(ii,jj)<=0.01)
					{
						mQ_interval_color.at<Vec3b>(ii,jj).val[0] = 0;
						mQ_interval_color.at<Vec3b>(ii,jj).val[1] = 255;
						mQ_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
					}
					else if (vQuality_final[i].at<float>(ii,jj)<=0.05)
					{
						mQ_interval_color.at<Vec3b>(ii,jj).val[0] = 255;
						mQ_interval_color.at<Vec3b>(ii,jj).val[1] = 0;
						mQ_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
					}
					else if (vQuality_final[i].at<float>(ii,jj)<=0.1)
					{
						mQ_interval_color.at<Vec3b>(ii,jj).val[0] = 255;
						mQ_interval_color.at<Vec3b>(ii,jj).val[1] = 255;
						mQ_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
					}
					else
					{
						mQ_interval_color.at<Vec3b>(ii,jj).val[0] = 0;
						mQ_interval_color.at<Vec3b>(ii,jj).val[1] = 0;
						mQ_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
					}
				}

				//////////////////////////////////////////////////////////////////////////
				if (vVisis_diff[i].at<uchar>(ii,jj)==0)
				{
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[0] = 255;
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[1] = 0;
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[2] = 0;
				}
				else if (vVisis_diff[i].at<uchar>(ii,jj)==1)
				{
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[0] = 0;
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[1] = 255;
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[2] = 0;
				}
				else if (vVisis_diff[i].at<uchar>(ii,jj)==2)
				{
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[0] = 0;
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[1] = 255;
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
				}
				else if (vVisis_diff[i].at<uchar>(ii,jj)==3)
				{
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[0] = 255;
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[1] = 0;
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
				}
				else
				{
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[0] = 0;
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[1] = 0;
					mVisiDiff_interval_color.at<Vec3b>(ii,jj).val[2] = 255;
				}
			}
		}

		CString str;
		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\depth map %d checked.bmp", i);
		imwrite(str.GetBuffer(), mDepth_normed_color);

		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\hx map %d checked.bmp", i);
		imwrite(str.GetBuffer(), mHx_normed_color);

		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\hy map %d checked.bmp", i);
		imwrite(str.GetBuffer(), mHy_normed_color);

		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\quality map A %d checked.bmp", i);
		imwrite(str.GetBuffer(), mQ_normed_color);

		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\quality map B %d checked.bmp", i);
		imwrite(str.GetBuffer(), mQ_interval_color);

		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\visidiff map %d checked.bmp", i);
		imwrite(str.GetBuffer(), mVisiDiff_interval_color);

		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\cloud points %d checked.txt", i);
		WriteDepthMap(str, vAllCams[i], vAllImgs[i], vDepths_final[i]);

		str.Format("C:\\Users\\DeepVoid\\Desktop\\all\\weighted cloud points %d checked.txt", i);
		WriteDepthMap(str, vAllCams[i], mQ_interval_color, vDepths_final[i]);
	}

	return TRUE;
}

void CDeepVoidApp::OnNewpatchmatch()
{
	// TODO: Add your command handler code here
	AfxBeginThread(NewPatchMatch, this, THREAD_PRIORITY_NORMAL);
}

UINT StrechaData(LPVOID param)
{
	CDeepVoidApp * pApp = (CDeepVoidApp * )param;

	int i,j,ii,jj,k,kk;

	CString strInfo;

	vector<CString> vImgPaths;
	vector<vector<CloudPoint>> vClouds;
	vector<cam_data> vAllCams;

	// read in data
	int nImg = 11;  // fountain
//	int nImg = 8;  // herzjesu
	for (i=0;i<nImg;i++)
	{
		strInfo.Format("E:\\Test Data\\fountain\\fountain_dense_bounding\\%04d.png.bounding", i);
		FILE * file_bbox = fopen(strInfo, "r");
		strInfo.Format("E:\\Test Data\\fountain\\fountain_dense_cameras\\%04d.png.camera", i);
		FILE * file_cam = fopen(strInfo, "r");
		CString strPath;
		strPath.Format("E:\\Test Data\\fountain\\fountain_dense_images\\%04d.png", i);

// 		strInfo.Format("E:\\Test Data\\herzjesu\\herzjesu_dense_bounding\\%04d.png.bounding", i);
// 		FILE * file_bbox = fopen(strInfo, "r");
// 		strInfo.Format("E:\\Test Data\\herzjesu\\herzjesu_dense_cameras\\%04d.png.camera", i);
// 		FILE * file_cam = fopen(strInfo, "r");
// 		CString strPath;
// 		strPath.Format("E:\\Test Data\\herzjesu\\herzjesu_dense_images\\%04d.png", i);

		double bbox_xmin, bbox_ymin, bbox_zmin;
		double bbox_xmax, bbox_ymax, bbox_zmax;

		fscanf(file_bbox, "%lf	", &bbox_xmin);
		fscanf(file_bbox, "%lf	", &bbox_ymin);
		fscanf(file_bbox, "%lf	", &bbox_zmin);
		fscanf(file_bbox, "%lf	", &bbox_xmax);
		fscanf(file_bbox, "%lf	", &bbox_ymax);
		fscanf(file_bbox, "%lf	", &bbox_zmax);

		vector<CloudPoint> cloudpts_bbox;
		CloudPoint cloudpt;
		cloudpt.m_pt.x = bbox_xmin;	cloudpt.m_pt.y = bbox_ymin;	cloudpt.m_pt.z = bbox_zmin;	cloudpts_bbox.push_back(cloudpt);
		cloudpt.m_pt.x = bbox_xmin;	cloudpt.m_pt.y = bbox_ymin;	cloudpt.m_pt.z = bbox_zmax;	cloudpts_bbox.push_back(cloudpt);
		cloudpt.m_pt.x = bbox_xmin;	cloudpt.m_pt.y = bbox_ymax;	cloudpt.m_pt.z = bbox_zmin;	cloudpts_bbox.push_back(cloudpt);
		cloudpt.m_pt.x = bbox_xmin;	cloudpt.m_pt.y = bbox_ymax;	cloudpt.m_pt.z = bbox_zmax;	cloudpts_bbox.push_back(cloudpt);

		cloudpt.m_pt.x = bbox_xmax;	cloudpt.m_pt.y = bbox_ymax;	cloudpt.m_pt.z = bbox_zmax;	cloudpts_bbox.push_back(cloudpt);
		cloudpt.m_pt.x = bbox_xmax;	cloudpt.m_pt.y = bbox_ymax;	cloudpt.m_pt.z = bbox_zmin;	cloudpts_bbox.push_back(cloudpt);
		cloudpt.m_pt.x = bbox_xmax;	cloudpt.m_pt.y = bbox_ymin;	cloudpt.m_pt.z = bbox_zmax;	cloudpts_bbox.push_back(cloudpt);
		cloudpt.m_pt.x = bbox_xmax;	cloudpt.m_pt.y = bbox_ymin;	cloudpt.m_pt.z = bbox_zmin;	cloudpts_bbox.push_back(cloudpt);


		cam_data cam;

		double tmp;
		
		fscanf(file_cam, "%lf", &cam.fx);	fscanf(file_cam, "%lf", &cam.s);	fscanf(file_cam, "%lf", &cam.cx);
		fscanf(file_cam, "%lf", &tmp);		fscanf(file_cam, "%lf", &cam.fy);	fscanf(file_cam, "%lf", &cam.cy);
		fscanf(file_cam, "%lf", &tmp);		fscanf(file_cam, "%lf", &tmp);		fscanf(file_cam, "%lf", &tmp);

		fscanf(file_cam, "%lf", &tmp);		fscanf(file_cam, "%lf", &tmp);		fscanf(file_cam, "%lf", &tmp); // read in distortion

		Matx33d mR;
		for (ii=0;ii<3;ii++)
		{
			for (jj=0;jj<3;jj++)
			{
				fscanf(file_cam, "%lf", &mR(ii,jj));
			}
		}
		Matx31d mt;
		for (j=0;j<3;j++)
		{
			fscanf(file_cam, "%lf", &mt(j));
		}

		mR = mR.t();
		mt = -mR*mt;

// 		CString str;
// 		str.Format("E:\\Test Data\\fountain\\fountain_dense_images\\%04d.txt", i);
// 		FILE * file = fopen(str, "w");
// 		fprintf(file, "%.12f	%.12f	%.12f	%.12f	%.12f\n", cam.fx, cam.fy, cam.cx, cam.cy, cam.s);
		for (ii=0;ii<3;ii++)
		{
			for (jj=0;jj<3;jj++)
			{
				cam.R[ii*3+jj] = mR(ii,jj);
//				fprintf(file, "%.12f	", mR(ii,jj));
			}
//			fprintf(file, "\n");
		}
		for (j=0;j<3;j++)
		{
			cam.t[j] = mt(j);
//			fprintf(file, "%.12f	", mt(j));
		}
// 		fprintf(file, "\n");
// 		tmp = 0;
// 		fprintf(file, "%.12f	%.12f	%.12f	%.12f	%.12f", tmp, tmp, tmp, tmp, tmp);
// 		fclose(file);

		memset(cam.k, 0, 5*sizeof(double));
		cam.dist_type = 1;

		vAllCams.push_back(cam);
		vImgPaths.push_back(strPath);
		vClouds.push_back(cloudpts_bbox);
	}

	vector<vector<int>> vIdxSupports;
	FILE * file_spt = fopen("E:\\Test Data\\fountain\\fountain_dense_images\\support images.txt", "r");
//	FILE * file_spt = fopen("E:\\Test Data\\herzjesu\\herzjesu_dense_images\\support images.txt", "r");
	
	for (i=0;i<vAllCams.size();i++)
	{
		vector<int> vIdxSpts;
		int tmp;
//		for (j=0;j<4;j++)
		for (j=0;j<2;j++)
		{
			fscanf(file_spt, "%d	", &tmp);
			vIdxSpts.push_back(tmp);
		}
		vIdxSupports.push_back(vIdxSpts);
	}
	fclose(file_spt);

	CString strOut;
//	strOut.Format("E:\\Test Data\\fountain\\results\\");
//	strOut.Format("E:\\Test Data\\herzjesu\\results\\");
	strOut.Format("E:\\all\\");

	vector<Mat> vDepths, vHxs, vHys, vScores, vVisis;

	int wndSize = 7;
	double thresh_norm = 80;
	int nPatchMatchIter = 2;
	double factor = 0.25;
	int nRandSamp = 5;
	double img_sigma = 0.5;

	vector<Mat> vSilhouettes;

	MVDE_package_final(strOut,vAllCams,vImgPaths,vSilhouettes,vIdxSupports,vClouds,vDepths,vHxs,vHys,vScores,vVisis,
		wndSize,thresh_norm,nPatchMatchIter,factor,nRandSamp,90,0.95,1.5,0.01,img_sigma,0.01,500,5);

	return TRUE;
}

void CDeepVoidApp::OnStrechatestdata()
{
	// TODO: Add your command handler code here
	AfxBeginThread(StrechaData, this, THREAD_PRIORITY_NORMAL);
}

UINT ZhouData(LPVOID param)
{
	CDeepVoidApp * pApp = (CDeepVoidApp * )param;

	int i,j,ii,jj,k,kk;

	CString strInfo;

	vector<CString> vImgPaths;
	vector<vector<CloudPoint>> vClouds;
	vector<cam_data> vAllCams;

	// read in data
//	int nImg = 26;
	int nImg = 8;

	for (i=0;i<nImg;i++)
	{
// 		strInfo.Format("E:\\Test Data\\zhoulangming-20141208\\20141206_%03d.orinfo", i+1);
// 		FILE * file_cam = fopen(strInfo, "r");
// 		CString strPath;
// 		strPath.Format("E:\\Test Data\\zhoulangming-20141208\\20141206_%03d.bmp", i+1);

// 		strInfo.Format("E:\\Test Data\\zhoulangming-20141208\\smaller set 01\\20141206_%03d.orinfo", i+1);
// 		FILE * file_cam = fopen(strInfo, "r");
// 		CString strPath;
// 		strPath.Format("E:\\Test Data\\zhoulangming-20141208\\smaller set 01\\20141206_%03d.bmp", i+1);

// 		strInfo.Format("E:\\Test Data\\zhoulangming-20141208\\smaller set 02 subsample\\20141206_%03d.orinfo", i+1);
// 		FILE * file_cam = fopen(strInfo, "r");
// 		CString strPath;
// 		strPath.Format("E:\\Test Data\\zhoulangming-20141208\\smaller set 02 subsample\\20141206_%03d.bmp", i+1);

// 		strInfo.Format("E:\\Test Data\\zhoulangming-20141208\\smaller set 02 subsample 0.125\\20141206_%03d.orinfo", i+1);
// 		FILE * file_cam = fopen(strInfo, "r");
// 		CString strPath;
// 		strPath.Format("E:\\Test Data\\zhoulangming-20141208\\smaller set 02 subsample 0.125\\20141206_%03d.bmp", i+1);

// 		strInfo.Format("E:\\Test Data\\zhoulangming-20141213\\with sock smaller set\\A_%03d.ouinfo", i+1);
// 		FILE * file_cam = fopen(strInfo, "r");
// 		CString strPath;
// 		strPath.Format("E:\\Test Data\\zhoulangming-20141213\\with sock smaller set\\A_%03d.bmp", i+1);

// 		strInfo.Format("E:\\Test Data\\zhoulangming-20141213\\with sock smaller set subsamp 0.125\\A_%03d.ouinfo", i+1);
// 		FILE * file_cam = fopen(strInfo, "r");
// 		CString strPath;
// 		strPath.Format("E:\\Test Data\\zhoulangming-20141213\\with sock smaller set subsamp 0.125\\A_%03d.bmp", i+1);

// 		strInfo.Format("E:\\Test Data\\zhoulangming-20141213\\no sock smaller set\\A_%03d.ouinfo", i+1);
// 		FILE * file_cam = fopen(strInfo, "r");
// 		CString strPath;
// 		strPath.Format("E:\\Test Data\\zhoulangming-20141213\\no sock smaller set\\A_%03d.bmp", i+1);

		strInfo.Format("E:\\Test Data\\zhoulangming-20150207\\pickup_dist_SIFT\\%05d.orinfo", i*10);
		FILE * file_cam = fopen(strInfo, "r");
		CString strPath;
		strPath.Format("E:\\Test Data\\zhoulangming-20150207\\pickup_dist_SIFT\\%05d.jpg", i*10);

// 		strInfo.Format("E:\\Test Data\\zhoulangming-20141213\\no sock smaller set subsamp 0.125\\A_%03d.ouinfo", i+1);
// 		FILE * file_cam = fopen(strInfo, "r");
// 		CString strPath;
// 		strPath.Format("E:\\Test Data\\zhoulangming-20141213\\no sock smaller set subsamp 0.125\\A_%03d.bmp", i+1);

		double bbox_xmin, bbox_ymin, bbox_zmin;
		double bbox_xmax, bbox_ymax, bbox_zmax;

		bbox_xmin = 0; bbox_xmax = 210;
		bbox_ymin = 0; bbox_ymax = 297;
		bbox_zmin = 0; bbox_zmax = 150;

		vector<CloudPoint> cloudpts_bbox;
		CloudPoint cloudpt;
		cloudpt.m_pt.x = bbox_xmin;	cloudpt.m_pt.y = bbox_ymin;	cloudpt.m_pt.z = bbox_zmin;	cloudpts_bbox.push_back(cloudpt);
		cloudpt.m_pt.x = bbox_xmin;	cloudpt.m_pt.y = bbox_ymin;	cloudpt.m_pt.z = bbox_zmax;	cloudpts_bbox.push_back(cloudpt);
		cloudpt.m_pt.x = bbox_xmin;	cloudpt.m_pt.y = bbox_ymax;	cloudpt.m_pt.z = bbox_zmin;	cloudpts_bbox.push_back(cloudpt);
		cloudpt.m_pt.x = bbox_xmin;	cloudpt.m_pt.y = bbox_ymax;	cloudpt.m_pt.z = bbox_zmax;	cloudpts_bbox.push_back(cloudpt);

		cloudpt.m_pt.x = bbox_xmax;	cloudpt.m_pt.y = bbox_ymax;	cloudpt.m_pt.z = bbox_zmax;	cloudpts_bbox.push_back(cloudpt);
		cloudpt.m_pt.x = bbox_xmax;	cloudpt.m_pt.y = bbox_ymax;	cloudpt.m_pt.z = bbox_zmin;	cloudpts_bbox.push_back(cloudpt);
		cloudpt.m_pt.x = bbox_xmax;	cloudpt.m_pt.y = bbox_ymin;	cloudpt.m_pt.z = bbox_zmax;	cloudpts_bbox.push_back(cloudpt);
		cloudpt.m_pt.x = bbox_xmax;	cloudpt.m_pt.y = bbox_ymin;	cloudpt.m_pt.z = bbox_zmin;	cloudpts_bbox.push_back(cloudpt);

		cam_data cam;

		// intrinsic for data set 20141208
// 		double factor_subSamp = 1;
// 		cam.fx = 1304.20840*factor_subSamp; cam.fy = 1316.07080*factor_subSamp;
// 		cam.cx = 649.03447*factor_subSamp;  cam.cy = 372.84348*factor_subSamp;
// 		cam.s = 0;

		// intrinsic for data set 20141213 with sock on
// 		m_pK[0] = 1271.94; m_pK[1] = 0.0; m_pK[2] = 636.91;
// 		m_pK[3] = 0.0; m_pK[4] = 1271.07; m_pK[5] = 365.85;
// 		m_pK[6] = 0.0; m_pK[7] = 0.0; m_pK[8] = 1.0;

// 		double factor_subSamp = 1;
// 		cam.fx = 1271.94*factor_subSamp; cam.fy = 1271.07*factor_subSamp;
// 		cam.cx = 636.91*factor_subSamp;  cam.cy = 365.85*factor_subSamp;
// 		cam.s = 0;

		// intrinsic for data set 20141213 without sock on
// 		m_pK[0] = 1287.83; m_pK[1] = 0.0; m_pK[2] = 628.30;
// 		m_pK[3] = 0.0; m_pK[4] = 1292.60; m_pK[5] = 369.45;
// 		m_pK[6] = 0.0; m_pK[7] = 0.0; m_pK[8] = 1.0;

// 		double factor_subSamp = 1;
// 		cam.fx = 1287.83*factor_subSamp; cam.fy = 1292.60*factor_subSamp;
// 		cam.cx = 628.30*factor_subSamp;  cam.cy = 369.45*factor_subSamp;
// 		cam.s = 0;

		// 20150207\\pickup
		double factor_subSamp = 1;
		cam.fx = 1091.772073*factor_subSamp; cam.fy = 1091.770416*factor_subSamp;
		cam.cx = 690.121742*factor_subSamp;  cam.cy = 343.244762*factor_subSamp;
		cam.s = 0;

		Matx33d mR;
		for (ii=0;ii<3;ii++)
		{
			for (jj=0;jj<3;jj++)
			{
				fscanf(file_cam, "%lf", &mR(ii,jj));
//				fscanf(file_cam, "%lf,", &mR(ii,jj));
			}
		}
		Matx31d mt;
		for (j=0;j<3;j++)
		{
			fscanf(file_cam, "%lf", &mt(j));
//			fscanf(file_cam, "%lf,", &mt(j));
		}

		for (ii=0;ii<3;ii++)
		{
			for (jj=0;jj<3;jj++)
			{
				cam.R[ii*3+jj] = mR(ii,jj);
			}
		}
		for (j=0;j<3;j++)
		{
			cam.t[j] = mt(j);
		}

		memset(cam.k, 0, 5*sizeof(double));
		cam.dist_type = 1;

		vAllCams.push_back(cam);
		vImgPaths.push_back(strPath);
		vClouds.push_back(cloudpts_bbox);

		fclose(file_cam);
	}

	vector<vector<int>> vIdxSupports;
//	FILE * file_spt = fopen("E:\\Test Data\\zhoulangming-20141208\\support images.txt", "r");
//	FILE * file_spt = fopen("E:\\Test Data\\zhoulangming-20141208\\smaller set 01\\support images.txt", "r");
//	FILE * file_spt = fopen("E:\\Test Data\\zhoulangming-20141208\\smaller set 02 subsample\\support images.txt", "r");
//	FILE * file_spt = fopen("E:\\Test Data\\zhoulangming-20141208\\smaller set 02 subsample 0.125\\support images.txt", "r");
//	FILE * file_spt = fopen("E:\\Test Data\\zhoulangming-20141213\\with sock smaller set subsamp 0.125\\support images.txt", "r");
//	FILE * file_spt = fopen("E:\\Test Data\\zhoulangming-20141213\\no sock smaller set subsamp 0.125\\support images.txt", "r");
	FILE * file_spt = fopen("E:\\Test Data\\zhoulangming-20150207\\support images.txt", "r");

	for (i=0;i<vAllCams.size();i++)
	{
		vector<int> vIdxSpts;
		int tmp;
//		for (j=0;j<4;j++)
		for (j=0;j<2;j++)
		{
			fscanf(file_spt, "%d	", &tmp);
			vIdxSpts.push_back(tmp);
		}
		vIdxSupports.push_back(vIdxSpts);
	}
	fclose(file_spt);

	CString strOut;
	strOut.Format("E:\\all\\");

	vector<Mat> vDepths, vHxs, vHys, vScores, vVisis;

	int wndSize = 5;
//	int wndSize = 7;
	double thresh_norm = 80;
	int nPatchMatchIter = 2;
	double factor = 0.25;
	int nRandSamp = 5;
	double img_sigma = 0.5;

	vector<Mat> vSilhouettes;

// 	MVDE_package_final(strOut,vAllCams,vImgPaths,vSilhouettes,vIdxSupports,vClouds,vDepths,vHxs,vHys,vScores,vVisis,
// 		wndSize,thresh_norm,nPatchMatchIter,factor,nRandSamp,90,0.95,1.5,0.01,img_sigma,0.1/*0.01*/,50/*300*/,5,1.0E-8,1.0E-6,false);

// 	MVDE_package_141209(strOut,vAllCams,vImgPaths,vSilhouettes,vIdxSupports,vClouds,vDepths,vHxs,vHys,vScores,vVisis,
// 		wndSize,thresh_norm,nPatchMatchIter,factor,nRandSamp,90,0.95,1.5,0.01,img_sigma,0.1,100,5);

	MVDE_package_150206(strOut,vAllCams,vImgPaths,vSilhouettes,vIdxSupports,vClouds[0],vDepths,vHxs,vHys,vScores,vVisis,
		wndSize,thresh_norm,nPatchMatchIter,factor,nRandSamp,90,0.95,1.5,0.01,img_sigma,4,3,0.01,0.02,5);

	return TRUE;
}

void CDeepVoidApp::OnZhoudata()
{
	// TODO: Add your command handler code here
	AfxBeginThread(ZhouData, this, THREAD_PRIORITY_NORMAL);
}


void CDeepVoidApp::OnFeaturematching()
{
	// TODO: Add your command handler code here
	/*int i,j;

	// read in the images
	Mat img0 = imread("E:\\Test Data\\zhoulangming-20141213\\no sock smaller set\\A_003.bmp", CV_LOAD_IMAGE_UNCHANGED);
	Mat img1 = imread("E:\\Test Data\\zhoulangming-20141213\\no sock smaller set\\A_004.bmp", CV_LOAD_IMAGE_UNCHANGED);
// 	Mat img0 = imread("E:\\Test Data\\xuyuhua\\1L.jpg", CV_LOAD_IMAGE_UNCHANGED);
// 	Mat img1 = imread("E:\\Test Data\\xuyuhua\\1R.jpg", CV_LOAD_IMAGE_UNCHANGED);
// 	Mat img0 = imread("E:\\Test Data\\liudata\\horse1.jpg", CV_LOAD_IMAGE_UNCHANGED);
// 	Mat img1 = imread("E:\\Test Data\\liudata\\horse6.jpg", CV_LOAD_IMAGE_UNCHANGED);

//	vector<KeyPoint> keypoints0, keypoints1;
	Features feats0, feats1;

	Mat mask0,mask1;

//	SIFT sift0, sift1;
	SURF sift0, sift1;

	// extract features
	sift0(img0, mask0, feats0.key_points, feats0.descriptors);
	sift1(img1, mask1, feats1.key_points, feats1.descriptors);

	int nFeat0 = feats0.key_points.size();
	int nFeat1 = feats1.key_points.size();

	KeyPoint kpt_pre;
	kpt_pre.pt.x = -1000;	kpt_pre.pt.y = -1000;
	kpt_pre.size = -1000;
	int idx_imgPt;
	for (j=0;j<feats0.key_points.size();j++)
	{
		feats0.tracks.push_back(-1);

		KeyPoint kpt_cur = feats0.key_points[j];
		if (fabs(kpt_cur.pt.x-kpt_pre.pt.x)<1.0E-12 && fabs(kpt_cur.pt.y-kpt_pre.pt.y)<1.0E-12
			&& fabs(kpt_cur.size - kpt_pre.size)<1.0E-12)
		{
			// indicating that current keypoint is identical to the previous keypoint
			feats0.idx_pt.push_back(idx_imgPt);
		} 
		else
		{
			feats0.idx_pt.push_back(j);
			idx_imgPt = j;
		}

		kpt_pre.pt.x = kpt_cur.pt.x;
		kpt_pre.pt.y = kpt_cur.pt.y;
		kpt_pre.size = kpt_cur.size;
	}

	kpt_pre.pt.x = -1000;	kpt_pre.pt.y = -1000;
	kpt_pre.size = -1000;
	for (j=0;j<feats1.key_points.size();j++)
	{
		feats1.tracks.push_back(-1);

		KeyPoint kpt_cur = feats1.key_points[j];
		if (fabs(kpt_cur.pt.x-kpt_pre.pt.x)<1.0E-12 && fabs(kpt_cur.pt.y-kpt_pre.pt.y)<1.0E-12
			&& fabs(kpt_cur.size - kpt_pre.size)<1.0E-12)
		{
			// indicating that current keypoint is identical to the previous keypoint
			feats1.idx_pt.push_back(idx_imgPt);
		} 
		else
		{
			feats1.idx_pt.push_back(j);
			idx_imgPt = j;
		}

		kpt_pre.pt.x = kpt_cur.pt.x;
		kpt_pre.pt.y = kpt_cur.pt.y;
		kpt_pre.size = kpt_cur.size;
	}

	Matx33d mF; vector<DMatch> matches_final;

	Get_F_Matches_knn(feats0,feats1,mF,matches_final,true,0.5,0.0,3.0,0.99,10);

	Mat disp_matches_better;
	drawMatches(img0, feats0.key_points, img1, feats1.key_points, matches_final, disp_matches_better);
	imwrite("E:\\matches\\matches_better.bmp", disp_matches_better);

	Get_F_Matches(feats0,feats1,mF,matches_final,3.0,0.99,10);

	SaveFeatures2File("E:\\matches\\features0.txt", feats0);
	SaveFeatures2File("E:\\matches\\features1.txt", feats1);
	
	// do flann matching
	FlannBasedMatcher matcher_flann;

	vector<DMatch> matches_raw;
//	vector<vector<DMatch>> matches_kNN;

	// ensure that the first i.e. the reference image has less features than the second i.e. matching image does
	if (nFeat0<=nFeat1)
	{
		matcher_flann.match(feats0.descriptors, feats1.descriptors, matches_raw);
	} 
	else
	{
		matcher_flann.match(feats1.descriptors, feats0.descriptors, matches_raw);

		for (i=0;i<matches_raw.size();i++)
		{
			// zhaokunz, 20150106, reverse the index
			int tmp = matches_raw[i].queryIdx;
			matches_raw[i].queryIdx = matches_raw[i].trainIdx;
			matches_raw[i].trainIdx = tmp;
		}
	}
//	matcher_flann.knnMatch(des0, des1, matches_kNN, 3);

	// use the image point index, especially for SIFT features
	vector<DMatch> matches_raw_idxImgPt;
	
	for (i=0;i<matches_raw.size();i++)
	{
		// zhaokunz, 20140324, use the unique image point index
		DMatch match_idxImgPt = matches_raw[i];
		match_idxImgPt.queryIdx = feats0.idx_pt[matches_raw[i].queryIdx];
		match_idxImgPt.trainIdx = feats1.idx_pt[matches_raw[i].trainIdx];

		matches_raw_idxImgPt.push_back(match_idxImgPt);
	}
	
	// then detect and delete those identical matches
	vector<int> vIdentical(matches_raw_idxImgPt.size(), 0);
	for (i=0;i<matches_raw_idxImgPt.size();i++)
	{
		if (vIdentical[i])
		{
			continue;
		}

		DMatch m1 = matches_raw_idxImgPt[i];

		for (j=i+1;j<matches_raw_idxImgPt.size();j++)
		{
			DMatch m2 = matches_raw_idxImgPt[j];

			if (m1.queryIdx == m2.queryIdx && m1.trainIdx == m2.trainIdx)
			{
				// found a identical match
				vIdentical[j] = 1;
			}
		}
	}

	vector<DMatch> matches_noIdentical;
	for (i=0;i<matches_raw_idxImgPt.size();i++) 
	{
		if (!vIdentical[i])
		{
			matches_noIdentical.push_back(matches_raw_idxImgPt[i]);
		}
	}

	// enforce one-to-one constraint
	vector<int> vCancel(matches_noIdentical.size(), 0);
	for (i=0;i<matches_noIdentical.size();i++)
	{
		DMatch m1 = matches_noIdentical[i];

		for (j=i+1;j<matches_noIdentical.size();j++)
		{
			DMatch m2 = matches_noIdentical[j];

			if (m1.queryIdx != m2.queryIdx && m1.trainIdx != m2.trainIdx)
			{
				continue;
			}

			vCancel[i] = 1;
			vCancel[j] = 1;
		}
	}

	vector<DMatch> matches_one2one;
	for (i=0;i<matches_noIdentical.size();i++) 
	{
		if (!vCancel[i])
		{
			matches_one2one.push_back(matches_noIdentical[i]);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	
	double max_dist = 0; double min_dist = 5000;
	//-- Quick calculation of max and min distances between keypoints
	for(i=0;i<matches_raw.size();i++)
	{
		double dist = matches_raw[i].distance;
		if(dist<min_dist)
			min_dist = dist;
		if(dist>max_dist)
			max_dist = dist;
	}

	//-- Draw only "good" matches (i.e. whose distance is less than param1*min_dist )
	//-- PS.- radiusMatch can also be used here.
	vector<DMatch> matches_good;
	for(i=0;i<matches_raw.size();i++)
	{
		if(matches_raw[i].distance<20*min_dist)
		{
			matches_good.push_back(matches_raw[i]);
		}
	}

	// RANSAC //////////////////////////////////////////////////////////////////////////
	vector<Point2f> points0(matches_one2one.size());
	vector<Point2f> points1(matches_one2one.size());

	// initialize the points here ... 
	for(i=0;i<matches_one2one.size();i++)
	{
		points0[i] = feats0.key_points[matches_one2one[i].queryIdx].pt;
		points1[i] = feats1.key_points[matches_one2one[i].trainIdx].pt;
	}

	// normalize the image points
// 	vector<Point2f> points0_t, points1_t;
// 	Matx33d mT0 = NormalizePts_2D(points0,points0_t);
// 	Matx33d mT1 = NormalizePts_2D(points1,points1_t);

	vector<uchar> status;

	Matx33d fundamental_matrix;
	
//	fundamental_matrix = findFundamentalMat(points0, points1, status, FM_RANSAC);
//	fundamental_matrix = findFundamentalMat(points0_t, points1_t, status, FM_RANSAC, 0.1);

	vector<DMatch> matches_RANSAC;
	vector<Point2d> vImgPts0, vImgPts1;

	for (i=0;i<matches_one2one.size();i++)
	{
		if (status[i])
		{
			matches_RANSAC.push_back(matches_one2one[i]);

			Point2d pt0,pt1;
			pt0.x = points0[i].x; pt0.y = points0[i].y;
			pt1.x = points1[i].x; pt1.y = points1[i].y;
			vImgPts0.push_back(pt0);
			vImgPts1.push_back(pt1);
		}
	}

// 	Matx33d mF; vector<DMatch> matches_final;
// 	Get_F_Matches(feats0,feats1,mF,matches_final,3.0,0.99,10);

	//////////////////////////////////////////////////////////////////////////

	// RANSAC //////////////////////////////////////////////////////////////////////////
// 	vector<Point2f> points0(matches_good.size());
// 	vector<Point2f> points1(matches_good.size());
// 
// 	// initialize the points here ...
// 	for(i=0;i<matches_good.size();i++)
// 	{
// 		points0[i] = feats0.key_points[matches_good[i].queryIdx].pt;
// 		points1[i] = feats1.key_points[matches_good[i].trainIdx].pt;
// 	}
// 
// 	vector<uchar> status;
// 
// 	Matx33d fundamental_matrix;
// 
// 	fundamental_matrix = findFundamentalMat(points0, points1, FM_RANSAC, 3, 0.99, status);
// 
// 	vector<DMatch> matches_RANSAC;
// 
// 	for (i=0;i<matches_good.size();i++)
// 	{
// 		if (status[i])
// 		{
// 			matches_RANSAC.push_back(matches_good[i]);
// 		}
// 	}
	//////////////////////////////////////////////////////////////////////////

	Mat disp_matches_raw, disp_matches_good, disp_matches_one2one, disp_matches_RANSAC, disp_matches_final;
	drawMatches(img0, feats0.key_points, img1, feats1.key_points, matches_raw, disp_matches_raw);
	drawMatches(img0, feats0.key_points, img1, feats1.key_points, matches_good, disp_matches_good);
	drawMatches(img0, feats0.key_points, img1, feats1.key_points, matches_one2one, disp_matches_one2one);
	drawMatches(img0, feats0.key_points, img1, feats1.key_points, matches_RANSAC, disp_matches_RANSAC);
	drawMatches(img0, feats0.key_points, img1, feats1.key_points, matches_final, disp_matches_final);

	imwrite("E:\\matches\\matches_raw.bmp", disp_matches_raw);
	imwrite("E:\\matches\\matches_good.bmp", disp_matches_good);
	imwrite("E:\\matches\\matches_one2one.bmp", disp_matches_one2one);
	imwrite("E:\\matches\\matches_RANSAC.bmp", disp_matches_RANSAC);
	imwrite("E:\\matches\\matches_final.bmp", disp_matches_final);*/
}


void CDeepVoidApp::OnXudata()
{
	// TODO: Add your command handler code here

	// read in the images
	Mat img0 = imread("E:\\Test Data\\xuyuhua\\5L.jpg", CV_LOAD_IMAGE_UNCHANGED);
	Mat img1 = imread("E:\\Test Data\\xuyuhua\\5R.jpg", CV_LOAD_IMAGE_UNCHANGED);

	Matx33d mK_l, mK_r, mR_l, mR_r;
	Matx31d mt_l, mt_r;

	// read in the parameters
	FILE * file_K_l = fopen("E:\\Test Data\\xuyuhua\\K_left.txt","r");
	FILE * file_K_r = fopen("E:\\Test Data\\xuyuhua\\K_right.txt","r");
	FILE * file_R = fopen("E:\\Test Data\\xuyuhua\\R.txt","r");
	FILE * file_t = fopen("E:\\Test Data\\xuyuhua\\t.txt","r");

	for (int i=0;i<3;++i)
	{
		for (int j=0;j<3;++j)
		{
			fscanf(file_K_l, "%lf", &mK_l(i,j));
			fscanf(file_K_r, "%lf", &mK_r(i,j));
			fscanf(file_R, "%lf", &mR_r(i,j));
		}
	}
	mR_l(0,0)=mR_l(1,1)=mR_l(2,2)=1;

	for (int i=0;i<3;++i)
	{
		fscanf(file_t, "%lf", &mt_r(i));
	}
	
	vector<Mat> vImgs_spt;
	vector<Matx33d> vKs_spt, vRs_spt;
	vector<Matx31d> vts_spt;
	vector<CloudPoint> vClouds;

	vImgs_spt.push_back(img1);
	vKs_spt.push_back(mK_r);
	vRs_spt.push_back(mR_r);
	vts_spt.push_back(mt_r);

	CloudPoint pt3d_n, pt3d_f;
	pt3d_n.m_pt.x = 0;
	pt3d_n.m_pt.y = 0;
	pt3d_n.m_pt.z = 4100;

	pt3d_f.m_pt.x = 0;
	pt3d_f.m_pt.y = 0;
	pt3d_f.m_pt.z = 4800;

	vClouds.push_back(pt3d_n);
	vClouds.push_back(pt3d_f);

	CString path_output;
	path_output.Format("E:\\all\\");

	int size = 7;
	double angLimit = 80;
	int nPatchMatchIter = 4;
	double factor = 0.25;
	int nRandSamp = 5;
	double img_sigma = 0.5;
	int nLevel = 3;
	int idxOutLevel = 2;

	int nPix = pow(2.0,idxOutLevel);
	double nPix_1 = 1.0/nPix;

	// start
	Mat mDepth_final, mHx_final, mHy_final, mScore_final, mVisi_final, img0_outLevel;
	SurfaceMapEstimation_oneImg_Pyramid(mK_l,mR_l,mt_l,img0,vKs_spt,vRs_spt,vts_spt,vImgs_spt,vClouds,
		path_output,mDepth_final,mHx_final,mHy_final,mScore_final,mVisi_final,img0_outLevel,size,angLimit,nPatchMatchIter,factor,nRandSamp,
		90,0.95,1.5,0.01,img_sigma,nLevel,idxOutLevel,5);

	// 输出结果至指定文件夹
	CString str;
	str.Format("left depth map at pyramid level %03d.bmp", idxOutLevel);
	SaveParaField2Img(path_output+str, mDepth_final);

	str.Format("left score map at pyramid level %03d.bmp", idxOutLevel);
	SaveScoreField2Img(path_output+str, mScore_final);

	Matx33d mK0_samp;
	mK0_samp(0,0) = mK_l(0,0)*nPix_1;
	mK0_samp(1,1) = mK_l(1,1)*nPix_1;
	mK0_samp(0,2) = mK_l(0,2)*nPix_1;
	mK0_samp(1,2) = mK_l(1,2)*nPix_1;
	mK0_samp(0,1) = mK_l(0,1);
	mK0_samp(2,2) = 1;

	double fx0l_1 = 1.0/mK0_samp(0,0);
	double fy0l_1 = 1.0/mK0_samp(1,1);

	Mat mNormColor;
	GetNormColorField(mK0_samp,mR_l,mt_l,fx0l_1,fy0l_1,mDepth_final,mHx_final,mHy_final,mNormColor);
	str.Format("left normal map at pyramid level %03d.bmp", idxOutLevel);
	imwrite((path_output+str).GetBuffer(), mNormColor);

	str.Format("left point cloud at pyramid level %03d.txt", idxOutLevel);
	OutputPointCloud(path_output+str,mK0_samp,mR_l,mt_l,fx0l_1,fy0l_1,img0_outLevel,
		mDepth_final,mHx_final,mHy_final,mScore_final);

	str.Format("left image down-sampled at pyramid level %03d.bmp", idxOutLevel);
	imwrite((path_output+str).GetBuffer(), img0_outLevel);

	// output to files
// 	str.Format("%03d depth map at pyramid level %03d.txt", k, idxOutLevel);
// 	FILE * file_depth = fopen(path_output+str, "w");
// 	str.Format("%03d hx map at pyramid level %03d.txt", k, idxOutLevel);
// 	FILE * file_hx = fopen(path_output+str, "w");
// 	str.Format("%03d hy map at pyramid level %03d.txt", k, idxOutLevel);
// 	FILE * file_hy = fopen(path_output+str, "w");
// 	str.Format("%03d score map at pyramid level %03d.txt", k, idxOutLevel);
// 	FILE * file_score = fopen(path_output+str, "w");
// 	str.Format("%03d visi map at pyramid level %03d.txt", k, idxOutLevel);
// 	FILE * file_visi = fopen(path_output+str, "w");
// 
// 	// at the same time evaluate all parameters
// 	for (i=0;i<img0_outLevel.rows;i++)
// 	{
// 		for (j=0;j<img0_outLevel.cols;j++)
// 		{
// 			fprintf(file_depth, "%.16f	", mDepth_final.at<float>(i,j));
// 			fprintf(file_hx, "%.16f	", mHx_final.at<float>(i,j));
// 			fprintf(file_hy, "%.16f	", mHy_final.at<float>(i,j));
// 			fprintf(file_score, "%.16f	", mScore_final.at<float>(i,j));
// 			fprintf(file_visi, "%d	", mVisi_final.at<uchar>(i,j));
// 		}
// 		fprintf(file_depth, "\n");
// 		fprintf(file_hx, "\n");
// 		fprintf(file_hy, "\n");
// 		fprintf(file_score, "\n");
// 		fprintf(file_visi, "\n");
// 	}
// 	fclose(file_depth);
// 	fclose(file_hx);
// 	fclose(file_hy);
// 	fclose(file_score);
// 	fclose(file_visi);
}


void CDeepVoidApp::OnStereo()
{
	// TODO: Add your command handler code here
	CDlgStereoFiles dlgFiles;
	dlgFiles.m_pathImg0 = m_pathStereoImg0;
	dlgFiles.m_pathImg1 = m_pathStereoImg1;
	dlgFiles.m_pathParam0 = m_pathStereoParam0;
	dlgFiles.m_pathParam1 = m_pathStereoParam1;
	dlgFiles.m_nPixDownSample = m_nPixDownSample;
	dlgFiles.m_bSetZeroDisparity = m_idxSetZeroDisparity;
	dlgFiles.m_idxRectifyScaling = m_idxRectifyScaling;
	dlgFiles.m_minDisparity = m_minDisparity;
	dlgFiles.m_maxDisparity = m_maxDisparity;

	// users should make sure that the 1st stereo image is the left or top one, 
	// and the 2nd is the right or bottom one respectively
	if (IDOK==dlgFiles.DoModal())
	{
		m_pathStereoImg0 = dlgFiles.m_pathImg0;
		m_pathStereoImg1 = dlgFiles.m_pathImg1;
		m_pathStereoParam0 = dlgFiles.m_pathParam0;
		m_pathStereoParam1 = dlgFiles.m_pathParam1;
		m_nPixDownSample = dlgFiles.m_nPixDownSample;
		m_idxSetZeroDisparity = dlgFiles.m_bSetZeroDisparity;
		m_idxRectifyScaling = dlgFiles.m_idxRectifyScaling;
		m_minDisparity = dlgFiles.m_minDisparity;
		m_maxDisparity = dlgFiles.m_maxDisparity;

		bool bGivenParams = true;

		if (m_pathStereoParam0 == "" || m_pathStereoParam1 == "")
		{
			// 说明没有输入参数，有时候输入的图像直接就是矫正完了的，可以直接进行后续的stereo密集匹配，这时就不用输入图像参数
			bGivenParams = false; 
		}

		double color_factor = 255.0/(m_maxDisparity-m_minDisparity);

		// read in stereo images //////////////////////////////////////////////////////////////////////////
		Mat img0, img1;
		if (!(img0 = imread(m_pathStereoImg0.GetBuffer(), CV_LOAD_IMAGE_UNCHANGED)).data)
		{
			AfxMessageBox("fail to read in image0");
			return;
		}
		if (!(img1 = imread(m_pathStereoImg1.GetBuffer(), CV_LOAD_IMAGE_UNCHANGED)).data)
		{
			AfxMessageBox("fail to read in image1");
			return;
		}

		// 没有给定图像对的参数，这时输入的图像就是极线矫正完了的图像
		// 直接进行密集匹配得到视差图，但不反投影生成点云
		if (!bGivenParams)
		{
			// 20151014 原图太大，就截取一下 /////////////////////////////////////////
// 			Mat img0_roi(img0, Range(160,1400), Range(180,1800));
// 			Mat img1_roi(img1, Range(160,1400), Range(180,1800));
// 
// 			img0 = img0_roi.clone();
// 			img1 = img1_roi.clone();
			//////////////////////////////////////////////////////////////////////////

			// original gray image
			

			Mat img0_gray, img1_gray;
// 			try
// 			{
// 				cvtColor(img0, img0_gray, CV_RGB2GRAY);
// 				cvtColor(img1, img1_gray, CV_RGB2GRAY);
// 			}
// 			catch (cv::Exception & e)
// 			{
// 				CString str;
// 				str = e.msg.c_str();
// 				AfxMessageBox(str);
// 			}

			if (img0.channels()<3)
			{
				img0_gray = img0.clone();
			}
			else
			{
				cvtColor(img0, img0_gray, CV_RGB2GRAY);
			}

			if (img1.channels()<3)
			{
				img1_gray = img1.clone();
			} 
			else
			{
				cvtColor(img1, img1_gray, CV_RGB2GRAY);
			}

			imwrite("E:\\results\\img0.jpg", img0);
			imwrite("E:\\results\\img1.jpg", img1);

			// choose different stereo methods //////////////////////////////////////////////////////////////////////////
			CDlgStereoMethods dlgMethods;
			dlgMethods.m_idxStereoMethod = m_idxStereoMethod;

			if (IDOK==dlgMethods.DoModal())
			{
				m_idxStereoMethod = dlgMethods.m_idxStereoMethod;

				if (0==m_idxStereoMethod) // SGM Zhaokun Zhu's version
				{
					CDlgSGMZZKSettings dlgSettings;
					dlgSettings.m_idxDSItype = m_idxDSItype;
					dlgSettings.m_idxPaths = m_idxPaths_ZZKSGM;
					dlgSettings.m_idxSubpixel = m_idxSubpixel_ZZKSGM;
					dlgSettings.m_P1 = m_P1_ZZKSGM;
					dlgSettings.m_P2 = m_P2_ZZKSGM;
					dlgSettings.m_threshConstCheck = m_threshConstCheck_ZZKSGM;

					if (IDOK==dlgSettings.DoModal())
					{
						m_idxDSItype = dlgSettings.m_idxDSItype;
						m_idxPaths_ZZKSGM = dlgSettings.m_idxPaths;
						m_idxSubpixel_ZZKSGM = dlgSettings.m_idxSubpixel;
						m_P1_ZZKSGM = dlgSettings.m_P1;
						m_P2_ZZKSGM = dlgSettings.m_P2;
						m_threshConstCheck_ZZKSGM = dlgSettings.m_threshConstCheck;

						SGM_PATHS paths;
						if (0==m_idxPaths_ZZKSGM)
						{
							paths = SGM_PATHS_8;
						} 
						else
						{
							paths = SGM_PATHS_16;
						}

						bool bSubPixRefine = true;
						if (m_idxSubpixel_ZZKSGM!=0)
						{
							bSubPixRefine = false;
						}

						Mat mDI;
						/*SemiGlobalMatching(img0_gray,img1_gray,m_minDisparity,m_maxDisparity,m_P1_ZZKSGM,m_P2_ZZKSGM,mDI,
							false,paths,m_threshConstCheck_ZZKSGM,bSubPixRefine);*/

						SemiGlobalMatching_CUDA(img0_gray, img1_gray, m_minDisparity, m_maxDisparity, m_P1_ZZKSGM, m_P2_ZZKSGM, mDI,
							false, paths, m_threshConstCheck_ZZKSGM, bSubPixRefine/*, 5, 5, 1, 32*/);

						SaveMat2File_float("E:\\results\\disparity by SGM (ZZK).txt",mDI);
					}
				}
				else if (1==m_idxStereoMethod) // SGM OpenCV version
				{

				} 
				else
				{

				}
			}

			return;
		}

		// read in parameters //////////////////////////////////////////////////////////////////////////
		FILE * file0 = fopen(m_pathStereoParam0, "r");
		if (!file0)
		{
			AfxMessageBox("fail to read in parameters of image0");
			return;
		}
		FILE * file1 = fopen(m_pathStereoParam1, "r");
		if (!file1)
		{
			AfxMessageBox("fail to read in parameters of image1");
			return;
		}

		double fx0,fx1,fy0,fy1,cx0,cx1,cy0,cy1,s0,s1;
		fscanf(file0, "%lf %lf %lf %lf %lf", &fx0, &fy0, &cx0, &cy0, &s0);
		fscanf(file1, "%lf %lf %lf %lf %lf", &fx1, &fy1, &cx1, &cy1, &s1);

		Matx33d mK0,mK1;
		mK0(0,0)=fx0; mK0(1,1)=fy0; mK0(0,1)=s0; mK0(0,2)=cx0; mK0(1,2)=cy0; mK0(2,2)=1;
		mK1(0,0)=fx1; mK1(1,1)=fy1; mK1(0,1)=s1; mK1(0,2)=cx1; mK1(1,2)=cy1; mK1(2,2)=1;

		Matx33d mR0,mR1;
		for (int i=0;i<3;++i)
		{
			for (int j=0;j<3;++j)
			{
				fscanf(file0, "%lf", &mR0(i,j));
				fscanf(file1, "%lf", &mR1(i,j));
			}
		}

		Matx31d mt0,mt1;
		fscanf(file0, "%lf %lf %lf", &mt0(0), &mt0(1), &mt0(2));
		fscanf(file1, "%lf %lf %lf", &mt1(0), &mt1(1), &mt1(2));

		Matx<double,5,1> mdist0,mdist1;
		for (int i=0;i<5;++i)
		{
			fscanf(file0, "%lf", &mdist0(i));
			fscanf(file1, "%lf", &mdist1(i));
		}

		fclose(file0);
		fclose(file1);

		// epipolar resampling -- stereo rectification //////////////////////////////////////////////////////////////////////////
		cv::Size size_old;	// original image size
		size_old.width  = img0.cols;
		size_old.height = img0.rows;

		cv::Size size_new;	// size of rectified stereo images, if (0,0) the rectified images have the original size
		size_new.width  = size_old.width/m_nPixDownSample;
		size_new.height = size_old.height/m_nPixDownSample;

		Matx33d mR01 = mR1*mR0.t(); // relative rotation
		Matx31d mt01 = -mR01*mt0+mt1; // relative translation vector

		int flag_stereoRectify;
		if (0==m_idxSetZeroDisparity)
		{
			flag_stereoRectify = CV_CALIB_ZERO_DISPARITY;
		} 
		else
		{
			flag_stereoRectify = 0;
		}

		double alpha_stereoRectify;
		if (0==m_idxRectifyScaling)
		{
			alpha_stereoRectify = 0;
		} 
		else if (1==m_idxRectifyScaling)
		{
			alpha_stereoRectify = 1;
		}
		else
		{
			alpha_stereoRectify = -1;
		}

		Matx33d mR0_new, mR1_new;
		Matx34d mP0_new, mP1_new;
		Matx44d mQ;

		// Computes rectification transforms for each calibrated stereo image
		stereoRectify(mK0, mdist0, mK1, mdist1, size_old, mR01, mt01, mR0_new, mR1_new, mP0_new, mP1_new, mQ,
			flag_stereoRectify, alpha_stereoRectify, size_new);

		// determine whether it's a horizontal stereo or a vertical stereo
		bool isVerticalStereo = fabs(mP1_new(1, 3)) > fabs(mP1_new(0, 3));

		Mat rmap[2][2];
		// Computes the undistortion and rectification transformation map
		initUndistortRectifyMap(mK0, mdist0, mR0_new, mP0_new, size_new, CV_32FC1, rmap[0][0], rmap[0][1]);
		initUndistortRectifyMap(mK1, mdist1, mR1_new, mP1_new, size_new, CV_32FC1, rmap[1][0], rmap[1][1]);

		Mat rimg0,rimg1;
		// Applies a generic geometrical transformation to an image
		remap(img0, rimg0, rmap[0][0], rmap[0][1], CV_INTER_LINEAR /*CV_INTER_CUBIC*/);
		remap(img1, rimg1, rmap[1][0], rmap[1][1], CV_INTER_LINEAR /*CV_INTER_CUBIC*/);

		// rectified gray image
		Mat rimg0_gray, rimg1_gray;
		cvtColor(rimg0, rimg0_gray, CV_RGB2GRAY);
		cvtColor(rimg1, rimg1_gray, CV_RGB2GRAY);

		// extract the new camera matrices of the rectified stereo images
		Matx33d mK0_new, mK1_new, mI;
		for (int i=0;i<3;i++)
		{
			for (int j=0;j<3;j++)
			{
				mK0_new(i,j) = mP0_new(i,j);
				mK1_new(i,j) = mP1_new(i,j);
			}
		}
		mI(0,0)=mI(1,1)=mI(2,2)=1; // identity matrix, representing the rotation matrices of the rectified stereo images
		double f_new = mK0_new(0,0);

		// extract the new relative translation vector
		Matx31d mt0_new, mt1_new;
		if (!isVerticalStereo)
		{
			double tx = mP1_new(0,3)/f_new;
			mt1_new(0) = tx;
		} 
		else
		{
			double ty = mP1_new(1,3)/f_new;
			mt1_new(1) = ty;
		}

		// determine the min and max depths
		Matx41d XYZW;
		XYZW(0) = mK0_new(0,2); XYZW(1) = mK0_new(1,2); XYZW(2) = m_minDisparity; XYZW(3) = 1;
		XYZW = mQ*XYZW;
		double W_1 = 1.0/XYZW(3);
		CloudPoint pt3d_n, pt3d_f;
		pt3d_f.m_pt.x = XYZW(0)*W_1; pt3d_f.m_pt.y = XYZW(1)*W_1; pt3d_f.m_pt.z = XYZW(2)*W_1;

		XYZW(0) = mK0_new(0,2); XYZW(1) = mK0_new(1,2); XYZW(2) = m_maxDisparity; XYZW(3) = 1;
		XYZW = mQ*XYZW;
		W_1 = 1.0/XYZW(3);
		pt3d_n.m_pt.x = XYZW(0)*W_1; pt3d_n.m_pt.y = XYZW(1)*W_1; pt3d_n.m_pt.z = XYZW(2)*W_1;

		imwrite("E:\\results\\rimg0.jpg", rimg0);
		imwrite("E:\\results\\rimg1.jpg", rimg1);

		// choose different stereo methods //////////////////////////////////////////////////////////////////////////
		CDlgStereoMethods dlgMethods;
		dlgMethods.m_idxStereoMethod = m_idxStereoMethod;

		if (IDOK==dlgMethods.DoModal())
		{
			m_idxStereoMethod = dlgMethods.m_idxStereoMethod;

			if (0==m_idxStereoMethod) // SGM Zhaokun Zhu's version
			{
				CDlgSGMZZKSettings dlgSettings;
				dlgSettings.m_idxDSItype = m_idxDSItype;
				dlgSettings.m_idxPaths = m_idxPaths_ZZKSGM;
				dlgSettings.m_idxSubpixel = m_idxSubpixel_ZZKSGM;
				dlgSettings.m_P1 = m_P1_ZZKSGM;
				dlgSettings.m_P2 = m_P2_ZZKSGM;
				dlgSettings.m_threshConstCheck = m_threshConstCheck_ZZKSGM;

				if (IDOK==dlgSettings.DoModal())
				{
					m_idxDSItype = dlgSettings.m_idxDSItype;
					m_idxPaths_ZZKSGM = dlgSettings.m_idxPaths;
					m_idxSubpixel_ZZKSGM = dlgSettings.m_idxSubpixel;
					m_P1_ZZKSGM = dlgSettings.m_P1;
					m_P2_ZZKSGM = dlgSettings.m_P2;
					m_threshConstCheck_ZZKSGM = dlgSettings.m_threshConstCheck;

					SGM_PATHS paths;
					if (0==m_idxPaths_ZZKSGM)
					{
						paths = SGM_PATHS_8;
					} 
					else
					{
						paths = SGM_PATHS_16;
					}

					bool bSubPixRefine = true;
					if (m_idxSubpixel_ZZKSGM!=0)
					{
						bSubPixRefine = false;
					}

					Mat mDI;

					m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo("SGM starts!");

 					/*SemiGlobalMatching(rimg0_gray,rimg1_gray,m_minDisparity,m_maxDisparity,m_P1_ZZKSGM,m_P2_ZZKSGM,mDI,
 						isVerticalStereo,paths,m_threshConstCheck_ZZKSGM,bSubPixRefine);*/

					// CUDA version
					SemiGlobalMatching_CUDA(rimg0_gray, rimg1_gray, m_minDisparity, m_maxDisparity, m_P1_ZZKSGM, m_P2_ZZKSGM, mDI,
						isVerticalStereo, paths, m_threshConstCheck_ZZKSGM, bSubPixRefine/*, 5, 5, 1, 32*/);

					m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo("SGM ends!");

//					FILE * fp = fopen("C:\\Users\\DeepVoid\\Desktop\\point cloud by SGM (ZZK).txt", "w");
					FILE * fp = fopen("E:\\results\\point cloud by SGM (ZZK).txt", "w");
					for(int y = 0; y < mDI.rows; y++)
					{
						for(int x = 0; x < mDI.cols; x++)
						{
							double d = mDI.at<float>(y,x);
//							if (d<=0)
							if (isnan(d) || isinf(d)) // if occluded or mismatched
							{
								continue;
							}

							XYZW(0) = x; XYZW(1) = y; XYZW(2) = d; XYZW(3) = 1;
							XYZW = mQ*XYZW;
							W_1 = 1.0/XYZW(3);
							Matx31d mX;
							mX(0) = XYZW(0)*W_1; mX(1) = XYZW(1)*W_1; mX(2) = XYZW(2)*W_1;
							Matx31d mWrdPt = mR0.t()*(mR0_new.t()*mX-mt0); // 转至世界系坐标

							int R,G,B;
							if (rimg0.channels()==1)
							{
								uchar gray = rimg0.at<uchar>(y,x);
								R = G = B = gray;
							}
							else
							{
								Vec3b pix = rimg0.at<Vec3b>(y,x);
								B = pix.val[0]; G = pix.val[1]; R = pix.val[2];
							}

							fprintf(fp, "%lf;%lf;%lf;%d;%d;%d\n", mWrdPt(0), mWrdPt(1), mWrdPt(2), R, G, B);
						}
					}
					fclose(fp);
				}
			}
			else if (1==m_idxStereoMethod) // SGM OpenCV version
			{

			} 
			else if (2==m_idxStereoMethod) // Zhaokun Zhu's method
			{
				CDlgZhuMethodSettings dlgSettings;
				dlgSettings.m_idxSimilarityMeasure = m_idxSimilarityMeasure_Zhu;
				dlgSettings.m_maxNormAng = m_maxNormAng_Zhu;
				dlgSettings.m_nPatchMatchIter = m_nPatchMatchIter_Zhu;
				dlgSettings.m_nRandSamp = m_nRandSamp_Zhu;
				dlgSettings.m_wndSize = m_wndSize_Zhu;
				dlgSettings.m_imgUncertainty = m_imgUncertainty_Zhu;
				dlgSettings.m_deFactor = m_deFactor_Zhu;

				if (IDOK==dlgSettings.DoModal())
				{
					m_idxSimilarityMeasure_Zhu = dlgSettings.m_idxSimilarityMeasure;
					m_maxNormAng_Zhu = dlgSettings.m_maxNormAng;
					m_nPatchMatchIter_Zhu = dlgSettings.m_nPatchMatchIter;
					m_nRandSamp_Zhu = dlgSettings.m_nRandSamp;
					m_wndSize_Zhu = dlgSettings.m_wndSize;
					m_imgUncertainty_Zhu = dlgSettings.m_imgUncertainty;
					m_deFactor_Zhu = dlgSettings.m_deFactor;

					vector<Mat> vImgs_spt;
					vector<Matx33d> vKs_spt, vRs_spt;
					vector<Matx31d> vts_spt;
					vector<CloudPoint> vClouds;

					vImgs_spt.push_back(rimg1);
					vKs_spt.push_back(mK1_new);
					vRs_spt.push_back(mI);
					vts_spt.push_back(mt1_new);

					vClouds.push_back(pt3d_n);
					vClouds.push_back(pt3d_f);

					// start
					Mat mDepth_final, mHx_final, mHy_final, mScore_final, mVisi_final, img0_outLevel;
					SurfaceMapEstimation_oneImg_Pyramid(mK0_new,mI,mt0_new,rimg0,vKs_spt,vRs_spt,vts_spt,vImgs_spt,vClouds,
						"",mDepth_final,mHx_final,mHy_final,mScore_final,mVisi_final,img0_outLevel,m_wndSize_Zhu,m_maxNormAng_Zhu,
						m_nPatchMatchIter_Zhu,m_deFactor_Zhu,m_nRandSamp_Zhu,90,0.95,1.5,0.01,m_imgUncertainty_Zhu,1,0,5);

					double f_new_1 = 1.0/f_new;
					double cx0_new = mK0_new(0,2);
					double cy0_new = mK0_new(1,2);

					double max_norm = 1.0;
					double min_norm = -1.0;
					double color_factor_norm = 255.0/(max_norm - min_norm);

					Mat mDisparityImg(rimg0.rows, rimg0.cols, CV_8UC3), mNormImg(rimg0.rows, rimg0.cols, CV_8UC3);

//					FILE * file_pointcloud = fopen("C:\\Users\\DeepVoid\\Desktop\\point cloud by Zhu method.txt", "w");
					FILE * file_pointcloud = fopen("E:\\results\\point cloud by Zhu method.txt", "w");

					// 输出结果
					for (int i=0;i<rimg0.rows;++i)
					{
						double nimgy = (i-cy0_new)*f_new_1;
						for (int j=0;j<rimg0.cols;++j)
						{
							double nimgx = (j-cx0_new)*f_new_1;

							double d = mDepth_final.at<float>(i,j);
							double hx = mHx_final.at<float>(i,j);
							double hy = mHy_final.at<float>(i,j);
							double score = mScore_final.at<float>(i,j);

							Matx31d XYZ = GetXYZ_givenDepth(mI, mt0_new, nimgx, nimgy, d);
							XYZW(0)=XYZ(0); XYZW(1)=XYZ(1); XYZW(2)=XYZ(2); XYZW(3)=1; 

							// project to the matching image
							Matx31d xyw = mP1_new*XYZW;
							double x1 = xyw(0)/xyw(2);
							double y1 = xyw(1)/xyw(2);

							// get the disparity
							double disparity;
							if (!isVerticalStereo)
							{
								disparity = j-x1;
							} 
							else
							{
								disparity = i-y1;
							}

							uchar disparity_gray = FTOI((disparity-m_minDisparity)*color_factor);

							if (score<0)
							{
								mDisparityImg.at<Vec3b>(i,j)[0]=0;
								mDisparityImg.at<Vec3b>(i,j)[1]=0;
								mDisparityImg.at<Vec3b>(i,j)[2]=255;
							} 
							else
							{
								mDisparityImg.at<Vec3b>(i,j)[0]=disparity_gray;
								mDisparityImg.at<Vec3b>(i,j)[1]=disparity_gray;
								mDisparityImg.at<Vec3b>(i,j)[2]=disparity_gray;
							}

							// transform the 3d coordinates to world coordinate system
							Matx31d mWrdPt = mR0.t()*(mR0_new.t()*XYZ-mt0);
							
							// get the normal of current obtained parameters
							Matx31d mn0; mn0(2)=1;
							get_normal_givendrhxhy(f_new, f_new, nimgx, nimgy, d, hx, hy, mn0(0), mn0(1));
							Matx31d mnw = -mR0.t()*mR0_new.t()*mn0; // convert the normal into world coordinate system
							double normN = norm(mnw);
							mnw(0)/=normN;mnw(1)/=normN;mnw(2)/=normN;

							uchar r = FTOI((mnw(0)-min_norm) * color_factor_norm);
							uchar g = FTOI((mnw(1)-min_norm) * color_factor_norm);
							uchar b = FTOI((mnw(2)-min_norm) * color_factor_norm);
							mNormImg.at<Vec3b>(i,j)[0]=b;
							mNormImg.at<Vec3b>(i,j)[1]=g;
							mNormImg.at<Vec3b>(i,j)[2]=r;

							if (score<0)
							{
								continue;
							}

							uchar R,G,B;
							if (rimg0.channels()==1)
							{
								uchar gray = rimg0.at<uchar>(i,j);
								R = G = B = gray;
							}
							else
							{
								Vec3b pix = rimg0.at<Vec3b>(i,j);
								B = pix.val[0]; G = pix.val[1]; R = pix.val[2];
							}

							fprintf(file_pointcloud, "%.12f;%.12f;%.12f;%d;%d;%d;%.12f;%.12f;%.12f\n",
								mWrdPt(0), mWrdPt(1), mWrdPt(2), R, G, B, mnw(0), mnw(1), mnw(2));
						}
					}
					fclose(file_pointcloud);

// 					imwrite("C:\\Users\\DeepVoid\\Desktop\\disparity by Zhu method.bmp", mDisparityImg);
// 					imwrite("C:\\Users\\DeepVoid\\Desktop\\normal by Zhu method.bmp", mNormImg);
					imwrite("E:\\results\\disparity by Zhu method.bmp", mDisparityImg);
					imwrite("E:\\results\\normal by Zhu method.bmp", mNormImg);
				}
			} 
			else
			{

			}
		}
	}
}

RNG rng_initRndField0(0xffffffff);
void CDeepVoidApp::OnCalibsimu()
{
	// TODO: Add your command handler code here

	// 仿真世界点坐标
	vector<Point3d> pts3d;
	Point3d pt3d;
	pt3d.x = -1;	pt3d.y = -1;	pt3d.z = 5;		pts3d.push_back(pt3d);
	pt3d.x =  1;	pt3d.y = -1;	pt3d.z = 5;		pts3d.push_back(pt3d);
	pt3d.x =  1;	pt3d.y =  1;	pt3d.z = 5;		pts3d.push_back(pt3d);
	pt3d.x = -1;	pt3d.y =  1;	pt3d.z = 5;		pts3d.push_back(pt3d);
	pt3d.x = 0;		pt3d.y =  0;	pt3d.z = 5;		pts3d.push_back(pt3d);

	pt3d.x = -1;	pt3d.y = -1;	pt3d.z = 7.5;	pts3d.push_back(pt3d);
	pt3d.x =  1;	pt3d.y = -1;	pt3d.z = 7.5;	pts3d.push_back(pt3d);
	pt3d.x =  1;	pt3d.y =  1;	pt3d.z = 7.5;	pts3d.push_back(pt3d);
	pt3d.x = -1;	pt3d.y =  1;	pt3d.z = 7.5;	pts3d.push_back(pt3d);
	pt3d.x = 0;		pt3d.y =  0;	pt3d.z = 7.5;	pts3d.push_back(pt3d);

	pt3d.x = -1;	pt3d.y = -1;	pt3d.z = 10;	pts3d.push_back(pt3d);
	pt3d.x =  1;	pt3d.y = -1;	pt3d.z = 10;	pts3d.push_back(pt3d);
	pt3d.x =  1;	pt3d.y =  1;	pt3d.z = 10;	pts3d.push_back(pt3d);
	pt3d.x = -1;	pt3d.y =  1;	pt3d.z = 10;	pts3d.push_back(pt3d);
	pt3d.x = 0;		pt3d.y =  0;	pt3d.z = 10;	pts3d.push_back(pt3d);

	pt3d.x = -1;	pt3d.y = -1;	pt3d.z = 12.5;	pts3d.push_back(pt3d);
	pt3d.x =  1;	pt3d.y = -1;	pt3d.z = 12.5;	pts3d.push_back(pt3d);
	pt3d.x =  1;	pt3d.y =  1;	pt3d.z = 12.5;	pts3d.push_back(pt3d);
	pt3d.x = -1;	pt3d.y =  1;	pt3d.z = 12.5;	pts3d.push_back(pt3d);
	pt3d.x = 0;		pt3d.y =  0;	pt3d.z = 12.5;	pts3d.push_back(pt3d);

	// 仿真内参数
	int w = 1280;
	int h = 720;

	double w_2 = (w-1)*0.5;
	double h_2 = (h-1)*0.5;

	double fx = 1024;
	double fy = 1024;
	double s = 1.5;

	Matx33d mK;
	mK(0,0) = fx;	mK(0,1) = s;	mK(0,2) = w_2;
	mK(1,1) = fy;	mK(1,2) = h_2;
	mK(2,2) = 1;

	Matx33d mK_1 = calib::invK(mK);

	double k1 = 0.012;
	double k2 = -0.006;
	double k3 = 0.005;
	double k4 = -0.010;
	double k5 = 0.004;

	// 仿真外参数
	Matx33d mR;
	mR(0,0)=mR(1,1)=mR(2,2)=1;

	Matx31d mt;

	vector<Point2d> pts2d; // 理想线性成像点
	vector<Point2d> pts2d_dist; // 畸变后的成像点
	vector<Point2d> pts2d_err;	// 带像点提取噪声的观测像点

	double img_sigma = 1.0;

	// 线性成像
	for (auto iter=pts3d.begin(); iter!=pts3d.end(); ++iter)
	{
		Matx31d mX;
		mX(0)=iter->x;	mX(1)=iter->y;	mX(2)=iter->z;

		Matx31d mx = mK*(mR*mX+mt);

		mx(0)=mx(0)/mx(2);
		mx(1)=mx(1)/mx(2);
		mx(2)=1;

		Point2d pt2d;
		pt2d.x = mx(0);
		pt2d.y = mx(1);

		pts2d.push_back(pt2d);

		Matx31d nx = mK_1*mx;

		double u = nx(0);
		double v = nx(1);

		double dx,dy;
		distortions::dxdy_brown(fx,fy,s,u,v,k1,k2,k3,k4,k5,dx,dy);

		pt2d.x+=dx;
		pt2d.y+=dy;

		pts2d_dist.push_back(pt2d);

		dx = rng_initRndField0.gaussian(img_sigma);
		dy = rng_initRndField0.gaussian(img_sigma);

		pt2d.x+=dx;
		pt2d.y+=dy;

		pts2d_err.push_back(pt2d);
	}

// 	FILE * file0 = fopen("E:\\20150504标定查错\\16.txt", "r");
// 	FILE * file1 = fopen("E:\\20150504标定查错\\1.bmp.calibratePt.txt", "r");
// 	int nPts;
// 	double tmp;
// 	fscanf(file0, "%lf", &tmp);
// 	fscanf(file0, "%lf", &tmp);
// 	fscanf(file0, "%lf", &tmp);
// 	fscanf(file0, "%d", &nPts);
// 	fscanf(file1, "%d", &nPts);
// 	vector<Point3d> pts3d;
// 	vector<Point2d> pts2d_err;
// 	for (int i=0;i<nPts;++i)
// 	{
// 		double X,Y,Z,x,y;
// 		fscanf(file0, "%lf	%lf	%lf	", &X,&Y,&Z);
// 		fscanf(file1, "%lf	%lf	", &x, &y);
// 
// 		Point3d XYZ;
// 		Point2d xy;
// 		XYZ.x = X; XYZ.y = Y; XYZ.z = Z;
// 		xy.x = x; xy.y = y;
// 
// 		pts3d.push_back(XYZ);
// 		pts2d_err.push_back(xy);
// 	}
// 	fclose(file0);
// 	fclose(file1);

	vector<Point3d> pts3d_t;
	Matx44d mT3d = calib::normalize_3d(pts3d, pts3d_t);

	vector<Point2d> pts2d_err_t;
	Matx33d mT2d = calib::normalize_2d(pts2d_err, pts2d_err_t);

	Matx34d mP = calib::calib_P(pts3d_t, pts2d_err_t);

	mP = mT2d.inv()*mP*mT3d;

	Matx33d mK_init, mR_init;
	Matx31d mt_init;
	calib::decomp_P(mP, mK_init, mR_init, mt_init);

	Matx33d mK_optim1 = mK_init;
	Matx33d mR_optim1 = mR_init;
	Matx31d mt_optim1 = mt_init;
	double dist1[5], info1[5];
	memset(dist1, 0, 5*sizeof(double));

	optim::optim_lm_fxfycxcys_w_t_k1k2k3k4k5(pts3d,pts2d_err,mK_optim1,mR_optim1,mt_optim1,dist1,0,info1,0.001,128);


	Matx33d mK_optim2 = mK_init;
	Matx33d mR_optim2 = mR_init;
	Matx31d mt_optim2 = mt_init;
	double dist2[5], info2[5];
	memset(dist2, 0, 5*sizeof(double));

	optim::optim_lm_fxfycxcys_w_t_k1k2k3k4k5(pts3d,pts2d_err,mK_optim2,mR_optim2,mt_optim2,dist2,1,info2,0.001,128);


	Matx33d mK_optim3 = mK_init;
	mK_optim3(0,1) = 0/*s*/;
	Matx33d mR_optim3 = mR_init;
	Matx31d mt_optim3 = mt_init;
	double dist3[5], info3[5];
	memset(dist3, 0, 5*sizeof(double));

	optim::optim_lm_fxfycxcy_w_t_k1k2k3k4k5(pts3d,pts2d_err,mK_optim3,mR_optim3,mt_optim3,dist3,0,info3,0.001,128);


	Matx33d mK_optim4 = mK_init;
	mK_optim4(0,1) = 0/*s*/;
	Matx33d mR_optim4 = mR_init;
	Matx31d mt_optim4 = mt_init;
	double dist4[5], info4[5];
	memset(dist4, 0, 5*sizeof(double));

	optim::optim_lm_fxfycxcy_w_t_k1k2k3k4k5(pts3d,pts2d_err,mK_optim4,mR_optim4,mt_optim4,dist4,1,info4,0.001,128);


	Matx33d mK_optim5 = mK_init;
	mK_optim5(0,1) = 0/*s*/;
	Matx33d mR_optim5 = mR_init;
	Matx31d mt_optim5 = mt_init;
	double dist5[5], info5[5];
	memset(dist5, 0, 5*sizeof(double));

	optim::optim_lm_fcxcy_w_t_k1k2k3k4k5(pts3d,pts2d_err,mK_optim5,mR_optim5,mt_optim5,dist5,0,info5,0.001,128);


	Matx33d mK_optim6 = mK_init;
	mK_optim6(0,1) = 0/*s*/;
	Matx33d mR_optim6 = mR_init;
	Matx31d mt_optim6 = mt_init;
	double dist6[5], info6[5];
	memset(dist6, 0, 5*sizeof(double));

	optim::optim_lm_fcxcy_w_t_k1k2k3k4k5(pts3d,pts2d_err,mK_optim6,mR_optim6,mt_optim6,dist6,1,info6,0.001,128);


	Matx33d mK_optim7 = mK_init;
	mK_optim7(0,1) = 0;
	mK_optim7(0,2) = /*w_2*/1024;
	mK_optim7(1,2) = /*h_2*/544;
	Matx33d mR_optim7 = mR_init;
	Matx31d mt_optim7 = mt_init;
	double dist7[5], info7[5];
	memset(dist7, 0, 5*sizeof(double));

	optim::optim_lm_f_w_t_k1k2k3k4k5(pts3d,pts2d_err,mK_optim7,mR_optim7,mt_optim7,dist7,0,info7,0.001,128);


	Matx33d mK_optim8 = mK_init;
	mK_optim8(0,1) = 0;
	mK_optim8(0,2) = /*w_2*/1024;
	mK_optim8(1,2) = /*h_2*/544;
	Matx33d mR_optim8 = mR_init;
	Matx31d mt_optim8 = mt_init;
	double dist8[5], info8[5];
	memset(dist8, 0, 5*sizeof(double));

	optim::optim_lm_f_w_t_k1k2k3k4k5(pts3d,pts2d_err,mK_optim8,mR_optim8,mt_optim8,dist8,1,info8,0.001,128);


	Matx33d mK_optim9 = mK_init;
	mK_optim9(0,1) = 0/*s*/;
	Matx33d mR_optim9 = mR_init;
	Matx31d mt_optim9 = mt_init;
	double dist9[5], info9[5];
	memset(dist9, 0, 5*sizeof(double));

	optim::optim_lm_fxfycxcy_w_t_k1(pts3d,pts2d_err,mK_optim9,mR_optim9,mt_optim9,dist9,0,info9,0.001,128);


	Matx33d mK_optim10 = mK_init;
	mK_optim10(0,1) = 0/*s*/;
	Matx33d mR_optim10 = mR_init;
	Matx31d mt_optim10 = mt_init;
	double dist10[5], info10[5];
	memset(dist10, 0, 5*sizeof(double));

	optim::optim_lm_fxfycxcy_w_t_k1(pts3d,pts2d_err,mK_optim10,mR_optim10,mt_optim10,dist10,1,info10,0.001,128);


	Matx33d mK_optim11 = mK_init;
	mK_optim11(0,1) = 0/*s*/;
	Matx33d mR_optim11 = mR_init;
	Matx31d mt_optim11 = mt_init;
	double dist11[5], info11[5];
	memset(dist11, 0, 5*sizeof(double));

	optim::optim_lm_fcxcy_w_t_k1(pts3d,pts2d_err,mK_optim11,mR_optim11,mt_optim11,dist11,0,info11,0.001,128);


	Matx33d mK_optim12 = mK_init;
	mK_optim12(0,1) = 0/*s*/;
	Matx33d mR_optim12 = mR_init;
	Matx31d mt_optim12 = mt_init;
	double dist12[5], info12[5];
	memset(dist12, 0, 5*sizeof(double));

	optim::optim_lm_fcxcy_w_t_k1(pts3d,pts2d_err,mK_optim12,mR_optim12,mt_optim12,dist12,1,info12,0.001,128);


	Matx33d mK_optim13 = mK_init;
	mK_optim13(0,1) = 0/*s*/;
	Matx33d mR_optim13 = mR_init;
	Matx31d mt_optim13 = mt_init;
	double dist13[5], info13[5];
	memset(dist13, 0, 5*sizeof(double));

	optim::optim_lm_fxfycxcy_w_t_k1k2(pts3d,pts2d_err,mK_optim13,mR_optim13,mt_optim13,dist13,1,info13,0.001,128);


	Matx33d mK_optim14 = mK_init;
	mK_optim14(0,1) = 0/*s*/;
	Matx33d mR_optim14 = mR_init;
	Matx31d mt_optim14 = mt_init;
	double dist14[5], info14[5];
	memset(dist14, 0, 5*sizeof(double));

	optim::optim_lm_fcxcy_w_t_k1k2(pts3d,pts2d_err,mK_optim14,mR_optim14,mt_optim14,dist14,1,info14,0.001,128);

// 	int mmm = 15;
// 	int nnn = 5;
// 	int size[] = {mmm,nnn};
// 	SparseMat sM(2,size,CV_64FC1);
// 
// 	uchar * ptr = sM.ptr(0,0,false);
// 	const double * ptrd = sM.find<double>(0,0);
// 	sM.ref<double>(0,0) = 0;
// 	double xxx = sM.value<double>(0,0);
// 	xxx = sM.value<double>(0,1);
// 	ptrd = sM.find<double>(0,0);

	double shit = 235;
	double shit1 = sqrt(shit);

	
// 	double ix1,iy1,ix2,iy2;
// 
// 	double dk1[5],dk2[5];
// 	dk1[0] = -0.231404;   dk1[1] = 0.188846;   dk1[2] = 0.004077;   dk1[3] = -0.029018;   dk1[4] = 1.289987;
// 	dk2[0] = -0.311229;   dk2[1] = 1.605225;   dk2[2] = 0.000208;	dk2[3] = 0.007570;    dk2[4] = -2.333372;
// 
// 	RemoveDistortion_DCBrown(923.702797,572.029468,1579.443025,1540.117825,dk1,986.0,228.0,ix1,iy1);
// 	RemoveDistortion_DCBrown(1059.839798,522.676599,1530.689034,1525.931862,dk2,1237.0,178.0,ix2,iy2);
// 
// 	vector<Matx33d> vRis,vRijs;
// 	
// 	Matx33d R1 = calib::converse_rotvec_R(0, 0, 0);
// 	Matx33d R12 = calib::converse_rotvec_R(0.5, 0, 0);
// 	Matx33d R2 = R12*R1;
// 	Matx33d R23 = calib::converse_rotvec_R(0, 0.5, 0);
// 	Matx33d R3 = R23*R2;
// 	Matx33d R34 = calib::converse_rotvec_R(0, 0, 0.5);
// 	Matx33d R4 = R34*R3;
// 	Matx33d R13 = R3*R1.t();
// 	Matx33d R14 = R4*R1.t();
// 	Matx33d R24 = R4*R2.t();
// 
// 	Matx33d R12_init = calib::converse_rotvec_R(rng_initRndField0.gaussian(0.03), rng_initRndField0.gaussian(0.03), rng_initRndField0.gaussian(0.03)) * R12;
// 	vRijs.push_back(R12_init);
// 	Matx33d R13_init = calib::converse_rotvec_R(rng_initRndField0.gaussian(0.03), rng_initRndField0.gaussian(0.03), rng_initRndField0.gaussian(0.03)) * R13;
// 	vRijs.push_back(R13_init);
// 	Matx33d R14_init = calib::converse_rotvec_R(rng_initRndField0.gaussian(0.03), rng_initRndField0.gaussian(0.03), rng_initRndField0.gaussian(0.03)) * R14;
// 	vRijs.push_back(R14_init);
// 	Matx33d R23_init = calib::converse_rotvec_R(rng_initRndField0.gaussian(0.03), rng_initRndField0.gaussian(0.03), rng_initRndField0.gaussian(0.03)) * R23;
// 	vRijs.push_back(R23_init);
// 	Matx33d R24_init = calib::converse_rotvec_R(rng_initRndField0.gaussian(0.03), rng_initRndField0.gaussian(0.03), rng_initRndField0.gaussian(0.03)) * R24;
// 	vRijs.push_back(R24_init);
// 	Matx33d R34_init = calib::converse_rotvec_R(rng_initRndField0.gaussian(0.03), rng_initRndField0.gaussian(0.03), rng_initRndField0.gaussian(0.03)) * R34;
// 	vRijs.push_back(R34_init);
// 
// 	Matx33d R2_init = calib::converse_rotvec_R(-0.04, 0.08, 0.06) * R2;
// 	Matx33d R3_init = calib::converse_rotvec_R(0.02, -0.04, -0.10) * R3;
// 	Matx33d R4_init = calib::converse_rotvec_R(0.02, 0.016, -0.04) * R4;
// 
// 	vRis.push_back(R1);
// 	vRis.push_back(R2_init);
// 	vRis.push_back(R3_init);
// 	vRis.push_back(R4_init);
// 
// 	vector<Point2i> idxptsss;
// 	Point2i ptidx;
// 	ptidx.x = 0; ptidx.y = 1;
// 	idxptsss.push_back(ptidx);
// 	ptidx.x = 0; ptidx.y = 2;
// 	idxptsss.push_back(ptidx);
// 	ptidx.x = 0; ptidx.y = 3;
// 	idxptsss.push_back(ptidx);
// 	ptidx.x = 1; ptidx.y = 2;
// 	idxptsss.push_back(ptidx);
// 	ptidx.x = 1; ptidx.y = 3;
// 	idxptsss.push_back(ptidx);
// 	ptidx.x = 2; ptidx.y = 3;
// 	idxptsss.push_back(ptidx);
// 
// 	int nnn0,nnn1,nnn2;
// 	double rrr0,rrr1,rrr2;
// 	vector<Matx33d> vRis0 = vRis;
// 	SfM_ZZK::optim_nr_Ri_Govindu04(vRijs,idxptsss,vRis0,0,&nnn0,&rrr0);
// 
// 	Matx33d dR11 = vRis0[0]*R1.t();
// 	Matx31d dw11 = calib::converse_R_rotvec(dR11);
// 	double dr110 = norm(dw11);
// 
// 	Matx33d dR22 = vRis0[1]*R2.t();
// 	Matx31d dw22 = calib::converse_R_rotvec(dR22);
// 	double dr220 = norm(dw22);
// 
// 	Matx33d dR33 = vRis0[2]*R3.t();
// 	Matx31d dw33 = calib::converse_R_rotvec(dR33);
// 	double dr330 = norm(dw33);
// 
// 	Matx33d dR44 = vRis0[3]*R4.t();
// 	Matx31d dw44 = calib::converse_R_rotvec(dR44);
// 	double dr440 = norm(dw44);
// 
// 
// 
// 	vector<Matx33d> vRis1 = vRis;
// 	SfM_ZZK::optim_nr_Ri(vRijs,idxptsss,vRis1,0,&nnn1,&rrr1);
// 
// 	dR11 = vRis1[0]*R1.t();
// 	dw11 = calib::converse_R_rotvec(dR11);
// 	double dr111 = norm(dw11);
// 
// 	dR22 = vRis1[1]*R2.t();
// 	dw22 = calib::converse_R_rotvec(dR22);
// 	double dr221 = norm(dw22);
// 
// 	dR33 = vRis1[2]*R3.t();
// 	dw33 = calib::converse_R_rotvec(dR33);
// 	double dr331 = norm(dw33);
// 
// 	dR44 = vRis1[3]*R4.t();
// 	dw44 = calib::converse_R_rotvec(dR44);
// 	double dr441 = norm(dw44);
// 
// 
// 
// 	vector<Matx33d> vRis2 = vRis;
// 	SfM_ZZK::optim_gn_Ri(vRijs,idxptsss,vRis2,0,&nnn2,&rrr2);
// 
// 	dR11 = vRis2[0]*R1.t();
// 	dw11 = calib::converse_R_rotvec(dR11);
// 	double dr112 = norm(dw11);
// 
// 	dR22 = vRis2[1]*R2.t();
// 	dw22 = calib::converse_R_rotvec(dR22);
// 	double dr222 = norm(dw22);
// 
// 	dR33 = vRis2[2]*R3.t();
// 	dw33 = calib::converse_R_rotvec(dR33);
// 	double dr332 = norm(dw33);
// 
// 	dR44 = vRis2[3]*R4.t();
// 	dw44 = calib::converse_R_rotvec(dR44);
// 	double dr442 = norm(dw44);
// 
// 
// 
// 	vector<Matx33d> vRis3 = vRis;
// 	double info[5];
// 	SfM_ZZK::optim_lm_Ri(vRijs,idxptsss,vRis3,0,info,0.001,64);
// 
// 	dR11 = vRis3[0]*R1.t();
// 	dw11 = calib::converse_R_rotvec(dR11);
// 	double dr113 = norm(dw11);
// 
// 	dR22 = vRis3[1]*R2.t();
// 	dw22 = calib::converse_R_rotvec(dR22);
// 	double dr223 = norm(dw22);
// 
// 	dR33 = vRis3[2]*R3.t();
// 	dw33 = calib::converse_R_rotvec(dR33);
// 	double dr333 = norm(dw33);
// 
// 	dR44 = vRis3[3]*R4.t();
// 	dw44 = calib::converse_R_rotvec(dR44);
// 	double dr443 = norm(dw44);
}

UINT DSBA_Simu_increaseImgPtNoise(LPVOID param)
{
	CDeepVoidApp * pApp = (CDeepVoidApp * )param;

	RNG rng(0xffffffff);			// Initializes a random number generator state
//	RNG rng((unsigned)time(NULL));	// Initializes a random number generator state

	// parameters setting /////////////////////////////////////////////////////////////////////////////
	int nObj = 30;				// number of object points
	int nImg = 7;				// number of images
	int nImgperPt = 7;			// each object point is observed by this many images
	double range = 1;			// the spread of object points
	double distance = 3;		// distance between camera and object points center
	double interval_ang = 15;	// the angular interval between neighboring images (in degree)
	double img_noise_std = 1.0;	// standard deviation of image point error
	double t_noise_std = 0.1;	// standard deviation of the initial translation vector
	double rad_noise_std = 0.5*calib::D2R;			// standard deviation of the initial rotation, in radian
	double rv_noise_std = rad_noise_std/sqrt(3.0);	// standard deviation of the initial rotation vector element
	int nSimu = 200;			// the times of simulation in order to do the statistics

	double f = 1500;			// equivalent focal length
	double w = 1280;			// image width
	double h = 720;				// image height
	double cx = 0.5*(w-1);		// principal point
	double cy = 0.5*(h-1);		// principal point
	double s = 0;				// screw factor

	double f_e = f + 0;		// equivalent focal length with error
	double cx_e = cx + 0;		// principal point with error
	double cy_e = cy + 0;		// principal point with error
	double s_e = s + 0;			// screw factor with error

	double k1 = 0.01;			// 2nd order radial distortion coefficient
	double k2 = -0.02;			// 4th order radial distortion coefficient
	double k3 = 0.003;			// one tangential distortion coefficient
	double k4 = 0.002;			// the other tangential distortion coefficient
	double k5 = -0.001;			// 6th order radial distortion coefficient

	double k1_e = k1 + 0;		// 2nd order radial distortion coefficient with error
	double k2_e = k2 + 0;		// 4th order radial distortion coefficient with error
	double k3_e = k3 + 0;		// one tangential distortion coefficient with error 
	double k4_e = k4 + 0;		// the other tangential distortion coefficient with error
	double k5_e = k5 + 0;		// 6th order radial distortion coefficient with error

	// simulate object points //////////////////////////////////////////////////////////////////////////
//	FILE * file_objpts = fopen("C:\\Users\\DeepVoid\\Desktop\\wrdpts.txt", "w");
	vector<Point3d> vObjPts;
	for (int i=0;i<nObj;++i)
	{
		Point3d objPt;

		objPt.x = rng.uniform(-range*0.5, range*0.5);
		objPt.y = rng.uniform(-range*0.5, range*0.5);
		objPt.z = rng.uniform(-range*0.5, range*0.5);

		vObjPts.push_back(objPt);

//		fprintf(file_objpts, "%lf	%lf	%lf\n", objPt.x,objPt.y,objPt.z);
	}
//	fclose(file_objpts);

	// simulate images /////////////////////////////////////////////////////////////////////////////////
	vector<Matx33d> Ks,K_es;
	vector<Matx33d> Rs,R_es;
	vector<Matx31d> ts,t_es;
	vector<Matx<double,5,1>> dists,dist_es;
	vector<int> distTypes;

	vector<uchar> j_fixed(nImg);		// indicates which images' parameters are fixed
	vector<uchar> i_fixed(nObj);		// indicates which object points' are fixed
	j_fixed[0] = 1;						// the 1st image's parameters are fixed

	Matx33d mK;
	mK(0,0) = mK(1,1) = f;
	mK(0,2) = cx; mK(1,2) = cy;
	mK(0,1) = s;  mK(2,2) = 1;
	Ks.push_back(mK);

	Matx33d mK_e;
	mK_e(0,0) = mK_e(1,1) = f_e;
	mK_e(0,2) = cx_e; mK_e(1,2) = cy_e;
	mK_e(0,1) = s_e;  mK_e(2,2) = 1;
	K_es.push_back(mK_e);

	Matx33d mR;
	mR(0,0)=mR(1,1)=mR(2,2)=1;
	Rs.push_back(mR);
	R_es.push_back(mR);

	Matx31d mt;
	mt(2) = distance;
	ts.push_back(mt);
	t_es.push_back(mt);

// 	FILE * file_cam = fopen("C:\\Users\\DeepVoid\\Desktop\\RT00.txt", "w");
// 	for (int i=0;i<3;++i)
// 	{
// 		fprintf(file_cam, "%lf	%lf	%lf	%lf\n", mR(i,0), mR(i,1), mR(i,2), mt(i));
// 	}
// 	fprintf(file_cam, "%lf	%lf	%lf	%lf", 0.0, 0.0, 0.0, 1.0);
// 	fclose(file_cam);

	Matx<double,5,1> dist;
	dist(0) = k1;	dist(1) = k2;	dist(2) = k3;	dist(3) = k4;	dist(4) = k5;
	dists.push_back(dist);

	Matx<double,5,1> dist_e;
	dist_e(0) = k1_e;	dist_e(1) = k2_e;	dist_e(2) = k3_e;	dist_e(3) = k4_e;	dist_e(4) = k5_e;
	dist_es.push_back(dist_e);

	distTypes.push_back(1);

	for (int j=1;j<nImg;++j)
	{
		// real parameters
		Ks.push_back(mK);
		ts.push_back(mt);
		dists.push_back(dist);

		double ang_Y = j*interval_ang;

		Matx33d dR = calib::converse_angY_R(ang_Y);
		Rs.push_back(dR*mR);

// 		CString strfile;
// 		strfile.Format("C:\\Users\\DeepVoid\\Desktop\\RT%02d.txt", j);
// 		file_cam = fopen(strfile, "w");
// 		for (int i=0;i<3;++i)
// 		{
// 			fprintf(file_cam, "%lf	%lf	%lf	%lf\n", Rs[j](i,0), Rs[j](i,1), Rs[j](i,2), mt(i));
// 		}
// 		fprintf(file_cam, "%lf	%lf	%lf	%lf", 0.0, 0.0, 0.0, 1.0);
// 		fclose(file_cam);

		// parameters with error, ie given estimates of image parameters
		K_es.push_back(mK_e);
		dist_es.push_back(dist_e);
		
		double drvx = rng.gaussian(rv_noise_std);
		double drvy = rng.gaussian(rv_noise_std);
		double drvz = rng.gaussian(rv_noise_std);

		dR = calib::converse_rotvec_R(drvx,drvy,drvz);
		R_es.push_back(dR*Rs[j]);

		double dtx = rng.gaussian(t_noise_std);
		double dty = rng.gaussian(t_noise_std);
		double dtz = rng.gaussian(t_noise_std);

		Matx31d mdt;
		mdt(0) = dtx;
		mdt(1) = dty;
		mdt(2) = dtz;

		t_es.push_back(mdt+ts[j]);

		distTypes.push_back(1);
	}

	// choose randomly the given number of images for each object point in which the object point is observed
	// and further choose randomly one reference image from this observing image set ////////////////////////
	vector<vector<int>> idxVisiImg;
	vector<int> idxRefVisiImg;
	for (int i=0;i<nObj;++i)
	{
		vector<int> idxVisi;

		while (idxVisi.size()<nImgperPt)
		{
			int k = rng.next()%nImg;

			if (find(idxVisi.begin(),idxVisi.end(),k) != idxVisi.end()) // found, already exist
			{
				continue;
			}

			idxVisi.push_back(k);
		}

		int k = rng.next()%nImgperPt;

		idxVisiImg.push_back(idxVisi);
		idxRefVisiImg.push_back(idxVisi[k]);
//		idxRefVisiImg.push_back(3);
	}

	// generate the visibility matrix /////////////////////////////////////////////////////////////////
	// and real observations without random noise /////////////////////////////////////////////////////
	int sizes[] = {nObj, nImg};
	SparseMat ptrMat(2,sizes,CV_32SC1);

	vector<Point2d> vImgPts_nonDist; // non-distorted image points
	vector<Point2d> vImgPts_dist;	 // distorted image points
	vector<Matx22d> covInvs;		 // inverses of all the covariance matrix of image points

	Matx22d I22;
	I22(0,0) = I22(1,1) = 1;

	for (int i=0;i<nObj;++i)
	{
		Point3d pt3d = vObjPts[i];

		Matx31d XYZ;
		XYZ(0) = pt3d.x;
		XYZ(1) = pt3d.y;
		XYZ(2) = pt3d.z;

		vector<int> idxVisi = idxVisiImg[i];
		int idxRef = idxRefVisiImg[i];

		sort(idxVisi.begin(), idxVisi.end());
		
		for (int k=0;k<idxVisi.size();++k)
		{
			int j = idxVisi[k];

			Matx33d K = Ks[j];	Matx33d K_e = K_es[j];
			Matx33d R = Rs[j];
			Matx31d t = ts[j];
			Matx<double,5,1> dist = dists[j];	Matx<double,5,1> dist_e = dist_es[j];

			Matx31d uv = R*XYZ+t;
			Matx31d xy = K*uv;

			double u = uv(0)/uv(2);
			double v = uv(1)/uv(2);

			double x = xy(0)/xy(2);
			double y = xy(1)/xy(2);

			double dx,dy;

			distortions::dxdy_brown(K(0,0),K(1,1),K(0,1),u,v,dist(0),dist(1),dist(2),dist(3),dist(4),dx,dy);

			double x_d = x+dx;
			double y_d = y+dy;

// 			double e_x = rng.gaussian(img_noise_std);
// 			double e_y = rng.gaussian(img_noise_std);
// 
// 			double x_d_e = x_d + e_x;
// 			double y_d_e = y_d + e_y;

			Point2d imgPt;
			imgPt.x = x;
			imgPt.y = y;
			vImgPts_nonDist.push_back(imgPt);

			imgPt.x = x_d;
			imgPt.y = y_d;
			vImgPts_dist.push_back(imgPt);

			ptrMat.ref<int>(i,j) = vImgPts_nonDist.size()-1;

			covInvs.push_back(I22);
		}
	}

	// 像点提取噪声递增
	double img_noise_std_min = 0;
	double img_noise_std_max = 4;
	double img_noise_std_step = 0.1;
	int n_step = (img_noise_std_max-img_noise_std_min)/img_noise_std_step;

	vector<vector<double>> record_SBA(nImg), record_DSBA(nImg); // 记录每幅图像每个状态下统计出来的姿态角中误差

	FILE * file_SBA = fopen("C:\\Users\\DeepVoid\\Desktop\\error_SBA.txt", "w");
	FILE * file_DSBA = fopen("C:\\Users\\DeepVoid\\Desktop\\error_DSBA.txt", "w");

	for (int iter=0;iter<n_step;++iter)
	{
		CString strInfo;
		strInfo.Format("%03d iterations left", n_step-1-iter);
		pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

		img_noise_std = iter*img_noise_std_step; // 当前的像点噪声等级

		vector<vector<double>> SBA(nImg),DSBA(nImg);

		fprintf(file_SBA,"%lf	", img_noise_std);
		fprintf(file_DSBA,"%lf	", img_noise_std);

		// run multiple times, add observation noises, and do statistics /////////////////////////////////
		for (int kk=0;kk<nSimu;++kk)
		{
			vector<Point2d> vImgPts_dist_err;// distorted image points with random noise
			vector<Matx31d> nxys;			 // contains non-distorted normalized reference image point of each object point

			for (int i=0;i<nObj;++i)
			{
				vector<int> idxVisi = idxVisiImg[i];
				int idxRef = idxRefVisiImg[i];

				sort(idxVisi.begin(), idxVisi.end());

				for (int k=0;k<idxVisi.size();++k)
				{
					int j = idxVisi[k];

					const int * ptr = ptrMat.find<int>(i,j);

					if (NULL==ptr)
					{
						continue;
					}

					Matx33d K_e = K_es[j];
					Matx<double,5,1> dist_e = dist_es[j];

					int idx_ij = (*ptr);

					Point2d pt2d = vImgPts_dist[idx_ij];

					double x_d = pt2d.x;
					double y_d = pt2d.y;

					double e_x = rng.gaussian(img_noise_std);
					double e_y = rng.gaussian(img_noise_std);

					double x_d_e = x_d + e_x;
					double y_d_e = y_d + e_y;

					if (j==idxRef)
					{
						// reference image point has random noise
// 						pt2d.x = x_d_e;
// 						pt2d.y = y_d_e;

						// reference image point has no random noise
						vImgPts_dist_err.push_back(pt2d);

						// remove distortion using given estimates of interior orientations
						double ideal_x,ideal_y;
						distortions::remove_brown(K_e(0,0),K_e(1,1),K_e(0,2),K_e(1,2),K_e(0,1),
							dist_e(0),dist_e(1),dist_e(2),dist_e(3),dist_e(4),pt2d.x,pt2d.y,ideal_x,ideal_y);

						Matx31d nxy;
						nxy(0) = ideal_x;
						nxy(1) = ideal_y;
						nxy(2) = 1;

						// compute the normalized image point using given estimates of interior orientations
						nxys.push_back(calib::invK(K_e)*nxy);
					} 
					else
					{
						// non-reference image point has random noise
						pt2d.x = x_d_e;
						pt2d.y = y_d_e;
						vImgPts_dist_err.push_back(pt2d);
					}
				}
			}


			double info[5];

			// run SBA first //////////////////////////////////////////////////////////////////////////
			vector<Point3d> objPts_SBA = vObjPts;
			vector<Matx33d> Rs_SBA = R_es;
			vector<Matx31d> ts_SBA = t_es;

			SBA_ZZK::optim_sparse_lm_wj_tj_XiYiZi(objPts_SBA,K_es,Rs_SBA,ts_SBA,dist_es,distTypes,vImgPts_dist_err,covInvs,
				j_fixed,i_fixed,ptrMat,info);

			vector<double> drads_SBA,drads_DSBA;
			for (int j=0;j<nImg;++j)
			{
				Matx33d dR = Rs_SBA[j].t()*Rs[j];
				Matx31d rov = calib::converse_R_rotvec(dR);
				double drad = norm(rov);
				drads_SBA.push_back(drad*calib::R2D);

				SBA[j].push_back(drad*calib::R2D);
			}

			// then run DSBA //////////////////////////////////////////////////////////////////////////
			vector<Point3d> objPts_DSBA = vObjPts;
			vector<Matx33d> Rs_DSBA = R_es;
			vector<Matx31d> ts_DSBA = t_es;

			SBA_ZZK::optim_sparse_lm_wj_tj_di(objPts_DSBA,K_es,Rs_DSBA,ts_DSBA,dist_es,distTypes,vImgPts_dist_err,covInvs,
				nxys,idxRefVisiImg,j_fixed,i_fixed,ptrMat,info);

			for (int j=0;j<nImg;++j)
			{
				Matx33d dR = Rs_DSBA[j].t()*Rs[j];
				Matx31d rov = calib::converse_R_rotvec(dR);
				double drad = norm(rov);
				drads_DSBA.push_back(drad*calib::R2D);

				DSBA[j].push_back(drad*calib::R2D);
			}
		}

		vector<vector<double>> dSBA_DSBA(nImg);
		vector<double> davg_SBA_DSBA;
		vector<double> dds_rms_SBA;
		vector<double> dds_rms_BSBA;
		for (int j=0;j<nImg;++j)
		{
			double sum = 0;

			double sum_d2_SBA = 0;
			double sum_d2_DSBA = 0;

			for (int k=0;k<SBA[j].size();++k)
			{
				double dd_SBA = SBA[j][k];
				double dd_DSBA = DSBA[j][k];

				sum += dd_SBA-dd_DSBA;

				sum_d2_SBA += dd_SBA*dd_SBA;
				sum_d2_DSBA += dd_DSBA*dd_DSBA;

				dSBA_DSBA[j].push_back(dd_SBA-dd_DSBA);
			}

			davg_SBA_DSBA.push_back(sum/SBA[j].size());

			double rms_dd_SBA = sqrt(sum_d2_SBA/SBA[j].size());
			double rms_dd_DSBA = sqrt(sum_d2_DSBA/DSBA[j].size());

			dds_rms_SBA.push_back(rms_dd_SBA);
			dds_rms_BSBA.push_back(rms_dd_DSBA);

			record_SBA[j].push_back(rms_dd_SBA);
			record_DSBA[j].push_back(rms_dd_DSBA);

			fprintf(file_SBA,"%lf	", rms_dd_SBA);
			fprintf(file_DSBA,"%lf	", rms_dd_DSBA);
		}
		fprintf(file_SBA,"\n");
		fprintf(file_DSBA,"\n");
	}
	fclose(file_SBA);
	fclose(file_DSBA);

// 	vector<vector<double>> SBA(nImg),DSBA(nImg);
// 
// 	// run multiple times, add observation noises, and do statistics /////////////////////////////////
// 	for (int kk=0;kk<nSimu;++kk)
// 	{
// 		vector<Point2d> vImgPts_dist_err;// distorted image points with random noise
// 		vector<Matx31d> nxys;			 // contains non-distorted normalized reference image point of each object point
// 
// 		for (int i=0;i<nObj;++i)
// 		{
// 			vector<int> idxVisi = idxVisiImg[i];
// 			int idxRef = idxRefVisiImg[i];
// 
// 			sort(idxVisi.begin(), idxVisi.end());
// 
// 			for (int k=0;k<idxVisi.size();++k)
// 			{
// 				int j = idxVisi[k];
// 
// 				const int * ptr = ptrMat.find<int>(i,j);
// 
// 				if (NULL==ptr)
// 				{
// 					continue;
// 				}
// 
// 				Matx33d K_e = K_es[j];
// 				Matx<double,5,1> dist_e = dist_es[j];
// 
// 				int idx_ij = (*ptr);
// 
// 				Point2d pt2d = vImgPts_dist[idx_ij];
// 
// 				double x_d = pt2d.x;
// 				double y_d = pt2d.y;
// 
// 				double e_x = rng.gaussian(img_noise_std);
// 				double e_y = rng.gaussian(img_noise_std);
// 
// 				double x_d_e = x_d + e_x;
// 				double y_d_e = y_d + e_y;
// 
// 				if (j==idxRef)
// 				{
// 					// reference image point has random noise
// 					pt2d.x = x_d_e;
// 					pt2d.y = y_d_e;
// 
// 					// reference image point has no random noise
// 					vImgPts_dist_err.push_back(pt2d);
// 
// 					// remove distortion using given estimates of interior orientations
// 					double ideal_x,ideal_y;
// 					distortions::remove_brown(K_e(0,0),K_e(1,1),K_e(0,2),K_e(1,2),K_e(0,1),
// 						dist_e(0),dist_e(1),dist_e(2),dist_e(3),dist_e(4),pt2d.x,pt2d.y,ideal_x,ideal_y);
// 
// 					Matx31d nxy;
// 					nxy(0) = ideal_x;
// 					nxy(1) = ideal_y;
// 					nxy(2) = 1;
// 
// 					// compute the normalized image point using given estimates of interior orientations
// 					nxys.push_back(calib::invK(K_e)*nxy);
// 				} 
// 				else
// 				{
// 					// non-reference image point has random noise
// 					pt2d.x = x_d_e;
// 					pt2d.y = y_d_e;
// 					vImgPts_dist_err.push_back(pt2d);
// 				}
// 			}
// 		}
// 
// 
// 		double info[5];
// 
// 		// run SBA first //////////////////////////////////////////////////////////////////////////
// 		vector<Point3d> objPts_SBA = vObjPts;
// 		vector<Matx33d> Rs_SBA = R_es;
// 		vector<Matx31d> ts_SBA = t_es;
// 
// 		SBA_ZZK::optim_sparse_lm_wj_tj_XiYiZi(objPts_SBA,K_es,Rs_SBA,ts_SBA,dist_es,distTypes,vImgPts_dist_err,covInvs,
// 			j_fixed,i_fixed,ptrMat,info);
// 
// 		vector<double> drads_SBA,drads_DSBA;
// 		for (int j=0;j<nImg;++j)
// 		{
// 			Matx33d dR = Rs_SBA[j].t()*Rs[j];
// 			Matx31d rov = calib::converse_R_rotvec(dR);
// 			double drad = norm(rov);
// 			drads_SBA.push_back(drad*calib::R2D);
// 
// 			SBA[j].push_back(drad*calib::R2D);
// 		}
// 
// 		// then run DSBA //////////////////////////////////////////////////////////////////////////
// 		vector<Point3d> objPts_DSBA = vObjPts;
// 		vector<Matx33d> Rs_DSBA = R_es;
// 		vector<Matx31d> ts_DSBA = t_es;
// 
// 		SBA_ZZK::optim_sparse_lm_wj_tj_di(objPts_DSBA,K_es,Rs_DSBA,ts_DSBA,dist_es,distTypes,vImgPts_dist_err,covInvs,
// 			nxys,idxRefVisiImg,j_fixed,i_fixed,ptrMat,info);
// 
// 		for (int j=0;j<nImg;++j)
// 		{
// 			Matx33d dR = Rs_DSBA[j].t()*Rs[j];
// 			Matx31d rov = calib::converse_R_rotvec(dR);
// 			double drad = norm(rov);
// 			drads_DSBA.push_back(drad*calib::R2D);
// 
// 			DSBA[j].push_back(drad*calib::R2D);
// 		}
// 	}
// 
// 	vector<vector<double>> dSBA_DSBA(nImg);
// 	vector<double> davg_SBA_DSBA;
// 	vector<double> dds_rms_SBA;
// 	vector<double> dds_rms_BSBA;
// 	for (int j=0;j<nImg;++j)
// 	{
// 		double sum = 0;
// 
// 		double sum_d2_SBA = 0;
// 		double sum_d2_DSBA = 0;
// 
// 		for (int k=0;k<SBA[j].size();++k)
// 		{
// 			double dd_SBA = SBA[j][k];
// 			double dd_DSBA = DSBA[j][k];
// 
// 			sum += dd_SBA-dd_DSBA;
// 
// 			sum_d2_SBA += dd_SBA*dd_SBA;
// 			sum_d2_DSBA += dd_DSBA*dd_DSBA;
// 
// 			dSBA_DSBA[j].push_back(dd_SBA-dd_DSBA);
// 		}
// 
// 		davg_SBA_DSBA.push_back(sum/SBA[j].size());
// 
// 		double rms_dd_SBA = sqrt(sum_d2_SBA/SBA[j].size());
// 		double rms_dd_DSBA = sqrt(sum_d2_DSBA/DSBA[j].size());
// 
// 		dds_rms_SBA.push_back(rms_dd_SBA);
// 		dds_rms_BSBA.push_back(rms_dd_DSBA);
// 	}
// 
// 	FILE * file = fopen("C:\\Users\\DeepVoid\\Desktop\\error.txt", "a");
// 	for (int i=0;i<nImg;++i)
// 	{
// 		fprintf(file, "%d	%lf	%lf\n", i, dds_rms_SBA[i], dds_rms_BSBA[i]);
// 	}
// 	fprintf(file, "\n");
// 	fclose(file);

	return TRUE;
}

UINT DSBA_Simu_increaseObservations(LPVOID param)
{
	CDeepVoidApp * pApp = (CDeepVoidApp * )param;

	RNG rng(0xffffffff);			// Initializes a random number generator state
//	RNG rng((unsigned)time(NULL));	// Initializes a random number generator state

	// parameters setting /////////////////////////////////////////////////////////////////////////////
	int nObj = 30;				// number of object points
	int nImg = 20;				// number of images
	int nImgperPt = 7;			// each object point is observed by this many images
	double range = 1;			// the spread of object points
	double distance = 3;		// distance between camera and object points center
	double interval_ang = 15;	// the angular interval between neighboring images (in degree)
	double img_noise_std = 0.1;	// standard deviation of image point error
	double t_noise_std = 0.1;	// standard deviation of the initial translation vector
	double rad_noise_std = 0.5*calib::D2R;			// standard deviation of the initial rotation, in radian
	double rv_noise_std = rad_noise_std/sqrt(3.0);	// standard deviation of the initial rotation vector element
	int nSimu = 200;			// the times of simulation in order to do the statistics

	double f = 1500;			// equivalent focal length
	double w = 1280;			// image width
	double h = 720;				// image height
	double cx = 0.5*(w-1);		// principal point
	double cy = 0.5*(h-1);		// principal point
	double s = 0;				// screw factor

	double f_e = f + 0;		// equivalent focal length with error
	double cx_e = cx + 0;		// principal point with error
	double cy_e = cy + 0;		// principal point with error
	double s_e = s + 0;			// screw factor with error

	double k1 = 0.01;			// 2nd order radial distortion coefficient
	double k2 = -0.02;			// 4th order radial distortion coefficient
	double k3 = 0.003;			// one tangential distortion coefficient
	double k4 = 0.002;			// the other tangential distortion coefficient
	double k5 = -0.001;			// 6th order radial distortion coefficient

	double k1_e = k1 + 0;		// 2nd order radial distortion coefficient with error
	double k2_e = k2 + 0;		// 4th order radial distortion coefficient with error
	double k3_e = k3 + 0;		// one tangential distortion coefficient with error 
	double k4_e = k4 + 0;		// the other tangential distortion coefficient with error
	double k5_e = k5 + 0;		// 6th order radial distortion coefficient with error

	// simulate object points //////////////////////////////////////////////////////////////////////////
//	FILE * file_objpts = fopen("C:\\Users\\DeepVoid\\Desktop\\wrdpts.txt", "w");
	vector<Point3d> vObjPts;
	for (int i=0;i<nObj;++i)
	{
		Point3d objPt;

		objPt.x = rng.uniform(-range*0.5, range*0.5);
		objPt.y = rng.uniform(-range*0.5, range*0.5);
		objPt.z = rng.uniform(-range*0.5, range*0.5);

		vObjPts.push_back(objPt);

//		fprintf(file_objpts, "%lf	%lf	%lf\n", objPt.x,objPt.y,objPt.z);
	}
//	fclose(file_objpts);

	// simulate images /////////////////////////////////////////////////////////////////////////////////
	vector<Matx33d> Ks,K_es;
	vector<Matx33d> Rs,R_es;
	vector<Matx31d> ts,t_es;
	vector<Matx<double,5,1>> dists,dist_es;
	vector<int> distTypes;

	vector<uchar> j_fixed(nImg);		// indicates which images' parameters are fixed
	vector<uchar> i_fixed(nObj);		// indicates which object points' are fixed
	j_fixed[0] = 1;						// the 1st image's parameters are fixed

	Matx33d mK;
	mK(0,0) = mK(1,1) = f;
	mK(0,2) = cx; mK(1,2) = cy;
	mK(0,1) = s;  mK(2,2) = 1;
	Ks.push_back(mK);

	Matx33d mK_e;
	mK_e(0,0) = mK_e(1,1) = f_e;
	mK_e(0,2) = cx_e; mK_e(1,2) = cy_e;
	mK_e(0,1) = s_e;  mK_e(2,2) = 1;
	K_es.push_back(mK_e);

	Matx33d mR;
	mR(0,0)=mR(1,1)=mR(2,2)=1;
	Rs.push_back(mR);
	R_es.push_back(mR);

	Matx31d mt;
	mt(2) = distance;
	ts.push_back(mt);
	t_es.push_back(mt);

// 	FILE * file_cam = fopen("C:\\Users\\DeepVoid\\Desktop\\RT00.txt", "w");
// 	for (int i=0;i<3;++i)
// 	{
// 		fprintf(file_cam, "%lf	%lf	%lf	%lf\n", mR(i,0), mR(i,1), mR(i,2), mt(i));
// 	}
// 	fprintf(file_cam, "%lf	%lf	%lf	%lf", 0.0, 0.0, 0.0, 1.0);
// 	fclose(file_cam);

	Matx<double,5,1> dist;
	dist(0) = k1;	dist(1) = k2;	dist(2) = k3;	dist(3) = k4;	dist(4) = k5;
	dists.push_back(dist);

	Matx<double,5,1> dist_e;
	dist_e(0) = k1_e;	dist_e(1) = k2_e;	dist_e(2) = k3_e;	dist_e(3) = k4_e;	dist_e(4) = k5_e;
	dist_es.push_back(dist_e);

	distTypes.push_back(1);

	for (int j=1;j<nImg;++j)
	{
		// real parameters
		Ks.push_back(mK);
		ts.push_back(mt);
		dists.push_back(dist);

		double ang_Y = j*interval_ang;

		Matx33d dR = calib::converse_angY_R(ang_Y);
		Rs.push_back(dR*mR);

// 		CString strfile;
// 		strfile.Format("C:\\Users\\DeepVoid\\Desktop\\RT%02d.txt", j);
// 		file_cam = fopen(strfile, "w");
// 		for (int i=0;i<3;++i)
// 		{
// 			fprintf(file_cam, "%lf	%lf	%lf	%lf\n", Rs[j](i,0), Rs[j](i,1), Rs[j](i,2), mt(i));
// 		}
// 		fprintf(file_cam, "%lf	%lf	%lf	%lf", 0.0, 0.0, 0.0, 1.0);
// 		fclose(file_cam);

		// parameters with error, ie given estimates of image parameters
		K_es.push_back(mK_e);
		dist_es.push_back(dist_e);
		
		double drvx = rng.gaussian(rv_noise_std);
		double drvy = rng.gaussian(rv_noise_std);
		double drvz = rng.gaussian(rv_noise_std);

		dR = calib::converse_rotvec_R(drvx,drvy,drvz);
		R_es.push_back(dR*Rs[j]);

		double dtx = rng.gaussian(t_noise_std);
		double dty = rng.gaussian(t_noise_std);
		double dtz = rng.gaussian(t_noise_std);

		Matx31d mdt;
		mdt(0) = dtx;
		mdt(1) = dty;
		mdt(2) = dtz;

		t_es.push_back(mdt+ts[j]);

		distTypes.push_back(1);
	}

	vector<vector<double>> record_SBA(nImg), record_DSBA(nImg); // 记录每幅图像每个状态下统计出来的姿态角中误差

	FILE * file_SBA = fopen("C:\\Users\\DeepVoid\\Desktop\\error_SBA.txt", "w");
	FILE * file_DSBA = fopen("C:\\Users\\DeepVoid\\Desktop\\error_DSBA.txt", "w");

	// 递增每个物点被观测次数
	for (int iter=3;iter<=nImg;++iter)
	{
		CString strInfo;
		strInfo.Format("%03d iterations left", nImg-iter);
		pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

		nImgperPt = iter;

		// choose randomly the given number of images for each object point in which the object point is observed
		// and further choose randomly one reference image from this observing image set ////////////////////////
		vector<vector<int>> idxVisiImg;
		vector<int> idxRefVisiImg;
		for (int i=0;i<nObj;++i)
		{
			vector<int> idxVisi;

			while (idxVisi.size()<nImgperPt)
			{
				int k = rng.next()%nImg;

				if (find(idxVisi.begin(),idxVisi.end(),k) != idxVisi.end()) // found, already exist
				{
					continue;
				}

				idxVisi.push_back(k);
			}

			int k = rng.next()%nImgperPt;

			idxVisiImg.push_back(idxVisi);
			idxRefVisiImg.push_back(idxVisi[k]);
//			idxRefVisiImg.push_back(3);
		}

		// generate the visibility matrix /////////////////////////////////////////////////////////////////
		// and real observations without random noise /////////////////////////////////////////////////////
		int sizes[] = {nObj, nImg};
		SparseMat ptrMat(2,sizes,CV_32SC1);

		vector<Point2d> vImgPts_nonDist; // non-distorted image points
		vector<Point2d> vImgPts_dist;	 // distorted image points
		vector<Matx22d> covInvs;		 // inverses of all the covariance matrix of image points

		Matx22d I22;
		I22(0,0) = I22(1,1) = 1;

		for (int i=0;i<nObj;++i)
		{
			Point3d pt3d = vObjPts[i];

			Matx31d XYZ;
			XYZ(0) = pt3d.x;
			XYZ(1) = pt3d.y;
			XYZ(2) = pt3d.z;

			vector<int> idxVisi = idxVisiImg[i];
			int idxRef = idxRefVisiImg[i];

			sort(idxVisi.begin(), idxVisi.end());

			for (int k=0;k<idxVisi.size();++k)
			{
				int j = idxVisi[k];

				Matx33d K = Ks[j];	Matx33d K_e = K_es[j];
				Matx33d R = Rs[j];
				Matx31d t = ts[j];
				Matx<double,5,1> dist = dists[j];	Matx<double,5,1> dist_e = dist_es[j];

				Matx31d uv = R*XYZ+t;
				Matx31d xy = K*uv;

				double u = uv(0)/uv(2);
				double v = uv(1)/uv(2);

				double x = xy(0)/xy(2);
				double y = xy(1)/xy(2);

				double dx,dy;

				distortions::dxdy_brown(K(0,0),K(1,1),K(0,1),u,v,dist(0),dist(1),dist(2),dist(3),dist(4),dx,dy);

				double x_d = x+dx;
				double y_d = y+dy;

				Point2d imgPt;
				imgPt.x = x;
				imgPt.y = y;
				vImgPts_nonDist.push_back(imgPt);

				imgPt.x = x_d;
				imgPt.y = y_d;
				vImgPts_dist.push_back(imgPt);

				ptrMat.ref<int>(i,j) = vImgPts_nonDist.size()-1;

				covInvs.push_back(I22);
			}
		}

		vector<vector<double>> SBA(nImg),DSBA(nImg);

		fprintf(file_SBA,"%02d	", nImgperPt);
		fprintf(file_DSBA,"%02d	", nImgperPt);

		double t_total_SBA = 0; // SBA 总耗时
		double t_total_DSBA = 0; // DSBA 总耗时

		// run multiple times, add observation noises, and do statistics /////////////////////////////////
		for (int kk=0;kk<nSimu;++kk)
		{
			vector<Point2d> vImgPts_dist_err;// distorted image points with random noise
			vector<Matx31d> nxys;			 // contains non-distorted normalized reference image point of each object point

			for (int i=0;i<nObj;++i)
			{
				vector<int> idxVisi = idxVisiImg[i];
				int idxRef = idxRefVisiImg[i];

				sort(idxVisi.begin(), idxVisi.end());

				for (int k=0;k<idxVisi.size();++k)
				{
					int j = idxVisi[k];

					const int * ptr = ptrMat.find<int>(i,j);

					if (NULL==ptr)
					{
						continue;
					}

					Matx33d K_e = K_es[j];
					Matx<double,5,1> dist_e = dist_es[j];

					int idx_ij = (*ptr);

					Point2d pt2d = vImgPts_dist[idx_ij];

					double x_d = pt2d.x;
					double y_d = pt2d.y;

					double e_x = rng.gaussian(img_noise_std);
					double e_y = rng.gaussian(img_noise_std);

					double x_d_e = x_d + e_x;
					double y_d_e = y_d + e_y;

					if (j==idxRef)
					{
						// reference image point has random noise
// 						pt2d.x = x_d_e;
// 						pt2d.y = y_d_e;

						// reference image point has no random noise
						vImgPts_dist_err.push_back(pt2d);

						// remove distortion using given estimates of interior orientations
						double ideal_x,ideal_y;
						distortions::remove_brown(K_e(0,0),K_e(1,1),K_e(0,2),K_e(1,2),K_e(0,1),
							dist_e(0),dist_e(1),dist_e(2),dist_e(3),dist_e(4),pt2d.x,pt2d.y,ideal_x,ideal_y);

						Matx31d nxy;
						nxy(0) = ideal_x;
						nxy(1) = ideal_y;
						nxy(2) = 1;

						// compute the normalized image point using given estimates of interior orientations
						nxys.push_back(calib::invK(K_e)*nxy);
					} 
					else
					{
						// non-reference image point has random noise
						pt2d.x = x_d_e;
						pt2d.y = y_d_e;
						vImgPts_dist_err.push_back(pt2d);
					}
				}
			}

			double info[5];

			// run SBA first //////////////////////////////////////////////////////////////////////////
			vector<Point3d> objPts_SBA = vObjPts;
			vector<Matx33d> Rs_SBA = R_es;
			vector<Matx31d> ts_SBA = t_es;

			tic();
			SBA_ZZK::optim_sparse_lm_wj_tj_XiYiZi(objPts_SBA,K_es,Rs_SBA,ts_SBA,dist_es,distTypes,vImgPts_dist_err,covInvs,
				j_fixed,i_fixed,ptrMat,info);
			double t_SBA = toc();
//			t_total_SBA += t_SBA;
			t_total_SBA += t_SBA/info[3];

			vector<double> drads_SBA,drads_DSBA;
			for (int j=0;j<nImg;++j)
			{
				Matx33d dR = Rs_SBA[j].t()*Rs[j];
				Matx31d rov = calib::converse_R_rotvec(dR);
				double drad = norm(rov);
				drads_SBA.push_back(drad*calib::R2D);

				SBA[j].push_back(drad*calib::R2D);
			}

			// then run DSBA //////////////////////////////////////////////////////////////////////////
			vector<Point3d> objPts_DSBA = vObjPts;
			vector<Matx33d> Rs_DSBA = R_es;
			vector<Matx31d> ts_DSBA = t_es;

			tic();
			SBA_ZZK::optim_sparse_lm_wj_tj_di(objPts_DSBA,K_es,Rs_DSBA,ts_DSBA,dist_es,distTypes,vImgPts_dist_err,covInvs,
				nxys,idxRefVisiImg,j_fixed,i_fixed,ptrMat,info);
			double t_DSBA = toc();
//			t_total_DSBA += t_DSBA;
			t_total_DSBA += t_DSBA/info[3];

			for (int j=0;j<nImg;++j)
			{
				Matx33d dR = Rs_DSBA[j].t()*Rs[j];
				Matx31d rov = calib::converse_R_rotvec(dR);
				double drad = norm(rov);
				drads_DSBA.push_back(drad*calib::R2D);

				DSBA[j].push_back(drad*calib::R2D);
			}
		}

		double t_mean_SBA = 1000*t_total_SBA/nSimu; // 单次执行毫秒
		double t_mean_DSBA = 1000*t_total_DSBA/nSimu; // 单次执行毫秒

		vector<vector<double>> dSBA_DSBA(nImg);
		vector<double> davg_SBA_DSBA;
		vector<double> dds_rms_SBA;
		vector<double> dds_rms_BSBA;
		for (int j=0;j<nImg;++j)
		{
			double sum = 0;

			double sum_d2_SBA = 0;
			double sum_d2_DSBA = 0;

			for (int k=0;k<SBA[j].size();++k)
			{
				double dd_SBA = SBA[j][k];
				double dd_DSBA = DSBA[j][k];

				sum += dd_SBA-dd_DSBA;

				sum_d2_SBA += dd_SBA*dd_SBA;
				sum_d2_DSBA += dd_DSBA*dd_DSBA;

				dSBA_DSBA[j].push_back(dd_SBA-dd_DSBA);
			}

			davg_SBA_DSBA.push_back(sum/SBA[j].size());

			double rms_dd_SBA = sqrt(sum_d2_SBA/SBA[j].size());
			double rms_dd_DSBA = sqrt(sum_d2_DSBA/DSBA[j].size());

			dds_rms_SBA.push_back(rms_dd_SBA);
			dds_rms_BSBA.push_back(rms_dd_DSBA);

			record_SBA[j].push_back(rms_dd_SBA);
			record_DSBA[j].push_back(rms_dd_DSBA);

			fprintf(file_SBA,"%lf	", rms_dd_SBA);
			fprintf(file_DSBA,"%lf	", rms_dd_DSBA);
		}
		fprintf(file_SBA,"%lf\n", t_mean_SBA);
		fprintf(file_DSBA,"%lf\n", t_mean_DSBA);
	}
	fclose(file_SBA);
	fclose(file_DSBA);
	
	return TRUE;
}

UINT DSBA_Simu_increaseObjpts(LPVOID param)
{
	CDeepVoidApp * pApp = (CDeepVoidApp * )param;

	RNG rng(0xffffffff);			// Initializes a random number generator state
//	RNG rng((unsigned)time(NULL));	// Initializes a random number generator state

	// parameters setting /////////////////////////////////////////////////////////////////////////////
	int nObj = 30;				// number of object points
	int nImg = 7;				// number of images
	int nImgperPt = 5;			// each object point is observed by this many images
	double range = 1;			// the spread of object points
	double distance = 3;		// distance between camera and object points center
	double interval_ang = 15;	// the angular interval between neighboring images (in degree)
	double img_noise_std = 0.1;	// standard deviation of image point error
	double t_noise_std = 0.1;	// standard deviation of the initial translation vector
	double rad_noise_std = 0.5*calib::D2R;			// standard deviation of the initial rotation, in radian
	double rv_noise_std = rad_noise_std/sqrt(3.0);	// standard deviation of the initial rotation vector element
	int nSimu = 200;			// the times of simulation in order to do the statistics
	int nObj_min = 10;
//	int nObj_max = 100;
	int nObj_step = 10;
	int nObj_n = 30;

	double f = 1500;			// equivalent focal length
	double w = 1280;			// image width
	double h = 720;				// image height
	double cx = 0.5*(w-1);		// principal point
	double cy = 0.5*(h-1);		// principal point
	double s = 0;				// screw factor

	double f_e = f + 0;		// equivalent focal length with error
	double cx_e = cx + 0;		// principal point with error
	double cy_e = cy + 0;		// principal point with error
	double s_e = s + 0;			// screw factor with error

	double k1 = 0.01;			// 2nd order radial distortion coefficient
	double k2 = -0.02;			// 4th order radial distortion coefficient
	double k3 = 0.003;			// one tangential distortion coefficient
	double k4 = 0.002;			// the other tangential distortion coefficient
	double k5 = -0.001;			// 6th order radial distortion coefficient

	double k1_e = k1 + 0;		// 2nd order radial distortion coefficient with error
	double k2_e = k2 + 0;		// 4th order radial distortion coefficient with error
	double k3_e = k3 + 0;		// one tangential distortion coefficient with error 
	double k4_e = k4 + 0;		// the other tangential distortion coefficient with error
	double k5_e = k5 + 0;		// 6th order radial distortion coefficient with error

	vector<vector<double>> record_SBA(nImg), record_DSBA(nImg); // 记录每幅图像每个状态下统计出来的姿态角中误差

	FILE * file_SBA = fopen("C:\\Users\\DeepVoid\\Desktop\\error_SBA.txt", "w");
	FILE * file_DSBA = fopen("C:\\Users\\DeepVoid\\Desktop\\error_DSBA.txt", "w");

	for (int iter=0;iter<nObj_n;++iter)
	{
		CString strInfo;
		strInfo.Format("%03d iterations left", nObj_n-1-iter);
		pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

		nObj = nObj_min + iter*nObj_step;

		// simulate object points //////////////////////////////////////////////////////////////////////////
		vector<Point3d> vObjPts;
		for (int i=0;i<nObj;++i)
		{
			Point3d objPt;

			objPt.x = rng.uniform(-range*0.5, range*0.5);
			objPt.y = rng.uniform(-range*0.5, range*0.5);
			objPt.z = rng.uniform(-range*0.5, range*0.5);

			vObjPts.push_back(objPt);
		}

		// simulate images /////////////////////////////////////////////////////////////////////////////////
		vector<Matx33d> Ks,K_es;
		vector<Matx33d> Rs,R_es;
		vector<Matx31d> ts,t_es;
		vector<Matx<double,5,1>> dists,dist_es;
		vector<int> distTypes;

		vector<uchar> j_fixed(nImg);		// indicates which images' parameters are fixed
		vector<uchar> i_fixed(nObj);		// indicates which object points' are fixed
		j_fixed[0] = 1;						// the 1st image's parameters are fixed

		Matx33d mK;
		mK(0,0) = mK(1,1) = f;
		mK(0,2) = cx; mK(1,2) = cy;
		mK(0,1) = s;  mK(2,2) = 1;
		Ks.push_back(mK);

		Matx33d mK_e;
		mK_e(0,0) = mK_e(1,1) = f_e;
		mK_e(0,2) = cx_e; mK_e(1,2) = cy_e;
		mK_e(0,1) = s_e;  mK_e(2,2) = 1;
		K_es.push_back(mK_e);

		Matx33d mR;
		mR(0,0)=mR(1,1)=mR(2,2)=1;
		Rs.push_back(mR);
		R_es.push_back(mR);

		Matx31d mt;
		mt(2) = distance;
		ts.push_back(mt);
		t_es.push_back(mt);

		Matx<double,5,1> dist;
		dist(0) = k1;	dist(1) = k2;	dist(2) = k3;	dist(3) = k4;	dist(4) = k5;
		dists.push_back(dist);

		Matx<double,5,1> dist_e;
		dist_e(0) = k1_e;	dist_e(1) = k2_e;	dist_e(2) = k3_e;	dist_e(3) = k4_e;	dist_e(4) = k5_e;
		dist_es.push_back(dist_e);

		distTypes.push_back(1);

		for (int j=1;j<nImg;++j)
		{
			// real parameters
			Ks.push_back(mK);
			ts.push_back(mt);
			dists.push_back(dist);

			double ang_Y = j*interval_ang;

			Matx33d dR = calib::converse_angY_R(ang_Y);
			Rs.push_back(dR*mR);

			// parameters with error, ie given estimates of image parameters
			K_es.push_back(mK_e);
			dist_es.push_back(dist_e);

			double drvx = rng.gaussian(rv_noise_std);
			double drvy = rng.gaussian(rv_noise_std);
			double drvz = rng.gaussian(rv_noise_std);

			dR = calib::converse_rotvec_R(drvx,drvy,drvz);
			R_es.push_back(dR*Rs[j]);

			double dtx = rng.gaussian(t_noise_std);
			double dty = rng.gaussian(t_noise_std);
			double dtz = rng.gaussian(t_noise_std);

			Matx31d mdt;
			mdt(0) = dtx;
			mdt(1) = dty;
			mdt(2) = dtz;

			t_es.push_back(mdt+ts[j]);

			distTypes.push_back(1);
		}

		// choose randomly the given number of images for each object point in which the object point is observed
		// and further choose randomly one reference image from this observing image set ////////////////////////
		vector<vector<int>> idxVisiImg;
		vector<int> idxRefVisiImg;
		for (int i=0;i<nObj;++i)
		{
			vector<int> idxVisi;

			while (idxVisi.size()<nImgperPt)
			{
				int k = rng.next()%nImg;

				if (find(idxVisi.begin(),idxVisi.end(),k) != idxVisi.end()) // found, already exist
				{
					continue;
				}

				idxVisi.push_back(k);
			}

			int k = rng.next()%nImgperPt;

			idxVisiImg.push_back(idxVisi);
			idxRefVisiImg.push_back(idxVisi[k]);
		}

		// generate the visibility matrix /////////////////////////////////////////////////////////////////
		// and real observations without random noise /////////////////////////////////////////////////////
		int sizes[] = {nObj, nImg};
		SparseMat ptrMat(2,sizes,CV_32SC1);

		vector<Point2d> vImgPts_nonDist; // non-distorted image points
		vector<Point2d> vImgPts_dist;	 // distorted image points
		vector<Matx22d> covInvs;		 // inverses of all the covariance matrix of image points

		Matx22d I22;
		I22(0,0) = I22(1,1) = 1;

		for (int i=0;i<nObj;++i)
		{
			Point3d pt3d = vObjPts[i];

			Matx31d XYZ;
			XYZ(0) = pt3d.x;
			XYZ(1) = pt3d.y;
			XYZ(2) = pt3d.z;

			vector<int> idxVisi = idxVisiImg[i];
			int idxRef = idxRefVisiImg[i];

			sort(idxVisi.begin(), idxVisi.end());

			for (int k=0;k<idxVisi.size();++k)
			{
				int j = idxVisi[k];

				Matx33d K = Ks[j];	Matx33d K_e = K_es[j];
				Matx33d R = Rs[j];
				Matx31d t = ts[j];
				Matx<double,5,1> dist = dists[j];	Matx<double,5,1> dist_e = dist_es[j];

				Matx31d uv = R*XYZ+t;
				Matx31d xy = K*uv;

				double u = uv(0)/uv(2);
				double v = uv(1)/uv(2);

				double x = xy(0)/xy(2);
				double y = xy(1)/xy(2);

				double dx,dy;

				distortions::dxdy_brown(K(0,0),K(1,1),K(0,1),u,v,dist(0),dist(1),dist(2),dist(3),dist(4),dx,dy);

				double x_d = x+dx;
				double y_d = y+dy;

				Point2d imgPt;
				imgPt.x = x;
				imgPt.y = y;
				vImgPts_nonDist.push_back(imgPt);

				imgPt.x = x_d;
				imgPt.y = y_d;
				vImgPts_dist.push_back(imgPt);

				ptrMat.ref<int>(i,j) = vImgPts_nonDist.size()-1;

				covInvs.push_back(I22);
			}
		}

		vector<vector<double>> SBA(nImg),DSBA(nImg);

		fprintf(file_SBA,"%05d	", nObj);
		fprintf(file_DSBA,"%05d	", nObj);

		double t_total_SBA = 0; // SBA 总耗时
		double t_total_DSBA = 0; // DSBA 总耗时

		// run multiple times, add observation noises, and do statistics /////////////////////////////////
		for (int kk=0;kk<nSimu;++kk)
		{
			vector<Point2d> vImgPts_dist_err;// distorted image points with random noise
			vector<Matx31d> nxys;			 // contains non-distorted normalized reference image point of each object point

			for (int i=0;i<nObj;++i)
			{
				vector<int> idxVisi = idxVisiImg[i];
				int idxRef = idxRefVisiImg[i];

				sort(idxVisi.begin(), idxVisi.end());

				for (int k=0;k<idxVisi.size();++k)
				{
					int j = idxVisi[k];

					const int * ptr = ptrMat.find<int>(i,j);

					if (NULL==ptr)
					{
						continue;
					}

					Matx33d K_e = K_es[j];
					Matx<double,5,1> dist_e = dist_es[j];

					int idx_ij = (*ptr);

					Point2d pt2d = vImgPts_dist[idx_ij];

					double x_d = pt2d.x;
					double y_d = pt2d.y;

					double e_x = rng.gaussian(img_noise_std);
					double e_y = rng.gaussian(img_noise_std);

					double x_d_e = x_d + e_x;
					double y_d_e = y_d + e_y;

					if (j==idxRef)
					{
						// reference image point has random noise
// 						pt2d.x = x_d_e;
// 						pt2d.y = y_d_e;

						// reference image point has no random noise
						vImgPts_dist_err.push_back(pt2d);

						// remove distortion using given estimates of interior orientations
						double ideal_x,ideal_y;
						distortions::remove_brown(K_e(0,0),K_e(1,1),K_e(0,2),K_e(1,2),K_e(0,1),
							dist_e(0),dist_e(1),dist_e(2),dist_e(3),dist_e(4),pt2d.x,pt2d.y,ideal_x,ideal_y);

						Matx31d nxy;
						nxy(0) = ideal_x;
						nxy(1) = ideal_y;
						nxy(2) = 1;

						// compute the normalized image point using given estimates of interior orientations
						nxys.push_back(calib::invK(K_e)*nxy);
					} 
					else
					{
						// non-reference image point has random noise
						pt2d.x = x_d_e;
						pt2d.y = y_d_e;
						vImgPts_dist_err.push_back(pt2d);
					}
				}
			}

			double info[5];

			// run SBA first //////////////////////////////////////////////////////////////////////////
			vector<Point3d> objPts_SBA = vObjPts;
			vector<Matx33d> Rs_SBA = R_es;
			vector<Matx31d> ts_SBA = t_es;

			tic();
			SBA_ZZK::optim_sparse_lm_wj_tj_XiYiZi(objPts_SBA,K_es,Rs_SBA,ts_SBA,dist_es,distTypes,vImgPts_dist_err,covInvs,
				j_fixed,i_fixed,ptrMat,info);
			double t_SBA = toc();
//			t_total_SBA += t_SBA;
			t_total_SBA += t_SBA/info[3];

			vector<double> drads_SBA,drads_DSBA;
			for (int j=0;j<nImg;++j)
			{
				Matx33d dR = Rs_SBA[j].t()*Rs[j];
				Matx31d rov = calib::converse_R_rotvec(dR);
				double drad = norm(rov);
				drads_SBA.push_back(drad*calib::R2D);

				SBA[j].push_back(drad*calib::R2D);
			}

			// then run DSBA //////////////////////////////////////////////////////////////////////////
			vector<Point3d> objPts_DSBA = vObjPts;
			vector<Matx33d> Rs_DSBA = R_es;
			vector<Matx31d> ts_DSBA = t_es;

			tic();
			SBA_ZZK::optim_sparse_lm_wj_tj_di(objPts_DSBA,K_es,Rs_DSBA,ts_DSBA,dist_es,distTypes,vImgPts_dist_err,covInvs,
				nxys,idxRefVisiImg,j_fixed,i_fixed,ptrMat,info);
			double t_DSBA = toc();
//			t_total_DSBA += t_DSBA;
			t_total_DSBA += t_DSBA/info[3];

			for (int j=0;j<nImg;++j)
			{
				Matx33d dR = Rs_DSBA[j].t()*Rs[j];
				Matx31d rov = calib::converse_R_rotvec(dR);
				double drad = norm(rov);
				drads_DSBA.push_back(drad*calib::R2D);

				DSBA[j].push_back(drad*calib::R2D);
			}
		}

		double t_mean_SBA = 1000*t_total_SBA/nSimu; // 单次执行毫秒
		double t_mean_DSBA = 1000*t_total_DSBA/nSimu; // 单次执行毫秒

		vector<vector<double>> dSBA_DSBA(nImg);
		vector<double> davg_SBA_DSBA;
		vector<double> dds_rms_SBA;
		vector<double> dds_rms_BSBA;
		for (int j=0;j<nImg;++j)
		{
			double sum = 0;

			double sum_d2_SBA = 0;
			double sum_d2_DSBA = 0;

			for (int k=0;k<SBA[j].size();++k)
			{
				double dd_SBA = SBA[j][k];
				double dd_DSBA = DSBA[j][k];

				sum += dd_SBA-dd_DSBA;

				sum_d2_SBA += dd_SBA*dd_SBA;
				sum_d2_DSBA += dd_DSBA*dd_DSBA;

				dSBA_DSBA[j].push_back(dd_SBA-dd_DSBA);
			}

			davg_SBA_DSBA.push_back(sum/SBA[j].size());

			double rms_dd_SBA = sqrt(sum_d2_SBA/SBA[j].size());
			double rms_dd_DSBA = sqrt(sum_d2_DSBA/DSBA[j].size());

			dds_rms_SBA.push_back(rms_dd_SBA);
			dds_rms_BSBA.push_back(rms_dd_DSBA);

			record_SBA[j].push_back(rms_dd_SBA);
			record_DSBA[j].push_back(rms_dd_DSBA);

			fprintf(file_SBA,"%lf	", rms_dd_SBA);
			fprintf(file_DSBA,"%lf	", rms_dd_DSBA);
		}
		fprintf(file_SBA,"%lf\n", t_mean_SBA);
		fprintf(file_DSBA,"%lf\n", t_mean_DSBA);
	}
	fclose(file_SBA);
	fclose(file_DSBA);

	return TRUE;
}

UINT DSBA_Simu_increaseImgs(LPVOID param)
{
	CDeepVoidApp * pApp = (CDeepVoidApp * )param;

	RNG rng(0xffffffff);			// Initializes a random number generator state
//	RNG rng((unsigned)time(NULL));	// Initializes a random number generator state

	// parameters setting /////////////////////////////////////////////////////////////////////////////
	int nObj = 50;				// number of object points
	int nImg = 7;				// number of images
	int nImgperPt = 3;			// each object point is observed by this many images
	double range = 1;			// the spread of object points
	double distance = 3;		// distance between camera and object points center
	double interval_ang = 15;	// the angular interval between neighboring images (in degree)
	double img_noise_std = 0.1;	// standard deviation of image point error
	double t_noise_std = 0.1;	// standard deviation of the initial translation vector
	double rad_noise_std = 0.5*calib::D2R;			// standard deviation of the initial rotation, in radian
	double rv_noise_std = rad_noise_std/sqrt(3.0);	// standard deviation of the initial rotation vector element
	int nSimu = 200;			// the times of simulation in order to do the statistics
	int nImg_min = 3;
	int nImg_step = 1;
	int nImg_n = 18;

	double f = 1500;			// equivalent focal length
	double w = 1280;			// image width
	double h = 720;				// image height
	double cx = 0.5*(w-1);		// principal point
	double cy = 0.5*(h-1);		// principal point
	double s = 0;				// screw factor

	double f_e = f + 0;		// equivalent focal length with error
	double cx_e = cx + 0;		// principal point with error
	double cy_e = cy + 0;		// principal point with error
	double s_e = s + 0;			// screw factor with error

	double k1 = 0.01;			// 2nd order radial distortion coefficient
	double k2 = -0.02;			// 4th order radial distortion coefficient
	double k3 = 0.003;			// one tangential distortion coefficient
	double k4 = 0.002;			// the other tangential distortion coefficient
	double k5 = -0.001;			// 6th order radial distortion coefficient

	double k1_e = k1 + 0;		// 2nd order radial distortion coefficient with error
	double k2_e = k2 + 0;		// 4th order radial distortion coefficient with error
	double k3_e = k3 + 0;		// one tangential distortion coefficient with error 
	double k4_e = k4 + 0;		// the other tangential distortion coefficient with error
	double k5_e = k5 + 0;		// 6th order radial distortion coefficient with error

//	vector<vector<double>> record_SBA(nImg), record_DSBA(nImg); // 记录每幅图像每个状态下统计出来的姿态角中误差

	FILE * file_SBA = fopen("C:\\Users\\DeepVoid\\Desktop\\error_SBA.txt", "w");
	FILE * file_DSBA = fopen("C:\\Users\\DeepVoid\\Desktop\\error_DSBA.txt", "w");

	for (int iter=0;iter<nImg_n;++iter)
	{
		CString strInfo;
		strInfo.Format("%03d iterations left", nImg_n-1-iter);
		pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

		nImg = nImg_min + iter*nImg_step;

		// simulate object points //////////////////////////////////////////////////////////////////////////
		vector<Point3d> vObjPts;
		for (int i=0;i<nObj;++i)
		{
			Point3d objPt;

			objPt.x = rng.uniform(-range*0.5, range*0.5);
			objPt.y = rng.uniform(-range*0.5, range*0.5);
			objPt.z = rng.uniform(-range*0.5, range*0.5);

			vObjPts.push_back(objPt);
		}

		// simulate images /////////////////////////////////////////////////////////////////////////////////
		vector<Matx33d> Ks,K_es;
		vector<Matx33d> Rs,R_es;
		vector<Matx31d> ts,t_es;
		vector<Matx<double,5,1>> dists,dist_es;
		vector<int> distTypes;

		vector<uchar> j_fixed(nImg);		// indicates which images' parameters are fixed
		vector<uchar> i_fixed(nObj);		// indicates which object points' are fixed
		j_fixed[0] = 1;						// the 1st image's parameters are fixed

		Matx33d mK;
		mK(0,0) = mK(1,1) = f;
		mK(0,2) = cx; mK(1,2) = cy;
		mK(0,1) = s;  mK(2,2) = 1;
		Ks.push_back(mK);

		Matx33d mK_e;
		mK_e(0,0) = mK_e(1,1) = f_e;
		mK_e(0,2) = cx_e; mK_e(1,2) = cy_e;
		mK_e(0,1) = s_e;  mK_e(2,2) = 1;
		K_es.push_back(mK_e);

		Matx33d mR;
		mR(0,0)=mR(1,1)=mR(2,2)=1;
		Rs.push_back(mR);
		R_es.push_back(mR);

		Matx31d mt;
		mt(2) = distance;
		ts.push_back(mt);
		t_es.push_back(mt);

		Matx<double,5,1> dist;
		dist(0) = k1;	dist(1) = k2;	dist(2) = k3;	dist(3) = k4;	dist(4) = k5;
		dists.push_back(dist);

		Matx<double,5,1> dist_e;
		dist_e(0) = k1_e;	dist_e(1) = k2_e;	dist_e(2) = k3_e;	dist_e(3) = k4_e;	dist_e(4) = k5_e;
		dist_es.push_back(dist_e);

		distTypes.push_back(1);

		for (int j=1;j<nImg;++j)
		{
			// real parameters
			Ks.push_back(mK);
			ts.push_back(mt);
			dists.push_back(dist);

			double ang_Y = j*interval_ang;

			Matx33d dR = calib::converse_angY_R(ang_Y);
			Rs.push_back(dR*mR);

			// parameters with error, ie given estimates of image parameters
			K_es.push_back(mK_e);
			dist_es.push_back(dist_e);

			double drvx = rng.gaussian(rv_noise_std);
			double drvy = rng.gaussian(rv_noise_std);
			double drvz = rng.gaussian(rv_noise_std);

			dR = calib::converse_rotvec_R(drvx,drvy,drvz);
			R_es.push_back(dR*Rs[j]);

			double dtx = rng.gaussian(t_noise_std);
			double dty = rng.gaussian(t_noise_std);
			double dtz = rng.gaussian(t_noise_std);

			Matx31d mdt;
			mdt(0) = dtx;
			mdt(1) = dty;
			mdt(2) = dtz;

			t_es.push_back(mdt+ts[j]);

			distTypes.push_back(1);
		}

		// choose randomly the given number of images for each object point in which the object point is observed
		// and further choose randomly one reference image from this observing image set ////////////////////////
		vector<vector<int>> idxVisiImg;
		vector<int> idxRefVisiImg;
		for (int i=0;i<nObj;++i)
		{
			vector<int> idxVisi;

			while (idxVisi.size()<nImgperPt)
			{
				int k = rng.next()%nImg;

				if (find(idxVisi.begin(),idxVisi.end(),k) != idxVisi.end()) // found, already exist
				{
					continue;
				}

				idxVisi.push_back(k);
			}

			int k = rng.next()%nImgperPt;

			idxVisiImg.push_back(idxVisi);
			idxRefVisiImg.push_back(idxVisi[k]);
		}

		// generate the visibility matrix /////////////////////////////////////////////////////////////////
		// and real observations without random noise /////////////////////////////////////////////////////
		int sizes[] = {nObj, nImg};
		SparseMat ptrMat(2,sizes,CV_32SC1);

		vector<Point2d> vImgPts_nonDist; // non-distorted image points
		vector<Point2d> vImgPts_dist;	 // distorted image points
		vector<Matx22d> covInvs;		 // inverses of all the covariance matrix of image points

		Matx22d I22;
		I22(0,0) = I22(1,1) = 1;

		for (int i=0;i<nObj;++i)
		{
			Point3d pt3d = vObjPts[i];

			Matx31d XYZ;
			XYZ(0) = pt3d.x;
			XYZ(1) = pt3d.y;
			XYZ(2) = pt3d.z;

			vector<int> idxVisi = idxVisiImg[i];
			int idxRef = idxRefVisiImg[i];

			sort(idxVisi.begin(), idxVisi.end());

			for (int k=0;k<idxVisi.size();++k)
			{
				int j = idxVisi[k];

				Matx33d K = Ks[j];	Matx33d K_e = K_es[j];
				Matx33d R = Rs[j];
				Matx31d t = ts[j];
				Matx<double,5,1> dist = dists[j];	Matx<double,5,1> dist_e = dist_es[j];

				Matx31d uv = R*XYZ+t;
				Matx31d xy = K*uv;

				double u = uv(0)/uv(2);
				double v = uv(1)/uv(2);

				double x = xy(0)/xy(2);
				double y = xy(1)/xy(2);

				double dx,dy;

				distortions::dxdy_brown(K(0,0),K(1,1),K(0,1),u,v,dist(0),dist(1),dist(2),dist(3),dist(4),dx,dy);

				double x_d = x+dx;
				double y_d = y+dy;

				Point2d imgPt;
				imgPt.x = x;
				imgPt.y = y;
				vImgPts_nonDist.push_back(imgPt);

				imgPt.x = x_d;
				imgPt.y = y_d;
				vImgPts_dist.push_back(imgPt);

				ptrMat.ref<int>(i,j) = vImgPts_nonDist.size()-1;

				covInvs.push_back(I22);
			}
		}

		vector<vector<double>> SBA(nImg),DSBA(nImg);

		fprintf(file_SBA,"%05d	", nImg);
		fprintf(file_DSBA,"%05d	", nImg);

		double t_total_SBA = 0; // SBA 总耗时
		double t_total_DSBA = 0; // DSBA 总耗时

		// run multiple times, add observation noises, and do statistics /////////////////////////////////
		for (int kk=0;kk<nSimu;++kk)
		{
			vector<Point2d> vImgPts_dist_err;// distorted image points with random noise
			vector<Matx31d> nxys;			 // contains non-distorted normalized reference image point of each object point

			for (int i=0;i<nObj;++i)
			{
				vector<int> idxVisi = idxVisiImg[i];
				int idxRef = idxRefVisiImg[i];

				sort(idxVisi.begin(), idxVisi.end());

				for (int k=0;k<idxVisi.size();++k)
				{
					int j = idxVisi[k];

					const int * ptr = ptrMat.find<int>(i,j);

					if (NULL==ptr)
					{
						continue;
					}

					Matx33d K_e = K_es[j];
					Matx<double,5,1> dist_e = dist_es[j];

					int idx_ij = (*ptr);

					Point2d pt2d = vImgPts_dist[idx_ij];

					double x_d = pt2d.x;
					double y_d = pt2d.y;

					double e_x = rng.gaussian(img_noise_std);
					double e_y = rng.gaussian(img_noise_std);

					double x_d_e = x_d + e_x;
					double y_d_e = y_d + e_y;

					if (j==idxRef)
					{
						// reference image point has random noise
// 						pt2d.x = x_d_e;
// 						pt2d.y = y_d_e;

						// reference image point has no random noise
						vImgPts_dist_err.push_back(pt2d);

						// remove distortion using given estimates of interior orientations
						double ideal_x,ideal_y;
						distortions::remove_brown(K_e(0,0),K_e(1,1),K_e(0,2),K_e(1,2),K_e(0,1),
							dist_e(0),dist_e(1),dist_e(2),dist_e(3),dist_e(4),pt2d.x,pt2d.y,ideal_x,ideal_y);

						Matx31d nxy;
						nxy(0) = ideal_x;
						nxy(1) = ideal_y;
						nxy(2) = 1;

						// compute the normalized image point using given estimates of interior orientations
						nxys.push_back(calib::invK(K_e)*nxy);
					} 
					else
					{
						// non-reference image point has random noise
						pt2d.x = x_d_e;
						pt2d.y = y_d_e;
						vImgPts_dist_err.push_back(pt2d);
					}
				}
			}

			double info[5];

			// run SBA first //////////////////////////////////////////////////////////////////////////
			vector<Point3d> objPts_SBA = vObjPts;
			vector<Matx33d> Rs_SBA = R_es;
			vector<Matx31d> ts_SBA = t_es;

			tic();
			SBA_ZZK::optim_sparse_lm_wj_tj_XiYiZi(objPts_SBA,K_es,Rs_SBA,ts_SBA,dist_es,distTypes,vImgPts_dist_err,covInvs,
				j_fixed,i_fixed,ptrMat,info);
			double t_SBA = toc();
//			t_total_SBA += t_SBA;
			t_total_SBA += t_SBA/info[3];

			vector<double> drads_SBA,drads_DSBA;
			for (int j=0;j<nImg;++j)
			{
				Matx33d dR = Rs_SBA[j].t()*Rs[j];
				Matx31d rov = calib::converse_R_rotvec(dR);
				double drad = norm(rov);
				drads_SBA.push_back(drad*calib::R2D);

				SBA[j].push_back(drad*calib::R2D);
			}

			// then run DSBA //////////////////////////////////////////////////////////////////////////
			vector<Point3d> objPts_DSBA = vObjPts;
			vector<Matx33d> Rs_DSBA = R_es;
			vector<Matx31d> ts_DSBA = t_es;

			tic();
			SBA_ZZK::optim_sparse_lm_wj_tj_di(objPts_DSBA,K_es,Rs_DSBA,ts_DSBA,dist_es,distTypes,vImgPts_dist_err,covInvs,
				nxys,idxRefVisiImg,j_fixed,i_fixed,ptrMat,info);
			double t_DSBA = toc();
//			t_total_DSBA += t_DSBA;
			t_total_DSBA += t_DSBA/info[3];

			for (int j=0;j<nImg;++j)
			{
				Matx33d dR = Rs_DSBA[j].t()*Rs[j];
				Matx31d rov = calib::converse_R_rotvec(dR);
				double drad = norm(rov);
				drads_DSBA.push_back(drad*calib::R2D);

				DSBA[j].push_back(drad*calib::R2D);
			}
		}

		double t_mean_SBA = 1000*t_total_SBA/nSimu; // 单次执行毫秒
		double t_mean_DSBA = 1000*t_total_DSBA/nSimu; // 单次执行毫秒

		vector<vector<double>> dSBA_DSBA(nImg);
		vector<double> davg_SBA_DSBA;
		vector<double> dds_rms_SBA;
		vector<double> dds_rms_BSBA;
		for (int j=0;j<nImg;++j)
		{
			double sum = 0;

			double sum_d2_SBA = 0;
			double sum_d2_DSBA = 0;

			for (int k=0;k<SBA[j].size();++k)
			{
				double dd_SBA = SBA[j][k];
				double dd_DSBA = DSBA[j][k];

				sum += dd_SBA-dd_DSBA;

				sum_d2_SBA += dd_SBA*dd_SBA;
				sum_d2_DSBA += dd_DSBA*dd_DSBA;

				dSBA_DSBA[j].push_back(dd_SBA-dd_DSBA);
			}

			davg_SBA_DSBA.push_back(sum/SBA[j].size());

			double rms_dd_SBA = sqrt(sum_d2_SBA/SBA[j].size());
			double rms_dd_DSBA = sqrt(sum_d2_DSBA/DSBA[j].size());

			dds_rms_SBA.push_back(rms_dd_SBA);
			dds_rms_BSBA.push_back(rms_dd_DSBA);

// 			record_SBA[j].push_back(rms_dd_SBA);
// 			record_DSBA[j].push_back(rms_dd_DSBA);

			fprintf(file_SBA,"%lf	", rms_dd_SBA);
			fprintf(file_DSBA,"%lf	", rms_dd_DSBA);
		}
		fprintf(file_SBA,"%lf\n", t_mean_SBA);
		fprintf(file_DSBA,"%lf\n", t_mean_DSBA);
	}
	fclose(file_SBA);
	fclose(file_DSBA);

	return TRUE;
}

void CDeepVoidApp::OnDsbasimu()
{
	// TODO: Add your command handler code here
	AfxBeginThread(DSBA_Simu_increaseImgPtNoise, this, THREAD_PRIORITY_NORMAL);
//	AfxBeginThread(DSBA_Simu_increaseObservations, this, THREAD_PRIORITY_NORMAL);
//	AfxBeginThread(DSBA_Simu_increaseObjpts, this, THREAD_PRIORITY_NORMAL);
//	AfxBeginThread(DSBA_Simu_increaseImgs, this, THREAD_PRIORITY_NORMAL);
}

UINT SfM_Simu(LPVOID param)
{
	CDeepVoidApp * pApp = (CDeepVoidApp * )param;

// 	std::map<int,int> map_map;
// 
// 	map_map.insert(make_pair(0,1));
// 	map_map.insert(make_pair(1,3));
// 	map_map.insert(make_pair(0,5));

	Matx33d K;
	K(0,0) = 849.812399662762910; 
	K(1,1) = 849.028798494762330;
	K(0,2) = 401.558250358447480;  
	K(1,2) = 273.689303692577480;
	K(0,1) = 0;
	K(2,2) = 1;
	
	Matx<double,5,1> dist;
	dist(0) = -0.039256019093033;
	dist(1) = 0.171222750044158 ;
	dist(2) = 0.001771298684322;
	dist(3) = 0.001188277543124;
	dist(4) = 0.000000000000000;	

	int m = 20;

	CString fileName;
	for (int i=0;i<m;++i)
	{
//		fileName.Format("E:\\paper\\PhD Thesis\\data\\20151206 橘子洲头毛泽东像\\mao\\800×534\\A%02d.JPG", i);
		fileName.Format("E:\\paper\\PhD Thesis\\data\\20151206 橘子洲头毛泽东像\\horse\\800×534\\A%02d.JPG", i);

		Mat img = imread(fileName.GetBuffer(), CV_LOAD_IMAGE_UNCHANGED);

		Mat img_undistort;

		undistortImage_Brown(img, K, dist, img_undistort);

//		fileName.Format("E:\\paper\\PhD Thesis\\data\\20151206 橘子洲头毛泽东像\\mao\\800×534\\undistorted\\A%02d.JPG", i);
		fileName.Format("E:\\paper\\PhD Thesis\\data\\20151206 橘子洲头毛泽东像\\horse\\800×534\\undistorted\\A%02d.JPG", i);

		imwrite(fileName.GetBuffer(), img_undistort);
	}

	return TRUE;
}

void CDeepVoidApp::OnSfmSimu()
{
	// TODO: Add your command handler code here
	AfxBeginThread(SfM_Simu, this, THREAD_PRIORITY_NORMAL);
}

UINT Featuretrackingexp(LPVOID param)
{
	CDeepVoidApp * pApp = (CDeepVoidApp * )param;

	/*int i,j,k,ii,jj;

	CString strInfo;

	int nCam = pApp->m_vCams.size();

	// zhaokunz, 20140404, store every image into memory
	vector<Mat> vImages;

	vector<CString> vImgPaths;

	int imgWidth, imgHeight;

	// first, extract all the features in each image
	for (i=0;i<nCam;i++)
	{
		// read corresponding image data into memory first
		char * pDir = (char *)pApp->m_pMainFrame->m_wndImgThumbnailPane.m_wndImgListCtrl.GetItemData(i);

		CString strDir;
		strDir.Format(_T("%s"), pDir);
		strDir.Trim();

		Mat img = imread(strDir.GetBuffer(), CV_LOAD_IMAGE_UNCHANGED);

		vImgPaths.push_back(strDir);

		vImages.push_back(img); // store every image

		imgWidth = img.cols;
		imgHeight = img.rows;

		Mat mask;

//		SIFT sift;
		SURF sift;

		sift(img, mask, pApp->m_vCams[i].m_feats.key_points, pApp->m_vCams[i].m_feats.descriptors);
		pApp->m_vCams[i].m_feats.type = Feature_SIFT;

		KeyPoint kpt_pre;
		kpt_pre.pt.x = -1000;	kpt_pre.pt.y = -1000;
		kpt_pre.size = -1000;
		int idx_imgPt;
		for (j=0;j<pApp->m_vCams[i].m_feats.key_points.size();j++)
		{
			pApp->m_vCams[i].m_feats.tracks.push_back(-1);

			KeyPoint kpt_cur = pApp->m_vCams[i].m_feats.key_points[j];
			if (fabs(kpt_cur.pt.x-kpt_pre.pt.x)<1.0E-12 && fabs(kpt_cur.pt.y-kpt_pre.pt.y)<1.0E-12
				&& fabs(kpt_cur.size - kpt_pre.size)<1.0E-12)
			{
				// indicating that current keypoint is identical to the previous keypoint
				pApp->m_vCams[i].m_feats.idx_pt.push_back(idx_imgPt);
			} 
			else
			{
				pApp->m_vCams[i].m_feats.idx_pt.push_back(j);
				idx_imgPt = j;
			}

			kpt_pre.pt.x = kpt_cur.pt.x;
			kpt_pre.pt.y = kpt_cur.pt.y;
			kpt_pre.size = kpt_cur.size;

			// 20150215, zhaokunz, 把该特征点的颜色信息插值出来
			uchar r,g,b;
			Vec3b rgb;
			if (BilinearInterp(img,kpt_cur.pt.x,kpt_cur.pt.y,r,g,b))
			{
				rgb[0]=b;
				rgb[1]=g;
				rgb[2]=r;	
			}
			pApp->m_vCams[i].m_feats.rgbs.push_back(rgb);
		}

		strInfo.Format("Image %03d extracted %07d features", i, pApp->m_vCams[i].m_feats.key_points.size());
		pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
	}

	for (i=0;i<pApp->m_vCams.size();i++)
	{
		pApp->m_vCams[i].fx = 707.244864;
		pApp->m_vCams[i].fy = 707.135373;
		pApp->m_vCams[i].s  = 0;
		pApp->m_vCams[i].cx = 200.696825;
		pApp->m_vCams[i].cy = 154.045817;

		pApp->m_vCams[i].m_K(0,0) = 707.244864;
		pApp->m_vCams[i].m_K(1,1) = 707.135373;
		pApp->m_vCams[i].m_K(0,1) = 0;
		pApp->m_vCams[i].m_K(0,2) = 200.696825;
		pApp->m_vCams[i].m_K(1,2) = 154.045817;
		pApp->m_vCams[i].m_K(2,2) = 1;
		pApp->m_vCams[i].m_bCalibed = true;

		memset(pApp->m_vCams[i].k, 0, 5*sizeof(double));
		pApp->m_vCams[i].dist_type = 1;
	}

	// remove distortions and set the distortion coefficients to be all zeros
	for (i=0;i<nCam;i++)
	{
		RemoveDistortion_DCBrown(pApp->m_vCams[i]);
		memset(pApp->m_vCams[i].k, 0, 5*sizeof(double));
	}

	double thresh_match = 5;
	double thresh_d2epiline = 3;
	double thresh_matchConf = 0.99;
	int thresh_n_matches = 20;

	double thresh_stdev_YZ = 0.2;
	double thresh_reproj_ro = 1.0;
	double thresh_pyerr = 0.01;
	double thresh_ang = 15;

	int nSptImgs_desired = 2; // each image will be associated with at most this number of support images

	if (nSptImgs_desired>(pApp->m_vCams.size()-1))
	{
		nSptImgs_desired = pApp->m_vCams.size()-1; // at most n-1 support images
	}

	double opts[5];
//	opts[0] = 1.0E-3;	// levmar 优化方法中要用到的参数 u 的初始尺度因子，默认为 1.0E-3
	opts[0] = 1.0E-6;	// levmar 优化方法中要用到的参数 u 的初始尺度因子，默认为 1.0E-3
	opts[1] = 1.0E-8;	// 当目标函数对各待优化参数的最大导数小于等于该值时优化结束，默认为 1.0E-12
	opts[2] = 1.0E-8;	// 当待优化参数 2 范数的变化量小于该阈值时优化结束，默认为 1.0E-12
	opts[3] = 1.0E-12;	// 当误差矢量的 2 范数小于该阈值时优化结束，默认为 1.0E-12
	opts[4] = 0;		// 当误差矢量的 2 范数的相对变化量小于该阈值时优化结束，默认为 0

	vector<vector<DMatch>> allMatches;
	vector<DMatch> matches;

	SfM_ZZK::PairWiseMatches map_pairwise_matches; // 20150920, <(i,j), <DMatch>>

	FILE * fileMatches = fopen("D:\\matches number.txt", "w");

	for (i=0;i<nCam;i++)
	{
		for (j=0;j<nCam;j++)
		{
			if (j==i)
			{
				vector<DMatch> nullMatch;
				allMatches.push_back(nullMatch);
				fprintf(fileMatches, "%d	", nullMatch.size());
				continue;
			}
			if (j<i)
			{
				vector<DMatch> match_ji = allMatches[j*nCam+i];
				for (k=0;k<match_ji.size();k++)
				{
					int tmp = match_ji[k].queryIdx;
					match_ji[k].queryIdx = match_ji[k].trainIdx;
					match_ji[k].trainIdx = tmp;
				}
				allMatches.push_back(match_ji);
				fprintf(fileMatches, "%d	", match_ji.size());
				continue;
			}

			// match all image pairs
			// 20150113, zhaokunz, new matching function
			Matx33d mF;
			bool bSuc = Get_F_Matches_knn(pApp->m_vCams[i].m_feats,pApp->m_vCams[j].m_feats,mF,matches,false,0.65,0.5,thresh_d2epiline,thresh_matchConf,64);
			// 20151017，draw matches from each stage from inside the function
			Mat disp_matches;
			drawMatches(vImages[i], pApp->m_vCams[i].m_feats.key_points, vImages[j], pApp->m_vCams[j].m_feats.key_points, matches,
				disp_matches, Scalar(0,255,0), Scalar(0,0,255));
			strInfo.Format("E:\\matches\\matches between image %03d and %03d.bmp", i, j);
//			imwrite(strInfo.GetBuffer(), disp_matches);

			strInfo.Format("matching between image %03d and %03d finished: %04d matches are found", i, j, matches.size());
			pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

//			if (matches.size() < thresh_n_matches)
			if (!bSuc)		
			{
				vector<DMatch> nullMatch;
				allMatches.push_back(nullMatch);
				fprintf(fileMatches, "%d	", nullMatch.size());
			} 
			else
			{
				allMatches.push_back(matches);
				fprintf(fileMatches, "%d	", matches.size());

				map_pairwise_matches.insert(make_pair(make_pair(i,j), matches));
			}
		}
		fprintf(fileMatches, "\n");
	}
	fclose(fileMatches);


	// 对比整条剔除策略和匹配个数加权策略的代码段 /////////////////////////////////////////////////
	SfM_ZZK::MultiTracks_old map_tracks2;
	SfM_ZZK::FindAllTracks_Least(map_pairwise_matches, map_tracks2);
	std::map<int,int> hist3;
	double avg_trackLeng1 = SfM_ZZK::BuildTrackLengthHistogram(map_tracks2, hist3);

	CString str111;
	str111.Format("E:\\paper\\PhD Thesis\\实验结果复现\\特征跟踪算法对比实验结果复现\\%d images - hist - old.txt", vImages.size());
	FILE * file111 = fopen(str111, "w");
	int n_total=0;
	for (auto iter111=hist3.begin(); iter111!=hist3.end(); ++iter111)
	{
		fprintf(file111, "%d	%d\n", iter111->first, iter111->second);
		n_total+=iter111->second;
	}
	fprintf(file111, "%d	%lf", n_total, avg_trackLeng1);
	fclose(file111);

	SfM_ZZK::MultiTracks_old map_tracks;
	SfM_ZZK::FindAllTracks_Olsson(map_pairwise_matches, map_tracks);
	std::map<int,int> hist;
	double avg_trackLeng2 = SfM_ZZK::BuildTrackLengthHistogram(map_tracks, hist);

	str111.Format("E:\\paper\\PhD Thesis\\实验结果复现\\特征跟踪算法对比实验结果复现\\%d images - hist - new.txt", vImages.size());
	file111 = fopen(str111, "w");
	n_total=0;
	int sum_length=0;
	for (auto iter111=hist.begin(); iter111!=hist.end(); ++iter111)
	{
		fprintf(file111, "%d	%d\n", iter111->first, iter111->second);

		if (iter111->first>1)
		{
			n_total+=iter111->second;
			sum_length+=iter111->first*iter111->second;
		}
	}
	fprintf(file111, "%d	%lf", n_total, sum_length/(double)n_total);
	fclose(file111);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 对比
// 	int nIter = 100;
// 
// 	pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo("my own version starts");
// 	tic();
// 	for (int iiiii=0;iiiii<nIter;++iiiii)
// 	{
// 		SfM_ZZK::MultiTracks_old map_tracks;
// 		SfM_ZZK::FindAllTracks_Olsson(map_pairwise_matches, map_tracks);
// 	}
// 	double t2 = toc()*1000/nIter;
// 	pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo("my own version ends");
// 
// 	pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo("original version starts");
// 	tic();
// 	for (int iiiii=0;iiiii<nIter;++iiiii)
// 	{
// 		SfM_ZZK::MultiTracks_old map_tracks1;
// 		SfM_ZZK::FindAllTracks_Olsson_Original(map_pairwise_matches, map_tracks1);
// 	}
// 	double t1 = toc()*1000/nIter;
// 	pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo("original version ends");
// 	
// 	FILE * file111 = fopen("E:\\paper\\PhD Thesis\\实验结果复现\\特征跟踪算法对比实验结果复现\\time record - original.txt", "a");
// 	fprintf(file111, "%d	%lf\n", vImages.size(), t1);
// 	fclose(file111);
// 
// 	file111 = fopen("E:\\paper\\PhD Thesis\\实验结果复现\\特征跟踪算法对比实验结果复现\\time record - my own.txt", "a");
// 	fprintf(file111, "%d	%lf\n", vImages.size(), t2);
// 	fclose(file111);*/

	return TRUE;
}

void CDeepVoidApp::OnFeaturetrackingexp()
{
	// TODO: Add your command handler code here
	AfxBeginThread(Featuretrackingexp, this, THREAD_PRIORITY_NORMAL);
}

UINT SfMEXP(LPVOID param)
{
	CDeepVoidApp * pApp = (CDeepVoidApp * )param;

	/*int i,j,k,ii,jj;

	CString strInfo;

	int nCam = pApp->m_vCams.size();

	// zhaokunz, 20140404, store every image into memory
	vector<Mat> vImages;

	vector<CString> vImgPaths;

	int imgWidth, imgHeight;

	// first, extract all the features in each image
	for (i=0;i<nCam;i++)
	{
		// read corresponding image data into memory first
		char * pDir = (char *)pApp->m_pMainFrame->m_wndImgThumbnailPane.m_wndImgListCtrl.GetItemData(i);

		CString strDir;
		strDir.Format(_T("%s"), pDir);
		strDir.Trim();

		Mat img = imread(strDir.GetBuffer(), CV_LOAD_IMAGE_UNCHANGED);

		vImgPaths.push_back(strDir);

		vImages.push_back(img); // store every image

		imgWidth = img.cols;
		imgHeight = img.rows;

		Mat mask;

//		SIFT sift;
		SURF sift;

		sift(img, mask, pApp->m_vCams[i].m_feats.key_points, pApp->m_vCams[i].m_feats.descriptors);
		pApp->m_vCams[i].m_feats.type = Feature_SIFT;

		KeyPoint kpt_pre;
		kpt_pre.pt.x = -1000;	kpt_pre.pt.y = -1000;
		kpt_pre.size = -1000;
		int idx_imgPt;
		for (j=0;j<pApp->m_vCams[i].m_feats.key_points.size();j++)
		{
			pApp->m_vCams[i].m_feats.tracks.push_back(-1);

			KeyPoint kpt_cur = pApp->m_vCams[i].m_feats.key_points[j];
			if (fabs(kpt_cur.pt.x-kpt_pre.pt.x)<1.0E-12 && fabs(kpt_cur.pt.y-kpt_pre.pt.y)<1.0E-12
				&& fabs(kpt_cur.size - kpt_pre.size)<1.0E-12)
			{
				// indicating that current keypoint is identical to the previous keypoint
				pApp->m_vCams[i].m_feats.idx_pt.push_back(idx_imgPt);
			} 
			else
			{
				pApp->m_vCams[i].m_feats.idx_pt.push_back(j);
				idx_imgPt = j;
			}

			kpt_pre.pt.x = kpt_cur.pt.x;
			kpt_pre.pt.y = kpt_cur.pt.y;
			kpt_pre.size = kpt_cur.size;

			// 20150215, zhaokunz, 把该特征点的颜色信息插值出来
			uchar r,g,b;
			Vec3b rgb;
			if (BilinearInterp(img,kpt_cur.pt.x,kpt_cur.pt.y,r,g,b))
			{
				rgb[0]=b;
				rgb[1]=g;
				rgb[2]=r;	
			}
			pApp->m_vCams[i].m_feats.rgbs.push_back(rgb);
		}

		strInfo.Format("Image %03d extracted %07d features", i, pApp->m_vCams[i].m_feats.key_points.size());
		pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
	}

	for (i=0;i<pApp->m_vCams.size();i++)
	{
		pApp->m_vCams[i].fx = 1414.489728;
		pApp->m_vCams[i].fy = 1414.270745;
		pApp->m_vCams[i].s  = 0;
		pApp->m_vCams[i].cx = 401.3936505;
		pApp->m_vCams[i].cy = 308.091633;

		pApp->m_vCams[i].m_K(0,0) = 1414.489728;
		pApp->m_vCams[i].m_K(1,1) = 1414.270745;
		pApp->m_vCams[i].m_K(0,1) = 0;
		pApp->m_vCams[i].m_K(0,2) = 401.3936505;
		pApp->m_vCams[i].m_K(1,2) = 308.091633;
		pApp->m_vCams[i].m_K(2,2) = 1;
		pApp->m_vCams[i].m_bCalibed = true;
		
		memset(pApp->m_vCams[i].k, 0, 5*sizeof(double));
		pApp->m_vCams[i].dist_type = 1;
	}

	// remove distortions and set the distortion coefficients to be all zeros
	for (i=0;i<nCam;i++)
	{
		RemoveDistortion_DCBrown(pApp->m_vCams[i]);
		memset(pApp->m_vCams[i].k, 0, 5*sizeof(double));
	}

	double thresh_match = 5;
	double thresh_d2epiline = 3;
	double thresh_matchConf = 0.99;
	int thresh_n_matches = 20;

	double thresh_stdev_YZ = 0.2;
	double thresh_reproj_ro = 1.0;
	double thresh_pyerr = 0.01;
	double thresh_ang = 15;

	int nSptImgs_desired = 2; // each image will be associated with at most this number of support images

	if (nSptImgs_desired>(pApp->m_vCams.size()-1))
	{
		nSptImgs_desired = pApp->m_vCams.size()-1; // at most n-1 support images
	}

	double opts[5];
//	opts[0] = 1.0E-3;	// levmar 优化方法中要用到的参数 u 的初始尺度因子，默认为 1.0E-3
	opts[0] = 1.0E-6;	// levmar 优化方法中要用到的参数 u 的初始尺度因子，默认为 1.0E-3
	opts[1] = 1.0E-8;	// 当目标函数对各待优化参数的最大导数小于等于该值时优化结束，默认为 1.0E-12
	opts[2] = 1.0E-8;	// 当待优化参数 2 范数的变化量小于该阈值时优化结束，默认为 1.0E-12
	opts[3] = 1.0E-12;	// 当误差矢量的 2 范数小于该阈值时优化结束，默认为 1.0E-12
	opts[4] = 0;		// 当误差矢量的 2 范数的相对变化量小于该阈值时优化结束，默认为 0

	vector<vector<DMatch>> allMatches;
	vector<DMatch> matches;

	SfM_ZZK::PairWiseMatches map_pairwise_matches; // 20150920, <(i,j), <DMatch>>

	FILE * fileMatches = fopen("D:\\matches number.txt", "w");

	for (i=0;i<nCam;i++)
	{
		for (j=0;j<nCam;j++)
		{
			if (j==i)
			{
				vector<DMatch> nullMatch;
				allMatches.push_back(nullMatch);
				fprintf(fileMatches, "%d	", nullMatch.size());
				continue;
			}
			if (j<i)
			{
				vector<DMatch> match_ji = allMatches[j*nCam+i];
				for (k=0;k<match_ji.size();k++)
				{
					int tmp = match_ji[k].queryIdx;
					match_ji[k].queryIdx = match_ji[k].trainIdx;
					match_ji[k].trainIdx = tmp;
				}
				allMatches.push_back(match_ji);
				fprintf(fileMatches, "%d	", match_ji.size());
				continue;
			}

			// match all image pairs
			// 20150113, zhaokunz, new matching function
			Matx33d mF;
			bool bSuc = Get_F_Matches_knn(pApp->m_vCams[i].m_feats,pApp->m_vCams[j].m_feats,mF,matches,false,0.65,0.5,thresh_d2epiline,thresh_matchConf,64);

			// 20151017，draw matches from each stage from inside the function
			Mat disp_matches;
			drawMatches(vImages[i], pApp->m_vCams[i].m_feats.key_points, vImages[j], pApp->m_vCams[j].m_feats.key_points, matches,
				disp_matches, Scalar(0,255,0), Scalar(0,0,255));
			strInfo.Format("E:\\matches\\matches between image %03d and %03d.bmp", i, j);
//			imwrite(strInfo.GetBuffer(), disp_matches);

			strInfo.Format("matching between image %03d and %03d finished: %04d matches are found", i, j, matches.size());
			pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

//			if (matches.size() < thresh_n_matches)
			if (!bSuc)		
			{
				vector<DMatch> nullMatch;
				allMatches.push_back(nullMatch);
				fprintf(fileMatches, "%d	", nullMatch.size());
			} 
			else
			{
				allMatches.push_back(matches);
				fprintf(fileMatches, "%d	", matches.size());

				map_pairwise_matches.insert(make_pair(make_pair(i,j), matches));
			}
		}
		fprintf(fileMatches, "\n");
	}
	fclose(fileMatches);


	// feature tracking
	SfM_ZZK::MultiTracks map_tracks_init;
	SfM_ZZK::FindAllTracks_Olsson(map_pairwise_matches, map_tracks_init);
//	SfM_ZZK::FindAllTracks_Least(map_pairwise_matches, map_tracks);

	// 确保特征轨迹从0开始依次计数
	// 并建立特征轨迹中包含的特征点至该特征轨迹的映射
	SfM_ZZK::MultiTracks map_tracks;
	int idx_count = 0;
	for (auto iter_track=map_tracks_init.begin();iter_track!=map_tracks_init.end();++iter_track)
	{
		map_tracks.insert(make_pair(idx_count,iter_track->second));

		// 建立该特征轨迹中包含的特征点至该特征轨迹的映射，通过 trackID 来索引
		for (auto iter_Ii=iter_track->second.begin();iter_Ii!=iter_track->second.end();++iter_Ii)
		{
			const int & I = iter_Ii->first; // image I
//			const int & i = iter_Ii->second; // feature i
			const int & i = iter_Ii->second.first; // feature i
			pApp->m_vCams[I].m_feats.tracks[i] = idx_count;
		}

		++idx_count;
	}

	// 统计特征轨迹直方图
	std::map<int,int> hist_track;
	SfM_ZZK::BuildTrackLengthHistogram(map_tracks, hist_track);
	int n_tracklength_more_than_1 = 0;
	for (auto iter_n=hist_track.begin();iter_n!=hist_track.end();++iter_n)
	{
		if (iter_n->first<2)
		{
			continue;
		}
		n_tracklength_more_than_1+=iter_n->second;
	}

	strInfo.Format("number of good tracks: %07d", n_tracklength_more_than_1);
	pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

	// relative orientation //////////////////////////////////////////////////////////////////////////
	CMatrix mRT; vector<CloudPoint> clouds;

	// 20151105 point cloud based on std::map
	SfM_ZZK::PointCloud map_pointcloud;

	vector<int> status(nCam);
	for (i=0;i<nCam;i++)
	{
		status[i] = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	vector<SfM_ZZK::pair_ij_k> pairs;
	SfM_ZZK::RankImagePairs_TrackLengthSum(map_pairwise_matches, map_tracks, pairs);

// 	vector<Point2i> pairs;
// 	FindPairObservingMostCommonTracks(pApp->m_vCams,allTracks,pairs);

	CString strFile;
	int idx_refimg;

//	for (k=0;k<pairs.size();k++)
	for (auto iter_pair=pairs.begin();iter_pair!=pairs.end();++iter_pair)
	{
// 		i = pairs[k].x;
// 		j = pairs[k].y;

		i = iter_pair->first.first;
		j = iter_pair->first.second;

		cam_data & cam_i = pApp->m_vCams[i];
		cam_data & cam_j = pApp->m_vCams[j];

		// 找到两者之间的特征匹配
		auto iter_found = map_pairwise_matches.find(iter_pair->first);

		// 尝试做相对定向
// 		bool bSuc_RO = RelativeOrientation_Features_PIRO_givenMatches(cam_i, cam_j, i, j, iter_found->second,
// 			mRT, clouds, thresh_reproj_ro);

		Matx33d mR;
		Matx31d mt;
		SfM_ZZK::PointCloud map_pointcloud_tmp;

		bool bSuc_RO = RelativeOrientation_Features_PIRO_givenMatches(cam_i, cam_j, i, j, iter_found->second,
			mR, mt, map_pointcloud_tmp, thresh_reproj_ro);

		if (bSuc_RO)
		{
			cam_i.m_R = Matx33d();
			cam_i.m_t = Matx31d();
			cam_i.m_R(0,0)=cam_i.m_R(1,1)=cam_i.m_R(2,2)=1;

			cam_j.m_R = mR;
			cam_j.m_t = mt;

			map_pointcloud = map_pointcloud_tmp; // 正式录用所有生成的物点

//			strInfo.Format("Relative orientation between image %03d and %03d finished, the rank is: %d, number of cloud points are %d", i, j, k, clouds.size());
			strInfo.Format("Relative orientation between image %03d and %03d finished, number of cloud points are %d", i, j, map_pointcloud.size());
			pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

			status[i] = 1;
			status[j] = 1;

			cam_i.m_bOriented = true;
			cam_j.m_bOriented = true;

			idx_refimg = i;

			// 将所有重建出来的像点暂时赋上都为内点
			for (auto iter_wrdpt = map_pointcloud.begin();iter_wrdpt!=map_pointcloud.end();++iter_wrdpt)
			{
				const int & trackID = iter_wrdpt->first;

				auto iter_found_track = map_tracks.find(trackID);

				auto iter_found_i = iter_found_track->second.find(i);
				auto iter_found_j = iter_found_track->second.find(j);

				iter_found_i->second.second = 1; // 为内点
				iter_found_j->second.second = 1; // 为内点
			}

			strFile.Format("point cloud after RO of images %03d and %03d.txt", i, j);

			break;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	CString strOut;
	strOut.Format("E:\\all\\");

//	OutputPointCloud(strOut+strFile,pApp->m_vCams,clouds);

	vector<CloudPoint> cloud_old;

	SfM_ZZK::OutputPointCloud(strOut+strFile,map_pointcloud,pApp->m_vCams,map_tracks,cloud_old,2);

	double d_mean = MeanMinDistance_3D(clouds);
	strInfo.Format("average distance between cloud points is: %lf", d_mean);
	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

	vector<SfM_ZZK::pair_ij> imgRank;
//	RankImages_NumObjPts(pApp->m_vCams, clouds, status, imgRank);
	RankImages_NumObjPts(pApp->m_vCams, map_pointcloud, imgRank);

	// 20151103，全自动的增量式 SfM 循环 ////////////////////////////////////
	double thresh_inlier_rpjerr = 1.5;
	double thresh_inlier_ratio = 0.15;

	int n_pointcloud_size_old = map_pointcloud.size();

	bool bAllFail = false;
	while (imgRank.size()>0 && !bAllFail) // 还有图没有完成定向且并不是所有剩余图像都定向失败了就继续
	{
		bAllFail = true;
		vector<SfM_ZZK::pair_ij> imgRank_tmp = imgRank;

		for (auto iter_img=imgRank_tmp.begin();iter_img!=imgRank_tmp.end();++iter_img)
		{
			const int & I = iter_img->first; // image I

			cam_data & cam_I = pApp->m_vCams[I];

			// 1. 尝试 RANSAC 后方交会
			Matx33d mR;
			Matx31d mt;
			bool bEOSuc;
			try
			{
				bEOSuc = EO_PnP_RANSAC(cam_I, map_pointcloud, mR, mt, thresh_inlier_rpjerr, thresh_inlier_ratio);
			}
			catch (cv::Exception & e)
			{
				CString str;
				str = e.msg.c_str();
				str+="	error happened in RANSAC EO";
				AfxMessageBox(str);
			}
			
			if (!bEOSuc) // 如果后方交会失败就考虑排在后面的图像
			{
				continue;
			}

			// 到这里就说明后方交会成功了
			bAllFail = false;
			
			// 正式录入外参初值
			cam_I.m_R = mR;
			cam_I.m_t = mt;

			status[I] = 1;
			cam_I.m_bOriented = true;

			// 2. 稀疏光束法平差。只利用现有的物点，还没有利用新加入的图像前方交会新的物点
			theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(_T("SBA starts"));
			double info[10];
			try
			{
				int nnn = optim_sba_levmar_XYZ_ext_rotvec(map_pointcloud, pApp->m_vCams, map_tracks, idx_refimg, thresh_inlier_rpjerr, 1024, opts, info);
			}
			catch (cv::Exception & e)
			{
				CString str;
				str = e.msg.c_str();
				str+="	error happened in SBA";
				AfxMessageBox(str);
			}
			strInfo.Format("SBA ends, point cloud size: %d, initial err: %lf, final err: %lf, iter: %04.0f, code: %01.0f",
				map_pointcloud.size(), info[0], info[1], info[3], info[4]);
			theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

			strFile.Format("point cloud after successful EO of image %03d and bundle adjustment.txt", I);
			try
			{
				SfM_ZZK::OutputPointCloud(strOut+strFile,map_pointcloud,pApp->m_vCams,map_tracks,cloud_old,3);	
			}
			catch (cv::Exception & e)
			{
				CString str;
				str = e.msg.c_str();
				str+="	error happened in OutputPointCloud";
				AfxMessageBox(str);
			}

			// 更新阈值
//			thresh_inlier_rpjerr = 3*info[1];

			// 3. 前方交会
			try
			{
//				SfM_ZZK::Triangulation_AddOneImg(map_pointcloud,pApp->m_vCams,map_tracks,I,thresh_inlier_rpjerr);
				SfM_ZZK::Triangulation_AllImgs(map_pointcloud,pApp->m_vCams,map_tracks,thresh_inlier_rpjerr);
			}
			catch (cv::Exception & e)
			{
				CString str;
				str = e.msg.c_str();
				str+="	error happened in Triangulation";
				AfxMessageBox(str);
			}
			

			// 4. 如果点云拓展了，就对剩下的图像重新进行排序
			int n_pointcloud_size_new = map_pointcloud.size();
			if (n_pointcloud_size_new != n_pointcloud_size_old)
			{
				try
				{
					RankImages_NumObjPts(pApp->m_vCams, map_pointcloud, imgRank);
				}
				catch (cv::Exception & e)
				{
					CString str;
					str = e.msg.c_str();
					str+="	error happened in RankImages";
					AfxMessageBox(str);
				}

				n_pointcloud_size_old = n_pointcloud_size_new;
				break;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////

	// 最后总的来一次稀疏光束法平差
	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(_T("Final SBA starts"));
	double info[10];
	int nnn = optim_sba_levmar_XYZ_ext_rotvec(map_pointcloud, pApp->m_vCams, map_tracks, idx_refimg, thresh_inlier_rpjerr, 1024,opts, info);
	strInfo.Format("Final SBA ends, point cloud size: %d, initial err: %lf, final err: %lf, iter: %04.0f, code: %01.0f",
		map_pointcloud.size(), info[0], info[1], info[3], info[4]);
	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

	std::map<int,int> hist_cloudpoint_inlier;
	double length_average = SfM_ZZK::BuildCloudPointInlierHistogram(map_pointcloud,map_tracks,hist_cloudpoint_inlier);

	FILE * file = fopen("E:\\all\\hist.txt", "w");
	for (auto iter_hist=hist_cloudpoint_inlier.begin();iter_hist!=hist_cloudpoint_inlier.end();++iter_hist)
	{
		fprintf(file,"%d	%d\n", iter_hist->first, iter_hist->second);
	}
	fprintf(file,"%lf", length_average);
	fclose(file);

	// 输出点云
	strFile.Format("Final point cloud.txt");
	SfM_ZZK::OutputPointCloud(strOut+strFile,map_pointcloud,pApp->m_vCams,map_tracks,cloud_old,3);

	// 输出像机定向
	// save all cameras' parameters
	for (i=0;i<nCam;i++)
	{
		CString strtmp;
		strFile.Format("cam%02d.txt", i);
		SaveCameraData(strOut+strFile, pApp->m_vCams[i]);
	}*/
		
	return TRUE;
}

void CDeepVoidApp::OnSfmexp()
{
	// TODO: Add your command handler code here
	AfxBeginThread(SfMEXP, this, THREAD_PRIORITY_NORMAL);
}


void CDeepVoidApp::OnDsbasimuNoise()
{
	// TODO: Add your command handler code here
	AfxBeginThread(DSBA_Simu_increaseImgPtNoise, this, THREAD_PRIORITY_NORMAL);
}


void CDeepVoidApp::OnDsbasimuObjpts()
{
	// TODO: Add your command handler code here
	AfxBeginThread(DSBA_Simu_increaseObjpts, this, THREAD_PRIORITY_NORMAL);
}


void CDeepVoidApp::OnDsbasimuImgnum()
{
	// TODO: Add your command handler code here
	AfxBeginThread(DSBA_Simu_increaseImgs, this, THREAD_PRIORITY_NORMAL);
}


void CDeepVoidApp::OnDsbasimuObsnum()
{
	// TODO: Add your command handler code here
	AfxBeginThread(DSBA_Simu_increaseObservations, this, THREAD_PRIORITY_NORMAL);
}

viz::Viz3d wrd3d("world window");

UINT NetworkOrientation(LPVOID param)
{
	CDeepVoidApp * pApp = (CDeepVoidApp * )param;

	CString strInfo;

	int nCam = pApp->m_vCams.size();

	// zhaokunz, 20140404, store every image into memory
	vector<Mat> vImages;

//	vector<CString> vImgPaths;

	pApp->m_vImgPaths.clear(); // 20161029

	int imgWidth, imgHeight;

	// first, extract all the features in each image
	for (int i=0;i<nCam;i++)
	{
		// read corresponding image data into memory first
		char * pDir = (char *)pApp->m_pMainFrame->m_wndImgThumbnailPane.m_wndImgListCtrl.GetItemData(i);

		CString strDir;
		strDir.Format(_T("%s"), pDir);
		strDir.Trim();

		Mat img = imread(strDir.GetBuffer(), CV_LOAD_IMAGE_UNCHANGED);

//		vImgPaths.push_back(strDir);
		pApp->m_vImgPaths.push_back(strDir);

		vImages.push_back(img); // store every image

		imgWidth = img.cols;
		imgHeight = img.rows;

		// reference: http://stackoverflow.com/questions/27533203/how-do-i-use-sift-in-opencv-3-0-with-c
		// create a feature class
		cv::Ptr<Feature2D> f2d = cv::xfeatures2d::SIFT::create(0, 3, 0.01);
//		cv::Ptr<Feature2D> f2d = cv::xfeatures2d::SURF::create();
//		cv::Ptr<Feature2D> f2d = cv::ORB::create();

		// detect features and compute descriptors
		Mat mask;
		f2d->detectAndCompute(img, mask, pApp->m_vCams[i].m_feats.key_points, pApp->m_vCams[i].m_feats.descriptors);

//		SIFT sift;
//		SURF sift;
//
//		/*const double contrast = sift.get("contrastThreshold");*/
//		/*std::string str_contrastThreshold = sift.paramHelp("contrastThreshold");*/
//		sift.set("contrastThreshold",0.01);
//
//		sift(img, mask, pApp->m_vCams[i].m_feats.key_points, pApp->m_vCams[i].m_feats.descriptors);

		pApp->m_vCams[i].m_feats.type = Feature_SIFT_SIFT;

		KeyPoint kpt_pre;
		kpt_pre.pt.x = -1000;	kpt_pre.pt.y = -1000;
		kpt_pre.size = -1000;
		int idx_imgPt;
		for (int j=0;j<pApp->m_vCams[i].m_feats.key_points.size();j++)
		{
			pApp->m_vCams[i].m_feats.tracks.push_back(-1);

			KeyPoint kpt_cur = pApp->m_vCams[i].m_feats.key_points[j];
			if (fabs(kpt_cur.pt.x-kpt_pre.pt.x)<1.0E-12 && fabs(kpt_cur.pt.y-kpt_pre.pt.y)<1.0E-12
				&& fabs(kpt_cur.size - kpt_pre.size)<1.0E-12)
			{
				// indicating that current keypoint is identical to the previous keypoint
				pApp->m_vCams[i].m_feats.idx_pt.push_back(idx_imgPt);
			} 
			else
			{
				pApp->m_vCams[i].m_feats.idx_pt.push_back(j);
				idx_imgPt = j;
			}

			kpt_pre.pt.x = kpt_cur.pt.x;
			kpt_pre.pt.y = kpt_cur.pt.y;
			kpt_pre.size = kpt_cur.size;

			// 20150215, zhaokunz, 把该特征点的颜色信息插值出来
			uchar r,g,b;
			Vec3b rgb;
			if (BilinearInterp(img,kpt_cur.pt.x,kpt_cur.pt.y,r,g,b))
			{
				rgb[0]=b;
				rgb[1]=g;
				rgb[2]=r;	
			}
			pApp->m_vCams[i].m_feats.rgbs.push_back(rgb);
		}

		strInfo.Format("Image %03d extracted %07d features", i, pApp->m_vCams[i].m_feats.key_points.size());
		pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
	}

	for (int i=0;i<pApp->m_vCams.size();i++)
	{
		// 20161029
		pApp->m_vCams[i].fx = pApp->m_vCams[i].m_K(0, 0) = pApp->m_fx;
		pApp->m_vCams[i].fy = pApp->m_vCams[i].m_K(1, 1) = pApp->m_fy;
		pApp->m_vCams[i].s  = pApp->m_vCams[i].m_K(0, 1) = pApp->m_s;
		pApp->m_vCams[i].cx = pApp->m_vCams[i].m_K(0, 2) = pApp->m_cx;
		pApp->m_vCams[i].cy = pApp->m_vCams[i].m_K(1, 2) = pApp->m_cy;
		pApp->m_vCams[i].m_K(2, 2) = 1;
		pApp->m_vCams[i].m_bCalibed = true;		

		// 20161029
		pApp->m_vCams[i].k[0] = pApp->m_k1;
		pApp->m_vCams[i].k[1] = pApp->m_k2;
		pApp->m_vCams[i].k[2] = pApp->m_k3;
		pApp->m_vCams[i].k[3] = pApp->m_k4;
		pApp->m_vCams[i].k[4] = pApp->m_k5;
		pApp->m_vCams[i].dist_type = pApp->m_distType;
	}

	// remove distortions and set the distortion coefficients to be all zeros
	for (int i=0;i<nCam;i++)
	{
		RemoveDistortion_DCBrown(pApp->m_vCams[i]);
		memset(pApp->m_vCams[i].k, 0, 5*sizeof(double));
	}

	double thresh_match = 5;
	double thresh_d2epiline = 3;
	double thresh_matchConf = 0.99;
	int thresh_n_matches = 20;

	double thresh_stdev_YZ = 0.2;
	double thresh_reproj_ro = 1.0;
	double thresh_pyerr = 0.01;
	double thresh_ang = 15;

	int nSptImgs_desired = 2; // each image will be associated with at most this number of support images

	if (nSptImgs_desired>(pApp->m_vCams.size()-1))
	{
		nSptImgs_desired = pApp->m_vCams.size()-1; // at most n-1 support images
	}

	double opts[5];
//	opts[0] = 1.0E-3;	// levmar 优化方法中要用到的参数 u 的初始尺度因子，默认为 1.0E-3
	opts[0] = 1.0E-6;	// levmar 优化方法中要用到的参数 u 的初始尺度因子，默认为 1.0E-3
	opts[1] = 1.0E-8;	// 当目标函数对各待优化参数的最大导数小于等于该值时优化结束，默认为 1.0E-12
	opts[2] = 1.0E-8;	// 当待优化参数 2 范数的变化量小于该阈值时优化结束，默认为 1.0E-12
	opts[3] = 1.0E-12;	// 当误差矢量的 2 范数小于该阈值时优化结束，默认为 1.0E-12
	opts[4] = 0;		// 当误差矢量的 2 范数的相对变化量小于该阈值时优化结束，默认为 0

	SfM_ZZK::PairWiseMatches map_pairwise_matches; // 20150920, <(i,j), <DMatch>>

	// 20151217，为了并行，先把所有可能的图像对列出来，后续直接通过循环而非嵌套循环的方式并行
	vector<std::pair<int,int>> vec_pairs;
	for (int i=0;i<nCam;i++)
	{
		for (int j=i+1;j<nCam;j++)
		{
			vec_pairs.push_back(make_pair(i,j));
		}
	}

	// 20151216，尝试 OpenMP CPU 并行
	omp_set_dynamic(0); // Disable dynamic adjustment of the number of threads.
	int nProcs = omp_get_num_procs(); // number of processors of this computer
	omp_set_num_threads(nProcs);

	// 20151217，居然发现 cpu 并行后还不如串行的耗时短。。。有点无语，没搞懂为什么。
//	#pragma omp parallel for
	for (int k=0;k<vec_pairs.size();++k)
	{
		const std::pair<int,int> & ij = vec_pairs[k];
		int i = ij.first;
		int j = ij.second;

		vector<DMatch> matches;

		// match all image pairs
		// 20150113, zhaokunz, new matching function
		Matx33d mF;
		bool bSuc = Get_F_Matches_knn(pApp->m_vCams[i].m_feats, pApp->m_vCams[j].m_feats, mF, matches, true, 0.65, 0.5, thresh_d2epiline, thresh_matchConf, 64);

		// draw matches
// 		Mat disp_matches;
// 		drawMatches(vImages[i], pApp->m_vCams[i].m_feats.key_points, vImages[j], pApp->m_vCams[j].m_feats.key_points, matches,
// 			disp_matches, Scalar(0,255,0), Scalar(0,0,255));
// 		strInfo.Format("E:\\matches\\matches between image %03d and %03d.bmp", i, j);
// 		imwrite(strInfo.GetBuffer(), disp_matches);

		strInfo.Format("matching between image %03d and %03d finished: %04d matches are found", i, j, matches.size());
		pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

// 		#pragma omp critical // 同时只能有一个thread执行下面这段代码，为了避免潜在的冲突
// 		{
			if (bSuc)		
			{
				map_pairwise_matches.insert(make_pair(make_pair(i,j), matches));
			}
//		}
	}

	// feature tracking
	SfM_ZZK::MultiTracks map_tracks_init;
	SfM_ZZK::FindAllTracks_Olsson(map_pairwise_matches, map_tracks_init);

	// 确保特征轨迹从0开始依次计数
	// 并建立特征轨迹中包含的特征点至该特征轨迹的映射
	SfM_ZZK::MultiTracks map_tracks;
	int idx_count = 0;
	for (auto iter_track=map_tracks_init.begin();iter_track!=map_tracks_init.end();++iter_track)
	{
		map_tracks.insert(make_pair(idx_count,iter_track->second));

		// 建立该特征轨迹中包含的特征点至该特征轨迹的映射，通过 trackID 来索引
		for (auto iter_Ii=iter_track->second.begin();iter_Ii!=iter_track->second.end();++iter_Ii)
		{
			const int & I = iter_Ii->first; // image I
//			const int & i = iter_Ii->second; // feature i
			const int & i = iter_Ii->second.first; // feature i
			pApp->m_vCams[I].m_feats.tracks[i] = idx_count;
		}

		++idx_count;
	}

	// 统计特征轨迹直方图
	std::map<int,int> hist_track;
	SfM_ZZK::BuildTrackLengthHistogram(map_tracks, hist_track);
	int n_tracklength_more_than_1 = 0;
	for (auto iter_n=hist_track.begin();iter_n!=hist_track.end();++iter_n)
	{
		if (iter_n->first<2)
		{
			continue;
		}
		n_tracklength_more_than_1+=iter_n->second;
	}

	strInfo.Format("number of good tracks: %07d", n_tracklength_more_than_1);
	pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

	// relative orientation //////////////////////////////////////////////////////////////////////////
	// 20151105 point cloud based on std::map
	SfM_ZZK::PointCloud map_pointcloud;

	vector<int> status(nCam);
	for (int i=0;i<nCam;i++)
	{
		status[i] = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	vector<SfM_ZZK::pair_ij_k> pairs;
	SfM_ZZK::RankImagePairs_TrackLengthSum(map_pairwise_matches, map_tracks, pairs);

	CString strFile;
	int idx_refimg;

	for (auto iter_pair=pairs.begin();iter_pair!=pairs.end();++iter_pair)
	{
		int i = iter_pair->first.first;
		int j = iter_pair->first.second;

		cam_data & cam_i = pApp->m_vCams[i];
		cam_data & cam_j = pApp->m_vCams[j];

		// 找到两者之间的特征匹配
		auto iter_found = map_pairwise_matches.find(iter_pair->first);

		Matx33d mR;
		Matx31d mt;
		SfM_ZZK::PointCloud map_pointcloud_tmp;

		bool bSuc_RO = RelativeOrientation_Features_PIRO_givenMatches(cam_i, cam_j, i, j, iter_found->second,
			mR, mt, map_pointcloud_tmp, thresh_reproj_ro, 5.0);

		if (bSuc_RO)
		{
			cam_i.m_R = Matx33d();
			cam_i.m_t = Matx31d();
			cam_i.m_R(0,0)=cam_i.m_R(1,1)=cam_i.m_R(2,2)=1;

			cam_j.m_R = mR;
			cam_j.m_t = mt;

			map_pointcloud = map_pointcloud_tmp; // 正式录用所有生成的物点

			strInfo.Format("Relative orientation between image %03d and %03d succeeded, number of cloud points are %d", i, j, map_pointcloud.size());
			pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

			status[i] = 1;
			status[j] = 1;

			cam_i.m_bOriented = true;
			cam_j.m_bOriented = true;

			idx_refimg = i;

			// 将所有重建出来的像点暂时赋上都为内点
			for (auto iter_wrdpt = map_pointcloud.begin();iter_wrdpt!=map_pointcloud.end();++iter_wrdpt)
			{
				const int & trackID = iter_wrdpt->first;

				auto iter_found_track = map_tracks.find(trackID);

				auto iter_found_i = iter_found_track->second.find(i);
				auto iter_found_j = iter_found_track->second.find(j);

				iter_found_i->second.second = 1; // 为内点
				iter_found_j->second.second = 1; // 为内点
			}

			strFile.Format("point cloud after RO of images %03d and %03d.txt", i, j);

			break;
		}
	}
	//////////////////////////////////////////////////////////////////////////

//	CString strOut;
//	strOut.Format("E:\\results\\");
	pApp->m_strOut.Format("E:\\results\\");

//	vector<CloudPoint> cloud_old;

	SfM_ZZK::OutputPointCloud(pApp->m_strOut+strFile,map_pointcloud,pApp->m_vCams,map_tracks,pApp->m_cloud_old,2);

//	SfM_ZZK::Draw3DScene(pApp->m_wnd3d, pApp->m_ptcloud, map_pointcloud, pApp->m_vCams, map_tracks, 2);

	vector<SfM_ZZK::pair_ij> imgRank;
	RankImages_NumObjPts(pApp->m_vCams, map_pointcloud, imgRank);

	// 20151103，全自动的增量式 SfM 循环 ////////////////////////////////////
	double thresh_inlier_rpjerr = 1.5;
	double thresh_inlier_ratio = 0.15;

	int n_pointcloud_size_old = map_pointcloud.size();

	bool bAllFail = false;
	while (imgRank.size()>0 && !bAllFail) // 还有图没有完成定向且并不是所有剩余图像都定向失败了就继续
	{
		bAllFail = true;
		vector<SfM_ZZK::pair_ij> imgRank_tmp = imgRank;

		for (auto iter_img=imgRank_tmp.begin();iter_img!=imgRank_tmp.end();++iter_img)
		{
			const int & I = iter_img->first; // image I

			cam_data & cam_I = pApp->m_vCams[I];

			// 1. 尝试 RANSAC 后方交会
			Matx33d mR;
			Matx31d mt;
			bool bEOSuc;
			try
			{
				bEOSuc = EO_PnP_RANSAC(cam_I, map_pointcloud, mR, mt, thresh_inlier_rpjerr, thresh_inlier_ratio);
			}
			catch (cv::Exception & e)
			{
				CString str;
				str = e.msg.c_str();
				str+="	error happened in RANSAC EO";
				AfxMessageBox(str);
			}
			
			if (!bEOSuc) // 如果后方交会失败就考虑排在后面的图像
			{
				continue;
			}

			// 到这里就说明后方交会成功了
			bAllFail = false;
			
			// 正式录入外参初值
			cam_I.m_R = mR;
			cam_I.m_t = mt;

			status[I] = 1;
			cam_I.m_bOriented = true;

			// 2. 稀疏光束法平差。只利用现有的物点，还没有利用新加入的图像前方交会新的物点
			theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(_T("SBA starts"));
			double info[10];
			try
			{
				int nnn = optim_sba_levmar_XYZ_ext_rotvec(map_pointcloud, pApp->m_vCams, map_tracks, idx_refimg, thresh_inlier_rpjerr, 64, opts, info);
			}
			catch (cv::Exception & e)
			{
				CString str;
				str = e.msg.c_str();
				str+="	error happened in SBA";
				AfxMessageBox(str);
			}
			strInfo.Format("SBA ends, point cloud size: %d, initial err: %lf, final err: %lf, iter: %04.0f, code: %01.0f",
				map_pointcloud.size(), info[0], info[1], info[3], info[4]);
			theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

			strFile.Format("point cloud after successful EO of image %03d and bundle adjustment.txt", I);
			try
			{
				SfM_ZZK::OutputPointCloud(pApp->m_strOut+strFile,map_pointcloud,pApp->m_vCams,map_tracks,pApp->m_cloud_old,3);
//				SfM_ZZK::Draw3DScene(pApp->m_wnd3d, pApp->m_ptcloud, map_pointcloud, pApp->m_vCams, map_tracks, 3);
			}
			catch (cv::Exception & e)
			{
				CString str;
				str = e.msg.c_str();
				str+="	error happened in OutputPointCloud";
				AfxMessageBox(str);
			}

			// 更新阈值
//			thresh_inlier_rpjerr = 3*info[1];

			// 3. 前方交会
			try
			{
				SfM_ZZK::Triangulation_AddOneImg(map_pointcloud,pApp->m_vCams,map_tracks,I,thresh_inlier_rpjerr);
			}
			catch (cv::Exception & e)
			{
				CString str;
				str = e.msg.c_str();
				str+="	error happened in Triangulation";
				AfxMessageBox(str);
			}
			

			// 4. 如果点云拓展了，就对剩下的图像重新进行排序
			int n_pointcloud_size_new = map_pointcloud.size();
			if (n_pointcloud_size_new != n_pointcloud_size_old)
			{
				try
				{
					RankImages_NumObjPts(pApp->m_vCams, map_pointcloud, imgRank);
				}
				catch (cv::Exception & e)
				{
					CString str;
					str = e.msg.c_str();
					str+="	error happened in RankImages";
					AfxMessageBox(str);
				}

				n_pointcloud_size_old = n_pointcloud_size_new;
				break;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////

	// 最后总的来一次稀疏光束法平差
	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(_T("Final SBA starts"));
	double info[10];
	int nnn = optim_sba_levmar_XYZ_ext_rotvec(map_pointcloud, pApp->m_vCams, map_tracks, idx_refimg, thresh_inlier_rpjerr, /*1024*/64,opts, info);
	strInfo.Format("Final SBA ends, point cloud size: %d, initial err: %lf, final err: %lf, iter: %04.0f, code: %01.0f",
		map_pointcloud.size(), info[0], info[1], info[3], info[4]);
	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

	std::map<int,int> hist_cloudpoint_inlier;
	double length_average = SfM_ZZK::BuildCloudPointInlierHistogram(map_pointcloud,map_tracks,hist_cloudpoint_inlier);

	FILE * file = fopen("E:\\results\\hist.txt", "w");
	for (auto iter_hist=hist_cloudpoint_inlier.begin();iter_hist!=hist_cloudpoint_inlier.end();++iter_hist)
	{
		fprintf(file,"%d	%d\n", iter_hist->first, iter_hist->second);
	}
	fprintf(file,"%lf", length_average);
	fclose(file);

	// 输出点云
	strFile.Format("Final point cloud.txt");
	SfM_ZZK::OutputPointCloud(pApp->m_strOut+strFile,map_pointcloud,pApp->m_vCams,map_tracks,pApp->m_cloud_old,3);
//	SfM_ZZK::Draw3DScene(pApp->m_wnd3d, pApp->m_ptcloud, map_pointcloud, pApp->m_vCams, map_tracks, 3);

	// 输出像机定向
	// save all cameras' parameters
	for (int i=0;i<nCam;i++)
	{
		CString strtmp;
		strFile.Format("cam%02d.txt", i);
//		SaveCameraData(strOut+strFile, pApp->m_vCams[i]);
	}

//	vector<vector<int>> vIdxSupports;
	ScoreMatchingImages(map_pointcloud, pApp->m_vCams, map_pairwise_matches, map_tracks, pApp->m_vIdxSupports, nSptImgs_desired, 10);

	return TRUE;
}

UINT DenseReconstruction(LPVOID param)
{
	CDeepVoidApp * pApp = (CDeepVoidApp * )param;

	// 20150206, zhaokunz, try dense reconstruction //////////////////////////
	vector<Mat> vDepths, vHxs, vHys, vScores, vVisis;

	int wndSize = 5;
	double thresh_norm = 80;
	int nPatchMatchIter = 2;
	double factor = 0.25;
	int nRandSamp = 5;
	double img_sigma = 0.5;

	vector<Mat> vSilhouettes;

	/*CString strOut;
	vector<CString> vImgPaths;
	vector<vector<int>> vIdxSupports;
	vector<CloudPoint> cloud_old;*/

	MVDE_package_150206(pApp->m_strOut,pApp->m_vCams,pApp->m_vImgPaths,vSilhouettes,pApp->m_vIdxSupports,pApp->m_cloud_old,vDepths,vHxs,vHys,vScores,vVisis,
		wndSize,thresh_norm,nPatchMatchIter,factor,nRandSamp,90,0.95,1.5,0.01,img_sigma,2,1,0.01,0.02,5);

	return TRUE;
}

void CDeepVoidApp::On3dreconstructionParametersettings()
{
	// TODO: Add your command handler code here

//	wrd3d.saveScreenshot("C:\\Users\\DeepV\\Desktop\\screenshot.png");
	
	CDlgStereoFiles dlgFiles;
	dlgFiles.m_pathParam0 = m_pathImageCalibration;
	dlgFiles.m_nPixDownSample = m_nPixDownSample;
	
	if (IDOK == dlgFiles.DoModal())
	{
		m_pathImageCalibration = dlgFiles.m_pathParam0;
		
		if (m_pathImageCalibration == "")
		{
			// 说明没有输入参数
			return;
		}

		// 有参数的话，就读入参数
		FILE * file = fopen(m_pathImageCalibration, "r");
		if (!file)
		{
			AfxMessageBox("fail to read in parameters of image0");
			return;
		}
		
		fscanf(file, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", 
			&m_fx, &m_fy, &m_cx, &m_cy, &m_s, &m_k1, &m_k2, &m_k3, &m_k4, &m_k5);
				
		fclose(file);
	}
}

void CDeepVoidApp::On3dreconstructionNetworkorientationandsparsereconstruction()
{
	// TODO: Add your command handler code here
	AfxBeginThread(NetworkOrientation, this, THREAD_PRIORITY_NORMAL);

//	m_wnd3d = viz::Viz3d("3D Window");
//	m_wnd3d.spinOnce();
}

void CDeepVoidApp::On3dreconstructionDensereconstruction()
{
	// TODO: Add your command handler code here
	AfxBeginThread(DenseReconstruction, this, THREAD_PRIORITY_NORMAL);

// 	m_wnd3d.spinOnce();
// 
// 	wrd3d.showWidget("Coordinate Widget", viz::WCoordinateSystem());
// 
// 	/// Let's assume camera has the following properties
// 	Point3d cam_pos(2.0f, 0.0f, 0.0f), cam_focal_point(3.0f, 0.0f, 0.0f), cam_y_dir(-2.0f, -1.0f, 0.0f);
// 
// 	/// We can get the pose of the cam using makeCameraPose
// 	Affine3f cam_pose = viz::makeCameraPose(cam_pos, cam_focal_point, cam_y_dir);
// 
// 	Mat image = imread("C:\\Users\\DeepV\\Pictures\\DSC_1557_edited.JPG", CV_LOAD_IMAGE_UNCHANGED);
// 
// 	double width = image.cols;
// 	double height = image.rows;
// 
// 	Matx33d K;
// 	K(0, 0) = K(1, 1) = 7500;
// 	K(0, 2) = (width - 1)*0.5;
// 	K(1, 2) = (height - 1)*0.5;
// 	K(2, 2) = 1;
// 
// 	viz::WCameraPosition cpw(0.5); // Coordinate axes
// 	viz::WCameraPosition cpw_frustum(Vec2f(0.889484, 0.223599), 0.5); // Camera frustum
// 	viz::WCameraPosition cpw_image(K,image);
// 	wrd3d.showWidget("CPW", cpw, cam_pose);
// //	wrd3d.showWidget("CPW_FRUSTUM", cpw_frustum, cam_pose); 
// 	wrd3d.showWidget("CPW_FRUSTUM", cpw_image, cam_pose);
// 
// 	Mat XYZs(1, 5, CV_64FC3);
// 	XYZs.at<Vec3d>(0, 0).val[0] = 0;	XYZs.at<Vec3d>(0, 0).val[1] = 1;	XYZs.at<Vec3d>(0, 0).val[2] = 1;
// 	XYZs.at<Vec3d>(0, 1).val[0] = 0;	XYZs.at<Vec3d>(0, 1).val[1] = 0;	XYZs.at<Vec3d>(0, 1).val[2] = 1;
// 	XYZs.at<Vec3d>(0, 2).val[0] = 1;	XYZs.at<Vec3d>(0, 2).val[1] = 1;	XYZs.at<Vec3d>(0, 2).val[2] = 2;
// 	XYZs.at<Vec3d>(0, 3).val[0] = 2;	XYZs.at<Vec3d>(0, 3).val[1] = 2;	XYZs.at<Vec3d>(0, 3).val[2] = 1;
// 	XYZs.at<Vec3d>(0, 4).val[0] = 2;	XYZs.at<Vec3d>(0, 4).val[1] = 3;	XYZs.at<Vec3d>(0, 4).val[2] = 0;
// 	viz::WCloud cld(XYZs);
// 	wrd3d.showWidget("cloud", cld);
// 
// 	image.release();
// 
// //	wrd3d.saveScreenshot("C:\\Users\\DeepV\\Desktop\\screenshot.png");
// 
// 	wrd3d.spinOnce();
// 
// //	wrd3d.saveScreenshot("C:\\Users\\DeepV\\Desktop\\screenshot.png");
}

viz::Viz3d wnd3d("3D View");

void CDeepVoidApp::On3dview()
{
	// TODO: Add your command handler code here
//	SfM_ZZK::Draw3DScene(m_wnd3d, m_ptcloud, m_mapPointCloud, m_vCams, m_mapTracks, 2);

	// 初始化三维显示窗口
//	viz::Viz3d wnd3d("3D View");
	wnd3d.setWindowSize(cv::Size(800, 600));
	wnd3d.setWindowPosition(cv::Point(150, 150));
	wnd3d.setBackgroundColor(); // black by default
//	wnd3d.setBackgroundColor(cv::viz::Color::white());
//	wnd3d.setBackgroundMeshLab(); // MeshLab style background

	CString str;

	viz::Color color = viz::Color::white(); // 默认画白色

	// 画不确定度椭球的颜色
	vector<viz::Color> colors;
	colors.push_back(viz::Color::Color(255, 255, 0));	// 青
	colors.push_back(viz::Color::Color(255, 0, 255));	// 品
	colors.push_back(viz::Color::Color(0, 255, 255));	// 黄

	// 可视化的最差相对不确定度和最优相对不确定度
	double rltUctt_worst = 0.3;
	double rltUctt_best = 0.001;

	// 显示多少倍sigma的不确定度椭球
	double nSigma = 3.0;

	// 先显示三维点云
	// 要显示出来的点云点及其颜色
	int n_minInilier = 2;

	vector<Point3d> pts_output;
	vector<Point3i> colors_output;

	// 统计看哪些点满足要求可以显示
	for (auto iter_wrdpt = m_mapPointCloud.begin(); iter_wrdpt != m_mapPointCloud.end(); ++iter_wrdpt)
	{
		const int & trackID = iter_wrdpt->first;

		auto iter_found_track = m_mapTracks.find(trackID);

		double sumR = 0;
		double sumG = 0;
		double sumB = 0;

		int count = 0;

		for (auto iter_imgpt = iter_found_track->second.begin(); iter_imgpt != iter_found_track->second.end(); ++iter_imgpt)
		{
			const int & I = iter_imgpt->first;
			const int & i = iter_imgpt->second.first;
			const int & bInlier = iter_imgpt->second.second[0]; // 20220202，第一个标志指明该点是否为内点

			if (!bInlier)
			{
				continue;
			}

			const cam_data & cam = m_vCams[I];

			sumR += cam.m_feats.rgbs[i][2];
			sumG += cam.m_feats.rgbs[i][1];
			sumB += cam.m_feats.rgbs[i][0];

			++count;
		}

		if (count < n_minInilier)
		{
			continue;
		}

		int R = (int)sumR / count;
		int G = (int)sumG / count;
		int B = (int)sumB / count;

		Point3i color;
		color.x = R;
		color.y = G;
		color.z = B;

		// 20200717，根据相对不确定度大小输出颜色 //////////////////////////
		DeepVoid::getRGColorforRelativeUncertainty(iter_wrdpt->second.m_rltUctt, rltUctt_worst, rltUctt_best, color.x, color.y, color.z);
		//////////////////////////////////////////////////////////////////

		pts_output.push_back(iter_wrdpt->second.m_pt);
		colors_output.push_back(color);

		// 20200712，把物点位置不确定度椭球可视化
		//Matx31d XYZ;
		//XYZ(0) = iter_wrdpt->second.m_pt.x;
		//XYZ(1) = iter_wrdpt->second.m_pt.y;
		//XYZ(2) = iter_wrdpt->second.m_pt.z;

		//Point3d ptStart, ptEnd;
		//// 有 3 个互为正交的轴要画
		//for (int ii = 0; ii < 3; ++ii)
		//{
		//	Matx31d vec = nSigma*iter_wrdpt->second.m_uncertaintyEllipsoid.row(ii).t();
		//	Matx31d XYZ_start = XYZ - vec;
		//	Matx31d XYZ_end = XYZ + vec;

		//	ptStart.x = XYZ_start(0);
		//	ptStart.y = XYZ_start(1);
		//	ptStart.z = XYZ_start(2);

		//	ptEnd.x = XYZ_end(0);
		//	ptEnd.y = XYZ_end(1);
		//	ptEnd.z = XYZ_end(2);

		//	str.Format("wrdpt ellipsoid %d	%d", pts_output.size() - 1, ii);

		//	wnd3d.showWidget(str.GetBuffer(), viz::WLine(ptStart, ptEnd, colors[ii]));
		//}
	}

	int n_pts_output = pts_output.size();

	Mat XYZs(1, n_pts_output, CV_64FC3), RGBs(1, n_pts_output, CV_8UC3);

	for (int i = 0; i < n_pts_output; ++i)
	{
		Point3d XYZ = pts_output[i];
		Point3i RGB = colors_output[i];

		XYZs.at<Vec3d>(i).val[0] = XYZ.x;
		XYZs.at<Vec3d>(i).val[1] = XYZ.y;
		XYZs.at<Vec3d>(i).val[2] = XYZ.z;

		RGBs.at<Vec3b>(i).val[0] = RGB.z;
		RGBs.at<Vec3b>(i).val[1] = RGB.y;
		RGBs.at<Vec3b>(i).val[2] = RGB.x;
	}

	viz::WCloud cld(XYZs, RGBs);
//	viz::WCloud cld(XYZs, viz::Color::green());

	wnd3d.showWidget("cloud", cld);

	// 再显示相机坐标系及图像
	int m = m_vCams.size();	

	vector<cv::Affine3d> imgTraj;

	for (int i = 0; i < m; ++i)
	{
		const cam_data & cam = m_vCams[i];

		if (!cam.m_bOriented) // 没有完成定向的图像就不显示
		{
			continue;
		}

		// Xc = R*Xw + t => Xw = R'(Xc - t)
		// 将光心坐标 Xc=(0,0,0)代入，则有光心世界坐标 Xw = -R't
		Matx31d C = -cam.m_R.t()*cam.m_t;

		cv::Vec3d c;
		c(0) = C(0);
		c(1) = C(1);
		c(2) = C(2);

		cv::Affine3d pose(cam.m_R.t(), c); // 实际尝试发现这个 cv::Affine3d 中的所谓 t 应该是光心坐标，而旋转矩阵是从相机系往世界系转的。。。

		imgTraj.push_back(pose);
		
		str.Format("image %03d", i);

		// 画视景锥，以及实测图像
		wnd3d.showWidget(str.GetBuffer(), viz::WCameraPosition(cam.m_K, m_imgsOriginal[i], 1.0, color), pose);

		str.Format("%d", i);

		// 标出图像序号
		wnd3d.showWidget(str.GetBuffer(), viz::WText3D(m_vNameImgs[i].GetBuffer()/*str.GetBuffer()*/, cv::Point3d(c), 0.5, true, color));

		// 20200712，把光心位置不确定度椭球可视化
		Point3d ptStart, ptEnd;
		// 有 3 个互为正交的轴要画
		for (int ii = 0; ii < 3; ++ii)
		{
			Matx31d vec = nSigma*cam.m_optCtrUncertEllipsoid.row(ii).t();
			Matx31d XYZ_start = C - vec;
			Matx31d XYZ_end = C + vec;

			ptStart.x = XYZ_start(0);
			ptStart.y = XYZ_start(1);
			ptStart.z = XYZ_start(2);

			ptEnd.x = XYZ_end(0);
			ptEnd.y = XYZ_end(1);
			ptEnd.z = XYZ_end(2);

			str.Format("%d	%d", i, ii);

			wnd3d.showWidget(str.GetBuffer(), viz::WLine(ptStart, ptEnd, colors[ii]));
		}
	}

	// 显示图像移动路径
	// BOTH 是既画路径线PATH，又画坐标轴FRAMES
	wnd3d.showWidget("image trajectory", viz::WTrajectory(imgTraj, viz::WTrajectory::BOTH, 1.0, color));

	// 20220127，表征开启了三维显示，使能三维视图截图功能
	m_b3DViewOn = TRUE;

	// 正式开独立线程开始显示。
	wnd3d.spin();
}

UINT ExtractSift(LPVOID param)
{
	CDeepVoidApp * pApp = (CDeepVoidApp *)param;

	CShowInfoListCtrl & listCtrl = pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl;

	const CString & dirOut = pApp->m_pathDirOut; // 20220130，结果输出文件夹路径
	const std::vector<CString> & vNameImgs = pApp->m_vNameImgs; // 20220130，所有图像的纯文件名（不含路径和尾缀）

	int nImg = pApp->m_vCams.size();
	CString strInfo;

	for (int i = 0; i < nImg; ++i)
	{
		const cv::Mat & img = pApp->m_imgsOriginal[i];
		cam_data & cam = pApp->m_vCams[i];
		CImageDoc * pDoc = pApp->m_vPImgCocs[i];

		cam.m_featsCorner.clear();

		cam.ExtractSiftFeatures(img, pApp->m_nfeaturesSift, pApp->m_nOctaveLayersSift, pApp->m_contrastThresholdSift, pApp->m_edgeThresholdSift, pApp->m_sigmaSift);
		cam.GenSfMFeatures(pApp->m_nSfMFeatures, pApp->m_nPrptFeatures);

		strInfo.Format(vNameImgs[i] + " extracted %07d features", /*i, */cam.m_feats.key_points.size());
		listCtrl.AddOneInfo(strInfo);

		if (!pDoc || !pDoc->m_pImgView)
		{
			continue;
		}

		pDoc->m_pImgView->m_flagShow = 2; // 仅显示入选参加 SfM 的特征点
		pDoc->m_pImgView->m_bShowBlob = TRUE; // 显示 sift 特征点
		pDoc->m_pImgView->m_bShowManual = TRUE; // 显示手提点
//		pDoc->m_pImgView->m_bShowID = FALSE; // 不显示 ID 号
		pDoc->m_pImgView->m_bShowTrackID = FALSE; // 更不显示 track ID 号，因为重新提取了特征点要重新进行特征跟踪
		pDoc->m_pImgView->Invalidate(TRUE);
	}

	pApp->m_bSfMFeatsReady = TRUE;

	return TRUE;
}

void CDeepVoidApp::OnFeaturesExtractsift()
{
	// TODO: Add your command handler code here

	if (m_vCams.size() < 1) // 没有图像就直接退出
	{
		return;
	}

	AfxBeginThread(ExtractSift, this, THREAD_PRIORITY_NORMAL);
}

UINT ExtractFAST(LPVOID param)
{
	CDeepVoidApp * pApp = (CDeepVoidApp *)param;

	CShowInfoListCtrl & listCtrl = pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl;

	const CString & dirOut = pApp->m_pathDirOut; // 20220130，结果输出文件夹路径
	const std::vector<CString> & vNameImgs = pApp->m_vNameImgs; // 20220130，所有图像的纯文件名（不含路径和尾缀）

	int nImg = pApp->m_vCams.size();
	CString strInfo;

	for (int i = 0; i < nImg; ++i)
	{
		const cv::Mat & img = pApp->m_imgsOriginal[i];
		cam_data & cam = pApp->m_vCams[i];
		CImageDoc * pDoc = pApp->m_vPImgCocs[i];

		cam.m_featsBlob.clear();

		cam.ExtractFASTFeatures(img, pApp->m_thresholdOffset, pApp->m_rMax, pApp->m_thresholdFast, pApp->m_nonmaxSuppressionFast, pApp->m_typeFast,
			pApp->m_nfeaturesSift, pApp->m_nOctaveLayersSift, pApp->m_contrastThresholdSift, pApp->m_edgeThresholdSift, pApp->m_sigmaSift);
		cam.GenSfMFeatures(pApp->m_nSfMFeatures, pApp->m_nPrptFeatures);

		strInfo.Format(vNameImgs[i] + " extracted %07d features", /*i, */cam.m_feats.key_points.size());
		listCtrl.AddOneInfo(strInfo);

		if (!pDoc || !pDoc->m_pImgView)
		{
			continue;
		}

		pDoc->m_pImgView->m_flagShow = 2; // 仅显示入选参加 SfM 的特征点
		pDoc->m_pImgView->m_bShowCorner = TRUE; // 显示 FAST 特征点
		pDoc->m_pImgView->m_bShowManual = TRUE; // 显示手提点
//		pDoc->m_pImgView->m_bShowID = FALSE; // 不显示 ID 号
		pDoc->m_pImgView->m_bShowTrackID = FALSE; // 更不显示 track ID 号，因为重新提取了特征点要重新进行特征跟踪
		pDoc->m_pImgView->Invalidate(TRUE);
	}

	pApp->m_bSfMFeatsReady = TRUE;

	return TRUE;
}

void CDeepVoidApp::OnFeaturesExtractfast()
{
	// TODO: Add your command handler code here

	if (m_vCams.size() < 1) // 没有图像就直接退出
	{
		return;
	}

	AfxBeginThread(ExtractFAST, this, THREAD_PRIORITY_NORMAL);
}

UINT ExtractSiftandFAST(LPVOID param)
{
	CDeepVoidApp * pApp = (CDeepVoidApp *)param;

	CShowInfoListCtrl & listCtrl = pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl;

	const CString & dirOut = pApp->m_pathDirOut; // 20220130，结果输出文件夹路径
	const std::vector<CString> & vNameImgs = pApp->m_vNameImgs; // 20220130，所有图像的纯文件名（不含路径和尾缀）

	int nImg = pApp->m_vCams.size();
	CString strInfo;

	for (int i = 0; i < nImg; ++i)
	{
		const cv::Mat & img = pApp->m_imgsOriginal[i];
		cam_data & cam = pApp->m_vCams[i];
		CImageDoc * pDoc = pApp->m_vPImgCocs[i];

		cam.ExtractSiftFeatures(img, pApp->m_nfeaturesSift, pApp->m_nOctaveLayersSift, pApp->m_contrastThresholdSift, pApp->m_edgeThresholdSift, pApp->m_sigmaSift);
		cam.ExtractFASTFeatures(img, pApp->m_thresholdOffset, pApp->m_rMax, pApp->m_thresholdFast, pApp->m_nonmaxSuppressionFast, pApp->m_typeFast,
			pApp->m_nfeaturesSift, pApp->m_nOctaveLayersSift, pApp->m_contrastThresholdSift, pApp->m_edgeThresholdSift, pApp->m_sigmaSift);
		cam.GenSfMFeatures(pApp->m_nSfMFeatures, pApp->m_nPrptFeatures);

		strInfo.Format(vNameImgs[i] + " extracted %07d features", /*i, */cam.m_feats.key_points.size());
		listCtrl.AddOneInfo(strInfo);

		if (!pDoc || !pDoc->m_pImgView)
		{
			continue;
		}

		pDoc->m_pImgView->m_flagShow = 2; // 仅显示入选参加 SfM 的特征点
		pDoc->m_pImgView->m_bShowBlob = TRUE; // 显示 sift 特征点
		pDoc->m_pImgView->m_bShowCorner = TRUE; // 显示 FAST 特征点
		pDoc->m_pImgView->m_bShowManual = TRUE; // 显示手提点
//		pDoc->m_pImgView->m_bShowID = FALSE; // 不显示 ID 号
		pDoc->m_pImgView->m_bShowTrackID = FALSE; // 更不显示 track ID 号，因为重新提取了特征点要重新进行特征跟踪
		pDoc->m_pImgView->Invalidate(TRUE);
	}

	pApp->m_bSfMFeatsReady = TRUE;

	return TRUE;
}

void CDeepVoidApp::OnFeaturesSiftfast()
{
	// TODO: Add your command handler code here

	if (m_vCams.size() < 1) // 没有图像就直接退出
	{
		return;
	}

	AfxBeginThread(ExtractSiftandFAST, this, THREAD_PRIORITY_NORMAL);
}

UINT GenSfMFeatures(LPVOID param)
{
	CDeepVoidApp * pApp = (CDeepVoidApp *)param;

	CShowInfoListCtrl & listCtrl = pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl;

	const CString & dirOut = pApp->m_pathDirOut; // 20220130，结果输出文件夹路径
	const std::vector<CString> & vNameImgs = pApp->m_vNameImgs; // 20220130，所有图像的纯文件名（不含路径和尾缀）

	int nImg = pApp->m_vCams.size();
	CString strInfo;

	for (int i = 0; i < nImg; ++i)
	{
		cam_data & cam = pApp->m_vCams[i];
		CImageDoc * pDoc = pApp->m_vPImgCocs[i];

		cam.GenSfMFeatures(pApp->m_nSfMFeatures, pApp->m_nPrptFeatures);

		strInfo.Format(vNameImgs[i] + " extracted %07d features", /*i, */cam.m_feats.key_points.size());
		listCtrl.AddOneInfo(strInfo);

		if (!pDoc || !pDoc->m_pImgView)
		{
			continue;
		}

		pDoc->m_pImgView->m_flagShow = 2; // 仅显示入选参加 SfM 的特征点
		pDoc->m_pImgView->m_bShowBlob = TRUE; // 显示 sift 特征点
		pDoc->m_pImgView->m_bShowCorner = TRUE; // 显示 FAST 特征点
		pDoc->m_pImgView->m_bShowManual = TRUE; // 显示手提点
//		pDoc->m_pImgView->m_bShowID = FALSE; // 不显示 ID 号
		pDoc->m_pImgView->m_bShowTrackID = FALSE; // 更不显示 track ID 号，因为重新提取了特征点要重新进行特征跟踪
		pDoc->m_pImgView->Invalidate(TRUE);
	}

	pApp->m_bSfMFeatsReady = TRUE;

	return TRUE;
}

void CDeepVoidApp::On1featuresGenfeaturesforsfm()
{
	// TODO: Add your command handler code here

	if (m_vCams.size() < 1) // 没有图像就直接退出
	{
		return;
	}

	AfxBeginThread(GenSfMFeatures, this, THREAD_PRIORITY_NORMAL);
}


void CDeepVoidApp::OnFeaturesDeleteall()
{
	// TODO: Add your command handler code here
	int nImg = m_vCams.size();

	if (nImg < 1) // 没有图像就直接退出
	{
		return;
	}

	for (int i = 0; i < nImg; ++i)
	{
		cam_data & cam = m_vCams[i];
		CImageDoc * pDoc = m_vPImgCocs[i];

		cam.DeleteAllFeatures();

		if (!pDoc || !pDoc->m_pImgView)
		{
			continue;
		}

		pDoc->m_pImgView->Invalidate(TRUE);
	}

	m_bSfMFeatsReady = FALSE;
}


void CDeepVoidApp::On0settingsFeatures()
{
	// TODO: Add your command handler code here
}


void CDeepVoidApp::On0settings2featurematching()
{
	// TODO: Add your command handler code here
}


UINT TwoViewFeatureMatching(LPVOID param)
{
	CDeepVoidApp * pApp = (CDeepVoidApp *)param;

	CShowInfoListCtrl & listCtrl = pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl;

	const CString & dirOut = pApp->m_pathDirOut; // 20220130，输出文件夹路径
	const std::vector<CString> & vNameImgs = pApp->m_vNameImgs; // 20220130，所有图像的纯名称（不带路径、尾缀）

	// 下面这两个变量是图像特征匹配环节的核心成果，所以需要先清空归零。
	pApp->m_mapPairwiseFMatchesWrdPts.clear(); // 先清除掉所有匹配映射
	pApp->m_mapTracks.clear(); // 先清空

	int nImg = pApp->m_vCams.size();
	CString strInfo;

	// 20151217，为了并行，先把所有可能的图像对列出来，后续直接通过循环而非嵌套循环的方式并行
	vector<std::pair<int, int>> vec_pairs;

	for (int i = 0; i < nImg; ++i)
	{
		for (int j = i + 1; j < nImg; ++j)
		{
			vec_pairs.push_back(make_pair(i, j));
		}
	}

	int nPair = vec_pairs.size();


	// 1. Two-View Feature Matching.
	for (int k = 0; k < nPair; ++k)
	{
		const std::pair<int, int> & ij = vec_pairs[k];
		int i = ij.first;
		int j = ij.second;

		const cam_data & cam_i = pApp->m_vCams[i];
		const cam_data & cam_j = pApp->m_vCams[j];

		vector<DMatch> matches;

		// 20150113, zhaokunz, new matching function
		Matx33d mF;
		vector<Point3d> wrdPts;

		bool bSuc = false;

		// 20200816 ////////////////////////////////////////////////////////////////////////////////////////
		vector<DMatch> matchesTmp;
		Matx33d mFtmp;
		vector<Point3d> wrdPtstmp;

		if (cam_i.m_featsManual.key_points.size() > 0 && cam_j.m_featsManual.key_points.size() > 0)
		{
			ManualFeatureMatching(cam_i.m_featsManual, cam_j.m_featsManual, mFtmp, matchesTmp, wrdPtstmp, 2);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////

		// 20200629, “先发制人”特征匹配
		if (pApp->m_nPrptFeatures < cam_i.m_feats.key_points.size())
		{
			if (PreemptiveFeatureMatching(cam_i.m_subFeats, cam_j.m_subFeats, pApp->m_fmThreshRatioTest/*0.65*/, pApp->m_prptTh/*1*/))
			{
				// 20200621, 同步输出射影重建物点坐标
				bSuc = Get_F_Matches_pWrdPts_knn(cam_i.m_feats, cam_j.m_feats, mF, matches, wrdPts,
					pApp->m_fmbOptim, pApp->m_fmThreshRatioTest/*0.65*/, pApp->m_fmThreshMinInlierRatio/*0.5*/,
					pApp->m_fmThreshP2L, pApp->m_fmThreshConf, pApp->m_fmMaxIter/*64*/, pApp->m_fmxEps, pApp->m_fmfEps);
			}
		}
		else
		{
			bSuc = Get_F_Matches_pWrdPts_knn(cam_i.m_feats, cam_j.m_feats, mF, matches, wrdPts,
				pApp->m_fmbOptim, pApp->m_fmThreshRatioTest/*0.65*/, pApp->m_fmThreshMinInlierRatio/*0.5*/,
				pApp->m_fmThreshP2L, pApp->m_fmThreshConf, pApp->m_fmMaxIter/*64*/, pApp->m_fmxEps, pApp->m_fmfEps);
		}

		if (bSuc)
		{
			strInfo.Format("matching between " + vNameImgs[i] + " and " + vNameImgs[j] + " finished: %04d matches are found", matches.size());
			listCtrl.AddOneInfo(strInfo);

			pApp->m_mapPairwiseFMatchesWrdPts.insert(make_pair(make_pair(i, j), make_pair(make_pair(mF, matches), wrdPts)));
		}
	}

	
	// 2. Feature Tracking.
// 	SfM_ZZK::MultiTracks map_tracks_init;
	SfM_ZZK::MultiTracksWithFlags map_tracks_init; // 20220202，新数据结构
	SfM_ZZK::FindAllTracks_Olsson(pApp->m_mapPairwiseFMatchesWrdPts, map_tracks_init, pApp->m_nFlagPerImgPt); // 20200622

	// 确保特征轨迹从0开始依次计数
	// 并建立特征轨迹中包含的特征点至该特征轨迹的映射
	int idx_count = 0;
	for (auto iter_track = map_tracks_init.begin(); iter_track != map_tracks_init.end(); ++iter_track)
	{
		pApp->m_mapTracks.insert(make_pair(idx_count, iter_track->second));

		// 建立该特征轨迹中包含的特征点至该特征轨迹的映射，通过 trackID 来索引
		for (auto iter_Ii = iter_track->second.begin(); iter_Ii != iter_track->second.end(); ++iter_Ii)
		{
			const int & I = iter_Ii->first; // image I
			const int & i = iter_Ii->second.first; // feature i

			cam_data & cam = pApp->m_vCams[I];

			cam.m_feats.tracks[i] = idx_count;

			// 20200810，给 sift、fast 特征点 和 手提点赋上全局 trackID，以便显示。
			if (i < cam.m_nSiftElected)
			{
				cam.m_featsBlob.tracks[i] = idx_count;
			}
			else if (i >= cam.m_nSiftElected && i < (cam.m_nSiftElected + cam.m_nFastElected))
			{
				cam.m_featsCorner.tracks[i - cam.m_nSiftElected] = idx_count;
			}
			else
			{
				cam.m_featsManual.tracks[i - cam.m_nSiftElected - cam.m_nFastElected] = idx_count;
			}
		}

		++idx_count;
	}

	// 刷新并显示每个特征/手提点的全局 trackID 号
	for (int i = 0; i < nImg; ++i)
	{
		CImageDoc * pDoc = pApp->m_vPImgCocs[i];

		if (!pDoc || !pDoc->m_pImgView)
		{
			continue;
		}

		pDoc->m_pImgView->m_bShowID = TRUE;
		pDoc->m_pImgView->m_bShowTrackID = TRUE;
		pDoc->m_pImgView->Invalidate(TRUE);
	}

	// 统计特征轨迹直方图
	std::map<int, int> hist_track;
	SfM_ZZK::BuildTrackLengthHistogram(pApp->m_mapTracks, hist_track);

	int n_tracklength_more_than_1 = 0;
	for (auto iter_n = hist_track.begin(); iter_n != hist_track.end(); ++iter_n)
	{
		if (iter_n->first < 2)
		{
			continue;
		}
		n_tracklength_more_than_1 += iter_n->second;
	}

	strInfo.Format("number of good tracks: %07d", n_tracklength_more_than_1);
	listCtrl.AddOneInfo(strInfo);

	pApp->m_bTracksReady = TRUE;

	return TRUE;
}


void CDeepVoidApp::On2featurematching()
{
	// TODO: Add your command handler code here

	if (m_vCams.size() < 2) // 至少得有 2 幅图像才能进行两视图特征匹配
	{
		return;
	}

	AfxBeginThread(TwoViewFeatureMatching, this, THREAD_PRIORITY_NORMAL);
}


// UINT ExtractManual(LPVOID param)
// {
// 	CImageDoc * pDoc = (CImageDoc *)param;
// 
// 	pDoc->ExtractPointsContinuously(FALSE);
// 
// 	return TRUE;
// }


void CDeepVoidApp::On1featuresManual()
{
	// TODO: Add your command handler code here
	int nImg = m_vCams.size();

	if (nImg < 1) // 没有图像就直接退出
	{
		return;
	}

	for (int i = 0; i < nImg; ++i)
	{
		CImageDoc * pDoc = m_vPImgCocs[i];

		if (!pDoc || !pDoc->m_pImgView)
		{
			continue;
		}

		pDoc->ExtractPointsContinuously(FALSE);
//		AfxBeginThread(ExtractManual, pDoc, THREAD_PRIORITY_NORMAL);
	}
}


void CDeepVoidApp::OnUpdateFeaturesExtractsift(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_bNoneImages);
}


void CDeepVoidApp::OnUpdateFeaturesExtractfast(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_bNoneImages);
}


void CDeepVoidApp::OnUpdateFeaturesSiftfast(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_bNoneImages);
}


void CDeepVoidApp::OnUpdate1featuresGenfeaturesforsfm(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_bNoneImages);
}


void CDeepVoidApp::OnUpdateFeaturesDeleteall(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_bNoneImages);
}


void CDeepVoidApp::OnUpdate2featurematching(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bSfMFeatsReady);
}


void CDeepVoidApp::OnUpdate1featuresManual(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_bNoneImages);
}


void CDeepVoidApp::OnCapture3dview()
{
	// TODO: Add your command handler code here
	CString fileName = m_pathDirOut + "3D view capture.png";
	wnd3d.saveScreenshot(fileName.GetBuffer());
}


void CDeepVoidApp::OnUpdate3dview(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_b3DReady_sparse);
}


void CDeepVoidApp::OnUpdateCapture3dview(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_b3DViewOn);
}


void CDeepVoidApp::On3sfmIncrementalsfm()
{
	// TODO: Add your command handler code here
// 	if (m_vCams.size() < 1) // 没有图像就直接退出
// 	{
// 		return;
// 	}

	AfxBeginThread(SfM_incremental, this, THREAD_PRIORITY_NORMAL);
}


void CDeepVoidApp::OnUpdate3sfmIncrementalsfm(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bTracksReady);
}


void CDeepVoidApp::OnUpdate3sfmGlobalsfm(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bTracksReady);
}


UINT SBA_DbSBA_compare_realdata(LPVOID param)
{
	CDeepVoidApp * pApp = (CDeepVoidApp *)param;

	// 把要用的全局变量引用过来 ////////////////////////////////////////////////
	const vector<cam_data> & cams = pApp->m_vCams;
	const SfM_ZZK::PointCloud & pointCloud = pApp->m_mapPointCloud;
	const SfM_ZZK::MultiTracksWithFlags & tracks = pApp->m_mapTracks; // 20220202，新数据结构
	const SfM_ZZK::PairWise_F_matches_pWrdPts & pairMatchInfos = pApp->m_mapPairwiseFMatchesWrdPts; // 20220128，<<i,j>, <<F,matches>, pWrdPts>>，两视图i和j间的所有匹配信息，包括基础矩阵F、所有匹配matches、射影重建物点坐标pWrdPts
	const CString & dirOut = pApp->m_pathDirOut; // 结果输出文件夹路径
	const std::vector<CString> & vImgNames = pApp->m_vNameImgs; // 所有图像的纯名称（不含路径和尾缀）
	const int & idxRefImg = pApp->m_idxRefImg;	// 该变量指明 SfM 和 SBA 中哪个图像坐标系代表了参考坐标系
	//////////////////////////////////////////////////////////////////////////

	CString strInfo;
	
	// 20220128，在重建图像所在目录下新建结果输出文件夹（如果事先不存在的话）///
//	int code = mkdir(dirOut); // code=0 说明成功新建；code=-1 说明文件夹已存在

	double info[5];

	// 必须所有图像都成功完成定向，都参与后续的 BA
	int m = cams.size();

	// 1. 先把所有真值读进来 ///////////////////////////////////////////////////////////////////
	vector<Matx33d> vKs_true, vRs_true/*, vRitRj_true*/;
	vector<Matx31d> vts_true;

	for (int i = 0; i < m; ++i)
	{
		Matx33d K, R;
		Matx31d t, dist;

		ReadinEPFLCamera(pApp->m_vPathImgs[i] + ".camera", K, dist, R, t);

		vKs_true.push_back(K);
		vRs_true.push_back(R);
		vts_true.push_back(t);
	}

	// 以 SfM 中确定的参考图像坐标系为基准
	{
		// 先把 R 和 t 全部转至参考图像坐标系
		const Matx33d & R_ref_t = vRs_true[idxRefImg].t();
		const Matx31d & t_ref = vts_true[idxRefImg];
		const Matx31d & C_ref = -R_ref_t*t_ref;

		for (int i = 0; i < m; ++i)
		{
			Matx33d Ri = vRs_true[i];
			Matx31d ti = vts_true[i];

			vRs_true[i] = Ri*R_ref_t;
			vts_true[i] = Ri*C_ref + ti;
		}

// 		for (int i = 0; i < m; ++i)
// 		{
// 			const Matx33d & Rit = vRs_true[i].t();
// 
// 			for (int j = i + 1; j < m; ++j)
// 			{
// 				const Matx33d & Rj = vRs_true[j];
// 
// 				vRitRj_true.push_back(Rit*Rj);
// 			}
// 		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////


	// 2. 把当前物点、像点坐标，图像外参数初值摘过来 /////////////////////////////////////////////
	vector<Point3d> vObjPts;			// n 个物点坐标
//	vector<Matx33d> vKs;				// m 幅图像的标定矩阵，应为测试图像给定的
	vector<Matx33d> vRs;				// m 幅图像的旋转矩阵初值
	vector<Matx31d> vts;				// m 幅图像的平移向量初值
	vector<Matx<double, 5, 1>> vDists;	// 输入：m个图像像差系数
	vector<int> vDistTypes;				// 输入：m个图像的像差系数类型
	vector<Point2d> vImgPts;			// l 个观测像点坐标，最多为 m*n
	vector<Matx22d> vCovInvs;			// 输入：l个所有像点坐标协方差矩阵的逆矩阵
//	vector<uchar> j_fixed;				// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
//	vector<uchar> i_fixed;				// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
//	SparseMat ptrMat;					// 输入：带一维存储索引的可视矩阵，ptrMat(i,j)存的是像点xij在xys向量中存储的位置索引，以及Aij，Bij和eij在各自向量中存储的位置索引
	vector<Matx31d> nxys;				// 输入：n个物点关联的于其参考图像系中的观测视线方向，也即去像差归一化像点坐标
	vector<int> ri_j;					// 输入：n个物点关联的参考图像的索引，一个 i 仅对应一个 j

	for (int i = 0; i < m; ++i)
	{
		const cam_data & cam = cams[i];
//		vKs.push_back(K_true);
		vRs.push_back(cam.m_R);
		vts.push_back(cam.m_t);
		vDists.push_back(cam.m_dist);
		vDistTypes.push_back(cam.dist_type);
	}

	SfM_ZZK::PointCloud pointCloudValid;

	// 先看看有多少有效的物点能参与后续 SBA
	for (auto iterObjPt = pointCloud.begin(); iterObjPt != pointCloud.end(); ++iterObjPt)
	{
		const int & ID = iterObjPt->first;	// 先取出点的全局 ID 号

		const DeepVoid::CloudPoint & objpt = iterObjPt->second;

		auto track = tracks.find(ID);	// 再把该点对应的特征轨迹 track 给取出来

		int nInliers = 0;	// 该物点当前有多少个有效观测，必须至少得有 2 个有效观测才行

		bool bFoundRef = false;	// 该物点有效观测像点中有没有指定的参考像点

		// 这个循环就是把相关的特征信息给取出来
		for (auto iterImgPt = track->second.begin(); iterImgPt != track->second.end(); ++iterImgPt)
		{
			const int & I = iterImgPt->first;					// 取出特征轨迹中每个像点的图像索引号
			const int & i = iterImgPt->second.first;			// 取出特征轨迹中每个像点于其图像中的像点索引号
			const int & bInlier = iterImgPt->second.second[0];	// 该像点当前是否被判定为内点
			const int & bRef = iterImgPt->second.second[1];		// 该像点是否为参考像点

			if (bInlier)
			{
				++nInliers;

				if (bRef)
				{
					bFoundRef = true;
				}
			}
		}

		if (nInliers < 2 || !bFoundRef) // 2 个及以上有效观测，且于其中找到指定的参考像点坐标才行
		{
			continue;
		}

		pointCloudValid.insert(make_pair(ID, objpt));
	}

	int n = pointCloudValid.size(); // 这里才是真正参与后续SBA的物点数量

	// 输入：m维向量，哪些图像的参数是固定的（j_fixed[j]=1），如果图像 j 参数固定，那么 Aij = 0 对于任何其中的观测点 i 都成立
	// 输入：n维向量，哪些空间点坐标是固定的（i_fixed[i]=1），如果点 i 坐标固定，那么 Bij = 0 对于任何观测到该点的图像 j 都成立
	vector<uchar> j_fixed(m), i_fixed(n);
	j_fixed[idxRefImg] = 1;

	// 20200607，n行m列的稀疏矩阵，用于存储各点于各图像上的可见性，其实不单单是可见性，其元素值其实为每个像点在像点坐标向量中的序号
	int sizes[] = { n, m };
	SparseMat ptrMat(2, sizes, CV_32SC1);

	// 按可视矩阵按行扫描扫得的各像点坐标
	int i_tmp = 0;
	for (auto iter_objpt = pointCloudValid.begin(); iter_objpt != pointCloudValid.end(); ++iter_objpt)
	{
		const int & trackID = iter_objpt->first;

		vObjPts.push_back(iter_objpt->second.m_pt);

		auto track = tracks.find(trackID);

		const SfM_ZZK::trackWithFlags & one_track = track->second;

		for (int j = 0; j < m; ++j)
		{
			auto iter_found_img = one_track.find(j);

			if (iter_found_img == one_track.end())
			{
				// 在该 track 中没有找到指定已完成定向的观测图像
				continue;
			}

			// 找到了指定图像
			const int & bInlier = iter_found_img->second.second[0];	// 该像点当前是否被判定为内点

			// 经过上面那一大步骤已经可以确保所有物点均有至少 2 个有效（inlier）观测像点了，且其中一个肯定为参考像点
			// 所以这里只要把 outliers 像点排除出去即可，所有 inliers 都应是该纳入 SBA 的有效观测像点，且其中定有参考像点
			if (!bInlier)
			{
				continue;
			}

			const int & bRef = iter_found_img->second.second[1];	// 该像点是否为参考像点
			const KeyPoint & feat = cams[j].m_feats.key_points[iter_found_img->second.first];

			Point2d imgpt;
			imgpt.x = feat.pt.x;
			imgpt.y = feat.pt.y;
			vImgPts.push_back(imgpt);

			ptrMat.ref<int>(i_tmp, j) = vImgPts.size() - 1;

			// 20220210，在这里检查一下是否存在重投影残差明显过大的点 ///////////////////
// 			{
// 				Matx31d X;
// 				X(0) = iter_objpt->second.m_pt.x;
// 				X(1) = iter_objpt->second.m_pt.y;
// 				X(2) = iter_objpt->second.m_pt.z;
// 				Matx31d X_C = vKs_true[j] * (vRs[j] * X + vts[j]);
// 				double x_rpj = X_C(0) / X_C(2);
// 				double y_rpj = X_C(1) / X_C(2);
// 
// 				double dx = x_rpj - imgpt.x;
// 				double dy = y_rpj - imgpt.y;
// 
// 				double d = sqrt(dx*dx + dy*dy);
// 
// 				if (d > pApp->m_threshRpjErrInlier)
// 				{
// 					double shit = 100;
// 					double shithappens = 10000;
// 				}
// 			}
			//////////////////////////////////////////////////////////////////////////

			if (bRef)
			{
				const Matx33d & K = vKs_true[j];

				Matx31d xy1;
				xy1(0) = imgpt.x;
				xy1(1) = imgpt.y;
				xy1(2) = 1;

				nxys.push_back(K.inv()*xy1);

				ri_j.push_back(j);
			}
		}

		++i_tmp;
	}

	int l = vImgPts.size(); // 所有观测像点的个数

	// covInv 存放每个观测像点的不确定度（协方差矩阵）的逆矩阵，其实也就是每个观测像点的权值矩阵
	Matx22d cov = Matx22d::eye();
	for (int k = 0; k < l; ++k)
	{
		vCovInvs.push_back(cov);
	}
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////

	double tau = 1.0E-6;
	int nMaxIter = 128;
	double eps1 = 1.0E-12;
	double eps2 = 1.0E-12;

	// 3. run SBA first ///////////////////////////////////////////////////////////////////////
	vector<Point3d> objPts_SBA = vObjPts;
	vector<Matx33d> Rs_SBA = vRs;
	vector<Matx31d> ts_SBA = vts;

	tic();

	SBA_ZZK::optim_sparse_lm_wj_tj_XiYiZi(objPts_SBA, vKs_true, Rs_SBA, ts_SBA, vDists, vDistTypes, vImgPts, vCovInvs, j_fixed, i_fixed, ptrMat, info, tau, nMaxIter, eps1, eps2);

	double t_SBA = 1000 * toc();			// 单位：毫秒
	double t_SBA_perIter = t_SBA / info[3];	// 单次迭代耗时

	strInfo.Format("SBA ends, point cloud size: %d, initial err: %lf, final err: %lf, iter: %04.0f, code: %01.0f",
		vObjPts.size(), info[0], info[1], info[3], info[4]);

	pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

	vector<double> drads_SBA, drads_DSBA;

// 	for (int j = 0; j < m; ++j)
// 	{
// 		Matx33d dR = Rs_SBA[j].t()*vRs_true[j];
// 		Matx31d rov = calib::converse_R_rotvec(dR);
// 		double drad = norm(rov);
// 		drads_SBA.push_back(drad*calib::R2D);
// 	}

	for (int i = 0; i < m; ++i)
	{
		const Matx33d & Rit = Rs_SBA[i].t();
		const Matx33d & Rit_true = vRs_true[i].t();

		for (int j = i + 1; j < m; ++j)
		{
			const Matx33d & Rj = Rs_SBA[j];
			const Matx33d & Rj_true = vRs_true[j];

			Matx33d RitRj = Rit*Rj;
			Matx33d RitRj_true = Rit_true*Rj_true;

			Matx33d dR = RitRj.t()*RitRj_true;
			Matx31d rov = calib::converse_R_rotvec(dR);
			double drad = norm(rov);
			drads_SBA.push_back(drad*calib::R2D);
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////


	// 4. then run DSBA ///////////////////////////////////////////////////////////////////////
	vector<Point3d> objPts_DSBA = vObjPts;
	vector<Matx33d> Rs_DSBA = vRs;
	vector<Matx31d> ts_DSBA = vts;

	tic();

	SBA_ZZK::optim_sparse_lm_wj_tj_di(objPts_DSBA, vKs_true, Rs_DSBA, ts_DSBA, vDists, vDistTypes, vImgPts, vCovInvs, nxys, ri_j, j_fixed, i_fixed, ptrMat, info, tau, nMaxIter, eps1, eps2);

	double t_DSBA = 1000 * toc();				// 单位：毫秒
	double t_DSBA_perIter = t_DSBA / info[3];	// 单次迭代耗时

	strInfo.Format("DbSBA ends, point cloud size: %d, initial err: %lf, final err: %lf, iter: %04.0f, code: %01.0f",
		vObjPts.size(), info[0], info[1], info[3], info[4]);

	pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

// 	for (int j = 0; j < m; ++j)
// 	{
// 		Matx33d dR = Rs_DSBA[j].t()*vRs_true[j];
// 		Matx31d rov = calib::converse_R_rotvec(dR);
// 		double drad = norm(rov);
// 		drads_DSBA.push_back(drad*calib::R2D);
// 	}

	for (int i = 0; i < m; ++i)
	{
		const Matx33d & Rit = Rs_DSBA[i].t();
		const Matx33d & Rit_true = vRs_true[i].t();

		for (int j = i + 1; j < m; ++j)
		{
			const Matx33d & Rj = Rs_DSBA[j];
			const Matx33d & Rj_true = vRs_true[j];

			Matx33d RitRj = Rit*Rj;
			Matx33d RitRj_true = Rit_true*Rj_true;

			Matx33d dR = RitRj.t()*RitRj_true;
			Matx31d rov = calib::converse_R_rotvec(dR);
			double drad = norm(rov);
			drads_DSBA.push_back(drad*calib::R2D);
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////


	// 5. 结果输出 ////////////////////////////////////////////////////////////////////////////
	{
		FILE * file = fopen(dirOut + "SBA_DbSBA_compare.txt", "w");

		if (file)
		{
			int nSample = drads_SBA.size();

			fprintf(file, "SBA	DbSBA\n");

			for (int i = 0; i < nSample; ++i)
			{
				fprintf(file, "%lf	", drads_SBA[i]);
				fprintf(file, "%lf\n", drads_DSBA[i]);
			}

			fprintf(file, "%lf	", t_SBA);
			fprintf(file, "%lf\n", t_DSBA);

			fprintf(file, "%lf	", t_SBA_perIter);
			fprintf(file, "%lf\n", t_DSBA_perIter);

// 			fprintf(file, "\n");
// 
// 			for (int i = 0; i < nSample; ++i)
// 			{
// 				fprintf(file, "%lf	", drads_DSBA[i]);
// 			}

			fclose(file);
		}
	}	
	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////


	return TRUE;
}


void CDeepVoidApp::OnTestSbaanddbsbacomparison()
{
	// TODO: Add your command handler code here
	AfxBeginThread(SBA_DbSBA_compare_realdata, this, THREAD_PRIORITY_NORMAL);
}


void CDeepVoidApp::OnUpdateTestSbaanddbsbacomparison(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_b3DReady_sparse);
}
