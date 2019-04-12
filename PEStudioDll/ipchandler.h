/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  ipchandler.h

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

// Handles communication with the parent exe

	/*************************************************************/
	/*			DLLIPCFunctor									 */
	/*															 */
	/*	This class handles the messages sent by the main Packet	 */
	/*  Edit Studio exe.										 */
	/*************************************************************/
		class DLLIPCFunctor : public CHookFunctor
		{
			public:

			/////////////////////////////
			//  Constructor / Destructor
			/////////////////////////////

				DLLIPCFunctor();

				virtual ~DLLIPCFunctor();

			/////////////////////////////
			//  Hook Event Overwrides
			/////////////////////////////
			protected:

				//initialization code
				virtual void InitFunctor();

				//data is ready
				virtual void CallbackRaised();

			/////////////////////////////
			//  Private Variables
			/////////////////////////////

				UINT *m_IPCData;						//ipc object pointer to the shared mem location
		};