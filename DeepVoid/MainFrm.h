
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "ImgThumbnailPane.h"
#include "ShowInfoPane.h"

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
	CImgThumbnailPane	m_wndImgThumbnailPane;	// zhaokunz, 20140226
	CShowInfoPane		m_wndShowInfoPane;		// zhaokunz, 20140301
	ChildFrmLayoutType	m_childFrmLayout;		// zhaokunz, 20140226

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

	BOOL CMainFrame::CreateDockingWindows();
public:
	afx_msg void OnWndlayoutsTabbed();
	afx_msg void OnWndlayoutsCascade();
	afx_msg void OnWndlayoutsTilehorz();
	afx_msg void OnWndlayoutsTilevert();
	afx_msg void OnUpdateWndlayoutsCascade(CCmdUI *pCmdUI);
	afx_msg void OnUpdateWndlayoutsTabbed(CCmdUI *pCmdUI);
	afx_msg void OnUpdateWndlayoutsTilehorz(CCmdUI *pCmdUI);
	afx_msg void OnUpdateWndlayoutsTilevert(CCmdUI *pCmdUI);
};


