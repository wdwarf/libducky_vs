/*
 * Mutex.cpp
 *
 *  Created on: Oct 15, 2016
 *      Author: ducky
 */

#include "stdafx.h"
#include <ducky/thread/Mutex.h>

namespace ducky {
namespace thread {

Mutex::Mutex(bool recursive, bool shared) {
	pthread_mutexattr_init(&this->attr);

	if (recursive) {
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	} else {
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);
	}

	if (shared) {
		pthread_mutexattr_setpshared(&this->attr, PTHREAD_PROCESS_SHARED);
	} else {
		pthread_mutexattr_setpshared(&this->attr, PTHREAD_PROCESS_PRIVATE);
	}

	pthread_mutex_init(&this->mutex, &this->attr);
}

Mutex::~Mutex() {
	pthread_mutex_destroy(&this->mutex);
	pthread_mutexattr_destroy(&attr);
}

Mutex::operator pthread_mutex_t*() {
	return &this->mutex;
}

void Mutex::lock() {
	pthread_mutex_lock(&this->mutex);
}

void Mutex::unlock() {
	pthread_mutex_unlock(&this->mutex);
}

bool Mutex::trylock() {
	return pthread_mutex_trylock(&this->mutex);
}

bool Mutex::isRecursive() const {
	int type = 0;
	pthread_mutexattr_gettype(&this->attr, &type);
	return (type == PTHREAD_MUTEX_RECURSIVE);
}

bool Mutex::isShared() const {
	int shared = 0;
	pthread_mutexattr_getpshared(&this->attr, &shared);
	return (shared == PTHREAD_PROCESS_SHARED);
}

MutexCondition::MutexCondition(Mutex* mutex, bool shared) :
		_mutex(mutex) {
	pthread_condattr_init(&this->attr);
	if (shared) {
		pthread_condattr_setpshared(&this->attr, PTHREAD_PROCESS_SHARED);
	} else {
		pthread_condattr_setpshared(&this->attr, PTHREAD_PROCESS_PRIVATE);
	}
	pthread_cond_init(&this->cond, &this->attr);
}

MutexCondition::~MutexCondition() {
	pthread_cond_destroy(&this->cond);
	pthread_condattr_destroy(&this->attr);
}

void MutexCondition::setMutex(Mutex* mutex) {
	if (NULL == mutex) {
		throw MutexException("param \"mutex\" can not be NULL.");
	}

	this->_mutex = mutex;
}

MutexCondition::operator pthread_cond_t*() {
	return &this->cond;
}

bool MutexCondition::isShared() const {
	int shared = 0;
	pthread_condattr_getpshared(&this->attr, &shared);
	return (shared == PTHREAD_PROCESS_SHARED);
}

int MutexCondition::wait(int mSec, Mutex* mutex) {
	if (mutex) {
		this->_mutex = mutex;
	}

	if (NULL == this->_mutex) {
		throw MutexException("param \"mutex\" can not be NULL.");
	}

	if (mSec < 0) {
		return pthread_cond_wait(&this->cond, *this->_mutex);
	} else {
		timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += mSec / 1000;
		ts.tv_nsec += (mSec % 1000) * 1000000;
		if(ts.tv_nsec >= 1000000000){
			ts.tv_sec += 1;
			ts.tv_nsec -= 1000000000;
		}
		return pthread_cond_timedwait(&this->cond, *this->_mutex, &ts);
	}
}

int MutexCondition::lockAndWait(int mSec, Mutex* mutex) {
	if (mutex) {
		this->_mutex = mutex;
	}

	if (NULL == this->_mutex) {
		throw MutexException("param \"mutex\" can not be NULL.");
	}

	MutexLocker lk(*this->_mutex);

	if (mSec < 0) {
		return pthread_cond_wait(&this->cond, *this->_mutex);
	} else {
		timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += mSec / 1000;
		ts.tv_nsec += (mSec % 1000) * 1000000;
		if(ts.tv_nsec >= 1000000000){
			ts.tv_sec += 1;
			ts.tv_nsec -= 1000000000;
		}
		return pthread_cond_timedwait(&this->cond, *this->_mutex, &ts);
	}
}

int MutexCondition::wakeOne() {
	return pthread_cond_signal(&this->cond);
}

int MutexCondition::wakeAll() {
	return pthread_cond_broadcast(&this->cond);
}

MutexLocker::MutexLocker(Mutex& mutex) :
		_mutex(mutex) {
	this->_mutex.lock();
}

MutexLocker::~MutexLocker() {
	this->_mutex.unlock();
}

} /* namespace thread */
} /* namespace ducky */
