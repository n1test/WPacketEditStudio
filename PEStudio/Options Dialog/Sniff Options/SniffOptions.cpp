/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  SniffOptions.h

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

// The sniffing options child dialog which is loaded from the main dialog

	#include "stdafx.h"

/*************************************************************/
/*			CSniffOptions									 */
/*															 */
/*	Sniffing Options Dialog									 */
/*************************************************************/

	/*************************************************************/
	/*			Constructor / Destructor						 */
	/*************************************************************/

		//init
		CSniffOptions::CSniffOptions(CWnd* pParent /*=NULL*/)
			: CDialog(CSniffOptions::IDD, pParent)
		{

		}

		//free
		CSniffOptions::~CSniffOptions()
		{
		}

	/*************************************************************/
	/*			Variable Access									 */
	/*************************************************************/

		//dll to inject
		char *CSniffOptions::GetDllName()
		{
			m_DllNameEdit.GetWindowTextA(DllName, MAX_PATH);
			fixeditdata(DllName);
			return DllName;
		}

		//injection type
		int CSniffOptions::GetSniffMethod()
		{
			int i;
			i = m_SniffMethodList.GetCurSel();
			return (i >= 0) ? i : 0;
		}

		//allocation size
		int CSniffOptions::GetBufferSize()
		{
			return m_BufferSizeSlider.GetPos();
		}

		//max packet
		int CSniffOptions::GetPacketSize()
		{
			return m_PacketSizeSlider.GetPos();
		}

	/*************************************************************/
	/*			Window Events									 */
	/*************************************************************/

		//set up controls
		void CSniffOptions::DoDataExchange(CDataExchange* pDX)
		{
			CDialog::DoDataExchange(pDX);
			DDX_Control(pDX, IDC_DLLNAMEEDIT, m_DllNameEdit);
			DDX_Control(pDX, IDC_SNIFFMETHODBOX, m_SniffMethodList);
			DDX_Control(pDX, IDC_BUFFERSIZESLIDE, m_BufferSizeSlider);
			DDX_Control(pDX, IDC_BUFFERSIZETXT, m_BufferSizeText);
			DDX_Control(pDX, IDC_PACKETSIZESLIDER, m_PacketSizeSlider);
			DDX_Control(pDX, IDC_PACKETSIZETEXT, m_PacketSizeText);
		}

		//init code
		BOOL CSniffOptions::OnInitDialog()
		{
			if(!CDialog::OnInitDialog())
				return FALSE;

			// sniff method
			m_SniffMethodList.ResetContent();
			m_SniffMethodList.InsertString(0, "Hook API Calls (single process)");
			m_SniffMethodList.InsertString(1, "Hook API Calls (all Processes)");
			m_SniffMethodList.InsertString(2, "WinPCap (all processes)");

			// dll name
			const char *dll = PESApp.GetDllName();

			if(dll[0])
				m_DllNameEdit.SetWindowTextA(dll);
			else
				m_DllNameEdit.SetWindowTextA("PEStudio.dll");

			m_SniffMethodList.SetCurSel(PESApp.GetSniffMethod());

			// buffer size
			m_BufferSizeSlider.SetRange(100, 1048576, TRUE);
			m_BufferSizeSlider.SetTicFreq(1);

			int buffersize = PESApp.GetBufferSize();

			if(buffersize >= 100 && buffersize <= 1048576)
			{
				m_BufferSizeText.SetWindowTextA(itoa(buffersize));
				m_BufferSizeSlider.SetPos(buffersize);
			}
			else
			{
				m_BufferSizeText.SetWindowTextA("16384");
				m_BufferSizeSlider.SetPos(16384);
			}

			m_PacketSizeSlider.SetRange(100, 10240);
			m_BufferSizeSlider.SetTicFreq(1);

			//  packet size
			int packetsize = PESApp.GetMaxPacketSize();

			if(packetsize >= 100 && packetsize <= 10240)
			{
				m_PacketSizeText.SetWindowTextA(itoa(packetsize));
				m_PacketSizeSlider.SetPos(packetsize);
			}
			else
			{
				m_PacketSizeText.SetWindowTextA("6144");
				m_PacketSizeSlider.SetPos(6144);
			}

			return TRUE;
		}

		//slider moved
		void CSniffOptions::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
		{
			m_BufferSizeText.SetWindowTextA(itoa(m_BufferSizeSlider.GetPos()));
			m_PacketSizeText.SetWindowTextA(itoa(m_PacketSizeSlider.GetPos()));
			return CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
		}

/*************************************************************/
/*			CSniffOptions Message Map						 */
/*************************************************************/

IMPLEMENT_DYNAMIC(CSniffOptions, CDialog)

BEGIN_MESSAGE_MAP(CSniffOptions, CDialog)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()
