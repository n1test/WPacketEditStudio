/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  shared_memory.h

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

// A templatized class for shared memory IPC

//disable cast warning for CreateFileMapping
#pragma warning(disable : 4312)

#pragma once

/*************************************************************/
/*			Initial Definitions								 */
/*************************************************************/
	
	// max number of clients to serve
	#define MAX_CLIENTS 255

	// class definition
	template <class CallbackObject> class CSharedMemory;

	// c style callback definition
	typedef void (*SHAREDMEM_CSTYLE_CALLBACK) (void *data, int sz);

	/*************************************************************/
	/*			CSharedMemory									 */
	/*															 */
	/*	A class template for using shared memory between two	 */
	/*  processes in order to communicate.  This class template  */
	/*  supports either C style callback functions or C++ style  */
	/*  functors.												 */
	/*************************************************************/
	template <class CallbackObject> class CSharedMemory
	{
		public:

		/*************************************************************/
		/*			Constructor / Destructor						 */
		/*************************************************************/

			// init
			CSharedMemory(char *name)
			{
				read_callback = NULL;
				write_callback = NULL;

				char tmp[MAX_PATH];
				int i;

				mypid = GetCurrentProcessId();

				this->hMap = NULL;
				this->hFree = NULL;
				this->hThread = NULL;
				this->hDone = NULL;
				this->mem_ptr = NULL;
				this->hExec = NULL;
	
				this->mem_size = 0;
				this->timeout = INFINITE;

				strcpy_s(this->map_name, 260, name);

				GetModuleFileName(NULL, tmp, MAX_PATH);

				for(i = (int)strlen(tmp); i >= 0 ; --i)
					if(tmp[i] == '\\' || tmp[i] == '/')
					{
						strcpy_s(chProcess, MAX_PATH, &tmp[i+1]);
						break;
					}
			}

			// free
			virtual ~CSharedMemory()
			{
				this->StopListening();
				this->Free();
				this->CloseMemory();
			}

		/*************************************************************/
		/*			Server functions								 */
		/*************************************************************/

			// allocate space for the shared memory region
			bool Malloc(int sz)
			{
				this->hMap = CreateFileMapping((HANDLE)0xFFFFFFFF,NULL,PAGE_READWRITE,0,sz+MAX_PATH,this->map_name);

				if(!this->hMap)
					return false;

				this->mem_size = sz+MAX_PATH;

				return this->SetPointer();
			}

			// free the shared memory region
			void Free()
			{
				UnmapViewOfFile(this->mem_ptr);
			}

			// set up a listening thread for reading from the memory region
			bool Listen()
			{
				DWORD tid;
				this->hThread = CreateThread(NULL, 0, this->ListenThread, (LPVOID)this, 0, &tid);

				if(!this->hThread)
					return false;

				return true;
			}

			// stop the listen thread
			void StopListening()
			{
				TerminateThread(this->hThread, 0);
			}

		/*************************************************************/
		/*			Client functions								 */
		/*************************************************************/

			// for running a client
			bool OpenMemory(int sz)
			{
				char map_free_mutex[255];
				strcpy_s(map_free_mutex, 255, this->map_name);
				strcat_s(map_free_mutex, 255, "Free");

    			this->hMap = OpenFileMapping(FILE_MAP_WRITE,FALSE,this->map_name);
	
				if(!this->hMap)
					return false;

    			this->hFree = CreateMutex(NULL,FALSE,map_free_mutex);

				if(!this->hFree)
					return false;

				this->mem_size = sz+MAX_PATH;

				return this->SetPointer();
			}

			// closes all the handles for the memory
			void CloseMemory()
			{
				if(this->hMap)
					CloseHandle(this->hMap);

				if(this->hExec)
					CloseHandle(this->hExec);

				if(this->hDone)
					CloseHandle(this->hDone);

				if(this->hFree)
					CloseHandle(this->hFree);

				if(this->hThread)
					CloseHandle(this->hThread);

				CloseHandle(this->hMap);

				this->hMap = NULL;
				this->hFree = NULL;
				this->hThread = NULL;
				this->hDone = NULL;
				this->mem_ptr = NULL;
				this->hExec = NULL;
			}

		/*************************************************************/
		/*			Public Access Functions							 */
		/*************************************************************/

			// returns a pointer to the shared memory location
			void *GetPointer()
			{
				return this->mem_ptr;
			}

			// returns the allocation size
			int GetSize()
			{
				return this->mem_size;
			}

			// done event handle
			HANDLE GetDoneEventHandle()
			{
				return this->hDone;
			}

			// exec event handle
			HANDLE GetExecEventHandle()
			{
				return this->hExec;
			}

			// thread handle
			HANDLE GetThreadHandle()
			{
				return this->hThread;
			}

			// install read callback
			void SetReadCallback(CallbackObject *callback)
			{
				this->read_callback = callback;
			}

			// install the write callback
			void SetWriteCallback(CallbackObject *callback)
			{
				this->write_callback = callback;
			}

			// set max time before giving up on a write procedure
			void SetWriteTimeout(DWORD milliseconds)
			{
				this->timeout = milliseconds;
			}

		/*************************************************************/
		/*			Memory Access									 */
		/*************************************************************/

			// writes data to the shared memory region
			bool WriteData(void *data, int sz)
			{
 				if (WaitForSingleObject(this->hFree,timeout) == WAIT_OBJECT_0)
				{
					memcpy(mem_ptr, data, sz);
   					SetEvent(this->hExec);
    
   					if (WaitForSingleObject(this->hDone, 250) == WAIT_OBJECT_0)
						WriteCallback(data, this->mem_size);

    				ReleaseMutex(this->hFree);
  				}
  				else
	  				return false;

  				return true;
			}

			// fired when memory is ready to be read
			void ReadCallback(void *data, int sz)
			{
				CallbackObject &mycallback = *read_callback;
				if(data)
					if(read_callback != NULL)
						mycallback(data,  sz);
			}

			// fired after memory has been written to
			void WriteCallback(void *data, int sz)
			{
				if(write_callback != NULL)
					*write_callback(data, sz);
			}

		private:

		/*************************************************************/
		/*			Memory Initialization							 */
		/*************************************************************/

			// set the pointer to the shared memory location
			bool SetPointer()
			{
				char exec_event[255];
				char done_event[255];

				strcpy_s(exec_event, 255, this->map_name);
				strcat_s(exec_event, 255, "Exec");
				strcpy_s(done_event, 255, this->map_name);
				strcat_s(done_event, 255, "Done");

				this->mem_ptr = MapViewOfFile(this->hMap,FILE_MAP_WRITE,0,0,this->mem_size);

				if(!this->mem_ptr)
					return false;

				this->hExec = CreateEvent(NULL,FALSE,FALSE,exec_event);
				this->hDone = CreateEvent(NULL,FALSE,FALSE,done_event);

				if(!this->hExec)
					return false;

				if(!this->hDone)
					return false;
	
				return true;
			}

		/*************************************************************/
		/*			Read Thread										 */
		/*************************************************************/

			// wait for an ipc event
			static DWORD WINAPI ListenThread(LPVOID data)
			{
				CSharedMemory *MemObject = (CSharedMemory *)data;
				DWORD ret;

				while(ret = WaitForSingleObject(MemObject->GetExecEventHandle(), INFINITE) >= 0)
				{
					if(ret == TRUE)
					{
						MemObject->ReadCallback(MemObject->GetPointer(), MemObject->GetSize());
						SetEvent(MemObject->GetDoneEventHandle());	
					}
					else if (ret == 0xFFFFFFFF)
						return 0;
				}

				return 0;
			}

		/*************************************************************/
		/*			Local Variables									 */
		/*************************************************************/

			char map_name[MAX_PATH];
			char chProcess[MAX_PATH];
			HANDLE hMap, hFree, hThread, hExec, hDone;
			void *mem_ptr;
			int mem_size;
			CallbackObject *read_callback;
			CallbackObject *write_callback;
			DWORD timeout;
			DWORD mypid;

	};

	
	void CSharedMemory <void*>::ReadCallback(void *data, int sz)
	{
		return;
	}

	void CSharedMemory <void*>::WriteCallback(void *data, int sz)
	{
		return;
	}