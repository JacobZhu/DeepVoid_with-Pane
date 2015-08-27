#pragma once


// CShowInfoListCtrl

class CShowInfoListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CShowInfoListCtrl)

// attributes
public:

// operations
public: 
	void AddOneInfo(CString info);

public:
	CShowInfoListCtrl();
	virtual ~CShowInfoListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
};


