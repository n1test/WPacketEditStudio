/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  CalculatorDlgBar.h

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

#pragma once


// CCalculatorDlgBar dialog

	/* CONSTANTS */

	#define CALCMODE_8BIT  1        //CHAR
	#define CALCMODE_16BIT 2        //SHORT
	#define CALCMODE_32BIT 3        //INT
	#define CALCMODE_64BIT 4        //__INT64

	#define CALCMODE_BIN 1			//BINARY
	#define CALCMODE_OCT 2			//OCTAL
	#define CALCMODE_DEC 3			//DECIMAL
	#define CALCMODE_HEX 4			//HEXADECIMAL

	#define CALC_FUNCTION_ADD 1     //ADD
	#define CALC_FUNCTION_SUB 2     //SUBTRACT
	#define CALC_FUNCTION_MUL 3     //MULTIPLY
	#define CALC_FUNCTION_DEV 4     //DEVIDE

	/*************************************************************/
	/*			CCalculatorDlgBar								 */
	/*															 */
	/*	A multipurpose calculator dialog						 */
	/*************************************************************/
	class CCalculatorDlgBar : public CDialogBar
	{
		DECLARE_DYNAMIC(CCalculatorDlgBar)
		public:

			enum { IDD = IDD_CALCULATOR };

			/////////////////////////////
			//  Constructor / Destructor
			/////////////////////////////

				CCalculatorDlgBar();

				virtual ~CCalculatorDlgBar();

			/////////////////////////////
			//  Window Events
			/////////////////////////////
		protected:

				void OnUpdateOPButton(CCmdUI *pCmdUI);

				DECLARE_MESSAGE_MAP()
				afx_msg LONG OnInitDialog ( UINT, LONG );
				afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

			/////////////////////////////
			//  Calculator APIS
			/////////////////////////////

				//display the current value
				void CalcOutput(unsigned __int64 display);

				/* sets the calculator byte structure mode
				  (CALCMODE_BIN, CALCMODE_OCT, CALCMODE_DEC, CALCMODE_HEX)
				*/
				void ResetModes(INT mode);

				/* sets the current function flag
				  (CALC_FUNCTION_ADD, CALC_FUNCTION_SUB, CALC_FUNCTION_MUL, CALC_FUNCTION_DEV)
				*/
				void SetFunction(INT function);

				// applies current function flag and flushes the output
				void ApplyFunction();

				/* this just limits the size of the calc display
				  (CALCMODE_8BIT, CALCMODE_16BIT, CALCMODE_32BIT, CALCMODE_64BIT)
				*/
				void SetBitConfig(INT bits);

				// applies data formatting for output; called automatically by CalcOutput
				void FormatOutput(unsigned __int64 display);

				// handles input
				void HandleNumber(int i);

			/////////////////////////////
			//  Specific Button Events
			/////////////////////////////

				/* NUMBERS */

					//base 2
					void OnClickZero();
					void OnClickOne();

					//base 8
					void OnClickTwo();
					void OnClickThree();
					void OnClickFour();
					void OnClickFive();
					void OnClickSix();
					void OnClickSeven();

					//base 10
					void OnClickEight();
					void OnClickNine();

					//base 16
					void OnClickA();
					void OnClickB();
					void OnClickC();
					void OnClickD();
					void OnClickE();
					void OnClickF();

				/* FUNCTIONS */

					void OnClickEquals();
					void OnClickDevide();
					void OnClickMultiply();
					void OnClickClear();
					void OnClickAdd();
					void OnClickSubtract();
					void OnClickBackspace();

				/* BASE CONFIGURATIONS */

					void OnClickBin();
					void OnClickHex();
					void OnClickDec();
					void OnClickOct();

				/* BIT SIZE */

					void OnClick8bit();
					void OnClick16bit();
					void OnClick32bit();
					void OnClick64bit();

			/////////////////////////////
			//  Private Variables
			/////////////////////////////

				INT m_iVarSize;
				INT m_iViewMode;
				INT m_iFunction;

				// current value in base 2 (char [])
				CHAR In_Bin[255];

				// current value in base 8 (char [])
				CHAR In_Oct[255];

				// current value in base 10 (char [])
				CHAR In_Dec[255];

				// current value in base 16 (char [])
				CHAR In_Hex[255];

				//current value in integer(__int64)
				unsigned __int64 m_i64bit;

				//current argument value (for functions like addition / sub)
				unsigned __int64 m_i64bit_arg;

				// the edit window which displays the text
				CEdit *m_pCalcWindow;

				/* BUTTON ACCESS CONTROLS

					(so we can manage buttons that shouldn't be available
					in certain editing modes [like 'A' in decimal mode
					or '3' in binary mode]).

					The variable names are self explainatory.

				*/

				CButton *m_pBinButton;
				CButton *m_pOctButton;
				CButton *m_pHexButton;
				CButton *m_pDecButton;

				CButton *m_pTwoButton;
				CButton *m_pThreeButton;
				CButton *m_pFourButton;
				CButton *m_pFiveButton;
				CButton *m_pSixButton;
				CButton *m_pSevenButton;
				CButton *m_pEightButton;
				CButton *m_pNineButton;

				CButton *m_pAButton;
				CButton *m_pBButton;
				CButton *m_pCButton;
				CButton *m_pDButton;
				CButton *m_pEButton;
				CButton *m_pFButton;

				CButton *m_p8bitButton;
				CButton *m_p16bitButton;
				CButton *m_p32bitButton;
				CButton *m_p64bitButton;

				CButton *m_pSignedButton;
				CButton *m_pUnsignedButton;
				CButton *m_pWholeButton;
				CButton *m_pFloatButton;
	};
