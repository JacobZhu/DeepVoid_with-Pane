#pragma once


// CDlgStereoMethods dialog

class CDlgStereoMethods : public CDialog
{
	DECLARE_DYNAMIC(CDlgStereoMethods)

public:
	CDlgStereoMethods(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgStereoMethods();

// Dialog Data
	enum { IDD = IDD_STEREOMETHODS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_idxStereoMethod;
};
