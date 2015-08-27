// DlgSGMZZKSettings.cpp : implementation file
//

#include "stdafx.h"
#include "DeepVoid.h"
#include "DlgSGMZZKSettings.h"
#include "afxdialogex.h"


// CDlgSGMZZKSettings dialog

IMPLEMENT_DYNAMIC(CDlgSGMZZKSettings, CDialog)

CDlgSGMZZKSettings::CDlgSGMZZKSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSGMZZKSettings::IDD, pParent)
	, m_idxDSItype(0)
// 	, m_minDisparity(0)
// 	, m_maxDisparity(0)
	, m_P1(0)
	, m_P2(0)
	, m_idxPaths(0)
	, m_idxSubpixel(0)
	, m_threshConstCheck(0)
{

}

CDlgSGMZZKSettings::~CDlgSGMZZKSettings()
{
}

void CDlgSGMZZKSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO7, m_idxDSItype);
// 	DDX_Text(pDX, IDC_EDIT1, m_minDisparity);
// 	DDX_Text(pDX, IDC_EDIT2, m_maxDisparity);
	DDX_Text(pDX, IDC_EDIT6, m_P1);
	DDX_Text(pDX, IDC_EDIT7, m_P2);
	DDX_Radio(pDX, IDC_RADIO2, m_idxPaths);
	DDX_Radio(pDX, IDC_RADIO5, m_idxSubpixel);
	DDX_Text(pDX, IDC_EDIT8, m_threshConstCheck);
}


BEGIN_MESSAGE_MAP(CDlgSGMZZKSettings, CDialog)
END_MESSAGE_MAP()


// CDlgSGMZZKSettings message handlers
