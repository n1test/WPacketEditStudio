// SmartSniffView.h : interface of the CSmartSniffView class
//


#pragma once


class CPHPEditorView : public CView
{
protected: // create from serialization only
	CPHPEditorView();
	DECLARE_DYNCREATE(CPHPEditorView)

// Attributes
public:
	CPHPDoc* GetDocument() const;

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
	virtual void OnOpenProcess(void);

// Implementation
public:
	virtual ~CPHPEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SmartSniffView.cpp
//inline CSmartSniffDoc* CSmartSniffView::GetDocument() const
//   { return reinterpret_cast<CSmartSniffDoc*>(m_pDocument); }
#endif

