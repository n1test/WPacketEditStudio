/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  SniffResultsDoc.cpp

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

// The code for handling sniff results documents


	#include "stdafx.h"

/*************************************************************/
/*			MFC Debug Code									 */
/*************************************************************/

	#ifdef _DEBUG
		#define new DEBUG_NEW
	#endif

/*************************************************************/
/*			CSniffResultsDoc								 */
/*															 */
/*	Data Sniffing Results MDI Document Interface			 */
/*************************************************************/

	/*************************************************************/
	/*			Constructor / Destructor						 */
	/*************************************************************/

		//free
		CSniffResultsDoc::CSniffResultsDoc()
		{
		}

		//free
		CSniffResultsDoc::~CSniffResultsDoc()
		{
		}

	/*************************************************************/
	/*			Document Events									 */
	/*************************************************************/

		//new document opened
		BOOL CSniffResultsDoc::OnNewDocument()
		{
			if (!CDocument::OnNewDocument())
				return FALSE;

			return TRUE;
		}

		//document closed
		void CSniffResultsDoc::OnCloseDocument()
		{
			PESApp.DestroyHook(m_hHookHandle);
			CDocument::OnCloseDocument();
		}

		//importing / exporting document
		void CSniffResultsDoc::Serialize(CArchive& ar)
		{
			if (ar.IsStoring())
			{
			}
			else
			{
			}
		}

		//sets the document's window title
		void CSniffResultsDoc::SetTitle(LPCTSTR lpszTitle)
		{
			char tmp[MAX_PATH];
			sprintf_s(tmp, MAX_PATH, "Results (%s) %i", PESApp.GetHookExe(), PESApp.GetTimesHooked());
			CDocument::SetTitle(tmp);
		}

	/*************************************************************/
	/*			Variable Access									 */
	/*************************************************************/

		void CSniffResultsDoc::SetHookHandle(HOOK_HANDLE hHook)
		{
			m_hHookHandle = hHook;
		}

	/*************************************************************/
	/*			Debug Helper Functions							 */
	/*************************************************************/

		#ifdef _DEBUG
			void CSniffResultsDoc::AssertValid() const
			{
				CDocument::AssertValid();
			}

			void CSniffResultsDoc::Dump(CDumpContext& dc) const
			{
				CDocument::Dump(dc);
			}
		#endif


/*************************************************************/
/*			CSniffResultsDoc Message Map					 */
/*************************************************************/

IMPLEMENT_DYNCREATE(CSniffResultsDoc, CDocument)

BEGIN_MESSAGE_MAP(CSniffResultsDoc, CDocument)
END_MESSAGE_MAP()
