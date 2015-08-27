
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
			((CMainFrame *)m_pMainWnd)->m_wndImgThumbnailPane.m_wndImgListCtrl.AddOneImage(strFilePath);
		}
	}

	delete [] pBuffer;
}


void CDeepVoidApp::OnFileAddimages()
{
	// TODO: Add your command handler code here
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
			((CMainFrame *)m_pMainWnd)->m_wndImgThumbnailPane.m_wndImgListCtrl.AddOneImage(strFilePath);
		}
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

	int i;

	int nCam = pApp->m_vCams.size();

	// first, extract all the features in each image
	for (i=0;i<nCam;i++)
	{
		// read corresponding image data into memory first
		char * pDir = (char *)pApp->m_pMainFrame->m_wndImgThumbnailPane.m_wndImgListCtrl.GetItemData(i);

		CString strDir;
		strDir.Format(_T("%s"), pDir);
		strDir.Trim();

		Mat img = imread(strDir.GetBuffer(), CV_LOAD_IMAGE_UNCHANGED);

		Mat mask;

		SURF sift;

		sift(img, mask, pApp->m_vCams[i].m_feats.key_points, pApp->m_vCams[i].m_feats.descriptors);
		pApp->m_vCams[i].m_feats.type = Feature_SIFT;

		CString strInfo;
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

	// 	char * pDir = (char *)m_pMainFrame->m_wndImgThumbnailPane.m_wndImgListCtrl.GetItemData(0);
	// 	CString strDir;
	// 	strDir.Format(_T("%s"), pDir);
	// 	strDir.Trim();
	// 	Mat img1 = imread(strDir.GetBuffer(), CV_LOAD_IMAGE_UNCHANGED);

	double thresh_match = 5;
	double thresh_d2epiline = 1;
	double thresh_matchConf = 0.99;
	double thresh_stdev_YZ = 0.08;
	double thresh_reproj_ro = 1;
	double thresh_pyerr = 0.001;
	double thresh_reproj_eo = 1;
	int sba_iter = 10;
	int thresh_minNum = 3;
	double thresh_ratio_inliers_EO = 0.75;
	double thresh_ang = 15;

	double opts[5];
	opts[0] = 1.0E-3;	// levmar 优化方法中要用到的参数 u 的初始尺度因子，默认为 1.0E-3
	opts[1] = 1.0E-12;	// 当目标函数对各待优化参数的最大导数小于等于该值时优化结束，默认为 1.0E-12
	opts[2] = 1.0E-12;	// 当待优化参数 2 范数的变化量小于该阈值时优化结束，默认为 1.0E-12
	opts[3] = 1.0E-12;	// 当误差矢量的 2 范数小于该阈值时优化结束，默认为 1.0E-12
	opts[4] = 0;		// 当误差矢量的 2 范数的相对变化量小于该阈值时优化结束，默认为 0

	CMatrix mRT; vector<CloudPoint> clouds;

	vector<int> status(pApp->m_vCams.size());
	for (i=0;i<pApp->m_vCams.size();i++)
	{
		status[i] = 0;
	}
	status[0] = 1;

	for (int k=1;k<pApp->m_vCams.size();k++)
	{
		bool bSuc = RelativeOrientation_RANSAC_Features_PIRO(pApp->m_vCams[0], pApp->m_vCams[k], 0, k, mRT, clouds, 
			thresh_match, thresh_d2epiline, thresh_matchConf, thresh_stdev_YZ, thresh_reproj_ro, thresh_pyerr, thresh_ang);

		if (bSuc)
		{
			for (int i=0;i<3;i++)
			{
				for (int j=0;j<3;j++)
				{
					pApp->m_vCams[0].R[i*3+j] = 0;
					pApp->m_vCams[k].R[i*3+j] = mRT(i+1,j+1);
				}
			}

			pApp->m_vCams[0].R[0]=pApp->m_vCams[0].R[4]=pApp->m_vCams[0].R[8]=1;
			pApp->m_vCams[0].t[0]=pApp->m_vCams[0].t[1]=pApp->m_vCams[0].t[2]=0;

			pApp->m_vCams[k].t[0]=mRT(1,4);
			pApp->m_vCams[k].t[1]=mRT(2,4);
			pApp->m_vCams[k].t[2]=mRT(3,4);

			CString strInfo;
			strInfo.Format("Relative orientation of image %03d finished, number of cloud points are %d", k, clouds.size());
			pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

			status[k] = 1;

			break;
		}
	}

	double info[10];

	try
	{
		ExteriorOrientation_PnP_RANSAC_All(pApp->m_vCams, status, clouds,
			thresh_match, thresh_d2epiline, thresh_matchConf, thresh_reproj_eo, thresh_ratio_inliers_EO);
	}
	catch (cv::Exception & e)
	{
		CString str;
		str = e.msg.c_str();
		AfxMessageBox(str);
	}

// 	for (int k=2;k<pApp->m_vCams.size();k++)
// 	{
// 		bool bSuc = ExteriorOrientation_PnP_RANSAC(pApp->m_vCams, k-1, k, clouds, 
// 			thresh_match, thresh_d2epiline, thresh_matchConf, thresh_reproj_eo, thresh_ratio_inliers_EO);
// 
// 		if (bSuc)
// 		{
// 			CString strInfo;
// 			strInfo.Format("Exterior orientation of image %03d finished, number of cloud points are %d", k, clouds.size());
// 			pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 
// 			pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(_T("SBA starts"));
// 			int nnn = optim_sba_levmar_XYZ_ext_rotvec(clouds, pApp->m_vCams, sba_iter, opts, info);
// 			double rrr = sqrt(info[1]/nnn);
// 			strInfo.Format("SBA ends, err: %lf, iter: %04.0f, code: %01.0f", rrr, info[5], info[6]);
// 			pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 		}
// 		else
// 		{
// 			break;
// 		}
// 	}

	FILE * file = fopen("D:\\result_linear.txt", "w");
	for (i=0;i<clouds.size();i++)
	{
		int R = 0;
		int G = 255;
		int B = 0;
		fprintf(file, "%lf;%lf;%lf;%d;%d;%d\n", clouds[i].m_pt.x, clouds[i].m_pt.y, clouds[i].m_pt.z, R, G, B);
	}
	fclose(file);

	// delete all the cloud points that can only be seen by two images
	vector<CloudPoint> clouds_valid;
	for (i=0;i<clouds.size();i++)
	{
		if (clouds[i].m_vImgInfos.size()>=thresh_minNum)
		{
			clouds_valid.push_back(clouds[i]);
		}
	}

	CString strInfo;
	strInfo.Format("number of valid cloud points are %d", clouds_valid.size());
	pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

	clouds = clouds_valid;

	pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(_T("SBA starts"));
	int nnn = optim_sba_levmar_XYZ_ext_rotvec(clouds, pApp->m_vCams, 2048, NULL, info);
	double rrr = sqrt(info[1]/nnn);
	strInfo.Format("SBA ends, err: %lf, iter: %04.0f, code: %01.0f", rrr, info[5], info[6]);
	pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
	
	file = fopen("D:\\result_SBA.txt", "w");
	for (i=0;i<clouds.size();i++)
	{
		int R = 255;
		int G = 255;
		int B = 0;
		fprintf(file, "%lf;%lf;%lf;%d;%d;%d\n", clouds[i].m_pt.x, clouds[i].m_pt.y, clouds[i].m_pt.z, R, G, B);
	}
	fclose(file);

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

		/*SIFT sift;*/
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
	opts[0] = 1.0E-3;	// levmar 优化方法中要用到的参数 u 的初始尺度因子，默认为 1.0E-3
	opts[1] = 1.0E-8;	// 当目标函数对各待优化参数的最大导数小于等于该值时优化结束，默认为 1.0E-12
	opts[2] = 1.0E-8;	// 当待优化参数 2 范数的变化量小于该阈值时优化结束，默认为 1.0E-12
	opts[3] = 1.0E-12;	// 当误差矢量的 2 范数小于该阈值时优化结束，默认为 1.0E-12
	opts[4] = 0;		// 当误差矢量的 2 范数的相对变化量小于该阈值时优化结束，默认为 0

	vector<vector<DMatch>> allMatches;
	vector<DMatch> matches;

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
// 			CalibFundamentalMat_RANSAC_Features_Original(pApp->m_vCams[i].m_feats, pApp->m_vCams[j].m_feats, matches,
// 				thresh_match, thresh_d2epiline, thresh_matchConf);

// 			CalibFundamentalMat_RANSAC_Features(pApp->m_vCams[i].m_feats, pApp->m_vCams[j].m_feats, matches,
// 				thresh_match, thresh_d2epiline, thresh_matchConf);

// 			CalibFundamentalMat_RANSAC_Features_idxImgPt(pApp->m_vCams[i].m_feats, pApp->m_vCams[j].m_feats, matches,
// 				thresh_match, thresh_d2epiline, thresh_matchConf);

			// 20150113, zhaokunz, new matching function
			Matx33d mF;
//			Get_F_Matches(pApp->m_vCams[i].m_feats,pApp->m_vCams[j].m_feats,mF,matches,thresh_d2epiline,thresh_matchConf);
			bool bSuc = Get_F_Matches_knn(pApp->m_vCams[i].m_feats,pApp->m_vCams[j].m_feats,mF,matches,false,0.65,0.5,thresh_d2epiline,thresh_matchConf,64);

			Mat disp_matches;
			drawMatches(vImages[i], pApp->m_vCams[i].m_feats.key_points, vImages[j], pApp->m_vCams[j].m_feats.key_points, matches, disp_matches);
			strInfo.Format("E:\\matches\\matches between image %03d and %03d.bmp", i, j);
			imwrite(strInfo.GetBuffer(), disp_matches);

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
			}
		}
		fprintf(fileMatches, "\n");
	}
	fclose(fileMatches);

	vector<vector<Point2i>> allTracks;
	/*FindAllMatchingTracks_with121mappingcheck(pApp->m_vCams, allMatches, allTracks);*/
	FindAllMatchingTracks(pApp->m_vCams, allMatches, allTracks);

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

	strInfo.Format("number of good tracks: %07d", allTracks.size());
	pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

	// relative orientation //////////////////////////////////////////////////////////////////////////
	CMatrix mRT; vector<CloudPoint> clouds;

	vector<int> status(nCam);
	for (i=0;i<nCam;i++)
	{
		status[i] = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	vector<Point2i> pairs;
	FindPairObservingMostCommonTracks(pApp->m_vCams,allTracks,pairs);

	CString strFile;
	int idx_refimg;

	for (k=0;k<pairs.size();k++)
	{
		i = pairs[k].x;
		j = pairs[k].y;

		bool bSuc_RO = RelativeOrientation_Features_PIRO_givenMatches(pApp->m_vCams[i], pApp->m_vCams[j], i, j, allMatches[i*nCam+j],
			mRT, clouds, thresh_reproj_ro);

		if (bSuc_RO)
		{
			for (int ii=0;ii<3;ii++)
			{
				for (int jj=0;jj<3;jj++)
				{
					pApp->m_vCams[i].R[ii*3+jj] = 0;
					pApp->m_vCams[j].R[ii*3+jj] = mRT(ii+1,jj+1);
				}
			}

			pApp->m_vCams[i].R[0]=pApp->m_vCams[i].R[4]=pApp->m_vCams[i].R[8]=1;
			pApp->m_vCams[i].t[0]=pApp->m_vCams[i].t[1]=pApp->m_vCams[i].t[2]=0;

			pApp->m_vCams[j].t[0]=mRT(1,4);
			pApp->m_vCams[j].t[1]=mRT(2,4);
			pApp->m_vCams[j].t[2]=mRT(3,4);

			strInfo.Format("Relative orientation between image %03d and %03d finished, the rank is: %d, number of cloud points are %d", i, j, k, clouds.size());
			pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

			status[i] = 1;
			status[j] = 1;

			idx_refimg = i;

			strFile.Format("point cloud after RO of images %03d and %03d.txt", i, j);

			break;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	CString strOut;
	strOut.Format("E:\\all\\");

	OutputPointCloud(strOut+strFile,pApp->m_vCams,clouds);

// 	bool bSuc_RO = false;
// 	for (i=0;i<nCam;i++)
// 	{
// 		for (j=i+1;j<nCam;j++)
// 		{
// // 			bSuc_RO = RelativeOrientation_RANSAC_Features_PIRO_givenMatches(pApp->m_vCams[i], pApp->m_vCams[j], i, j, allMatches[i*nCam+j],
// // 				mRT, clouds, thresh_stdev_YZ, thresh_reproj_ro, thresh_pyerr, thresh_ang);
// 			// 20150115, zhaokunz, new more robust RO
// 			bSuc_RO = RelativeOrientation_Features_PIRO_givenMatches(pApp->m_vCams[i], pApp->m_vCams[j], i, j, allMatches[i*nCam+j],
// 				mRT, clouds, thresh_reproj_ro);
// 
// 			if (bSuc_RO)
// 			{
// 				for (int ii=0;ii<3;ii++)
// 				{
// 					for (int jj=0;jj<3;jj++)
// 					{
// 						pApp->m_vCams[i].R[ii*3+jj] = 0;
// 						pApp->m_vCams[j].R[ii*3+jj] = mRT(ii+1,jj+1);
// 					}
// 				}
// 
// 				pApp->m_vCams[i].R[0]=pApp->m_vCams[i].R[4]=pApp->m_vCams[i].R[8]=1;
// 				pApp->m_vCams[i].t[0]=pApp->m_vCams[i].t[1]=pApp->m_vCams[i].t[2]=0;
// 
// 				pApp->m_vCams[j].t[0]=mRT(1,4);
// 				pApp->m_vCams[j].t[1]=mRT(2,4);
// 				pApp->m_vCams[j].t[2]=mRT(3,4);
// 
// 				strInfo.Format("Relative orientation between image %03d and %03d finished, number of cloud points are %d", i, j, clouds.size());
// 				pApp->m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 
// 				status[i] = 1;
// 				status[j] = 1;
// 
// 				break;
// 			}
// 		}
// 
// 		if (bSuc_RO)
// 		{
// 			break;
// 		}
// 	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	double d_mean = MeanMinDistance_3D(clouds);
	strInfo.Format("average distance between cloud points is: %lf", d_mean);
	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);

// 	int idx_max, nPts_max;
// 
// 	idx_max = FindImgObservingMostTracks_hashSet(pApp->m_vCams, clouds, status, nPts_max);
// 	strInfo.Format("maximum tracks: %05d by image %03d", nPts_max, idx_max);
// 	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 
// 	while (idx_max>=0 && nPts_max > 6)
// 	{
// //		ExteriorOrientation_PnP_RANSAC(pApp->m_vCams, idx_max, clouds, allTracks, 1, 0.5);
// //		ExteriorOrientation_PnP_RANSAC_detachOutliers(pApp->m_vCams, idx_max, clouds, allTracks, 1, 0.5);
// 		bool bSuc_EO = EO_PnP_RANSAC(pApp->m_vCams, idx_max, clouds, allTracks, 1.5, 0.0);
// 		strInfo.Format("Exterior orientation of image %03d finished, code: %d, number of cloud points are %d", idx_max, bSuc_EO, clouds.size());
// 		theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 
// 		if (!bSuc_EO) // failed
// 		{
// 			idx_max = FindImgObservingMostTracks_hashSet(pApp->m_vCams, clouds, status, nPts_max);
// 			strInfo.Format("maximum tracks: %05d by image %03d", nPts_max, idx_max);
// 			theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 
// 			continue;
// 		}
// 		
// 		status[idx_max] = 1;
// 
// 		theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(_T("SBA starts"));
// 		double info[10];
// 		int nnn = optim_sba_levmar_XYZ_ext_rotvec(clouds, pApp->m_vCams, 1024, opts, info);
// 		/*int nnn = optim_sba_levmar_XYZ_ext_rotvec_iterative(clouds, pApp->m_vCams, allTracks, 1024, 1, opts, info);*/
// 		double rrr = sqrt(info[1]/nnn);
// 		strInfo.Format("SBA ends, err: %lf, iter: %04.0f, code: %01.0f", rrr, info[5], info[6]);
// 		theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 
// 		d_mean = MeanMinDistance_3D(clouds);
// 		strInfo.Format("average distance between cloud points is: %lf", d_mean);
// 		theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 
// 		idx_max = FindImgObservingMostTracks_hashSet(pApp->m_vCams, clouds, status, nPts_max);
// 		strInfo.Format("maximum tracks: %05d by image %03d", nPts_max, idx_max);
// 		theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);
// 	}

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

	FILE * file = fopen("D:\\result_SfM_outliers_inliers.txt", "w");
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
	double info[10];

// 	int nnn = optim_sba_levmar_XYZ_ext_rotvec(clouds_no_outliers, pApp->m_vCams, idx_refimg, 1024, opts, info);
// 	double rrr = sqrt(info[1]/nnn);
// 	strInfo.Format("Final SBA ends, err: %lf, iter: %04.0f, code: %01.0f", rrr, info[5], info[6]);

	int nnn = optim_sba_levmar_XYZ_ext_rotvec_own(clouds_no_outliers, pApp->m_vCams, idx_refimg, 1024, opts, info);
	double rrr = info[1];
	strInfo.Format("Final SBA ends, err: %lf, iter: %04.0f, code: %01.0f", rrr, info[3], info[4]);

	theApp.m_pMainFrame->m_wndShowInfoPane.m_wndShowInfoListCtrl.AddOneInfo(strInfo);



	// 20150204, zhaokunz, find several matching images for each image for dense matching
// 	vector<int> matchingImgs;
// 	RankMatchingImages(pApp->m_vCams, clouds_no_outliers, 0, matchingImgs);

	// 20150206, associate each input image with several support images
	vector<vector<int>> vIdxSupports;

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
		StereoSGBM sgbm;

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
			/*sgbm(rimg1, rimg2, disp);*/
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
		fclose(fp);
	}
	
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
//	FILE * file = fopen("E:\\Test Data\\templeRing\\templeR_par.txt", "r");
//	FILE * file = fopen("E:\\Test Data\\dinoSparseRing\\dinoSR_par.txt", "r");
	FILE * file = fopen("E:\\Test Data\\dinoRing\\dinoR_par.txt", "r");
	
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
//		strDir.Format("E:\\Test Data\\templeRing\\templeR%04d.png", i+1);
//		strDir.Format("E:\\Test Data\\dinoSparseRing\\dinoSR%04d.png", i+1);
		strDir.Format("E:\\Test Data\\dinoRing\\dinoR%04d.png", i+1);

		vPaths_imgs.push_back(strDir);

		Mat img = imread(strDir.GetBuffer(), CV_LOAD_IMAGE_UNCHANGED);

		imgWidth = img.cols; imgHeight = img.rows;

		vAllImgs.push_back(img); // store every image

		// read in contour images
//		strDir.Format("E:\\Test Data\\templeSparseRing\\contours\\templeSR%04d_seg.png", i+1);
//		strDir.Format("E:\\Test Data\\templeRing\\contours\\templeR%04d_seg.png", i+1);
//		strDir.Format("E:\\Test Data\\dinoSparseRing\\contours\\dinoSR%04d_seg.png", i+1);
		strDir.Format("E:\\Test Data\\dinoRing\\contours\\dinoR%04d_seg.png", i+1);
		img = imread(strDir.GetBuffer(), CV_LOAD_IMAGE_GRAYSCALE);
		vSilhouettes.push_back(img);
	}

	fclose(file);

	// bounding box of temple sparse ring
// 	double bbox_xmin = -0.073568;	double bbox_xmax = 0.028855;
// 	double bbox_ymin = 0.021728;	double bbox_ymax = 0.181892;
// 	double bbox_zmin = -0.012445;	double bbox_zmax = 0.062736;

	// bounding box of temple ring
// 	double bbox_xmin = -0.023121;	double bbox_xmax = 0.078626;
// 	double bbox_ymin = -0.038009;	double bbox_ymax = 0.121636;
// 	double bbox_zmin = -0.091940;	double bbox_zmax = -0.017395;

	// bounding box of dino sparse ring
// 	double bbox_xmin = -0.061897;	double bbox_xmax = 0.010897;
// 	double bbox_ymin = -0.018874;	double bbox_ymax = 0.068227;
// 	double bbox_zmin = -0.057845;	double bbox_zmax = 0.015495;

	// bounding box of dino ring
	double bbox_xmin = -0.021897;	double bbox_xmax = 0.050897;
	double bbox_ymin = 0.021126;	double bbox_ymax = 0.108227;
	double bbox_zmin = -0.017845;	double bbox_zmax = 0.055495;

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
//	file = fopen("E:\\Test Data\\templeRing\\support images - without 180 roll.txt", "r");
//	file = fopen("E:\\Test Data\\dinoSparseRing\\support images.txt", "r");
	file = fopen("E:\\Test Data\\dinoRing\\support images.txt", "r");
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
	int i,j;

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
		if(matches_raw[i].distance</*max_dist*/20*min_dist)
		{
			matches_good.push_back(matches_raw[i]);
		}
	}

	// RANSAC //////////////////////////////////////////////////////////////////////////
	vector<Point2f> points0(matches_one2one.size());
	vector<Point2f> points1(matches_one2one.size());

	// initialize the points here ... */
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
	
	fundamental_matrix = findFundamentalMat(points0, points1, status, FM_RANSAC/*, 3.0, 0.99*/);
//	fundamental_matrix = findFundamentalMat(points0_t, points1_t, status, FM_RANSAC, 0.1/*, 0.99*/);

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
// 	// initialize the points here ... */
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
	imwrite("E:\\matches\\matches_final.bmp", disp_matches_final);
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

// 		imwrite("C:\\Users\\DeepVoid\\Desktop\\rimg0.jpg", rimg0);
// 		imwrite("C:\\Users\\DeepVoid\\Desktop\\rimg1.jpg", rimg1);
// 		imwrite("C:\\Users\\DeepVoid\\Desktop\\rimg0_gray.jpg", rimg0_gray);
// 		imwrite("C:\\Users\\DeepVoid\\Desktop\\rimg1_gray.jpg", rimg1_gray);

		imwrite("D:\\stereo\\rimg0.jpg", rimg0);
		imwrite("D:\\stereo\\rimg1.jpg", rimg1);
// 		imwrite("D:\\stereo\\rimg0_gray.jpg", rimg0_gray);
// 		imwrite("D:\\stereo\\rimg1_gray.jpg", rimg1_gray);

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
					SemiGlobalMatching(rimg0_gray,rimg1_gray,m_minDisparity,m_maxDisparity,m_P1_ZZKSGM,m_P2_ZZKSGM,mDI,
						isVerticalStereo,paths,m_threshConstCheck_ZZKSGM,bSubPixRefine);

//					FILE * fp = fopen("C:\\Users\\DeepVoid\\Desktop\\point cloud by SGM (ZZK).txt", "w");
					FILE * fp = fopen("D:\\stereo\\point cloud by SGM (ZZK).txt", "w");
					for(int y = 0; y < mDI.rows; y++)
					{
						for(int x = 0; x < mDI.cols; x++)
						{
							double d = mDI.at<float>(y,x);
							if (d<=0)
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
					FILE * file_pointcloud = fopen("D:\\stereo\\point cloud by Zhu method.txt", "w");

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
					imwrite("D:\\stereo\\disparity by Zhu method.bmp", mDisparityImg);
					imwrite("D:\\stereo\\normal by Zhu method.bmp", mNormImg);
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
