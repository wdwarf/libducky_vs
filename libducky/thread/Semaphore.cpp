/*
 * Semaphore.cpp
 *
 *  Created on: 2016-10-26
 *      Author: liyawu
 */

#include "stdafx.h"
#include <libducky/thread/Semaphore.h>
#include <stdarg.h>
#include <iostream>
using namespace std;

namespace ducky {
namespace thread {

#ifdef _WINNT_
	Semaphore::Semaphore() :
		sem(NULL), named(false) {
		sem = CreateSemaphore(NULL, 0, 999999, NULL);
	}
#else
Semaphore::Semaphore() :
		sem(new sem_t), named(false) {
	sem_init(this->sem, 0, 0);
}
#endif

#ifdef _WINNT_
Semaphore::Semaphore(const _W(string)& name)
{
	sem = CreateSemaphore(NULL, 0, 999999, name.c_str());
}
#else
Semaphore::Semaphore(const string& name, int flag, ...) :
		named(true) {
	va_list ap;
	va_start(ap, flag);
	this->sem = sem_open(name.c_str(), flag, ap);
	va_end(ap);
	this->name = name;
}
#endif

Semaphore::~Semaphore() {
#ifdef _WINNT_
	if (NULL != this->sem)
	{
		CloseHandle(this->sem);
	}
#else
	if (this->named) {
		sem_close(this->sem);
	} else {
		sem_destroy(this->sem);
		delete this->sem;
	}
#endif
}

int Semaphore::wait(int mSec) {
#ifdef _WINNT_
	return WaitForSingleObject(this->sem, mSec);
#else
	if (mSec < 0) {
		return sem_wait(this->sem);
	} else {
		timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += mSec / 1000;
		ts.tv_nsec += (mSec % 1000) * 1000000;
		if (ts.tv_nsec >= 1000000000) {
			ts.tv_sec += 1;
			ts.tv_nsec -= 1000000000;
		}
		return sem_timedwait(this->sem, &ts);
	}
#endif
}

int Semaphore::release() {
#ifdef _WINNT_
	return ReleaseSemaphore(this->sem, 1, NULL);
#else
	return sem_post(this->sem);
#endif
}

int Semaphore::getValue() {
#ifdef _WINNT_
	LONG previousCount = 0;
	return ReleaseSemaphore(this->sem, 0, &previousCount);
	return previousCount;
#else
	int re = 0;
	sem_getvalue(this->sem, &re);
	return re;
#endif
}

#ifndef _WINNT_
void Semaphore::unlink() {
	if (this->named) {
		sem_unlink(this->name.c_str());
	}
}
#endif

} /* namespace thread */
} /* namespace comba */
