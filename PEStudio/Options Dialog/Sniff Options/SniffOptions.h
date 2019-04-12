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

	#pragma once

	#include "afxwin.h"
	#include "FlatToolTipCtrl.h"

	/*************************************************************/
	/*			CSniffOptions									 */
	/*															 */
	/*	Sniffing Options Dialog									 */
	/*************************************************************/
	class CSniffOptions : public CDialog
	{
		DECLARE_DYNAMIC(CSniffOptions)
		public:

			enum { IDD = IDD_SNIFFINGOPS };

			/////////////////////////////
			//  Constructor / Destructor
			/////////////////////////////
			
				CSniffOptions(CWnd* pParent = NULL);

				virtual ~CSniffOptions();

			/////////////////////////////
			//  Variable Access
			/////////////////////////////
			
				char *GetDllName();
				int GetSniffMethod();
				int GetBufferSize();
				int GetPacketSize();

			/////////////////////////////
			//  Window Events
			/////////////////////////////
		protected:

			virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
			virtual BOOL OnInitDialog();

			DECLARE_MESSAGE_MAP()
			afx_msg LONG OnInitDialog ( UINT, LONG );
			afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);


			/////////////////////////////
			//  Private Variables
			/////////////////////////////
			
				char DllName[MAX_PATH];						//for storing the dll name to inject

			/////////////////////////////
			//  Controls
			/////////////////////////////

				CEdit m_DllNameEdit;						//the dll name to inject edit box
				CComboBox m_SniffMethodList;				//the sniff method list edit box
				CSliderCtrl m_BufferSizeSlider;				//the buffer size slider
				CStatic m_BufferSizeText;					//the buffer size text control
				CSliderCtrl m_PacketSizeSlider;				//the packet size slider
				CStatic m_PacketSizeText;					//the packet size text control
};
