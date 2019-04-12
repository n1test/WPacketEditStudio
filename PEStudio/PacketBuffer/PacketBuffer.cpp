/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  PacketBuffer.cpp

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

// A class for storing and formatting the packets returned by the dll's
// sniffing mechanism


#include "stdafx.h"

/*************************************************************/
/*			CPacketBuffer									 */
/*															 */
/*	A class for storing recieved packet data.				 */
/*************************************************************/

	/*************************************************************/
	/*			Constructor / Destructor						 */
	/*************************************************************/

		//init
		CPacketBuffer::CPacketBuffer(UINT size)
		{
			max_alloc = size;
			base = NULL;

			written = 0;

			send_calls = 0;
			recv_calls = 0;
			WSASend_calls = 0;
			WSARecv_calls = 0;
			sendto_calls = 0;
			recvfrom_calls = 0;
			WSASendTo_calls = 0;
			WSARecvFrom_calls = 0;

			pckbuff = NULL;

			pckbuff = new unsigned char [size];
			c_pos = pckbuff;

			canwrite = true;
		}

		//free
		CPacketBuffer::~CPacketBuffer()
		{
			PacketBufferNode *p, *n;

			if(pckbuff)
				delete [] pckbuff;

			p = base;

			while(p)
			{
				n = p->next;
				delete p;
				p = n;
			}
		}

	/*************************************************************/
	/*			Packet Management								 */
	/*************************************************************/

		//adds a packet to the buffer
		bool CPacketBuffer::AddPacket(PEStudioIPC *data)
		{

			// check if we can access the correct memory portion
			if((data->dwSize + written) > max_alloc)
			{
				canwrite = false;
				return false;
			}

			// update statistics
			switch(data->dwMessage)
			{
				case IPC_FUNC_SEND:
					++send_calls;
					break;
				case IPC_FUNC_RECV:
					++recv_calls;
					break;
				case IPC_FUNC_WSASEND:
					++WSASend_calls;
					break;
				case IPC_FUNC_WSARECV:
					++WSARecv_calls;
					break;
				case IPC_FUNC_SENDTO:
					++sendto_calls;
					break;
				case IPC_FUNC_RECVFROM:
					++recvfrom_calls;
					break;
				case IPC_FUNC_WSASENDTO:
					++WSASendTo_calls;
					break;
				case IPC_FUNC_WSARECVFROM:
					++WSARecvFrom_calls;
					break;
				default:
					return false;
			}

			// allocate for a new buffer node
			PacketBufferNode *p;
			if(!base)
			{
				base = new PacketBufferNode;
				p = base;
			}
			else
			{
				p = base;

				while(p)
				{
					if(!p->next)
					{
						p->next = new PacketBufferNode;
						p = p->next;
						break;
					}

					p = p->next;
				}
			}

			// copy the data to its memory portion
			memcpy(&p->data, data, sizeof(PEStudioIPC));

			p->packet = c_pos;
			unsigned char *pck = (unsigned char *)data;
			pck += sizeof(PEStudioIPC);
			memcpy(p->packet, pck, p->data.dwSize);

			c_pos += p->data.dwSize;
			written += p->data.dwSize;

			p->next = NULL;

			return true;
		}

		//get first node in the list
		PacketBufferNode *CPacketBuffer::GetFirstPacket()
		{
			iterator = base;
			return iterator;
		}

		//get next node in the list
		PacketBufferNode *CPacketBuffer::GetNextPacket()
		{
			if(!iterator)
				return NULL;

			iterator = iterator->next;

			return iterator;
		}

	/*************************************************************/
	/*			Buffer Info / Statistics						 */
	/*************************************************************/

		//whether or not there is write space left
		bool CPacketBuffer::CanWrite()
		{
			return canwrite;
		}

		//get a packets byte data
		unsigned char * CPacketBuffer::GetPacketData(int i, DWORD *sz)
		{
			PacketBufferNode *p = base;
			int y = 1;

			while(p)
			{
				if(i == y)
				{
					memcpy(sz, &p->data.dwSize, sizeof(DWORD));
					return p->packet;
				}

				p = p->next;
				++y;
			}

			return 0;
		}

		//get the percentage of buffer space that has been used
		unsigned int CPacketBuffer::GetFillPercent()
		{
			return (unsigned int) (((double)written / (double)max_alloc) * 100);
		}

		//get total packets sent
		unsigned int CPacketBuffer::GetPacketsSent()
		{
			return
				send_calls +
				WSASend_calls +
				sendto_calls +
				WSASendTo_calls;
		}

		//get total packets recieved
		unsigned int CPacketBuffer::GetPacketsRecved()
		{
			return
				recv_calls +
				WSARecv_calls +
				recvfrom_calls +
				WSARecvFrom_calls;
		}

		//get total packets sent + recved
		unsigned int CPacketBuffer::GetTotalPackets()
		{
			return
				send_calls +
				recv_calls +
				WSASend_calls +
				WSARecv_calls +
				sendto_calls +
				recvfrom_calls +
				WSASendTo_calls +
				WSARecvFrom_calls;
		}

		//get the avarage size of transferred packets
		unsigned int CPacketBuffer::GetAvgSize()
		{
			return (unsigned int)(written / GetTotalPackets());
		}

		//get total bytes written to the packet buffer
		unsigned int CPacketBuffer::GetTotalBytes()
		{
			return written;
		}

		//get api call details
		void CPacketBuffer::FillDetailStruct(SniffDetails *details)
		{
			details->SendCalls = send_calls;
			details->RecvCalls = recv_calls;
			details->WSASendCalls = WSASend_calls;
			details->WSARecvCalls = WSARecv_calls;
			details->SendToCalls = sendto_calls;
			details->RecvFromCalls = recvfrom_calls;
			details->WSASendToCalls = WSASendTo_calls;
			details->WSARecvFromCalls = WSARecvFrom_calls;
		}

		//get a string representation of an api call
		char *CPacketBuffer::strGetFunc(DWORD dwType)
		{
			switch(dwType)
			{
				case IPC_FUNC_SEND: return "Send";
				case IPC_FUNC_RECV: return "Recv";
				case IPC_FUNC_WSASEND: return "WSASend";
				case IPC_FUNC_WSARECV: return "WSARecv";
				case IPC_FUNC_SENDTO: return "SendTo";
				case IPC_FUNC_RECVFROM: return "RecvFrom";
				case IPC_FUNC_WSASENDTO: return "WSASendTo";
				case IPC_FUNC_WSARECVFROM: return "WSARecvFrom";
				default: return "NULL";
			}
		}
