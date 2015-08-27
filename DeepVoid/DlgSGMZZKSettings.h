#pragma once


// CDlgSGMZZKSettings dialog

class CDlgSGMZZKSettings : public CDialog
{
	DECLARE_DYNAMIC(CDlgSGMZZKSettings)

public:
	CDlgSGMZZKSettings(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSGMZZKSettings();

// Dialog Data
	enum { IDD = IDD_SGMZZKSETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_idxDSItype;
// 	int m_minDisparity;
// 	int m_maxDisparity;
	double m_P1;
	double m_P2;
	int m_idxPaths;
	int m_idxSubpixel;
	int m_threshConstCheck;
};
