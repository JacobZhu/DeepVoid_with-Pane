// ImageView.cpp : implementation file
//

#include "stdafx.h"
#include "DeepVoid.h"
#include "ImageView.h"

#include "ImageDoc.h"

// CImageView

IMPLEMENT_DYNCREATE(CImageView, CScrollView)

CImageView::CImageView()
{
	m_pMVSDoc = NULL;
	m_pImage = NULL;

	m_idxImgDisplayScale = FITSCALENUM;

	m_bDragImg = FALSE;
	m_bShowProcessed = FALSE;

	m_bStartExtractPt = FALSE;

	m_flagShow = 1;
	m_bShowBlob = TRUE;
	m_bShowCorner = TRUE;
	m_bShowManual = TRUE;
	m_bShowID = FALSE;
	m_bShowInfo = TRUE;
	m_bShowTrackID = FALSE;
	m_bShowRadiusAngle = FALSE;

	m_nPenWidth = 1;		// CDC pen width
	m_penStyle = PS_SOLID;	// PS_SOLID(0); PS_DASH(1); PS_DOT(2); PS_DASHDOT(3); PS_DASHDOTDOT(4)
	m_nBasicHalfLength = 2;	// 1 ����ʾ������ʮ��˿�볤������ؿ��
	m_nBasicRadius = 2;		// 1 ����ʾ������ԲȦ�İ뾶���ؿ��
	m_nHeightInfoFont = 20;
	m_nHeightInfoRect = 150; // ����߶�20���ò���120��ζ�������ʾ 6 ����
	m_nWidthInfoRect = 280;

// 	m_nSiftElected = 0;
// 	m_nFastElected = 0;
// 	m_nManualElected = 0;
}

CImageView::~CImageView()
{
}


BEGIN_MESSAGE_MAP(CImageView, CScrollView)
	ON_WM_CREATE()
	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
END_MESSAGE_MAP()


// CImageView drawing

void CImageView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CImageView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	float scale = ImageDisplayScales[m_idxImgDisplayScale];

	// 1�����Ȱ�ͼ����ʾ�������»����˰Ѻ������ۺ���Ϣ�Լ�ͼ�����������ǵ���
	if (m_pImage && !m_pImage->empty())
	{
		DisplayImage(pDC, *m_pImage, 0, 0, m_pImage->cols * scale, m_pImage->rows * scale);
	}
	
// 	CClientDC dc(this);
// 	OnPrepareDC(&dc);
// 	dc.SetBkMode(TRANSPARENT);
	pDC->SetBkMode(TRANSPARENT);

	// 2���ٰ��ۺ���Ϣ��ʾ����
	if (m_bShowInfo) // ��ʾ info
	{
		CFont font;
		font.CreateFont(
			m_nHeightInfoFont,
			0,
			0,
			0,
			FW_HEAVY,
			FALSE,
			FALSE,
			0,
			ANSI_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_SWISS,
			_T("Arial Black"));

		CFont * pOldFont = (CFont *)pDC->SelectObject(&font);

//		pDC->SetTextColor(RGB(218, 165, 32)); // goldenrod, Hex code: #DAA520
		pDC->SetTextColor(RGB(255, 215, 0)); // gold, Hex code: #FFD700
//		pDC->SetTextColor(RGB(255, 165, 0)); // orange, Hex code: #FFA500
//		pDC->SetTextColor(RGB(192, 192, 192)); // silver, Hex code: #C0C0C0
//		pDC->SetTextColor(RGB(255, 255, 240)); // ivory, Hex code: #FFFFF0
//		pDC->SetTextColor(RGB(240, 128, 128)); // lightcoral, Hex code: #F08080
//		pDC->SetTextColor(RGB(135, 206, 235)); // skyblue, Hex code: #87CEEB

		CPoint scrollPos = GetScrollPosition();

//		CRect rectInfo;
		m_rectInfo.top = scrollPos.y;
		m_rectInfo.left = scrollPos.x;
		m_rectInfo.bottom = m_rectInfo.top + m_nHeightInfoRect;
		m_rectInfo.right = m_rectInfo.left + m_nWidthInfoRect;

		DrawInfo(pDC, m_rectInfo);

		pDC->SelectObject(pOldFont);

		font.DeleteObject();
	}
	
	// 3���ٰ�����ͼ�񼸺�����������
	if (m_pImage && !m_pImage->empty() && m_flagShow)
	{
		if (m_bShowBlob) // ��ʾ����������
		{
			int n = m_pMVSDoc->m_pCam->m_featsBlob.key_points.size();

			if (m_flagShow == 2) // ֻ��ʾ��ѡ�μ� SfM �� sift ������
			{
				n = m_pMVSDoc->m_pCam->m_nSiftElected;
			}			

			CPen newPen(m_penStyle, m_nPenWidth, RGB(0, 255, 0));
			CPen * pOldPen = (CPen *)pDC->SelectObject(&newPen);

			pDC->SetTextColor(RGB(0, 255, 0));

			for (int i = 0; i < n; ++i)
			{
				const cv::KeyPoint & keypt = m_pMVSDoc->m_pCam->m_featsBlob.key_points[i];

				double x = keypt.pt.x*scale;
				double y = keypt.pt.y*scale;
				// size ��������ֱ����diameter����ԭ������Ӧ�ü� 1 �ٳ��� 2 �ȽϺ��ʣ����Ƿ��� sift �����в���size��1.*�ģ��Һܶ�Ҳ����ż�������Ի���ֱ�ӳ��� 2 ���õ㡣
//				double radius = (keypt.size - 1)*0.5*scale;
				double radius = keypt.size*0.5*scale;

				if (m_bShowTrackID)
				{
					int trackID = m_pMVSDoc->m_pCam->m_featsBlob.tracks[i];

					if (trackID < 0)
					{
						continue;
					}

					//DrawCircle(pDC, x, y, trackID, m_bShowID, m_nBasicRadius*scale);
					if (!m_bShowRadiusAngle || radius <= 0) // �������û�г߶���Ϣ�Ǿ�ֻ����ʮ��˿
					{
						DrawCrosshair(pDC, x, y, trackID, m_bShowID, m_nBasicHalfLength*scale);
					} 
					else
					{
						DrawFeature(pDC, x, y, radius, keypt.angle, trackID, m_bShowID);
					}					
				}
				else
				{
					//DrawCircle(pDC, x, y, i, m_bShowID, m_nBasicRadius*scale);
					if (!m_bShowRadiusAngle || radius <= 0)
					{
						DrawCrosshair(pDC, x, y, i, m_bShowID, m_nBasicHalfLength*scale);
					} 
					else
					{
						DrawFeature(pDC, x, y, radius, keypt.angle, i, m_bShowID);
					}					
				}				
			}

			pDC->SelectObject(pOldPen);

			newPen.DeleteObject();
		}

		if (m_bShowCorner) // ��ʾ�ǵ�����
		{
			int n = m_pMVSDoc->m_pCam->m_featsCorner.key_points.size();

			if (m_flagShow == 2) // ֻ��ʾ��ѡ�μ� SfM �� FAST ������
			{
				n = m_pMVSDoc->m_pCam->m_nFastElected;
			}

			CPen newPen(m_penStyle, m_nPenWidth, RGB(0, 255, 255));
			CPen * pOldPen = (CPen *)pDC->SelectObject(&newPen);

			pDC->SetTextColor(RGB(0, 255, 255));

			for (int i = 0; i < n; ++i)
			{
				const cv::KeyPoint & keypt = m_pMVSDoc->m_pCam->m_featsCorner.key_points[i];

				double x = keypt.pt.x*scale;
				double y = keypt.pt.y*scale;
				double radius = keypt.size*0.5*scale;

				if (m_bShowTrackID)
				{
					int trackID = m_pMVSDoc->m_pCam->m_featsCorner.tracks[i];

					if (trackID < 0)
					{
						continue;
					}

					//DrawCross(pDC, x, y, trackID, m_bShowID, m_nBasicHalfLength*sqrt2inv*scale);
					if (!m_bShowRadiusAngle || radius <= 0)
					{
						DrawCrosshair(pDC, x, y, trackID, m_bShowID, m_nBasicHalfLength*scale);
					} 
					else
					{
						DrawFeature(pDC, x, y, radius, keypt.angle, trackID, m_bShowID);
					}					
				}
				else
				{
					//DrawCross(pDC, x, y, i, m_bShowID, m_nBasicHalfLength*sqrt2inv*scale);
					if (!m_bShowRadiusAngle || radius <= 0)
					{
						DrawCrosshair(pDC, x, y, i, m_bShowID, m_nBasicHalfLength*scale);
					} 
					else
					{
						DrawFeature(pDC, x, y, radius, keypt.angle, i, m_bShowID);
					}					
				}				
			}

			pDC->SelectObject(pOldPen);

			newPen.DeleteObject();
		}

		if (m_bShowManual) // ��ʾ�����
		{
			int n = m_pMVSDoc->m_pCam->m_featsManual.key_points.size();

			if (m_flagShow == 2) // ֻ��ʾ��ѡ�μ� SfM �������
			{
				n = m_pMVSDoc->m_pCam->m_nManualElected;
			}

			CPen newPen(m_penStyle, m_nPenWidth, RGB(255, 255, 0));
			CPen * pOldPen = (CPen *)pDC->SelectObject(&newPen);

			pDC->SetTextColor(RGB(255, 255, 0));

			for (int i = 0; i < n; ++i)
			{
				const cv::KeyPoint & keypt = m_pMVSDoc->m_pCam->m_featsManual.key_points[i];

				double x = keypt.pt.x*scale;
				double y = keypt.pt.y*scale;
				double radius = keypt.size*0.5*scale;

				if (m_bShowTrackID)
				{
					// 20200901������������Ϊkey_points��������ˢ��֮ǰ�ͳ����ˣ�����tracks������ȴ���ܻ��ǿյ���
					// ��Ȼ�ˣ��������ֻ�������ʱ���п��ܳ��֣���Ϊ�Զ�����ŵ���߽ǵ�ʱ�����������������ˢ�µ�
					if (i >= m_pMVSDoc->m_pCam->m_featsManual.tracks.size())
					{
						break;
					}

					int trackID = m_pMVSDoc->m_pCam->m_featsManual.tracks[i];

					if (trackID < 0)
					{
						continue;
					}

					if (!m_bShowRadiusAngle || radius <= 0)
					{
						DrawCrosshair(pDC, x, y, trackID, m_bShowID, m_nBasicHalfLength*scale);
					} 
					else
					{
						DrawFeature(pDC, x, y, radius, keypt.angle, trackID, m_bShowID);
					}					
				}
				else
				{
					if (!m_bShowRadiusAngle || radius <= 0)
					{
						DrawCrosshair(pDC, x, y, i, m_bShowID, m_nBasicHalfLength*scale);
					} 
					else
					{
						DrawFeature(pDC, x, y, radius, keypt.angle, i, m_bShowID);
					}					
				}				
			}

			pDC->SelectObject(pOldPen);

			newPen.DeleteObject();
		}
	}
}


// CImageView diagnostics

#ifdef _DEBUG
void CImageView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CImageView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CImageView message handlers


int CImageView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_pMVSDoc = (CImageDoc *)GetDocument();
	m_pMVSDoc->m_pImgView = this;

	return 0;
}

//void CImageView::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
//{
//	// This feature requires Windows Vista or greater.
//	// The symbol _WIN32_WINNT must be >= 0x0600.
//	// TODO: Add your message handler code here and/or call default
//	
//	// CScrollView::OnMouseHWheel(nFlags, zDelta, pt);
//}

void CImageView::SetImageScrollSize(void)
{
	CSize scrollSize;

	scrollSize.cx = m_pImage->cols * ImageDisplayScales[m_idxImgDisplayScale];
	scrollSize.cy = m_pImage->rows * ImageDisplayScales[m_idxImgDisplayScale];

	SetScrollSizes(MM_TEXT, scrollSize);
}

cv::Point2d CImageView::ExtractPoint(int * pFlag)
{
	m_flagPointExtracted = 0;
	m_bStartExtractPt = TRUE;
	SetCursor(hCursorCross);

	MSG msg;
	do
	{
		GetMessage(&msg,NULL,0, 0);
		if(msg.message == WM_COMMAND)
		{
// 			if(msg.wParam == ID_PANDU_STOP)
// 			{
// 				m_flagPointSelected = -1;
// 			}
		}
		else
		{
			if(msg.message == WM_KEYDOWN)
			{
				if (msg.wParam == VK_ESCAPE)
				{
					m_flagPointExtracted = -1;	
				} 
			
				SendMessage(msg.message,msg.wParam,msg.lParam);
			}
			else
			{
				DispatchMessage(&msg);
			}
		}
	} while(m_flagPointExtracted == 0);

	m_bStartExtractPt = FALSE;

	if (pFlag)
	{
		(*pFlag) = m_flagPointExtracted;
	}

	SetCursor(hCursorArrow);

	return m_ptExtracted;
}

//void CImageView::DrawCrosshair(double x, double y, uchar r, uchar g, uchar b, int id,
//	BOOL bShowID, int penStyle /*= PS_SOLID*/, int nWidth /*= 1*/, int halfLength /*= 5*/)
//{
//	CClientDC dc(this);
//	OnPrepareDC(&dc);
//	CPen newPen(penStyle, nWidth, RGB(r, g, b));
//	
//	CPen * pOldPen = (CPen *)dc.SelectObject(&newPen);
//
//	int ptX = int(x + 0.5);
//	int ptY = int(y + 0.5);
//
//	dc.MoveTo(ptX - halfLength, ptY);
//	dc.LineTo(ptX + halfLength, ptY);
//	dc.MoveTo(ptX, ptY - halfLength);
//	dc.LineTo(ptX, ptY + halfLength);
//
//	if (bShowID)
//	{
//		CString strTmp;
//		strTmp.Format("%d", id);
//
//		int x_offset = int(0.6*halfLength + 0.5);
//		int y_offset = int(1 * halfLength + 0.5);
//
//		dc.SetTextColor(RGB(r, g, b));
//		dc.SetBkMode(TRANSPARENT);
//		dc.TextOut(ptX + x_offset, ptY - y_offset, strTmp);
//	}
//
//	dc.SelectObject(pOldPen);
//}

//void CImageView::DrawCrosshair(CClientDC & dc, double x, double y, int id, BOOL bShowID, int halfLength)
//{
//	int ptX = int(x + 0.5);
//	int ptY = int(y + 0.5);
//
//	dc.MoveTo(ptX - halfLength, ptY);
//	dc.LineTo(ptX + halfLength, ptY);
//	dc.MoveTo(ptX, ptY - halfLength);
//	dc.LineTo(ptX, ptY + halfLength);
//
//	if (bShowID)
//	{
//		CString strTmp;
//		strTmp.Format("%d", id);
//
//		int x_offset = int(0.6*halfLength + 0.5);
//		int y_offset = int(1 * halfLength + 0.5);
//
//		dc.TextOut(ptX + x_offset, ptY - y_offset, strTmp);
//	}
//}

void CImageView::DrawCrosshair(CDC * pDC, double x, double y, int id, BOOL bShowID, int halfLength)
{
	int ptX = int(x + 0.5);
	int ptY = int(y + 0.5);

	pDC->MoveTo(ptX - halfLength, ptY);
	pDC->LineTo(ptX + halfLength, ptY);
	pDC->MoveTo(ptX, ptY - halfLength);
	pDC->LineTo(ptX, ptY + halfLength);

	if (bShowID)
	{
		CString strTmp;
		strTmp.Format("%d", id);

		int x_offset = int(0.6*halfLength + 0.5);
		int y_offset = int(1 * halfLength + 0.5);

		pDC->TextOut(ptX + x_offset, ptY - y_offset, strTmp);
	}
}

//void CImageView::DrawCross(double x, double y, uchar r, uchar g, uchar b, int id, BOOL bShowID, int penStyle, int nWidth, int halfLength)
//{
//	CClientDC dc(this);
//	OnPrepareDC(&dc);
//	CPen newPen(penStyle, nWidth, RGB(r, g, b));
//
//	CPen * pOldPen = (CPen *)dc.SelectObject(&newPen);
//
//	int ptX = int(x + 0.5);
//	int ptY = int(y + 0.5);
//
//	dc.MoveTo(ptX - halfLength, ptY - halfLength);
//	dc.LineTo(ptX + halfLength, ptY + halfLength);
//	dc.MoveTo(ptX + halfLength, ptY - halfLength);
//	dc.LineTo(ptX - halfLength, ptY + halfLength);
//
//	if (bShowID)
//	{
//		CString strTmp;
//		strTmp.Format("%d", id);
//
//		int x_offset = int(0.4*halfLength + 0.5);
//		int y_offset = int(1.3 * halfLength + 0.5);
//
//		dc.SetTextColor(RGB(r, g, b));
//		dc.SetBkMode(TRANSPARENT);
//		dc.TextOut(ptX + x_offset, ptY - y_offset, strTmp);
//	}
//
//	dc.SelectObject(pOldPen);
//}

//void CImageView::DrawCross(CClientDC & dc, double x, double y, int id, BOOL bShowID, int halfLength)
//{
//	int ptX = int(x + 0.5);
//	int ptY = int(y + 0.5);
//
//	dc.MoveTo(ptX - halfLength, ptY - halfLength);
//	dc.LineTo(ptX + halfLength, ptY + halfLength);
//	dc.MoveTo(ptX + halfLength, ptY - halfLength);
//	dc.LineTo(ptX - halfLength, ptY + halfLength);
//
//	if (bShowID)
//	{
//		CString strTmp;
//		strTmp.Format("%d", id);
//
//		int x_offset = int(0.4*halfLength + 0.5);
//		int y_offset = int(1.3 * halfLength + 0.5);
//
//		dc.TextOut(ptX + x_offset, ptY - y_offset, strTmp);
//	}
//}

void CImageView::DrawCross(CDC * pDC, double x, double y, int id, BOOL bShowID, int halfLength)
{
	int ptX = int(x + 0.5);
	int ptY = int(y + 0.5);

	pDC->MoveTo(ptX - halfLength, ptY - halfLength);
	pDC->LineTo(ptX + halfLength, ptY + halfLength);
	pDC->MoveTo(ptX + halfLength, ptY - halfLength);
	pDC->LineTo(ptX - halfLength, ptY + halfLength);

	if (bShowID)
	{
		CString strTmp;
		strTmp.Format("%d", id);

		int x_offset = int(0.4*halfLength + 0.5);
		int y_offset = int(1.3 * halfLength + 0.5);

		pDC->TextOut(ptX + x_offset, ptY - y_offset, strTmp);
	}
}

//void CImageView::DrawCircle(double x, double y, uchar r, uchar g, uchar b, int id, BOOL bShowID, int penStyle, int nWidth, int nRadius)
//{
//	CClientDC dc(this);
//	OnPrepareDC(&dc);
//	CPen newPen(penStyle, nWidth, RGB(r, g, b));
//	
//	CPen * pOldPen = (CPen *)dc.SelectObject(&newPen);
//
//	int ptX = int(x + 0.5);
//	int ptY = int(y + 0.5);
//
//	dc.MoveTo(ptX + nRadius, ptY);
//	dc.AngleArc(ptX, ptY, nRadius, 0, 360);
//
//	if (bShowID)
//	{
//		CString strTmp;
//		strTmp.Format("%d", id);
//
//		int x_offset = int(0.6*nRadius + 0.5);
//		int y_offset = int(1 * nRadius + 0.5);
//
//		dc.SetTextColor(RGB(r, g, b));
//		dc.SetBkMode(TRANSPARENT);
//		dc.TextOut(ptX + x_offset, ptY - y_offset, strTmp);
//	}
//
//	dc.SelectObject(pOldPen);
//}

//void CImageView::DrawCircle(CClientDC & dc, double x, double y, int id, BOOL bShowID, int nRadius)
//{
//	int ptX = int(x + 0.5);
//	int ptY = int(y + 0.5);
//
//	dc.MoveTo(ptX + nRadius, ptY);
//	dc.AngleArc(ptX, ptY, nRadius, 0, 360);
//
//	if (bShowID)
//	{
//		CString strTmp;
//		strTmp.Format("%d", id);
//
//		int x_offset = int(0.6*nRadius + 0.5);
//		int y_offset = int(1 * nRadius + 0.5);
//
//		dc.TextOut(ptX + x_offset, ptY - y_offset, strTmp);
//	}
//}

void CImageView::DrawCircle(CDC * pDC, double x, double y, int id, BOOL bShowID, int nRadius)
{
	int ptX = int(x + 0.5);
	int ptY = int(y + 0.5);

	pDC->MoveTo(ptX + nRadius, ptY);
	pDC->AngleArc(ptX, ptY, nRadius, 0, 360);

	if (bShowID)
	{
		CString strTmp;
		strTmp.Format("%d", id);

		int x_offset = int(0.6*nRadius + 0.5);
		int y_offset = int(1 * nRadius + 0.5);

		pDC->TextOut(ptX + x_offset, ptY - y_offset, strTmp);
	}
}

void CImageView::DrawFeature(CDC * pDC, double x, double y, double radius, double angle, int id, BOOL bShowID /*= TRUE*/)
{
	int ptX = int(x + 0.5);
	int ptY = int(y + 0.5);

	int nr = int(radius + 0.5);

	// 1. �Ȼ�Բ //////////////////////////////////////////////////////////////
	pDC->MoveTo(ptX + nr, ptY);
	pDC->AngleArc(ptX, ptY, nr, 0, 360);
	//////////////////////////////////////////////////////////////////////////

	// 2. �ٻ��������� ////////////////////////////////////////////////////////
	// �ȸ�����������Ƕ����ɶ�ά��ת����
	if (angle >= 0 && angle <= 360) // �Ƕȷ�Χ����ʱ�Ż�
	{
		cv::Matx22d R = DeepVoid::GenR2D_Angle(angle);
		cv::Matx21d X;
		X(0) = nr;
		cv::Matx21d X0 = R*X;

		pDC->MoveTo(ptX, ptY);
		pDC->LineTo(int(ptX + X0(0) + 0.5), int(ptY + X0(1) + 0.5));
	}	
	//////////////////////////////////////////////////////////////////////////

	// 3. �����ʾ��� ////////////////////////////////////////////////////////
	if (bShowID)
	{
		CString strTmp;
		strTmp.Format("%d", id);

		int x_offset = int(0.6*nr + 0.5);
		int y_offset = int(1 * nr + 0.5);

		pDC->TextOut(ptX + x_offset, ptY - y_offset, strTmp);
	}
	//////////////////////////////////////////////////////////////////////////
}

//void CImageView::DrawInfo(const CString & info, double x, double y, uchar r, uchar g, uchar b, int penStyle, int nWidth)
//{
//	CClientDC dc(this);
//	OnPrepareDC(&dc);
//	CPen newPen(penStyle, nWidth, RGB(r, g, b));
//
//	CPen * pOldPen = (CPen *)dc.SelectObject(&newPen);
//
//	int ptX = int(x + 0.5);
//	int ptY = int(y + 0.5);
//
//	dc.SetTextColor(RGB(r, g, b));
//	dc.SetBkMode(TRANSPARENT);
//	dc.TextOut(ptX, ptY, info);
//
//	dc.SelectObject(pOldPen);
//}

//void CImageView::DrawInfo(CClientDC & dc, const CString & info, double x, double y)
//{
//	int ptX = int(x + 0.5);
//	int ptY = int(y + 0.5);
//
//	dc.TextOut(ptX, ptY, info);
//}

//void CImageView::DrawInfo(CClientDC & dc)
//{
//	CString strAll, str;
//
//	if (m_bShowProcessed)
//	{
//		str.Format("��SPACE��Image shown: Processed\n");
//		strAll += str;
//	}
//	else
//	{
//		strAll += "��SPACE��Image shown: Original\n";
//	}
//
//	if (m_bShowAll)
//	{
//		strAll += "��Z��Show all: Yes\n";
//	}
//	else
//	{
//		strAll += "��Z��Show all: No\n";
//	}
//
//	if (m_bShowID)
//	{
//		strAll += "��X��Show ID: Yes\n";
//	}
//	else
//	{
//		strAll += "��X��Show ID: No\n";
//	}
//
//	if (m_bShowBlob)
//	{
//		strAll += "��C��Show sift: Yes ";
//	}
//	else
//	{
//		strAll += "��C��Show sift: No ";
//	}
//
//	if (m_pMVSDoc && m_pMVSDoc->m_pCam)
//	{
//		str.Format("(%d)\n", m_pMVSDoc->m_pCam->m_featsBlob.key_points.size());
//	}
//	else
//	{
//		str.Format("(null)\n");
//	}
//	strAll += str;
//
//	if (m_bShowCorner)
//	{
//		strAll += "��V��Show FAST: Yes ";
//	}
//	else
//	{
//		strAll += "��V��Show FAST: No ";
//	}
//
//	if (m_pMVSDoc && m_pMVSDoc->m_pCam)
//	{
//		str.Format("(%d)\n", m_pMVSDoc->m_pCam->m_featsCorner.key_points.size());
//	}
//	else
//	{
//		str.Format("(null)\n");
//	}
//	strAll += str;
//
//	if (m_bShowManual)
//	{
//		strAll += "��B��Show Manual: Yes ";
//	}
//	else
//	{
//		strAll += "��B��Show Manual: No ";
//	}
//
//	if (m_pMVSDoc && m_pMVSDoc->m_pCam)
//	{
//		str.Format("(%d)\n", m_pMVSDoc->m_pCam->m_featsManual.key_points.size());
//	}
//	else
//	{
//		str.Format("(null)\n");
//	}
//	strAll += str;
//
//	CPoint scrollPos = GetScrollPosition();
//
//	CRect rectText;
//	rectText.top = scrollPos.y;
//	rectText.left = scrollPos.x;
//	rectText.bottom = scrollPos.y + 300;
//	rectText.right = scrollPos.x + 300;
//
//	dc.DrawText(strAll, &rectText, DT_LEFT | DT_EXTERNALLEADING | DT_WORDBREAK);
//}

void CImageView::DrawInfo(CDC * pDC, CRect & rect)
{
	CString strAll, str;

	if (m_bShowProcessed)
	{
		strAll += "[SPACE] Image shown: Processed\n";
	}
	else
	{
		strAll += "[SPACE] Image shown: Original\n";
	}

	if (m_flagShow == 1)
	{
		strAll += "[Z] Show: ALL\n";
	}
	else if (m_flagShow == 2)
	{
		strAll += "[Z] Show: Selected\n";
	}
	else // m_flagShow == 0
	{
		strAll += "[Z] Show: NONE\n";
	}

	if (m_bShowID)
	{
		if (m_bShowTrackID)
		{
			strAll += "[X] Show ID: Track ID\n";
		}
		else
		{
			strAll += "[X] Show ID: Image ID\n";
		}		
	}
	else
	{
		strAll += "[X] Show ID: No\n";
	}
	
	if (m_bShowBlob)
	{
		strAll += "[C] Show sift: Yes ";
	}
	else
	{
		strAll += "[C] Show sift: No ";
	}

	if (m_pMVSDoc && m_pMVSDoc->m_pCam)
	{
		str.Format("(%d/%d)\n", m_pMVSDoc->m_pCam->m_nSiftElected, m_pMVSDoc->m_pCam->m_featsBlob.key_points.size());
	}
	else
	{
		str.Format("(null)\n");
	}
	strAll += str;

	if (m_bShowCorner)
	{
		strAll += "[V] Show FAST: Yes ";
	}
	else
	{
		strAll += "[V] Show FAST: No ";
	}

	if (m_pMVSDoc && m_pMVSDoc->m_pCam)
	{
		str.Format("(%d/%d)\n", m_pMVSDoc->m_pCam->m_nFastElected, m_pMVSDoc->m_pCam->m_featsCorner.key_points.size());
	}
	else
	{
		str.Format("(null)\n");
	}
	strAll += str;

	if (m_bShowManual)
	{
		strAll += "[B] Show Manual: Yes ";
	}
	else
	{
		strAll += "[B] Show Manual: No ";
	}

	if (m_pMVSDoc && m_pMVSDoc->m_pCam)
	{
		str.Format("(%d/%d)\n", m_pMVSDoc->m_pCam->m_nManualElected, m_pMVSDoc->m_pCam->m_featsManual.key_points.size());
	}
	else
	{
		str.Format("(null)\n");
	}
	strAll += str;

	pDC->DrawText(strAll, &rect, DT_LEFT | DT_EXTERNALLEADING | DT_WORDBREAK);
}

BOOL CImageView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	if (!m_pImage || m_pImage->empty() || m_bDragImg)
	{
		return FALSE;
	}

	if (zDelta > 0 && m_idxImgDisplayScale < MAXSCALENUM)
	{
		int preShowScale = m_idxImgDisplayScale;

		m_idxImgDisplayScale++;
		Invalidate(TRUE);
		CSize sz;
		sz.cx = m_pImage->cols * ImageDisplayScales[m_idxImgDisplayScale];
		sz.cy = m_pImage->rows * ImageDisplayScales[m_idxImgDisplayScale];

		CPoint scrollPos = GetScrollPosition();
		SetScrollSizes(MM_TEXT,sz);
		CRect rectClient;
		GetClientRect(&rectClient);

		if (rectClient.Width() < sz.cx && rectClient.Height() < sz.cy)
		{
			CPoint ptCenter;

			ptCenter.x = scrollPos.x + rectClient.Width() / 2;
			ptCenter.y = scrollPos.y + rectClient.Height() / 2;
			ptCenter.x = ptCenter.x * ImageDisplayScales[m_idxImgDisplayScale] / ImageDisplayScales[preShowScale];
			ptCenter.y = ptCenter.y * ImageDisplayScales[m_idxImgDisplayScale] / ImageDisplayScales[preShowScale];
			scrollPos.x = ptCenter.x - rectClient.Width() / 2;
			scrollPos.y = ptCenter.y - rectClient.Height() / 2;

			ScrollToPosition(scrollPos);
		}
	}
	else if (zDelta < 0 && m_idxImgDisplayScale > MINSCALENUM)
	{
		int preShowScale = m_idxImgDisplayScale;

		m_idxImgDisplayScale--;
		Invalidate(TRUE);
		CSize sz;
		sz.cx = m_pImage->cols * ImageDisplayScales[m_idxImgDisplayScale];
		sz.cy = m_pImage->rows * ImageDisplayScales[m_idxImgDisplayScale];

		CPoint scrollPos = GetScrollPosition();
		SetScrollSizes(MM_TEXT,sz);
		CRect rectClient;
		GetClientRect(&rectClient);

		if (rectClient.Width() < sz.cx && rectClient.Height() < sz.cy)
		{
			CPoint ptCenter;

			ptCenter.x = scrollPos.x + rectClient.Width() / 2;
			ptCenter.y = scrollPos.y + rectClient.Height() / 2;
			ptCenter.x = ptCenter.x * ImageDisplayScales[m_idxImgDisplayScale] / ImageDisplayScales[preShowScale];
			ptCenter.y = ptCenter.y * ImageDisplayScales[m_idxImgDisplayScale] / ImageDisplayScales[preShowScale];
			scrollPos.x = ptCenter.x - rectClient.Width() / 2;
			scrollPos.y = ptCenter.y - rectClient.Height() / 2;

			ScrollToPosition(scrollPos);
		}
	}

	CString strTmp;
	strTmp.Format("SCALE:%lf", ImageDisplayScales[m_idxImgDisplayScale]);
	CMFCStatusBar * pStaBar;
	pStaBar = (CMFCStatusBar *)AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	pStaBar->SetPaneText(1,strTmp);

	return TRUE;

	//return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}


void CImageView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (m_pImage && !m_pImage->empty())
	{
		CPoint scrollPos;
		scrollPos = GetScrollPosition();

		if (nFlags == MK_MBUTTON) // ������ּ�����
		{
			CPoint posChange = point - m_ptPreGrab;
			scrollPos -= posChange;
			ScrollToPosition(scrollPos);
			m_ptPreGrab = point;

			if (m_bShowInfo)
			{
				CRect rectInfo;
				rectInfo.top = 0;
				rectInfo.left = 0;
				rectInfo.bottom = rectInfo.top + m_nHeightInfoRect;
				rectInfo.right = rectInfo.left + m_nWidthInfoRect;

				InvalidateRect(&rectInfo, FALSE);
			}
		}

		double pX = (point.x + scrollPos.x) / (ImageDisplayScales[m_idxImgDisplayScale]);
		double pY = (point.y + scrollPos.y) / (ImageDisplayScales[m_idxImgDisplayScale]);
		int lPX = (int)pX;
		int lPY = (int)pY;

		if (lPX >= 0 && lPY >= 0 && lPX < m_pImage->cols && lPY < m_pImage->rows)
		{
			uchar rgbBlue, rgbGreen, rgbRed;

			if (1 == m_pImage->channels())
			{
				Scalar pix = m_pImage->at<uchar>(lPY, lPX);
				rgbRed = rgbGreen = rgbBlue = pix.val[0];
			}
			else
			{
				Vec3b pix = m_pImage->at<Vec3b>(lPY, lPX);
				rgbBlue = pix.val[0]; rgbGreen = pix.val[1]; rgbRed = pix.val[2];
			}

			CString strTmp;
			strTmp.Format("POS:(%4.2lf,%4.2lf)  RGB:(%3d,%3d,%3d)", pX, pY, rgbRed, rgbGreen, rgbBlue);
			CMFCStatusBar * pStaBar;
			pStaBar = (CMFCStatusBar *)AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR);
			pStaBar->SetPaneText(0, strTmp);
			strTmp.Format("SCALE:%lf", ImageDisplayScales[m_idxImgDisplayScale]);
			pStaBar->SetPaneText(1, strTmp);
		}		
	}
		
	CScrollView::OnMouseMove(nFlags, point);
}


void CImageView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	if (nChar == VK_SPACE) // �л�ԭͼ��Ԥ����ͼ����ʾ��ע�������ǲ�Ҫ��ͼ��Ϊ�յģ�����ӿ�ͼ�����л�������ԭͼ�ġ�
	{
		if (!m_bShowProcessed)
		{
			m_pImage = m_pMVSDoc->m_pImgProcessed;
			m_bShowProcessed = TRUE;
		}
		else
		{
			m_pImage = m_pMVSDoc->m_pImgOriginal;
			m_bShowProcessed = FALSE;
		}
//		SetImageScrollSize();
		Invalidate(TRUE);
	}

	if (m_pImage && !m_pImage->empty())
	{
		switch (nChar)
		{
		case VK_HOME:
			OnVScroll(SB_TOP, 0, NULL);
			OnHScroll(SB_LEFT, 0, NULL);
			break;
		case VK_END:
			OnVScroll(SB_BOTTOM, 0, NULL);
			OnHScroll(SB_RIGHT, 0, NULL);
			break;
		case VK_UP:
			OnVScroll(SB_LINEUP, 0, NULL);
			break;
		case VK_DOWN:
			OnVScroll(SB_LINEDOWN, 0, NULL);
			break;
		case VK_LEFT:
			OnHScroll(SB_LINELEFT, 0, NULL);
			break;
		case VK_RIGHT:
			OnHScroll(SB_LINERIGHT, 0, NULL);
			break;
		default:
			break;
		}
		
		if (nChar == 'e' || nChar == 'E')
		{
			if (GetKeyState(VK_CONTROL) < 0) // Ctrl is pressed at the same time
			{
				m_pMVSDoc->ExtractPointsContinuously(TRUE);
			} 
			else
			{
				m_pMVSDoc->ExtractPointsContinuously(FALSE);
			}			
		}

		if (nChar == 'q' || nChar == 'Q')
		{
			if (GetKeyState(VK_CONTROL) < 0) // Ctrl is pressed at the same time
			{
				m_pMVSDoc->ExtractSiftFeatures();
			}
		}

		if (nChar == 't' || nChar == 'T')
		{
			if (GetKeyState(VK_CONTROL) < 0) // Ctrl is pressed at the same time
			{
				m_pMVSDoc->ExtractFASTFeatures();
			}
		}

		if (nChar == 'r' || nChar == 'R')
		{
			if (GetKeyState(VK_CONTROL) < 0) // Ctrl is pressed at the same time
			{
				m_pMVSDoc->ExtractORBFeatures();
			}
		}

		if (nChar == 'g' || nChar == 'G')
		{
			if (GetKeyState(VK_CONTROL) < 0) // Ctrl is pressed at the same time
			{
				m_pMVSDoc->GenSfMFeatures();
			}
		}

		if (nChar == VK_DELETE)
		{
			m_pMVSDoc->DeleteAllFeatures();
		}

		if (nChar == 'z' || nChar == 'Z') // �л��Ƿ���ʾ��������
		{
			if (m_flagShow == 1)
			{
				m_flagShow = 2;
			}
			else if (m_flagShow == 2)
			{
				m_flagShow = 0;
			}
			else // m_flagShow == 0
			{
				m_flagShow = 1;
			}
			Invalidate(TRUE); // TRUE ������ԭ������ͼ��߿�� text �ػ�
		}

		if (nChar == 'x' || nChar == 'X') // �л��Ƿ���ʾ�������
		{
			if (!m_bShowID && !m_bShowTrackID) // ʲô ID �Ŷ�����ʾ
			{
				m_bShowID = TRUE;
			}
			else if (m_bShowID && !m_bShowTrackID) // ��ʾͼ��ֲ� ID ��
			{
				m_bShowTrackID = TRUE;
			}
			else // ��ʾÿ�����ȫ�� Track ID ��
			{
				m_bShowID = FALSE;
				m_bShowTrackID = FALSE;
			}
			Invalidate(TRUE);
		}

		if (nChar == 'c' || nChar == 'C') // �л��Ƿ���ʾ sift ����
		{
			if (m_bShowBlob)
			{
				m_bShowBlob = FALSE;
			}
			else
			{
				m_bShowBlob = TRUE;
			}
			Invalidate(TRUE);
		}

		if (nChar == 'v' || nChar == 'V') // �л��Ƿ���ʾ fast ����
		{
			if (m_bShowCorner)
			{
				m_bShowCorner = FALSE;
			}
			else
			{
				m_bShowCorner = TRUE;
			}
			Invalidate(TRUE);
		}

		if (nChar == 'b' || nChar == 'B') // �л��Ƿ���ʾ�����
		{
			if (m_bShowManual)
			{
				m_bShowManual = FALSE;
			}
			else
			{
				m_bShowManual = TRUE;
			}
			Invalidate(TRUE);
		}

		if (nChar == 'n' || nChar == 'N') // �л��Ƿ���ʾ�����߶Ⱥͷ���
		{
			if (m_bShowRadiusAngle)
			{
				m_bShowRadiusAngle = FALSE;
			}
			else
			{
				m_bShowRadiusAngle = TRUE;
			}
			Invalidate(TRUE);
		}
	}

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CImageView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (m_pImage && !m_pImage->empty())
	{
		CPoint scrollPos = GetScrollPosition();

		double pX = (point.x + scrollPos.x) / (ImageDisplayScales[m_idxImgDisplayScale]);
		double pY = (point.y + scrollPos.y) / (ImageDisplayScales[m_idxImgDisplayScale]);
		int lPX = (int)pX;
		int lPY = (int)pY;

		if (lPX >= 0 && lPY >= 0 && lPX < m_pImage->cols && lPY < m_pImage->rows)
		{
			if (m_bStartExtractPt)
			{
				m_ptExtracted.x = pX;
				m_ptExtracted.y = pY;
				m_flagPointExtracted = 1;
			}
		}		
	}

	CScrollView::OnLButtonDown(nFlags, point);
}


BOOL CImageView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default

	//���������״������SetCursor�����OnSetCursor���������Ч��
	if (nHitTest == HTCLIENT)
	{
		if (m_bDragImg)
		{
			SetCursor(hCursorGrab);
		}
		else if (m_bStartExtractPt)
		{
			SetCursor(hCursorCross);
		}
		else
		{
			SetCursor(hCursorArrow);
		}

		return TRUE;
	}

	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}


void CImageView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	if (m_bShowInfo)
	{
		CRect rectInfo;
		rectInfo.top = 0;
		rectInfo.left = 0;
		rectInfo.bottom = rectInfo.top + m_nHeightInfoRect;
		rectInfo.right = rectInfo.left + m_nWidthInfoRect;

		InvalidateRect(&rectInfo, FALSE);
	}

	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CImageView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	if (m_bShowInfo)
	{
		CRect rectInfo;
		rectInfo.top = 0;
		rectInfo.left = 0;
		rectInfo.bottom = rectInfo.top + m_nHeightInfoRect;
		rectInfo.right = rectInfo.left + m_nWidthInfoRect;

		InvalidateRect(&rectInfo, FALSE);
	}

	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CImageView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	m_bDragImg = TRUE;
	SetCursor(hCursorGrab);
	m_ptPreGrab = point;

	CScrollView::OnMButtonDown(nFlags, point);
}


void CImageView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	m_bDragImg = FALSE;
	SetCursor(hCursorArrow);

	CScrollView::OnMButtonUp(nFlags, point);
}
