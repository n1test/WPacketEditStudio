/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  ProcessSelectDlg.cpp

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

	#include "stdafx.h"

/*************************************************************/
/*			CProcessSelectDlg								 */
/*															 */
/*	This will display a list of processes with their icons   */
/*  to choose from.											 */
/*************************************************************/

	/*************************************************************/
	/*			Constructor / Destructor						 */
	/*************************************************************/

		//init
		CProcessSelectDlg::CProcessSelectDlg() : CDialog(CProcessSelectDlg::IDD)
		{
		}

	/*************************************************************/
	/*			Window Events									 */
	/*************************************************************/

		//set up controls
		void CProcessSelectDlg::DoDataExchange(CDataExchange* pDX)
		{
			CDialog::DoDataExchange(pDX);
			DDX_Control(pDX, IDC_PROCESSLIST, m_ProcessList);
			DDX_Control(pDX, IDC_PROCESS_NAME, m_ProcessNameText);
			DDX_Control(pDX, IDC_PID, m_ProcessIdText);
			DDX_Control(pDX, IDC_EXE_SZ, m_ProcessEXESizeText);
			DDX_Control(pDX, IDC_USER, m_ProcessOwnerText);
			DDX_Control(pDX, IDC_OPENPROCESS_STATE, m_OpenProcessStatusText);
			DDX_Control(pDX, IDC_PS_WINDOWS, m_WindowList);
		}

		//init code
		BOOL CProcessSelectDlg::OnInitDialog()
		{
			if(!CDialog::OnInitDialog())
				return FALSE;

			// create tool tips
			m_ProcessListTip.Create(this);
			m_ProcessListTip.AddTool(&m_ProcessList,"A red exclamation mark flags a process\r\n"
													" where the OpenProcess API failed");
			m_ProcessListTip.Activate(TRUE);

			m_ProcessNameTip.Create(this);
			m_ProcessNameTip.AddTool(&m_ProcessNameText,"Process name as it apears in the process list");
			m_ProcessNameTip.Activate(TRUE);

			m_ProcessIdTip.Create(this);
			m_ProcessIdTip.AddTool(&m_ProcessIdText,"Process id (PID)");
			m_ProcessIdTip.Activate(TRUE);

			m_ProcessEXESizeTip.Create(this);
			m_ProcessEXESizeTip.AddTool(&m_ProcessEXESizeText,"Size in bytes of the process file");
			m_ProcessEXESizeTip.Activate(TRUE);

			m_ProcessOwnerTip.Create(this);
			m_ProcessOwnerTip.AddTool(&m_ProcessOwnerText,"Process owner (Creator)");
			m_ProcessOwnerTip.Activate(TRUE);

			m_OpenProcessStatusTip.Create(this);
			m_OpenProcessStatusTip.AddTool(&m_OpenProcessStatusText,"Tells whether you have access to the program \r\n"
																	"**NOTE you can still gain access to \r\n"
																	"the rogram by giving Packet Edit Studio Debug access \r\n"
																	"even if the test fails**");
			m_OpenProcessStatusTip.Activate(TRUE);

			// create image list
			m_Images.Create(32, 32, ILC_COLOR32 | ILC_MASK, 1, 1);
			CBitmap cb;
			cb.LoadMappedBitmap(IDR_NOTFOUNDICO);
			m_Images.Add(&cb, RGB(255, 255, 245));
			m_ProcessList.SetImageList(&m_Images,  LVSIL_NORMAL);

			// go through processes
			HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

			PROCESSENTRY32 uProcess;
			uProcess.dwSize = sizeof(uProcess);

			HICON hIcon;
			CHAR tmp[MAX_PATH];
			HANDLE hProcess;
			INT Img;

			LONG rProcessFound = (LONG)Process32First(hSnapshot, &uProcess);
			rProcessFound = (LONG)Process32Next(hSnapshot, &uProcess);
			rProcessFound = (LONG)Process32Next(hSnapshot, &uProcess);
			while(rProcessFound) {
				if(_stricmp(uProcess.szExeFile, "pestudio.exe"))
				{

					hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, uProcess.th32ProcessID);
					if(hProcess)
					{
						if(GetModuleFileNameEx(hProcess, NULL, tmp, MAX_PATH))
						{
							// get the icon
							hIcon = ExtractIcon(AfxGetInstanceHandle(), tmp, 0); 

							if(hIcon)
							{
								Img = m_Images.Add(hIcon);
								if(Img == -1)
									Img = 0;
							}
							else
								Img = 0;
						}
						else
							Img = 0;
					}
					else
						Img = 0;

					// add it to the list
					sprintf_s(tmp, "%s\r\n%d", uProcess.szExeFile, uProcess.th32ProcessID);
					m_ProcessList.InsertItem(0, tmp, Img);
				}

				rProcessFound = (LONG)Process32Next(hSnapshot, &uProcess);
			}

			return TRUE;
		}

		//for tool tips
		BOOL CProcessSelectDlg::PreTranslateMessage(MSG *pMsg)
		{
			m_ProcessListTip.RelayEvent(pMsg);
			m_ProcessNameTip.RelayEvent(pMsg);
			m_ProcessIdTip.RelayEvent(pMsg);
			m_ProcessEXESizeTip.RelayEvent(pMsg);
			m_ProcessOwnerTip.RelayEvent(pMsg);
			m_OpenProcessStatusTip.RelayEvent(pMsg);
			return CDialog::PreTranslateMessage(pMsg);
		}

		//clicked on process list
		void CProcessSelectDlg::OnNMClickProcesslist(NMHDR *pNMHDR, LRESULT *pResult)
		{
			m_WindowList.ResetContent();

			// get selected item
			int itm = m_ProcessList.GetSelectionMark();

			// get selected item pid
			char tmp[255];
			m_ProcessList.GetItemText(itm, 0, tmp, 255);

			int i;
			int pid;
			for(i=0,pid=-1;i<(INT)strlen(tmp);++i)
				if(tmp[i] == '\n')
				{
					pid = atoi(&tmp[i+1]);
					break;
				}

			// pid is valid
			if(pid != -1)
			{
				HANDLE hProcess;
				HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

				PROCESSENTRY32 uProcess;
				uProcess.dwSize = sizeof(uProcess);


				LONG rProcessFound = (LONG)Process32First(hSnapshot, &uProcess);
				rProcessFound = (LONG)Process32Next(hSnapshot, &uProcess);
				rProcessFound = (LONG)Process32Next(hSnapshot, &uProcess);
				while(rProcessFound)
				{
					hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, uProcess.th32ProcessID);

					if(uProcess.th32ProcessID == pid)
					{

						// set exe name and pid text
						m_ProcessNameText.SetWindowTextA(uProcess.szExeFile);
						m_ProcessIdText.SetWindowTextA(itoa(pid));

						// get the file size of the process
						LARGE_INTEGER FileSz;
						if(hProcess)
						{
							GetModuleFileNameEx(hProcess, NULL, tmp, MAX_PATH-1);
 							HANDLE hFile = CreateFile(tmp, FILE_READ_EA, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY | FILE_FLAG_NO_BUFFERING, NULL);
							
							if(GetFileSizeEx(hFile, &FileSz))
								sprintf_s(tmp, "%d bytes", FileSz);
							else
								FileSz.LowPart = -1;

							 m_ProcessEXESizeText.SetWindowTextA(tmp);
						}

						if((!hProcess) || ((INT)FileSz.LowPart<=0))
							m_ProcessEXESizeText.SetWindowTextA("Could not read file");

						// get process creator
						HANDLE hToken;
						if(OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))
						{
							DWORD needed;
							PTOKEN_OWNER pTokenOwner = (PTOKEN_OWNER) malloc(16384);
							
							GetTokenInformation(hToken, TokenOwner, pTokenOwner, 16384, &needed);

							char user[MAX_PATH];
							char domain[MAX_PATH];
							DWORD sizeUser = sizeof(user);
							DWORD sizeDomain = sizeof(domain);
							SID_NAME_USE snu;

							LookupAccountSid(NULL, pTokenOwner->Owner, user, &sizeUser, domain, &sizeDomain, &snu);

							free(pTokenOwner);
							
							m_ProcessOwnerText.SetWindowTextA(user);
							
							CloseHandle(hToken);
						}
						else
							m_ProcessOwnerText.SetWindowTextA("?Unknown");

						CloseHandle(hProcess);

						// see if we can access the process
						hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, uProcess.th32ProcessID);
						if(hProcess)
							m_OpenProcessStatusText.SetWindowTextA("PASSED");
						else
							m_OpenProcessStatusText.SetWindowTextA("FAILED");

						CloseHandle(hProcess);

						// set selected pid
						dpid = uProcess.th32ProcessID;

						// show windows associated with the process
						EnumWindows(&EnumWindowsProc, (LPARAM)this);

						// only one pid should ever be found
						break;
					}
					rProcessFound = (LONG)Process32Next(hSnapshot, &uProcess);
				}
			}

			*pResult = 0;
		}

		// double clicked an item in the process list
		void CProcessSelectDlg::OnNMDblClickProcesslist(NMHDR *pNMHDR, LRESULT *pResult)
		{
			if(m_ProcessList.GetSelectedCount())
			{

				// get the full string
				CString str = m_ProcessList.GetItemText(GetSelectedItem(), 0);

				char arr[255];
				strcpy_s(arr, str);

				size_t len = strlen(arr);

				// get the process id
				UINT i;
				char *pid;
				for(i = 0; i < len ; ++i)
					if(arr[i] == '\r')
						arr[i] = 0;
					else if(arr[i] == '\n')
					{
						arr[i] = 0;
						pid = &arr[i+1];
						break;
					}

				// set the hook
				PESApp.SetHook(arr, atoi(pid));
			
				char title[255];
				strcpy_s(title, 255, "Packet Edit Studio - ");
				strcat_s(title, 255, arr);

				::SetWindowText(::GetParent(GetSafeHwnd()), title);

				::EndDialog(GetSafeHwnd(), NULL);
			}
			return;
		}

	/*************************************************************/
	/*			Internal Utility Functions						 */
	/*************************************************************/

		//window found
		void CProcessSelectDlg::WindowFound(LPSTR lpWindow)
		{
			m_WindowList.AddString(lpWindow);
		}

		//get the currently selected list item
		int CProcessSelectDlg::GetSelectedItem()
		{
			int nItem = -1;
			POSITION nPos = m_ProcessList.GetFirstSelectedItemPosition();
			if (nPos)
				nItem = m_ProcessList.GetNextSelectedItem(nPos);
			return nItem;
		}

	/*************************************************************/
	/*			Enum Window Procedure							 */
	/*************************************************************/

		BOOL CALLBACK CProcessSelectDlg::EnumWindowsProc(HWND hwnd, LPARAM lParam)
		{

			// add each individual window
			CProcessSelectDlg *MyClass = (CProcessSelectDlg *)lParam;

			if(!MyClass)
				return FALSE;

			DWORD pid;
			GetWindowThreadProcessId(hwnd, (LPDWORD)&pid);

			char Name[255];
			if(pid == MyClass->dpid)
			{
				::GetWindowText(hwnd, Name, 255);
				if(Name[0])
					MyClass->WindowFound(Name);
			}
			return TRUE;
		}

/*************************************************************/
/*			CProcessSelectDlg Message Map					 */
/*************************************************************/

BEGIN_MESSAGE_MAP(CProcessSelectDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_PROCESSLIST, &CProcessSelectDlg::OnNMClickProcesslist)
	ON_NOTIFY(NM_DBLCLK, IDC_PROCESSLIST, &CProcessSelectDlg::OnNMDblClickProcesslist)
	//ON_COMMAND(IDC_OPTIONS_OK, &CProcessSelectDlg::OnClickOK)
	//ON_COMMAND(IDC_OPTIONS_CANCEL, &CProcessSelectDlg::OnClickCancel)
END_MESSAGE_MAP()
