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
	m_nBasicHalfLength = 2;	// 1 倍显示倍率下十字丝半长轴的像素跨度
	m_nBasicRadius = 2;		// 1 倍显示倍率下圆圈的半径像素跨度
	m_nHeightInfoFont = 20;
	m_nHeightInfoRect = 150; // 字体高度20，该参数120意味着最多显示 6 行字
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

	// 1、最先把图像显示出来，怕画晚了把后续的综合信息以及图像特征给覆盖掉了
	if (m_pImage && !m_pImage->empty())
	{
		DisplayImage(pDC, *m_pImage, 0, 0, m_pImage->cols * scale, m_pImage->rows * scale);
	}
	
// 	CClientDC dc(this);
// 	OnPrepareDC(&dc);
// 	dc.SetBkMode(TRANSPARENT);
	pDC->SetBkMode(TRANSPARENT);

	// 2、再把综合信息显示出来
	if (m_bShowInfo) // 显示 info
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
	
	// 3、再把所有图像几何特征画出来
	if (m_pImage && !m_pImage->empty() && m_flagShow)
	{
		if (m_bShowBlob) // 显示光团特征点
		{
			int n = m_pMVSDoc->m_pCam->m_featsBlob.key_points.size();

			if (m_flagShow == 2) // 只显示入选参加 SfM 的 sift 特征点
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
				// size 是特征的直径（diameter），原本觉得应该减 1 再除以 2 比较合适，但是发现 sift 特征中不少size是1.*的，且很多也不是偶数，所以还是直接除以 2 更好点。
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
					if (!m_bShowRadiusAngle || radius <= 0) // 如果特征没有尺度信息那就只画个十字丝
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

		if (m_bShowCorner) // 显示角点特征
		{
			int n = m_pMVSDoc->m_pCam->m_featsCorner.key_points.size();

			if (m_flagShow == 2) // 只显示入选参加 SfM 的 FAST 特征点
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

		if (m_bShowManual) // 显示手提点
		{
			int n = m_pMVSDoc->m_pCam->m_featsManual.key_points.size();

			if (m_flagShow == 2) // 只显示入选参加 SfM 的手提点
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
					// 20200901，添加这句是因为key_points向量组在刷新之前就成型了，但是tracks向量组却可能还是空的呢
					// 当然了，这个问题只在手提点时才有可能出现，因为自动提光团点或者角点时，是完成所有任务后才刷新的
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

	// 1. 先画圆 //////////////////////////////////////////////////////////////
	pDC->MoveTo(ptX + nr, ptY);
	pDC->AngleArc(ptX, ptY, nr, 0, 360);
	//////////////////////////////////////////////////////////////////////////

	// 2. 再画特征方向 ////////////////////////////////////////////////////////
	// 先根据特征方向角度生成二维旋转矩阵
	if (angle >= 0 && angle <= 360) // 角度范围合理时才画
	{
		cv::Matx22d R = DeepVoid::GenR2D_Angle(angle);
		cv::Matx21d X;
		X(0) = nr;
		cv::Matx21d X0 = R*X;

		pDC->MoveTo(ptX, ptY);
		pDC->LineTo(int(ptX + X0(0) + 0.5), int(ptY + X0(1) + 0.5));
	}	
	//////////////////////////////////////////////////////////////////////////

	// 3. 最后显示序号 ////////////////////////////////////////////////////////
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
//		str.Format("【SPACE】Image shown: Processed\n");
//		strAll += str;
//	}
//	else
//	{
//		strAll += "【SPACE】Image shown: Original\n";
//	}
//
//	if (m_bShowAll)
//	{
//		strAll += "【Z】Show all: Yes\n";
//	}
//	else
//	{
//		strAll += "【Z】Show all: No\n";
//	}
//
//	if (m_bShowID)
//	{
//		strAll += "【X】Show ID: Yes\n";
//	}
//	else
//	{
//		strAll += "【X】Show ID: No\n";
//	}
//
//	if (m_bShowBlob)
//	{
//		strAll += "【C】Show sift: Yes ";
//	}
//	else
//	{
//		strAll += "【C】Show sift: No ";
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
//		strAll += "【V】Show FAST: Yes ";
//	}
//	else
//	{
//		strAll += "【V】Show FAST: No ";
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
//		strAll += "【B】Show Manual: Yes ";
//	}
//	else
//	{
//		strAll += "【B】Show Manual: No ";
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

		if (nFlags == MK_MBUTTON) // 如果滚轮键按下
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

	if (nChar == VK_SPACE) // 切换原图和预处理图像显示。注意这里是不要求图像不为空的，否则从空图像是切换不回来原图的。
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

		if (nChar == 'z' || nChar == 'Z') // 切换是否显示各种特征
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
			Invalidate(TRUE); // TRUE 才能让原本画出图像边框的 text 重绘
		}

		if (nChar == 'x' || nChar == 'X') // 切换是否显示特征编号
		{
			if (!m_bShowID && !m_bShowTrackID) // 什么 ID 号都不显示
			{
				m_bShowID = TRUE;
			}
			else if (m_bShowID && !m_bShowTrackID) // 显示图像局部 ID 号
			{
				m_bShowTrackID = TRUE;
			}
			else // 显示每个点的全局 Track ID 号
			{
				m_bShowID = FALSE;
				m_bShowTrackID = FALSE;
			}
			Invalidate(TRUE);
		}

		if (nChar == 'c' || nChar == 'C') // 切换是否显示 sift 特征
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

		if (nChar == 'v' || nChar == 'V') // 切换是否显示 fast 特征
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

		if (nChar == 'b' || nChar == 'B') // 切换是否显示手提点
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

		if (nChar == 'n' || nChar == 'N') // 切换是否显示特征尺度和方向
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

	//设置鼠标形状的命令SetCursor在这个OnSetCursor里面才是有效的
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
