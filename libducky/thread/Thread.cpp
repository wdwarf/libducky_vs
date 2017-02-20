#include "stdafx.h"
#include "libducky/thread/Thread.h"
#include <Windows.h>
#include <iostream>
using namespace std;

namespace ducky {
	namespace thread {


		//-----------------------------------------------------

		Thread::Thread() : handle(NULL), threadId(0),
			stackSize(0), threadState(TS_STOPPED),
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
			if (!pThread) return -1;

			pThread->threadState = TS_RUNNING;
			try {
				pThread->run();
			}
			catch (std::exception& e){
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

			pThread->threadState = TS_STOPPED;
			pThread->threadId = 0;
			if (pThread->freeOnTerminated) {
				pThread->deleteThis();
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
			if (TS_STOPPED != this->threadState) {
				THROW_EXCEPTION(ThreadException, "thread is running.", 0);
			}

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

		ThreadState Thread::getState() const {
			return this->threadState;
		}

		//-----------------------------------------------------

		DWORD Thread::suspend()
		{
			if (TS_STOPPED != this->threadState) {
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
			return ((TS_RUNNING == this->threadState)
				|| (TS_STOP_REQUIRING == this->threadState));
		}

		//-----------------------------------------------------

		bool Thread::canStop() {
			return (TS_STOP_REQUIRING == this->threadState);
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

			this->threadState = TS_STOP_REQUIRING;
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