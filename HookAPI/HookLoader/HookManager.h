/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  HookManager.h

  This file is part of the Packet Edit Studio Project.
  version: ALPHA

  *Distributed under the terms of the zlib/libpng public license.*
  
  Author: 

	Chris Smith (sourcemagik.com) - API Hooking Interface
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

// This is a free template library for Dll Injection and API Hooking

	#pragma once

	template <class CallbackObject> class CHookManager;

/*************************************************************/
/*			Loop Commands									 */
/*************************************************************/
	const static UINT HOOK_OP_INJECT = 0x00;
	const static UINT HOOK_OP_EJECT  = 0x01;
	const static UINT HOOK_OP_SEND   = 0x02;
	const static UINT HOOK_OP_SETIPC = 0x03;
	const static UINT HOOK_OP_GETP   = 0x04;

/*************************************************************/
/*			Custom Errors (starts at 298)					 */
/*************************************************************/
	const static DWORD ERROR_INJECT_FAILURE = 0xE000012AL;

/*************************************************************/
/*			Structures										 */
/*************************************************************/

	template <class CallbackObject> class CProcessHook;
	
	// structure for linked list of processes to associate with
	// a hook
	template <class CallbackObject>
	struct HookProcess
	{
		UINT id;
		CProcessHook <CallbackObject> *ProcessHook;
		HookProcess <CallbackObject> *next;
	};

	// The main hooking APIs
	struct CHookBase
	{

		/*************************************************************/
		/*			Inject For Single Process						 */
		/*************************************************************/

			//injects a dll into a process
			BOOL HBInject(DWORD dwProcess, LPSTR lpDll);

			//ejects a dll from a process
			BOOL HBEject(DWORD dwProcess, LPSTR lpDll);

		/*************************************************************/
		/*			Inject For All Processes						 */
		/*************************************************************/

			//injects a dll into all processes
			BOOL HBInjectAll(LPSTR lpDll);

			//ejects a dll from all processes
			BOOL HBEjectAll(LPSTR lpDll);

		/*************************************************************/
		/*			Utility Functions								 */
		/*************************************************************/
		
			//returns TRUE if the path is a valid file
			BOOL HBFileExists(LPCSTR lpName);

			//fills lpBuffer with the directory the exe was executed from
			BOOL HBGetModuleDirectory(LPSTR lpBuffer, UINT iSize);
			
			//returns TRUE if we can get access to CreateRemoteThread
			BOOL HBCanAccessProcess(DWORD dwPid);
	};

	/*************************************************************/
	/*			CHookDll										 */
	/*															 */
	/*	The dll class which holds the name of the dll to inject	 */
	/*	and a list processes to inject it into.					 */
	/*************************************************************/
	template <class CallbackObject>
	class CHookDll
	{

		/*************************************************************/
		/*			Constructor / Destructor						 */
		/*************************************************************/
		public:

			//init
			CHookDll() { HookList = NULL; }

			//free
			virtual ~CHookDll()
			{
				HookProcess <CallbackObject> *ptr = HookList, *next = NULL;

				while(ptr)
				{
					next = ptr->next;
					delete ptr;
					ptr = next;
				}
			}

		/*************************************************************/
		/*			Public Variables								 */
		/*************************************************************/

			//DLL INFO
			CHAR chName[MAX_PATH];							//dll name
			CHAR chDirectory[MAX_PATH];						//dll directory
			
			//HOOKED PROCESSES
			HookProcess <CallbackObject> *HookList;			//list of processes in the hook
			HookProcess <CallbackObject> *ProcessLoop;		//iterator for accessing processes

			//HOOK DATA
			HOOK_HANDLE hHook;								//hook handle identifier
			CHookDll <CallbackObject> *next;				//next hook object in the list
			BOOL state;										//whether hook is active
			CallbackObject *read_callback;					//a pointer to the hook's callback object
	};

	/*************************************************************/
	/*			CProcessHook									 */
	/*															 */
	/*	A template class for creating a hooking interface for a  */
	/*	single process.											 */
	/*************************************************************/
	template <class CallbackObject=SHAREDMEM_CSTYLE_CALLBACK>
	class CProcessHook : public CHookBase
	{

		/*************************************************************/
		/*			Constructor / Destructor						 */
		/*************************************************************/
		public:

			//init
			CProcessHook()
			{
				Init();
			}

			CProcessHook(HOOK_HANDLE hHook)
			{
				Init();
				CProcessHook();
				m_hMyHook = hHook;
			}

			//free
			virtual ~CProcessHook()
			{
				if(m_lpMemoryClient)
					delete m_lpMemoryClient;

				if(m_mybuffer)
					delete [] m_mybuffer;
			}

		/*************************************************************/
		/*			Injection Setup									 */
		/*************************************************************/

			//sets the process id of the process to hook
			VOID SetInjectProcess(DWORD Pid)
			{
				m_dwHookPid = Pid;
				m_iTimesHooked = 1;
				GetProcessName();
				_itoa_s(m_dwHookPid, m_chMyPid, 50, 10);
			}

			//sets the dll to inject into the process
			// (if lpDllDir == NULL then it uses the exe module's
			// directory)
			VOID SetInjectDll(LPSTR lpDll, LPSTR lpDllDir=NULL)
			{
				strcpy_s(m_chHookDll, MAX_PATH, lpDll);

				if(lpDllDir)
					strcpy_s(m_chHookDllPath, MAX_PATH, lpDllDir);
				else
					HBGetModuleDirectory(m_chHookDllPath, MAX_PATH);

				strcat_s(m_chHookDllPath, MAX_PATH, "\\");
				strcat_s(m_chHookDllPath, MAX_PATH, lpDll);
			}

		/*************************************************************/
		/*			Injection Controls								 */
		/*************************************************************/

			//injects the dll
			BOOL InjectDll()
			{
				// clean up after old injections
				if(m_lpMemoryClient)
					delete m_lpMemoryClient;

				// try to inject the dll into the process
				m_bHookState = HBInject(m_dwHookPid, m_chHookDllPath);

				// ipc setup
				if(m_Alloc)
				{

					// allocate for the ipc buffer if necessary
					if(!m_mybuffer)
						m_mybuffer = new BYTE [ m_Alloc +sizeof(HOOK_HANDLE)+sizeof(DWORD) ];

					if(!m_bHookState)
						return FALSE;

					// setup the ipc client for sending data
					strcpy_s(m_chClient, MAX_PATH*2, m_chServer);
					strcat_s(m_chClient, MAX_PATH*2, m_chHookExe);
					strcat_s(m_chClient, MAX_PATH*2, m_chMyPid);

					m_lpMemoryClient = new CSharedMemory <void*> (m_chClient);

					// try to connect to the dll's ipc server
					int tries = 0;
					while(!m_lpMemoryClient->OpenMemory(m_Alloc))
					{
						if(tries > IPC_MAXTRIES)
						{

							// if it fails then clean up
							EjectDll();
							delete m_lpMemoryClient;
							m_lpMemoryClient = NULL;
							SetLastError(ERROR_PATH_NOT_FOUND);
							m_bHookState = FALSE;

							return FALSE;

						}
						++tries;

						Sleep(1000);
					}

					// send a small chunk of data to tell the dll the hook handle id
					SendData((LPBYTE)" ", 1);

				}

				return m_bHookState;
			}

			//ejects the dll
			BOOL EjectDll()
			{
				m_lpMemoryClient = NULL;
				m_bHookState = !HBEject(m_dwHookPid, m_chHookDll);
				return !m_bHookState;
			}

		/*************************************************************/
		/*			IPC Controls									 */
		/*************************************************************/

			//sets the ipc info (required for using ipc)
			BOOL SetIPC(LPSTR lpServer, UINT sz)
			{
				strcpy_s(m_chServer, MAX_PATH, lpServer);
				m_Alloc = sz;
				return TRUE;
			}

			//sends data to the dll's ipc server
			BOOL SendData(LPBYTE Data, UINT sz)
			{
				HOOK_HANDLE *pHook;
				DWORD *pPid;
				UINT headersize;
				LPBYTE datastart;

				// we need a buffer to send data!
				if(!m_mybuffer)
					return FALSE;

				// fill in hook information
				headersize = sizeof(HOOK_HANDLE) + sizeof(DWORD);

				pHook = (HOOK_HANDLE*)m_mybuffer;
				*pHook = m_hMyHook;

				pPid = (DWORD*) (m_mybuffer + sizeof(HOOK_HANDLE));
				*pPid = m_dwHookPid;

				// add the send data
				datastart = m_mybuffer + headersize;
				memcpy(datastart, Data, sz);

				// try to send it
				return m_lpMemoryClient->WriteData((LPVOID)m_mybuffer, sz + headersize);
			}
			
		/*************************************************************/
		/*			Injection Info Access							 */
		/*************************************************************/

			//name of the process being injected into
			LPSTR GetProcess()
			{
				return m_chHookExe;
			}

			//the process id of the process to be injected into
			DWORD GetPid()
			{
				return m_dwHookPid;
			}

			//name of the dll being injected
			LPSTR GetDll()
			{
				return m_chHookDll;
			}

			//path of the dll being injected
			LPSTR GetDllPath()
			{
				return m_chHookDllPath;
			}

			//number of times the injection api has been fired
			UINT GetTimesInjected()
			{
				return m_iTimesHooked;
			}

			//whether hooked or not
			BOOL GetState()
			{
				return m_bHookState;
			}

		/*************************************************************/
		/*			Public Variables								 */
		/*************************************************************/

			CallbackObject *read_callback;					//a pointer to the process' callback object

		/*************************************************************/
		/*			Helper Functions								 */
		/*************************************************************/

			//constructor helper function
			void Init()
			{
				m_lpMemoryClient = NULL;
				m_chHookExe[0] = '\0';
				m_bHookState = FALSE;
				m_hMyHook = 0;
				m_mybuffer = NULL;
			}

			//gets the name of the process whose pid has been set
			BOOL GetProcessName()
			{
				HANDLE handle;
				PROCESSENTRY32 pe32;
				BOOL flag = TRUE;

				pe32.dwSize = sizeof(PROCESSENTRY32);
				handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

				if(Process32First(handle, &pe32))
					while(Process32Next(handle, &pe32))
						if(m_dwHookPid == pe32.th32ProcessID)
						{
							strcpy_s(m_chHookExe, MAX_PATH, pe32.szExeFile);
							flag = FALSE;
						}

				return !flag;
			}

		/*************************************************************/
		/*			Private Variables								 */
		/*************************************************************/
		private:

			//CURRENT EXE INFO
			CHAR m_chMyPid[50];							//current exe pid in a string

			//DLL INFO
			CHAR m_chHookDll[MAX_PATH];					//the dll name to be injected
			CHAR m_chHookDllPath[MAX_PATH];				//the full path to the dll to be injected

			//EXE INFO
			DWORD m_dwHookPid;							//the target process pid
			CHAR m_chHookExe[MAX_PATH];					//the target process' name

			//HOOK INFO
			HOOK_HANDLE m_hMyHook;						//handle to the process' hook object
			BOOL m_bHookState;							//whether the dll is injected
			UINT m_iTimesHooked;						//number of times the injection api has been fired

			//IPC INFO
			CHAR m_chServer[MAX_PATH];					//the name of the current exe's ipc server
			CHAR m_chClient[MAX_PATH*2];				//the name of the dll's ipc server
			LPBYTE m_mybuffer;							//buffer for sending data to the dll
			UINT m_Alloc;								//size of the ipc memory chunk
			CSharedMemory <void *> *m_lpMemoryClient;	//the ipc object for communicating with the dll
	};

	/*************************************************************/
	/*			CHookServerFunctor								 */
	/*															 */
	/*	The object that handles incoming data from a dll		 */
	/*  it divies out the data to the other callback objects	 */
	/*************************************************************/
	template <class CallbackObject>
	class CHookServerFunctor : public CHookFunctor
	{

		/*************************************************************/
		/*			Constructor / Destructor						 */
		/*************************************************************/
		public:

			//init
			CHookServerFunctor() {}

			//free
			virtual ~CHookServerFunctor() {}

		/*************************************************************/
		/*			Hook Manager Set Up								 */
		/*************************************************************/

			//sets the hook manager object that will handle ipc
			void SetHookManager(CHookManager <CallbackObject> *manager)
			{
				m_HookManager = manager;
			}

		/*************************************************************/
		/*			Functor Methods									 */
		/*************************************************************/

			//functor operation
			void operator() (LPVOID lpData, INT iSz)
			{
				//get the info about the sending process
				LPBYTE lpBuff = (LPBYTE)lpData;
				LPBYTE dataptr;
				UINT datasize, headersize;
				HOOK_HANDLE *pHook;
				DWORD *pPid;

				headersize = sizeof(HOOK_HANDLE) + sizeof(DWORD);
				datasize = iSz - headersize;

				pHook = (HOOK_HANDLE*)lpBuff;
				pPid = (DWORD*) (lpBuff + sizeof(HOOK_HANDLE));
				dataptr = lpBuff + headersize;

				//pass the ipc data on to the hook manager
				if(m_HookManager)
					m_HookManager->HandleCallback(*pHook, *pPid, dataptr, datasize);
			}

		/*************************************************************/
		/*			Private Variables								 */
		/*************************************************************/
		private:

			CHookManager <CallbackObject> *m_HookManager;				//the hook manager object that handles the ipc
	};

	/*************************************************************/
	/*			CHookManager									 */
	/*															 */
	/*	A class template for managing multiple hooks between 	 */
	/*	multiple dlls and processes.							 */
	/*************************************************************/
	template <class CallbackObject>
	class CHookManager : public CHookBase
	{
		/*************************************************************/
		/*			Constructor / Destructor						 */
		/*************************************************************/
		public:

			//init
			CHookManager()
			{
				m_base = NULL;
				m_next_handle = 0;
				m_next_id = 0;
				SetWorkingDirectory();
				m_timeshooked=0;
			}

			//free
			virtual ~CHookManager()
			{
				DeleteAllHooks();
			}

		/*************************************************************/
		/*			Hook Management	Functions						 */
		/*************************************************************/

			//Sets the directory that the class will look for
			//the dll in, by default the directory is the application
			//directory.
			void SetWorkingDirectory(LPSTR lpDirectory=NULL)
			{
				if(lpDirectory)
					strcpy_s(m_WorkingDirectory, MAX_PATH, lpDirectory);
				else
					HBGetModuleDirectory(m_WorkingDirectory, MAX_PATH);
			}

			//Creates a new hook object and returns a handle to
			//that object.  Note that it does not return the object
			//itself.  To get the object call GetDllNode(HOOK_HANDLE)
			HOOK_HANDLE CreateHook(LPSTR lpDll, CallbackObject *callback=NULL)
			{
				CHookDll <CallbackObject> *dllNode;

				//create the hook object
				dllNode = CreateDllNode();

				strcpy_s(dllNode->chName, MAX_PATH, lpDll);
				strcpy_s(dllNode->chDirectory, MAX_PATH, m_WorkingDirectory);

				dllNode->read_callback = callback;

				dllNode->state = FALSE;

				return dllNode->hHook;
			}

			//Add a specific process to a hook object
			CProcessHook <CallbackObject> *AddProcess(HOOK_HANDLE hHook, DWORD dwPid, CallbackObject *callback=NULL)
			{
				CHookDll <CallbackObject> *dllNode;
				CProcessHook <CallbackObject> *ProcessHook;

				//check if exe can be injected
				if(!HBCanAccessProcess(dwPid))
					return 0;

				//find the hook
				dllNode = GetDllNode(hHook);

				if(!dllNode)
					return FALSE;

				//create a new process object
				ProcessHook = CreateProcessNode(dllNode->hHook);

				ProcessHook->read_callback = callback;
				ProcessHook->SetIPC(m_Server, m_cAlloc);

				//init object
				ProcessHook->SetInjectDll(dllNode->chName, dllNode->chDirectory);
				ProcessHook->SetInjectProcess(dwPid);

				return ProcessHook;
			}

			//Add all running processes to a hook object
			BOOL AddAllProcesses(HOOK_HANDLE hHook, CallbackObject *callback=NULL)
			{
				HANDLE handle;
				PROCESSENTRY32 pe32;
				DWORD MyPid;
				BOOL flag;

				//find the hook
				if(!GetDllNode(hHook))
					return FALSE;

				flag = FALSE;
				MyPid = GetCurrentProcessId();

				//for each running process call addprocess
				pe32.dwSize = sizeof(PROCESSENTRY32);
				handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

				if(Process32First(handle, &pe32))
					while(Process32Next(handle, &pe32))
						if(MyPid != pe32.th32ProcessID)
							if(!AddProcess(hHook, pe32.th32ProcessID, callback))
								flag = TRUE;

				CloseHandle(handle);

				return !flag;
			}

			//remove a hook object and all of its process objects
			void DeleteHook(HOOK_HANDLE hHook)
			{
				CHookDll <CallbackObject> *ptr = m_base, *last = NULL;

				while(ptr)
				{
					if(ptr->hHook == hHook)
					{
						if(last)
							last->next = ptr->next;
						else if(ptr == m_base)
						{
							m_base = NULL;
							delete m_base;
							return;
						}

						delete ptr;
						return;
					}
					last = ptr;
					ptr = ptr->next;
				}
			}

			//remove all hook objects and their process objects
			void DeleteAllHooks()
			{
				CHookDll <CallbackObject> *ptr = m_base, *next = NULL;

				while(ptr)
				{
					next = ptr->next;
					delete ptr;
					ptr = next;
				}
			}

		/*************************************************************/
		/*			Hook Injection Functions						 */
		/*************************************************************/

			//Loads a hook
			BOOL InjectLibrary(HOOK_HANDLE hHook)
			{
				return PerformLoopOperation(HOOK_OP_INJECT, hHook);
			}

			//Loads all hooks
			BOOL InjectLibraries()
			{
				return PerformLoopOperation(HOOK_OP_INJECT);
			}

			//Unloads a hook
			BOOL EjectLibrary(HOOK_HANDLE hHook)
			{
				return PerformLoopOperation(HOOK_OP_EJECT, hHook);
			}

			//Unloads all hooks
			BOOL EjectLibraries()
			{
				return PerformLoopOperation(HOOK_OP_EJECT);
			}

		/*************************************************************/
		/*			Hook IPC Functions								 */
		/*************************************************************/

			//communicate with a hooked dll with only a 4 byte code
			BOOL SendData(HOOK_HANDLE hHook, UINT i32bit)
			{
				return SendData(hHook, (LPBYTE)&i32bit, sizeof(UINT));
			}

			//communicate with a hooked dll with any size data
			BOOL SendData(HOOK_HANDLE hHook, LPBYTE lpData, UINT iSize)
			{
				m_DataPtr = lpData;
				m_DataSize = iSize;
				return PerformLoopOperation(HOOK_OP_SEND, hHook);
			}

			//communicate with all hooked dlls with only a 4 byte code
			BOOL SendDataAll(UINT i32bit)
			{
				return SendDataAll((LPBYTE)&i32bit, sizeof(UINT));
			}

			//communicate with all hooked dlls with any size data
			BOOL SendDataAll(LPBYTE lpData, UINT iSize)
			{
				m_DataPtr = lpData;
				m_DataSize = iSize;
				return PerformLoopOperation(HOOK_OP_SEND);
			}

			//sets up the local exe ipc server
			BOOL SetIPC(LPSTR lpServer, UINT Server_sz, UINT Client_sz)
			{
				//clean up after any old data
				if(m_lpMemoryServer)
					delete m_lpMemoryServer;

				//init data
				strcpy_s(m_Server, MAX_PATH, lpServer);
				m_Alloc = Server_sz;
				m_cAlloc = Client_sz;

				//make sure the callback object knows to use this hook manager
				m_server_callback.SetHookManager(this);

				//create the server
				m_lpMemoryServer = new CSharedMemory <CHookServerFunctor <CallbackObject>> (m_Server);
				m_lpMemoryServer->Malloc(m_Alloc);
				m_lpMemoryServer->SetReadCallback(&m_server_callback);

				if(!m_lpMemoryServer->Listen())
					return FALSE;

				return TRUE;
			}

			//handles incoming ipc data
			void HandleCallback(HOOK_HANDLE hHook, DWORD dwProcess, LPVOID lpDat, UINT iSz)
			{
				CHookDll <CallbackObject> *dllNode;
				CProcessHook <CallbackObject> *process_ptr;
				CallbackObject *callback_ptr = NULL;


				//try to use either the hook object or process object's ipc functor if possible
				dllNode = GetDllNode(hHook);
				process_ptr = GetProcess(hHook, dwProcess);

				if(dllNode)
				{
					if(dllNode->read_callback != NULL)
						callback_ptr = dllNode->read_callback;
				}
				else if(process_ptr)
				{
					if(process_ptr->read_callback != NULL)
						callback_ptr = process_ptr->read_callback;
				}

				//if we have a valid pointer then call the functor
				if(callback_ptr)
				{
					CallbackObject &mycallback = *callback_ptr;
					mycallback(hHook, dwProcess, lpDat, iSz);
				}
			}

		/*************************************************************/
		/*			Hook Info										 */
		/*************************************************************/

			//get the number of processes currently hooked
			UINT GetHookedProcesses()
			{
				return m_HookedProcesses;
			}

			//get a handle to the first hook object
			HOOK_HANDLE GetFirstHook()
			{
				if(!m_base)
					return NULL;
				m_DllLoop = m_base->next;
				return m_base->hHook;
			}

			//hook object iterator
			HOOK_HANDLE GetNextHook()
			{
				HOOK_HANDLE next;
				if(!m_DllLoop)
					return NULL;
				next = m_DllLoop->hHook;
				m_DllLoop = m_DllLoop->next;
				return next;
			}

			//get first process object
			CProcessHook <CallbackObject> *GetFirstProcess(HOOK_HANDLE hHook)
			{
				CHookDll <CallbackObject> *dll = GetDllNode(hHook);
				if(!dll || !dll->HookList)
					return NULL;
				dll->ProcessLoop = dll->HookList->next;
				return dll->HookList->ProcessHook;
			}

			//process object iterator
			CProcessHook <CallbackObject> *GetNextProcess(HOOK_HANDLE hHook)
			{
				CHookDll <CallbackObject> *dll = GetDllNode(hHook);
				if(!dll || !dll->ProcessLoop)
					return NULL;
				CProcessHook <CallbackObject> *next = dll->ProcessLoop->ProcessHook;
				dll->ProcessLoop = dll->ProcessLoop->next;
				return next;
			}

			//get a hook object by its handle
			CHookDll <CallbackObject> *GetDllNode(HOOK_HANDLE hHook)
			{
				CHookDll <CallbackObject> *ptr = m_base;
				while(ptr)
				{
					if(ptr->hHook == hHook)
					{
						SetLastError(ERROR_SUCCESS);
						return ptr;
					}
					ptr = ptr->next;
				}
				SetLastError(ERROR_INVALID_HANDLE);
				return NULL;
			}

			//get the current state of a hook object
			BOOL GetState(HOOK_HANDLE hHook)
			{
				CHookDll <CallbackObject> *dllNode = GetDllNode(hHook);
				if(!dllNode)
					return FALSE;
				return dllNode->state;
			}

			//get the number of times an injection api has been executed successfully
			INT GetTimesHooked()
			{
				return m_timeshooked;
			}

			//get the directory currently being used for dlls
			LPCSTR GetWorkingDirectory()
			{
				return (LPCSTR)m_WorkingDirectory;
			}

		/*************************************************************/
		/*			Internal Management								 */
		/*************************************************************/
		private:

			//creates a new hook object
			CHookDll <CallbackObject> *CreateDllNode()
			{
				//no two handles should ever be the same
				++m_next_handle;

				//allocate
				CHookDll <CallbackObject> *ptr;
				if(!m_base)
				{
					m_base = new CHookDll <CallbackObject>;
					ptr = m_base;
				}
				else
				{
					ptr = m_base;
					while(ptr->next)
						ptr = ptr->next;
					ptr->next = new CHookDll <CallbackObject>;
					ptr = ptr->next;
				}

				//init
				ptr->hHook = m_next_handle;
				ptr->next = NULL;
				ptr->HookList = NULL;

				return ptr;
			}

			//creates a process hook object
			CProcessHook <CallbackObject> *CreateProcessNode(HOOK_HANDLE hHook)
			{
				HookProcess <CallbackObject> *ptr;
				CHookDll <CallbackObject> *dllNode;

				//no two ids should ever be the same
				++m_next_id;

				//allocate
				dllNode = GetDllNode(hHook);
				if(!dllNode)
					return NULL;

				if(!dllNode->HookList)
				{
					dllNode->HookList = new HookProcess <CallbackObject>;
					ptr = dllNode->HookList;
				}
				else
				{
					ptr = dllNode->HookList;
					while(ptr->next)
						ptr = ptr->next;
					ptr->next = new HookProcess <CallbackObject>;
					ptr = ptr->next;
				}

				//init
				ptr->id = m_next_id;
				ptr->next = NULL;

				//allocate process hook
				ptr->ProcessHook = new CProcessHook <CallbackObject> (hHook);

				return ptr->ProcessHook;
			}

			//gets a process by its hook and process ids
			CProcessHook <CallbackObject> *GetProcess(HOOK_HANDLE hHook, DWORD dwPid)
			{
				BOOL res = PerformLoopOperation(HOOK_OP_GETP, hHook, dwPid);
				if(!res)
					return NULL;
				else
					return m_pResult;
			}

			//an iterator to perferm various internal tasks
			//
			//   This function cycles through hook objects and / or process objects.
			//   and performs a requested operation (bOp) on them.  bOp can be any of
			//	 the values declared above in the "Loop Commands" section.  If a hook
			//   handle is supplied or a process id, that hook / process object will be
			//   the only one affected by the function.
			//

				//these member hold loop data
				LPBYTE m_DataPtr;
				UINT m_DataSize;
				CProcessHook <CallbackObject> *m_pResult;

			BOOL PerformLoopOperation(BYTE bOp, HOOK_HANDLE hFindHook=0, DWORD dwFindPid=0)
			{
				CProcessHook <CallbackObject> *process_ptr;
				DWORD dwSearch;
				CHookDll <CallbackObject> *dllNode = NULL;

				// setup the search
				HOOK_HANDLE hSearch = hFindHook;

				// whether there was an error
				BOOL flag = FALSE;

				HOOK_HANDLE hHook = GetFirstHook();
				while(hHook)
				{

					// if a search handle was specified we want to check for
					// that specific handle
					if(!hFindHook)
						hSearch = hHook;
					else
						dwSearch = -1;

					// the current hook meets the criteria
					if(hHook == hSearch)
					{

						// start searching processes
						process_ptr = GetFirstProcess(hHook);
						while(process_ptr)
						{

							// finds a process in the same way we found the hook above
							if(!dwFindPid)
								dwSearch = process_ptr->GetPid();
							else
								dwSearch = -1;

							if(dwSearch == process_ptr->GetPid())
							{

								//perform an operation
								switch(bOp)
								{

									///////////////////////
									// Inject a dll
									///////////////////////
									case HOOK_OP_INJECT:
										if(!process_ptr->InjectDll())
											flag = TRUE;
										else
										{
											dllNode = GetDllNode(hHook);
											if(dllNode)
												dllNode->state = TRUE;
										}
										break;

									///////////////////////
									// Eject a dll
									///////////////////////
									case HOOK_OP_EJECT:
										if(!process_ptr->EjectDll())
											flag = TRUE;
										else
										{
											dllNode = GetDllNode(hHook);
											if(dllNode)
												dllNode->state = FALSE;
										}
										break;

									///////////////////////
									// Send a message to a dll
									///////////////////////
									case HOOK_OP_SEND:
										if(!process_ptr->SendData(m_DataPtr, m_DataSize))
											flag = TRUE;
										break;

									///////////////////////
									// Set up ipc info
									///////////////////////
									case HOOK_OP_SETIPC:
										process_ptr->SetIPC(m_Server, m_Alloc);
										break;

									///////////////////////
									// Get A Process Pointer
									///////////////////////
									case HOOK_OP_GETP:
										m_pResult = process_ptr;
										return TRUE;

									// bad option
									default:
										return FALSE;
								}
							}

							// iterate
							process_ptr = GetNextProcess(hHook);
						}
					}

					// iterate
					hHook = GetNextHook();
				}

				// if hook was successful, increment the count
				if(dllNode && dllNode->state == TRUE)
					++m_timeshooked;

				return !flag;
			}

		/*************************************************************/
		/*			Private Variables								 */
		/*************************************************************/

			//SETTINGS
			char m_WorkingDirectory[MAX_PATH];						//dll search directory

			//HOOK INFO
			UINT m_next_handle;										//next hook handle to be assigned
			UINT m_next_id;											//next internal hook process id to be assigned
			BOOL m_inhook;											//whether or not there is a hook active
			INT m_timeshooked;										//total times an injection api succeeded
			CHookDll <CallbackObject> *m_base;						//first hook object in the manager
			CHookDll <CallbackObject> *m_DllLoop;					//hook object iterator

			//IPC INFO
			UINT m_Alloc;											//ipc mem size
			UINT m_cAlloc;											//client ipc mem size
			CHAR m_Server[MAX_PATH];								//ipc server name
			CHookServerFunctor <CallbackObject> m_server_callback;	//handler object for incoming ipc
			CSharedMemory <CHookServerFunctor <CallbackObject>> *m_lpMemoryServer;	//The ipc server
	};