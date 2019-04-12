/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  PHPDoc.cpp

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

// The document code for handling php documents

	#include "stdafx.h"

/*************************************************************/
/*			MFC Debug Code									 */
/*************************************************************/

	#ifdef _DEBUG
		#define new DEBUG_NEW
	#endif

/*************************************************************/
/*			CPHPDoc											 */
/*															 */
/*	PHP MDI Document Interface								 */
/*************************************************************/

	/*************************************************************/
	/*			Constructor / Destructor						 */
	/*************************************************************/

		//init
		CPHPDoc::CPHPDoc()
		{
		}

		//free
		CPHPDoc::~CPHPDoc()
		{
		}

	/*************************************************************/
	/*			Document Events									 */
	/*************************************************************/

		//new document opened
		BOOL CPHPDoc::OnNewDocument()
		{
			if (!CDocument::OnNewDocument())
				return FALSE;

			return TRUE;
		}

		//importing / exporting document
		void CPHPDoc::Serialize(CArchive& ar)
		{
			POSITION pos = GetFirstViewPosition();
			CScintillaDemoView* pView = (CScintillaDemoView *)GetNextView(pos);

			// save
			if (ar.IsStoring())
			{
				int sz = (int)pView->GetTotalSize();
				char *tmp = new char [sz];
				pView->GetText(tmp, sz);
				ar.Write(tmp, (UINT)sz);
				delete [] tmp;
			}

			// load
			else
			{
				CString final = "";
				CString line;
		
				while(ar.ReadString(line))
					final += line + "\r\n";

				char *ptr = const_cast <char*> ((LPCTSTR) final);
				pView->SetText(ptr);
			}
		}

	/*************************************************************/
	/*			Constructor / Destructor						 */
	/*************************************************************/

		#ifdef _DEBUG
			void CPHPDoc::AssertValid() const
			{
				CDocument::AssertValid();
			}

			void CPHPDoc::Dump(CDumpContext& dc) const
			{
				CDocument::Dump(dc);
			}
		#endif

/*************************************************************/
/*			CPHPDoc Message Map								 */
/*************************************************************/

IMPLEMENT_DYNCREATE(CPHPDoc, CDocument)

BEGIN_MESSAGE_MAP(CPHPDoc, CDocument)
END_MESSAGE_MAP()
