#include "stdafx.h"
#include <libducky/serial_port/SerialPort.h>
#include <libducky/registry/Registry.h>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <sstream>

using namespace ducky::thread;
using namespace ducky::registry;
using namespace ducky::cpio;
using namespace std;

namespace ducky
{
	namespace serial_port
	{
		struct SerialContext : public OVERLAPPED
		{
			SerialPort::SerialOperateTypes operateType;
			WSABUF buf;
			static const unsigned long DEFAULT_BUF_SIZE = 2048;

			SerialContext()
			{
				memset(this, 0, sizeof(OVERLAPPED));
				this->operateType = SerialPort::OT_READ;
				memset(&this->buf, 0, sizeof(WSABUF));
				this->SetBufferSize(DEFAULT_BUF_SIZE);
			}

			~SerialContext()
			{
				this->SetBufferSize(0);
			}

			void SetBufferSize(unsigned long bufSize)
			{
				if (buf.buf)
				{
					delete[] buf.buf;
					buf.buf = NULL;
					buf.len = 0;
				}

				if (bufSize > 0)
				{
					buf.buf = new char[bufSize];
					buf.len = bufSize;
					memset(buf.buf, 0, bufSize);
				}
			}
		};

		class SerialPortWorkThread : public Thread
		{
		public:
			SerialPortWorkThread(SerialPort* pSerialPort) : cp(1)
			{
				this->pSerialPort = pSerialPort;
				cp.Connect(*this->pSerialPort, (ULONG_PTR)this->pSerialPort);
			}

			bool Start()
			{
				try {
					Thread::start();
				}
				catch (...) {
					return false;
				}
				return true;
			}

			DWORD run()
			{
				if (NULL == this->pSerialPort)
				{
					return 0;
				}
				this->pSerialPort->OnOpen();
				this->pSerialPort->Read(NULL, 0);

				while (!this->canStop())
				{
					DWORD numberOfBytesTransferred = 0;
					ULONG_PTR completionKey;
					SerialContext* context = NULL;
					bool re = cp.GetQueuedCompletionStatus(&numberOfBytesTransferred,
						&completionKey, (LPOVERLAPPED*)&context, -1);

					if (re)
					{
						switch (context->operateType)
						{
						case SerialPort::OT_READ:
						{
							if (numberOfBytesTransferred > 0)
							{
								this->pSerialPort->OnRead(context->buf.buf, numberOfBytesTransferred);
							}
							else
							{
								this->pSerialPort->OnReadTimeout();
							}
							this->pSerialPort->Read(NULL, 0);
						}
						break;
						case SerialPort::OT_WRITE:
						{
							this->pSerialPort->OnWrite(context->buf.buf, numberOfBytesTransferred);
						}
						break;
						}
					}
					else
					{
						break;
					}

					if (NULL != context)
					{
						delete context;
					}
				}

				this->pSerialPort->OnClose();

				return 0;
			}

		private:
			SerialPort* pSerialPort;
			CompletionPort cp;
		};

		//---------------------------------------------------------------------------
		//---------------------------------------------------------------------------

		SerialPort::SerialPort(bool isAsync)
			: hComPort(INVALID_HANDLE_VALUE)
			, readTimeout(-1)
			, isAsync(false)
		{
			this->isAsync = isAsync;

			memset(&this->commTimeouts, 0, sizeof(COMMTIMEOUTS));
			memset(&this->dcb, 0, sizeof(DCB));
			this->dcb.DCBlength = sizeof(DCB);
			this->dcb.fBinary = true;
			this->dcb.fParity = false;
			this->dcb.fDtrControl = false;

			this->commTimeouts.ReadIntervalTimeout = 150;
			this->commTimeouts.ReadTotalTimeoutMultiplier = 0;
			this->commTimeouts.ReadTotalTimeoutConstant = -1;
			this->commTimeouts.ReadTotalTimeoutConstant = this->readTimeout;
			this->commTimeouts.WriteTotalTimeoutConstant = 2000;
			this->commTimeouts.WriteTotalTimeoutMultiplier = 0;

			this->SetPort(_T("COM1"));
			this->SetBaudRate(9600);
			this->SetParity(None);
			this->SetByteSize(8);
			this->SetStopBits(0);
		}

		//---------------------------------------------------------------------------

		SerialPort::~SerialPort()
		{
			this->Close();
		}

		//---------------------------------------------------------------------------

		static bool SortPorts(_W(string) s1, _W(string) s2)
		{
			int p1 = 0;
			int p2 = 0;
			boost::to_lower(s1);
			boost::to_lower(s2);
			if (int(s1.find(_T("com"))) >= 0)
			{
				_W(stringstream) str;
				str << s1.substr(3, s1.length() - 3);
				str >> p1;
			}
			if (int(s2.find(_T("com"))) >= 0)
			{
				_W(stringstream) str;
				str << s2.substr(3, s2.length() - 3);
				str >> p2;
			}
			return p1 < p2;
		}

		list<_W(string)> SerialPort::ListComPorts()
		{
			list<_W(string)> ports;
			Registry reg;
			reg.Open(HKEY_LOCAL_MACHINE,
				_T("HARDWARE\\DEVICEMAP\\SERIALCOMM\\"), KEY_READ);
			list<RegistryValue> valueList = reg.EnumStringValues();
			BOOST_FOREACH(RegistryValue& v, valueList)
			{
				ports.push_back(v.asString());
			}
			ports.sort(SortPorts);
			return ports;
		}

		//---------------------------------------------------------------------------

		void SerialPort::SetAsync(bool isAsync)
		{
			if (!this->IsOpen())
			{
				this->isAsync = isAsync;
			}
		}

		//---------------------------------------------------------------------------

		bool SerialPort::Open()
		{
			this->Close();

			_W(string) strPort = _T("\\\\.\\") + this->port;

			this->hComPort = CreateFile(
				strPort.c_str(),
				GENERIC_READ | GENERIC_WRITE,
				0,
				0,
				OPEN_EXISTING,
				(this->isAsync ? FILE_FLAG_OVERLAPPED : FILE_ATTRIBUTE_NORMAL),
				0
				);

			if (INVALID_HANDLE_VALUE != this->hComPort)
			{
				if (!SetupComm(this->hComPort, 1024, 1024) ||
					!SetCommTimeouts(this->hComPort, &this->commTimeouts) ||
					!SetCommState(this->hComPort, &this->dcb))
				{
					this->Close();
					return false;
				}

				if (this->isAsync)
				{
					this->workThread.reset(new SerialPortWorkThread(this));
					try {
						this->workThread->start();
					}
					catch (...) {
						return false;
					}
					return true;
				}

				return true;
			}
			else
			{
				//
			}

			return false;
		}

		//---------------------------------------------------------------------------

		bool SerialPort::Close()
		{
			if (this->isAsync)
			{
				if (this->workThread.get())
				{
					this->workThread->stop();
					CloseHandle(this->hComPort);
					this->workThread->join();
					this->workThread.reset();
					this->hComPort = INVALID_HANDLE_VALUE;
				}
			}
			else
			{
				if (INVALID_HANDLE_VALUE != this->hComPort)
				{
					CloseHandle(this->hComPort);
					this->hComPort = INVALID_HANDLE_VALUE;
				}
			}
			return false;
		}

		//---------------------------------------------------------------------------

		bool SerialPort::IsOpen()
		{
			return (INVALID_HANDLE_VALUE != this->hComPort);
		}

		//---------------------------------------------------------------------------

		int SerialPort::Read(char* buf, int len)
		{
			if (this->IsOpen())
			{
				DWORD readBytes = 0;

				if (this->isAsync)
				{
					SerialContext* context = new SerialContext();
					context->operateType = OT_READ;
					if (!ReadFile(this->hComPort, context->buf.buf, context->buf.len,
						&readBytes, context))
					{
						if (ERROR_IO_PENDING != GetLastError())
						{
							delete context;
							return -1;
						}
					}
				}
				else
				{
					if (!ReadFile(this->hComPort, buf, len,
						&readBytes, NULL))
					{
						return -1;
					}
				}

				return readBytes;
			}

			return -1;
		}

		//---------------------------------------------------------------------------

		int SerialPort::Write(const char* buf, int len)
		{
			if (this->IsOpen())
			{
				DWORD writeBytes = 0;

				if (this->isAsync)
				{
					SerialContext* context = new SerialContext();
					context->operateType = OT_WRITE;
					context->SetBufferSize(len);
					memcpy(context->buf.buf, buf, len);
					if (!WriteFile(this->hComPort, context->buf.buf, context->buf.len,
						&writeBytes, context))
					{
						if (ERROR_IO_PENDING != GetLastError())
						{
							delete context;
							return -1;
						}
					}
				}
				else
				{
					if (!WriteFile(this->hComPort, buf, len,
						&writeBytes, NULL))
					{
						return -1;
					}
				}

				return writeBytes;
			}

			return -1;
		}

		//---------------------------------------------------------------------------

		void SerialPort::SetPort(const _W(string)& port)
		{
			this->port = port;
			if (this->IsOpen())
			{
				this->Close();
				this->Open();
			}
		}

		//---------------------------------------------------------------------------

		const _W(string) SerialPort::GetPort() const
		{
			return this->port;
		}

		//---------------------------------------------------------------------------

		unsigned char SerialPort::GetParity() const
		{
			return this->dcb.Parity;
		}

		//---------------------------------------------------------------------------

		bool SerialPort::SetParity(unsigned char  parity)
		{
			this->dcb.Parity = parity;
			if (this->IsOpen())
			{
				return SetCommState(this->hComPort, &this->dcb);
			}
			return false;
		}

		//---------------------------------------------------------------------------

		unsigned long SerialPort::GetBaudRate() const
		{
			return this->dcb.BaudRate;
		}

		//---------------------------------------------------------------------------

		bool SerialPort::SetBaudRate(unsigned long  baudRate)
		{
			this->dcb.BaudRate = baudRate;
			if (this->IsOpen())
			{
				if (SetCommState(this->hComPort, &this->dcb))
				{
					return true;
				}
			}
			return false;
		}

		//---------------------------------------------------------------------------

		unsigned char SerialPort::GetStopBits() const
		{
			return this->dcb.StopBits;
		}

		//---------------------------------------------------------------------------

		bool SerialPort::SetStopBits(unsigned char stopBits)
		{
			this->dcb.StopBits = this->dcb.StopBits;
			if (this->IsOpen())
			{
				return SetCommState(this->hComPort, &this->dcb);
			}
			return false;
		}

		//---------------------------------------------------------------------------

		unsigned char SerialPort::GetByteSize() const
		{
			return this->dcb.ByteSize;
		}

		//---------------------------------------------------------------------------

		bool SerialPort::SetByteSize(unsigned char byteSize)
		{
			this->dcb.ByteSize = byteSize;
			if (this->IsOpen())
			{
				return SetCommState(this->hComPort, &this->dcb);
			}
			return false;
		}

		//---------------------------------------------------------------------------

		DWORD SerialPort::GetReadTimeout() const
		{
			return this->readTimeout;
		}

		//---------------------------------------------------------------------------

		bool SerialPort::SetReadTimeout(DWORD timeout)
		{
			this->readTimeout = timeout;
			this->commTimeouts.ReadTotalTimeoutConstant = this->readTimeout;
			if (this->IsOpen())
			{
				return SetCommTimeouts(this->hComPort, &this->commTimeouts);
			}
			return false;
		}

		//---------------------------------------------------------------------------

		DWORD SerialPort::GetReadInterval() const
		{
			return this->commTimeouts.ReadIntervalTimeout;
		}

		//---------------------------------------------------------------------------

		bool SerialPort::SetReadInterval(DWORD interval)
		{
			this->commTimeouts.ReadIntervalTimeout = interval;
			if (this->IsOpen())
			{
				return SetCommTimeouts(this->hComPort, &this->commTimeouts);
			}
			return false;
		}

		//---------------------------------------------------------------------------

		SerialPort::operator HANDLE() const
		{
			return this->hComPort;
		}
	}
}