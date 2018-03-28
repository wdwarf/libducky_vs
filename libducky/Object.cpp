#include "stdafx.h"
#include "libducky/Object.h"
#include <typeinfo>
#include <sstream>
#include <set>

using namespace std;

namespace ducky {

#ifdef __OBJ_DELETE_THIS__
static bool __obj_onheap_mutex_inited__ = false;
static pthread_mutex_t __obj_onheap_mutex__;
static set<const void*> __heabObjs__;
#endif

Object::Object() {
	// TODO Auto-generated constructor stub
}

Object::~Object() {
	// TODO Auto-generated destructor stub
}
#ifdef _WINNT_0

string Object::getClassName() const {
	string className = typeid(*this).name();
	if (className.find("class ") != string::npos) {
		className = className.substr(6, className.length() - 6);
	}
	return className;
}

#else
std::string Object::getClassName() const {
	std::string className;
	std::string fullName = typeid(*this).name();

	if ('N' == fullName[0]) {
		fullName = fullName.substr(1, fullName.length());
		int len = 0;
		do {
			stringstream str;
			str << fullName;
			str >> len;
			if (len > 0) {
				stringstream sLen;
				sLen << len;
				int pos = sLen.str().length();
				int partLen = pos + len;

				if (className.empty()) {
					className = fullName.substr(pos, len);
				} else {
					className += "::" + fullName.substr(pos, len);
				}
				fullName = fullName.substr(partLen,
						fullName.length() - partLen);
			}
		} while (len > 0);
	} else {
		stringstream str;
		str << fullName;
		int len = 0;
		str >> len;

		str.clear();
		str.str("");
		str << len;

		if (len > 0)
			className = fullName.substr(str.str().length(), len);
	}

	return className;
}
#endif

#ifdef __OBJ_DELETE_THIS__
bool Object::isOnHeap() const {
	bool isOnHeap = false;
	pthread_mutex_lock(&__obj_onheap_mutex__);
	try {
		const void* rawThisAddr = dynamic_cast<const void*>(this);
		if (__heabObjs__.find(rawThisAddr) != __heabObjs__.end()) {
			isOnHeap = true;
		}
	} catch (...) {
	}
	pthread_mutex_unlock(&__obj_onheap_mutex__);
	return isOnHeap;
}
#endif

#if __cplusplus < 201103L
void* Object::operator new(std::size_t size) throw (std::bad_alloc) {
#else
void* Object::operator new(std::size_t size) {
#endif

#ifdef __OBJ_DELETE_THIS__
	if (!__obj_onheap_mutex_inited__) {
		__heabObjs__.clear(); //make sure the __heabObjs__ object is constructed
		__obj_onheap_mutex_inited__ = true;
		pthread_mutex_init(&__obj_onheap_mutex__, NULL);
	}
#endif

	void* p = ::operator new(size);

#ifdef __OBJ_DELETE_THIS__
	pthread_mutex_lock(&__obj_onheap_mutex__);
	try {
		__heabObjs__.insert(p);
	} catch (...) {
	}
	pthread_mutex_unlock(&__obj_onheap_mutex__);
#endif
	return p;
}

void* Object::operator new(size_t size, const std::nothrow_t&) throw () {
	return Object::operator new(size);
}

void* Object::operator new(std::size_t size, void *ptr) throw () {
	return Object::operator new(size);
}

void Object::operator delete(void* ptr) {
	if (0 == ptr)
		return;

#ifdef __OBJ_DELETE_THIS__
	pthread_mutex_lock(&__obj_onheap_mutex__);
	if (__heabObjs__.find(ptr) != __heabObjs__.end()) {
		__heabObjs__.erase(ptr);
	} else {
		ptr = 0;
	}
	pthread_mutex_unlock(&__obj_onheap_mutex__);
#endif

	if (ptr) {
		::operator delete(ptr);
	}
}

#ifdef __OBJ_DELETE_THIS__
void Object::deleteThis() {
	if (!this->isOnHeap()) {
		return;
	}

	delete this;
}
#endif

unsigned long Object::CppVeraion(){
	return __cplusplus;
}

} /* namespace ducky */