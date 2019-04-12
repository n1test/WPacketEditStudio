/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  ProrcessList.h

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

// This is the interface the drop down process list uses to set
// the hook exe

	#pragma once

	//node for the process list
	struct ProcessNode
	{
		int id;
		char proc[MAX_PATH];
		DWORD dwPid;
		ProcessNode *next;
	};

	/*************************************************************/
	/*			CProcessList									 */
	/*															 */
	/*	Linked List for displaying the drop down list of		 */
	/*	processes												 */
	/*************************************************************/
	template <class HookManagerType, class FrameClassType>
	class CProcessList
	{

		/*************************************************************/
		/*			Constructor / Destructor						 */
		/*************************************************************/
		public:

			//init
			CProcessList(HookManagerType &HookManager, FrameClassType &Frame) :
			hook(HookManager),
			frm(Frame)
			{
				base = NULL;
			}

			//free
			virtual ~CProcessList()
			{
				ProcessNode *p, *n;

				p = base;

				while(p)
				{
					n = p->next;
					delete p;
					p = n;
				}
			}

		/*************************************************************/
		/*			Process Management								 */
		/*************************************************************/

			//add process to the list
			void AddProcess(int i, char *proc, DWORD dwPid)
			{
				ProcessNode *p, *c;

				//allocate
				if(!base)
				{
					base = new ProcessNode;
					p = base;
				}
				else
				{
					p = base;

					while(p)
					{
						c = p;
						p = p->next;
					}

					c->next = new ProcessNode;
					p = c->next;
				}

				//init
				p->id = i;
				p->dwPid = dwPid;
				p->next = NULL;

				strcpy_s(p->proc, MAX_PATH, proc);
			}

			//set the process to hook
			void SetHook(int i)
			{
				char title[255];
				ProcessNode *p;

				p = base;

				while(p)
				{
					if(p->id == i)
					{
						PESApp.SetHook(p->proc, p->dwPid);
						strcpy_s(title, 255, "Packet Edit Studio - ");
						strcat_s(title, 255, p->proc);

						frm.SetWindowTextA(title);
						break;
					}

					p = p->next;
				}
			}

		/*************************************************************/
		/*			Private Variables								 */
		/*************************************************************/
		private:

			ProcessNode *base;						//first process in list
			HookManagerType &hook;					//hook manager object
			CMDIFrameWnd &frm;						//frame object
	};