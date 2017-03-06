// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <libducky/Object.h>
#include <libducky/thread/Thread.h>
#include <libducky/exception/Exception.h>
#include <libducky/factory/Factory.h>
#include <libducky/singleton/Singleton.h>
#include <libducky/serial_port/SerialPort.h>
#include <iostream>
#include <sstream>

using namespace ducky::thread;
using namespace ducky::exception;
using namespace ducky::factory;
using namespace ducky::serial_port;
using namespace std;

class T : public Thread
{
public:
	~T() {
		cout << "~T" << endl;
	}
	unsigned long run() {
		while (!this->canStop())
		{
			cout << "running" << endl;
			this->Sleep(1010);
			//throw std::exception("aa");
		}

		return 0;
	}
};

typedef ducky::singleton::Singleton2<Factory> ObjFac;

class MyComm : public SerialPort {
	virtual void onRead(const char* buf, int len) {
		string str(buf, len);
		cout << str << endl;
		this->write("thanks", 6);
	}
	virtual void onWrite(const char* buf, int len) {
		cout << "send: " << string(buf, len) << endl;
	}
	virtual void onReadTimeout() {}
	virtual void onOpen() {
		cout << this->getPort() << " opened" << endl;
	}
	virtual void onClose() {
		cout << this->getPort() << " closed" << endl;
	}
};

int main()try
{
	auto comPorts = SerialPort::ListComPorts();
	for (const string& port : comPorts) {
		cout << port << endl;
	}

	MyComm com;
	com.setPort("com8");
	com.setAsync();
	com.open();
	cin.get();
	com.close();
	cin.get();
	return 0;

	auto f = ObjFac::getInstance();
	f->regiesterCreator<T>("T");
	T* t = f->createObject<T>("T");
	try {
		try {
			cout << "running class: " << t->getClassName() << endl;
			t->setFreeOnTerminated(true);
			t->start();
			Thread::Sleep(100);
			t->start();
		}
		catch (Exception& e) {
			Exception e2 = MK_EXCEPTION(Exception, "thread maybe call start func twice.", 0);
			e2.setInnerException(&e);
			throw e2;
		}
	}
	catch (Exception& e) {
		string info;
		if (t && t->isRunning()) {
			info = "thread is running, try to stop it.";
			t->stop();
		}
		else {
			info = "thread is not rnaning.";
		}

		Exception e2 = MK_EXCEPTION(Exception, info, 0);
		e2.setInnerException(&e);
		throw e2;
	}
	

	cin.get();
	return 0;
}
catch (Exception& e) {
	const Exception* pe = &e;
	do {
		cout << pe->getFileName() << ", L" << pe->getLineNumber() << ": " << pe->what() << endl;
		pe = pe->getInnerException();
	} while (pe);
	cin.get();
	return -1;
}

