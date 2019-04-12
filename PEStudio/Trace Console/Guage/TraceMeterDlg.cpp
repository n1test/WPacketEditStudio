/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  TraceMeterDlg.cpp

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

	#include "stdafx.h"

/*************************************************************/
/*			CTraceMeterDlg									 */
/*															 */
/*	A fille meter and packet counter dialog.				 */
/*************************************************************/

	/*************************************************************/
	/*			Constructor / Destructor						 */
	/*************************************************************/

		//init
		CTraceMeterDlg::CTraceMeterDlg(CWnd* pParent /*=NULL*/)
			: CDialog(CTraceMeterDlg::IDD, pParent)
		{
			m_BufferMeter.SetTitle("Buffer%%");
			m_PacketMeter.SetTitle("Packets");

			m_BufferMeter.SetRange(0, 100, false);
			m_BufferMeter.SetUnit("%");

			m_PacketMeter.SetRange(0, 5000, false);
			m_PacketMeter.SetUnit("P");
		}

		//free
		CTraceMeterDlg::~CTraceMeterDlg()
		{
		}

	/*************************************************************/
	/*			Meter Control									 */
	/*************************************************************/

		//update the needles
		void CTraceMeterDlg::UpdateMeters(unsigned int packets, unsigned int percent)
		{
			m_PacketMeter.UpdateNeedle(packets);
			m_BufferMeter.UpdateNeedle(percent);
		}

	/*************************************************************/
	/*			Window Events									 */
	/*************************************************************/

		//set up controls
		void CTraceMeterDlg::DoDataExchange(CDataExchange* pDX)
		{
			CDialog::DoDataExchange(pDX);
			DDX_Control(pDX, IDC_BUFFERMETER, m_BufferMeter);
			DDX_Control(pDX, IDC_PACKETMETER, m_PacketMeter);
		}

/*************************************************************/
/*			CTraceMeterDlg Message Map						 */
/*************************************************************/

IMPLEMENT_DYNAMIC(CTraceMeterDlg, CDialog)

BEGIN_MESSAGE_MAP(CTraceMeterDlg, CDialog)
END_MESSAGE_MAP()
