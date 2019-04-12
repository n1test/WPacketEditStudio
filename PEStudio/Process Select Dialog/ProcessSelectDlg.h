/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  ProcessSelectDlg.h

  This file is part of the Packet Edit Studio Project.
  version: ALPHA

  *Distributed under the terms of the zlib/libpng public license.*
  
  Author: 

	Chris Smith (sourcemagik.com) - CPacketList
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

// The select dialog for setting the process to hook

	#pragma once
	#include "afxcmn.h"

	#include "FlatToolTipCtrl.h"

	/*************************************************************/
	/*			CProcessSelectDlg								 */
	/*															 */
	/*	This will display a list of processes with their icons   */
	/*  to choose from.											 */
	/*************************************************************/
	class CProcessSelectDlg : public CDialog
	{
		public:

			enum { IDD = IDD_SELECTPROCESS };

			/////////////////////////////
			//  Constructor / Destructor
			/////////////////////////////
			
				CProcessSelectDlg();

			/////////////////////////////
			//  Window Events
			/////////////////////////////
		protected:

				virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
				virtual BOOL OnInitDialog();
				virtual BOOL PreTranslateMessage(MSG *pMsg);

				DECLARE_MESSAGE_MAP()
				afx_msg void OnNMClickProcesslist(NMHDR *pNMHDR, LRESULT *pResult);
				afx_msg void OnNMDblClickProcesslist(NMHDR *pNMHDR, LRESULT *pResult);

			/////////////////////////////
			//  Internal Utility Functions
			/////////////////////////////
			
				//fired each time a window is found in the list
				void WindowFound(LPSTR lpWindow);

				//gets selected process
				int GetSelectedItem();
			
			/////////////////////////////
			//  Private Variables
			/////////////////////////////

				DWORD dpid;						//pid of selected process

			/////////////////////////////
			//  Controls
			/////////////////////////////

				CToolTipCtrlEx m_ProcessListTip;			//process list tool tip
				CToolTipCtrlEx m_ProcessNameTip;			//process name tool tip
				CToolTipCtrlEx m_ProcessIdTip;				//process id tool tip
				CToolTipCtrlEx m_ProcessEXESizeTip;			//process size tool tip
				CToolTipCtrlEx m_ProcessOwnerTip;			//process owner tool tip
				CToolTipCtrlEx m_OpenProcessStatusTip;		//process status tool tip
				CListCtrl m_ProcessList;					//list of processes currently running
				CImageList m_Images;						//list which holds the icons for the programs
				CStatic m_ProcessNameText;					//text which shows the name of the selected process
				CStatic m_ProcessIdText;					//text which shows the process id of the program
				CStatic m_ProcessEXESizeText;				//text which shows the byte size of the program
				CStatic m_ProcessOwnerText;					//text which shows the owner of a process
				CStatic m_OpenProcessStatusText;			//text which shows whether the process could be opened
				CListBox m_WindowList;						//the list box containing windows associated with a process

	static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
};