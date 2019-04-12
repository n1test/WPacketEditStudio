/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  PEStudio.cpp

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

// The application class

#include "stdafx.h" 

/*************************************************************/
/*			File Variables									 */
/*************************************************************/

	static LPSTR lpConfigFiles[] = { "pestudio.ini", "\0" };

/*************************************************************/
/*			MFC Debug Code									 */
/*************************************************************/

	#ifdef _DEBUG
		#define new DEBUG_NEW
	#endif

/*************************************************************/
/*			CPEStudioApp									 */
/*															 */
/*	Handles general application loading and management.		 */
/*  This class is the front end for adding on to PES'		 */
/*  capabilities.											 */
/*************************************************************/

	/*************************************************************/
	/*			Constructor / Destructor						 */
	/*************************************************************/

		//init
		CPEStudioApp::CPEStudioApp()
		{
			pConfigParser = NULL;
		}

	/*************************************************************/
	/*			Instance Events									 */
	/*************************************************************/

		//application startup code
		BOOL CPEStudioApp::InitInstance()
		{
			//load settings
			RefreshSettings();

			//load scintilla
			m_hSciDLL = LoadLibrary("SciLexer.dll");

			if (!m_hSciDLL)
				return InitInstanceFailure(IDP_SCINTILLA_INIT_FAILED);

			//init common controls
			INITCOMMONCONTROLSEX InitCtrls;
			InitCtrls.dwSize = sizeof(InitCtrls);
			InitCtrls.dwICC = ICC_WIN95_CLASSES;
			if(!InitCommonControlsEx(&InitCtrls))
				return InitInstanceFailure(IDP_CONTROLS_INIT_FAILED);

			//init instance
			if(!CWinApp::InitInstance())
			{
				FreeLibrary(m_hSciDLL);
				return FALSE;
			}

			//init less common controls
			if (!AfxOleInit())
				return InitInstanceFailure(IDP_OLE_INIT_FAILED);
	
			AfxEnableControlContainer();
	
			if(!AfxInitRichEdit())
				return InitInstanceFailure(IDP_RICHEDIT_INIT_FAILED);

			//load registry settings
			SetRegistryKey("Packet Edit Studio");
			LoadStdProfileSettings(4);
	
			// build a php document interface
			m_pPHPDocTemplate = new CMultiDocTemplate(IDR_PHPDOCUMENT,
				RUNTIME_CLASS(CPHPDoc),
				RUNTIME_CLASS(CChildFrame),
				RUNTIME_CLASS(CScintillaDemoView));

			if (!m_pPHPDocTemplate)
				return InitInstanceFailure(IDP_PHPDOC_INIT_FAILED);
			AddDocTemplate(m_pPHPDocTemplate);


			// build a packet list document interface
			m_pSResultsDocTemplate = new CMultiDocTemplate(IDR_RESULTSDOCUMENT,
				RUNTIME_CLASS(CSniffResultsDoc),
				RUNTIME_CLASS(CChildFrame),
				RUNTIME_CLASS(CPacketListView));

			if (!m_pSResultsDocTemplate)
				return InitInstanceFailure(IDP_RESULTSDOC_INIT_FAILED);
			AddDocTemplate(m_pSResultsDocTemplate);

			// create main MDI Frame window
			m_pMainFrame = new CMainFrame;

			if (!m_pMainFrame || !m_pMainFrame->LoadFrame(IDR_MAINFRAME))
				return InitInstanceFailure(IDP_MAIN_INIT_FAILED);

			m_pMainWnd = m_pMainFrame;
			// call DragAcceptFiles only if there's a suffix
			//  In an MDI app, this should occur immediately after setting m_pMainWnd

			// Parse command line for standard shell commands, DDE, file open
			CCommandLineInfo cmdInfo;
			ParseCommandLine(cmdInfo);

			// Dispatch commands specified on the command line.  Will return FALSE if
			// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
			if (!ProcessShellCommand(cmdInfo))
				return InitInstanceFailure(IDP_ARGS_INIT_FAILED);
	
			// The main window has been initialized, so show and update it
			m_pMainFrame->ShowWindow(m_nCmdShow);
			m_pMainFrame->UpdateWindow();

			DialogWindowClass = AfxRegisterWndClass(NULL, NULL, (HBRUSH) ::GetStockObject(WHITE_BRUSH), NULL);

			return TRUE;
		}

		//application exit code
		int CPEStudioApp::ExitInstance()
		{
			PrepareExitInstance();
			return CWinApp::ExitInstance();
		}

		//wrapper function for cleaning up
		void CPEStudioApp::PrepareExitInstance()
		{
			if(m_hSciDLL)
				FreeLibrary(m_hSciDLL);

			if(!pConfigParser)
				delete pConfigParser;
		}

		//startup failure code
		BOOL CPEStudioApp::InitInstanceFailure(UINT type)
		{
			AfxMessageBox(type);
			PrepareExitInstance();
			return FALSE;
		}

		//handles command line arguemnts
		BOOL CPEStudioApp::ProcessShellCommand(CCommandLineInfo &rCmdInfo)
		{
			unsigned int i;
			char *ptr;

			ptr = rCmdInfo.m_strFileName.GetBuffer();

			if(ptr[0])
			{
				for(i=0;i<strlen(ptr);++i)
					if(ptr[i] == '.')
					{
						if((!strcmp(&ptr[i+1], "php")))
						    return CWinApp::ProcessShellCommand(rCmdInfo);
					}
				return TRUE;
			}
			else return TRUE;
		}

	/*************************************************************/
	/*			PHP Script Loading								 */
	/*************************************************************/

		//set whether or not to load php in the dll
		void CPEStudioApp::SetLoadPHP(bool load)
		{
			if(load)
				m_HookManager.SendData(m_hCurrentHook, IPC_COMMAND_PHP_START);
			else
				m_HookManager.SendData(m_hCurrentHook, IPC_COMMAND_PHP_END);

			m_LoadPHP = load;
		}

		//get whether or not to load php in the dll
		bool CPEStudioApp::GetLoadPHP()
		{
			return m_LoadPHP;
		}

	/*************************************************************/
	/*			Hook Management									 */
	/*************************************************************/

		//set the process to hook
		void CPEStudioApp::SetHook(LPSTR lpProcess, DWORD dwPid)
		{
			strcpy_s(m_HookExe, MAX_PATH, lpProcess);
			m_dwHookPid = dwPid;
		}

		//removes a hook from the hook list
		//  (call this only if you will never need the hook info
		//   again)
		void CPEStudioApp::DestroyHook(HOOK_HANDLE hHook)
		{
			m_HookManager.DeleteHook(hHook);
		}

		//starts packet capturing
		bool CPEStudioApp::StartHooking()
		{
			// look for dlls in the exe directory
			m_HookManager.SetWorkingDirectory(m_Directory);

			// create a new hook interface object
			ClientIPCFunctor *pIPCObject = m_IPCHandlers.CreateFunctor();
	
			// create a new hook object tied to that interface
			m_hCurrentHook = m_HookManager.CreateHook(m_lpDll, pIPCObject);
			if(!m_hCurrentHook)
				return false;

			// update the linked list hook info
			pIPCObject->SetHookHandle(m_hCurrentHook);

			// store a pointer to the hook object
			m_pCurrentHookObject = m_HookManager.GetDllNode(m_hCurrentHook);
			if(!m_pCurrentHookObject)
				return false;

			// create a process hooking interface
			if(!m_HookManager.AddProcess(m_hCurrentHook, m_dwHookPid))
				return false;

			// do the injection
			if(m_HookManager.InjectLibrary(m_hCurrentHook))
			{
				// tell the dll to start sniffing
				m_HookManager.SendData(m_hCurrentHook, IPC_COMMAND_START);

				// tell the dll how to handle php loading
				if(PESApp.GetLoadPHP())
					m_HookManager.SendData(m_hCurrentHook, IPC_COMMAND_PHP_START);
				else
					m_HookManager.SendData(m_hCurrentHook, IPC_COMMAND_PHP_END);

				return true;
			}
			else
				return false;
		}

		//tells the dll to pause packet capturing if "pause" is true otherwise
		// tells the dll to start capturing
		void CPEStudioApp::PauseHooking(bool pause)
		{
			if(pause)
				m_HookManager.SendData(m_hCurrentHook, IPC_COMMAND_PAUSE);
			else
				m_HookManager.SendData(m_hCurrentHook, IPC_COMMAND_START);
		}

		//tells the dll to stop capturing (if possible)
		//and attempts to unload the dll
		void CPEStudioApp::StopHooking()
		{

			// attempt to force the dll to unload
			m_HookManager.EjectLibrary(m_hCurrentHook);

			// output captured packets
			ClientIPCFunctor *pIPCObject = m_IPCHandlers.GetFunctor(m_hCurrentHook);

			if(pIPCObject)
			{
				if(pIPCObject->GetTotalPackets())
					pIPCObject->FillPacketListView(*CreateNewPacketList(m_hCurrentHook));
				else
				{
					// invalid hook interface, just get rid of it
					m_HookManager.DeleteHook(m_hCurrentHook);
					m_IPCHandlers.DeleteFunctor(m_hCurrentHook);
					m_hCurrentHook = NULL;
				}
			}
		}

		//referance to the hook management object
		CPESHookManager &CPEStudioApp::GetHookMgr()
		{
			return m_HookManager;
		}

	/*************************************************************/
	/*			Hook Info Access								 */
	/*************************************************************/

		//returns TRUE if the current hook is active, otherwise returns false
		BOOL CPEStudioApp::IsHooking()
		{
			return m_HookManager.GetState(m_hCurrentHook);
		}

		//get the exe name of the current hook object's associated process
		LPCSTR CPEStudioApp::GetHookExe()
		{
			return m_HookExe;
		}

		//get the number of times the current hook has injected the library successfully
		INT CPEStudioApp::GetTimesHooked()
		{
			return m_HookManager.GetTimesHooked();
		}

		//get a pointer to a hook's interface
		ClientIPCFunctor *CPEStudioApp::GetIPCObject(HOOK_HANDLE hHook)
		{
			return m_IPCHandlers.GetFunctor(hHook);
		}

	/*************************************************************/
	/*			Settings Management								 */
	/*************************************************************/

		//load application settings (can be called more than once)
		void CPEStudioApp::RefreshSettings()
		{

			// assume valid config
			m_ValidConfig = TRUE;

			// clean up after old data
			if(pConfigParser)
				delete pConfigParser;

			// fill in settings
			m_HookManager.HBGetModuleDirectory(m_Directory, MAX_PATH);

			pConfigParser = new INIParser();
			pConfigParser->SetWorkingDirectory(m_Directory);
			pConfigParser->LoadFiles(lpConfigFiles);

			m_buffersize = pConfigParser->iGetVar("pestudio.ini", "buffersz");
			m_packetsize = pConfigParser->iGetVar("pestudio.ini", "packetsz");
			m_SniffMethod = pConfigParser->iGetVar("pestudio.ini", "method");
			m_LogPHP = pConfigParser->boolGetVar("pestudio.ini", "logphp");

			m_lpPHPVer = pConfigParser->strGetVar("pestudio.ini", "phpver");
			m_lpDll = pConfigParser->strGetVar("pestudio.ini", "dll");
	
			// fix newlines
			fixeditdata(m_lpPHPVer);
			fixeditdata(m_lpDll);

			// check values
			if(m_buffersize < 100 || m_buffersize > 1048576)
				m_ValidConfig = FALSE;

			if(m_packetsize < 100 || m_packetsize > 10240)
				m_ValidConfig = FALSE;

			// setup ipc size
			m_HookManager.SetIPC("PacketEditStudioServer", IPC_MEMSIZE, sizeof(UINT));
		}

		//updates or creates a new setting value
		void CPEStudioApp::ConfigWriteValue(LPCSTR lpName, LPCSTR lpVal)
		{
			pConfigParser->WriteValue(lpConfigFiles[0], lpName, lpVal);
		}

	/*************************************************************/
	/*			Settings Access									 */
	/*************************************************************/

		//size of packet buffer
		int CPEStudioApp::GetBufferSize()
		{
			return m_buffersize;
		}

		//max packet size
		int CPEStudioApp::GetMaxPacketSize()
		{
			return m_packetsize;
		}

		//which sniff type to use
		int CPEStudioApp::GetSniffMethod()
		{
			return m_SniffMethod;
		}

		//the main module directory
		LPCSTR CPEStudioApp::GetAppDirectory()
		{
			return (LPCSTR)m_Directory;
		}

		//name of the dll to inject
		LPCSTR CPEStudioApp::GetDllName()
		{
			return (LPCSTR)m_lpDll;
		}

		//php version to use
		LPCSTR CPEStudioApp::GetPHPVer()
		{
			return (LPCSTR)m_lpPHPVer;
		}

		//whether to log php
		bool CPEStudioApp::GetLogPHP()
		{
			return m_LogPHP;
		}

		//whether the settings are valid
		BOOL CPEStudioApp::IsConfigValid()
		{
			return m_ValidConfig;
		}

	/*************************************************************/
	/*			Main Form Access								 */
	/*************************************************************/

		//referance to the main form object
		CMainFrame &CPEStudioApp::GetMainFrm()
		{
			return *m_pMainFrame;
		}

	/*************************************************************/
	/*			Doc Helper Functions							 */
	/*************************************************************/

		//create a sniff results document
		CPacketListView *CPEStudioApp::CreateNewPacketList(HOOK_HANDLE hHook)
		{

			// create the document
			CSniffResultsDoc *SniffPage = 
						     (CSniffResultsDoc *)m_pSResultsDocTemplate->OpenDocumentFile(NULL);

			// update the document hook info
			SniffPage->SetHookHandle(hHook);

			// init the packet list view object
			POSITION p = SniffPage->GetFirstViewPosition();
	
			CPacketListView *SniffPageView = (CPacketListView *)SniffPage->GetNextView(p);
			SniffPageView->InitList();

			SniffPageView->SetHookHandle(hHook);

			return SniffPageView;
		}

		//load a php document
		BOOL CPEStudioApp::LoadPHPDocumentFromFile(LPSTR lpFile)
		{
			CPHPDoc *pDoc;
			pDoc = (CPHPDoc *)m_pPHPDocTemplate->OpenDocumentFile(lpFile);
			return TRUE;
		}

/*************************************************************/
/*			CMainFrame Message Map							 */
/*************************************************************/

BEGIN_MESSAGE_MAP(CPEStudioApp, CWinApp)

	/* MENU */
		/***********File Menu***********/
			ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
			ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
			ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
		/**********************************/

END_MESSAGE_MAP()

/*************************************************************/
/*			Globals											 */
/*************************************************************/
	CPEStudioApp PESApp;