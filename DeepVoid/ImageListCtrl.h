#pragma once


// CImageListCtrl

class CImageListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CImageListCtrl)

public:
	CImageListCtrl();
	virtual ~CImageListCtrl();

// attributes
public:
	int m_w;
	int m_h;

	CImageList m_imgList;

// operations
public:
	BOOL InitImgListCtrl(int w, int h);
	BOOL InitImgListCtrl(void);
	BOOL AddOneImage(CString path);
	void DeleteSelImages(void);
	void OpenSelImages(void);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


