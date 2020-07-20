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
//	Features * m_pFeatures;				// feature struct pointer to corresponding cam_data's feature struct


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
