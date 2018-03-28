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
	void run() {
		while (!this->canStop())
		{
			cout << "running" << endl;
			this->Sleep(1010);
			//throw std::exception("aa");
		}

		return;
	}

	void onTerminated() {
		cout << "thread terminated" << endl;
	}
};

int main()try
{
	T* t = new T;
	t->start();
	cin.get();
	t->stop();
	t->join();
	

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

