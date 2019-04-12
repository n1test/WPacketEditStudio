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

/*************************************************************/
/*			Library Version Control							 */
/*************************************************************/

	// for shared headers
	#define PESTUDIO_DLL


	#ifndef WINVER
		#define WINVER 0x0501
	#endif

	#ifndef _WIN32_WINNT
		#define _WIN32_WINNT 0x0501
	#endif

	#ifndef _WIN32_WINDOWS
		#define _WIN32_WINDOWS 0x0410
	#endif

	#ifndef _WIN32_IE
		#define _WIN32_IE 0x0600
	#endif

/*************************************************************/
/*			Win32 Includes									 */
/*************************************************************/

	#define WIN32_LEAN_AND_MEAN

	#include <windows.h>
	#include <winsock2.h>

	#pragma comment(lib, "ws2_32.lib")

/*************************************************************/
/*			Standard Library Includes						 */
/*************************************************************/

	#include <cmath>
	#include <fstream>
	#include <ctime>

/*************************************************************/
/*			Packet Edit Studio Includes						 */
/*************************************************************/

	#include "../HookAPI/hookapi.h"
	#include "../IPC/ipc.h"
	#include "ipchandler.h"
	#include "../detourlibs/include/detours.h"
	#include "hooks.h"
	#include "../INIParser/INIParser.h"
	#include "../Convert/convert.h"
	#include "../tinyxml/tinyxml.h"
	#include "MemBlock.h"
	#include "PHPScripts.h"

	#pragma comment(lib, "../detourlibs/lib/detours.lib")
	#pragma comment(lib, "../tinyxml/tinyxmld.lib")

/*************************************************************/
/*			Globals											 */
/*************************************************************/

	//TODO: Organize this into a dll class such as

		/*
			class CPESDll
			{
				public:
					...
					void InitInstance();
					void ExitInstance();
				protected:
					variables
					...
					methods
					...
			}
		*/

	//information related to the current running process
	namespace Process
	{
		extern CHAR name[MAX_PATH];
		extern CHAR dlldir[MAX_PATH];
		extern DWORD pid;
	}

	//capture engine variables
	namespace CaptureEngine
	{
		extern bool Capture;
		extern char script[255];
		extern bool LoadPHP;
		extern bool EjectLock;
	}

	//php engine variables
	namespace PHPEngine
	{
		extern PHPScripts ScriptList;
		extern bool PHPLock;
	}

	//ipc buffer
	extern int buffersize;
	extern unsigned char *sendbuffer;

	//config
	extern INIParser *pConfigParser;

	//hook info
	extern CHookDllInterface <DLLIPCFunctor> HookInterface;
	extern DLLIPCFunctor HookHandler;

	//write ipc data
	void MemoryClient_Write(PEStudioIPC *Data, LPBYTE PacketData);
