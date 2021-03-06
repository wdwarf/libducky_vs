/*
* Buffer.cpp
*
*  Created on: Oct 14, 2016
*      Author: ducky
*/

#include "stdafx.h"
#include <libducky/buffer/Buffer.h>

#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>

using namespace std;

namespace ducky {
	namespace buffer {

		/* Buffer::BufferImpl */
		class Buffer::BufferImpl {
		public:
			BufferImpl();
			BufferImpl(unsigned int initSize);
			BufferImpl(const char* data, unsigned int size);
			BufferImpl(const BufferImpl& buffer);
			virtual ~BufferImpl();

			BufferImpl& operator=(const BufferImpl& buffer);
			BufferImpl operator+(const BufferImpl& buffer) const;
			char& operator[](unsigned index);
			const char& operator[](unsigned index) const;

			void append(const char* data, unsigned int size);
			void append(const BufferImpl& buffer);

			void setData(const char* data, unsigned int size);
			char* getData() const;
			unsigned int getSize() const;
			void clear();
			bool isEmpty() const;
			void reverse();
			void alloc(int size);

			string toString();
			int read(void* buf, int size) const;
			void resetReadPos() const;

		private:
			char* data;
			unsigned int size;
			mutable unsigned int readPos;
		};

		Buffer::BufferImpl::BufferImpl() :
			data(NULL), size(0), readPos(0) {

		}

		Buffer::BufferImpl::BufferImpl(unsigned int initSize) :
			data(NULL), size(0), readPos(0) {
			this->size = initSize;
			this->data = new char[this->size];
		}

		Buffer::BufferImpl::BufferImpl(const char* data, unsigned int size) :
			data(NULL), size(0), readPos(0) {
			this->setData(data, size);
		}

		Buffer::BufferImpl::BufferImpl(const Buffer::BufferImpl& buffer) :
			data(NULL), size(0), readPos(0) {
			this->setData(buffer.getData(), buffer.getSize());
		}

		int Buffer::BufferImpl::read(void* buf, int size) const {
			int avaliableSize = this->getSize() - this->readPos;
			cout << "avaliableSize: " << avaliableSize << endl;
			if (avaliableSize <= 0)
				return 0;

			int readSzie = avaliableSize > size ? size : avaliableSize;
			memcpy(buf, this->data + this->readPos, readSzie);
			this->readPos += readSzie;
			return readSzie;
		}

		void Buffer::BufferImpl::resetReadPos() const {
			this->readPos = 0;
		}
		Buffer::BufferImpl& Buffer::BufferImpl::operator=(
			const Buffer::BufferImpl& buffer) {
			this->setData(buffer.getData(), buffer.getSize());
			return *this;
		}

		Buffer::BufferImpl Buffer::BufferImpl::operator+(
			const Buffer::BufferImpl& buffer) const {
			Buffer::BufferImpl newBuffer;
			newBuffer.append(*this);
			newBuffer.append(buffer);
			return newBuffer;
		}

		void Buffer::BufferImpl::append(const char* data, unsigned int size) {
			if ((NULL == data) || (size <= 0))
				return;

			char* newData = new char[this->size + size];
			memcpy(newData, this->data, this->size);
			memcpy(newData + this->size, data, size);

			if (NULL != this->data)
				delete[] this->data;
			this->data = newData;
			this->size += size;
		}

		void Buffer::BufferImpl::append(const BufferImpl& buffer) {
			if (buffer.isEmpty())
				return;

			char* newData = new char[this->size + buffer.getSize()];
			memcpy(newData, this->data, this->size);
			memcpy(newData + this->size, buffer.getData(), buffer.getSize());

			if (NULL != this->data)
				delete[] this->data;

			this->data = newData;
			this->size += buffer.getSize();
		}

		void Buffer::ReverseBytes(char* buf, int size) {
			if (!buf || (size <= 0)) {
				return;
			}

			char* pH = buf;
			char* pE = buf + size - 1;
			while (pH < pE) {
				char c = *pH;
				*pH = *pE;
				*pE = c;
				++pH;
				--pE;
			}
		}

		void Buffer::BufferImpl::reverse() {
			Buffer::ReverseBytes(this->data, this->size);

			this->resetReadPos();
		}

		void Buffer::BufferImpl::alloc(int size) {
			this->clear();

			if (size <= 0) {
				return;
			}

			this->size = size;
			this->data = new char[this->size];
			if (!this->data) {
				this->size = 0;
				throw MK_EXCEPTION(BufferException, "Alloc buffer failed", size);
			}
			memset(this->data, 0, this->size);

			this->resetReadPos();
		}

		char& Buffer::BufferImpl::operator[](unsigned index) {
			if (index >= this->size) {
				THROW_EXCEPTION(BufferException, "Index out of bound.", this->size);
			}
			return this->data[index];
		}

		const char& Buffer::BufferImpl::operator[](unsigned index) const {
			if (index >= this->size) {
				THROW_EXCEPTION(BufferException, "Index out of bound.", this->size);
			}
			return this->data[index];
		}

		void Buffer::BufferImpl::setData(const char* data, unsigned int size) {
			char* oldData = this->data;
			if ((NULL != data) && (size > 0)) {
				this->size = size;
				this->data = new char[this->size];
				memcpy(this->data, data, this->size);
			}
			else {
				this->size = 0;
				this->data = NULL;
			}

			this->resetReadPos();

			if (oldData)
				delete[] oldData;
		}

		Buffer::BufferImpl::~BufferImpl() {
			this->clear();
		}

		void Buffer::setData(const char* data, unsigned int size) {
			this->impl->setData(data, size);
		}

		char* Buffer::BufferImpl::getData() const {
			return this->data;
		}

		unsigned int Buffer::BufferImpl::getSize() const {
			return this->size;
		}

		void Buffer::BufferImpl::clear() {
			if (NULL != this->data) {
				delete[] this->data;
				this->data = NULL;
				this->size = 0;

				this->resetReadPos();
			}
		}

		bool Buffer::BufferImpl::isEmpty() const {
			return (NULL == this->data);
		}

		string Buffer::BufferImpl::toString() {
			stringstream strBuf;
			if (!this->isEmpty()) {
				unsigned char* data = (unsigned char*)this->getData();
				unsigned int size = this->getSize();

				strBuf.fill('0');
				for (unsigned int i = 0; i < size; ++i) {
					strBuf.width(2);
					strBuf << hex << (int)data[i] << " ";
				}
			}

			return strBuf.str();
		}

		/* end of Buffer::BufferImpl */

		Buffer::Buffer() :
			impl(new Buffer::BufferImpl()) {

		}

		Buffer::Buffer(unsigned int initSize) :
			impl(new Buffer::BufferImpl(initSize)) {
		}

		Buffer::Buffer(const char* data, unsigned int size) :
			impl(new Buffer::BufferImpl(data, size)) {
		}

		Buffer::Buffer(const Buffer& buffer) :
			impl(new Buffer::BufferImpl(*buffer.impl)) {
		}

		Buffer& Buffer::operator=(const Buffer& buffer) {
			*this->impl = *buffer.impl;
			return *this;
		}

		Buffer& Buffer::operator+=(const Buffer& buffer) {
			this->append(buffer);
			return *this;
		}

		Buffer Buffer::operator+(const Buffer& buffer) const {
			Buffer newBuffer;
			newBuffer.append(*this);
			newBuffer.append(buffer);
			return newBuffer;
		}

		void Buffer::append(const char* data, unsigned int size) {
			this->impl->append(data, size);
		}

		void Buffer::append(const Buffer& buffer) {
			this->impl->append(*buffer.impl);
		}

		char& Buffer::operator[](unsigned index) {
			return this->impl->operator [](index);
		}

		const char& Buffer::operator[](unsigned index) const {
			return this->impl->operator [](index);
		}

		Buffer::~Buffer() {
			if (NULL != this->impl) {
				delete this->impl;
			}
		}

		char* Buffer::getData() const {
			return this->impl->getData();
		}

		unsigned int Buffer::getSize() const {
			return this->impl->getSize();
		}

		void Buffer::clear() {
			this->impl->clear();
		}

		bool Buffer::isEmpty() const {
			return this->impl->isEmpty();
		}

		string Buffer::toString() const {
			return this->impl->toString();
		}

		Buffer& Buffer::reverse() {
			this->impl->reverse();
			return *this;
		}

		void Buffer::alloc(int size) {
			this->impl->alloc(size);
		}

		int Buffer::read(void* buf, int size) const {
			return this->impl->read(buf, size);
		}

		void Buffer::resetReadPos() const {
			this->impl->resetReadPos();
		}

	} /* namespace buffer */
} /* namespace ducky */

ostream& operator<<(ostream& o, const ducky::buffer::Buffer& buffer) {
	o << buffer.toString();
	return o;
}

ducky::buffer::Buffer& operator<<(ducky::buffer::Buffer& buffer,
	const ducky::buffer::Buffer& in_buffer) {
	buffer.append(in_buffer);
	return buffer;
}

ducky::buffer::Buffer& operator<<(ducky::buffer::Buffer& buffer, istream& i) {
	istream::pos_type pos = i.tellg();
	i.seekg(0, ios::end);
	istream::pos_type endPos = i.tellg();
	i.seekg(pos, ios::beg);
	istream::pos_type len = endPos - pos;
	vector<char> v(len);
	i.read(&v[0], len);
	buffer.append(&v[0], len);

	return buffer;
}

BUF_IN_OPERATOR_IMPL(long long);
BUF_IN_OPERATOR_IMPL(long);
BUF_IN_OPERATOR_IMPL(int);
BUF_IN_OPERATOR_IMPL(short);
BUF_IN_OPERATOR_IMPL(char);
BUF_IN_OPERATOR_IMPL(unsigned long long);
BUF_IN_OPERATOR_IMPL(unsigned long);
BUF_IN_OPERATOR_IMPL(unsigned int);
BUF_IN_OPERATOR_IMPL(unsigned short);
BUF_IN_OPERATOR_IMPL(unsigned char);
BUF_IN_OPERATOR_IMPL(float);
BUF_IN_OPERATOR_IMPL(double);

BUF_OUT_OPERATOR_IMPL(long long);
BUF_OUT_OPERATOR_IMPL(long);
BUF_OUT_OPERATOR_IMPL(int);
BUF_OUT_OPERATOR_IMPL(short);
BUF_OUT_OPERATOR_IMPL(char);
BUF_OUT_OPERATOR_IMPL(unsigned long long);
BUF_OUT_OPERATOR_IMPL(unsigned long);
BUF_OUT_OPERATOR_IMPL(unsigned int);
BUF_OUT_OPERATOR_IMPL(unsigned short);
BUF_OUT_OPERATOR_IMPL(unsigned char);
BUF_OUT_OPERATOR_IMPL(float);
BUF_OUT_OPERATOR_IMPL(double);
