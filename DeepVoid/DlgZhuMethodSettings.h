#pragma once


// CDlgZhuMethodSettings dialog

class CDlgZhuMethodSettings : public CDialog
{
	DECLARE_DYNAMIC(CDlgZhuMethodSettings)

public:
	CDlgZhuMethodSettings(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgZhuMethodSettings();

// Dialog Data
	enum { IDD = IDD_ZHUMETHODSETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_idxSimilarityMeasure;
	int m_wndSize;
	double m_maxNormAng;
	int m_nPatchMatchIter;
	double m_deFactor;
	int m_nRandSamp;
	double m_imgUncertainty;
};
