/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  hooks.h

  This file is part of the Packet Edit Studio Project.
  version: ALPHA

  *Distributed under the terms of the zlib/libpng public license.*
  
  Author: 

	Chris Smith (sourcemagik.com)
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
/*			Types											 */
/*************************************************************/

	typedef int    (WINAPI *tsend)        (SOCKET s, const char* buf, int len, int flags);
	typedef int    (WINAPI *trecv)        (SOCKET s, char* buf, int len, int flags);
	typedef int    (WINAPI *tWSARecv)     (SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
	typedef int    (WINAPI *tWSASend)     (SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
	typedef int    (WINAPI *tsendto)      (SOCKET s, const char* buf, int len, int flags, const struct sockaddr* to, int tolen);
	typedef int    (WINAPI *trecvfrom)    (SOCKET s, char* buf, int len, int flags, struct sockaddr* from, int* fromlen);
	typedef int    (WINAPI *tWSASendTo)   (SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, const struct sockaddr *lpTo, int iToLen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
	typedef int    (WINAPI *tWSARecvFrom) (SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, struct sockaddr *lpFrom, LPINT lpFromlen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
	typedef SOCKET (WINAPI *tsocket)      (int af, int type, int protocol);
	typedef int    (WINAPI *tbind)        (SOCKET s, const struct sockaddr *name, int namelen);
	typedef int    (WINAPI *tclosesocket) (SOCKET s);
	typedef int    (WINAPI *tconnect)     (SOCKET s, const struct sockaddr *name, int namelen);
	typedef int    (WINAPI *tlisten)      (SOCKET s, int backlog);
	typedef SOCKET (WINAPI *taccept)      (SOCKET s, struct sockaddr *addr, int *addrlen);

/*************************************************************/
/*			Original Functions								 */
/*************************************************************/

	extern tsend        send_Original;
	extern trecv        recv_Original;
	extern tWSARecv     WSARecv_Original;
	extern tWSASend     WSASend_Original;
	extern tsendto      sendto_Original;
	extern trecvfrom    recvfrom_Original;
	extern tWSASendTo   WSASendTo_Original;
	extern tWSARecvFrom WSARecvFrom_Original;
	extern tsocket      socket_Original;
	extern tbind        bind_Original;
	extern tclosesocket closesocket_Original;
	extern tconnect		connect_Original;
	extern tlisten      listen_Original;
	extern taccept      accept_Original;

/*************************************************************/
/*			Hook Functions									 */
/*************************************************************/

	int WINAPI send_Hook(SOCKET s, const char* buf, int len, int flags);
	int WINAPI recv_Hook(SOCKET s, char* buf, int len, int flags);
	int WINAPI WSARecv_Hook(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
	int WINAPI WSASend_Hook(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
	int WINAPI sendto_Hook(SOCKET s, const char* buf, int len, int flags, const struct sockaddr* to, int tolen);
	int WINAPI recvfrom_Hook(SOCKET s, char* buf, int len, int flags, struct sockaddr* from, int* fromlen);
	int WINAPI WSASendTo_Hook (SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, const struct sockaddr *lpTo, int iToLen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
	int WINAPI WSARecvFrom_Hook (SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, struct sockaddr *lpFrom, LPINT lpFromlen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
	SOCKET WINAPI socket_Hook(int af, int type, int protocol);
	int WINAPI bind_Hook(SOCKET s, const struct sockaddr *name, int namelen);
	int WINAPI closesocket_Hook(SOCKET s);
	int WINAPI connect_Hook(SOCKET s, const struct sockaddr *name, int namelen);
	int WINAPI listen_Hook(SOCKET s, int backlog);
	SOCKET WINAPI accept_Hook(SOCKET s, struct sockaddr *addr, int *addrlen);

/*************************************************************/
/*			Hook APIs										 */
/*************************************************************/

	void InstallHooks();
	void RemoveHooks();
