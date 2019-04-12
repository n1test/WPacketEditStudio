/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  TraceConsoleDlgBar.cpp

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

	#include "stdafx.h"

/*************************************************************/
/*			CTraceConsoleDlgBar								 */
/*															 */
/*	Dialog for create a MDI based sniffing UI with easy		 */
/*  use and integration.									 */
/*************************************************************/

	/*************************************************************/
	/*			Constructor / Destructor						 */
	/*************************************************************/

		//init
		CTraceConsoleDlgBar::CTraceConsoleDlgBar()
			: CDialogBar()
		{
		}

		//free
		CTraceConsoleDlgBar::~CTraceConsoleDlgBar()
		{
		}
		
	/*************************************************************/
	/*			Specific Button Events							 */
	/*************************************************************/

		//start sniffing button clicked
		void CTraceConsoleDlgBar::OnStartSniffButtonDown()
		{

			// not already hooking: START HOOKING
			if(m_SniffToolBar.GetToolBarCtrl().IsButtonChecked(ID_SNIFFER_START))
			{
				m_SnoopBttnState = TRUE;

				// if it is not already hooking, start
				if(!PESApp.IsHooking())
				{
					if(!m_SnoopBttnState)
						OnStopSniffButtonDown();

					// try to start
					if(!PESApp.StartHooking())
					{
						m_StatusBar->SetPaneText(0, "Dll Injection Failed");
						m_SnoopBttnState = FALSE;
						m_SniffToolBar.SetButtonInfo(0, ID_SNIFFER_START, TBBS_CHECKBOX, 0); 
					}

					// failed to start
					else
					{
						m_SnoopBttnState = TRUE;
						m_SniffToolBar.GetToolBarCtrl().EnableButton(ID_SNIFFER_PAUSE, m_SnoopBttnState);
						m_SniffToolBar.GetToolBarCtrl().EnableButton(ID_SNIFFER_STOP, m_SnoopBttnState);

						m_SniffActivityDlg.StartGraphTimer();
					}
				}
			}

			// already hooking: STOP HOOKING
			else
				OnStopSniffButtonDown();
		}

		//pause sniffing button clicked
		void CTraceConsoleDlgBar::OnPauseSniffButtonDown()
		{
			// toggle
			if(m_SniffToolBar.GetToolBarCtrl().IsButtonChecked(ID_SNIFFER_PAUSE))
				PESApp.PauseHooking(true);
			else
				PESApp.PauseHooking(false);
		}

		//stop sniffing button clicked
		void CTraceConsoleDlgBar::OnStopSniffButtonDown()
		{

			// clean up if it was hooking
			if(m_SnoopBttnState)
			{
				m_SnoopBttnState = FALSE;
				m_SniffToolBar.GetToolBarCtrl().EnableButton(ID_SNIFFER_PAUSE, m_SnoopBttnState);
				m_SniffToolBar.GetToolBarCtrl().EnableButton(ID_SNIFFER_STOP, m_SnoopBttnState);
				m_SniffToolBar.GetToolBarCtrl().CheckButton(ID_SNIFFER_PAUSE, FALSE);
				m_SniffToolBar.SetButtonInfo(0, ID_SNIFFER_START, TBBS_CHECKBOX, 0);
			}

			// end hooking
			PESApp.StopHooking();

			// tell the graph to stop graphing
			m_SniffActivityDlg.EndGraphTimer();
		}

		void CTraceConsoleDlgBar::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
		{
			m_Meters.ShowWindow(SW_HIDE);
			m_SniffDetailsDlg.ShowWindow(SW_HIDE);
			m_SniffStatsDlg.ShowWindow(SW_HIDE);
			m_SniffActivityDlg.ShowWindow(SW_HIDE);
			m_SniffLogDlg.ShowWindow(SW_HIDE);

			switch(m_TabMenu.GetCurSel())
			{
				//guage
				case 0:
					m_Meters.ShowWindow(SW_SHOW);
					break;

				//details
				case 1:
					m_SniffDetailsDlg.ShowWindow(SW_SHOW);
					break;

				//stats
				case 2:
					m_SniffStatsDlg.ShowWindow(SW_SHOW);
					break;

				//activity
				case 3:
					m_SniffActivityDlg.ShowWindow(SW_SHOW);
					break;

				//log
				/*case 4:
					m_SniffLogDlg.ShowWindow(SW_SHOW);
					break;*/

				default:
					break;

			}
		}

	/*************************************************************/
	/*			Window Events									 */
	/*************************************************************/

		//set up controls
		void CTraceConsoleDlgBar::DoDataExchange(CDataExchange* pDX)
		{
			CDialogBar::DoDataExchange(pDX);
		}

		//init code
		LONG CTraceConsoleDlgBar::OnInitDialog ( UINT wParam, LONG lParam)
		{

			m_SnoopBttnState = FALSE;
			BOOL bRet = (BOOL)HandleInitDialog(wParam, lParam);

			// create sniff bar
			if (!UpdateData(FALSE))
			{
				TRACE0("Warning: UpdateData failed during dialog init.\n");
			}

			if(!m_SniffToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC  ) || !m_SniffToolBar.LoadToolBar(IDR_SNIFFERBAR))
			{
				TRACE0("Failed to create toolbar\n");
				return -1;      // fail to create
			}

			// create sub pages
			m_Meters.Create(IDD_METERBOX, this);
			m_SniffDetailsDlg.Create(IDD_SNIFFDETAILS, this);
			m_SniffStatsDlg.Create(IDD_SNIFFSTATS, this);
			m_SniffActivityDlg.Create(IDD_SNIFFACTIVITY, this);
			m_SniffLogDlg.Create(IDD_SNIFFLOG, this);
			m_Meters.ShowWindow(SW_SHOW);

			m_TabMenu.Create(WS_VISIBLE|WS_CHILD|CTCS_AUTOHIDEBUTTONS|CTCS_TOOLTIPS, 
						CRect(0,150,240,170), this, IDC_SNIFFERTAB);

			DWORD dwStyle = m_SniffToolBar.GetButtonStyle(m_SniffToolBar.CommandToIndex(ID_SNIFFER_START));
			m_SniffToolBar.SetButtonStyle(m_SniffToolBar.CommandToIndex(ID_SNIFFER_START), dwStyle | TBBS_CHECKBOX);
			m_SniffToolBar.SetButtonStyle(m_SniffToolBar.CommandToIndex(ID_SNIFFER_PAUSE), dwStyle | TBBS_CHECKBOX);

			m_SniffToolBar.SetFullColorImage(IDR_SNIFFERBAR24, RGB(255, 0, 255), 24, 24);
			RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST,0);
			m_SniffToolBar.GetToolBarCtrl().EnableButton(ID_SNIFFER_START, TRUE);

			// set up tab menu
			m_TabMenu.InsertItem(0,"Gauge");
			m_TabMenu.InsertItem(1,"Details");
			m_TabMenu.InsertItem(2,"Stats");
			m_TabMenu.InsertItem(3,"Activity");
			//m_TabMenu.InsertItem(4,"Log");

			m_TabMenu.SetItemTooltipText(0, "Buffer usage / Packets recieved");
			m_TabMenu.SetItemTooltipText(1, "API call count");
			m_TabMenu.SetItemTooltipText(2, "Statistics");
			m_TabMenu.SetItemTooltipText(3, "socket stream activity plot");
			//m_TabMenu.SetItemTooltipText(4, "Socket stream log");

			m_TabMenu.SetCurSel(0);

			return bRet;
		}

		//creation code
		int CTraceConsoleDlgBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
		{
			if (CDialogBar::OnCreate(lpCreateStruct) == -1)
				return -1;
			
			return 0;
		}

	/*************************************************************/
	/*			Console Management								 */
	/*************************************************************/

		//update the trace console with a packet buffer
		void CTraceConsoleDlgBar::UpdateConsole(CPacketBuffer &PacketList)
		{
			// basic info
			int PacketsParsed = PacketList.GetTotalPackets();
			int BufferFillSpace = PacketList.GetFillPercent();

			SniffDetails detailstruct;
			PacketList.FillDetailStruct(&detailstruct);

			// update sub pages
			m_SniffDetailsDlg.UpdateDetails(detailstruct);
			m_SniffStatsDlg.UpdateStats(PacketList.GetAvgSize(), PacketList.GetPacketsRecved(), PacketList.GetPacketsSent(), PacketList.GetTotalBytes());
			m_SniffActivityDlg.UpdateGraph();
			m_Meters.UpdateMeters(PacketsParsed, BufferFillSpace);
		}

	/*************************************************************/
	/*			Variable Access									 */
	/*************************************************************/

		//set a pointer to the main status bar object
		VOID CTraceConsoleDlgBar::SetStatusBar(CStatusBar *pStatusBar)
		{
			m_StatusBar = pStatusBar;
		}

		//whether the sniff button is down
		BOOL CTraceConsoleDlgBar::GetSnifferButtonState()
		{
			return m_SnoopBttnState;
		}

/*************************************************************/
/*			CTraceConsoleDlgBar Message Map					 */
/*************************************************************/

IMPLEMENT_DYNAMIC(CTraceConsoleDlgBar, CDialogBar)

BEGIN_MESSAGE_MAP(CTraceConsoleDlgBar, CDialogBar)
	ON_WM_CREATE()
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog)

	//sniff buttons
	ON_COMMAND(ID_SNIFFER_START, OnStartSniffButtonDown)
	ON_COMMAND(ID_SNIFFER_PAUSE, OnPauseSniffButtonDown)
	ON_COMMAND(ID_SNIFFER_STOP, OnStopSniffButtonDown)

	//tab menu
	ON_NOTIFY(CTCN_SELCHANGE, IDC_SNIFFERTAB, OnSelchangeTab)
END_MESSAGE_MAP()
