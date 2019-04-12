/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  ipc.h

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

// Definitions for IPC data structure / constants

/*************************************************************/
/*			Settings										 */
/*************************************************************/

	//mem size is differant based on whether the dll or exe is being compiled
	//this is used to set up the ipc server
	#ifdef PESTUDIO_EXE
		#define IPC_MEMSIZE sizeof(PEStudioIPC) + GetBufferSize()
	#else
		#define IPC_MEMSIZE sizeof(PEStudioIPC) + pConfigParser->iGetVar("pestudio.ini", "packetsz")
	#endif

	//max number of times to try to connect to an ipc server
	#define IPC_MAXTRIES 3

/*************************************************************/
/*			Structures										 */
/*************************************************************/

	//for passing socket information between the dll and exe
	struct winsockargs
	{
		SOCKET s;
		char ip_to[26];
		char ip_from[256];
		unsigned short port_to;
		unsigned short port_from;
	};

	//for sending IPC data
	struct PEStudioIPC
	{
		DWORD dwError;
		DWORD dwMessage;
		DWORD dwSize;
		winsockargs fcall;
	};

/*************************************************************/
/*			IPC Messages									 */
/*************************************************************/

	//WINSOCK FUNCTIONS
	#define IPC_FUNC_SEND		  1
	#define IPC_FUNC_RECV		  2
	#define IPC_FUNC_SENDTO		  3
	#define IPC_FUNC_RECVFROM	  4
	#define IPC_FUNC_WSASEND	  5
	#define IPC_FUNC_WSARECV	  6
	#define IPC_FUNC_WSASENDTO	  7
	#define IPC_FUNC_WSARECVFROM  8

	//SNIFFING COMMANDS
	#define IPC_COMMAND_START	  9
	#define IPC_COMMAND_PAUSE	  10

	//PHP COMMANDS
	#define IPC_COMMAND_PHP_START 11
	#define IPC_COMMAND_PHP_END   12
