#pragma once

#include "libducky_config.h"
#include <libducky/Object.h>

namespace ducky
{
	namespace cpio
	{

		class CompletionPort : virtual public Object
		{
		public:
			CompletionPort(DWORD concurrentThreads = 0);
			virtual ~CompletionPort();

			virtual void Close();
			virtual bool Create(DWORD NumberOfConcurrentThreads);
			virtual bool Connect(HANDLE FileHandle, ULONG_PTR CompletionKey);
			operator HANDLE() const;
			operator bool() const;
			virtual bool GetQueuedCompletionStatus(
				LPDWORD lpNumberOfBytesTransferred,
				PULONG_PTR lpCompletionKey,
				LPOVERLAPPED *lpOverlapped,
				DWORD dwMilliseconds);

		protected:
			HANDLE hCp;
		};

	}
}