#ifndef DUCKY_THREAD_CriticalSection_H_
#define DUCKY_THREAD_CriticalSection_H_

#include <Windows.h>
#include <string>

using std::string;

namespace ducky
{
	namespace thread {
		class CriticalSection
		{
		public:
			CriticalSection();
			virtual ~CriticalSection();

			void lock();
			void unlock();
			void enable(bool enable);

		private:
			CRITICAL_SECTION cs;
			bool enabled;
			CriticalSection(CriticalSection&);
			CriticalSection& operator=(CriticalSection&);
		};

		class CSLocker
		{
		public:
			CSLocker(CriticalSection& cs);
			~CSLocker();

		private:
			CriticalSection& wcs;
			CSLocker(CSLocker&);
			CSLocker& operator=(CSLocker&);
		};
	}
}
#endif