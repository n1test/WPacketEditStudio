/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  MainFrm.cpp

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

// The main window, handles the toolbar buttons, ipc events and gui
// loading

#include "stdafx.h"

/*************************************************************/
/*			Option Launching Thread							 */
/*	(called upon invalid configuration on startup)			 */
/*************************************************************/

	DWORD WINAPI LaunchOptions(LPVOID arg)
	{
		CMainFrame *ptr;

		ptr = (CMainFrame *) arg;

		Sleep(500);
		COptionsDlg optionsDlg;
		optionsDlg.DoModal();

		return 0;
	}

/*************************************************************/
/*			MFC Debug Code									 */
/*************************************************************/
	#ifdef _DEBUG
		#define new DEBUG_NEW
	#endif

/*************************************************************/
/*			Status Bar Indicators							 */
/*************************************************************/
static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/*************************************************************/
/*			CMainFrame										 */
/*															 */
/*	The main form which handles all toolbar / dialog bar	 */
/*	creation and program initialization code.				 */
/*************************************************************/

	/*************************************************************/
	/*			Constructor / Destructor						 */
	/*************************************************************/

		//init
		CMainFrame::CMainFrame()
		{
		}

		//free
		CMainFrame::~CMainFrame()
		{
		}

	/*************************************************************/
	/*			General Button Events							 */
	/*************************************************************/

		//enables a button
		void CMainFrame::OnUpdateOPButton(CCmdUI *pCmdUI)
		{
			if(pCmdUI->m_nID == ID_SNIFFER_PAUSE || pCmdUI->m_nID == ID_SNIFFER_STOP )
				pCmdUI->Enable(m_TraceDlgBar.GetSnifferButtonState());
			else
				pCmdUI->Enable(TRUE);
		}

		//a function template for handling the view menu
		//and check buttons to show / hide the dialog / toolbar
		//only supply the toolbar_res argument if there is a toolbar
		//view button associated with the dialog
		template <class DialogClass>
		void CMainFrame::UpdateView (DialogClass &dlg, UINT menu_res, UINT toolbar_res=0)
		{
			CMenu *pMenu;
			UINT state;

			pMenu = GetMenu();
			state = pMenu->GetMenuState(menu_res, MF_BYCOMMAND);

			if(state == 0xFFFFFFFF)
				return;

			if (state & MF_CHECKED)
			{
				if(toolbar_res) m_wndSnifferToolBar.GetToolBarCtrl().CheckButton(toolbar_res, FALSE);
				pMenu->CheckMenuItem(menu_res, MF_UNCHECKED);
				ShowControlBar(&dlg, FALSE, FALSE);
			}
			else
			{
				if(toolbar_res) m_wndSnifferToolBar.GetToolBarCtrl().CheckButton(toolbar_res, TRUE);
				pMenu->CheckMenuItem(menu_res, MF_CHECKED);
				ShowControlBar(&dlg, TRUE, FALSE);
			}
			return;
		}

	/*************************************************************/
	/*			Specific Button Events							 */
	/*************************************************************/

		//menu -> view or toolbar view button clicks
		//use the "UpdateView" function template for all
		//view handling.
		void CMainFrame::OnViewMainToolbar() { UpdateView <CToolBar24> (m_wndSnifferToolBar, ID_VIEW_MAINTOOLBAR); }
		void CMainFrame::OnViewTraceConsole() { UpdateView <CTraceConsoleDlgBar> (m_TraceDlgBar, ID_VIEW_TRACECONSOLE, ID_TRACE_CONSOLE); }
		void CMainFrame::OnViewActionsConsole(){ UpdateView <CActionsConsoleDlgBar> (m_ActionsDlgBar, ID_VIEW_ACTIONSCONSOLE, ID_ACTIONS_CONSOLE); }
		void CMainFrame::OnViewCalculator() { UpdateView <CCalculatorDlgBar> (m_CalculatorDlgBar, ID_VIEW_CALCULATOR, ID_CALCULATOR); }
		void CMainFrame::OnViewResultsWindow() { UpdateView <CSizingTabCtrlBar> (m_ResultsWnd, ID_VIEW_RESULTS_WINDOW); }

		//process select button down
		void CMainFrame::OnProcessSelectButtonDown()
		{
			//show the open process dialog
			CProcessSelectDlg ProcessSelectDlg;
			ProcessSelectDlg.DoModal();
		}

		//handles the creation of the process select menu
		//when the drop down arrow is clicked.
		void CMainFrame::OnProcessDropDown(NMHDR* pnmtb1, LRESULT *plr)
		{
			NMTOOLBAR *pnmtb = (NMTOOLBAR*)pnmtb1;
			HANDLE hSnapshot;
			PROCESSENTRY32 uProcess;
			LONG rProcessFound;
			CMenu pPopup;
			CRect rc;
			int	i;

			// make sure it is a the
			// open process drop down
			switch (pnmtb->iItem)
			{
				case ID_OPEN_PROCESS:
					break;
				default:
					return;
			}
	
			// load and display popup menu
			CProcessList <CPESHookManager, CMainFrame> *ProcList = new CProcessList <CPESHookManager, CMainFrame> (PESApp.GetHookMgr(), *this);

			pPopup.CreatePopupMenu();

			i = 1;

			// build a list of open processes

			hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			uProcess.dwSize = sizeof(uProcess);

			rProcessFound = (LONG)Process32First(hSnapshot, &uProcess);
			rProcessFound = (LONG)Process32Next(hSnapshot, &uProcess);
			rProcessFound = (LONG)Process32Next(hSnapshot, &uProcess);
			while(rProcessFound)
			{
				if(_stricmp(uProcess.szExeFile, "pestudio.exe")) // we dont want to hook our own exe
				{
					// add the process to our menu list
					ProcList->AddProcess(i, uProcess.szExeFile, uProcess.th32ProcessID);
					pPopup.AppendMenuA(MF_STRING, i, uProcess.szExeFile);
					++i;
				}
				rProcessFound = (LONG)Process32Next(hSnapshot, &uProcess);
			}

			CloseHandle(hSnapshot);
	
			// draw the menu
			::SendMessage(m_wndSnifferToolBar.m_hWnd, TB_GETRECT, pnmtb->iItem, (LPARAM)&rc);
			m_wndSnifferToolBar.ClientToScreen(&rc);

			// get the user selection
			// then free the popup menu
			int Selection = pPopup.TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL | TPM_NONOTIFY | TPM_RETURNCMD,
			rc.left, rc.bottom, this, &rc);
			pPopup.DestroyMenu();

			// set the process and free the process list
			ProcList->SetHook(Selection);
			delete ProcList;
		}

		/****************************************************************/

		//options button down
		void CMainFrame::OnToolsOptions()
		{
			//show options dialog
			COptionsDlg optionsDlg;
			optionsDlg.DoModal();
		}

		//php button down
		void CMainFrame::OnPHPScripts()
		{
			//show add script dialog
			m_ActionsDlgBar.OnClickAdd();
		}

		//find buffer button down
		void CMainFrame::OnFindBuffer()
		{
			// TODO: add this code
			// This feature will freeze the target process at it's next
			// winsock call and search its memory space for the packet
			// buffer of the current call
			MessageBox("This feature has not been implimented yet.", "Packet Edit Studio", MB_OK);
		}

		void CMainFrame::OnAbout()
		{
			CAboutDlg aboutDlg;
			aboutDlg.DoModal();
		}

	/*************************************************************/
	/*			Window Events									 */
	/*************************************************************/

		//precreation(style) code
		BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
		{
			if( !CMDIFrameWnd::PreCreateWindow(cs) )
				return FALSE;

			return TRUE;
		}

		//window creation
		int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
		{
			bShowOptions = !PESApp.IsConfigValid();

			if(bShowOptions)
			{
				this->EnableWindow(FALSE);
				CreateThread(NULL, 0, LaunchOptions, (LPVOID)this, 0, NULL);
			}

			PESApp.SetLoadPHP(false);

			if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
				return -1;

			if(!CreateToolbars())
				return -1;

			// CMainFrame::InitToolbars()
			InitToolbars();

			// CMainFrame::InitMenu()
			InitMenu();

			// CMainFrame::DockToolbars()
			DockToolbars();

			// set up the MDI tabs
			m_wndMDITabs.Create(this, MT_TOP|MT_IMAGES);

			return 0;
		}

		//required for mdi to update the title
		void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
		{
			CMDIFrameWnd::OnUpdateFrameTitle(bAddToTitle);
			m_wndMDITabs.Update(); // sync the mditabctrl with all views
		}

	/*************************************************************/
	/*			Hook Events										 */
	/*************************************************************/
	
		//data recieved
		void CMainFrame::OnSniffUpdate(CPacketBuffer &PacketBuffer)
		{
			m_TraceDlgBar.UpdateConsole(PacketBuffer);
		}

		//error occurred
		void CMainFrame::OnSniffError(CPacketBuffer &PacketBuffer)
		{
		}

	/*************************************************************/
	/*			Creation Helper Functions						 */
	/*************************************************************/

		//creates the toolbar / dialog bar controls
		bool CMainFrame::CreateToolbars()
		{
			CMainFrame::ShowWindow(SW_SHOWMAXIMIZED);
	
			// document toolbar
			if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
				| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
				!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
				return false;

			// top tool bar
			if (!m_wndSnifferToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
				!m_wndSnifferToolBar.LoadToolBar(IDR_SNIFFTOOL))
				return false;

			// status bar
			if (!m_wndStatusBar.Create(this) ||
				!m_wndStatusBar.SetIndicators(indicators,
				sizeof(indicators)/sizeof(UINT)))
				return false;

			// trace dialog bar
			if (!m_TraceDlgBar.Create(this, IDD_TRACECONSOLE, CBRS_LEFT,
				IDD_TRACECONSOLE))
				return false;

			// action dialog bar
			if (!m_ActionsDlgBar.Create(this, IDD_TRACECONSOLE, CBRS_LEFT,
				IDD_TRACECONSOLE))
				return false;

			// calculator dialog bar
			if (!m_CalculatorDlgBar.Create(this, IDD_CALCULATOR, CBRS_LEFT, IDD_CALCULATOR))
				return false;

			// results window
			if (!m_ResultsWnd.Create(this, CSize(200, 200), IDW_PROP_BAR))
				return false;

			return true;
		}

		//loads the toolbar styles / data
		void CMainFrame::InitToolbars()
		{
			CFont font;
			TBBUTTONINFO tbi;

			// load the 24 bit images
			m_wndToolBar.SetFullColorImage(IDR_MAINFRAME24, RGB(255, 0, 255), 16, 16);
			m_wndSnifferToolBar.SetFullColorImage(IDR_SNIFFTOOL24, RGB(255, 0, 255), 32, 32);

			// allow process list drop down menu
			m_wndSnifferToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);

			// set the main toolbar button styles
			DWORD dwStyle = m_wndSnifferToolBar.GetButtonStyle(m_wndSnifferToolBar.CommandToIndex(ID_OPEN_PROCESS));
			m_wndSnifferToolBar.SetButtonStyle(m_wndSnifferToolBar.CommandToIndex(ID_OPEN_PROCESS), dwStyle | TBSTYLE_DROPDOWN);
			m_wndSnifferToolBar.SetButtonStyle(m_wndSnifferToolBar.CommandToIndex(ID_TRACE_CONSOLE), dwStyle | TBBS_CHECKBOX);
			m_wndSnifferToolBar.SetButtonStyle(m_wndSnifferToolBar.CommandToIndex(ID_ACTIONS_CONSOLE), dwStyle | TBBS_CHECKBOX);
			m_wndSnifferToolBar.SetButtonStyle(m_wndSnifferToolBar.CommandToIndex(ID_CALCULATOR), dwStyle | TBBS_CHECKBOX);

			// set main toolbar text
			m_wndSnifferToolBar.SetButtonText(0, "    Open Process");
			m_wndSnifferToolBar.SetButtonText(1, "    Trace Console");
			m_wndSnifferToolBar.SetButtonText(2, "    Action Console");
			m_wndSnifferToolBar.SetButtonText(3, "    Scripts");
			m_wndSnifferToolBar.SetButtonText(4, "    Find Buffer");
			m_wndSnifferToolBar.SetButtonText(5, "    Calculator");
			m_wndSnifferToolBar.SetButtonText(6, "    Options");
			m_wndSnifferToolBar.SetSizes(CSize(92,62),CSize(16,36));

			// set the button states on the toolbar
			CToolBarCtrl& tbCtrl = m_wndSnifferToolBar.GetToolBarCtrl();
			tbCtrl.CheckButton(ID_TRACE_CONSOLE, !tbCtrl.IsButtonChecked(ID_TRACE_CONSOLE));
			tbCtrl.CheckButton(ID_ACTIONS_CONSOLE, !tbCtrl.IsButtonChecked(ID_ACTIONS_CONSOLE));
			tbCtrl.CheckButton(ID_CALCULATOR, !tbCtrl.IsButtonChecked(ID_CALCULATOR));

			// set up the drop down menu for opening processes
			tbi.dwMask= TBIF_STYLE;
			tbi.cbSize= sizeof(TBBUTTONINFO);
			m_wndToolBar.GetToolBarCtrl().GetButtonInfo(ID_OPEN_PROCESS, &tbi);
			tbi.fsStyle |= TBSTYLE_DROPDOWN;
			m_wndToolBar.GetToolBarCtrl().SetButtonInfo(ID_OPEN_PROCESS, &tbi);

			// set the dialog bar styles
			m_TraceDlgBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
			CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

			m_ActionsDlgBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
			CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

			m_CalculatorDlgBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
			CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

			m_ResultsWnd.SetBarStyle(m_wndToolBar.GetBarStyle() |
				CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

			// set up the results window
			m_ResultsWnd.AddView("Script Results", RUNTIME_CLASS(CHtmlView));
			m_ResultsWnd.AddView("Packet View", RUNTIME_CLASS(CRichEditView));

			// store a handle to these views
			m_pPHPResultsView = (CHtmlView *)m_ResultsWnd.GetView(0);
			m_pSelPacketView = (CRichEditView *)m_ResultsWnd.GetView(1);

			// set up the packet view
			font.CreatePointFont(120, "Fixedsys");
			CRichEditCtrl &ctrl = m_pSelPacketView->GetRichEditCtrl();
			ctrl.SetFont(&font, TRUE);
			ctrl.SetReadOnly(TRUE);
			m_pSelPacketView->SetWindowTextA("no packet selected");

			// set the status bar info
			m_TraceDlgBar.SetStatusBar(&m_wndStatusBar);
		}

		//sets up initial docking info
		void CMainFrame::DockToolbars()
		{
			CRect rc;
			RECT r1;

			//init rect
			r1.top = 1;
			r1.left = 1;

			//enable docking for windows
			m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
			m_wndSnifferToolBar.EnableDocking(CBRS_ALIGN_ANY);
			m_TraceDlgBar.EnableDocking(CBRS_ALIGN_ANY);
			m_ActionsDlgBar.EnableDocking(CBRS_ALIGN_ANY);
			m_CalculatorDlgBar.EnableDocking(CBRS_ALIGN_ANY);
			m_ResultsWnd.EnableDocking(CBRS_ALIGN_ANY);

			//enable mfc docking
			EnableDocking(CBRS_ALIGN_ANY);

			//start docking
			DockControlBar(&m_ResultsWnd, AFX_IDW_DOCKBAR_BOTTOM);
			DockControlBar(&m_TraceDlgBar, AFX_IDW_DOCKBAR_LEFT);

			//you need to load the rectangle information in order
			//to dock the dialog underneath another toolbar / dialog
			RecalcLayout(TRUE);
			m_TraceDlgBar.GetWindowRect(rc);
			rc.OffsetRect(0, 1);

			DockControlBar(&m_ActionsDlgBar, AFX_IDW_DOCKBAR_LEFT, rc);
			
			//repeat for as many times as necessary
			m_ActionsDlgBar.GetWindowRect(rc);
			rc.OffsetRect(0, 1);

			//finish docking
			DockControlBar(&m_CalculatorDlgBar, AFX_IDW_DOCKBAR_RIGHT);
			DockControlBar(&m_wndSnifferToolBar, (UINT)AFX_IDW_DOCKBAR_TOP);
			DockControlBar(&m_wndToolBar, (UINT)AFX_IDW_DOCKBAR_TOP);
		}

		//sets up the menu
		void CMainFrame::InitMenu()
		{
			CMenu *pMenu;

			pMenu = GetMenu();
			pMenu->CheckMenuItem(ID_VIEW_MAINTOOLBAR, MF_CHECKED);
			pMenu->CheckMenuItem(ID_VIEW_TRACECONSOLE, MF_CHECKED);
			pMenu->CheckMenuItem(ID_VIEW_ACTIONSCONSOLE, MF_CHECKED);
			pMenu->CheckMenuItem(ID_VIEW_CALCULATOR, MF_CHECKED);
			pMenu->CheckMenuItem(ID_VIEW_RESULTS_WINDOW, MF_CHECKED);
		}

	/*************************************************************/
	/*			Public Control Functions						 */
	/*************************************************************/

		//macros for the packet view loading function
		#define _TMP_SIZE (STATIC_BUFFER_SIZE*30)

		#define _VIEW_ATTACH_BUFFER(x)			for(y = 0; y <= (int)strlen(x) ; ++y) \
												{ \
													if(x[y] == '\n' || x[y] == '\0') \
													{ \
														x[y] = '\0'; \
														strcat_s(tmp, _TMP_SIZE, x); \
														l = strlen(x); \
														x = &x[y+1]; \
														break; \
													} \
												}

		#define _VIEW_ATTACH_CHAR(x)			t = strlen(tmp);\
												tmp[t] = x;\
												tmp[t+1] = '\0';

		//load packet into packet view
		void CMainFrame::LoadPacketView(int i, HOOK_HANDLE hHook)
		{
			static char octal[STATIC_BUFFER_SIZE*10],
						hex[STATIC_BUFFER_SIZE*10],
						ascii[STATIC_BUFFER_SIZE*10],
						tmp[_TMP_SIZE],
						*octal_ptr,
						*hex_ptr,
						*ascii_ptr;
			unsigned char *p;
			DWORD dwSize;
			int z, y;
			size_t t, l;

			ClientIPCFunctor *pIPCObject = PESApp.GetIPCObject(hHook);
			if(!pIPCObject)
				return;

			CPacketBuffer &PacketBuffer = pIPCObject->GetPacketBuffer();

			p = PacketBuffer.GetPacketData(i, &dwSize);

			full_dump(octal, hex, ascii, p, dwSize);
			tmp[0] = '\0';

			octal_ptr = octal;
			hex_ptr = hex;
			ascii_ptr = ascii;
			for(z = 0; z < (int)strlen(octal); ++z)
			{
				if(octal[z] == '\n')
				{
					octal[z] = '\0';
					strcat_s(tmp, _TMP_SIZE, octal_ptr);

					_VIEW_ATTACH_CHAR('\t');
					_VIEW_ATTACH_BUFFER(hex_ptr);

					while(l < 50)
					{
						_VIEW_ATTACH_CHAR(' ');
						++l;
					}

					_VIEW_ATTACH_CHAR('\t');
					_VIEW_ATTACH_BUFFER(ascii_ptr);
					_VIEW_ATTACH_CHAR('\n');

					octal[z] = '\n';
					octal_ptr = &octal[z+1];
				}
			}

			m_pSelPacketView->SetWindowTextA(tmp);
		}

		//these macros are no longer needed
		#undef _TMP_SIZE
		#undef _VIEW_ATTACH_BUFFER
		#undef _VIEW_ATTACH_CHAR

		//load output html file
		void CMainFrame::LoadPHPResultsView(LPSTR lpPath)
		{
			m_pPHPResultsView->Navigate2(lpPath);
		}

	/*************************************************************/
	/*			Debug Helper Functions							 */
	/*************************************************************/

		#ifdef _DEBUG
			void CMainFrame::AssertValid() const
			{
				CMDIFrameWnd::AssertValid();
			}

			void CMainFrame::Dump(CDumpContext& dc) const
			{
				CMDIFrameWnd::Dump(dc);
			}
		#endif

/*************************************************************/
/*			CMainFrame Message Map							 */
/*************************************************************/
IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()

	/* TOOLBARS */

		/***********MAIN TOOLBAR***********/
			//Enable main toolbar buttons
			ON_UPDATE_COMMAND_UI(ID_OPEN_PROCESS, &CMainFrame::OnUpdateOPButton)
			ON_UPDATE_COMMAND_UI(ID_TRACE_CONSOLE, &CMainFrame::OnUpdateOPButton)
			ON_UPDATE_COMMAND_UI(ID_ACTIONS_CONSOLE, &CMainFrame::OnUpdateOPButton)
			ON_UPDATE_COMMAND_UI(ID_DATASCRIPTS, &CMainFrame::OnUpdateOPButton)
			ON_UPDATE_COMMAND_UI(ID_FINDDATABUFFER, &CMainFrame::OnUpdateOPButton)
			ON_UPDATE_COMMAND_UI(ID_CALCULATOR, &CMainFrame::OnUpdateOPButton)
			ON_UPDATE_COMMAND_UI(ID_TOOLS_OPTIONS, &CMainFrame::OnUpdateOPButton)

			//Handle main toolbar clicks
			ON_COMMAND(ID_OPEN_PROCESS, &CMainFrame::OnProcessSelectButtonDown)
			ON_COMMAND(ID_TRACE_CONSOLE, &CMainFrame::OnViewTraceConsole)
			ON_COMMAND(ID_ACTIONS_CONSOLE, &CMainFrame::OnViewActionsConsole)
			ON_COMMAND(ID_DATASCRIPTS, &CMainFrame::OnPHPScripts)
			ON_COMMAND(ID_FINDDATABUFFER, &CMainFrame::OnFindBuffer)
			ON_COMMAND(ID_CALCULATOR, &CMainFrame::OnViewCalculator)
			ON_COMMAND(ID_OPTIONS, &CMainFrame::OnToolsOptions)

			//Handles drop down menus on the toolbar
			ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, &CMainFrame::OnProcessDropDown)
		/**********************************/

		/***********Sniff Toolbar***********/
			//Enable sniff toolbar buttons
			ON_UPDATE_COMMAND_UI(ID_SNIFFER_START, &CMainFrame::OnUpdateOPButton)
			ON_UPDATE_COMMAND_UI(ID_SNIFFER_PAUSE, &CMainFrame::OnUpdateOPButton)
			ON_UPDATE_COMMAND_UI(ID_SNIFFER_STOP, &CMainFrame::OnUpdateOPButton)

			//Clicks are handled in Dialogs/Trace Console/Main/TraceConsoleDlgBar.cpp
		/**********************************/

		/***********Actions Toolbar***********/
			//Enable actions toolbar buttons
			ON_UPDATE_COMMAND_UI(ID_SCRIPTS_START, &CMainFrame::OnUpdateOPButton)
			ON_UPDATE_COMMAND_UI(ID_SCRIPTS_STOP, &CMainFrame::OnUpdateOPButton)
			ON_UPDATE_COMMAND_UI(ID_SCRIPTS_ADD, &CMainFrame::OnUpdateOPButton)
			ON_UPDATE_COMMAND_UI(ID_SCRIPTS_DELETE, &CMainFrame::OnUpdateOPButton)

			//Clicks are handled in Dialogs/Actions Console/Main/ActionsConsoleDlgBar.cpp
		/**********************************/

	/* MENU */

		/***********View Menu***********/
		ON_COMMAND(ID_VIEW_MAINTOOLBAR, &CMainFrame::OnViewMainToolbar)
		ON_COMMAND(ID_VIEW_ACTIONSCONSOLE, &CMainFrame::OnViewActionsConsole)
		ON_COMMAND(ID_VIEW_TRACECONSOLE, &CMainFrame::OnViewTraceConsole)
		ON_COMMAND(ID_VIEW_CALCULATOR, &CMainFrame::OnViewCalculator)
		ON_COMMAND(ID_VIEW_RESULTS_WINDOW, &CMainFrame::OnViewResultsWindow)
		/**********************************/

		/***********Tools Menu***********/
			ON_COMMAND(ID_TOOLS_OPTIONS, &CMainFrame::OnToolsOptions)
			ON_COMMAND(ID_TOOLS_APPLYDATAMETHOD, &CMainFrame::OnPHPScripts)
			ON_COMMAND(ID_TOOLS_FINDSTATICBUFFER, &CMainFrame::OnFindBuffer)
		/**********************************/

		ON_COMMAND(ID_APP_ABOUT, &CMainFrame::OnAbout)

END_MESSAGE_MAP()
