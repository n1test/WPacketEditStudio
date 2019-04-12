#include "stdafx.h"

#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <commdlg.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <winbase.h>
#include <vdmdbg.h>
#include <Windowsx.h>
#include <commctrl.h>
#include <shellapi.h>
#include <shlwapi.h>

#include "..\forcelib\ForceLib.h"

bool GetProcessOf(char exename[], PROCESSENTRY32 *process)
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
			if( (_stricmp(process->szExeFile, exename) == 0) && (Pid != process->th32ProcessID) )
			{
				CloseHandle(handle);
				return true;
			}
		}while(Process32Next(handle, process));
	}

	CloseHandle(handle);
	return false;
}

unsigned int GetProcessCount(char exename[], PROCESSENTRY32 *process)
{
	HANDLE handle ;
	process->dwSize = sizeof(PROCESSENTRY32);
	handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	unsigned int count = 0;

	if(Process32First(handle, process))
	{
		do
		{
			if(_strcmpi(process->szExeFile, exename) == 0)
			{
				++count;
			}
		}while(Process32Next(handle, process));
	}

	CloseHandle(handle);
	return count;
}

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
 
bool fileExists(const char filename[]) 
{
	WIN32_FIND_DATA finddata;
	HANDLE handle = FindFirstFile(filename,&finddata);
	return (handle!=INVALID_HANDLE_VALUE);
}

bool InjectDll(char *exe, char *dll)
{
	PROCESSENTRY32 pe32;
	THREADENTRY32 te32;
	PROCESS_INFORMATION PI;

	if(!fileExists(dll))
		return false;

	while(!GetProcessOf(exe, &pe32))
		Sleep(5);

	while(!GetThreadOf(pe32.th32ProcessID, &te32))
		Sleep(2);

	PI.dwProcessId = pe32.th32ProcessID;
	PI.dwThreadId = te32.th32ThreadID;
	PI.hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pe32.th32ProcessID);

	if(!ForceLibrary(dll, &PI))
	{
		CloseHandle(PI.hProcess);
		return false;
	}

	CloseHandle(PI.hProcess);

	return true;
}

bool EjectDll(char *exe, char *dll)
{
	HANDLE hSnapShot, hProcess, hThread;
	PROCESSENTRY32 pe;
	MODULEENTRY32 ModuleEntry32;
	HMODULE hDll;

	hDll = NULL;
	if(!GetProcessOf(exe, &pe))
		return false;

	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pe.th32ProcessID);
	
	if (hSnapShot == (HANDLE)-1)
		return false;

	ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

	do
	{
		if(!strcmp(ModuleEntry32.szModule, dll))
			hDll = ModuleEntry32.hModule;

	} while (Module32Next(hSnapShot, &ModuleEntry32));

	CloseHandle(hSnapShot);

	if(!hDll)
		return false;

	hProcess =OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID);

	if(!hProcess)
		return false;

	PTHREAD_START_ROUTINE pfnRemote =(PTHREAD_START_ROUTINE) GetProcAddress(GetModuleHandle("Kernel32"), "FreeLibrary");

	if(!pfnRemote)
		return false;

	hThread = CreateRemoteThread(hProcess, NULL, 0, pfnRemote, hDll, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(hProcess);
	CloseHandle(hThread);

	return true;
}

int HookAllProcesses(char *dll)
{
	HANDLE handle ;
	static DWORD Pid;
	PROCESSENTRY32 *process;
	int hooked;

	hooked = 0;
	handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	process = new PROCESSENTRY32;

	Pid = GetCurrentProcessId();

	if(Process32First(handle, process))
	{
		do
		{
			if(Pid != process->th32ProcessID )
			{
				if(InjectDll(process->szExeFile, dll))
					++hooked;
			}
		}while(Process32Next(handle, process));
	}

	CloseHandle(handle);
	delete process;
	return hooked;
}


int UnhookAllProcesses(char *dll)
{
	HANDLE handle;
	static DWORD Pid;
	PROCESSENTRY32 process;
	int unhooked;

	unhooked = 0;
	handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	Pid = GetCurrentProcessId();

	process.dwSize = sizeof(PROCESSENTRY32);

	if(Process32First(handle, &process))
	{
		do
		{
			if(Pid != process.th32ProcessID )
			{
				if(EjectDll(process.szExeFile, dll))
					++unhooked;
			}
		}while(Process32Next(handle, &process));
	}

	CloseHandle(handle);

	return unhooked;
}