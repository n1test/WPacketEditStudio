/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  PHPResultsView.cpp

  This file is part of the Packet Edit Studio Project.
  version: ALPHA

  *Distributed under the terms of the zlib/libpng public license.*
  
  Author: 

	Chris Smith (sourcemagik.com) - CPHPResultsView
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

// PHP Results view class

	#include "stdafx.h"
	#include "PEStudio.h"
	#include "PHPResultsView.h"

/*************************************************************/
/*			CPHPResultsView									 */
/*															 */
/*	The view for the php results portion of the bottom		 */
/*  window.													 */
/*************************************************************/

	/*************************************************************/
	/*			Constructor / Destructor						 */
	/*************************************************************/

		//init
		CPHPResultsView::CPHPResultsView()
		{
		}

		//free
		CPHPResultsView::~CPHPResultsView()
		{
		}

	/*************************************************************/
	/*			View Events										 */
	/*************************************************************/

		//controls setup
		void CPHPResultsView::DoDataExchange(CDataExchange* pDX)
		{
			CHtmlView::DoDataExchange(pDX);
		}

	/*************************************************************/
	/*			Debug Helper Functions							 */
	/*************************************************************/

		#ifdef _DEBUG
			void CPHPResultsView::AssertValid() const
			{
				CHtmlView::AssertValid();
			}

			void CPHPResultsView::Dump(CDumpContext& dc) const
			{
				CHtmlView::Dump(dc);
			}
		#endif

/*************************************************************/
/*			CPHPResultsView Message Map						 */
/*************************************************************/

IMPLEMENT_DYNCREATE(CPHPResultsView, CHtmlView)

BEGIN_MESSAGE_MAP(CPHPResultsView, CHtmlView)
END_MESSAGE_MAP()