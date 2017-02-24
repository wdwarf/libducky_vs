#include "stdafx.h"
#include "libducky/db/ADOCatalog.h"
#include <boost/algorithm/string.hpp>

namespace ducky
{
	namespace db {
		//--------------------------------------------------------------
		_W(string) Table::getName()
		{
			if (this->ptr)
			{
				return bstrToStr(this->ptr->Name);
			}
			return _T("");
		}

		_W(string) Table::getType()
		{
			if (this->ptr)
			{
				return bstrToStr(this->ptr->Type);
			}
			return _T("");
		}

		//--------------------------------------------------------------

		Table Tables::operator[](long i)
		{
			return this->ptr->Item[i];
		}

		long Tables::getCount()
		{
			return this->ptr->Count;
		}

		//--------------------------------------------------------------

		void ADOCatalog::setConnection(_ConnectionPtr conn)
		{
			this->ptr->PutActiveConnection(conn.GetInterfacePtr());
		}

		Tables ADOCatalog::getTables()
		{
			Tables tbs;
			if (*this)
			{
				tbs = this->ptr->Tables;
			}
			return tbs;
		}

		Table ADOCatalog::getTable(_W(string) tableName)
		{
			Tables tbs = this->getTables();
			boost::trim(tableName);
			boost::to_upper(tableName);
			long count = tbs.getCount();
			for (long i = 0; i < count; ++i)
			{
				Table tb = tbs[i];
				_W(string) name = tb.getName();
				boost::trim(name);
				boost::to_upper(name);
				if (name == tableName)
				{
					return tb;
				}
			}
			return Table();
		}
	}
}
