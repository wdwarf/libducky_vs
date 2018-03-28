/*
 * Semaphore.h
 *
 *  Created on: 2016-10-26
 *      Author: ducky
 */

#ifndef DUCKY_THREAD_SEMAPHORE_H_
#define DUCKY_THREAD_SEMAPHORE_H_

#include <string>
#include "libducky/Object.h"

#ifdef _WINNT_
#include <Windows.h>
#else
#include <semaphore.h>
#endif

namespace ducky {
namespace thread {

using std::_W(string);

class Semaphore: virtual public Object {
public:
	Semaphore();
#ifdef _WINNT_
	Semaphore(const _W(string)& name);
#else
	Semaphore(const string& name, int flag, ...);
#endif

	~Semaphore();

	int wait(int mSec = -1);
	int release();
	int getValue();

#ifndef _WINNT_
	void unlink();
#endif

private:
#ifdef _WINNT_
	HANDLE sem;
#else
	sem_t* sem;
#endif
	_W(string) name;
	bool named;
};

} /* namespace thread */
} /* namespace ducky */
#endif /* SEMAPHORE_H_ */
