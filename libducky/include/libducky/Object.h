#pragma once

#ifndef DUCKY_OBJECT_H_
#define DUCKY_OBJECT_H_

#include <libducky_config.h>
#include <string>

namespace ducky {

	class Object {
	public:
		virtual std::string getClassName() const;

#if __cplusplus < 201103L
		static void* operator new(std::size_t size) throw (std::bad_alloc);
#else
		static void* operator new(std::size_t size);
#endif
		static void* operator new(std::size_t size, const std::nothrow_t&) throw ();
		static void *operator new(std::size_t size, void *ptr) throw ();
		void operator delete(void* ptr);

#ifdef __OBJ_DELETE_THIS__
		virtual bool isOnHeap() const;
		void deleteThis();
#endif

		static unsigned long CppVeraion();

	protected:
		Object();
		virtual ~Object();
	};

} /* namespace ducky */

#endif /* DUCKY_OBJECT_H_ */

