/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  PHPOptions.h

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

	#pragma once

	/*************************************************************/
	/*			CPHPOptions										 */
	/*															 */
	/*	PHP Options Dialog										 */
	/*************************************************************/
	class CPHPOptions : public CDialog
	{
		DECLARE_DYNAMIC(CPHPOptions)
		public:

			enum { IDD = IDD_PHPOPS };

			/////////////////////////////
			//  Constructor / Destructor
			/////////////////////////////
			
				CPHPOptions(CWnd* pParent = NULL);

				virtual ~CPHPOptions();

			/////////////////////////////
			//  Variable Access
			/////////////////////////////
			
				LPSTR GetPHPVer();
				int GetLogPHP();

			/////////////////////////////
			//  Window Events
			/////////////////////////////
		protected:
			
				virtual BOOL OnInitDialog();
				virtual void DoDataExchange(CDataExchange* pDX);

				DECLARE_MESSAGE_MAP()

			/////////////////////////////
			//  Controls
			/////////////////////////////
			
				CComboBox m_PHPVersionBox;						//version select box
				CButton m_SavePHPOutputCheck;					//output check box
	};
