// DlgStereoFiles.cpp : implementation file
//

#include "stdafx.h"
#include "DeepVoid.h"
#include "DlgStereoFiles.h"
#include "afxdialogex.h"


// CDlgStereoFiles dialog

IMPLEMENT_DYNAMIC(CDlgStereoFiles, CDialog)

CDlgStereoFiles::CDlgStereoFiles(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStereoFiles::IDD, pParent)
	, m_pathImg0(_T(""))
	, m_pathImg1(_T(""))
	, m_pathParam0(_T(""))
	, m_pathParam1(_T(""))
	, m_nPixDownSample(0)
	, m_bSetZeroDisparity(FALSE)
	, m_idxRectifyScaling(0)
	, m_minDisparity(0)
	, m_maxDisparity(0)
{

}

CDlgStereoFiles::~CDlgStereoFiles()
{
}

void CDlgStereoFiles::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_pathImg0);
	DDX_Text(pDX, IDC_EDIT2, m_pathImg1);
	DDX_Text(pDX, IDC_EDIT3, m_pathParam0);
	DDX_Text(pDX, IDC_EDIT4, m_pathParam1);
	DDX_Text(pDX, IDC_EDIT5, m_nPixDownSample);
	DDV_MinMaxInt(pDX, m_nPixDownSample, 1, 64);
	DDX_Radio(pDX, IDC_RADIO7, m_bSetZeroDisparity);
	DDX_Radio(pDX, IDC_RADIO9, m_idxRectifyScaling);
	DDX_Text(pDX, IDC_EDIT9, m_minDisparity);
	DDX_Text(pDX, IDC_EDIT10, m_maxDisparity);
}


BEGIN_MESSAGE_MAP(CDlgStereoFiles, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgStereoFiles::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgStereoFiles::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgStereoFiles::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgStereoFiles::OnBnClickedButton4)
END_MESSAGE_MAP()


// CDlgStereoFiles message handlers


void CDlgStereoFiles::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CString filt = "Image(BMP;IMG;JPG;PCX;GIF;TIF;TGA;PGM;PBM;PNG)|*.BMP;*.IMG;*.JPG;*.PCX;*.GIF;*.TIF;*.TGA;*.PGM;*.PBM;*.PNG|(*.BMP)|*.BMP|(*.IMG)|*.IMG|(*.JPG)|*.JPG|(*.PCX)|*.PCX|(*.GIF)|*.GIF|(*.TIF)|*.TIF|(*.TGA)|*.TGA|(*.PGM)|*.PGM|(*.PBM)|*.PBM|(*.PNG)|*.PNG||";
	CFileDialog  dlgFile(TRUE,"*.BMP","", OFN_HIDEREADONLY, filt, NULL);
	dlgFile.m_ofn.Flags|=OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT;

	if (dlgFile.DoModal() == IDOK)
	{
		m_pathImg0 = dlgFile.GetPathName();
		UpdateData(FALSE);
	}
}


void CDlgStereoFiles::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	CString filt = "Image(BMP;IMG;JPG;PCX;GIF;TIF;TGA;PGM;PBM;PNG)|*.BMP;*.IMG;*.JPG;*.PCX;*.GIF;*.TIF;*.TGA;*.PGM;*.PBM;*.PNG|(*.BMP)|*.BMP|(*.IMG)|*.IMG|(*.JPG)|*.JPG|(*.PCX)|*.PCX|(*.GIF)|*.GIF|(*.TIF)|*.TIF|(*.TGA)|*.TGA|(*.PGM)|*.PGM|(*.PBM)|*.PBM|(*.PNG)|*.PNG||";
	CFileDialog  dlgFile(TRUE,"*.BMP","", OFN_HIDEREADONLY, filt, NULL);
	dlgFile.m_ofn.Flags|=OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT;

	if (dlgFile.DoModal() == IDOK)
	{
		m_pathImg1 = dlgFile.GetPathName();
		UpdateData(FALSE);
	}
}


void CDlgStereoFiles::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	CString filt = "Param File(*.txt)|*.txt";
	CFileDialog  dlgFile(TRUE,"*.txt","", OFN_HIDEREADONLY, filt, NULL);
	dlgFile.m_ofn.Flags|=OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT;

	if (dlgFile.DoModal() == IDOK)
	{
		m_pathParam0 = dlgFile.GetPathName();
		UpdateData(FALSE);
	}
}


void CDlgStereoFiles::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	CString filt = "ExPara File(*.txt)|*.txt";
	CFileDialog  dlgFile(TRUE,"*.txt","", OFN_HIDEREADONLY, filt, NULL);
	dlgFile.m_ofn.Flags|=OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT;

	if (dlgFile.DoModal() == IDOK)
	{
		m_pathParam1 = dlgFile.GetPathName();
		UpdateData(FALSE);
	}
}
