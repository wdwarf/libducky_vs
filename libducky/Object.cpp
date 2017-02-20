#include "stdafx.h"
#include "libducky/Object.h"
#include <typeinfo>
#include <set>

using namespace std;

namespace ducky {

	static set<const void*> __heabObjs__;
	static CRITICAL_SECTION __obj_onheap_cs__;
	static bool __obj_onheap_mutex_inited__ = false;

	Object::Object() {
		// TODO Auto-generated constructor stub
		if (!__obj_onheap_mutex_inited__) {
			__obj_onheap_mutex_inited__ = true;
			::InitializeCriticalSection(&__obj_onheap_cs__);
		}
	}

	Object::~Object() {
		// TODO Auto-generated destructor stub
	}

	string Object::getClassName() const {
		string className = typeid(*this).name();
		if (className.find("class ") != string::npos) {
			className = className.substr(6, className.length() - 6);
		}
		return className;
	}

	bool Object::isOnHeap() const {
		bool isOnHeap = false;
		EnterCriticalSection(&__obj_onheap_cs__);
		try {
			const void* rawThisAddr = dynamic_cast<const void*>(this);
			if (__heabObjs__.find(rawThisAddr) != __heabObjs__.end()) {
				isOnHeap = true;
			}
		}
		catch (...) {
		}
		LeaveCriticalSection(&__obj_onheap_cs__);
		return isOnHeap;
	}

	void* Object::operator new(std::size_t size) throw (std::bad_alloc){
		if (!__obj_onheap_mutex_inited__) {
			__obj_onheap_mutex_inited__ = true;
			::InitializeCriticalSection(&__obj_onheap_cs__);
		}

	void* p = ::operator new(size);
	EnterCriticalSection(&__obj_onheap_cs__);
	try {
		__heabObjs__.insert(p);
	}
	catch (...) {
	}
	LeaveCriticalSection(&__obj_onheap_cs__);
	return p;
	}

		void* Object::operator new(size_t size, const std::nothrow_t&) throw (){
		if (!__obj_onheap_mutex_inited__) {
			__obj_onheap_mutex_inited__ = true;
			::InitializeCriticalSection(&__obj_onheap_cs__);
		}

	void* p = ::operator new(size);
	EnterCriticalSection(&__obj_onheap_cs__);
	try {
		__heabObjs__.insert(p);
	}
	catch (...) {
	}
	LeaveCriticalSection(&__obj_onheap_cs__);
	return p;
	}

		void* Object::operator new(std::size_t size, void *ptr) throw (){
		if (!__obj_onheap_mutex_inited__) {
			__obj_onheap_mutex_inited__ = true;
			::InitializeCriticalSection(&__obj_onheap_cs__);
		}

	void* p = ::operator new(size);
	EnterCriticalSection(&__obj_onheap_cs__);
	try {
		__heabObjs__.insert(p);
	}
	catch (...) {
	}
	LeaveCriticalSection(&__obj_onheap_cs__);
	return p;
	}

		void Object::operator delete(void* ptr) {
		if (0 == ptr)
			return;

		EnterCriticalSection(&__obj_onheap_cs__);
		if (__heabObjs__.find(ptr) != __heabObjs__.end()) {
			__heabObjs__.erase(ptr);
		}
		else {
			ptr = 0;
		}
		LeaveCriticalSection(&__obj_onheap_cs__);

		if (ptr) {
			::operator delete(ptr);
		}
	}

	void Object::deleteThis() {
		if (!this->isOnHeap()) {
			return;
		}

		delete this;
	}

}