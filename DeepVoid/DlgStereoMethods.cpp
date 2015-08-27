// DlgStereoMethods.cpp : implementation file
//

#include "stdafx.h"
#include "DeepVoid.h"
#include "DlgStereoMethods.h"
#include "afxdialogex.h"


// CDlgStereoMethods dialog

IMPLEMENT_DYNAMIC(CDlgStereoMethods, CDialog)

CDlgStereoMethods::CDlgStereoMethods(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStereoMethods::IDD, pParent)
	, m_idxStereoMethod(0)
{

}

CDlgStereoMethods::~CDlgStereoMethods()
{
}

void CDlgStereoMethods::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_idxStereoMethod);
}


BEGIN_MESSAGE_MAP(CDlgStereoMethods, CDialog)
END_MESSAGE_MAP()


// CDlgStereoMethods message handlers
