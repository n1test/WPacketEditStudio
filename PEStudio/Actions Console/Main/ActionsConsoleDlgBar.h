/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  AboutDlg.h

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

//this is the console with the siniffing information in the upper
//left hand corner

	#pragma once

	#include "EnBitmap.h"
	#include "CustomTabCtrl.h"

	/*************************************************************/
	/*			CActionsConsoleDlgBar							 */
	/*															 */
	/*	The dialog for handling packet actions (Below the trace  */
	/*  console).												 */
	/*************************************************************/
	class CActionsConsoleDlgBar : public CDialogBar
	{
		DECLARE_DYNAMIC(CActionsConsoleDlgBar)
		public:

			enum { IDD = IDD_CONSOLE };

			/////////////////////////////
			//  Constructor / Destructor
			/////////////////////////////

				CActionsConsoleDlgBar();

				virtual ~CActionsConsoleDlgBar();

			/////////////////////////////
			//  Specific Control Events
			/////////////////////////////

				void OnClickAdd();
				void OnClickStart();
				void OnClickStop();
				void OnClickDelete();

				void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);

			/////////////////////////////
			//  Window Events
			/////////////////////////////
		protected:

				virtual void DoDataExchange(CDataExchange* pDX);

				DECLARE_MESSAGE_MAP()
				afx_msg LONG OnInitDialog ( UINT, LONG );
				afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);


			/////////////////////////////
			//  Controls
			/////////////////////////////

			
				CToolBar24 m_ActionToolBar;						//The bar with the four script option buttons
				CCustomTabCtrl m_TabMenu;						//The tab menu at the bottom of the dialog
				CScriptsDlg m_ScriptsDialog;					//The child window with the script list
	};
