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

	#include "stdafx.h"

/*************************************************************/
/*			File Size Function								 */
/*************************************************************/

	long get_file_size(std::ifstream &f)
	{
		long begin,end;
		f.seekg (0, std::ios::beg);
		begin = f.tellg();
		f.seekg (0, std::ios::end);
		end = f.tellg();
		f.seekg (0, std::ios::beg);
		return end-begin;
	}


/*************************************************************/
/*			CActionsConsoleDlgBar							 */
/*															 */
/*	The dialog for handling packet actions (Below the trace  */
/*  console).												 */
/*************************************************************/

	/*************************************************************/
	/*			Constructor / Destructor						 */
	/*************************************************************/

		//init
		CActionsConsoleDlgBar::CActionsConsoleDlgBar()
			: CDialogBar()
		{
		}

		//free
		CActionsConsoleDlgBar::~CActionsConsoleDlgBar()
		{
		}

	/*************************************************************/
	/*			Specific Control Events							 */
	/*************************************************************/

		//add script to the load list
		void CActionsConsoleDlgBar::OnClickAdd()
		{

			// create a open file dialog
			CFileDialog FileDlg(TRUE, ".php", NULL, 0, "PHP Files (*.php;*.php4;*.php5)|*.php;*.php4;*.php5|All Files (*.*)|*.*||");

			// load the file and check if it is valid then unload it
			if(FileDlg.DoModal() == IDOK)
			{
				std::ifstream f;
				
				f.open(FileDlg.GetFileName(), std::ios::in);
		
				if(!f.is_open())
				{
					MessageBox("ERROR: Could not open php script for parsing!", "Packet Edit Studio", MB_ICONERROR);
					f.close();
					return;
				}
		
				long sz = get_file_size(f);

				if(!sz)
				{
					MessageBox("ERROR: The script file is empty!", "Packet Edit Studio", MB_ICONERROR);
					f.close();
					return;
				}		

				char *buffer = new char [sz];

				f.read(buffer, sz);

				buffer[(int)sz-1] = '\0';

				m_ScriptsDialog.AddItem(buffer, FileDlg.GetPathName().GetBuffer(MAX_PATH));

				delete [] buffer;
				f.close();
			}
		}

		//enable / disable php loading
		void CActionsConsoleDlgBar::OnClickStart()
		{
			if(m_ActionToolBar.GetToolBarCtrl().IsButtonChecked(ID_SCRIPTS_START))
				PESApp.SetLoadPHP(true);
			else
				PESApp.SetLoadPHP(false);
		}

		//disable php loading
		void CActionsConsoleDlgBar::OnClickStop()
		{
			PESApp.SetLoadPHP(false);
			m_ActionToolBar.SetButtonInfo(0, ID_SCRIPTS_START, TBBS_CHECKBOX, 0);
		}

		//remove item from the list
		void CActionsConsoleDlgBar::OnClickDelete()
		{
			m_ScriptsDialog.RemoveItem();
		}

	/*************************************************************/
	/*			Window Events									 */
	/*************************************************************/

		//set up controls
		void CActionsConsoleDlgBar::DoDataExchange(CDataExchange* pDX)
		{
			CDialogBar::DoDataExchange(pDX);
		}

		//init code
		LONG CActionsConsoleDlgBar::OnInitDialog ( UINT wParam, LONG lParam)
		{
			BOOL bRet = (BOOL)HandleInitDialog(wParam, lParam);
	
			if (!UpdateData(FALSE))
			{
				TRACE0("Warning: UpdateData failed during dialog init.\n");
			}

			// load scripts dialog
			if(!m_ActionToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC  ) || !m_ActionToolBar.LoadToolBar(IDR_ACTIONBAR))
			{
				TRACE0("Failed to create toolbar\n");
				return -1;      // fail to create
			}

			m_ScriptsDialog.Create(IDD_SCRIPTSDLG, this);
			m_ScriptsDialog.ShowWindow(SW_SHOW);

			// set up tabs
			m_TabMenu.Create(WS_VISIBLE|WS_CHILD|CTCS_AUTOHIDEBUTTONS|CTCS_TOOLTIPS, 
					    CRect(0,150,240,170), this, IDC_SNIFFERTAB);

			m_TabMenu.InsertItem(0,"Scripts");

			m_TabMenu.SetItemTooltipText(0, "Apply a Packet Edit Studio PHP script");

			//set up action bar
			m_ActionToolBar.SetFullColorImage(IDB_ACTIONBAR24, RGB(255, 0, 255), 24, 24);
			RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST,0);
			m_ActionToolBar.GetToolBarCtrl().EnableButton(ID_SCRIPTS_START, TRUE);
			DWORD dwStyle = m_ActionToolBar.GetButtonStyle(m_ActionToolBar.CommandToIndex(ID_SCRIPTS_START));
			m_ActionToolBar.SetButtonStyle(m_ActionToolBar.CommandToIndex(ID_SCRIPTS_START), dwStyle | TBBS_CHECKBOX);

			m_TabMenu.SetCurSel(0);

			return bRet;
		}

		//creation code
		int CActionsConsoleDlgBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
		{
			if (CDialogBar::OnCreate(lpCreateStruct) == -1)
				return -1;
	
			return 0;
		}

		//tab selection changed
		void CActionsConsoleDlgBar::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
		{
			// switched

			m_ScriptsDialog.ShowWindow(SW_HIDE);

			switch(m_TabMenu.GetCurSel())
			{
				//scripts
				case 0:
					m_ScriptsDialog.ShowWindow(SW_SHOW);
					break;

				default:
					break;

			}
		}

/*************************************************************/
/*			CActionsConsoleDlgBar Message Map				 */
/*************************************************************/

IMPLEMENT_DYNAMIC(CActionsConsoleDlgBar, CDialogBar)

BEGIN_MESSAGE_MAP(CActionsConsoleDlgBar, CDialogBar)
	ON_WM_CREATE()
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog)
	ON_NOTIFY(CTCN_SELCHANGE, IDC_SNIFFERTAB, OnSelchangeTab)

	/* BUTTONS */

		/***********MAIN TOOLBAR***********/
			ON_COMMAND(ID_SCRIPTS_START, &CActionsConsoleDlgBar::OnClickStart)
			ON_COMMAND(ID_SCRIPTS_STOP, &CActionsConsoleDlgBar::OnClickStop)
			ON_COMMAND(ID_SCRIPTS_ADD, &CActionsConsoleDlgBar::OnClickAdd)
			ON_COMMAND(ID_SCRIPTS_DELETE, &CActionsConsoleDlgBar::OnClickDelete)
END_MESSAGE_MAP()


// CActionsConsoleDlgBar message handlers
