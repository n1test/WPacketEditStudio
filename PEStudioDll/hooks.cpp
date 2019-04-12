/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  hooks.cpp

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

// the api hooks

#include "stdafx.h"

/*************************************************************/
/*			Hook Helpers									 */
/*************************************************************/

	//time to wait before ending process
	#define HOOK_TIMEOUT 3

	//add api hook
	#define HOOK_INSTALL(func_type, func, hook_func) (func_type)DetourFunction((BYTE*)GetProcAddress(GetModuleHandle("ws2_32.dll"), func), (BYTE*)hook_func);

	//remove api hook
	#define HOOK_UNINSTALL(func, hook_func) i = 0; \
											while(CaptureEngine::EjectLock) \
											{ \
												Sleep(100); \
												++i; \
												if(i > HOOK_TIMEOUT*10) \
												{ \
													ExitProcess(0); \
													return; \
												} \
											} \
											DetourRemove((BYTE*)func, (BYTE*)hook_func);

	//function init hooking
	#define HOOK_INIT() Sleep(1); CaptureEngine::EjectLock = true

	//function return value
	#define HOOK_RET int ret; ret

	//function end hooking
	#define HOOK_END() CaptureEngine::EjectLock = false; return ret

/*************************************************************/
/*			Original Functions								 */
/*************************************************************/

	tsend        send_Original;
	trecv        recv_Original;
	tWSARecv     WSARecv_Original;
	tWSASend     WSASend_Original;
	tsendto      sendto_Original;
	trecvfrom    recvfrom_Original;
	tWSASendTo   WSASendTo_Original;
	tWSARecvFrom WSARecvFrom_Original;
	tsocket      socket_Original;
	tbind        bind_Original;
	tclosesocket closesocket_Original;
	tconnect     connect_Original;
	tlisten      listen_Original;
	taccept      accept_Original;

/*************************************************************/
/*			IPC Buffer										 */
/*************************************************************/

	PEStudioIPC DataBuff;

/*************************************************************/
/*			Socket Helper Functions							 */
/*************************************************************/

	//local port
	int GetLocalPortBySocket(SOCKET s)
	{
		struct sockaddr name;
		int namelen =sizeof(name);

		getsockname(s, &name, &namelen);
		return ntohs(((struct sockaddr_in *)&name)->sin_port);
	}

	//local ip
	int GetLocalIPBySocket(SOCKET s, char *ip)
	{
		struct sockaddr name;
		int namelen =sizeof(name);

		getsockname(s, &name, &namelen);
		strcpy_s(ip, 255, inet_ntoa(((struct sockaddr_in *)&name)->sin_addr));
		return 0;
	}

	//remote port
	int GetRemotePortBySocket(SOCKET s)
	{
		struct sockaddr name;
		int namelen =sizeof(name);

		getpeername(s, &name, &namelen);
		return ntohs(((struct sockaddr_in *)&name)->sin_port);
	}

	//sockaddr -> ip : port
	int GetIPAndPortByAddr(struct sockaddr *paddr, char *ip, unsigned short *port)
	{
		*ip =0;
		*port =0;
		if(paddr ==NULL) return -1;

		strcpy_s(ip, 255, inet_ntoa(((struct sockaddr_in *)paddr)->sin_addr));
		*port =ntohs(((struct sockaddr_in *)paddr)->sin_port);

		return 0;
	}

	//remote ip
	int GetRemoteIPBySocket(SOCKET s, char *ip)
	{
		struct sockaddr name;
		int namelen =sizeof(name);

		getpeername(s, &name, &namelen);
		strcpy_s(ip, 255, inet_ntoa(((struct sockaddr_in *)&name)->sin_addr));
		return 0;
	}

/*************************************************************/
/*			Hook APIs										 */
/*************************************************************/

	//TODO: improve synchronization (Critical Sections?)

	//install hooks
	void InstallHooks()
	{
		send_Original = HOOK_INSTALL(tsend, "send", send_Hook);
		recv_Original = HOOK_INSTALL(trecv, "recv", recv_Hook);
		WSASend_Original = HOOK_INSTALL(tWSASend, "WSASend", WSASend_Hook);
		WSARecv_Original = HOOK_INSTALL(tWSARecv, "WSARecv", WSARecv_Hook);
		sendto_Original = HOOK_INSTALL(tsendto, "sendto", sendto_Hook);
		recvfrom_Original = HOOK_INSTALL(trecvfrom, "recvfrom", recvfrom_Hook);
		WSASendTo_Original = HOOK_INSTALL(tWSASendTo, "WSASendTo", WSASendTo_Hook);
		WSARecvFrom_Original = HOOK_INSTALL(tWSARecvFrom, "WSARecvFrom", WSARecvFrom_Hook);
		return;
	}

	//remove hooks
	void RemoveHooks()
	{
		int i;

		HOOK_UNINSTALL(send_Original, send_Hook);
		HOOK_UNINSTALL(recv_Original, recv_Hook);
		HOOK_UNINSTALL(WSARecv_Original, WSARecv_Hook);
		HOOK_UNINSTALL(WSASend_Original, WSASend_Hook);
		HOOK_UNINSTALL(sendto_Original, sendto_Hook);
		HOOK_UNINSTALL(recvfrom_Original, recvfrom_Hook);
		HOOK_UNINSTALL(WSASendTo_Original, WSASendTo_Hook);
		HOOK_UNINSTALL(WSARecvFrom_Original, WSARecvFrom_Hook);
		return;
	}

/*************************************************************/
/*			IPC APIs										 */
/*************************************************************/

	#define IPC_OUT FALSE
	#define IPC_IN  TRUE

	//init the DataBuff for sending
	void SocketIPCInit(SOCKET s, BOOL inout, sockaddr *to=NULL, sockaddr *from=NULL)
	{
		memset(&DataBuff, 0, sizeof(DataBuff));

		if(!to)
		{
			if(inout)
				GetLocalIPBySocket(s, DataBuff.fcall.ip_to);
			else
				GetRemoteIPBySocket(s, DataBuff.fcall.ip_to);

			DataBuff.fcall.port_to = (inout) ? GetLocalPortBySocket(s) : GetRemotePortBySocket(s);
		}
		else
			GetIPAndPortByAddr(to, DataBuff.fcall.ip_to, &DataBuff.fcall.port_to);

		if(!from)
		{
			if(inout)
				GetRemoteIPBySocket(s, DataBuff.fcall.ip_from);
			else
				GetLocalIPBySocket(s, DataBuff.fcall.ip_from);

			DataBuff.fcall.port_from = (inout) ? GetRemotePortBySocket(s) : GetLocalPortBySocket(s);
		}
		else
			GetIPAndPortByAddr(from, DataBuff.fcall.ip_from, &DataBuff.fcall.port_from);
	}

	//send the message
	void SocketIPCUpdate(DWORD Message, DWORD len, const char *buf)
	{
		if(CaptureEngine::Capture)
		{
			DataBuff.dwError = 0;
			DataBuff.dwMessage = Message;
			DataBuff.dwSize = len;
			if((len > 0) && (len <= (DWORD)buffersize))
				MemoryClient_Write(&DataBuff, (LPBYTE)buf);
		}
	}

/*************************************************************/
/*			Hook Functions									 */
/*************************************************************/

	//send()
	int WINAPI send_Hook(SOCKET s, const char* buf, int len, int flags)
	{
		HOOK_INIT();
		MemBlock Packet;

		SocketIPCInit(s, IPC_OUT);
		
		PHPEngine::ScriptList.ApplyScript((char *)buf, len, Packet, WINSOCK_API_SEND);

		HOOK_RET = send_Original(s, (const char *)Packet.get_ptr(), (int)Packet.get_size(), flags);
		SocketIPCUpdate(IPC_FUNC_SEND, ret, (const char *)Packet.get_ptr());

		HOOK_END();
	}

	//recv()
	int WINAPI recv_Hook(SOCKET s, char* buf, int len, int flags)
	{
		HOOK_INIT();
		MemBlock Packet;

		SocketIPCInit(s, IPC_IN);
		HOOK_RET = recv_Original(s, buf, len, flags);

		PHPEngine::ScriptList.ApplyScript((char *)buf, len, Packet, WINSOCK_API_RECV);
		ret = (int)Packet.get_size();
		memcpy(buf, Packet.get_ptr(), ret);

		SocketIPCUpdate(IPC_FUNC_RECV, ret, (const char *)buf);

		HOOK_END();
	}

	//WSARecv()
	int WINAPI WSARecv_Hook(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
	{
		HOOK_INIT();
		MemBlock Packet;

		SocketIPCInit(s, IPC_IN);
		HOOK_RET = WSARecv_Original(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, lpCompletionRoutine);

		if(!ret)
		{
			PHPEngine::ScriptList.ApplyScript((char *)lpBuffers->buf, *lpNumberOfBytesRecvd, Packet, WINSOCK_API_WSARECV);
			*lpNumberOfBytesRecvd = (DWORD)Packet.get_size();
			memcpy(lpBuffers->buf, Packet.get_ptr(), *lpNumberOfBytesRecvd);
			SocketIPCUpdate(IPC_FUNC_WSARECV, *lpNumberOfBytesRecvd, (const char *)lpBuffers->buf);
		}

		HOOK_END();
	}

	//TODO: Add php support to WSASend

	//WSASend()
	int WINAPI WSASend_Hook(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
	{
		HOOK_INIT();
		MemBlock Packet;

		SocketIPCInit(s, IPC_OUT);

		PHPEngine::ScriptList.ApplyScript((char *)lpBuffers->buf, *lpNumberOfBytesSent, Packet, WINSOCK_API_WSASEND);
		HOOK_RET = WSASend_Original(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpOverlapped, lpCompletionRoutine);

		if(!ret)
			SocketIPCUpdate(IPC_FUNC_WSASEND, *lpNumberOfBytesSent, (const char *)lpBuffers->buf);

		HOOK_END();
	}

	//sendto()
	int WINAPI sendto_Hook(SOCKET s, const char* buf, int len, int flags, const struct sockaddr* to, int tolen)
	{
		HOOK_INIT();
		MemBlock Packet;

		SocketIPCInit(s, IPC_OUT, (sockaddr *)to);

		PHPEngine::ScriptList.ApplyScript((char *)buf, len, Packet, WINSOCK_API_SENDTO);

		HOOK_RET = sendto_Original(s, (const char *)Packet.get_ptr(), (int)Packet.get_size(), flags, to, tolen);
		SocketIPCUpdate(IPC_FUNC_SENDTO, ret, (const char *)Packet.get_ptr());

		HOOK_END();
	}

	//recvfrom()
	int WINAPI recvfrom_Hook(SOCKET s, char* buf, int len, int flags, struct sockaddr* from, int* fromlen)
	{
		HOOK_INIT();
		MemBlock Packet;

		SocketIPCInit(s, IPC_IN, NULL, from);
		HOOK_RET = recvfrom_Original(s, buf, len, flags, from, fromlen);

		PHPEngine::ScriptList.ApplyScript((char *)buf, len, Packet, WINSOCK_API_RECVFROM);
		ret = (int)Packet.get_size();
		memcpy(buf, Packet.get_ptr(), ret);

		SocketIPCUpdate(IPC_FUNC_RECVFROM, ret, buf);

		HOOK_END();
	}

	//TODO: Add php support to WSASendTo

	//WSASendTo()
	int WINAPI WSASendTo_Hook (SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, const struct sockaddr *lpTo, int iToLen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
	{
		HOOK_INIT();
		static int iret;

		SocketIPCInit(s, IPC_OUT, (sockaddr*)lpTo);
		HOOK_RET = WSASendTo_Original(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpTo, iToLen, lpOverlapped, lpCompletionRoutine);

		iret = ( !ret ) ? *lpNumberOfBytesSent : SOCKET_ERROR;
		SocketIPCUpdate(IPC_FUNC_WSASENDTO, iret, lpBuffers->buf);

		HOOK_END();
	}

	//WSARecvFrom()
	int WINAPI WSARecvFrom_Hook (SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, struct sockaddr *lpFrom, LPINT lpFromlen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
	{
		HOOK_INIT();
		MemBlock Packet;

		SocketIPCInit(s, IPC_OUT, NULL, lpFrom);
		HOOK_RET = WSARecvFrom_Original(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpFrom, lpFromlen, lpOverlapped, lpCompletionRoutine);

		if(!ret)
		{
			PHPEngine::ScriptList.ApplyScript((char *)lpBuffers->buf, *lpNumberOfBytesRecvd, Packet, WINSOCK_API_WSARECVFROM);
			*lpNumberOfBytesRecvd = (DWORD)Packet.get_size();
			memcpy(lpBuffers->buf, Packet.get_ptr(), *lpNumberOfBytesRecvd);
			SocketIPCUpdate(IPC_FUNC_WSARECVFROM, *lpNumberOfBytesRecvd, lpBuffers->buf);
		}

		HOOK_END();
	}

	/* TODO: Add logging for these APIs

	SOCKET WINAPI socket_Hook(int af, int type, int protocol)
	{
		HOOK_INIT();

		HOOK_RET = socket_Original(af, type, protocol);

		HOOK_END();
	}

	int WINAPI bind_Hook(SOCKET s, const struct sockaddr *name, int namelen)
	{
		HOOK_INIT();

		HOOK_RET = bind_Original(s, name, namelen);

		HOOK_END();
	}
	int WINAPI closesocket_Hook(SOCKET s)
	{
		HOOK_INIT();

		HOOK_RET = closesocket_Original(s);

		HOOK_END();
	}

	int WINAPI connect_Hook(SOCKET s, const struct sockaddr *name, int namelen)
	{
		HOOK_INIT();

		HOOK_RET = connect_Original(s, name, namelen);

		HOOK_END();
	}

	int WINAPI listen_Hook(SOCKET s, int backlog)
	{
		HOOK_INIT();

		HOOK_RET = listen_Original(s, backlog);

		HOOK_END();
	}

	SOCKET WINAPI accept_Hook(SOCKET s, struct sockaddr *addr, int *addrlen)
	{
		HOOK_INIT();

		HOOK_RET = accept_Original(s, addr, addrlen);

		HOOK_END();
	}*/