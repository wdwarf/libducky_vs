/*
 * Exception.h
 *
 *  Created on: Sep 6, 2016
 *      Author: ducky
 */

#ifndef _DUCKY_EXCEPTION_H_
#define _DUCKY_EXCEPTION_H_

#include <libducky/Object.h>
#include <exception>
#include <string>

namespace ducky {
	namespace exception {

#ifndef _GLIBCXX_USE_NOEXCEPT
#define _GLIBCXX_USE_NOEXCEPT
#endif

		class Exception : public std::exception, virtual public Object {
		public:
			Exception(const std::string& msg, unsigned long errNo = 0,
				unsigned long lineNumber = 0, const std::string& functionName = "",
				const std::string& fileName = "") _GLIBCXX_USE_NOEXCEPT;
			virtual ~Exception() _GLIBCXX_USE_NOEXCEPT;

			virtual const char* what() const _GLIBCXX_USE_NOEXCEPT;
			virtual unsigned long getErrNo() const;
			const std::string& getFileName() const;
			const std::string& getFunctionName() const;
			unsigned long getLineNumber() const;

			void setInnerException(const Exception* pInnerException);
			const Exception* getInnerException() const;
			Exception(const Exception& e);
			Exception& operator=(const Exception& e);

		private:
			unsigned long errNo;
			unsigned long lineNumber;
			std::string functionName;
			std::string fileName;
			std::string msg;
			Exception* pInnerException;
		};

	} /* namespace exception */

#define EXCEPTION_DEF(exceptionName) class exceptionName : public ducky::exception::Exception{\
	public:\
	exceptionName(const std::string& msg, unsigned long errNo = 0, unsigned long lineNumber = 0, \
			const std::string& functionName = "", const std::string& fileName = "") _GLIBCXX_USE_NOEXCEPT \
			: Exception(msg, errNo, lineNumber, functionName, fileName){}\
	virtual ~exceptionName() _GLIBCXX_USE_NOEXCEPT{}\
};

#define EXCEPTION_DEF2(exceptionName, parentClass) class exceptionName : public parentClass{\
	public:\
	exceptionName(const std::string& msg, unsigned long errNo = 0, unsigned long lineNumber = 0, \
			const std::string& functionName = "", const std::string& fileName = "") _GLIBCXX_USE_NOEXCEPT \
			: parentClass(msg, errNo, lineNumber, functionName, fileName){}\
	virtual ~exceptionName() _GLIBCXX_USE_NOEXCEPT{}\
};

#define MK_EXCEPTION(exceptionName, msg, errNo) exceptionName(msg, errNo, __LINE__, __FUNCTION__, __FILE__)
#define THROW_EXCEPTION(exceptionName, msg, errNo) throw MK_EXCEPTION(exceptionName, msg, errNo)

} /* namespace ducky */

#endif /* _DUCKY_EXCEPTION_H_ */
