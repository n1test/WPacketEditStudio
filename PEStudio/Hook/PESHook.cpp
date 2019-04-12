/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  PESHook.cpp

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

#include "stdafx.h"

/*************************************************************/
/*			ClientIPCFunctor								 */
/*															 */
/*	This class handles each individual hook.  In other words */
/*	it is the interface between the hook manager and the	 */
/*	application itself.										 */
/*************************************************************/

	/*************************************************************/
	/*			Constructor / Destructor						 */
	/*************************************************************/

		//init
		ClientIPCFunctor::ClientIPCFunctor()
		{
			m_ProtectedHandle = 0;
			m_Hook = NULL;
			m_pPacketBuffer = new CPacketBuffer(PESApp.GetBufferSize());
		}

		//free
		ClientIPCFunctor::~ClientIPCFunctor()
		{
			delete m_pPacketBuffer;
		}

	/*************************************************************/
	/*			Hook Info Access								 */
	/*************************************************************/

		//total packets processed
		UINT ClientIPCFunctor::GetTotalPackets()
		{
			return m_pPacketBuffer->GetTotalPackets();
		}

		//the objects hook handle
		HOOK_HANDLE ClientIPCFunctor::GetHookHandle()
		{
			return m_ProtectedHandle;
		}

		//the object's packet buffer
		CPacketBuffer &ClientIPCFunctor::GetPacketBuffer()
		{
			return *m_pPacketBuffer;
		}

	/*************************************************************/
	/*			Management										 */
	/*************************************************************/

		//takes a CPacketListView object which must have already been created and
		//fills it with packets from the object's packet buffer
		void ClientIPCFunctor::FillPacketListView(CPacketListView &ListView)
		{
			static char tmp[512];
			int i = 1;

			CMainFrame &MainFrm = PESApp.GetMainFrm();
			PacketBufferNode *packet = m_pPacketBuffer->GetFirstPacket();

			// process each packet
			while(packet)
			{
				strcpy_s(tmp, 512, packet->data.fcall.ip_from);
				strcat_s(tmp, 512, " : ");
				strcat_s(tmp, 512, itoa(packet->data.fcall.port_from));
				strcat_s(tmp, 512, "  ->  ");
				strcat_s(tmp, 512, packet->data.fcall.ip_to);
				strcat_s(tmp, 512, " : ");
				strcat_s(tmp, 512, itoa(packet->data.fcall.port_to));

				ListView.InsertRow(i, tmp, m_pPacketBuffer->strGetFunc(packet->data.dwMessage), packet->data.dwSize, packet->packet);

				++i;

				packet = m_pPacketBuffer->GetNextPacket();
			}
		}

		//sets the handle to the hook object
		void ClientIPCFunctor::SetHookHandle(HOOK_HANDLE hHook)
		{
			m_ProtectedHandle = hHook;
		}

	/*************************************************************/
	/*			Hook Event Overrides							 */
	/*************************************************************/

		//ipc data init
		void ClientIPCFunctor::InitFunctor()
		{
			CHookFunctor::InitFunctor();

			m_IPCData = (PEStudioIPC*)m_SendData;
		}

		//ipc data process
		void ClientIPCFunctor::CallbackRaised()
		{
			CMainFrame &MainFrm = PESApp.GetMainFrm();

			switch(m_IPCData->dwMessage)
			{
				// add packet messages to the buffer and raise
				// the necessary events
				case IPC_FUNC_SEND:
				case IPC_FUNC_RECV:
				case IPC_FUNC_WSASEND:
				case IPC_FUNC_WSARECV:
				case IPC_FUNC_SENDTO:
				case IPC_FUNC_RECVFROM:
				case IPC_FUNC_WSASENDTO:
				case IPC_FUNC_WSARECVFROM:

					if(m_pPacketBuffer->CanWrite())
					{
						if(!m_pPacketBuffer->AddPacket(m_IPCData))
							MainFrm.OnSniffError(*m_pPacketBuffer);
						else
							MainFrm.OnSniffUpdate(*m_pPacketBuffer);
					}
					break;
				default:
					break;
			}
		}