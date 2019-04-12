/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  stdafx.h

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

// The precompiled header file, all includes are included here

	#pragma once

	template <class CallbackObject> class CHookDllInterface;

	template <class CallbackObject>
	class CHookDLLServerFunctor : public CHookFunctor
	{
		public:
			CHookDLLServerFunctor()
			{
			}

			virtual ~CHookDLLServerFunctor()
			{
			}

			void operator() (LPVOID lpData, INT iSz)
			{
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

				if(m_HookManager)
					m_HookManager->HandleCallback(*pHook, *pPid, dataptr, datasize);
			}

			void SetHookManager(CHookDllInterface <CallbackObject> *manager)
			{
				m_HookManager = manager;
			}

		private:
			CHookDllInterface <CallbackObject> *m_HookManager;
	};

	template <class CallbackObject>
	class CHookDllInterface
	{
		public:
			CHookDllInterface()
			{
				lpMemoryServer = NULL;
				lpMemoryClient = NULL;

				mybuffer = NULL;

				m_hHook = 0;

				m_dwPid = GetCurrentProcessId();
				GetProcessName(chHookExe, MAX_PATH);
				_itoa_s(m_dwPid, chPid, 50, 10);
			}

			~CHookDllInterface()
			{
				if(lpMemoryServer)
					delete lpMemoryServer;

				if(lpMemoryClient)
					delete lpMemoryClient;

				if(mybuffer)
					delete [] mybuffer;
			}

			BOOL SetIPC(LPSTR lpServer, UINT Server_sz, UINT Client_sz, CallbackObject *callback)
			{
				char chClient[MAX_PATH*2];

				if(lpMemoryServer)
					delete lpMemoryServer;

				if(mybuffer)
					delete [] mybuffer;

				mybuffer = new BYTE [Server_sz+sizeof(HOOK_HANDLE)+sizeof(DWORD)];

				read_callback = callback;

				server_callback.SetHookManager(this);

				strcpy_s(chClient, MAX_PATH*2, lpServer);
				strcat_s(chClient, MAX_PATH*2, chHookExe);
				strcat_s(chClient, MAX_PATH*2, chPid);

				lpMemoryServer = new CSharedMemory <CHookDLLServerFunctor <CallbackObject>> (chClient);
				lpMemoryServer->Malloc(Client_sz);
				lpMemoryServer->SetReadCallback(&server_callback);

				if(!lpMemoryServer->Listen())
					return FALSE;

				lpMemoryClient = new CSharedMemory <void*> (lpServer);

				if(!lpMemoryClient->OpenMemory(Server_sz))
				{
					delete lpMemoryClient;
					delete lpMemoryServer;
					lpMemoryClient = NULL;
					lpMemoryServer = NULL;
					return FALSE;
				}

				return TRUE;
			}

			BOOL SendData(LPBYTE Data, UINT sz)
			{
				HOOK_HANDLE *pHook;
				DWORD *pPid;
				UINT headersize;
				LPBYTE datastart;

				if(!mybuffer)
					return FALSE;

				headersize = sizeof(HOOK_HANDLE) + sizeof(DWORD);

				pHook = (HOOK_HANDLE*)mybuffer;
				*pHook = m_hHook;

				pPid = (DWORD*) (mybuffer + sizeof(HOOK_HANDLE));
				*pPid = m_dwPid;

				datastart = mybuffer + headersize;
				memcpy(datastart, Data, sz);

				return lpMemoryClient->WriteData((LPVOID)mybuffer, sz + headersize);
			}

			VOID HandleCallback(HOOK_HANDLE hHook, DWORD dwPid, LPVOID lpData, UINT iSz)
			{
				CallbackObject &mycallback = *read_callback;

				if(!m_hHook)
				{
					m_hHook = hHook;
					return;
				}

				mycallback(m_hHook, dwPid, lpData, iSz);
			}

			HOOK_HANDLE GetHookHandle()
			{
				return m_hHook;
			}

		private:
			CHookDLLServerFunctor <CallbackObject> server_callback;
			CallbackObject *read_callback;
			char chHookExe[MAX_PATH];
			char chPid[50];
			LPBYTE mybuffer;
			
			DWORD m_dwPid;
			HOOK_HANDLE m_hHook;

			CSharedMemory <CHookDLLServerFunctor <CallbackObject>> *lpMemoryServer;
			CSharedMemory <void*> *lpMemoryClient;

			void GetProcessName(char *buffer, int size)
			{
				size_t i;
				char *ptr;

				GetModuleFileName(NULL, buffer, size);

				for(i = 0 ; i <= strlen(buffer) ; ++i)
					if(buffer[i] == '\\')
						ptr = &buffer[i+1];

				strcpy_s(buffer, size, ptr);
			}
	};