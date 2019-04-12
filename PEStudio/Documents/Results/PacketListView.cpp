// PacketListView.cpp : implementation file
//

#include "stdafx.h"

/*************************************************************/
/*			CPacketListView									 */
/*															 */
/*	A list view class for showing a results document with	 */
/*	a color formatted packet list.							 */
/*************************************************************/

	/*************************************************************/
	/*			Constructor / Destructor						 */
	/*************************************************************/

		//init
		CPacketListView::CPacketListView()
		{
			m_PHPOutputFile[0] = 0;
		}

		//free
		CPacketListView::~CPacketListView()
		{
		}

	/*************************************************************/
	/*			View Events										 */
	/*************************************************************/

		//list view load code
		int CPacketListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
		{
			if (CListView::OnCreate(lpCreateStruct) == -1)
				return -1;

			return 0;
		}

		//selection changed
		void CPacketListView::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
		{
			NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

			// get selected item
			POSITION p = m_listctrl.GetFirstSelectedItemPosition();
			int nItem = m_listctrl.GetNextSelectedItem(p);

			if (!(pNMListView->uNewState & LVIS_SELECTED) || (pNMListView->uOldState & LVIS_SELECTED))
				return;

			// select all packet data in that range
			CString txt = m_listctrl.GetItemText(nItem, 0);
	
			if(txt.FindOneOf(" "))
			{
				PESApp.GetMainFrm().LoadPacketView(atoi(txt), m_hHookHandle);
				return;
			}

			GetFarItem(nItem, PCKLIST_TOP);
			GetFarItem(nItem, PCKLIST_BOTTOM);

			*pResult = 0;
		}

		//initial update
		void CPacketListView::OnInitialUpdate()
		{
			DWORD dwStyle = ListView_GetExtendedListViewStyle(GetListCtrl());

			dwStyle |= LVS_EX_FULLROWSELECT;

			ListView_SetExtendedListViewStyle( GetListCtrl(), dwStyle);
		}

		// VIEW WRAPPER CODE
		
			BOOL CPacketListView::PreCreateWindow(CREATESTRUCT& cs)
			{
				cs.style |= LVS_REPORT | LVS_NOSORTHEADER ;

				return CListView::PreCreateWindow(cs);
			}

			BOOL CPacketListView::PreTranslateMessage(MSG* pMsg)
			{
				if( ! CListView::PreTranslateMessage(pMsg) )
					return m_listctrl.PreTranslateMessage(pMsg);

				return FALSE;
			}

			LRESULT CPacketListView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
			{
				LRESULT lResult = 0;
				if (!OnWndMsg(message, wParam, lParam, &lResult))
					if( !m_listctrl.OnWndMsg(message, wParam, lParam, &lResult))
						lResult = DefWindowProc(message, wParam, lParam);
				return lResult;
			}

			BOOL CPacketListView::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam,
										LRESULT* pLResult)
			{
				if( !CListView::OnChildNotify(message, wParam, lParam, pLResult) )
					return m_listctrl.OnChildNotify(message, wParam, lParam, pLResult) ;

				return FALSE;
			}

		//view has been brought to the front
		void CPacketListView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
		{
			CMainFrame &MainFrm = PESApp.GetMainFrm();

			CView::OnActivateView(bActivate, pActivateView, pDeactiveView);

			if(bActivate && m_PHPOutputFile[0])
				MainFrm.LoadPHPResultsView(m_PHPOutputFile);
		}

	/*************************************************************/
	/*			Display Methods									 */
	/*************************************************************/

		//call before adding members
		void CPacketListView::InitList()
		{
			// set up vars
			m_listctrl.m_hWnd = m_hWnd;
			m_listctrl.m_pfnSuper = m_pfnSuper;
			m_listctrl.PreSubclassWindow();

			// set up columns
			m_listctrl.InsertColumn(0, "Index", LVCFMT_LEFT, -1, 1);
			m_listctrl.InsertColumn(1, "Data", LVCFMT_LEFT, -1, 1);
			m_listctrl.InsertColumn(2, "Size", LVCFMT_LEFT, -1, 1);
			m_listctrl.InsertColumn(3, "", LVCFMT_LEFT, -1, 1);

			m_listctrl.SetColumnWidth(0, 75);
			m_listctrl.SetColumnWidth(1, 475);
			m_listctrl.SetColumnWidth(2, 55);
			m_listctrl.SetColumnWidth(3, 174);

			m_index = 0;
		}

		//insert a new packet into the list
		void CPacketListView::InsertRow(int id, char *route, char *func, int size, unsigned char *packet)
		{
			static char octal[STATIC_BUFFER_SIZE],
						hex[STATIC_BUFFER_SIZE*6],
						ascii[STATIC_BUFFER_SIZE],
						tmp[STATIC_BUFFER_SIZE];
			unsigned int i;

			// create a font for displaying (should be fixed size)
			static CFont font;

			if(!font.GetSafeHandle())
				font.CreatePointFont(60, "Courier");

			// get a formatted data dump of the packet (up to 0x40 bytes)
			full_dump(octal, hex, ascii, packet, size, 0x40);

			// row[1] column[1] = the index of the packet
			m_listctrl.InsertItem(m_index, itoa(id));

			// row[1] column[2] = packet route
			strcpy_s(tmp, STATIC_BUFFER_SIZE, "        ");
			strcat_s(tmp, STATIC_BUFFER_SIZE, route);

			m_listctrl.SetItemText(m_index, 1, route);

			// row[1] column[3] = data size
			m_listctrl.SetItemText(m_index, 2, itoa(size));

			// row[1] column[3] = winsock function
			strcpy_s(tmp, STATIC_BUFFER_SIZE, "        ");
			strcat_s(tmp, STATIC_BUFFER_SIZE, func);
			m_listctrl.SetItemText(m_index, 3, tmp);

			// set header colors and styles
			m_listctrl.SetRowBgColor(m_index, RGB(0x45, 0x45, 0x45), false);
			m_listctrl.SetRowTxtColor(m_index,RGB(0xff, 0xff, 0xff),false);
			m_listctrl.SetRowStyle(m_index, LIS_BGCOLOR | LIS_TXTCOLOR | LIS_BOLD);
			++m_index;

			// create each data row (with the octal data) column[1]
			char *ptr;
			size_t len;

			unsigned int back_ind1 = m_index;
			for(i = 0, ptr = octal, len = strlen(octal); i < len; ++i)
				if(octal[i] == '\n')
				{
					octal[i] = '\0';
					m_listctrl.InsertItem(m_index, ptr);
					m_listctrl.SetRowFont(m_index, &font, false);
					m_listctrl.SetRowBgColor(m_index, RGB(0xcf, 0xcf, 0xcf), false);
					m_listctrl.SetRowTxtColor(m_index, RGB(0x0a,0x24,0x6a), false);
					m_listctrl.SetRowStyle(m_index, LIS_BGCOLOR | LIS_TXTCOLOR);
					ptr = &octal[i+1];
					++m_index;
				}

			// add hex and ascii data (column[2] and column[3])
			unsigned int back_ind2 = back_ind1;
			for(i = 0, ptr = hex, len = strlen(hex); i <= len; ++i)
				if(hex[i] == '\n' || hex[i] == '\0')
				{
					hex[i] = '\0';
					m_listctrl.SetItemText(back_ind1, 1, ptr);
					ptr = &hex[i+1];
					++back_ind1;
				}

			for(i = 0, ptr = ascii, len = strlen(ascii); i <= len; ++i)
				if(ascii[i] == '\n' || ascii[i] == '\0')
				{
					ascii[i] = '\0';
					m_listctrl.SetItemText(back_ind2, 3, ptr);
					ptr = &ascii[i+1];
					++back_ind2;
				}

			// spacer before next item
			m_listctrl.InsertItem(m_index, " ");

			++m_index;
		}

		//gets the index of the furthest extent of the selected packet (pos)
		//in the (top_bottom) direction
		int CPacketListView::GetFarItem(int pos, int top_bottom)
		{
			int to, i;

			// make sure index is valid
			int max = m_listctrl.GetItemCount() - 1;

			if(pos < 0 || pos > max)
				return -1;

			int from = pos;

			// get topmost index
			if(top_bottom == PCKLIST_TOP)
			{
				to = 0;
				for(i = from; i >= to; --i)
				{
					if(m_listctrl.GetItemState(i, LVIS_SELECTED) != LVIS_SELECTED)
						m_listctrl.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);

					CString txt = m_listctrl.GetItemText(i, 0);
					if(txt.GetAllocLength())
						if(txt.FindOneOf(" "))
							break;
				}
			}

			// get bottommost index
			else
			{
				to = max;

				for(i = from; i <= to; ++i)
				{
					CString txt = m_listctrl.GetItemText(i, 0);
					if(txt.FindOneOf(" ") && i != from)
						break;

					if(m_listctrl.GetItemState(i, LVIS_SELECTED) != LVIS_SELECTED)
						m_listctrl.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
				}
			}

			return -1;
		}

	/*************************************************************/
	/*			Hook Setup										 */
	/*************************************************************/

		//the associated hook handle
		HOOK_HANDLE CPacketListView::GetHookHandle()
		{
			return m_hHookHandle;
		}

		//sets the associated hook handle (only call once)
		void CPacketListView::SetHookHandle(HOOK_HANDLE hHook)
		{
			CMainFrame &MainFrm = PESApp.GetMainFrm();

			// load php output window
			if(PESApp.GetLoadPHP())
			{
				strcpy_s(m_PHPOutputFile, 512, PESApp.GetAppDirectory());
				strcat_s(m_PHPOutputFile, 512, "\\logs\\php\\");
				strcat_s(m_PHPOutputFile, 512, PESApp.GetHookExe());
				strcat_s(m_PHPOutputFile, 512, "\\output-");
				strcat_s(m_PHPOutputFile, 512, itoa(hHook));
				strcat_s(m_PHPOutputFile, 512, ".html");
				MainFrm.LoadPHPResultsView(m_PHPOutputFile);
			}
			else
				MainFrm.LoadPHPResultsView("about:blank");

			m_hHookHandle = hHook;
		}

	/*************************************************************/
	/*			Debug Helper Functions							 */
	/*************************************************************/

		#ifdef _DEBUG
			void CPacketListView::AssertValid() const
			{
				CListView::AssertValid();
			}

			#ifndef _WIN32_WCE
				void CPacketListView::Dump(CDumpContext& dc) const
				{
					CListView::Dump(dc);
				}
			#endif
		#endif


/*************************************************************/
/*			CPacketListView Message Map						 */
/*************************************************************/

IMPLEMENT_DYNCREATE(CPacketListView, CListView)

BEGIN_MESSAGE_MAP(CPacketListView, CListView)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnLvnItemchanged)
END_MESSAGE_MAP()
