

	#pragma once

	typedef unsigned int HOOK_HANDLE;

	#define HOOK_DEBUG(x) MessageBox(NULL, x, "HOOK DEBUG", MB_OK);

	#include "shared_memory.h"
	#include "hookbase.h"
	
	#ifndef HOOK_DLL
		#include "HookLoader/HookManager.h"
	#endif

	#ifndef HOOK_INJECTOR
		#include "DLLComm/DLLInterface.h"
	#endif

