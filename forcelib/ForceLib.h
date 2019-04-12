/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  ForceLib.h

  This file is part of the Packet Edit Studio Project.
  version: ALPHA

  *Distributed under the terms of the zlib/libpng public license.*
  
  Author: 

	"Yoda"

	See below copyright.
  
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

// Force library dll loader include

/*

-> ForceLib.h

This is a small header file to make the usage of ForceLibrary.dll easier
with C/C++.
Just include this file and you can start using the functions.

  by yoda

*/

/*MSVC++8 COMPATABILITY*/
#if _MSC_VER > 1399
#define _CRT_SECURE_NO_DEPRECATE
#else
#define _stricmp stricmp
#endif

#ifndef __ForceLibraryHeader__
  #define __ForceLibraryHeader__

// the dll prototypes
typedef DWORD (WINAPI* fForceLibrary)(CHAR* szLibraryPath,PROCESS_INFORMATION* pProcInfo);
typedef BOOL  (WINAPI* fTrapEntry)(DWORD dwEntryPoint,PROCESS_INFORMATION *pPI);
typedef BOOL  (WINAPI* fForceLibraryDBG)(CHAR* szTargetLib,DWORD dwEntryPoint,PROCESS_INFORMATION *pPI);
typedef DWORD (WINAPI* fPerformCleanup)(DWORD dwEntryPoint,PROCESS_INFORMATION *pPI);

// functions
BOOL  LoadFL();
DWORD ForceLibrary(CHAR* szLibraryPath,PROCESS_INFORMATION* pProcInfo);
BOOL  TrapEntry(DWORD dwEntryPoint,PROCESS_INFORMATION *pPI);
BOOL  ForceLibraryDBG(CHAR* szTargetLib,DWORD dwEntryPoint,PROCESS_INFORMATION *pPI);
DWORD PerformCleanup(DWORD dwEntryPoint,PROCESS_INFORMATION *pPI);

// constants
const CHAR*   szDllName         = "ForceLibrary.dll";
const PSTR    szFunctNameFL     = "ForceLibrary";
const PSTR    szFunctNameTE     = "TrapEntry";
const PSTR    szFunctNameFLDBG  = "ForceLibraryDBG";
const PSTR    szFunctNamePC     = "PerformCleanup";

const CHAR*   szError           = "ERROR";
const DWORD   dwMsgFlags        = MB_ICONERROR | MB_TASKMODAL;
const CHAR*   szDllNotFound     = "ForceLibrary.dll wasn't found !";
const CHAR*   szGetProcError    = "Error during Dll initialization !";

// variables
fForceLibrary     _ForceLibrary;
fTrapEntry        _TrapEntry;
fForceLibraryDBG  _ForceLibraryDBG;
fPerformCleanup   _PerformCleanup;

HANDLE   hFL = 0;


BOOL LoadFL()
{
	hFL = LoadLibrary(szDllName);
	if (!hFL)
	{
		//MessageBox(0,szDllNotFound,szError,dwMsgFlags);
		return FALSE;
	}
	_ForceLibrary = (fForceLibrary)GetProcAddress((HMODULE)hFL,szFunctNameFL);
	_TrapEntry = (fTrapEntry)GetProcAddress((HMODULE)hFL,szFunctNameTE);
	_ForceLibraryDBG = (fForceLibraryDBG)GetProcAddress((HMODULE)hFL,szFunctNameFLDBG);
	_PerformCleanup = (fPerformCleanup)GetProcAddress((HMODULE)hFL,szFunctNamePC);

	if (!_ForceLibrary ||
		!_TrapEntry ||
		!_ForceLibraryDBG || 
		!_PerformCleanup)
	{
		MessageBox(0,szGetProcError,szDllName,dwMsgFlags);
		return FALSE;
	}
	return TRUE;
}


BOOL TrapEntry(DWORD dwEntryPoint,PROCESS_INFORMATION *pPI)
{
	if (!hFL)
		if (!LoadFL())
			return FALSE;
	return _TrapEntry(dwEntryPoint,pPI);
}

BOOL  ForceLibraryDBG(CHAR* szTargetLib,DWORD dwEntryPoint,PROCESS_INFORMATION *pPI)
{
	if (!hFL)
		if (!LoadFL())
			return FALSE;
	return _ForceLibraryDBG(szTargetLib,dwEntryPoint,pPI);
}

DWORD PerformCleanup(DWORD dwEntryPoint,PROCESS_INFORMATION *pPI)
{
	if (!hFL)
		if (!LoadFL())
			return 0;
	return _PerformCleanup(dwEntryPoint,pPI);
}

  #endif