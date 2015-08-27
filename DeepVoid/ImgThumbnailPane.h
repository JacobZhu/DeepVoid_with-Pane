#pragma once

#include "ImageListCtrl.h"

// CImgThumbnailPane

class CImgThumbnailPane : public CDockablePane
{
	DECLARE_DYNAMIC(CImgThumbnailPane)

public:
	CImageListCtrl m_wndImgListCtrl;

public:
	CImgThumbnailPane();
	virtual ~CImgThumbnailPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


