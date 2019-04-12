/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  PESHookList.cpp

  This file is part of the Packet Edit Studio Project.
  version: ALPHA

  *Distributed under the terms of the zlib/libpng public license.*
  
  Author: 

	Chris Smith (sourcemagik.com) - IPCFunctorList
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

//a management class for keeping track of hook objects

#include "stdafx.h"

/*************************************************************/
/*			IPCFunctorList									 */
/*															 */
/*	This class handles each individual hook.  In other words */
/*	it is the interface between the hook manager and the	 */
/*	application itself.										 */
/*************************************************************/

	/*************************************************************/
	/*			Constructor / Destructor						 */
	/*************************************************************/

		//init
		IPCFunctorList::IPCFunctorList()
		{
			base = NULL;
		}

		//free
		IPCFunctorList::~IPCFunctorList()
		{
			ClientIPCElmt *ptr, *next;

			ptr = base;

			while(ptr)
			{
				next = ptr->next;
				delete ptr;
				ptr = next;
			}
		}

	/*************************************************************/
	/*			Management										 */
	/*************************************************************/

		//creates a new hook interface object
		ClientIPCFunctor *IPCFunctorList::CreateFunctor()
		{
			ClientIPCElmt *ptr;

			if(!base)
			{
				base = new ClientIPCElmt;
				ptr = base;
			}
			else
			{
				ptr = base;
				while(ptr)
				{
					if(!ptr->next)
						break;
					ptr = ptr->next;
				}

				ptr->next = new ClientIPCElmt;
				ptr = ptr->next;
			}

			ptr->next = NULL;

			return &ptr->FunctorObject;
		}

		//remove a hook interface object from memory
		bool IPCFunctorList::DeleteFunctor(HOOK_HANDLE hHook)
		{
			ClientIPCElmt *ptr, *last;

			ptr = base;
			last = NULL;
			while(ptr)
			{
				if(ptr->hHook == hHook)
				{
					if(ptr == base)
						base = ptr->next;
					else if(last)
						last->next = ptr->next;
					delete ptr;

					return true;
				}
				last = ptr;
				ptr = ptr->next;
			}

			return false;
		}

	/*************************************************************/
	/*			Access											 */
	/*************************************************************/

		//get a pointer to a hook interface object
		ClientIPCFunctor *IPCFunctorList::GetFunctor(HOOK_HANDLE hHook)
		{
			ClientIPCElmt *ptr = base;

			while(ptr)
			{
				if(ptr->FunctorObject.GetHookHandle() == hHook)
					return &ptr->FunctorObject;
				ptr = ptr->next;
			}

			return NULL;
		}