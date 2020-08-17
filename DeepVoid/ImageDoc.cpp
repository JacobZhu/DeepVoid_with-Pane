// ImageDoc.cpp : implementation file
//

#include "stdafx.h"
#include "DeepVoid.h"
#include "ImageDoc.h"

#include "ChildFrm.h"

#include "ImageView.h"


// CImageDoc

IMPLEMENT_DYNCREATE(CImageDoc, CDocument)

CImageDoc::CImageDoc()
{
	m_pImgView = NULL;
	m_pImgOriginal = NULL;
	m_pImgProcessed = NULL;
	m_pCam = NULL;

	// parameters for sift feature extraction
	m_nfeaturesSift = 0;	// The number of best features to retain. The features are ranked by their scores (measured in SIFT algorithm as the local contrast)
	m_nOctaveLayersSift = 3;	// The number of layers in each octave. 3 is the value used in D.Lowe paper.The number of octaves is computed automatically from the image resolution.
	m_contrastThresholdSift = 0.03/*0.01*//*0.04*/;	// The contrast threshold used to filter out weak features in semi-uniform (low - contrast) regions.The larger the threshold, the less features are produced by the detector.
	m_edgeThresholdSift = 10;	// The threshold used to filter out edge-like features. Note that the its meaning is different from the contrastThreshold, i.e.the larger the edgeThreshold, the less features are filtered out(more features are retained).
	m_sigmaSift = 1.6;	// The sigma of the Gaussian applied to the input image at the octave \#0. If your image is captured with a weak camera with soft lenses, you might want to reduce the number.

	// parameters for FAST feature extraction
	m_thresholdFast = 20; // threshold on difference between intensity of the central pixel and pixels of a circle around this pixel.
	m_nonmaxSuppressionFast = true; // if true, non-maximum suppression is applied to detected corners (keypoints).
	m_typeFast = cv::FastFeatureDetector::TYPE_9_16; // one of the three neighborhoods as defined in the paper: FastFeatureDetector::TYPE_9_16, FastFeatureDetector::TYPE_7_12, FastFeatureDetector::TYPE_5_8

	m_nSfMFeatures = /*2048*/8192;
	m_nPrptFeatures = 150;
}

BOOL CImageDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CImageDoc::~CImageDoc()
{
}


BEGIN_MESSAGE_MAP(CImageDoc, CDocument)
	/*ON_COMMAND(ID_FILE_OPENFILES, &CImageDoc::OnFileOpenfiles)*/
	ON_COMMAND(ID_FEATURES_SIFT, &CImageDoc::OnFeaturesSift)
	ON_COMMAND(ID_FEATURES_SURF, &CImageDoc::OnFeaturesSurf)
END_MESSAGE_MAP()


// CImageDoc diagnostics

#ifdef _DEBUG
void CImageDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CImageDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CImageDoc serialization

void CImageDoc::ExtractPointsContinuously(BOOL bClear)
{
	if (bClear)
	{
		m_pCam->m_featsManual.clear();
		m_pImgView->Invalidate(TRUE);
	} 

	int flag = 0;

	std::vector<cv::KeyPoint> & keypts_all = m_pCam->m_featsManual.key_points;
	
	int nOld = keypts_all.size(); // 之前已有多少点

	std::vector<cv::KeyPoint> keypts;

	do
	{
		cv::Point2d pt = m_pImgView->ExtractPoint(&flag);

		if (flag != 1)
		{
			continue;
		}

		cv::KeyPoint keypt;
		keypt.pt.x = pt.x;
		keypt.pt.y = pt.y;

		keypts.push_back(keypt);
		keypts_all.push_back(keypt);

		m_pImgView->Invalidate(FALSE);
	} while (flag != -1);
		
	int nNew = keypts.size(); // 新增点数

	// 生成特征描述向量
	/*cv::Ptr<Feature2D>*/ auto f2d = cv::xfeatures2d::SIFT::create(m_nfeaturesSift, m_nOctaveLayersSift, m_contrastThresholdSift, m_edgeThresholdSift, m_sigmaSift);

	Mat imgTmp = m_pImgOriginal->clone();

	cv::Mat descrps;
//	f2d->compute(/**m_pImgOriginal*/imgTmp, keypts, descrps);

	try
	{
		f2d->compute(/**m_pImgOriginal*/imgTmp, keypts, descrps);
// 		cv::xfeatures2d::SiftDescriptorExtractor::create()
// 		cv::xfeatures2d::SiftDescriptorExtractor::compute(imgTmp, keypts, descrps);
	}
	catch (cv::Exception & e)
	{
		CString str;
		str = e.msg.c_str();
		AfxMessageBox(str);
	}

	///////////////////////////////////////////////////////////////////
	FILE * file = fopen("C:\\Users\\DeepV\\Desktop\\desp_manual.txt", "w");
	for (int i = 0; i < descrps.rows; ++i)
	{
		for (int j = 0; j < descrps.cols; ++j)
		{
			fprintf(file, "%lf	", descrps.at<float>(i, j));
		}
		fprintf(file, "\n");
	}
	fclose(file);
	//////////////////////////////////////////////////////////////////

	m_pCam->m_featsManual.type = Feature_MANUAL_SIFT; // manual keypoints + sift descriptors

	// 合入已有的手提特征点向量	
	cv::Mat & descrps_all = m_pCam->m_featsManual.descriptors;
	descrps_all.push_back(descrps);

	// 下面主要是为了将 sift 特征中重复位置但主方向不同的特征点编为统一的全局编号，并把每个特征点处的色彩值插值出来
	for (int i = 0; i < nNew; ++i)
	{
		const cv::KeyPoint & keypt = keypts[i];

		m_pCam->m_featsManual.tracks.push_back(-1);
		m_pCam->m_featsManual.idx_pt.push_back(i + nOld);

		// 20150215, zhaokunz, 把该特征点的颜色信息插值出来
		uchar r, g, b;
		Vec3b rgb;
		if (BilinearInterp(*m_pImgOriginal, keypt.pt.x, keypt.pt.y, r, g, b))
		{
			rgb[0] = b;
			rgb[1] = g;
			rgb[2] = r;
		}
		m_pCam->m_featsManual.rgbs.push_back(rgb);
	}
}

void CImageDoc::ExtractSiftFeatures()
{
	m_pCam->ExtractSiftFeatures(*m_pImgOriginal, m_nfeaturesSift, m_nOctaveLayersSift, m_contrastThresholdSift, m_edgeThresholdSift, m_sigmaSift);

	m_pImgView->Invalidate(TRUE);
}

void CImageDoc::ExtractFASTFeatures()
{
	m_pCam->ExtractFASTFeatures(*m_pImgOriginal, m_thresholdFast, m_nonmaxSuppressionFast, m_typeFast,
		m_nfeaturesSift, m_nOctaveLayersSift, m_contrastThresholdSift, m_edgeThresholdSift, m_sigmaSift);

	m_pImgView->Invalidate(TRUE);
}

void CImageDoc::DeleteAllFeatures()
{
	m_pCam->DeleteAllFeatures();

	m_pImgView->Invalidate(TRUE);
}

void CImageDoc::GenSfMFeatures()
{
	m_pCam->GenSfMFeatures(m_nSfMFeatures, m_nPrptFeatures);

	m_pImgView->m_flagShow = 2;
	m_pImgView->Invalidate(TRUE);
}

void CImageDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
#endif


// CImageDoc commands


//void CImageDoc::OnFileOpenfiles()
//{
//	// TODO: Add your command handler code here
//	/*Mat img = imread("C:\\Users\\zhaokunz\\Dropbox\\Info Uploads\\Test images for SGM\\im2.ppm");*/
//}


void CImageDoc::OnFeaturesSift()
{
	// TODO: Add your command handler code here
	/*if (m_image.empty())
	{
		return;
	}

	Mat mask;

	SIFT sift;

	sift(m_image, mask, m_pFeatures->key_points, m_pFeatures->descriptors);

	drawKeypoints(m_image, m_pFeatures->key_points, m_imgHandled);

	m_pFeatures->type = Feature_SIFT;

	m_pImgView->m_pImage = &m_imgHandled;
	m_pImgView->m_bShowHandled = TRUE;
	m_pImgView->Invalidate(TRUE);*/
}


void CImageDoc::OnFeaturesSurf()
{
	// TODO: Add your command handler code here
	/*if (m_image.empty())
	{
		return;
	}

	Mat mask;

	SURF surf;

	surf(m_image, mask, m_pFeatures->key_points, m_pFeatures->descriptors);

	drawKeypoints(m_image, m_pFeatures->key_points, m_imgHandled);

	m_pFeatures->type = Feature_SURF;

	m_pImgView->m_pImage = &m_imgHandled;
	m_pImgView->m_bShowHandled = TRUE;
	m_pImgView->Invalidate(TRUE);*/
}


void CImageDoc::OnCloseDocument()
{
	// TODO: Add your specialized code here and/or call the base class
	/*for (int i = 0; i < theApp.m_vPImgCocs.size(); ++i)
	{
		CImageDoc * pDoc = theApp.m_vPImgCocs[i];
		if (pDoc == this)
		{
			theApp.m_vPImgCocs.erase(theApp.m_vPImgCocs.begin() + i);
			break;
		}
	}*/

	auto iter_found = std::find_if(theApp.m_vPImgCocs.begin(), theApp.m_vPImgCocs.end(), [this](const CImageDoc * p) {return p == this; });

	if (iter_found != theApp.m_vPImgCocs.end())
	{
		*iter_found = NULL;
	}

	CDocument::OnCloseDocument();
}
