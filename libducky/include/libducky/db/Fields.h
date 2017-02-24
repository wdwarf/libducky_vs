#ifndef LIBDUCKY_DB_Fields_H_
#define LIBDUCKY_DB_Fields_H_

#include "libducky_config.h"
#include "AdoObject.h"
#include "Field.h"
#include <vector>

using std::vector;

namespace ducky
{
	namespace db {
		class Fields :
			public ADOObject
		{
		public:
			Fields(void);
			Fields(const Fields& fields);
			Fields(FieldsPtr& fields);
			virtual ~Fields(void);
			void operator =(const Fields& fields);
			void operator =(const FieldsPtr& fields);
			FieldsPtr& operator->();
			operator FieldsPtr();
			long getFieldCount();
			vector<_W(string)> getFieldNames();

			Field operator[](_W(string) fieldName);
			Field operator[](long fieldIndex);

		private:
			FieldsPtr fields;
		};
	}
}
#endif