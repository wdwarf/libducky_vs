#include "stdafx.h"
#include "libducky/thread/CriticalSection.h"
#include <sstream>
using namespace std;

namespace ducky {
	namespace thread {
		CriticalSection::CriticalSection() : enabled(true)
		{
			InitializeCriticalSection(&this->cs);
		}

		CriticalSection::~CriticalSection()
		{
			DeleteCriticalSection(&this->cs);
		}

		void CriticalSection::lock()
		{
			if (!this->enabled)
			{
				return;
			}
			EnterCriticalSection(&this->cs);
		}

		void CriticalSection::unlock()
		{
			LeaveCriticalSection(&this->cs);
		}

		void CriticalSection::enable(bool enable)
		{
			this->enabled = enable;
		}

		CSLocker::CSLocker(CriticalSection& cs) : wcs(cs)
		{
			this->wcs.lock();
		}

		CSLocker::~CSLocker()
		{
			this->wcs.unlock();
		}
	}
}