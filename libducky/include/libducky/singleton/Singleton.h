/*
* Singleton.h
*
*  Created on: Oct 15, 2016
*      Author: ducky
*/

#ifndef DUCKY_SINGLETON_SINGLETON_H_
#define DUCKY_SINGLETON_SINGLETON_H_

#include "libducky/Object.h"

namespace ducky {
	namespace singleton {

		/*
		* ����������ڵ�һ�ε���ʱ��ʼ��
		* */
		template<class T>
		class Singleton : public T, virtual public Object {
		public:
			static T* getInstance() {
				static Singleton<T> instance;
				return &instance;
			}

		private:
			~Singleton() {
			}
			Singleton() {
			}

			Singleton(const Singleton&);
			Singleton& operator=(const Singleton&);

		};

		/*
		* �����������main����ִ��֮ǰ��ʼ��
		* */
		template<class T>
		class Singleton2 : public T, virtual public Object {
		private:
			struct object_creator {
				object_creator() {
					Singleton2<T>::getInstance();
				}
				inline void doNothing() const {}
			};
			static object_creator creator_object;

		public:
			static T* getInstance() {
				static Singleton2<T> instance;
				creator_object.doNothing();
				return &instance;
			}

		private:
			~Singleton2() {
			}
			Singleton2() {
			}

			Singleton2(const Singleton2&);
			Singleton2& operator=(const Singleton2&);

		};

		template<class T>
		typename Singleton2<T>::object_creator Singleton2<T>::creator_object;

	} /* namespace singleton */
} /* namespace ducky */

#endif /* SINGLETON_SINGLETON_H_ */
