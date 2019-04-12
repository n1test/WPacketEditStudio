/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  SniffLogDlg.cpp

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

// The log window in the trace console dialog

/*

	THIS FEATURE HAS NOT BEEN IMPLEMENTED AND NEEDS TO BE COMPLETED.

*/

	#include "stdafx.h"

/*************************************************************/
/*			CSniffLogDlg									 */
/*															 */
/*	unknown exactly what this will encompass yet.			 */
/*************************************************************/

	/*************************************************************/
	/*			Constructor / Destructor						 */
	/*************************************************************/

		//init
		CSniffLogDlg::CSniffLogDlg(CWnd* pParent /*=NULL*/)
			: CDialog(CSniffLogDlg::IDD, pParent)
		{
		}

		//free
		CSniffLogDlg::~CSniffLogDlg()
		{
		}

	/*************************************************************/
	/*			Log Methods										 */
	/*************************************************************/

		//add log string
		void CSniffLogDlg::AddLog(LPSTR lpLog)
		{
			m_LogList.AddString(lpLog);
		}

	/*************************************************************/
	/*			Window Events									 */
	/*************************************************************/

		//set up controls
		void CSniffLogDlg::DoDataExchange(CDataExchange* pDX)
		{
			CDialog::DoDataExchange(pDX);
			DDX_Control(pDX, IDC_LOGLISTBOX, m_LogList);
		}

/*************************************************************/
/*			CSniffLogDlg Message Map						 */
/*************************************************************/

IMPLEMENT_DYNAMIC(CSniffLogDlg, CDialog)

BEGIN_MESSAGE_MAP(CSniffLogDlg, CDialog)
END_MESSAGE_MAP()
