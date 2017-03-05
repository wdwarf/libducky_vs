#pragma once
#include "libducky_config.h"
#include <libducky/Object.h>
#include "ADOConnection.h"
#include "libducky/thread/CriticalSection.h"
#include <list>
#include <boost/smart_ptr.hpp>
#include <boost/pool/object_pool.hpp>

using ducky::db::ADOConnection;
using ducky::thread::CriticalSection;
using std::list;

namespace ducky
{
	namespace db {

		class ConnectionPool : virtual public Object
		{
		public:
			ConnectionPool(_W(string) connStr = _T(""), int itemCount = 100);
			virtual ~ConnectionPool(void);

			void SetConnctionString(_W(string) connStr);
			int GetPoolSize();
			int GetIdelConnectionCount();

			boost::shared_ptr<ADOConnection>
				GetConnection(DWORD waitTimeout = INFINITE);
			void operator()(ADOConnection* conn);

		private:
			friend class ConnectionPoolPtr;
			void CollectConnection(ADOConnection* conn);
			int _poolSize;
			_W(string) _connStr;
			list<ADOConnection*> connList;

			CriticalSection cs;
			HANDLE hSem;
			boost::object_pool<ADOConnection> objPool;
		};

	}
}