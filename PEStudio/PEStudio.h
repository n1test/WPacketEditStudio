/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  PEStudio.h

  This file is part of the Packet Edit Studio Project.
  version: ALPHA

  *Distributed under the terms of the zlib/libpng public license.*
  
  Author: 

	Chris Smith (sourcemagik.com) - CPEStudioApp
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

// The loading and management code for Packet Edit Studio

	#pragma once

/*************************************************************/
/*			Prototypes										 */
/*************************************************************/
	class CMainFrame;

/*************************************************************/
/*			Structures										 */
/*************************************************************/

	/*************************************************************/
	/*			CPEStudioApp									 */
	/*															 */
	/*	Handles general application loading and management.		 */
	/*  This class is the front end for adding on to PES'		 */
	/*  capabilities.											 */
	/*************************************************************/
	class CPEStudioApp : public CWinApp
	{
		public:

			/////////////////////////////
			//  Constructor / Destructor
			/////////////////////////////

				CPEStudioApp();

			/////////////////////////////
			//  Instance Events
			/////////////////////////////
		protected:
				DECLARE_MESSAGE_MAP()

		public:
				virtual BOOL InitInstance();
				virtual int ExitInstance();
				void PrepareExitInstance();
				BOOL InitInstanceFailure(UINT type);
				virtual BOOL ProcessShellCommand(CCommandLineInfo &rCmdInfo);

			/////////////////////////////
			//  WIN32 Load Info
			/////////////////////////////

				LPCSTR DialogWindowClass;

			/////////////////////////////
			//  PHP Script Loading
			/////////////////////////////
			
				//set php loading in the dll
				void SetLoadPHP(bool load);

				//get whether to load php in the dll
				bool GetLoadPHP();

			/////////////////////////////
			//  Hook Management
			/////////////////////////////

				//set the hooking process info
				void SetHook(LPSTR lpProcess, DWORD dwPid);

				//remove a hook completely from memory
				void DestroyHook(HOOK_HANDLE hHook);

				//inject dll and begin capturing
				bool StartHooking();

				//pause capturing
				void PauseHooking(bool pause);

				//end capturing
				void StopHooking();

				//public hook manager access
				CPESHookManager &GetHookMgr();

			/////////////////////////////
			//  Hook Info Access
			/////////////////////////////

				//check if a hook is currently running
				BOOL IsHooking();

				//get the name of the currently hooked process
				LPCSTR GetHookExe();

				//get the number of times the currently hooked process
				//has been hooked
				INT GetTimesHooked();

				//public access to the hook object's ipc functor
				ClientIPCFunctor *GetIPCObject(HOOK_HANDLE hHook);

			/////////////////////////////
			//  Settings Management
			/////////////////////////////

				//loads main settings and refreshes the application
				void RefreshSettings();

				//writes the settings to the ini file and refreshes the app
				void ConfigWriteValue(LPCSTR lpName, LPCSTR lpVal);

			/////////////////////////////
			//  Settings Access
			/////////////////////////////
			
				//max size of packet buffer
				int GetBufferSize();

				//max individual packet size
				int GetMaxPacketSize();

				//type of sniffing to do
				int GetSniffMethod();

				//the directory the exe is in
				LPCSTR GetAppDirectory();

				//the name of the dll to load
				LPCSTR GetDllName();

				//which version of php to load
				LPCSTR GetPHPVer();

				//whether to log individual php output
				bool GetLogPHP();

				//was config loaded without a problem?
				BOOL IsConfigValid();

			/////////////////////////////
			//  Main Form Access
			/////////////////////////////

				//access to the programs main form
				CMainFrame &GetMainFrm();

			/////////////////////////////
			//  Doc Helper Functions
			/////////////////////////////

				//creates a new packet list for when data arrives
				CPacketListView *CreateNewPacketList(HOOK_HANDLE hHook);

				//creates a php document
				BOOL LoadPHPDocumentFromFile(LPSTR lpFile);

		private:

			/////////////////////////////
			//  Private Variables
			/////////////////////////////

				//SETTINGS
				INIParser *pConfigParser;							//config handler

				BOOL m_ValidConfig;									//is config valid
				CHAR m_Directory[MAX_PATH];							//app directory
				LPSTR m_lpDll;										//dll name
				LPSTR m_lpPHPVer;									//php version
				bool m_LogPHP;										//log php?
				int m_buffersize;									//max buffer size
				int m_packetsize;									//max packet size
				int m_SniffMethod;									//sniff type

				//HOOK INFO
				CHookManager <ClientIPCFunctor> m_HookManager;		//dll hooking / ipc manager
				CHookDll <ClientIPCFunctor> *m_pCurrentHookObject;	//pointer to current hook functor
				IPCFunctorList m_IPCHandlers;						//list of ipc functors

				HOOK_HANDLE m_hCurrentHook;							//currently active hook
				CHAR m_HookExe[MAX_PATH];							//the above hook's process name
				DWORD m_dwHookPid;									//the above hook's process id

				//PHP LOAD INFO
				bool m_LoadPHP;										//load php in dll?

				//DYNMIC LIBRARIES
				HINSTANCE m_hSciDLL;								//scintilla library

				//FORM LOAD VARIABLES
				CMainFrame *m_pMainFrame;							//pointer to the main window
				CMultiDocTemplate *m_pPHPDocTemplate;				//php document template
				CMultiDocTemplate *m_pSResultsDocTemplate;			//sniff results document template
	};

/*************************************************************/
/*			Globals											 */
/*************************************************************/
	extern CPEStudioApp PESApp;
