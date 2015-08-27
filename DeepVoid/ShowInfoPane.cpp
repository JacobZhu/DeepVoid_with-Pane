// ShowInfoPane.cpp : implementation file
//

#include "stdafx.h"
#include "DeepVoid.h"
#include "ShowInfoPane.h"


// CShowInfoPane

IMPLEMENT_DYNAMIC(CShowInfoPane, CDockablePane)

CShowInfoPane::CShowInfoPane()
{

}

CShowInfoPane::~CShowInfoPane()
{
}


BEGIN_MESSAGE_MAP(CShowInfoPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CShowInfoPane message handlers




int CShowInfoPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	// initialize ClistCtrl object //////////////////////////////////////////////////////////////////////////
	CRect rect;
	rect.SetRectEmpty();

	// Create show info list control:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | LVS_ALIGNTOP | LVS_REPORT | LVS_AUTOARRANGE | LVS_NOCOLUMNHEADER;

	if (!m_wndShowInfoListCtrl.Create(dwViewStyle, rect, this, 0))
	{
		TRACE0("Failed to create show info list control\n");
		return -1;      // fail to create
	}

	m_wndShowInfoListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES);  // show grid lines
	m_wndShowInfoListCtrl.InsertColumn(0, _T("")); // insert only one column to show info
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	return 0;
}


void CShowInfoPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	m_wndShowInfoListCtrl.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndShowInfoListCtrl.SetColumnWidth(0, cx);
	// TODO: Add your message handler code here
}
