/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  HookManager.cpp

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

#include "stdafx.h"
#include "../forcelib/forcelib.h"

/*************************************************************/
/*			Local Utility Functions							 */
/*************************************************************/

	//get pe structure for a process
	bool GetProcessOf(DWORD process_id, PROCESSENTRY32 *process)
	{
		HANDLE handle ;
		static DWORD Pid;
		process->dwSize = sizeof(PROCESSENTRY32);
		handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		Pid = GetCurrentProcessId();

		if(Process32First(handle, process))
		{
			do
			{
				if( (process_id == process->th32ProcessID) && (Pid != process->th32ProcessID) )
				{
					CloseHandle(handle);
					return true;
				}
			}while(Process32Next(handle, process));
		}

		CloseHandle(handle);
		return false;
	}

	//get main thread te structure for a process
	bool GetThreadOf(DWORD ProcessID, THREADENTRY32 *thread)
	{
		HANDLE handle;
		thread->dwSize = sizeof(THREADENTRY32);
		handle = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

		if(Thread32First(handle, thread))
		{
			do
			{
				if(thread->th32OwnerProcessID == ProcessID)
				{
					CloseHandle(handle);
					return true;
				}
			}while(Thread32Next(handle, thread));
		}

		CloseHandle(handle);
		return false;
	}

/*************************************************************/
/*			CHookBase										 */
/*************************************************************/

	//inject a single library
	BOOL CHookBase::HBInject(DWORD dwProcess, LPSTR lpDll)
	{
		PROCESSENTRY32 pe32;
		THREADENTRY32 te32;
		PROCESS_INFORMATION PI;
		int tries;

		tries = 0;

		// simple validate for dll
		if(!HBFileExists(lpDll))
			return FALSE;

		// clean up after old hooks
		HBEject(dwProcess, lpDll);

		// get access
		while(!GetProcessOf(dwProcess, &pe32))
			Sleep(5);

		while(!GetThreadOf(pe32.th32ProcessID, &te32))
			Sleep(2);

		PI.dwProcessId = pe32.th32ProcessID;
		PI.dwThreadId = te32.th32ThreadID;
		PI.hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pe32.th32ProcessID);

		if(!PI.hProcess)
		{
			SetLastError(ERROR_ACCESS_DENIED);
			CloseHandle(PI.hProcess);
			return FALSE;
		}

		// inject
		if(!ForceLibrary(lpDll, &PI))
		{
			CloseHandle(PI.hProcess);
			SetLastError(ERROR_INJECT_FAILURE);
			return FALSE;
		}

		CloseHandle(PI.hProcess);

		return TRUE;
	}

	BOOL CHookBase::HBEject(DWORD dwProcess, LPSTR lpDll)
	{
		PROCESSENTRY32 pe;
		MODULEENTRY32 ModuleEntry32;

		// get correct dll name
		LPSTR lpHookDll = strrchr(lpDll, '\\');
		if(!lpHookDll)
			lpHookDll = strrchr(lpDll, '/');
		if(lpHookDll)
			++lpHookDll;
		else
			lpHookDll = lpDll;

		// get module base address
		HMODULE hDll = NULL;
		if(!GetProcessOf(dwProcess, &pe))
			return false;

		HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pe.th32ProcessID);
	
		if (hSnapShot == (HANDLE)-1)
			return false;

		ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

		do
		{
			if(!strcmp(ModuleEntry32.szModule, lpHookDll))
				hDll = ModuleEntry32.hModule;

		} while (Module32Next(hSnapShot, &ModuleEntry32));

		CloseHandle(hSnapShot);

		if(!hDll)
		{
			SetLastError(ERROR_FILE_NOT_FOUND);
			return FALSE;
		}

		// get process handle
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID);

		if(!hProcess)
			return FALSE;

		PTHREAD_START_ROUTINE pfnRemote =(PTHREAD_START_ROUTINE) GetProcAddress(GetModuleHandle("Kernel32"), "FreeLibrary");

		if(!pfnRemote)
			return FALSE;

		// call FreeLibrary in the target proceess
		HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, pfnRemote, hDll, 0, NULL);

		if(!hThread)
			return FALSE;

		WaitForSingleObject(hThread, INFINITE);

		CloseHandle(hProcess);
		CloseHandle(hThread);

		SetLastError(ERROR_SUCCESS);
		return TRUE;
	}

	//inject dll into all running processes (execept this one)
	BOOL CHookBase::HBInjectAll(LPSTR lpDll)
	{
		PROCESSENTRY32 pe32;

		BOOL flag = TRUE;
		DWORD MyPid = GetCurrentProcessId();

		pe32.dwSize = sizeof(PROCESSENTRY32);
		HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if(Process32First(handle, &pe32))
			while(Process32Next(handle, &pe32))
				if(MyPid != pe32.th32ProcessID)
					if(!HBInject(pe32.th32ProcessID, lpDll))
						flag = FALSE;

		CloseHandle(handle);

		return !flag;
	}

	//eject dll from all running processes
	BOOL CHookBase::HBEjectAll(LPSTR lpDll)
	{
		PROCESSENTRY32 pe32;

		BOOL flag = TRUE;
		DWORD MyPid = GetCurrentProcessId();

		pe32.dwSize = sizeof(PROCESSENTRY32);
		HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if(Process32First(handle, &pe32))
			while(Process32Next(handle, &pe32))
				if(MyPid != pe32.th32ProcessID)
					if(!HBEject(pe32.th32ProcessID, lpDll))
						flag = FALSE;

		CloseHandle(handle);

		return !flag;
	}

	//check if a file exists
	BOOL CHookBase::HBFileExists(LPCSTR lpName)
	{
		WIN32_FIND_DATA finddata;
		HANDLE handle = FindFirstFile(lpName,&finddata);
		return (handle!=INVALID_HANDLE_VALUE);
	}

	//get the main module directory (current process)
	BOOL CHookBase::HBGetModuleDirectory(LPSTR lpBuffer, UINT iSize)
	{
		int i;
		char tmp[MAX_PATH];

		memset(tmp, 0, MAX_PATH);

		if(!GetModuleFileName(NULL, tmp, MAX_PATH))
			return FALSE;

		for (i = (int)strlen(tmp); tmp[i] != '\\' && i > 0; --i) tmp[i] = '\0';
		tmp[i] = '\0';

		if((UINT)strlen(tmp) > iSize)
		{
			SetLastError(ERROR_INSUFFICIENT_BUFFER);
			return FALSE;
		}

		strcpy_s(lpBuffer, MAX_PATH, tmp);
	
		SetLastError(ERROR_SUCCESS);
		return TRUE;
	}

	//see if we can get a valid handle to the process
	BOOL CHookBase::HBCanAccessProcess(DWORD dwPid)
	{
		PROCESSENTRY32 pe32;
		if(!GetProcessOf(dwPid, &pe32))
		{
			SetLastError(ERROR_FILE_NOT_FOUND);
			return FALSE;
		}

		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
		CloseHandle(hProcess);
		if(!hProcess)
		{
			SetLastError(ERROR_ACCESS_DENIED);
			return FALSE;
		}
		else
		{
			SetLastError(ERROR_SUCCESS);
			return TRUE;
		}
	}