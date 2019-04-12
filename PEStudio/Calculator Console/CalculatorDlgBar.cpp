/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  CalculatorDlgBar.cpp

  This file is part of the Packet Edit Studio Project.
  version: ALPHA

  *Distributed under the terms of the zlib/libpng public license.*
  
  Author: 

	Chris Smith (sourcemagik.com) - CCalculatorDlgBar
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

// The calculator box that apears in the lower left hand corner

	#include "stdafx.h"

/*************************************************************/
/*			CCalculatorDlgBar								 */
/*															 */
/*	A multipurpose calculator dialog						 */
/*************************************************************/

	/*************************************************************/
	/*			Constructor / Destructor						 */
	/*************************************************************/

		//init
		CCalculatorDlgBar::CCalculatorDlgBar()
		{
		}

		//free
		CCalculatorDlgBar::~CCalculatorDlgBar()
		{
		}

	/*************************************************************/
	/*			Window Events									 */
	/*************************************************************/

		//enables a button
		void CCalculatorDlgBar::OnUpdateOPButton(CCmdUI *pCmdUI)
		{
			CButton *btn = (CButton *)GetDlgItem(pCmdUI->m_nID);
			if(btn->IsWindowEnabled())
				pCmdUI->Enable(TRUE);
		}

		//init code
		LONG CCalculatorDlgBar::OnInitDialog ( UINT wParam, LONG lParam)
		{
			BOOL bRet = (BOOL)HandleInitDialog(wParam, lParam);

			// default settings
			this->m_iVarSize = CALCMODE_32BIT;
			this->m_iFunction = 0;
	
			// init to 0
			this->m_i64bit = 0;
			this->m_i64bit_arg = 0;

			strcpy_s(In_Bin, 255, "0");
			strcpy_s(In_Oct, 255, "0");
			strcpy_s(In_Dec, 255, "0");
			strcpy_s(In_Hex, 255, "0");

			// init variables
			m_pCalcWindow = (CEdit *)GetDlgItem(IDC_CALC_BOTTOMLIST);
			m_pCalcWindow->SetWindowText("0");

			// init mode
			OnClickDec();
			SetBitConfig(CALCMODE_32BIT);

			return bRet;
		}

		//creation code
		int CCalculatorDlgBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
		{
			if (CDialogBar::OnCreate(lpCreateStruct) == -1)
				return -1;
			return 0;
		}

	/*************************************************************/
	/*			Calculator APIs									 */
	/*************************************************************/

		//displays a number in the calculator edit box
		void CCalculatorDlgBar::CalcOutput(unsigned __int64 display)
		{
			m_pCalcWindow = (CEdit *)GetDlgItem(IDC_CALC_BOTTOMLIST);

			FormatOutput(display);

			switch(m_iViewMode)
			{
				case CALCMODE_BIN:
					m_pCalcWindow->SetWindowText(In_Bin);
					break;
				case CALCMODE_OCT:
					m_pCalcWindow->SetWindowText(In_Oct);
					break;
				case CALCMODE_DEC:
					m_pCalcWindow->SetWindowText(In_Dec);
					break;
				case CALCMODE_HEX:
					m_pCalcWindow->SetWindowText(In_Hex);
					break;
			}
		}

		//sets which buttons should be active based on the mode
		void CCalculatorDlgBar::ResetModes(INT mode)
		{

			// obtain pointers to each button object
			m_pBinButton = (CButton *)GetDlgItem(IDC_CALC_BIN);
			m_pOctButton = (CButton *)GetDlgItem(IDC_CALC_OCT);
			m_pHexButton = (CButton *)GetDlgItem(IDC_CALC_HEX);
			m_pDecButton = (CButton *)GetDlgItem(IDC_CALC_DEC);

			m_pTwoButton = (CButton *)GetDlgItem(IDC_CALC_2);
			m_pThreeButton = (CButton *)GetDlgItem(IDC_CALC_3);
			m_pFourButton = (CButton *)GetDlgItem(IDC_CALC_4);
			m_pFiveButton = (CButton *)GetDlgItem(IDC_CALC_5);
			m_pSixButton = (CButton *)GetDlgItem(IDC_CALC_6);
			m_pSevenButton = (CButton *)GetDlgItem(IDC_CALC_7);
			m_pEightButton = (CButton *)GetDlgItem(IDC_CALC_8);
			m_pNineButton = (CButton *)GetDlgItem(IDC_CALC_9);

			m_pAButton = (CButton *)GetDlgItem(IDC_CALC_A);
			m_pBButton = (CButton *)GetDlgItem(IDC_CALC_B);
			m_pCButton = (CButton *)GetDlgItem(IDC_CALC_C);
			m_pDButton = (CButton *)GetDlgItem(IDC_CALC_D);
			m_pEButton = (CButton *)GetDlgItem(IDC_CALC_E);
			m_pFButton = (CButton *)GetDlgItem(IDC_CALC_F);

			// reset buttons
			m_pBinButton->SetCheck(BST_UNCHECKED);
			m_pOctButton->SetCheck(BST_UNCHECKED);
			m_pHexButton->SetCheck(BST_UNCHECKED);
			m_pDecButton->SetCheck(BST_UNCHECKED);

			m_pAButton->EnableWindow(FALSE);

			m_pTwoButton->EnableWindow(TRUE);
			m_pThreeButton->EnableWindow(TRUE);
			m_pFourButton->EnableWindow(TRUE);
			m_pFiveButton->EnableWindow(TRUE);
			m_pSixButton->EnableWindow(TRUE);
			m_pSevenButton->EnableWindow(TRUE);
			m_pEightButton->EnableWindow(TRUE);
			m_pNineButton->EnableWindow(TRUE);

			m_pAButton->EnableWindow(FALSE);
			m_pBButton->EnableWindow(FALSE);
			m_pCButton->EnableWindow(FALSE);
			m_pDButton->EnableWindow(FALSE);
			m_pEButton->EnableWindow(FALSE);
			m_pFButton->EnableWindow(FALSE);

			// set the correct buttons
			switch(mode)
			{
				case CALCMODE_BIN:
					m_pBinButton->SetCheck(BST_CHECKED);
					m_pTwoButton->EnableWindow(FALSE);
					m_pThreeButton->EnableWindow(FALSE);
					m_pFourButton->EnableWindow(FALSE);
					m_pFiveButton->EnableWindow(FALSE);
					m_pSixButton->EnableWindow(FALSE);
					m_pSevenButton->EnableWindow(FALSE);
					m_pEightButton->EnableWindow(FALSE);
					m_pNineButton->EnableWindow(FALSE);
					break;
				case CALCMODE_OCT:
					m_pEightButton->EnableWindow(FALSE);
					m_pNineButton->EnableWindow(FALSE);
					m_pOctButton->SetCheck(BST_CHECKED);
					break;
				case CALCMODE_DEC:
					m_pDecButton->SetCheck(BST_CHECKED);
					break;
				case CALCMODE_HEX:
					m_pAButton->EnableWindow(TRUE);
					m_pBButton->EnableWindow(TRUE);
					m_pCButton->EnableWindow(TRUE);
					m_pDButton->EnableWindow(TRUE);
					m_pEButton->EnableWindow(TRUE);
					m_pFButton->EnableWindow(TRUE);
					m_pHexButton->SetCheck(BST_CHECKED);
					break;
			}

			m_iViewMode = mode;

			CalcOutput(m_i64bit);
		}

		//set the size of the variable display
		void CCalculatorDlgBar::SetBitConfig(INT bits)
		{
			
			// get control pointers
			m_p8bitButton = (CButton *)GetDlgItem(IDC_CALC_EIGHTBIT);
			m_p16bitButton = (CButton *)GetDlgItem(IDC_CALC_SIXTEENBIT);
			m_p32bitButton = (CButton *)GetDlgItem(IDC_CALC_THIRTYTWOBIT);
			m_p64bitButton = (CButton *)GetDlgItem(IDC_CALC_SIXTYFOURBIT);

			// reset buttons
			m_p8bitButton->SetCheck(BST_UNCHECKED);
			m_p16bitButton->SetCheck(BST_UNCHECKED);
			m_p32bitButton->SetCheck(BST_UNCHECKED);
			m_p64bitButton->SetCheck(BST_UNCHECKED);

			// set the correct button
			if(bits == CALCMODE_8BIT) m_p8bitButton->SetCheck(BST_CHECKED);
			if(bits == CALCMODE_16BIT) m_p16bitButton->SetCheck(BST_CHECKED);
			if(bits == CALCMODE_32BIT) m_p32bitButton->SetCheck(BST_CHECKED);
			if(bits == CALCMODE_64BIT) m_p64bitButton->SetCheck(BST_CHECKED);

			m_iVarSize = bits;
			CalcOutput(m_i64bit);
		}

		//sets the current function to apply when ApplyFunction() is called
		void CCalculatorDlgBar::SetFunction(INT function)
		{
			// apply any previous function
			ApplyFunction();

			// set the function
			m_iFunction = function;

			// reset the input buffers
			In_Bin[0] = '\0';
			In_Oct[0] = '\0';
			In_Dec[0] = '\0';
			In_Hex[0] = '\0';
		}

		//applies any previously set function to the input
		void CCalculatorDlgBar::ApplyFunction()
		{

			// apply the function
			switch(m_iFunction)
			{
				case CALC_FUNCTION_ADD:
					m_i64bit += m_i64bit_arg;
					break;
				case CALC_FUNCTION_SUB:
					m_i64bit -= m_i64bit_arg;
					break;
				case CALC_FUNCTION_MUL:
					m_i64bit *= m_i64bit_arg;
					break;
				case CALC_FUNCTION_DEV:
					m_i64bit /= m_i64bit_arg;
					break;
			}

			// update the display buffers
			_i64toa_s(m_i64bit, In_Bin, 255, 2);
			_i64toa_s(m_i64bit, In_Oct, 255, 8);
			_i64toa_s(m_i64bit, In_Dec, 255, 10);
			_i64toa_s(m_i64bit, In_Hex, 255, 16);

			// reset the function
			m_iFunction = 0;

			CalcOutput(m_i64bit);
		}

		//forces output to the correct byte size
		void CCalculatorDlgBar::FormatOutput(unsigned __int64 display)
		{

			// force size
			switch(m_iVarSize)
			{
				case CALCMODE_8BIT:
					if(display > 0xff)
						display = 0xff;
					break;
				case CALCMODE_16BIT:
					if(display > 0xffff)
						display = 0xffff;
					break;
				case CALCMODE_32BIT:
					if(display > INT_MAX)
						display = INT_MAX;
					break;
				case CALCMODE_64BIT:
					break;
			}

			// correct buffers
			_i64toa_s(display, In_Bin, 255, 2);
			_i64toa_s(display, In_Oct, 255, 8);
			_i64toa_s(display, In_Dec, 255, 10);
			_i64toa_s(display, In_Hex, 255, 16);
		}

		//handles a number input
		void CCalculatorDlgBar::HandleNumber(int i)
		{
			char s[2];
			unsigned __int64 var;

			// s = a string containing "i" as an ansii character
			s[0] = (i < 0x0a) ? s[0] = (char) i + 0x30 : (char) i + 0x57;
			s[1] = '\0';

			// update the buffers
			switch(m_iViewMode)
			{
				case CALCMODE_BIN:
					strcat_s(In_Bin, 255, s);
					var = _strtoui64(In_Bin, NULL, 2);
					break;
				case CALCMODE_OCT:
					strcat_s(In_Oct, 255, s);
					var = _strtoui64(In_Oct, NULL, 8);
					break;
				case CALCMODE_DEC:
					strcat_s(In_Dec, 255, s);
					var = (unsigned __int64)_atoi64(In_Dec);
					break;
				case CALCMODE_HEX:
					strcat_s(In_Hex, 255, s);
					var = _strtoui64(In_Hex, NULL, 16);
					break;
			}

			// determine if this is an argument or just a number
			if(m_iFunction)
				m_i64bit_arg = var;
			else
				m_i64bit = var;

			// update the buffers
			_i64toa_s(var, In_Bin, 255, 2);
			_i64toa_s(var, In_Oct, 255, 8);
			_i64toa_s(var, In_Dec, 255, 10);
			_i64toa_s(var, In_Hex, 255, 16);

			CalcOutput(var);
		}

	/*************************************************************/
	/*			Specific Button Events							 */
	/*************************************************************/

		//NUMBERS
		void CCalculatorDlgBar::OnClickZero() { HandleNumber(0); }
		void CCalculatorDlgBar::OnClickOne() { HandleNumber(1); }
		void CCalculatorDlgBar::OnClickTwo() { HandleNumber(2); }
		void CCalculatorDlgBar::OnClickThree() { HandleNumber(3); }
		void CCalculatorDlgBar::OnClickFour() { HandleNumber(4); }
		void CCalculatorDlgBar::OnClickFive() { HandleNumber(5); }
		void CCalculatorDlgBar::OnClickSix() { HandleNumber(6); }
		void CCalculatorDlgBar::OnClickSeven() { HandleNumber(7); }
		void CCalculatorDlgBar::OnClickEight() { HandleNumber(8); }
		void CCalculatorDlgBar::OnClickNine() { HandleNumber(9); }
		void CCalculatorDlgBar::OnClickA() { HandleNumber(0x0a); }
		void CCalculatorDlgBar::OnClickB() { HandleNumber(0x0b); }
		void CCalculatorDlgBar::OnClickC() { HandleNumber(0x0c); }
		void CCalculatorDlgBar::OnClickD() { HandleNumber(0x0d); }
		void CCalculatorDlgBar::OnClickE() { HandleNumber(0x0e); }
		void CCalculatorDlgBar::OnClickF() { HandleNumber(0x0f); }

		//FUNCTIONS
		void CCalculatorDlgBar::OnClickEquals() { ApplyFunction(); }
		void CCalculatorDlgBar::OnClickDevide(){ SetFunction(CALC_FUNCTION_DEV); }
		void CCalculatorDlgBar::OnClickMultiply(){ SetFunction(CALC_FUNCTION_MUL);}
		void CCalculatorDlgBar::OnClickAdd(){ SetFunction(CALC_FUNCTION_ADD); }
		void CCalculatorDlgBar::OnClickSubtract(){ SetFunction(CALC_FUNCTION_SUB); }

		//MODES
		void CCalculatorDlgBar::OnClickBin() { ResetModes(CALCMODE_BIN); }
		void CCalculatorDlgBar::OnClickHex() { ResetModes(CALCMODE_HEX); }
		void CCalculatorDlgBar::OnClickDec() { ResetModes(CALCMODE_DEC); }
		void CCalculatorDlgBar::OnClickOct() { ResetModes(CALCMODE_OCT); }

		//SIZE
		void CCalculatorDlgBar::OnClick8bit() { SetBitConfig(CALCMODE_8BIT); }
		void CCalculatorDlgBar::OnClick16bit() { SetBitConfig(CALCMODE_16BIT); }
		void CCalculatorDlgBar::OnClick32bit() { SetBitConfig(CALCMODE_32BIT); }
		void CCalculatorDlgBar::OnClick64bit() { SetBitConfig(CALCMODE_64BIT); }

		//remove one character
		void CCalculatorDlgBar::OnClickBackspace()
		{
			unsigned __int64 var;

			// trime the buffers
			switch(m_iViewMode)
			{
				case CALCMODE_BIN:
					if(strlen(In_Bin)) In_Bin[strlen(In_Bin)-1] = '\0';
					var = _strtoui64(In_Bin, NULL, 2);
					break;
				case CALCMODE_OCT:
					if(strlen(In_Oct)) In_Oct[strlen(In_Oct)-1] = '\0';
					var = _strtoui64(In_Oct, NULL, 8);
					break;
				case CALCMODE_DEC:
					if(strlen(In_Dec)) In_Dec[strlen(In_Dec)-1] = '\0';
					var = (unsigned __int64)_atoi64(In_Dec);
					break;
				case CALCMODE_HEX:
					if(strlen(In_Hex)) In_Hex[strlen(In_Hex)-1] = '\0';
					var = _strtoui64(In_Hex, NULL, 16);
					break;
			}

			// determine if this is an argument or just a number
			if(m_iFunction)
				m_i64bit_arg = var;
			else
				m_i64bit = var;

			// update the buffers
			_i64toa_s(var, In_Bin, 255, 2);
			_i64toa_s(var, In_Oct, 255, 8);
			_i64toa_s(var, In_Dec, 255, 10);
			_i64toa_s(var, In_Hex, 255, 16);

			CalcOutput(var);
		}

		//clear display
		void CCalculatorDlgBar::OnClickClear()
		{
			m_iFunction = 0;
			m_i64bit = 0;

			strcpy_s(In_Bin, 255, "0");
			strcpy_s(In_Oct, 255, "0");
			strcpy_s(In_Dec, 255, "0");
			strcpy_s(In_Hex, 255, "0");
	
			CalcOutput(m_i64bit);
		}

/*************************************************************/
/*			CCalculatorDlgBar Message Map					 */
/*************************************************************/

IMPLEMENT_DYNAMIC(CCalculatorDlgBar, CDialogBar)

BEGIN_MESSAGE_MAP(CCalculatorDlgBar, CDialogBar)
	ON_WM_CREATE()
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog)

	//enable buttons
	ON_UPDATE_COMMAND_UI(IDC_CALC_1, &CCalculatorDlgBar::OnUpdateOPButton)
	ON_UPDATE_COMMAND_UI(IDC_CALC_2, &CCalculatorDlgBar::OnUpdateOPButton)
	ON_UPDATE_COMMAND_UI(IDC_CALC_3, &CCalculatorDlgBar::OnUpdateOPButton)
	ON_UPDATE_COMMAND_UI(IDC_CALC_4, &CCalculatorDlgBar::OnUpdateOPButton)
	ON_UPDATE_COMMAND_UI(IDC_CALC_5, &CCalculatorDlgBar::OnUpdateOPButton)
	ON_UPDATE_COMMAND_UI(IDC_CALC_6, &CCalculatorDlgBar::OnUpdateOPButton)
	ON_UPDATE_COMMAND_UI(IDC_CALC_7, &CCalculatorDlgBar::OnUpdateOPButton)
	ON_UPDATE_COMMAND_UI(IDC_CALC_8, &CCalculatorDlgBar::OnUpdateOPButton)
	ON_UPDATE_COMMAND_UI(IDC_CALC_9, &CCalculatorDlgBar::OnUpdateOPButton)
	ON_UPDATE_COMMAND_UI(IDC_CALC_0, &CCalculatorDlgBar::OnUpdateOPButton)
	ON_UPDATE_COMMAND_UI(IDC_CALC_CLEAR, &CCalculatorDlgBar::OnUpdateOPButton)
	ON_UPDATE_COMMAND_UI(IDC_CALC_PLUS, &CCalculatorDlgBar::OnUpdateOPButton)
	ON_UPDATE_COMMAND_UI(IDC_CALC_MINUS, &CCalculatorDlgBar::OnUpdateOPButton)
	ON_UPDATE_COMMAND_UI(IDC_CALC_TIMES, &CCalculatorDlgBar::OnUpdateOPButton)
	ON_UPDATE_COMMAND_UI(IDC_CALC_DEVIDE, &CCalculatorDlgBar::OnUpdateOPButton)
	ON_UPDATE_COMMAND_UI(IDC_CALC_EQUALS, &CCalculatorDlgBar::OnUpdateOPButton)
	ON_UPDATE_COMMAND_UI(IDC_CALC_BACKSPACE, &CCalculatorDlgBar::OnUpdateOPButton)
	ON_UPDATE_COMMAND_UI(IDC_CALC_A, &CCalculatorDlgBar::OnUpdateOPButton)
	ON_UPDATE_COMMAND_UI(IDC_CALC_B, &CCalculatorDlgBar::OnUpdateOPButton)
	ON_UPDATE_COMMAND_UI(IDC_CALC_C, &CCalculatorDlgBar::OnUpdateOPButton)
	ON_UPDATE_COMMAND_UI(IDC_CALC_D, &CCalculatorDlgBar::OnUpdateOPButton)
	ON_UPDATE_COMMAND_UI(IDC_CALC_E, &CCalculatorDlgBar::OnUpdateOPButton)
	ON_UPDATE_COMMAND_UI(IDC_CALC_F, &CCalculatorDlgBar::OnUpdateOPButton)
	
	//process buttons
	ON_COMMAND(IDC_CALC_0, &CCalculatorDlgBar::OnClickZero)
	ON_COMMAND(IDC_CALC_1, &CCalculatorDlgBar::OnClickOne)
	ON_COMMAND(IDC_CALC_2, &CCalculatorDlgBar::OnClickTwo)
	ON_COMMAND(IDC_CALC_3, &CCalculatorDlgBar::OnClickThree)
	ON_COMMAND(IDC_CALC_4, &CCalculatorDlgBar::OnClickFour)
	ON_COMMAND(IDC_CALC_5, &CCalculatorDlgBar::OnClickFive)
	ON_COMMAND(IDC_CALC_6, &CCalculatorDlgBar::OnClickSix)
	ON_COMMAND(IDC_CALC_7, &CCalculatorDlgBar::OnClickSeven)
	ON_COMMAND(IDC_CALC_8, &CCalculatorDlgBar::OnClickEight)
	ON_COMMAND(IDC_CALC_9, &CCalculatorDlgBar::OnClickNine)
	ON_COMMAND(IDC_CALC_A, &CCalculatorDlgBar::OnClickA)
	ON_COMMAND(IDC_CALC_B, &CCalculatorDlgBar::OnClickB)
	ON_COMMAND(IDC_CALC_C, &CCalculatorDlgBar::OnClickC)
	ON_COMMAND(IDC_CALC_D, &CCalculatorDlgBar::OnClickD)
	ON_COMMAND(IDC_CALC_E, &CCalculatorDlgBar::OnClickE)
	ON_COMMAND(IDC_CALC_F, &CCalculatorDlgBar::OnClickF)

	ON_COMMAND(IDC_CALC_EQUALS, &CCalculatorDlgBar::OnClickEquals)
	ON_COMMAND(IDC_CALC_DEVIDE, &CCalculatorDlgBar::OnClickDevide)
	ON_COMMAND(IDC_CALC_TIMES, &CCalculatorDlgBar::OnClickMultiply)
	ON_COMMAND(IDC_CALC_CLEAR, &CCalculatorDlgBar::OnClickClear)
	ON_COMMAND(IDC_CALC_MINUS, &CCalculatorDlgBar::OnClickSubtract)
	ON_COMMAND(IDC_CALC_PLUS, &CCalculatorDlgBar::OnClickAdd)
	ON_COMMAND(IDC_CALC_BACKSPACE, &CCalculatorDlgBar::OnClickBackspace)

	ON_COMMAND(IDC_CALC_BIN, &CCalculatorDlgBar::OnClickBin)
	ON_COMMAND(IDC_CALC_HEX, &CCalculatorDlgBar::OnClickHex)
	ON_COMMAND(IDC_CALC_DEC, &CCalculatorDlgBar::OnClickDec)
	ON_COMMAND(IDC_CALC_OCT, &CCalculatorDlgBar::OnClickOct)

	ON_COMMAND(IDC_CALC_EIGHTBIT, &CCalculatorDlgBar::OnClick8bit)
	ON_COMMAND(IDC_CALC_SIXTEENBIT, &CCalculatorDlgBar::OnClick16bit)
	ON_COMMAND(IDC_CALC_THIRTYTWOBIT, &CCalculatorDlgBar::OnClick32bit)
	ON_COMMAND(IDC_CALC_SIXTYFOURBIT, &CCalculatorDlgBar::OnClick64bit)
END_MESSAGE_MAP()
