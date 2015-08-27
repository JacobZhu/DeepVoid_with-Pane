// DlgZhuMethodSettings.cpp : implementation file
//

#include "stdafx.h"
#include "DeepVoid.h"
#include "DlgZhuMethodSettings.h"
#include "afxdialogex.h"


// CDlgZhuMethodSettings dialog

IMPLEMENT_DYNAMIC(CDlgZhuMethodSettings, CDialog)

CDlgZhuMethodSettings::CDlgZhuMethodSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgZhuMethodSettings::IDD, pParent)
	, m_idxSimilarityMeasure(0)
	, m_wndSize(0)
	, m_maxNormAng(0)
	, m_nPatchMatchIter(0)
	, m_deFactor(0)
	, m_nRandSamp(0)
	, m_imgUncertainty(0)
{

}

CDlgZhuMethodSettings::~CDlgZhuMethodSettings()
{
}

void CDlgZhuMethodSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_idxSimilarityMeasure);
	DDX_Text(pDX, IDC_EDIT1, m_wndSize);
	DDX_Text(pDX, IDC_EDIT2, m_maxNormAng);
	DDX_Text(pDX, IDC_EDIT4, m_nPatchMatchIter);
	DDX_Text(pDX, IDC_EDIT8, m_deFactor);
	DDX_Text(pDX, IDC_EDIT11, m_nRandSamp);
	DDX_Text(pDX, IDC_EDIT12, m_imgUncertainty);
}


BEGIN_MESSAGE_MAP(CDlgZhuMethodSettings, CDialog)
END_MESSAGE_MAP()


// CDlgZhuMethodSettings message handlers
