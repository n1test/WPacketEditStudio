#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const TCHAR htmlKeyWords[] = 
	_T("and and_eq asm auto bitand bitor bool break ")
  _T("case catch char class compl const const_cast continue ")
  _T("default delete do double dynamic_cast else enum explicit export extern false float for ")
  _T("friend goto if inline int long mutable namespace new not not_eq ")
  _T("operator or or_eq private protected public ")
  _T("register reinterpret_cast return short signed sizeof static static_cast struct switch ")
  _T("template this throw true try typedef typeid typename union unsigned using ")
  _T("virtual void volatile wchar_t while xor xor_eq ");

//todo: add php funcs
const char phpKeyWords[] = "include and or xor __FILE__ exception __LINE__ array() as "
                           "break case class const continue declare default die do echo "
						   "else elseif empty enddeclare endfor endforeach endif endswitch "
						   "endwhile eval exit extends for foreach function global "
						   "include include_once isset list new print require require_once "
						   "return static switch unset use var while __FUNCTION__ __CLASS__ "
						   "__METHOD__ final php_user_filter interface implements instanceof "
						   "public private protected abstract clone try catch throw cfunction "
						   "old_function this final __NAMESPACE__ namespace goto __DIR__ "

						   "$bytelength $bytedata $api "
						   "$script_name $script_author $script_version ";

;


IMPLEMENT_DYNCREATE(CScintillaDemoView, CScintillaView)

BEGIN_MESSAGE_MAP(CScintillaDemoView, CScintillaView)
/*	ON_COMMAND(ID_OPTIONS_ADDMARKER, OnOptionsAddmarker)
	ON_COMMAND(ID_OPTIONS_DELETEMARKER, OnOptionsDeletemarker)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_DELETEMARKER, OnUpdateOptionsDeletemarker)
	ON_COMMAND(ID_OPTIONS_FIND_NEXTMARKER, OnOptionsFindNextmarker)
	ON_COMMAND(ID_OPTIONS_FIND_PREVMARKER, OnOptionsFindPrevmarker)
	ON_COMMAND(ID_OPTIONS_FOLD_MARGIN, OnOptionsFoldMargin)
	ON_COMMAND(ID_OPTIONS_SELECTION_MARGIN, OnOptionsSelectionMargin)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_SELECTION_MARGIN, OnUpdateOptionsSelectionMargin)
	ON_COMMAND(ID_OPTIONS_VIEW_LINENUMBERS, OnOptionsViewLinenumbers)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_VIEW_LINENUMBERS, OnUpdateOptionsViewLinenumbers)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_ADDMARKER, OnUpdateOptionsAddmarker)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_FOLD_MARGIN, OnUpdateOptionsFoldMargin)
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_LINE, OnUpdateLine) 
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_STYLE, OnUpdateStyle) 
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_FOLD, OnUpdateFold) 
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_OVR, OnUpdateInsert)*/
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


CScintillaDemoView::CScintillaDemoView()
{
  LoadMarginSettings();
}

void CScintillaDemoView::OnDraw(CDC* /*pDC*/)
{
	CDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

#ifdef _DEBUG
void CScintillaDemoView::AssertValid() const
{
	CScintillaView::AssertValid();
}

void CScintillaDemoView::Dump(CDumpContext& dc) const
{
	CScintillaView::Dump(dc);
}

CDocument* CScintillaDemoView::GetDocument() //non-debug version is inline
{
	//ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScintillaDemoDoc)));
	return static_cast<CDocument*>(m_pDocument);
}
#endif //_DEBUG

void CScintillaDemoView::SetAStyle(int style, COLORREF fore, COLORREF back, int size, const char* face) 
{
  CScintillaCtrl& rCtrl = GetCtrl();

	rCtrl.StyleSetFore(style, fore);
	rCtrl.StyleSetBack(style, back);
	if (size >= 1)
		rCtrl.StyleSetSize(style, size);
	if (face) 
		rCtrl.StyleSetFont(style, face);
}

void CScintillaDemoView::DefineMarker(int marker, int markerType, COLORREF fore, COLORREF back) 
{
	CScintillaCtrl& rCtrl = GetCtrl();

	rCtrl.MarkerDefine(marker, markerType);
	rCtrl.MarkerSetFore(marker, fore);
	rCtrl.MarkerSetBack(marker, back);
}

int CScintillaDemoView::GetTotalSize()
{
	CScintillaCtrl& rCtrl = GetCtrl();
	return rCtrl.GetTextLength();
}

void CScintillaDemoView::GetText(char *buffer, int sz)
{
	CScintillaCtrl& rCtrl = GetCtrl();
	rCtrl.GetText(sz, buffer);	
}

void CScintillaDemoView::OnInitialUpdate() 
{
  //Let the base class do its thing
	CScintillaView::OnInitialUpdate();
	
  CScintillaCtrl& rCtrl = GetCtrl();

  //Setup the Lexer
  rCtrl.SetLexer(SCLEX_CPP);
  //rCtrl.SetKeyWords(0, htmlKeyWords);
  rCtrl.SetKeyWords(0, phpKeyWords);
  int size = rCtrl.TextWidth(STYLE_LINENUMBER, "_99999");
  rCtrl.SetMarginWidthN(0, size);

  SetAStyle(STYLE_DEFAULT, RGB(0, 0, 0), RGB(0xff, 0xff, 0xff), 16, "Courier");
  //rCtrl.StyleClearAll();

 /* SetAStyle(SCE_H_DEFAULT, RGB(0x00, 0, 0));
  SetAStyle(SCE_H_TAG, RGB(0xFF, 0, 0));
  SetAStyle(SCE_H_TAGUNKNOWN, RGB(0xFF, 0, 0));
  SetAStyle(SCE_H_ATTRIBUTE, RGB(0xFF, 0, 0));
  SetAStyle(SCE_H_ATTRIBUTEUNKNOWN, RGB(0xFF, 0, 0));
  SetAStyle(SCE_H_NUMBER, RGB(0xFF, 0, 0));
  SetAStyle(SCE_H_DOUBLESTRING, RGB(0xFF, 0, 0));
  SetAStyle(SCE_H_SINGLESTRING, RGB(0xFF, 0, 0));
  SetAStyle(SCE_H_OTHER, RGB(0xFF, 0, 0));
  SetAStyle(SCE_H_COMMENT, RGB(0xFF, 0, 0));
  SetAStyle(SCE_H_ENTITY, RGB(0xFF, 0, 0));
  SetAStyle(SCE_H_VALUE, RGB(0xFF, 0, 0));*/

 // rCtrl.SetLexer(SCLEX_PHPSCRIPT);
  //rCtrl.StyleClearAll();
 /* SetAStyle(SCE_HPHP_COMPLEX_VARIABLE, RGB(0xFF, 0, 0));
  SetAStyle(SCE_HPHP_DEFAULT, RGB(0xFF, 0, 0));
  SetAStyle(SCE_HPHP_HSTRING, RGB(0xFF, 0, 0));
  SetAStyle(SCE_HPHP_SIMPLESTRING, RGB(0xFF, 0, 0));
  SetAStyle(SCE_HPHP_WORD, RGB(0xFF, 0, 0));
  SetAStyle(SCE_HPHP_NUMBER, RGB(0xFF, 0, 0));
  SetAStyle(SCE_HPHP_VARIABLE, RGB(0xFF, 0, 0));
  SetAStyle(SCE_HPHP_COMMENT, RGB(0xFF, 0, 0));
  SetAStyle(SCE_HPHP_COMMENTLINE, RGB(0xFF, 0, 0));
  SetAStyle(SCE_HPHP_HSTRING_VARIABLE, RGB(0xFF, 0, 0));
  SetAStyle(SCE_HPHP_OPERATOR, RGB(0xFF, 0, 0));
  SetAStyle(SCE_ERR_PHP, RGB(0xFF, 0, 0));*/

  //a bad hack for php highlighting not working!
  SetAStyle(STYLE_DEFAULT, RGB(0, 0, 0), RGB(0xff, 0xff, 0xff), 11, "Verdana");
  rCtrl.StyleClearAll();
  SetAStyle(SCE_C_DEFAULT, RGB(0, 0, 0));
  SetAStyle(SCE_C_COMMENT, RGB(0, 0x80, 0));
  SetAStyle(SCE_C_COMMENTLINE, RGB(0, 0x80, 0));
  SetAStyle(SCE_C_COMMENTDOC, RGB(0, 0x80, 0));
  SetAStyle(SCE_C_COMMENTLINEDOC, RGB(0, 0x80, 0));
  SetAStyle(SCE_C_COMMENTDOCKEYWORD, RGB(0, 0x80, 0));
  SetAStyle(SCE_C_COMMENTDOCKEYWORDERROR, RGB(0, 0x80, 0));
  SetAStyle(SCE_C_NUMBER, RGB(0, 0x80, 0x80));
  SetAStyle(SCE_C_WORD, RGB(0, 0, 0x80));
  rCtrl.StyleSetBold(SCE_C_WORD, 1);
  SetAStyle(SCE_C_CHARACTER, RGB(0x80, 0, 0x80));
  SetAStyle(SCE_C_STRING, RGB(0x80, 0, 0x80));
  SetAStyle(SCE_C_IDENTIFIER, RGB(0, 0, 0));
  //SetAStyle(SCE_C_PREPROCESSOR, RGB(0x80, 0, 0));
  SetAStyle(SCE_C_OPERATOR, RGB(0x80, 0x80, 0));

  //Setup folding
  rCtrl.SetMarginWidthN(2, 16);
	rCtrl.SetMarginSensitiveN(2, TRUE);
	rCtrl.SetMarginTypeN(2, SC_MARGIN_SYMBOL);
	rCtrl.SetMarginMaskN(2, SC_MASK_FOLDERS);
  rCtrl.SetProperty(_T("fold"), _T("1"));

  //Setup markers
	DefineMarker(SC_MARKNUM_FOLDEROPEN, SC_MARK_MINUS, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0xFF));
	DefineMarker(SC_MARKNUM_FOLDER, SC_MARK_PLUS, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
	DefineMarker(SC_MARKNUM_FOLDERSUB, SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
	DefineMarker(SC_MARKNUM_FOLDERTAIL, SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
	DefineMarker(SC_MARKNUM_FOLDEREND, SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
	DefineMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
	DefineMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));

  //Setup auto completion
  rCtrl.AutoCSetSeparator(10); //Use a separator of line feed*/

  //Setup call tips
  rCtrl.SetMouseDwellTime(1000);

#ifdef _DEBUG
  AfxDump(this);
#endif
}

void CScintillaDemoView::OnOptionsAddmarker() 
{
  CScintillaCtrl& rCtrl = GetCtrl();
  int nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
	rCtrl.MarkerAdd(nLine, 0);
}

void CScintillaDemoView::OnUpdateOptionsAddmarker(CCmdUI* pCmdUI) 
{
  CScintillaCtrl& rCtrl = GetCtrl();
  int nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
  int nBits = rCtrl.MarkerGet(nLine);
  pCmdUI->Enable((nBits & 0x1) == 0);	
}

void CScintillaDemoView::OnOptionsDeletemarker() 
{
  CScintillaCtrl& rCtrl = GetCtrl();
  int nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
  rCtrl.MarkerDelete(nLine, 0);
}

void CScintillaDemoView::OnUpdateOptionsDeletemarker(CCmdUI* pCmdUI) 
{
  CScintillaCtrl& rCtrl = GetCtrl();
  int nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
  int nBits = rCtrl.MarkerGet(nLine);
  pCmdUI->Enable(nBits & 0x1);	
}

void CScintillaDemoView::OnOptionsFindNextmarker() 
{
  CScintillaCtrl& rCtrl = GetCtrl();
  int nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
  int nFoundLine = rCtrl.MarkerNext(nLine + 1, 0x1);
  if (nFoundLine >= 0)
    rCtrl.GotoLine(nFoundLine);
  else
    MessageBeep(MB_ICONHAND);
}

void CScintillaDemoView::OnOptionsFindPrevmarker() 
{
  CScintillaCtrl& rCtrl = GetCtrl();
  int nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
  int nFoundLine = rCtrl.MarkerPrevious(nLine - 1, 0x1);
  if (nFoundLine >= 0)
    rCtrl.GotoLine(nFoundLine);
  else
    MessageBeep(MB_ICONHAND);
}

void CScintillaDemoView::OnOptionsFoldMargin() 
{
  CScintillaCtrl& rCtrl = GetCtrl();
	int nMarginWidth = rCtrl.GetMarginWidthN(2);
  if (nMarginWidth)
    rCtrl.SetMarginWidthN(2, 0);
  else
	  rCtrl.SetMarginWidthN(2, 16);
}

void CScintillaDemoView::OnUpdateOptionsFoldMargin(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(GetCtrl().GetMarginWidthN(2) != 0);
}

void CScintillaDemoView::OnOptionsSelectionMargin() 
{
  CScintillaCtrl& rCtrl = GetCtrl();
	int nMarginWidth = rCtrl.GetMarginWidthN(1);
  if (nMarginWidth)
    rCtrl.SetMarginWidthN(1, 0);
  else
	  rCtrl.SetMarginWidthN(1, 16);
}

void CScintillaDemoView::OnUpdateOptionsSelectionMargin(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(GetCtrl().GetMarginWidthN(1) != 0);
}

void CScintillaDemoView::OnOptionsViewLinenumbers() 
{
  CScintillaCtrl& rCtrl = GetCtrl();
	int nMarginWidth = rCtrl.GetMarginWidthN(0);
  if (nMarginWidth)
    rCtrl.SetMarginWidthN(0, 0);
  else
	  rCtrl.SetMarginWidthN(0, 32);
}

void CScintillaDemoView::OnUpdateOptionsViewLinenumbers(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetCtrl().GetMarginWidthN(0) != 0);
}

void CScintillaDemoView::OnUpdateInsert(CCmdUI* pCmdUI)
{
  CString sText;
  if (GetCtrl().GetOvertype())
    VERIFY(sText.LoadString(ID_INDICATOR_OVR));
	pCmdUI->SetText(sText);
}

void CScintillaDemoView::OnUpdateStyle(CCmdUI* pCmdUI)
{
   CScintillaCtrl& rCtrl = GetCtrl();
   int nPos = rCtrl.GetCurrentPos();
   int nStyle = rCtrl.GetStyleAt(nPos);
   CString sLine;
  // sLine.Format(IDS_STYLE_INDICATOR, nStyle);
   pCmdUI->SetText(sLine);
} 

void CScintillaDemoView::OnUpdateFold(CCmdUI* pCmdUI)
{
  CScintillaCtrl& rCtrl = GetCtrl();
  int nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
  int nLevel = rCtrl.GetFoldLevel(nLine) & SC_FOLDLEVELNUMBERMASK;
  nLevel -= 1024;

  CString sLine;
//  sLine.Format(IDS_FOLD_INDICATOR, nLevel);
  pCmdUI->SetText(sLine);
} 

void CScintillaDemoView::OnUpdateLine(CCmdUI* pCmdUI)
{
  CScintillaCtrl& rCtrl = GetCtrl();
  int nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
  int nColumn = rCtrl.GetColumn(nPos);

  CString sLine;
//  sLine.Format(IDS_LINE_INDICATOR, nLine, nColumn, nPos);
  pCmdUI->SetText(sLine);
} 

void CScintillaDemoView::SetText(char *buffer)
{
	CScintillaCtrl& rCtrl = GetCtrl();
	rCtrl.SelectAll();
	rCtrl.ReplaceSel(buffer);
}

//Some simple examples of implementing auto completion
void CScintillaDemoView::OnCharAdded(SCNotification* /*pSCNotification*/p)
{
  // for the code completion
/*
  //Get the previous 13 characters and if it is "scintilla is " case insensitive
  //then display a list which allows "very cool", "easy" or the "way cool!!"
  int nStartSel = rCtrl.GetSelectionStart();
  int nEndSel = rCtrl.GetSelectionEnd();
  if ((nStartSel == nEndSel) && (nStartSel >= 13))
  {
    TextRange tr;
    tr.chrg.cpMin = nStartSel - 13;
    tr.chrg.cpMax = nEndSel;
    char sText[14];
    sText[0] = '\0';
    tr.lpstrText = sText;
    rCtrl.GetTextRange(&tr);

    //Display the auto completion list
    if (_strcmpi(sText, "scintilla is ") == 0)
    {
      //Display the auto completion list
      rCtrl.AutoCShow(0, _T("very cool\neasy to use\nway cool!!"));
    }
  }
  
  //As another example, get the previous 2 characters and if it is "res" case sensitive
  //then display a list which allows "resize", "restart" and "restore"
  if ((nStartSel == nEndSel) && (nStartSel >= 3))
  {
	  TextRange tr;
	  tr.chrg.cpMin = nStartSel - 3;
	  tr.chrg.cpMax = nEndSel;
	  char sText[4];
	  sText[0] = '\0';
	  tr.lpstrText = sText;
	  rCtrl.GetTextRange(&tr);

	  //Display the auto completion list
	  if (strcmp(sText, "res") == 0)
	  {
		  //Display the auto completion list
		  rCtrl.AutoCShow(3, _T("resize\nrestart\nrestore"));
	  }
  }*/
}

//A simple example of call tips
void CScintillaDemoView::OnDwellStart(SCNotification* pSCNotification)
{
  CScintillaCtrl& rCtrl = GetCtrl();

  //Only display the call tip if the scintilla window has focus
  CWnd* pFocusWnd = GetFocus();
  if (pFocusWnd && (pFocusWnd->GetSafeHwnd() == rCtrl.GetSafeHwnd()))
  {
    //Get the previous 7 characters and next 7 characters arround
    //the current dwell position and if it is "author " case insensitive
    //then display "PJ Naughter" as a call tip
    TextRange tr;
    tr.chrg.cpMin = max(0, pSCNotification->position - 7);
    tr.chrg.cpMax = min(pSCNotification->position + 7, rCtrl.GetLength());
    char sText[15];
    sText[0] = '\0';
    tr.lpstrText = sText;
    rCtrl.GetTextRange(&tr);

    //Display the call tip
    _strupr_s(sText, sizeof(sText));
    if (strstr(sText, "AUTHOR "))
      rCtrl.CallTipShow(pSCNotification->position, _T("PJ Naughter"));
  }
}

void CScintillaDemoView::OnDwellEnd(SCNotification* /*pSCNotification*/)
{
  CScintillaCtrl& rCtrl = GetCtrl();

  //Cancel any outstanding call tip
  if (rCtrl.CallTipActive())
    rCtrl.CallTipCancel();
}

void CScintillaDemoView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
  //Let the base class do its thing
	CScintillaView::OnActivate(nState, pWndOther, bMinimized);

  CScintillaCtrl& rCtrl = GetCtrl();

  //Cancel any outstanding call tip
	if (nState == WA_INACTIVE && rCtrl.CallTipActive())
    rCtrl.CallTipCancel();
}

void CScintillaDemoView::OnModifyAttemptRO(SCNotification* /*pSCNotification*/)
{
//  if (AfxMessageBox(IDP_ALLOW_MODIFY_READONLY_FILE, MB_YESNO) == IDYES)
    GetCtrl().SetReadOnly(FALSE);
}