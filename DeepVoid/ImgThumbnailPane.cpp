// ImgThumbnailPane.cpp : implementation file
//

#include "stdafx.h"
#include "DeepVoid.h"
#include "ImgThumbnailPane.h"


// CImgThumbnailPane

IMPLEMENT_DYNAMIC(CImgThumbnailPane, CDockablePane)

CImgThumbnailPane::CImgThumbnailPane()
{

}

CImgThumbnailPane::~CImgThumbnailPane()
{
}


BEGIN_MESSAGE_MAP(CImgThumbnailPane, CDockablePane)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CImgThumbnailPane message handlers




void CImgThumbnailPane::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDockablePane::OnPaint() for painting messages

// 	CRect rectList;
// 	m_wndImgListCtrl.GetWindowRect(rectList);
// 	ScreenToClient(rectList);
// 
// 	rectList.InflateRect(1, 1);
// 	dc.Draw3dRect(rectList, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}


int CImgThumbnailPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	CRect rect;
	rect.SetRectEmpty();

	// Create image list control:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | LVS_ALIGNTOP | LVS_ICON | LVS_AUTOARRANGE;

	if (!m_wndImgListCtrl.Create(dwViewStyle, rect, this, 0))
	{
		TRACE0("Failed to create thumbnail pane\n");
		return -1;      // fail to create
	}

	m_wndImgListCtrl.InitImgListCtrl(160, 120);

	//
// 	m_wndImgListCtrl.InsertColumn(0, _T("Images"), LVCFMT_LEFT, 50);
// 	m_wndImgListCtrl.InsertColumn(1, _T("Index"), LVCFMT_LEFT, 50);
// 	m_wndImgListCtrl.InsertItem(0, _T("A"));
// 	m_wndImgListCtrl.InsertItem(1, _T("B"));
// 	m_wndImgListCtrl.InsertItem(2, _T("C"));
// 	m_wndImgListCtrl.InsertItem(3, _T("D"));
// 	m_wndImgListCtrl.InsertItem(4, _T("E"));
// 	m_wndImgListCtrl.InsertItem(5, _T("F"));
// 
// 	m_wndImgListCtrl.SetItemText(0, 1, _T("A0"));
// 	m_wndImgListCtrl.SetItemText(1, 1, _T("B0"));
// 	m_wndImgListCtrl.SetItemText(2, 1, _T("C0"));
// 	m_wndImgListCtrl.SetItemText(3, 1, _T("D0"));
// 	m_wndImgListCtrl.SetItemText(4, 1, _T("E0"));
// 	m_wndImgListCtrl.SetItemText(5, 1, _T("F0"));

	return 0;
}


void CImgThumbnailPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	m_wndImgListCtrl.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOACTIVATE | SWP_NOZORDER);
	// TODO: Add your message handler code here
}
