/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  SniffActivityDlg.cpp

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

	#include "stdafx.h"

/*************************************************************/
/*			CSniffActivityDlg								 */
/*															 */
/*	A dialog with a simple network activity graph.			 */
/*  (graphs packets/time)									 */
/*************************************************************/

	/*************************************************************/
	/*			Constructor / Destructor						 */
	/*************************************************************/

		//init
		CSniffActivityDlg::CSniffActivityDlg(CWnd* pParent /*=NULL*/)
			: CDialog(CSniffActivityDlg::IDD, pParent)
		{

		}

		//free
		CSniffActivityDlg::~CSniffActivityDlg()
		{
		}

	/*************************************************************/
	/*			Graph Methods									 */
	/*************************************************************/

		//increase activity
		void CSniffActivityDlg::UpdateGraph()
		{
			++packets;
		}

		//start graphing
		void CSniffActivityDlg::StartGraphTimer()
		{
			if(m_TimerId)
				KillTimer(m_TimerId);

			xmin = 0;
			xmax = 10;
			packets = 0;

			m_TimerId = (UINT)SetTimer(1, 500, NULL);

			m_ActivityGraph.ClearGraph();
		}

		//stop graphing
		void CSniffActivityDlg::EndGraphTimer()
		{
			KillTimer(m_TimerId);
		}

	/*************************************************************/
	/*			Window Events									 */
	/*************************************************************/

		//set up controls
		void CSniffActivityDlg::DoDataExchange(CDataExchange* pDX)
		{
			DDX_Control(pDX, IDC_ACTIVITYGRAPH, m_ActivityGraph);
			CDialog::DoDataExchange(pDX);
		}

		//update graph
		void CSniffActivityDlg::OnTimer(UINT nIDEvent)
		{
			m_ActivityGraph.SetElement(0);
			m_ActivityGraph.SetElementLineColor(RGB(0,255,0));
			m_ActivityGraph.SetElementIdentify(FALSE);

			++xmin;
			++xmax;
			
			m_ActivityGraph.PlotXY( (double)xmax , packets , 0);

			m_ActivityGraph.SetRange((double)xmin, (double)xmax, -20, 50);

			packets = 0;

			CDialog::OnTimer(nIDEvent);
		}

/*************************************************************/
/*			CSniffActivityDlg Message Map					 */
/*************************************************************/

IMPLEMENT_DYNAMIC(CSniffActivityDlg, CDialog)

BEGIN_MESSAGE_MAP(CSniffActivityDlg, CDialog)
	ON_WM_TIMER ( )
END_MESSAGE_MAP()
