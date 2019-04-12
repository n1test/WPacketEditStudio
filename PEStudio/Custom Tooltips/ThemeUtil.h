/*

  ThemeUtil.h

  This file is part of the Packet Edit Studio Project.
  version: ALPHA

  * THIS IS THIRD PARTY SOURCE CODE *

  *Distributed under the terms of the zlib/libpng public license.*
  
  Author: 

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

// Used by the custom tool tip control.
//
// This file has NOT been edited other than the addition of the above comments

#pragma once

/**********************************************************************
**
**	ThemeUtil.h : include file
**
**	by Andrzej Markowski June 2004
**
**********************************************************************/

#include "TmSchema.h"

#define THM_WM_THEMECHANGED     0x031A

typedef struct _MY_MARGINS
{
    int cxLeftWidth;
    int cxRightWidth;
    int cyTopHeight;
    int cyBottomHeight;
} MY_MARGINS;

class CThemeUtil
{
public:
	CThemeUtil();
	virtual ~CThemeUtil();
	BOOL OpenThemeData(HWND hWnd, LPCWSTR pszClassList);
	void CloseThemeData();
	BOOL DrawThemePart(HDC hdc, int iPartId, int iStateId, const RECT *pRect);
	BOOL GetThemeColor(int iPartId, int iStateId, int iPropId, const COLORREF *pColor);
	BOOL GetThemeInt(int iPartId, int iStateId, int iPropId, const int *piVal);
	BOOL GetThemeMargins(int iPartId, int iStateId, int iPropId, const MY_MARGINS *pMargins);
	BOOL GetThemeEnumValue(int iPartId, int iStateId, int iPropId, const int *piVal);
	BOOL GetThemeFilename(int iPartId, int iStateId, int iPropId, 
							OUT LPWSTR pszThemeFileName, int cchMaxBuffChars);
	BOOL GetCurrentThemeName(OUT LPWSTR pszThemeFileName, int cchMaxNameChars, 
							OUT OPTIONAL LPWSTR pszColorBuff, int cchMaxColorChars,
							OUT OPTIONAL LPWSTR pszSizeBuff, int cchMaxSizeChars);
	HBITMAP LoadBitmap(LPWSTR pszBitmapName);
private:
	BOOL IsWinXP(void);
	void FreeLibrary();
private:
	HINSTANCE m_hUxThemeDll;
	HINSTANCE m_hRcDll;
	UINT m_hTheme;
};
