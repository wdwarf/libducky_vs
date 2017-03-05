#ifndef _LIBDUCKY_THREAD_H_
#define _LIBDUCKY_THREAD_H_

#include "libducky_config.h"
#include <libducky/Object.h>
#include "libducky/exception/Exception.h"
#include <Windows.h>

namespace ducky
{
	namespace thread {

		EXCEPTION_DEF(ThreadException)

		enum ThreadState {
			TS_RUNNING, TS_STOP_REQUIRING, TS_STOPPED
		};

		class Thread : virtual public Object
		{
		public:
			Thread();
			virtual ~Thread(void);

			virtual void start();	//启动线程
			virtual void stop();	//设置停止标志
			virtual void join();	//等待线程结束
			virtual bool isRunning() const;	//线程是否正在执行

			virtual ThreadState getState() const;	//线程的状态
			bool isFreeOnTerminated() const;
			void setFreeOnTerminated(bool freeOnTerminated);
			bool canStop();
			bool operator==(const Thread& t) const;
			DWORD suspend();
			void setStackSize(size_t stackSize);
			size_t getStackSize();

			void* getThreadHandle();
			unsigned long  getThreadId();

			static void Sleep(DWORD ms);	//睡眠函数，单位为毫秒

		private:
			virtual DWORD  run() = 0;
			virtual void onTerminated();
			static DWORD  __stdcall ThreadFunc(Thread* pThread);

		protected:
			HANDLE handle;
			DWORD  threadId;
			size_t stackSize;
			ThreadState threadState;
			bool freeOnTerminated;
		};

	}
}

#endif
