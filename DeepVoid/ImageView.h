#pragma once



// CImageView view

class CImageDoc;
class CImageView : public CScrollView
{
	DECLARE_DYNCREATE(CImageView)

public:
	CImageView();           // protected constructor used by dynamic creation
	virtual ~CImageView();

public:
	CImageDoc * m_pMVSDoc;
	cv::Mat * m_pImage;
	int m_idxImgDisplayScale;
	BOOL m_bDragImg;

	BOOL m_bShowProcessed;		// if currently the handled image is showing

	BOOL m_bStartExtractPt;		//是否开始接收左键选的点坐标
	int m_flagPointExtracted;	// 仿效小钟的提点函数返回的标志，0 表示未提点并一直等待提点，1 表示已经完成提点，-1 表示取消提点

	BOOL m_bShowAll;
	BOOL m_bShowSIFT;
	BOOL m_bShowFAST;
	BOOL m_bShowManual;
	BOOL m_bShowID;

	cv::Point2d m_ptExtracted;	//存放每次选取的图像点坐标

	void SetImageScrollSize(void);
	cv::Point2d ExtractPoint(int * pFlag = NULL);

	void DrawCross(double x, double y, uchar r, uchar g, uchar b, int id,
		BOOL bShowID = TRUE, int penStyle = PS_SOLID, int nWidth = 1, int halfSize = 5);
	void DrawCircle(double x, double y, uchar r, uchar g, uchar b, int id,
		BOOL bShowID = TRUE, int penStyle = PS_SOLID, int nWidth = 1, int nRadius = 5);


public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};