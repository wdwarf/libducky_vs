#ifndef LIBDUCKY_DB_ADOCatalog_H_
#define LIBDUCKY_DB_ADOCatalog_H_

#include "libducky_config.h"
#include "ADOObject.h"
#include "ADOConnection.h"
#include <string>
#include <vector>

using std::_W(string);
using std::vector;
using namespace ADOX;

namespace ducky
{
	namespace db {
		////////#define DEFINE_ADO_OBJ
#define DEFINE_ADO_OBJ(className, adoInterface, adoClass) \
public:\
	className()\
	{\
	this->ptr.CreateInstance(__uuidof(adoClass));\
}\
	\
	className(adoInterface##Ptr p)\
	{\
	this->ptr = p;\
}\
	className(const className& className##Obj)\
	{\
	this->ptr = className##Obj.ptr;\
}\
	\
	~className()\
	{\
	if (this->ptr)\
	{\
	this->ptr.Release();\
}\
}\
	\
	operator adoInterface##Ptr()\
	{\
	return this->ptr;\
}\
	\
	operator bool()\
	{\
	return this->ptr;\
}\
	\
	adoInterface##Ptr operator->()\
	{\
	return this->ptr;\
}\
	\
	className& operator=(adoInterface##Ptr p)\
	{\
	this->ptr = p;\
	return *this;\
}\
private:\
	adoInterface##Ptr ptr;
	////////end of #define DEFINE_ADO_OBJ

		class Key :
			public ADOObject
		{
		private:
			vector<_W(string)> columnNames;
			_W(string) name;
			int type;
		};

		class Table :
			public ADOObject
		{
		public:
			_W(string) getName();
			_W(string) getType();
			vector<_W(string)> getPrimaryKey();

			DEFINE_ADO_OBJ(Table, _Table, ADOX::Table)
		};

		class Tables :
			public ADOObject
		{
		public:
			Table operator[](long i);
			long getCount();

			DEFINE_ADO_OBJ(Tables, Tables, ADOX::Tables)
		};

		class ADOCatalog :
			public ADOObject
		{
		public:
			void setConnection(_ConnectionPtr conn);
			Tables getTables();
			Table getTable(_W(string) tableName);

			DEFINE_ADO_OBJ(ADOCatalog, _Catalog, Catalog)
		};

	}
}

#endif