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

int main()
{
	T* t = new T;
	try {
		cout << "running class: " << t->getClassName() << endl;
		t->setFreeOnTerminated(true);
		t->start();
		Thread::Sleep(100);
		t->start();
	}
	catch (Exception& e) {
		const Exception* pe = &e;
		do {
			cout << pe->getFileName() << ", L" << pe->getLineNumber() << ": " << pe->what() << endl;
			pe = pe->getInnerException();
		} while (pe);
	}
	cin.get();
	if (t->isRunning()) {
		t->stop();
	}

	cin.get();
	return 0;
}

