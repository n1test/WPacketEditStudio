/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  PacketBuffer.h

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

/*************************************************************/
/*			Structures										 */
/*************************************************************/

	//for returning details about what ipc was recieved
	struct SniffDetails
	{
		int SendCalls;
		int RecvCalls;
		int WSASendCalls;
		int WSARecvCalls;
		int SendToCalls;
		int RecvFromCalls;
		int WSASendToCalls;
		int WSARecvFromCalls;
	};

	//a packet buffer element
	struct PacketBufferNode
	{
		PEStudioIPC data;
		LPBYTE packet;
		PacketBufferNode *next;
	};

	/*************************************************************/
	/*			CPacketBuffer									 */
	/*															 */
	/*	A class for storing recieved packet data.				 */
	/*************************************************************/
	class CPacketBuffer
	{
		public:

			/////////////////////////////
			//  Constructor / Destructor
			/////////////////////////////

				CPacketBuffer();

				CPacketBuffer(UINT size);

				virtual ~CPacketBuffer();

			/////////////////////////////
			//  Packet Management
			/////////////////////////////

				//add a packet to the list
				bool AddPacket(PEStudioIPC *data);

				//packet iterator
				PacketBufferNode *GetFirstPacket();
				PacketBufferNode *GetNextPacket();

			/////////////////////////////
			//  Buffer Info / Statistics
			/////////////////////////////

				//can the buffer be accessed
				bool CanWrite();

				//gets a packet's data (in original form)
				unsigned char *GetPacketData(int i, DWORD *sz);

				//statistics
				unsigned int GetFillPercent();
				unsigned int GetPacketsSent();
				unsigned int GetPacketsRecved();
				unsigned int GetTotalPackets();
				unsigned int GetAvgSize();
				unsigned int GetTotalBytes();

				//fills in a SniffDetails structure with current buffer info
				void FillDetailStruct(SniffDetails *details);

				//converts a DWORD PES type to it's string equivilant
				char *strGetFunc(DWORD dwType);

			/////////////////////////////
			//  Private Variables
			/////////////////////////////
		private:

			//BUFFER
			PacketBufferNode *base, *iterator;				//the first node in the buffer and the list iterator
			unsigned char *pckbuff;							//the buffer which holds the packets
			unsigned char *c_pos;							//point to insert next packet at
			unsigned int max_alloc;							//the size of the buffer
			unsigned int written;							//bytes written to the packet buffer
			bool canwrite;									//is the buffer accessible

			//STATS
			unsigned int send_calls;
			unsigned int recv_calls;
			unsigned int WSASend_calls;
			unsigned int WSARecv_calls;
			unsigned int sendto_calls;
			unsigned int recvfrom_calls;
			unsigned int WSASendTo_calls;
			unsigned int WSARecvFrom_calls;
	};