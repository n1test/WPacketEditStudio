/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  OptionsDlg.h

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

// Main options window (has the tab selection on it)

	#pragma once
	#include "stdafx.h"

	//for fixing newline / line carriage content
	void fixeditdata(char *str);

	/*************************************************************/
	/*			COptionsDlg										 */
	/*															 */
	/*	The options dialog which loads the option sub pages.	 */
	/*************************************************************/
	class COptionsDlg : public CDialog
	{
		DECLARE_DYNAMIC(COptionsDlg)
		public:

			enum { IDD = IDD_OPTIONS };

			/////////////////////////////
			//  Constructor / Destructor
			/////////////////////////////
			
				COptionsDlg(CWnd* pParent = NULL);   // standard constructor

				virtual ~COptionsDlg();

			/////////////////////////////
			//  Specific Control Events
			/////////////////////////////

				void OnClickApply();
				void OnClickCancel();

				void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);

			/////////////////////////////
			//  Window Events
			/////////////////////////////
		protected:

				virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
				virtual void OnClose();

				DECLARE_MESSAGE_MAP()
				afx_msg LONG OnInitDialog ( UINT, LONG );

			/////////////////////////////
			//  Variable Access
			/////////////////////////////
		public:
			
				//set pointer to the main window
				void SetMainWindow(CMainFrame *lpWindow);

			/////////////////////////////
			//  Internal Utility Functions
			/////////////////////////////
		protected:

				//allow program to continue executing
				void EnableMainWindow();

				//check if the options are valid
				bool CheckVars();

			/////////////////////////////
			//  Private Variables
			/////////////////////////////

				CMainFrame *m_MainWindow;						//pointer to the main window object

			/////////////////////////////
			//  Controls
			/////////////////////////////

				CCustomTabCtrl m_TabMenu;						//the option tabs
				CSniffOptions m_SniffOptions;					//sniff options dialog
				CPHPOptions m_PHPOptions;						//php options dialog
	};
