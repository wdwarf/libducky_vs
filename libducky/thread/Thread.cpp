#include "stdafx.h"
#include "libducky/thread/Thread.h"
#include <Windows.h>
#include <iostream>
#include <cassert>

using namespace std;

namespace ducky {
	namespace thread {


		//-----------------------------------------------------

		Thread::Thread() : handle(NULL), threadId(0), _canStop(false),
			stackSize(0),
			freeOnTerminated(false)
		{
			//
		}

		//-----------------------------------------------------

		Thread::~Thread(void)
		{
			if (!this->isRunning())
			{
				this->stop();
				this->join();
			}
		}

		//-----------------------------------------------------

		void Thread::setStackSize(size_t stackSize)
		{
			this->stackSize = stackSize;
		}

		//-----------------------------------------------------

		size_t Thread::getStackSize()
		{
			return this->stackSize;
		}

		//-----------------------------------------------------

		DWORD WINAPI Thread::ThreadFunc(Thread* pThread)
		{
			assert(pThread);

			try {
				pThread->run();
			}
			catch (std::exception& e) {
				cout << "[" << __FUNCTION__ << "] " << e.what() << endl;
			}
			catch (...) {
				cout << "[" << __FUNCTION__ << "] Unknown exception." << endl;
			}

			try {
				pThread->onTerminated();
			}
			catch (...) {
			}

			pThread->threadId = 0;
			if (pThread->freeOnTerminated) {
				delete pThread;
			}

			return 0;
		}

		//-----------------------------------------------------

		void Thread::onTerminated()
		{
			//do nothing...
		}

		//-----------------------------------------------------

		void Thread::start()
		{
			if (this->isRunning()) {
				return;
			}

			this->_canStop = false;

			this->handle = CreateThread(
				0,
				this->stackSize,
				(DWORD(__stdcall*)(void*))ThreadFunc,
				this,
				((this->stackSize > 0) ? STACK_SIZE_PARAM_IS_A_RESERVATION : 0),
				&this->threadId);

			if (NULL == this->handle) {
				THROW_EXCEPTION(ThreadException, "thread create failed.", GetLastError());
			}
		}

		//-----------------------------------------------------

		DWORD Thread::suspend()
		{
			if (!this->isRunning()) {
				return 0;
			}
			return ::SuspendThread(this->handle);
		}

		//-----------------------------------------------------

		void Thread::setFreeOnTerminated(bool isFreeOnTerminated)
		{
			this->freeOnTerminated = isFreeOnTerminated;
		}

		//-----------------------------------------------------

		bool Thread::isRunning() const {
			DWORD val = 0;
			GetExitCodeThread(this->handle, &val);
			if (STILL_ACTIVE == val) {
				return true;
			}
			return false;
		}

		//-----------------------------------------------------

		bool Thread::canStop() {
			return this->_canStop;
		}

		//-----------------------------------------------------

		bool Thread::operator==(const Thread& t) const {
			return ((this->handle == t.handle)
				&& (this->threadId == t.threadId));
		}

		//-----------------------------------------------------

		void Thread::stop()
		{
			if (!this->isRunning())
				return;

			this->_canStop = true;
		}

		//-----------------------------------------------------

		HANDLE Thread::getThreadHandle()
		{
			return this->handle;
		}

		//-----------------------------------------------------

		DWORD Thread::getThreadId()
		{
			return this->threadId;
		}

		//-----------------------------------------------------

		void Thread::join()
		{
			if (NULL != this->handle)
			{
				WaitForSingleObject(this->handle, INFINITE);
			}
		}

		//-----------------------------------------------------

		void Thread::Sleep(DWORD ms)
		{
			::Sleep(ms);
		}

		//-----------------------------------------------------

	}
}