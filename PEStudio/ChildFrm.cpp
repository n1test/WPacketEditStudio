/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  ChildFrm.cpp

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

// Child window for the main window

#include "stdafx.h"

/*************************************************************/
/*			MFC Debug Code									 */
/*************************************************************/

	#ifdef _DEBUG
		#define new DEBUG_NEW
	#endif

/*************************************************************/
/*			CChildFrame										 */
/*															 */
/*	This is a basic subclass of CMDIChildWnd.				 */
/*************************************************************/

	/*************************************************************/
	/*			Constructor / Destructor						 */
	/*************************************************************/

		//init
		CChildFrame::CChildFrame()
		{
		}

		//free
		CChildFrame::~CChildFrame()
		{
		}

	/*************************************************************/
	/*			Window Events									 */
	/*************************************************************/

		//precreation(style) code
		BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
		{
			if( !CMDIChildWnd::PreCreateWindow(cs) )
				return FALSE;

			return TRUE;
		}

	/*************************************************************/
	/*			Window Management								 */
	/*************************************************************/

		//enables the frame
		void CChildFrame::ActivateFrame(int nCmdShow)
		{
			if (nCmdShow == -1)
				nCmdShow = SW_SHOWMAXIMIZED;

			CMDIChildWnd::ActivateFrame(nCmdShow);
		}

	/*************************************************************/
	/*			Debug Helper Functions							 */
	/*************************************************************/

		#ifdef _DEBUG
			void CChildFrame::AssertValid() const
			{
				CMDIChildWnd::AssertValid();
			}

			void CChildFrame::Dump(CDumpContext& dc) const
			{
				CMDIChildWnd::Dump(dc);
			}

		#endif

/*************************************************************/
/*			CChildFrame Message Map							 */
/*************************************************************/

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
END_MESSAGE_MAP()

