/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  ScriptDlg.h

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

// This dialog is where php scripts are managed

	#pragma once
	#include "afxwin.h"

	/* STRUCTURE FOR THE PHP SCRIPT LINKED LIST */
	typedef struct _PHPScript
	{
		bool loaded;				// whether the php parser should apply this script
		char name[255];				// name of the script ($script_name value)
		char author[255];			// author of the script ($script_author value)
		char version[255];			// version of the script ($script_version value)
		char path[MAX_PATH];		// exact location of the script (beginning with drive letter)
		struct _PHPScript *next;	// next element in the linked list
	} PHPScript;


	/*************************************************************/
	/*			CScriptsDlg										 */
	/*															 */
	/*	The dialog for handling php loading.					 */
	/*************************************************************/
	class CScriptsDlg : public CDialog
	{
		DECLARE_DYNAMIC(CScriptsDlg)

		public:

			enum { IDD = IDD_SCRIPTSDLG };

			/////////////////////////////
			//  Constructor / Destructor
			/////////////////////////////

				CScriptsDlg(CWnd* pParent = NULL);

				virtual ~CScriptsDlg();

			/////////////////////////////
			//  Script Management
			/////////////////////////////

				//add php script to the list
				INT AddItem(LPSTR txt, LPSTR path);
				
				//remove currently selected php script from the list
				void RemoveItem();

			/////////////////////////////
			//  Specific Control Events
			/////////////////////////////

				void OnListDoubleClick();

		protected:

				void OnCheckChange();

			/////////////////////////////
			//  Window Events
			/////////////////////////////

				virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
				virtual  BOOL OnInitDialog();

				DECLARE_MESSAGE_MAP()

			/////////////////////////////
			//  Internal Helper Functions
			/////////////////////////////

				// save the current php list to \phpengine\scripts.xml
				void SaveXMLList(bool unload=false);

				//create a new php script linked list element
				PHPScript *AllocateItemSpace();

			/////////////////////////////
			//  Private Variables
			/////////////////////////////

				char list_path[MAX_PATH];				//path where the list file resides
				PHPScript *base;						// base PHPScript element

			/////////////////////////////
			//  Controls
			/////////////////////////////

				CCheckListBox m_ScriptList;				//the list box which displays the scripts and controls script state input
	};
