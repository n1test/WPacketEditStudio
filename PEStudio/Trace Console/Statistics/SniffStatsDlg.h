/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  SniffStatsDlg.h

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

	#pragma once

	/*************************************************************/
	/*			CSniffStatsDlg									 */
	/*															 */
	/*	Just a simple dialog with some text controls.			 */
	/*************************************************************/
	class CSniffStatsDlg : public CDialog
	{
		DECLARE_DYNAMIC(CSniffStatsDlg)
		public:

			enum { IDD = IDD_SNIFFSTATS };

			/////////////////////////////
			//  Constructor / Destructor
			/////////////////////////////

				CSniffStatsDlg(CWnd* pParent = NULL);

				virtual ~CSniffStatsDlg();

			/////////////////////////////
			//  Stats Management
			/////////////////////////////

				void UpdateStats(unsigned int avg, unsigned int recved, unsigned int sent, unsigned int total);

			/////////////////////////////
			//  Window Events
			/////////////////////////////
		protected:

				virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

				DECLARE_MESSAGE_MAP()

			/////////////////////////////
			//  Controls
			/////////////////////////////
	
				CStatic m_AvgSizeText;
				CStatic m_SumRecvedText;
				CStatic m_SumSentText;
				CStatic m_TotalBytesText;
	};
