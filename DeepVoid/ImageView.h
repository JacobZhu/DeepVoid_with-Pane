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

	CString m_info;				// CDC display some infos

	BOOL m_bShowProcessed;		// if currently the handled image is showing

	BOOL m_bStartExtractPt;		//是否开始接收左键选的点坐标
	int m_flagPointExtracted;	// 仿效小钟的提点函数返回的标志，0 表示未提点并一直等待提点，1 表示已经完成提点，-1 表示取消提点

	int m_flagShow;			// 0: show none; 1: show all; 2: show only selected
	BOOL m_bShowBlob;
	BOOL m_bShowCorner;
	BOOL m_bShowManual;
	BOOL m_bShowID;
	BOOL m_bShowInfo;
	BOOL m_bShowTrackID;	// 显示的是图像中的局部 ID 号，还是全局的 feature track ID 号
	BOOL m_bShowRadiusAngle;// 是否显示特征尺度和方向

	int m_nPenWidth;		// CDC pen width
	int m_penStyle;			// PS_SOLID(0); PS_DASH(1); PS_DOT(2); PS_DASHDOT(3); PS_DASHDOTDOT(4)
	int m_nBasicHalfLength;	// 1 倍显示倍率下十字丝半长轴的像素跨度
	int m_nBasicRadius;		// 1 倍显示倍率下圆圈的半径像素跨度
	int m_nHeightInfoFont;
	int m_nHeightInfoRect;
	int m_nWidthInfoRect;

// 	int m_nSiftElected;		// 被选中参与 SfM 的 sift 特征点个数，sift 特征肯定是已经排过序的
// 	int m_nFastElected;		// 被选中参与 SfM 的 fast 特征点个数，fast 特征肯定是已经排过序的
// 	int m_nManualElected;	// 被选中参与 SfM 的手提点个数，一般都是全部手提点都参加 SfM

	CRect m_rectInfo;
	CPoint m_ptPreGrab;

	cv::Point2d m_ptExtracted;	//存放每次选取的图像点坐标

	void SetImageScrollSize(void);
	cv::Point2d ExtractPoint(int * pFlag = NULL);

//	void DrawCrosshair(double x, double y, uchar r, uchar g, uchar b, int id,
//		BOOL bShowID = TRUE, int penStyle = PS_SOLID, int nWidth = 1, int halfLength = 5);
	// 利用已有的 dc、CPen 以及 Textcolor 和 BkMode 设置
//	void DrawCrosshair(CClientDC & dc, double x, double y, int id, BOOL bShowID = TRUE, int halfLength = 5);
	void DrawCrosshair(CDC * pDC, double x, double y, int id, BOOL bShowID = TRUE, int halfLength = 5);

//	void DrawCross(double x, double y, uchar r, uchar g, uchar b, int id,
//		BOOL bShowID = TRUE, int penStyle = PS_SOLID, int nWidth = 1, int halfLength = 5);
	// 利用已有的 dc、CPen 以及 Textcolor 和 BkMode 设置
//	void DrawCross(CClientDC & dc, double x, double y, int id, BOOL bShowID = TRUE, int halfLength = 5);
	void DrawCross(CDC * pDC, double x, double y, int id, BOOL bShowID = TRUE, int halfLength = 5);
	
//	void DrawCircle(double x, double y, uchar r, uchar g, uchar b, int id,
//		BOOL bShowID = TRUE, int penStyle = PS_SOLID, int nWidth = 1, int nRadius = 5);
	// 利用已有的 dc、CPen 以及 Textcolor 和 BkMode 设置
//	void DrawCircle(CClientDC & dc, double x, double y, int id, BOOL bShowID = TRUE, int nRadius = 5);
	void DrawCircle(CDC * pDC, double x, double y, int id, BOOL bShowID = TRUE, int nRadius = 5);

	void DrawFeature(CDC * pDC, double x, double y, double radius, double angle, int id, BOOL bShowID = TRUE);

//	void DrawInfo(const CString & info, double x, double y, uchar r, uchar g, uchar b,
//		int penStyle = PS_SOLID, int nWidth = 1);
	// 利用已有的 dc、CPen 以及 Textcolor 和 BkMode 设置
//	void DrawInfo(CClientDC & dc, const CString & info, double x, double y);
//	void DrawInfo(CClientDC & dc);
	void DrawInfo(CDC * pDC, CRect & rect);


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
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
};