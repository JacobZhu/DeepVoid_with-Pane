#pragma once


// CDlgSGMSettings dialog

class CDlgSGMSettings : public CDialog
{
	DECLARE_DYNAMIC(CDlgSGMSettings)

public:
	CDlgSGMSettings(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSGMSettings();

// Dialog Data
	enum { IDD = IDD_SGMSETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_idxLayout;
	int m_idxImplementation;
};
