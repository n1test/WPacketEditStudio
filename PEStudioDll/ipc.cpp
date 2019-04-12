/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  ipc.cpp

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

// ipc handler

#include "stdafx.h"

DLLIPCFunctor::DLLIPCFunctor()
{
}

DLLIPCFunctor::~DLLIPCFunctor()
{
}

void DLLIPCFunctor::InitFunctor()
{
	CHookFunctor::InitFunctor();

	m_IPCData = (UINT*)m_SendData;
}

void DLLIPCFunctor::CallbackRaised()
{
	switch(*m_IPCData)
	{
		case IPC_COMMAND_START:
			CaptureEngine::Capture = true;
			break;
		case IPC_COMMAND_PAUSE:
			CaptureEngine::Capture = false;
			break;
		case IPC_COMMAND_PHP_START:
			CaptureEngine::LoadPHP = true;
			PHPEngine::ScriptList.ReloadLoadList();
			break;
		case IPC_COMMAND_PHP_END:
			CaptureEngine::LoadPHP = false;
			PHPEngine::ScriptList.Empty();
			break;
		default:
			break;
	}
}

void MemoryClient_Write(PEStudioIPC *Data, LPBYTE PacketData)
{
	memcpy(sendbuffer, Data, sizeof(PEStudioIPC));
	memcpy(&sendbuffer[sizeof(PEStudioIPC)], PacketData, Data->dwSize);
	HookInterface.SendData(sendbuffer, sizeof(PEStudioIPC) + Data->dwSize);
}