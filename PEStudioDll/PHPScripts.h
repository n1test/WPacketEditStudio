/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  ipchandler.h

  This file is part of the Packet Edit Studio Project.
  version: ALPHA

  *Distributed under the terms of the zlib/libpng public license.*
  
  Author: 

	Chris Smith (sourcemagik.com) - PHPScripts
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

// Handles php scripts

/*************************************************************/
/*			PHP4Apps Import Types							 */
/*************************************************************/

	typedef int  (WINAPI * ExecutePHP)(int request_id, char * filename);
	typedef int  (WINAPI * ExecuteCode)(int request_id, char * acode);
	typedef void (WINAPI * RegisterVariable)(int request_id, char * aname, char * avalue);
	typedef int  (WINAPI * InitRequest)(void);
	typedef void (WINAPI * DoneRequest)(int request_id);
	typedef int  (WINAPI * GetResultText)(int request_id, char *buf, int buflen); 
	typedef int  (WINAPI * GetVariableSize)(int request_id, char *aname);
	typedef int  (WINAPI * GetVariable)(int request_id, char *aname, char *buf, int buflen);
	typedef void (WINAPI * SaveToFile)(int request_id, char *filename);
	typedef int  (WINAPI * GetResultBufferSize)(int request_id);

/*************************************************************/
/*			Winsock API Values								 */
/*************************************************************/

	#define WINSOCK_API_SEND		  0x01
	#define WINSOCK_API_RECV		  0x02
	#define WINSOCK_API_SENDTO		  0x03
	#define WINSOCK_API_RECVFROM	  0x04
	#define WINSOCK_API_WSASEND		  0x05
	#define WINSOCK_API_WSARECV		  0x06
	#define WINSOCK_API_WSASENDTO	  0x07
	#define WINSOCK_API_WSARECVFROM   0x08

/*************************************************************/
/*			Structures										 */
/*************************************************************/

	//php script node
	typedef struct _PHPScript
	{
		bool loaded;
		char path[MAX_PATH];
		char log_path[MAX_PATH];
		char name[255];
		int index;
		int log_index;
		struct _PHPScript *next;
	} PHPScript;

	/*************************************************************/
	/*			PHPScripts										 */
	/*															 */
	/*	Manages / executes php scripts.							 */
	/*************************************************************/
	class PHPScripts
	{
		public:

			/////////////////////////////
			//  Constructor / Destructor
			/////////////////////////////
			
				PHPScripts();

				virtual ~PHPScripts();

			/////////////////////////////
			//  Script Management
			/////////////////////////////

				//updates the script list with the xml php list
				void ReloadLoadList();

				//apply scripts to buffer
				void ApplyScript(char *buff, int sz, MemBlock &MemObject, unsigned int iAPI);

				//clear script list
				void Empty();

				//set curent directory
				void SetPath();

		private:

				//creates a new script object
				PHPScript *AllocateItemSpace();

			/////////////////////////////
			//  Private Variables
			/////////////////////////////

				//PHP VARS
				PHPScript *base;								//first element in script list
				char list_path[MAX_PATH];						//path of the list xml
				char base_log_path[MAX_PATH];					//path where to log results
				bool log;										//whether or not to log

				//PHP4APPS APIS
				ExecutePHP lpfnExecutePHP;						//execute a script
				RegisterVariable lpfnRegisterVariable;			//add a variable to an executing script
				ExecuteCode lpfnExecuteCode;					//execute a buffered script
				InitRequest lpfnInitRequest;					//init parsing
				DoneRequest lpfnDoneRequest;					//clean up parsing
				GetResultText lpfnGetResultText;				//get output of script
				GetVariableSize lpfnGetVariableSize;			//get the size of a variable's buffer
				GetVariable lpfnGetVariable;					//get variable value
				SaveToFile lpfnSaveToFile;						//save output to a file
				GetResultBufferSize lpfnGetResultBufferSize;	//get output buffer size
	};