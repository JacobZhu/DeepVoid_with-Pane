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
	cv::Mat m_image;					// corresponding image
	cv::Mat m_imgHandled;				// image has been handled with
	// Features m_features;				// features
	Features * m_pFeatures;				// feature struct pointer to corresponding cam_data's feature struct


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
