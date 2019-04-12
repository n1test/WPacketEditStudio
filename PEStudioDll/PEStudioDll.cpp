/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  PEStudioDll.cpp

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

// dll main
#include "stdafx.h"


#ifdef _MANAGED
#pragma managed(push, off)
#endif

namespace Process
{
	CHAR name[MAX_PATH];
	CHAR dlldir[MAX_PATH];
	DWORD pid;
}

namespace CaptureEngine
{
	bool Capture;
	char script[255];
	bool LoadPHP;
	bool EjectLock;
}

namespace PHPEngine
{
	PHPScripts ScriptList;
	bool PHPLock;
}

int buffersize;
unsigned char *sendbuffer;

INIParser *pConfigParser;

LPSTR lpConfigFiles[] = { "pestudio.ini", "\0" };

CHookDllInterface <DLLIPCFunctor> HookInterface;
DLLIPCFunctor HookHandler;

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

void GetDllDirectory(HMODULE hdll, char *buffer, int size)
{
	size_t i;

	GetModuleFileName(hdll, buffer, size);
	for(i = strlen(buffer) ; i >= 0 ; --i)
		if(buffer[i] == '\\')
		{
			buffer[i] = '\0';
			break;
		}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{

	switch(ul_reason_for_call)
	{

		case DLL_PROCESS_ATTACH:

			DisableThreadLibraryCalls(hModule);

			GetProcessName(Process::name, MAX_PATH);
			GetDllDirectory(hModule, Process::dlldir, MAX_PATH);
			Process::pid = GetCurrentProcessId();

			CaptureEngine::Capture = false;
			CaptureEngine::LoadPHP = false;
			CaptureEngine::EjectLock = false;

			pConfigParser = new INIParser();
			pConfigParser->SetWorkingDirectory(Process::dlldir);
			pConfigParser->LoadFiles(lpConfigFiles);

			PHPEngine::ScriptList.SetPath();
			PHPEngine::PHPLock = false;

			buffersize = IPC_MEMSIZE;
			sendbuffer = new unsigned char [buffersize];

			HookInterface.SetIPC("PacketEditStudioServer", IPC_MEMSIZE, sizeof(UINT), &HookHandler);

			InstallHooks();

			break;

		case DLL_PROCESS_DETACH:
			RemoveHooks();
			delete pConfigParser;
			delete [] sendbuffer;
			break;

		default:
			break;
	}

    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

