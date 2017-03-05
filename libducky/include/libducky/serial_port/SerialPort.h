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

		class SerialPort
		{
		public:
			SerialPort(bool isAsync = false);
			virtual ~SerialPort();

			enum Parity { None, Odd, Even, Mark, Space };
			enum SerialOperateTypes { OT_READ, OT_WRITE };

			//�г�ϵͳ���еĴ��ں�
			static list<_W(string)> ListComPorts();

			//�����첽���ڣ����ڹر�ʱ�ſ�����
			void SetAsync(bool isAsync = true);

			virtual bool Open();
			virtual bool Close();
			virtual bool IsOpen();
			virtual int Read(char* buf, int len);
			virtual int Write(const char* buf, int len);
			virtual void SetPort(const _W(string)& port);
			virtual const _W(string) GetPort() const;
			virtual bool SetParity(unsigned char  parity);
			virtual unsigned char GetParity() const;
			virtual bool SetBaudRate(unsigned long  baudRate);
			virtual unsigned long GetBaudRate() const;
			virtual bool SetStopBits(unsigned char stopBits);
			virtual unsigned char GetStopBits() const;
			virtual bool SetByteSize(unsigned char byteSize);
			virtual unsigned char GetByteSize() const;
			virtual bool SetReadTimeout(DWORD timeout);
			virtual DWORD GetReadTimeout() const;
			virtual bool SetReadInterval(DWORD interval);
			virtual DWORD GetReadInterval() const;

			operator HANDLE() const;

			//�첽�¼��ص�����,��SetAsync(true)ʱ��Ч
			virtual void OnRead(const char* buf, int len) {}
			virtual void OnWrite(const char* buf, int len) {}
			virtual void OnReadTimeout() {}
			virtual void OnOpen() {}
			virtual void OnClose() {}

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