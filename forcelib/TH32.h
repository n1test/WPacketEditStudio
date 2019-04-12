/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  AboutDlg.h

  This file is part of the Packet Edit Studio Project.
  version: ALPHA

  *Distributed under the terms of the zlib/libpng public license.*
  
  Author: 

	"Yoda"
  
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

// Part of the force lib library loader

#ifndef __TH32_h__
#define __TH32_h__

#include <windows.h>
#include <tlhelp32.h>

// ToolHelp32 function prototypes
typedef HANDLE (WINAPI* fCreateToolhelp32Snapshot)
(
  DWORD dwFlags,       
  DWORD th32ProcessID  
);

typedef BOOL (WINAPI* fProcess32First)
(
  HANDLE hSnapshot,      
  LPPROCESSENTRY32 lppe  
);
 
typedef BOOL (WINAPI* fProcess32Next)
(
  HANDLE hSnapshot,      
  LPPROCESSENTRY32 lppe  
);

typedef BOOL (WINAPI* fModule32First)
(
  HANDLE hSnapshot,     
  LPMODULEENTRY32 lpme  
);
 
typedef BOOL (WINAPI* fModule32Next)
(
  HANDLE hSnapshot,     
  LPMODULEENTRY32 lpme  
);

extern fCreateToolhelp32Snapshot    _CreateToolhelp32Snapshot;
extern fProcess32First              _Process32First;
extern fProcess32Next               _Process32Next;
extern fModule32First               _Module32First;
extern fModule32Next                _Module32Next;

BOOL GetTh32();

#endif