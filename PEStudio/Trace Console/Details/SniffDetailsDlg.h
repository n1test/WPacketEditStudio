/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  SniffDetailsDlg.h

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

	#pragma once

	/*************************************************************/
	/*			CSniffDetailsDlg								 */
	/*															 */
	/*	Simple dialog with a display of winsock api details.	 */
	/*************************************************************/
	class CSniffDetailsDlg : public CDialog
	{
		DECLARE_DYNAMIC(CSniffDetailsDlg)
		public:

			enum { IDD = IDD_SNIFFDETAILS };

			/////////////////////////////
			//  Constructor / Destructor
			/////////////////////////////

				CSniffDetailsDlg(CWnd* pParent = NULL);

				virtual ~CSniffDetailsDlg();

			/////////////////////////////
			//  Detail Management
			/////////////////////////////

				void UpdateDetails(SniffDetails &SniffInfo);

			/////////////////////////////
			//  Window Events
			/////////////////////////////
		protected:
			
				virtual void DoDataExchange(CDataExchange* pDX);

				DECLARE_MESSAGE_MAP()

			/////////////////////////////
			//  Controls
			/////////////////////////////

				CStatic m_SendCallsText;
				CStatic m_RecvCallsText;
				CStatic m_WSASendCallsText;
				CStatic m_WSARecvCallsText;
				CStatic m_SendToCallsText;
				CStatic m_RecvFromCallsText;
				CStatic m_WSASendToCallsText;
				CStatic m_WSARecvFromCallsText;
};
