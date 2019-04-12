/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  PacketListView.h

  This file is part of the Packet Edit Studio Project.
  version: ALPHA

  *Distributed under the terms of the zlib/libpng public license.*
  
  Author: 

	Chris Smith (sourcemagik.com) - CPacketList
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

// This is the view control used for displaying packets in the results document
// after the dll is finished sniffing.

	#pragma once

	// CPacketListView view

	/*  This really is just a dummy class to load
		A CListCtrlStyled list into the list view.
	*/
	class CPacketList : public CListCtrlStyled
	{
		CPacketList() {};

		#ifdef _DEBUG
			void AssertValid() const {}
		#endif

		friend class CPacketListView;
	};

	//for selecting packets
	#define PCKLIST_TOP 1
	#define PCKLIST_BOTTOM -1

	/*************************************************************/
	/*			CPacketListView									 */
	/*															 */
	/*	A list view class for showing a results document with	 */
	/*	a color formatted packet list.							 */
	/*************************************************************/
	class CPacketListView : public CListView
	{
		DECLARE_DYNCREATE(CPacketListView)
		protected:

			/////////////////////////////
			//  Constructor / Destructor
			/////////////////////////////
		
				CPacketListView();

				virtual ~CPacketListView();

			/////////////////////////////
			//  View Events
			/////////////////////////////
		public:

			virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
			void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);

			virtual void OnInitialUpdate();
			virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

			virtual BOOL PreTranslateMessage(MSG* pMsg);
			virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
			virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
			virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

		protected:
			DECLARE_MESSAGE_MAP()

			/////////////////////////////
			//  Display Methods
			/////////////////////////////
		public:

				//call this immediately after the packet list is created
				void InitList();

				//add a packet to the list
				void InsertRow(int id, char *route, char *func, int size, unsigned char *packet);

				//for selecting packets
				int GetFarItem(int pos, int top_bottom);

			/////////////////////////////
			//  Hook Setup
			/////////////////////////////

				HOOK_HANDLE GetHookHandle();

				void SetHookHandle(HOOK_HANDLE hHook);

			/////////////////////////////
			//  Debug Helper Functions
			/////////////////////////////
		public:
			#ifdef _DEBUG
				virtual void AssertValid() const;
				#ifndef _WIN32_WCE
					virtual void Dump(CDumpContext& dc) const;
				#endif
			#endif

			/////////////////////////////
			//  Private Variables
			/////////////////////////////
		private:

			CPacketList m_listctrl;						//the list control
			UINT m_index;								//current insert row
			HOOK_HANDLE m_hHookHandle;					//the hook object associated with this list
			char m_PHPOutputFile[512];					//the file containing the php output
	};


