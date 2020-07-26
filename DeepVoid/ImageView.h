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

	BOOL m_bStartExtractPt;		//�Ƿ�ʼ�������ѡ�ĵ�����
	int m_flagPointExtracted;	// ��ЧС�ӵ���㺯�����صı�־��0 ��ʾδ��㲢һֱ�ȴ���㣬1 ��ʾ�Ѿ������㣬-1 ��ʾȡ�����

	BOOL m_bShowAll;
	BOOL m_bShowSIFT;
	BOOL m_bShowFAST;
	BOOL m_bShowManual;
	BOOL m_bShowID;
	BOOL m_bShowInfo;

	int m_nPenWidth;		// CDC pen width
	int m_penStyle;			// PS_SOLID(0); PS_DASH(1); PS_DOT(2); PS_DASHDOT(3); PS_DASHDOTDOT(4)
	int m_nBasicHalfLength;	// 1 ����ʾ������ʮ��˿�볤������ؿ��
	int m_nBasicRadius;		// 1 ����ʾ������ԲȦ�İ뾶���ؿ��
	int m_nHeightInfoFont;
	int m_nHeightInfoRect;
	int m_nWidthInfoRect;

	CRect m_rectInfo;
	CPoint m_ptPreGrab;

	cv::Point2d m_ptExtracted;	//���ÿ��ѡȡ��ͼ�������

	void SetImageScrollSize(void);
	cv::Point2d ExtractPoint(int * pFlag = NULL);

//	void DrawCrosshair(double x, double y, uchar r, uchar g, uchar b, int id,
//		BOOL bShowID = TRUE, int penStyle = PS_SOLID, int nWidth = 1, int halfLength = 5);
	// �������е� dc��CPen �Լ� Textcolor �� BkMode ����
//	void DrawCrosshair(CClientDC & dc, double x, double y, int id, BOOL bShowID = TRUE, int halfLength = 5);
	void DrawCrosshair(CDC * pDC, double x, double y, int id, BOOL bShowID = TRUE, int halfLength = 5);

//	void DrawCross(double x, double y, uchar r, uchar g, uchar b, int id,
//		BOOL bShowID = TRUE, int penStyle = PS_SOLID, int nWidth = 1, int halfLength = 5);
	// �������е� dc��CPen �Լ� Textcolor �� BkMode ����
//	void DrawCross(CClientDC & dc, double x, double y, int id, BOOL bShowID = TRUE, int halfLength = 5);
	void DrawCross(CDC * pDC, double x, double y, int id, BOOL bShowID = TRUE, int halfLength = 5);
	
//	void DrawCircle(double x, double y, uchar r, uchar g, uchar b, int id,
//		BOOL bShowID = TRUE, int penStyle = PS_SOLID, int nWidth = 1, int nRadius = 5);
	// �������е� dc��CPen �Լ� Textcolor �� BkMode ����
//	void DrawCircle(CClientDC & dc, double x, double y, int id, BOOL bShowID = TRUE, int nRadius = 5);
	void DrawCircle(CDC * pDC, double x, double y, int id, BOOL bShowID = TRUE, int nRadius = 5);

//	void DrawInfo(const CString & info, double x, double y, uchar r, uchar g, uchar b,
//		int penStyle = PS_SOLID, int nWidth = 1);
	// �������е� dc��CPen �Լ� Textcolor �� BkMode ����
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