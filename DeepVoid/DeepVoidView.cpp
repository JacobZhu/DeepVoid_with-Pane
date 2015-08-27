
// DeepVoidView.cpp : implementation of the CDeepVoidView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DeepVoid.h"
#endif

#include "DeepVoidDoc.h"
#include "DeepVoidView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDeepVoidView

IMPLEMENT_DYNCREATE(CDeepVoidView, CView)

BEGIN_MESSAGE_MAP(CDeepVoidView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDeepVoidView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CDeepVoidView construction/destruction

CDeepVoidView::CDeepVoidView()
{
	// TODO: add construction code here

}

CDeepVoidView::~CDeepVoidView()
{
}

BOOL CDeepVoidView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CDeepVoidView drawing

void CDeepVoidView::OnDraw(CDC* /*pDC*/)
{
	CDeepVoidDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CDeepVoidView printing


void CDeepVoidView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDeepVoidView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDeepVoidView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDeepVoidView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CDeepVoidView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDeepVoidView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDeepVoidView diagnostics

#ifdef _DEBUG
void CDeepVoidView::AssertValid() const
{
	CView::AssertValid();
}

void CDeepVoidView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDeepVoidDoc* CDeepVoidView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDeepVoidDoc)));
	return (CDeepVoidDoc*)m_pDocument;
}
#endif //_DEBUG


// CDeepVoidView message handlers
