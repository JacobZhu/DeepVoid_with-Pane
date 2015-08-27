#pragma once


// CDlgStereoFiles dialog

class CDlgStereoFiles : public CDialog
{
	DECLARE_DYNAMIC(CDlgStereoFiles)

public:
	CDlgStereoFiles(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgStereoFiles();

// Dialog Data
	enum { IDD = IDD_STEREOFILES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_pathImg0;
	CString m_pathImg1;
	CString m_pathParam0;
	CString m_pathParam1;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	int m_nPixDownSample;
	BOOL m_bSetZeroDisparity;
	int m_idxRectifyScaling;
	int m_minDisparity;
	int m_maxDisparity;
};
