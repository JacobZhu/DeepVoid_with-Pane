#pragma once

#include "ShowInfoListCtrl.h"

// CShowInfoPane

class CShowInfoPane : public CDockablePane
{
	DECLARE_DYNAMIC(CShowInfoPane)

public:
	CShowInfoListCtrl m_wndShowInfoListCtrl;

public:
	CShowInfoPane();
	virtual ~CShowInfoPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


