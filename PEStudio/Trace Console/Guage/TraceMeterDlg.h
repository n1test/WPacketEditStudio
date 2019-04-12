/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  TraceMeterDlg.h

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

// The buffer % and packet count dialog in the trace console

	#pragma once

	#include "afxwin.h"
	#include "3DMeterCtrl.h"
	#include "afxcmn.h"

	/*************************************************************/
	/*			CTraceMeterDlg									 */
	/*															 */
	/*	A fille meter and packet counter dialog.				 */
	/*************************************************************/
	class CTraceMeterDlg : public CDialog
	{
		DECLARE_DYNAMIC(CTraceMeterDlg)
		public:

			enum { IDD = IDD_METERBOX };

			/////////////////////////////
			//  Constructor / Destructor
			/////////////////////////////

				CTraceMeterDlg(CWnd* pParent = NULL);

				virtual ~CTraceMeterDlg();

			/////////////////////////////
			//  Meter Control
			/////////////////////////////

				//updates the needles
				void UpdateMeters(unsigned int packets, unsigned int percent);

			/////////////////////////////
			//  Window Events
			/////////////////////////////
		protected:
	
				virtual void DoDataExchange(CDataExchange* pDX);

				DECLARE_MESSAGE_MAP()

			/////////////////////////////
			//  Controls
			/////////////////////////////

				C3DMeterCtrl m_BufferMeter;						//the buffer meter control
				C3DMeterCtrl m_PacketMeter;						//the packet counter control
};
