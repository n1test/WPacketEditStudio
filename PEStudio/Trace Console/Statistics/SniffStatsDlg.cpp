/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  SniffStatsDlg.cpp

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

// This dialog has some random statistics... If you can think of better stats
// please add them, I kind of rushed through this section :(

	#include "stdafx.h"

/*************************************************************/
/*			CSniffStatsDlg									 */
/*															 */
/*	Just a simple dialog with some text controls.			 */
/*************************************************************/

	/*************************************************************/
	/*			Constructor / Destructor						 */
	/*************************************************************/

		CSniffStatsDlg::CSniffStatsDlg(CWnd* pParent /*=NULL*/)
			: CDialog(CSniffStatsDlg::IDD, pParent)
		{

		}

		CSniffStatsDlg::~CSniffStatsDlg()
		{
		}

	/*************************************************************/
	/*			Stats Management								 */
	/*************************************************************/

		//update stats text boxes
		void CSniffStatsDlg::UpdateStats(unsigned int avg, unsigned int recved, unsigned int sent, unsigned int total)
		{
			m_AvgSizeText.SetWindowTextA(itoa(avg));
			m_SumRecvedText.SetWindowTextA(itoa(recved));
			m_SumSentText.SetWindowTextA(itoa(sent));
			m_TotalBytesText.SetWindowTextA(itoa(total));
		}

	/*************************************************************/
	/*			Window Events									 */
	/*************************************************************/

		//set up controls
		void CSniffStatsDlg::DoDataExchange(CDataExchange* pDX)
		{
			DDX_Control(pDX, IDC_AVGSIZE, m_AvgSizeText);
			DDX_Control(pDX, IDC_TOTALRECVED, m_SumRecvedText);
			DDX_Control(pDX, IDC_TOTALSENT, m_SumSentText);
			DDX_Control(pDX, IDC_TOTALBYTES, m_TotalBytesText);
			CDialog::DoDataExchange(pDX);
		}

/*************************************************************/
/*			CSniffStatsDlg Message Map						 */
/*************************************************************/

IMPLEMENT_DYNAMIC(CSniffStatsDlg, CDialog)

BEGIN_MESSAGE_MAP(CSniffStatsDlg, CDialog)
END_MESSAGE_MAP()
