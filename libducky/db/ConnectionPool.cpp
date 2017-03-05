#include "stdafx.h"
#include "libducky/db/ConnectionPool.h"
#include <boost/bind.hpp>

using namespace ducky::thread;

namespace ducky
{
	namespace db {
		ConnectionPool::ConnectionPool(_W(string) connStr, int poolSize)
			: _poolSize(poolSize)
			, _connStr(connStr)
		{
			this->hSem = CreateSemaphore(NULL, 0, poolSize, NULL);
			for (int i = 0; i < poolSize; ++i)
			{
				try
				{
					ADOConnection* conn = objPool.construct();
					if (conn)
					{
						this->CollectConnection(conn);
					}
				}
				catch (...)
				{
				}
			}
		}


		ConnectionPool::~ConnectionPool(void)
		{
			CloseHandle(this->hSem);
		}

		void ConnectionPool::CollectConnection(ADOConnection* conn)
		{
			CSLocker lk(cs);
			if (objPool.is_from(conn))
			{
				this->connList.push_front(conn);
				ReleaseSemaphore(this->hSem, 1, NULL);
			}
		}

		int ConnectionPool::GetPoolSize()
		{
			CSLocker lk(cs);
			return this->_poolSize;
		}

		int ConnectionPool::GetIdelConnectionCount()
		{
			CSLocker lk(cs);
			return this->connList.size();
		}

		boost::shared_ptr<ADOConnection>
			ConnectionPool::GetConnection(DWORD waitTimeout)
		{
			boost::shared_ptr<ADOConnection> re;
			if (WAIT_TIMEOUT != WaitForSingleObject(this->hSem, waitTimeout))
			{
				CSLocker lk(cs);

				if (!this->connList.empty())
				{
					ADOConnection* conn = this->connList.front();
					this->connList.pop_front();
					try
					{
						if (!this->_connStr.empty() && !conn->IsConnected())
						{
							conn->setConnectString(this->_connStr);
							conn->Open();
						}
					}
					catch (...) {}
					re = boost::shared_ptr<ADOConnection>(conn,
						boost::bind(&ConnectionPool::operator(), this, _1));
				}
			}

			return re;
		}

		void ConnectionPool::operator()(ADOConnection* conn)
		{
			this->CollectConnection(conn);
		}

		void ConnectionPool::SetConnctionString(_W(string) connStr)
		{
			this->_connStr = connStr;
		}

	}
}