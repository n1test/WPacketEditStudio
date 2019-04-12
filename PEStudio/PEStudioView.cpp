/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  PEStudioView.cpp

  This file is part of the Packet Edit Studio Project.
  version: ALPHA

  *Distributed under the terms of the zlib/libpng public license.*
  
  Author: 

	Chris Smith (sourcemagik.com)
	(chris@sourcemagik.com)
  
  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

// The view class for the main window client area

#include "stdafx.h"

/*************************************************************/
/*			MFC Debug Code									 */
/*************************************************************/

	#ifdef _DEBUG
		#define new DEBUG_NEW
	#endif


/*************************************************************/
/*			CPEStudioView									 */
/*															 */
/*	This is a basic subclass of CView.						 */
/*************************************************************/

	/*************************************************************/
	/*			Constructor / Destructor						 */
	/*************************************************************/

		//init
		CPEStudioView::CPEStudioView()
		{
		}

		//free
		CPEStudioView::~CPEStudioView()
		{
		}

	/*************************************************************/
	/*			View Events										 */
	/*************************************************************/

		//ready to print
		BOOL CPEStudioView::OnPreparePrinting(CPrintInfo* pInfo)
		{
			return DoPreparePrinting(pInfo);
		}

		//printing started
		void CPEStudioView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
		{
		}

		//printing over
		void CPEStudioView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
		{
		}

		//draw
		void CPEStudioView::OnDraw(CDC* pDC)
		{
			CView::OnDraw(pDC);
		}

		//precreation(style) code
		BOOL CPEStudioView::PreCreateWindow(CREATESTRUCT& cs)
		{
			return CView::PreCreateWindow(cs);
		}

	/*************************************************************/
	/*			Debug Helper Functions							 */
	/*************************************************************/

		#ifdef _DEBUG
			void CPEStudioView::AssertValid() const
			{
				CView::AssertValid();
			}

			void CPEStudioView::Dump(CDumpContext& dc) const
			{
				CView::Dump(dc);
			}
		#endif


// CPEStudioView message handlers

IMPLEMENT_DYNCREATE(CPEStudioView, CView)

BEGIN_MESSAGE_MAP(CPEStudioView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()