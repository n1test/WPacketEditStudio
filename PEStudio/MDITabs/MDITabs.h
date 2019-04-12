/*

  MDITabs.h

  This file is part of the Packet Edit Studio Project.
  version: ALPHA

  * THIS IS THIRD PARTY SOURCE CODE *

  *Distributed under the terms of the zlib/libpng public license.*
  
  Author: 

	Christian Rodemeyer (codeproject.com) - CMDITabs
	(http://www.codeproject.com/KB/tabs/mditabs.aspx)

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

// The tabbing control for handling multipile documents.
//
// This file has NOT been edited other than the addition of the above comments

// EnBitmap.h: interface for the CEnBitmap class.
//
//////////////////////////////////////////////////////////////////////

/****************************************************************************\
Datei  : MDITabs.h
Projekt: MDITabs, a tabcontrol for switching between MDI-views
Inhalt : EMDITabStyles, CMDITabs declaration
Datum  : 03.10.2001
Autor  : Christian Rodemeyer
Hinweis: © 2001 by Christian Rodemeyer
\****************************************************************************/
#pragma once

#ifndef __MDITABS_H
#define __MDITABS_H

/****************************************************************************\
 EMDITabStyles: Styles for customizing the appeareance of CMDITabs
\****************************************************************************/
enum EMDITabStyles
{
  MT_BOTTOM        = 0x0000, // places tabs at bottom (default)
  MT_TOP           = 0x0001, // place tabs at top
  MT_IMAGES        = 0x0002, // show images
  MT_HIDEWLT2VIEWS = 0x0004, // Hide Tabs when less than two views are open (default is one view)
  MT_TOOLTIPS      = 0x0008, // not implemented (a tooltip can appear about a tab) 
  MT_BUTTONS       = 0x0010, // not implemented (show tabs as buttons)
  MT_AUTOSIZE      = 0x0020, // not implemented (tabs are sized to fit the entire width of the control)
  MT_TASKBAR       = 0x0038  // MT_TOOLTIPS|MT_BUTTONS|MT_AUTOSIZE
};

enum EWMMTGetInfo
{
  WM_GETTABTIPTEXT = WM_APP + 0x0393, // send to associated view to get a tip text
  WM_GETTABSYSMENU = WM_APP + 0x0394  // if neccessary, the view can provide a different systemmenu by answering this message
};

/****************************************************************************\
 CMdiTabs: Deklaration
\****************************************************************************/
class CMDITabs : public CTabCtrl
{
public:
  CMDITabs();

  void Create(CFrameWnd* pMainFrame, DWORD dwStyle = MT_BOTTOM|MT_IMAGES);
  void Update(); // sync the tabctrl with all views

  void SetMinViews(int minViews) {m_minViews = minViews;}

private:
  HWND       m_mdiClient;
  int        m_height;
  int        m_width;
  CImageList m_images;
  int        m_minViews; // minimum number of views 
  bool       m_bImages;
  bool       m_bTop;

public:

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CMDITabs)
  //}}AFX_VIRTUAL

// Implementation
public:

  // Generated message map functions
protected:
  //{{AFX_MSG(CMDITabs)
  afx_msg void OnSelChange(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnPaint();
  afx_msg void OnNcPaint();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
  afx_msg LRESULT OnSizeParent(WPARAM, LPARAM lParam);

  DECLARE_MESSAGE_MAP()
};

#endif
