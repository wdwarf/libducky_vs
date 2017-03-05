#include "stdafx.h"
#include <libducky/cpio/CompletionPort.h>

namespace ducky
{
	namespace cpio
	{

		CompletionPort::CompletionPort(DWORD concurrentThreads) : hCp(NULL)
		{
			if (concurrentThreads > 0)
			{
				this->Create(concurrentThreads);
			}
		}


		CompletionPort::~CompletionPort()
		{
			this->Close();
		}

		CompletionPort::operator HANDLE() const
		{
			return this->hCp;
		}

		CompletionPort::operator bool() const
		{
			return (NULL != this->hCp);
		}

		bool CompletionPort::Create(DWORD NumberOfConcurrentThreads)
		{
			this->Close();

			this->hCp = CreateIoCompletionPort(
				INVALID_HANDLE_VALUE, NULL, NULL, NumberOfConcurrentThreads);
			if (NULL == this->hCp)
			{
				return false;
			}
			return true;
		}

		bool CompletionPort::Connect(HANDLE FileHandle, ULONG_PTR CompletionKey)
		{
			HANDLE hCp = CreateIoCompletionPort(
				FileHandle,
				this->hCp,
				CompletionKey,
				0);
			if (hCp != this->hCp)
			{
				return false;
			}
			//this->DoReceive(session);
			return true;
		}

		void CompletionPort::Close()
		{
			if (NULL != this->hCp)
			{
				CloseHandle(this->hCp);
				this->hCp = NULL;
			}
		}

		bool CompletionPort::GetQueuedCompletionStatus(
			__out LPDWORD lpNumberOfBytesTransferred,
			__out PULONG_PTR lpCompletionKey,
			__out LPOVERLAPPED *lpOverlapped,
			__in  DWORD dwMilliseconds
			)
		{
			return  (FALSE != ::GetQueuedCompletionStatus(this->hCp,
				lpNumberOfBytesTransferred,
				lpCompletionKey,
				lpOverlapped,
				dwMilliseconds));
		}
	}
}