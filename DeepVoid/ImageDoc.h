#pragma once

// CImageDoc document

class CImageView;
class CImageDoc : public CDocument
{
	DECLARE_DYNCREATE(CImageDoc)

public:
	CImageDoc();
	virtual ~CImageDoc();

	CImageView * m_pImgView;			// related view class
	cv::Mat * m_pImgOriginal;			// pointer to original image
	cv::Mat * m_pImgProcessed;			// pointer to processed image
	DeepVoid::cam_data * m_pCam;		// pointer to cam_data structure
	
	// parameters for sift feature extraction
	int m_nfeaturesSift;
	int m_nOctaveLayersSift;
	double m_contrastThresholdSift;
	double m_edgeThresholdSift;
	double m_sigmaSift;

	// parameters for FAST feature extraction
	int m_thresholdFast;
	bool m_nonmaxSuppressionFast;
	int m_typeFast;

	void ExtractPointsContinuously(BOOL bClear = FALSE);	// continuously extract image points manually until 'ESC' being pressed
	void ExtractSiftFeatures();			// automatically extract sift features
	void ExtractFASTFeatures();			// automatically extract sift features
	void DeleteAllFeatures();			// it does what its name suggests


#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnFileOpenfiles();
	afx_msg void OnFeaturesSift();
	afx_msg void OnFeaturesSurf();
	virtual void OnCloseDocument();
};
