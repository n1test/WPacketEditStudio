/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  PESHookList.h

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

	//linked list element
	struct ClientIPCElmt
	{
		ClientIPCFunctor FunctorObject;
		ClientIPCElmt *next;
		HOOK_HANDLE hHook;
	};

	/*************************************************************/
	/*			IPCFunctorList									 */
	/*															 */
	/*	This class handles each individual hook.  In other words */
	/*	it is the interface between the hook manager and the	 */
	/*	application itself.										 */
	/*************************************************************/
	class IPCFunctorList
	{
		public:

			/////////////////////////////
			//  Constructor / Destructor
			/////////////////////////////

				IPCFunctorList();

				virtual ~IPCFunctorList();

			/////////////////////////////
			//  Management
			/////////////////////////////

				ClientIPCFunctor *CreateFunctor();

				bool DeleteFunctor(HOOK_HANDLE hHook);

			/////////////////////////////
			//  Access
			/////////////////////////////

				ClientIPCFunctor *GetFunctor(HOOK_HANDLE hHook);

			/////////////////////////////
			//  Private Variables
			/////////////////////////////
		private:

			ClientIPCElmt *base;						//first object in the list
	};