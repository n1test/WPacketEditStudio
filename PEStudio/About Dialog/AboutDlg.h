/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  AboutDlg.h

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

// The about dialog class

	#pragma once
	#include "afxcmn.h"

	// CAboutDlg dialog used for App About

	/* This is the cookie cutter about dialog that comes
	   with MFC.  If you want to add to it please, be my
	   guest.  If a lot of people begin contributing code
	   Then I will make an edit box / or list box with
	   code contributers names.
	*/

	class CAboutDlg : public CDialog
	{
		public:
			CAboutDlg();

			// Dialog Data
			enum { IDD = IDD_ABOUTBOX };

		protected:
			virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

		// Implementation
		protected:
		DECLARE_MESSAGE_MAP()
	};
