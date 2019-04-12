/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  OptionsDlg.cpp

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

	#include "stdafx.h"

	//fix newlines in variables
	void fixeditdata(char *str)
	{
		int i;
		for(i = 0 ; i < (int)strlen(str) ; ++i)
			if(str[i] == '\r' || str[i] == '\n')
				str[i] = '\0';
	}

/*************************************************************/
/*			COptionsDlg										 */
/*															 */
/*	The options dialog which loads the option sub pages.	 */
/*************************************************************/

	/*************************************************************/
	/*			Constructor / Destructor						 */
	/*************************************************************/

		//init
		COptionsDlg::COptionsDlg(CWnd* pParent /*=NULL*/)
			: CDialog(COptionsDlg::IDD, pParent)
		{
			m_MainWindow = NULL;
		}

		//free
		COptionsDlg::~COptionsDlg()
		{
		}

	/*************************************************************/
	/*			Specific Control Events							 */
	/*************************************************************/

		//save options and close dialog
		void COptionsDlg::OnClickApply()
		{
			PESApp.ConfigWriteValue("dll", m_SniffOptions.GetDllName());
			PESApp.ConfigWriteValue("method", itoa(m_SniffOptions.GetSniffMethod()));
			PESApp.ConfigWriteValue("buffersz", itoa(m_SniffOptions.GetBufferSize()));
			PESApp.ConfigWriteValue("packetsz", itoa(m_SniffOptions.GetPacketSize()));
			PESApp.ConfigWriteValue("phpver", m_PHPOptions.GetPHPVer());
			PESApp.ConfigWriteValue("logphp", itoa(m_PHPOptions.GetLogPHP()));

			if(CheckVars())
			{	
				PESApp.RefreshSettings();

				::EndDialog(GetSafeHwnd(), NULL);
				EnableMainWindow();
			}
		}

		//close dialog
		void COptionsDlg::OnClickCancel()
		{
			if(CheckVars())
			{
				::EndDialog(GetSafeHwnd(), NULL);
				EnableMainWindow();
			}
		}

		//options selection tab changed
		void COptionsDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
		{
			m_SniffOptions.ShowWindow(SW_HIDE);
			m_PHPOptions.ShowWindow(SW_HIDE);

			switch(m_TabMenu.GetCurSel())
			{
				// sniffer
				case 0:
					m_SniffOptions.ShowWindow(SW_SHOW);
					break;

				// php
				case 1:
					m_PHPOptions.ShowWindow(SW_SHOW);
					break;

				default:
					break;

			}
		}

	/*************************************************************/
	/*			Window Events									 */
	/*************************************************************/

		//set up controls
		void COptionsDlg::DoDataExchange(CDataExchange* pDX)
		{
			CDialog::DoDataExchange(pDX);
		}

		//dialog closing
		void COptionsDlg::OnClose()
		{
			EnableMainWindow();
			return CDialog::OnClose();
		}

		//init code
		LONG COptionsDlg::OnInitDialog ( UINT wParam, LONG lParam)
		{
			BOOL bRet = (BOOL)HandleInitDialog(wParam, lParam);

			m_TabMenu.Create(WS_VISIBLE|WS_CHILD|CTCS_AUTOHIDEBUTTONS|CTCS_TOOLTIPS|CTCS_TOP, 
						CRect(0,5,500,25), this, IDC_OPTIONS_TAB);

			m_TabMenu.InsertItem(0, "Sniffing");
			m_TabMenu.InsertItem(1, "PHP");;

			CDialog::SetWindowTextA("Packet Edit Studio Options");

			m_SniffOptions.Create(IDD_SNIFFINGOPS, this);
			m_PHPOptions.Create(IDD_PHPOPS, this);
			m_SniffOptions.ShowWindow(SW_SHOW);

			m_TabMenu.SetCurSel(0);

			return bRet;
		}

	/*************************************************************/
	/*			Variable Access									 */
	/*************************************************************/

		//give a pointer to the main window
		void COptionsDlg::SetMainWindow(CMainFrame *lpWindow)
		{
			m_MainWindow = lpWindow;
		}

	/*************************************************************/
	/*			Internal Utility Functions						 */
	/*************************************************************/

		//activates the main window (passed by SetMainWindow())
		void COptionsDlg::EnableMainWindow()
		{
			if(m_MainWindow)
			{
				m_MainWindow->EnableWindow(TRUE);
				m_MainWindow = NULL;
			}
		}

		//validates the config
		bool COptionsDlg::CheckVars()
		{
			bool bOk;
			int buffersize, packetsize;

			bOk = true;


			buffersize = PESApp.GetBufferSize();
			packetsize = PESApp.GetMaxPacketSize();

			if(buffersize < 100 || buffersize > 1048576)
				bOk = false;

			if(packetsize < 100 || packetsize > 10240)
				bOk = false;

			if(!bOk)
				MessageBox("ERROR: One or more of your configuration settings is invalid.", "Packet Edit Studio", MB_ICONERROR);

			return bOk;
		}

/*************************************************************/
/*			COptionsDlg Message Map							 */
/*************************************************************/

IMPLEMENT_DYNAMIC(COptionsDlg, CDialog)

BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog)
	ON_COMMAND(IDC_OPTIONS_OK, &COptionsDlg::OnClickApply)
	ON_COMMAND(IDC_OPTIONS_CANCEL, &COptionsDlg::OnClickCancel)
	ON_NOTIFY(CTCN_SELCHANGE, IDC_OPTIONS_TAB, OnSelchangeTab)
END_MESSAGE_MAP()


// COptionsDlg message handlers
