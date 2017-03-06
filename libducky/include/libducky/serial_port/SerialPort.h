#pragma once

#include "libducky_config.h"
#include <libducky/Object.h>
#include <string>
#include <libducky/thread/Thread.h>
#include <boost/smart_ptr.hpp>
#include <libducky/cpio/CompletionPort.h>
#include <list>

using std::_W(string);
using std::list;

namespace ducky
{
	namespace serial_port 
	{

		class SerialPort : virtual public Object
		{
		public:
			SerialPort(bool isAsync = false);
			virtual ~SerialPort();

			enum Parity { None, Odd, Even, Mark, Space };
			enum SerialOperateTypes { OT_READ, OT_WRITE };

			//列出系统所有的串口号
			static list<_W(string)> ListComPorts();

			//开启异步串口，串口关闭时才可设置
			void setAsync(bool isAsync = true);

			virtual bool open();
			virtual bool close();
			virtual bool isOpen();
			virtual int read(char* buf, int len);
			virtual int write(const char* buf, int len);
			virtual void setPort(const _W(string)& port);
			virtual const _W(string) getPort() const;
			virtual bool setParity(unsigned char  parity);
			virtual unsigned char getParity() const;
			virtual bool setBaudRate(unsigned long  baudRate);
			virtual unsigned long getBaudRate() const;
			virtual bool setStopBits(unsigned char stopBits);
			virtual unsigned char getStopBits() const;
			virtual bool setByteSize(unsigned char byteSize);
			virtual unsigned char getByteSize() const;
			virtual bool setReadTimeout(DWORD timeout);
			virtual DWORD getReadTimeout() const;
			virtual bool setReadInterval(DWORD interval);
			virtual DWORD getReadInterval() const;

			operator HANDLE() const;

			//异步事件回调函数,当SetAsync(true)时生效
			virtual void onRead(const char* buf, int len) {}
			virtual void onWrite(const char* buf, int len) {}
			virtual void onReadTimeout() {}
			virtual void onOpen() {}
			virtual void onClose() {}

		protected:
			boost::scoped_ptr<ducky::thread::Thread> workThread;

			HANDLE hComPort;
			_W(string) port;
			DWORD readTimeout;
			COMMTIMEOUTS commTimeouts;
			DCB dcb;
			bool isAsync;
		};

	}
}