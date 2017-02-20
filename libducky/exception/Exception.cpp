/*
 * Exception.cpp
 *
 *  Created on: Sep 6, 2016
 *      Author: ducky
 */

#include "stdafx.h"
#include "libducky/exception/Exception.h"

using namespace std;

namespace ducky {
	namespace exception {

		static string GetFileName(string filePath) {
			string::size_type pos = filePath.find_last_of('\\');
			if (pos != string::npos) {
				filePath = filePath.substr(pos + 1);
			}
			pos = filePath.find_last_of('/');
			if (pos != string::npos) {
				filePath = filePath.substr(pos + 1);
			}

			return filePath;
		}

		Exception::Exception(const std::string& msg, unsigned long errNo, unsigned long lineNumber,
			const std::string& functionName, const std::string& fileName)
			: pInnerException(NULL) _GLIBCXX_USE_NOEXCEPT {
			// TODO Auto-generated constructor stub
			this->msg = msg;
			this->errNo = errNo;
			this->lineNumber = lineNumber;
			this->functionName = functionName;
			this->fileName = GetFileName(fileName);
		}

		Exception::Exception(const Exception& e) {
			this->msg = e.msg;
			this->errNo = e.errNo;
			this->lineNumber = e.lineNumber;
			this->functionName = e.functionName;
			this->fileName = e.fileName;
			if (e.pInnerException) {
				this->pInnerException = new Exception(e.pInnerException->what(),
					e.pInnerException->getErrNo(), e.pInnerException->getLineNumber(),
					e.pInnerException->getFunctionName(), e.pInnerException->getFileName());
			}
			else {
				this->pInnerException = NULL;
			}
		}

		Exception::~Exception() _GLIBCXX_USE_NOEXCEPT {
			// TODO Auto-generated destructor stub
			if (this->pInnerException) {
				delete this->pInnerException;
			}
		}

		void Exception::setInnerException(const Exception* pInnerException) {
			if (this->pInnerException) {
				delete this->pInnerException;
			}
			this->pInnerException = new Exception(pInnerException->what(),
				pInnerException->getErrNo(), pInnerException->getLineNumber(),
				pInnerException->getFunctionName(), pInnerException->getFileName());
		}

		const Exception* Exception::getInnerException() const {
			return this->pInnerException;
		}

		Exception& Exception::operator=(const Exception& e) {
			this->msg = e.msg;
			this->errNo = e.errNo;
			this->lineNumber = e.lineNumber;
			this->functionName = e.functionName;
			this->fileName = e.fileName;
			if (this->pInnerException) {
				delete this->pInnerException;
				this->pInnerException = NULL;
			}
			if (e.pInnerException) {
				this->pInnerException = new Exception(e.pInnerException->what(),
					e.pInnerException->getErrNo(), e.pInnerException->getLineNumber(),
					e.pInnerException->getFunctionName(), e.pInnerException->getFileName());
			}

			return *this;
		}

		const char* Exception::what() const _GLIBCXX_USE_NOEXCEPT {
			return this->msg.c_str();
		}

		unsigned long Exception::getErrNo() const {
			return this->errNo;
		}

		const std::string& Exception::getFileName() const {
			return fileName;
		}

		const std::string& Exception::getFunctionName() const {
			return functionName;
		}

		unsigned long Exception::getLineNumber() const {
			return lineNumber;
		}

	} /* namespace exception */
} /* namespace ducky */