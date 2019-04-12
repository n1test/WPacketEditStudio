/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  SniffActivityDlg.h

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

// The activity part of the trace console dialog which has the network activity
// ocx control in it

	#pragma once

	/*************************************************************/
	/*			CSniffActivityDlg								 */
	/*															 */
	/*	A dialog with a simple network activity graph.			 */
	/*  (graphs packets/time)									 */
	/*************************************************************/
	class CSniffActivityDlg : public CDialog
	{
		DECLARE_DYNAMIC(CSniffActivityDlg)
		public:

			enum { IDD = IDD_SNIFFACTIVITY };

			/////////////////////////////
			//  Constructor / Destructor
			/////////////////////////////
			
				CSniffActivityDlg(CWnd* pParent = NULL);

				virtual ~CSniffActivityDlg();

			/////////////////////////////
			//  Graph Methods
			/////////////////////////////
			
				//pings the graph
				void UpdateGraph();

				//starts graph pinging
				void StartGraphTimer();

				//stops graph pinging
				void EndGraphTimer();

			/////////////////////////////
			//  Window Events
			/////////////////////////////
		protected:
			
			virtual void DoDataExchange(CDataExchange* pDX);

			DECLARE_MESSAGE_MAP()
			afx_msg void OnTimer(UINT nIDEvent);

			/////////////////////////////
			//  Private Variables
			/////////////////////////////
			
				unsigned int packets;						//packets recieved
				UINT m_TimerId;								//timer identifier
				time_t xmin, xmax;							//graph scope

			/////////////////////////////
			//  Controls
			/////////////////////////////

				CNTGraph m_ActivityGraph;					//the graph control
	};
