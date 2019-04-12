/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  ScriptDlg.cpp

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


	#include "stdafx.h"

	#pragma warning ( disable : 4800 )

/*************************************************************/
/*			PHP Check Errors								 */
/*************************************************************/

	#define PHP_NO_ERR		    0
	#define PHP_ERR_SYNTAX	   -1
	#define PHP_ERR_NOT_INIT   -2
	#define PHP_ERR_NOT_STR    -3
	#define PHP_ERR_NOT_FOUND  -4
	#define PHP_ERR_TOO_BIG	   -5

	#define PHP_SCRIPT_NAMELEN -6

/*************************************************************/
/*			PHP Helper Functions							 */
/*************************************************************/

	//checks for a newline in a php variable
	#define PHP_ISNEWLINE(var, i) (var[i] == '\n' || var[i] == '\r') \
								   && (var[i+1] != '\n' && var[i+1] != '\r')

	//converts a php error code to string and displays an error message box
	void PHP_VarErrorMessage(LPSTR lpVariable, INT ErrCode, INT Line)
	{
		char tmp[1024];

		sprintf_s(tmp, 1024, "An error occurred parsing variable \"$%s\" on line %d: \n\n", lpVariable, Line);

		switch(ErrCode)
		{
			case PHP_ERR_TOO_BIG:
				strcat_s(tmp, 1024, "The variable cannot exceed 255 characters.");
				break;
			case PHP_ERR_SYNTAX:
				strcat_s(tmp, 1024, "Syntax Error");
				break;
			case PHP_ERR_NOT_INIT:
				strcat_s(tmp, 1024, "The variable has not been initialized properly.");
				break;
			case PHP_ERR_NOT_STR:
				strcat_s(tmp, 1024, "The variable must be a string.");
				break;
			case PHP_ERR_NOT_FOUND:
				strcat_s(tmp, 1024, "The variable does not exist.");
				break;

			case PHP_SCRIPT_NAMELEN:
				strcat_s(tmp, 1024, "The script name must be at least 3 characters long.");
				break;

			default:
				strcat_s(tmp, 1024, "General Parser Error");
				break;
		}

		MessageBox(NULL, tmp, "Packet Edit Studio PHP Parser", MB_ICONERROR);
	}

	//look for the next occurrance of a character
	LPSTR PHP_GetNextChar(LPSTR lpBuffer, CHAR character, INT *Line)
	{
		int i;

		int len = (int)strlen(lpBuffer);
		for(i = 0; i < len; ++i)
			if(!isspace(lpBuffer[i]))
			{
				if(lpBuffer[i] == character)
					return &lpBuffer[i];
				else
					return NULL;
			}
			else if(PHP_ISNEWLINE(lpBuffer, i))
				++*Line;

		return NULL;
	}

	//look for the first occurance of "Search"
	LPSTR PHP_strstr(LPSTR lpBuffer, LPSTR Search, INT *Line)
	{
		char *ptr = strstr(lpBuffer, Search);

		if(!ptr)
			return NULL;

		int len = (int) (ptr - lpBuffer);
		int i;
		for(i = 0; i < len; ++i)
			if(PHP_ISNEWLINE(lpBuffer, i))
				++*Line;

		return ptr;
	}

	//loads a string variable from a php buffer
	INT PHP_LoadVariable(LPSTR lpBuffer, LPSTR lpName, LPSTR lpCopyTo, int CopyToSz, INT *Line)
	{
		*Line = 1;

		// make variable name
		char PHPVar[255];
		strcpy_s(PHPVar, 255, "$");
		strcat_s(PHPVar, 255, lpName);

		// look for it
		char *var_ptr;
		if(!(var_ptr = PHP_strstr(lpBuffer, lpName, Line)))
			return PHP_ERR_NOT_FOUND;

		// skip past name to look for assignment
		var_ptr += strlen(lpName);

		// skip to initialization sign
		if(!(var_ptr = PHP_GetNextChar(var_ptr, '=', Line)))
			return PHP_ERR_NOT_INIT;

		++var_ptr;

		// look for string
		char *next = PHP_GetNextChar(var_ptr, '\'', Line);
		if(!next)
			next = PHP_GetNextChar(var_ptr, '"', Line);

		if(!next)
			return PHP_ERR_NOT_STR;
		else
			var_ptr = next;

		char look_for = (*var_ptr == '\'') ? '\'' : '"';

		++var_ptr;

		// look for delimiter and copy string
		// then return
		char tmp;
		int i;
		int len = (int)strlen(var_ptr);
		for(i = 0 ; i < len ; ++i)
			if(var_ptr[i] == look_for)
			{
				tmp = var_ptr[i];
				var_ptr[i] = '\0';
				strcpy_s(lpCopyTo, CopyToSz, var_ptr);
				var_ptr[i] = tmp;
				var_ptr = &var_ptr[i]+1;
				break;
			}
			else if(PHP_ISNEWLINE(var_ptr, i))
				++*Line;

		if(!(var_ptr = PHP_GetNextChar(var_ptr, ';', Line)))
			return PHP_ERR_SYNTAX;
		else if(strlen(lpCopyTo) > 254)
			return PHP_ERR_TOO_BIG;
		else
			return PHP_NO_ERR;
	}

/*************************************************************/
/*			CScriptsDlg										 */
/*															 */
/*	The dialog for handling php loading.					 */
/*************************************************************/

	/*************************************************************/
	/*			Constructor / Destructor						 */
	/*************************************************************/

		//init
		CScriptsDlg::CScriptsDlg(CWnd* pParent /*=NULL*/)
			: CDialog(CScriptsDlg::IDD, pParent)
		{
		}

		//free
		CScriptsDlg::~CScriptsDlg()
		{
		}

	/*************************************************************/
	/*			Script Management								 */
	/*************************************************************/

		//add php script
		INT CScriptsDlg::AddItem(LPSTR txt, LPSTR path)
		{
			int line, err;

			// load the variables
			char name[255];
			if((err = PHP_LoadVariable(txt, "script_name", name, 255, &line)) < 0)
			{
				PHP_VarErrorMessage("script_name", err, line);
				return false;
			}
			else if(strlen(name) < 3)
			{
				PHP_VarErrorMessage("script_name", PHP_SCRIPT_NAMELEN, line);
				return false;
			}

			char author[255];
			if((err = PHP_LoadVariable(txt, "script_author", author, 255, &line)) < 0)
			{
				PHP_VarErrorMessage("script_author", err, line);
				return false;
			}

			char version[255];
			if((err = PHP_LoadVariable(txt, "script_version", version, 255, &line)) < 0)
			{
				PHP_VarErrorMessage("script_version", err, line);
				return false;
			}

			// create a new script element and init it
			PHPScript *ptr = AllocateItemSpace();

			strcpy_s(ptr->name, 255, name);
			strcpy_s(ptr->author, 255, author);
			strcpy_s(ptr->version, 255, version);
			strcpy_s(ptr->path, MAX_PATH, path);

			// add the element to the list
			int index = m_ScriptList.AddString(name);
			m_ScriptList.SetItemData(index, (DWORD_PTR)ptr);
			m_ScriptList.SetCheck(index, 0 );

			ptr->loaded = false;

			// update the xml list
			SaveXMLList();

			return true;
		}

		//remove selected php script from the list
		void CScriptsDlg::RemoveItem()
		{
			// delete the string from the list control
			int sel = m_ScriptList.GetCurSel();
			PHPScript *p = (PHPScript *)m_ScriptList.GetItemData(sel);
			m_ScriptList.DeleteString(sel);

			// free the memory
			PHPScript *ptr = base;
			PHPScript *last = NULL;

			while(ptr)
			{
				if(ptr == p)
				{
					if(ptr == base)
						base = base->next;
					else if(last && ptr->next)
						last->next = ptr->next;
					else if(last)
						last->next = NULL;
					else if(ptr->next)
						base->next = ptr->next;
			
					delete ptr;
					break;
				}

				last = ptr;
				ptr = ptr->next;
			}

			// update the xml list
			SaveXMLList();
		}

	/*************************************************************/
	/*			Specific Control Events							 */
	/*************************************************************/

		//double clicked on the script list control
		void CScriptsDlg::OnListDoubleClick()
		{
			// open the selected document in the MDI window
			int sel = m_ScriptList.GetCurSel();
			PHPScript *ptr = (PHPScript *)m_ScriptList.GetItemData(sel);

			PESApp.LoadPHPDocumentFromFile(ptr->path);
		}

		//check state changed for a script
		void CScriptsDlg::OnCheckChange()
		{
			int sel = m_ScriptList.GetCurSel();
	
			if(m_ScriptList.GetCheck(sel) == 2)
				m_ScriptList.SetCheck(sel, 0);

			// update the memory and xml list with the new state
			bool state = (m_ScriptList.GetCheck(sel) == BST_CHECKED) ? true : false;
			PHPScript *ptr = (PHPScript *)m_ScriptList.GetItemData(sel);
			ptr->loaded = state;

			SaveXMLList();
		}

	/*************************************************************/
	/*			Window Events									 */
	/*************************************************************/

		//set up controls
		void CScriptsDlg::DoDataExchange(CDataExchange* pDX)
		{
			CDialog::DoDataExchange(pDX);
			DDX_Control(pDX, IDC_SCRIPTLISTBOX, m_ScriptList);
		}

		//init code
		BOOL CScriptsDlg::OnInitDialog()
		{			
			TiXmlNode *current_node, *script_node;
			TiXmlElement *elmt_node;
			TiXmlAttribute *elmt_attr;
			PHPScript *ptr;
			bool name_loaded, author_loaded, version_loaded, path_loaded;
			int index;

			if(!CDialog::OnInitDialog())
				return FALSE;

			base = NULL;

			m_ScriptList.SetCheckStyle( BS_AUTO3STATE );

			// open xml file
			strcpy_s(list_path, MAX_PATH, PESApp.GetAppDirectory());
			strcat_s(list_path, MAX_PATH, "\\phpengine\\scripts.xml");

			TiXmlDocument list_file;
			if(list_file.LoadFile(list_path))
			{
				current_node = list_file.FirstChildElement();
				while(current_node)
				{

					// found smart sniff node
					if(!strcmp(current_node->Value(), "PEStudio"))
					{
						current_node = current_node->FirstChild();
						while(current_node)
						{

							// found new script
							if(!strcmp(current_node->Value(), "Script"))
							{
								ptr = AllocateItemSpace();

								elmt_node = (TiXmlElement *)current_node;
								elmt_attr = (TiXmlAttribute *)elmt_node->FirstAttribute();
								ptr->loaded = (bool)atoi(elmt_attr->Value());

								script_node = current_node->FirstChild();

								while(script_node)
								{

									// name entry
									if(!strcmp(script_node->Value(), "name"))
									{
										name_loaded = true;
										strcpy_s(ptr->name, 255, script_node->FirstChild()->Value());
									}

									// author entry
									else if(!strcmp(script_node->Value(), "author"))
									{
										author_loaded = true;
										strcpy_s(ptr->author, 255, script_node->FirstChild()->Value());
									}

									// version entry
									else if(!strcmp(script_node->Value(), "version"))
									{
										version_loaded = true;
										strcpy_s(ptr->version, 255, script_node->FirstChild()->Value());
									}

									// path entry
									else if(!strcmp(script_node->Value(), "path"))
									{
										path_loaded = true;
										strcpy_s(ptr->path, 255, script_node->FirstChild()->Value());
									}

									// continue
									script_node = script_node->NextSibling();
								}

								// load script
								index = m_ScriptList.AddString(ptr->name);
								m_ScriptList.SetItemData(index, (DWORD_PTR)ptr);
								m_ScriptList.SetCheck(index, (int)ptr->loaded );

							}

							current_node = current_node->NextSibling();
						}

						break;
					}
					current_node = list_file.NextSiblingElement();
				}
			}


			return TRUE;
		}

	/*************************************************************/
	/*			Internal Helper Functions						 */
	/*************************************************************/

		//save all scripts in the list
		void CScriptsDlg::SaveXMLList(bool unload)
		{
			TiXmlDocument list_file;
			TiXmlElement *child, *child_node, *root;
 			TiXmlDeclaration* decl;
			PHPScript *ptr, *ptr_next;

			decl = new TiXmlDeclaration( "1.0", "", "" );
			list_file.LinkEndChild(decl);

			root = new TiXmlElement( "PEStudio" );
			list_file.LinkEndChild(root);

			ptr = base;

			while(ptr)
			{
				ptr_next = ptr->next;

				child = new TiXmlElement( "Script" );  
				root->LinkEndChild(child);

				child_node = new TiXmlElement( "name" );  
				child_node->LinkEndChild( new TiXmlText( ptr->name ) );  
				child->LinkEndChild(child_node);

				child_node = new TiXmlElement( "author" );  
				child_node->LinkEndChild( new TiXmlText( ptr->author ) );  
				child->LinkEndChild(child_node);

				child_node = new TiXmlElement( "version" );  
				child_node->LinkEndChild( new TiXmlText( ptr->version ) );  
				child->LinkEndChild(child_node);

				child_node = new TiXmlElement( "index" );  
				child_node->LinkEndChild( new TiXmlText( itoa(m_ScriptList.FindStringExact(-1, ptr->name)) ) );  
				child->LinkEndChild(child_node);

				child_node = new TiXmlElement( "path" );  
				child_node->LinkEndChild( new TiXmlText( ptr->path ) );  
				child->LinkEndChild(child_node);

				child->SetAttribute("loaded", itoa((int)ptr->loaded));

				ptr_next = ptr->next;

				if(unload)
					delete ptr;

				ptr = ptr_next;
			}

			list_file.SaveFile(list_path);
		}

		//create a new script node
		PHPScript *CScriptsDlg::AllocateItemSpace()
		{
			PHPScript *ptr;

			if(!base)
			{
				base = new PHPScript;
				ptr = base;
			}
			else
			{
				ptr = base;
				while(ptr)
				{
					if(!ptr->next)
					{
						ptr->next = new PHPScript;
						ptr = ptr->next;
						break;
					}
					ptr = ptr->next;
				}
			}

			ptr->next = NULL;

			return ptr;
		}

/*************************************************************/
/*			CScriptsDlg Message Map							 */
/*************************************************************/

IMPLEMENT_DYNAMIC(CScriptsDlg, CDialog)

BEGIN_MESSAGE_MAP(CScriptsDlg, CDialog)
	ON_LBN_DBLCLK(IDC_SCRIPTLISTBOX, OnListDoubleClick)
	ON_CLBN_CHKCHANGE(IDC_SCRIPTLISTBOX, OnCheckChange)
END_MESSAGE_MAP()

#pragma warning ( default : 4800 )
