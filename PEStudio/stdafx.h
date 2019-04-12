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
/*			Warning Control									 */
/*************************************************************/

	//turn off casting differant sized data type warnings
	#pragma warning ( disable : 4311 )
	#pragma warning ( disable : 4312 )

/*************************************************************/
/*			Library Version Control							 */
/*************************************************************/

	// for shared headers
	#define PESTUDIO_EXE

	#ifndef VC_EXTRALEAN
		#define VC_EXTRALEAN
	#endif

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
/*			AFX Includes									 */
/*************************************************************/

	#define _AFX_ALL_WARNINGS

	#include <afxwin.h>
	#include <afxext.h>
	#include <afxdisp.h>
	#include <afxcview.h>
	#include <afxdtctl.h>
	#include <afxcmn.h>
	#include <afxpriv.h>
	#include <afxrich.h>
	#include <afxhtml.h>

/*************************************************************/
/*			Win32 Includes									 */
/*************************************************************/

	#include <tlhelp32.h>
	#include <psapi.h>
	#include <winsock2.h>

	#pragma comment(lib, "psapi")

/*************************************************************/
/*			Standard Library Includes						 */
/*************************************************************/
	#include <sys\types.h> 
	#include <sys\stat.h> 
	#include <cstddef>
	#include <cmath>
	#include <iostream>
	#include <fstream>

/*************************************************************/
/*			Packet Edit Studio Includes						 */
/*************************************************************/

	//size of buffer for converting data
	#define STATIC_BUFFER_SIZE 1536

	//IPC
	#include "ipc.h"
	#include "PacketBuffer/PacketBuffer.h"

	//Scintilla
	#include "../scintilla/include/platform.h"
	#include "../scintilla/include/scintilla.h"
	#include "../scintilla/include/SciLexer.h"

	//Hooking
	#include "th32.h"
	#include "../HookAPI/HookAPI.h"
	#include "Hook/PESHook.h"
	#include "Hook/PESHookList.h"

	//tinyxml
	#include "tinyxml.h"
	#pragma comment(lib, "../tinyxml/tinyxmld.lib")

	//Main Includes
	#include "resource.h"
	#include "INIParser.h"
	#include "convert.h"
	#include "ProcessList.h"
	#include "PEStudio.h"

	//render / controls
	#include "ntgraph.h"
	#include "EnBitmap.h"
	#include "color.h"
	#include "themeutil.h"
	#include "CustomTabCtrl.h"
	#include "math.h"
	#include "3DMeterCtrl.h"
	#include "MemDC.h"
	#include "FlatToolTipCtrl.h"
	#include "ListCtrlStyled.h"
	#include "MDITabs/MDITabs.h"
	#include "ScintillaDocView.h"
	#include "ScintillaDemoView.h"
	#include "SizingTabCtrlBar.h"
	#include "sizecbar.h"
	#include "PHPResultsView.h"

	//Dialogs / Forms
	#include "AboutDlg.h"
	#include "ProcessSelectDlg.h"
	#include "SniffDetailsDlg.h"
	#include "TraceMeterDlg.h"
	#include "SniffStatsDlg.h"
	#include "SniffActivityDlg.h"
	#include "SniffLogDlg.h"
	#include "TraceConsoleDlgBar.h"
	#include "ScriptsDlg.h"
	#include "ActionsConsoleDlgBar.h"
	#include "CalculatorDlgBar.h"
	#include "MainFrm.h"
	#include "ChildFrm.h"
	#include "SniffOptions.h"
	#include "PHPOptions.h"
	#include "OptionsDlg.h"

	//Documents
	#include "PHPDoc.h"
	#include "SniffResultsDoc.h"

	//Views
	#include "PHPEditorView.h"
	#include "PEStudioView.h"
	#include "PacketListView.h"
