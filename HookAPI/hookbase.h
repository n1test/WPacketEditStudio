
	#pragma once

	/*************************************************************/
	/*			CHookFunctor									 */
	/*															 */
	/*	A class template for building hook ipc callback functors */
	/*************************************************************/
	class CHookFunctor
	{
		public:
			CHookFunctor() {}
			virtual ~CHookFunctor() {}

			void operator() (HOOK_HANDLE hHook, DWORD dwSendPid, LPVOID lpData, INT iSz)
			{
				m_Hook = hHook;
				m_SendPid = dwSendPid;
				m_SendData = (LPBYTE)lpData;
				m_SendSize = iSz;

				InitFunctor();
				CallbackRaised();
			}

		protected:
			LPBYTE m_SendData;
			INT m_SendSize;
			DWORD m_SendPid;
			HOOK_HANDLE m_Hook;

			CSharedMemory <void *> *m_MemObject;

			virtual void InitFunctor()
			{
			}
			
			virtual void CallbackRaised()
			{
			}
	};