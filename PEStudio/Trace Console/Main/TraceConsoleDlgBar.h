/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  TraceConsoleDlgBar.h

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

// The parent window for the trace console (handles button / dialog creation)

	#pragma once

	#include "EnBitmap.h"
	#include "CustomTabCtrl.h"
	#include "TraceMeterDlg.h"
	#include "SniffDetailsDlg.h"

	/*************************************************************/
	/*			CTraceConsoleDlgBar								 */
	/*															 */
	/*	Dialog for create a MDI based sniffing UI with easy		 */
	/*  use and integration.									 */
	/*************************************************************/
	class CTraceConsoleDlgBar : public CDialogBar
	{
		DECLARE_DYNAMIC(CTraceConsoleDlgBar)
		public:

			enum { IDD = IDD_TRACECONSOLE };

			/////////////////////////////
			//  Constructor / Destructor
			/////////////////////////////

	
				CTraceConsoleDlgBar();

				virtual ~CTraceConsoleDlgBar();

			/////////////////////////////
			//  Specific Control Events
			/////////////////////////////

				void OnStartSniffButtonDown();
				void OnPauseSniffButtonDown();
				void OnStopSniffButtonDown();

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
			//  Console Management
			/////////////////////////////
		public:

				//update packet information
				void UpdateConsole(CPacketBuffer &PacketList);

			/////////////////////////////
			//  Variable Access
			/////////////////////////////

				//get the state of the sniffing (play) button
				BOOL GetSnifferButtonState();

				//set the status bar object pointer
				VOID SetStatusBar(CStatusBar *pStatusBar);

			/////////////////////////////
			//  Private Variables
			/////////////////////////////
		protected:

				BOOL m_SnoopBttnState;					//whether the sniffer should be active

			/////////////////////////////
			//  Controls
			/////////////////////////////

				CToolBar24 m_SniffToolBar;				//dialog bar with the command buttons
				CCustomTabCtrl m_TabMenu;				//the page selection menu
				CTraceMeterDlg m_Meters;				//the meter dialog
				CSniffDetailsDlg m_SniffDetailsDlg;		//the details dialog
				CSniffStatsDlg m_SniffStatsDlg;			//the stats dialog
				CSniffActivityDlg m_SniffActivityDlg;	//the graph dialog
				CSniffLogDlg m_SniffLogDlg;				//the logger dialog
				CStatusBar *m_StatusBar;				//pointer to the main status bar object
};
