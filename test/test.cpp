// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <libducky\Object.h>
#include <libducky\thread\Thread.h>
#include <libducky/exception/Exception.h>
#include <iostream>

using namespace ducky::thread;
using namespace ducky::exception;
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

int main()try
{
	T* t = new T;
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

