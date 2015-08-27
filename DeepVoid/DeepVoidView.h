
// DeepVoidView.h : interface of the CDeepVoidView class
//

#pragma once


class CDeepVoidView : public CView
{
protected: // create from serialization only
	CDeepVoidView();
	DECLARE_DYNCREATE(CDeepVoidView)

// Attributes
public:
	CDeepVoidDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CDeepVoidView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DeepVoidView.cpp
inline CDeepVoidDoc* CDeepVoidView::GetDocument() const
   { return reinterpret_cast<CDeepVoidDoc*>(m_pDocument); }
#endif

