// SmartSniffView.cpp : implementation of the CSmartSniffView class
//

#include "stdafx.h"
#include "SmartSniff.h"

#include "Documents/DataMethodDoc.h"
#include "Documents/DataFormatDoc.h"
#include "Documents/DataScriptDoc.h"
#include "PHPEditorView.h"
#include "SmartSniffView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSmartSniffView construction/destruction

CPHPEditorView::CPHPEditorView()
{
	// TODO: add construction code here

}

CPHPEditorView::~CPHPEditorView()
{
}

BOOL CPHPEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSmartSniffView drawing

void CPHPEditorView::OnDraw(CDC* /*pDC*/)
{		return;
	// TODO: add draw code for native data here
}


// CSmartSniffView printing

BOOL CPHPEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPHPEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPHPEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CPHPEditorView::OnOpenProcess(void)
{
}

// CSmartSniffView diagnostics

#ifdef _DEBUG
void CPHPEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CPHPEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDataMethodDoc* CPHPEditorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDataMethodDoc)));
	return (CDataMethodDoc*)m_pDocument;
}
#endif //_DEBUG


// CSmartSniffView message handlers
