/*

  FlatToolTipCtrl.h

  This file is part of the Packet Edit Studio Project.
  version: ALPHA

  * THIS IS THIRD PARTY SOURCE CODE *

  *Distributed under the terms of the zlib/libpng public license.*
  
  Author: 

	"TOMCAT" (codeproject.com)            - CToolTipCtrlEx
	(http://www.codeproject.com/KB/applications/TBar.aspx)

	See Author Copyright Below

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

// This is used for all those neat little pop up tool tips (such as those used in
// the process select dialog.
//
// This file has NOT been edited other than the addition of the above comments

#pragma once

//Arrow drawing is ported from the FLATGUI version in Delphi.
//You are free to use and modify this as long as you don`t claim it .
//Copyright : Tomkat(.ro) 2004

class CToolTipCtrlEx : public CToolTipCtrl
{
	DECLARE_DYNAMIC(CToolTipCtrlEx)
	enum	Orientations
	{
		NW=1,
		NE,
		SW,
		SE,
	};
public:
	CToolTipCtrlEx();
	virtual ~CToolTipCtrlEx();
protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT, WPARAM, LPARAM);
private:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcPaint();
public:
	COLORREF	m_bkColor;//=RGB(255,255,255);
	COLORREF	m_leftColor;//=RGB(255, 210, 83);
	COLORREF	m_frameColor;//=RGB(155, 110, 53);
	COLORREF	m_textColor;//=RGB(0,0,0);
	COLORREF	m_arrowColor;//=RGB(0,0,0);
};