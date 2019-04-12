/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  PHPOptions.cpp

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

// The php options child dialog which is loaded from the main dialog

	#include "stdafx.h"
	#include "PEStudio.h"
	#include "PHPOptions.h"


/*************************************************************/
/*			CPHPOptions										 */
/*															 */
/*	PHP Options Dialog										 */
/*************************************************************/

	/*************************************************************/
	/*			Constructor / Destructor						 */
	/*************************************************************/

		//init
		CPHPOptions::CPHPOptions(CWnd* pParent /*=NULL*/)
			: CDialog(CPHPOptions::IDD, pParent)
		{

		}

		//free
		CPHPOptions::~CPHPOptions()
		{
		}

	/*************************************************************/
	/*			Variable Access									 */
	/*************************************************************/

		//version of php to use
		LPSTR CPHPOptions::GetPHPVer()
		{
			static char phpver[255];

			m_PHPVersionBox.GetLBText(m_PHPVersionBox.GetCurSel(), phpver);
			
			return phpver;
		}

		//whether to log php
		int CPHPOptions::GetLogPHP()
		{
			return (m_SavePHPOutputCheck.GetCheck() == BST_CHECKED) ? 1 : 0;
		}

	/*************************************************************/
	/*			Window Events									 */
	/*************************************************************/

		//init code
		BOOL CPHPOptions::OnInitDialog()
		{
			if(!CDialog::OnInitDialog())
				return FALSE;
			const char *phpver;
			bool logphp;

			phpver = PESApp.GetPHPVer();
			logphp = PESApp.GetLogPHP();

			m_PHPVersionBox.ResetContent();
			m_PHPVersionBox.InsertString(0, "PHP 4.4.6");
			m_PHPVersionBox.InsertString(1, "PHP 5.2.1");

			if(!strcmp(phpver, "PHP 4.4.6"))
				m_PHPVersionBox.SetCurSel(0);
			else
				m_PHPVersionBox.SetCurSel(1);

			if(logphp)
				m_SavePHPOutputCheck.SetCheck(BST_CHECKED);
			else
				m_SavePHPOutputCheck.SetCheck(BST_UNCHECKED);

			return TRUE;
		}

		//set up controls
		void CPHPOptions::DoDataExchange(CDataExchange* pDX)
		{
			CDialog::DoDataExchange(pDX);
			DDX_Control(pDX, IDC_PHPVERSIONBOX, m_PHPVersionBox);
			DDX_Control(pDX, IDC_PHPSAVECHECK, m_SavePHPOutputCheck);
		}

/*************************************************************/
/*			CPHPOptions Message Map							 */
/*************************************************************/

IMPLEMENT_DYNAMIC(CPHPOptions, CDialog)

BEGIN_MESSAGE_MAP(CPHPOptions, CDialog)
END_MESSAGE_MAP()
