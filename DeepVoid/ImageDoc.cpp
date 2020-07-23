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

void CImageDoc::ExtractPointsContinuously()
{
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
	cv::Ptr<Feature2D> f2d = cv::xfeatures2d::SIFT::create(m_nfeaturesSift, m_nOctaveLayersSift, m_contrastThresholdSift, m_edgeThresholdSift, m_sigmaSift);
	
	cv::Mat descrps;
	f2d->compute(*m_pImgOriginal, keypts, descrps);

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
	Features & feats = m_pCam->m_featsSIFT;
	feats.clear(); // 先把之前的清掉

	cv::Ptr<Feature2D> f2d = cv::xfeatures2d::SIFT::create(m_nfeaturesSift, m_nOctaveLayersSift, m_contrastThresholdSift, m_edgeThresholdSift, m_sigmaSift);

	// 先提取 sift 特征点
	f2d->detect(*m_pImgOriginal, feats.key_points);

	// 按特征 size 从大到小对 sift 特征点进行排序
	sort(feats.key_points.begin(), feats.key_points.end(), [](const KeyPoint & a, const KeyPoint & b) {return a.size > b.size; });

	m_pImgView->Invalidate(FALSE);

	// 生成特征描述向量
	f2d->compute(*m_pImgOriginal, feats.key_points, feats.descriptors);

	feats.type = Feature_SIFT;

	// 下面主要是为了将 sift 特征中重复位置但主方向不同的特征点编为统一的全局编号，并把每个特征点处的色彩值插值出来
	int n = feats.key_points.size();
	KeyPoint kpt_pre;
	kpt_pre.pt.x = -1000;	kpt_pre.pt.y = -1000;
	kpt_pre.size = -1000;
	int idx_imgPt;
	for (int i = 0; i < n; ++i)
	{
		feats.tracks.push_back(-1);

		KeyPoint kpt_cur = feats.key_points[i];
		if (fabs(kpt_cur.pt.x - kpt_pre.pt.x) < 1.0E-12 && fabs(kpt_cur.pt.y - kpt_pre.pt.y) < 1.0E-12
			&& fabs(kpt_cur.size - kpt_pre.size) < 1.0E-12)
		{
			// indicating that current keypoint is identical to the previous keypoint
			feats.idx_pt.push_back(idx_imgPt);
		}
		else
		{
			feats.idx_pt.push_back(i);
			idx_imgPt = i;
		}

		kpt_pre.pt.x = kpt_cur.pt.x;
		kpt_pre.pt.y = kpt_cur.pt.y;
		kpt_pre.size = kpt_cur.size;

		// 20150215, zhaokunz, 把该特征点的颜色信息插值出来
		uchar r, g, b;
		Vec3b rgb;
		if (BilinearInterp(*m_pImgOriginal, kpt_cur.pt.x, kpt_cur.pt.y, r, g, b))
		{
			rgb[0] = b;
			rgb[1] = g;
			rgb[2] = r;
		}
		feats.rgbs.push_back(rgb);
	}
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
