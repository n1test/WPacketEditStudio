/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  MainFrm.h

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

// The main window, handles the toolbar buttons, ipc events and gui
// loading


	#pragma once

	/*************************************************************/
	/*			CMainFrame										 */
	/*															 */
	/*	The main form which handles all toolbar / dialog bar	 */
	/*	creation and program initialization code.				 */
	/*************************************************************/
	class CMainFrame : public CMDIFrameWnd
	{
		DECLARE_DYNAMIC(CMainFrame)
		public:

			/////////////////////////////
			//  Constructor / Destructor
			/////////////////////////////

				CMainFrame();

				virtual ~CMainFrame();

			/////////////////////////////
			//  General Button Events
			/////////////////////////////

				void OnUpdateOPButton(CCmdUI *pCmdUI);
				
				template <class DialogClass>
				void UpdateView(DialogClass &dlg, UINT menu_res, UINT toolbar_res);

			/////////////////////////////
			//  Specific Button Events
			/////////////////////////////

				void OnViewMainToolbar();
				void OnViewTraceConsole();
				void OnViewActionsConsole();
				void OnViewCalculator();
				void OnViewResultsWindow();
				
				void OnProcessSelectButtonDown();
				void OnProcessDropDown(NMHDR* pnmtb, LRESULT *plr);
				void OnToolsOptions();
				void OnPHPScripts();
				void OnFindBuffer();
				void OnAbout();

			/////////////////////////////
			//  Window Events
			/////////////////////////////

				virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
				virtual void OnUpdateFrameTitle(BOOL bAddToTitle);

		protected:
				DECLARE_MESSAGE_MAP()
				afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

			/////////////////////////////
			//  Hook Events
			/////////////////////////////
		public:

				//when the user hits the stop button
				void OnDoneSniffing(CPacketBuffer &PacketBuffer);

				//when the sniffer has encountered an error
				void OnSniffError(CPacketBuffer &PacketBuffer);

				//when new sniff data arrives
				void OnSniffUpdate(CPacketBuffer &PacketBuffer);

		private:

			/////////////////////////////
			//  Creation Helper Functions
			/////////////////////////////

				bool CreateToolbars();
				void InitToolbars();
				void InitMenu();
				void DockToolbars();

		public:
			/////////////////////////////
			//  Public Control Functions
			/////////////////////////////

				//the bottom toolbar packet view
				void LoadPacketView(int i, HOOK_HANDLE hHook);

				//the bottom toolbar php results view
				void LoadPHPResultsView(LPSTR lpPath);

			/////////////////////////////
			//  IPC Handling
			/////////////////////////////

				//updates children with new infp
				void OnIPCDataArrival(PEStudioIPC *data);

			/////////////////////////////
			//  Debug Helper Functions
			/////////////////////////////

				#ifdef _DEBUG
					virtual void AssertValid() const;
					virtual void Dump(CDumpContext& dc) const;
				#endif

		protected:
			/////////////////////////////
			//  Controls
			/////////////////////////////

				CStatusBar m_wndStatusBar;							//the status bar
				CToolBar24 m_wndToolBar;							//the document bar
				CToolBar24 m_wndSnifferToolBar;						//the main toolbar
				CTraceConsoleDlgBar m_TraceDlgBar;					//the trace console
				CActionsConsoleDlgBar m_ActionsDlgBar;				//the actions console
				CCalculatorDlgBar m_CalculatorDlgBar;				//the calculator console
				CMDITabs m_wndMDITabs;								//for mdi tabs
				CSizingTabCtrlBar m_ResultsWnd;						//the bottom toolbar

			/////////////////////////////
			//  Private Variables
			/////////////////////////////
		private:

				CHtmlView *m_pPHPResultsView;						//pointer to the bottom toolbar's php results view
				CRichEditView *m_pSelPacketView;					//pointer to the bottom toolbar's packet view

				BOOL bShowOptions;									//show options on load?
	};


