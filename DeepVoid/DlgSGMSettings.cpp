// DlgSGMSettings.cpp : implementation file
//

#include "stdafx.h"
#include "DeepVoid.h"
#include "DlgSGMSettings.h"
#include "afxdialogex.h"


// CDlgSGMSettings dialog

IMPLEMENT_DYNAMIC(CDlgSGMSettings, CDialog)

CDlgSGMSettings::CDlgSGMSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSGMSettings::IDD, pParent)
	, m_idxLayout(0)
	, m_idxImplementation(0)
{

}

CDlgSGMSettings::~CDlgSGMSettings()
{
}

void CDlgSGMSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_idxLayout);
	DDX_Radio(pDX, IDC_RADIO3, m_idxImplementation);
}


BEGIN_MESSAGE_MAP(CDlgSGMSettings, CDialog)
END_MESSAGE_MAP()


// CDlgSGMSettings message handlers
