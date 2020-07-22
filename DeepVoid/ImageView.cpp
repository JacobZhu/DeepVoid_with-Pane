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
	m_bShowProcessed = TRUE;

	m_bStartExtractPt = FALSE;

	m_bShowAll = TRUE;
	m_bShowSIFT = TRUE;
	m_bShowFAST = TRUE;
	m_bShowManual = TRUE;
	m_bShowID = TRUE;
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
	
	if (!m_pImage || m_pImage->empty())
	{
		return;
	}

	DisplayImage(pDC, *m_pImage, 0, 0, m_pImage->cols * ImageDisplayScales[m_idxImgDisplayScale], m_pImage->rows * ImageDisplayScales[m_idxImgDisplayScale]);

	if (!m_bShowAll)
	{
		return;
	}

// 	CPoint scrollPosition;
// 	scrollPosition = GetScrollPosition();

	double x, y;

	if (m_bShowSIFT)
	{

	}

	if (m_bShowFAST)
	{

	}

	if (m_bShowManual)
	{
		int n = m_pMVSDoc->m_pCam->m_featsManual.key_points.size();

		for (int i = 0; i < n; ++i)
		{
			const cv::KeyPoint & keypt = m_pMVSDoc->m_pCam->m_featsManual.key_points[i];

			x = keypt.pt.x*ImageDisplayScales[m_idxImgDisplayScale];
			y = keypt.pt.y*ImageDisplayScales[m_idxImgDisplayScale];

			DrawCross(x, y, 0, 255, 0, i, m_bShowID, 0, 1, 5);
			DrawCircle(x, y, 255, 255, 0, i, m_bShowID, 0, 1, 3);
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

void CImageView::DrawCross(double x, double y, uchar r, uchar g, uchar b, int id,
	BOOL bShowID, int penStyle /*= PS_SOLID*/, int nWidth /*= 1*/, int halfLength /*= 5*/)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	CPen newPen(penStyle, nWidth, RGB(r, g, b));
	
	CPen * pOldPen = (CPen *)dc.SelectObject(&newPen);

	int ptX = int(x + 0.5);
	int ptY = int(y + 0.5);

	dc.MoveTo(ptX - halfLength, ptY);
	dc.LineTo(ptX + halfLength, ptY);
	dc.MoveTo(ptX, ptY - halfLength);
	dc.LineTo(ptX, ptY + halfLength);

	if (bShowID)
	{
		CString strTmp;
		strTmp.Format("%d", id);

		int x_offset = int(0.6*halfLength + 0.5);
		int y_offset = int(3 * halfLength + 0.5);

		dc.SetTextColor(RGB(r, g, b));
		dc.SetBkMode(TRANSPARENT);
		dc.TextOut(ptX + x_offset, ptY - y_offset, strTmp);
	}

	dc.SelectObject(pOldPen);
}

void CImageView::DrawCircle(double x, double y, uchar r, uchar g, uchar b, int id, BOOL bShowID, int penStyle, int nWidth, int nRadius)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	CPen newPen(penStyle, nWidth, RGB(r, g, b));
	
	CPen * pOldPen = (CPen *)dc.SelectObject(&newPen);

	int ptX = int(x + 0.5);
	int ptY = int(y + 0.5);

	dc.MoveTo(ptX + nRadius, ptY);
	dc.AngleArc(ptX, ptY, nRadius, 0, 360);

	if (bShowID)
	{
		CString strTmp;
		strTmp.Format("%d", id);

		int x_offset = int(0.6*nRadius + 0.5);
		int y_offset = int(3 * nRadius + 0.5);

		dc.SetTextColor(RGB(r, g, b));
		dc.SetBkMode(TRANSPARENT);
		dc.TextOut(ptX + x_offset, ptY - y_offset, strTmp);
	}

	dc.SelectObject(pOldPen);
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

	if (!m_pImage || m_pImage->empty())
	{
		return;
	}

	CPoint scrollPosition;
	scrollPosition = GetScrollPosition();

	double pX = (point.x + scrollPosition.x) / (ImageDisplayScales[m_idxImgDisplayScale]);
	double pY = (point.y + scrollPosition.y) / (ImageDisplayScales[m_idxImgDisplayScale]);
//	int lPX = ( (pX) < 0 ? ((int)(pX-0.5)) : ((int)(pX+0.5)) ); // round it
//	int lPY = ( (pY) < 0 ? ((int)(pY-0.5)) : ((int)(pY+0.5)) ); // round it
	int lPX = (int)pX;
	int lPY = (int)pY;
	
	if (lPX < 0 || lPY < 0 || lPX >= m_pImage->cols || lPY >= m_pImage->rows)
	{
		return;
	}

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
	pStaBar->SetPaneText(0,strTmp);
	strTmp.Format("SCALE:%lf", ImageDisplayScales[m_idxImgDisplayScale]);
	pStaBar->SetPaneText(1,strTmp);
		
	CScrollView::OnMouseMove(nFlags, point);
}


void CImageView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (!m_pImage || m_pImage->empty())
	{
		return;
	}

	switch(nChar) 
	{
	case VK_HOME:
		OnVScroll(SB_TOP,0,NULL);
		OnHScroll(SB_LEFT,0,NULL);
		break;
	case VK_END:
		OnVScroll(SB_BOTTOM,0,NULL);
		OnHScroll(SB_RIGHT,0,NULL);
		break;
	case VK_UP:
		OnVScroll(SB_LINEUP,0,NULL);
		break;
	case VK_DOWN:
		OnVScroll(SB_LINEDOWN,0,NULL);
		break;
	case VK_LEFT:
		OnHScroll(SB_LINELEFT,0,NULL);
		break;
	case VK_RIGHT:
		OnHScroll(SB_LINERIGHT,0,NULL);
		break;
	case VK_SPACE:
// 		if (m_pMVSDoc->m_imgHandled.empty())
// 		{
// 			return;
// 		}
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
		Invalidate(TRUE);
		break;
	default:
		break;
	}

	if ((nChar == 'e' || nChar == 'E') && GetKeyState(VK_CONTROL))
	{
		m_pMVSDoc->ExtractPointsContinuously();
	}

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CImageView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (!m_pImage || m_pImage->empty())
	{
		return;
	}

	CPoint scrollPosition;
	scrollPosition = GetScrollPosition();

	double pX = (point.x + scrollPosition.x) / (ImageDisplayScales[m_idxImgDisplayScale]);
	double pY = (point.y + scrollPosition.y) / (ImageDisplayScales[m_idxImgDisplayScale]);
	int lPX = (int)pX;
	int lPY = (int)pY;

	if (lPX < 0 || lPY < 0 || lPX >= m_pImage->cols || lPY >= m_pImage->rows)
	{
		return;
	}

	if (m_bStartExtractPt)
	{
		m_ptExtracted.x = pX;
		m_ptExtracted.y = pY;
		m_flagPointExtracted = 1;
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
