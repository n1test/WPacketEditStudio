/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  PESHook.h

  This file is part of the Packet Edit Studio Project.
  version: ALPHA

  *Distributed under the terms of the zlib/libpng public license.*
  
  Author: 

	Chris Smith (sourcemagik.com) - ClientIPCFunctor
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

//ipc callback object for handling hooks

	#pragma once

	#include "ListCtrlStyled.h"
	#include "PacketListView.h"

	/*************************************************************/
	/*			ClientIPCFunctor								 */
	/*															 */
	/*	This class handles each individual hook.  In other words */
	/*	it is the interface between the hook manager and the	 */
	/*	application itself.										 */
	/*************************************************************/
	class ClientIPCFunctor : public CHookFunctor
	{
		public:

			/////////////////////////////
			//  Constructor / Destructor
			/////////////////////////////
			
				ClientIPCFunctor();

				virtual ~ClientIPCFunctor();

			/////////////////////////////
			//  Hook Info Access
			/////////////////////////////

				//gets the number of packets captured by this hook
				UINT GetTotalPackets();

				//gets the hook handle associated with this object
				HOOK_HANDLE GetHookHandle();

				//gets the packet buffer object
				CPacketBuffer &GetPacketBuffer();

			/////////////////////////////
			//  Management
			/////////////////////////////

				//for outputting the data captured
				void FillPacketListView(CPacketListView &ListView);

				//only call this when the object is initialized
				void SetHookHandle(HOOK_HANDLE hHook);

		private:

			/////////////////////////////
			//  Hook Event Overrides
			/////////////////////////////
			
				//initialization code
				virtual void InitFunctor();

				//data is ready
				virtual void CallbackRaised();

			/////////////////////////////
			//  Private Variables
			/////////////////////////////

				CPacketBuffer *m_pPacketBuffer;						//the packet buffer object to store the data recieved
				PEStudioIPC *m_IPCData;								//ipc object pointer to the shared mem location
				HOOK_HANDLE m_ProtectedHandle;						//the handle to the hook object
	};

/*************************************************************/
/*			Types											 */
/*************************************************************/
	typedef CHookManager <ClientIPCFunctor> CPESHookManager;