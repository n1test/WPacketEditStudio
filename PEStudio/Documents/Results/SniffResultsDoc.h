/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  SniffResultsDoc.h

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

	#pragma once

	/*************************************************************/
	/*			CSniffResultsDoc								 */
	/*															 */
	/*	Data Sniffing Results MDI Document Interface			 */
	/*************************************************************/
	class CSniffResultsDoc : public CDocument
	{
		protected:
			DECLARE_DYNCREATE(CSniffResultsDoc)

			/////////////////////////////
			//  Constructor / Destructor
			/////////////////////////////

				CSniffResultsDoc();

		public:

				virtual ~CSniffResultsDoc();

			/////////////////////////////
			//  Document Events
			/////////////////////////////

				virtual BOOL OnNewDocument();
				virtual void OnCloseDocument();
				virtual void Serialize(CArchive& ar);
				virtual void SetTitle(LPCTSTR lpszTitle);

			/////////////////////////////
			//  Debug Helper Functions
			/////////////////////////////

				#ifdef _DEBUG
					virtual void AssertValid() const;
					virtual void Dump(CDumpContext& dc) const;
				#endif

			/////////////////////////////
			//  Variable Access
			/////////////////////////////

				void SetHookHandle(HOOK_HANDLE hHook);

			/////////////////////////////
			//  Private Variables
			/////////////////////////////
		protected:

				HOOK_HANDLE m_hHookHandle;					//the associated hook object handle

			/////////////////////////////
			//  Window Events
			/////////////////////////////

				DECLARE_MESSAGE_MAP()
	};


