/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  SniffDetailsDlg.cpp

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

// Dialog in the trace console which has some basic information about hook
// results (count of how many of each call were made)

	#include "stdafx.h"

/*************************************************************/
/*			CSniffDetailsDlg								 */
/*															 */
/*	Simple dialog with a display of winsock api details.	 */
/*************************************************************/

	/*************************************************************/
	/*			Constructor / Destructor						 */
	/*************************************************************/

		//init
		CSniffDetailsDlg::CSniffDetailsDlg(CWnd* pParent /*=NULL*/)
			: CDialog(CSniffDetailsDlg::IDD, pParent)
		{
		}

		//free
		CSniffDetailsDlg::~CSniffDetailsDlg()
		{
		}

	/*************************************************************/
	/*			Detail Management								 */
	/*************************************************************/

		void CSniffDetailsDlg::UpdateDetails(SniffDetails &SniffInfo)
		{
			m_SendCallsText.SetWindowTextA(itoa(SniffInfo.SendCalls));
			m_RecvCallsText.SetWindowTextA(itoa(SniffInfo.RecvCalls));
			m_WSASendCallsText.SetWindowTextA(itoa(SniffInfo.WSASendCalls));
			m_WSARecvCallsText.SetWindowTextA(itoa(SniffInfo.WSARecvCalls));
			m_SendToCallsText.SetWindowTextA(itoa(SniffInfo.SendToCalls));
			m_RecvFromCallsText.SetWindowTextA(itoa(SniffInfo.RecvFromCalls));
			m_WSASendToCallsText.SetWindowTextA(itoa(SniffInfo.WSASendToCalls));
			m_WSARecvFromCallsText.SetWindowTextA(itoa(SniffInfo.WSARecvFromCalls));
		}

	/*************************************************************/
	/*			Window Events									 */
	/*************************************************************/

		//set up controls
		void CSniffDetailsDlg::DoDataExchange(CDataExchange* pDX)
		{
			DDX_Control(pDX, IDC_SENDCALLS, m_SendCallsText);
			DDX_Control(pDX, IDC_RECVCALLS, m_RecvCallsText);
			DDX_Control(pDX, IDC_WSASENDCALLS, m_WSASendCallsText);
			DDX_Control(pDX, IDC_WSARECVCALLS, m_WSARecvCallsText);
			DDX_Control(pDX, IDC_SENDTOCALLS, m_SendToCallsText);
			DDX_Control(pDX, IDC_RECVFROMCALLS, m_RecvFromCallsText);
			DDX_Control(pDX, IDC_WSASENDTOCALLS, m_WSASendToCallsText);
			DDX_Control(pDX, IDC_WSARECVFROMCALLS, m_WSARecvFromCallsText);
			CDialog::DoDataExchange(pDX);
		}

/*************************************************************/
/*			CSniffDetailsDlg Message Map					 */
/*************************************************************/

IMPLEMENT_DYNAMIC(CSniffDetailsDlg, CDialog)

BEGIN_MESSAGE_MAP(CSniffDetailsDlg, CDialog)
END_MESSAGE_MAP()
