#ifndef LIBDUCKY_DB_Field_H_
#define LIBDUCKY_DB_Field_H_

#include "libducky_config.h"
#include "ADOObject.h"

namespace ducky
{
	namespace db {
		class Field :
			public ADOObject
		{
		public:
			Field(void);
			Field(const Field& field);
			Field(FieldPtr& field);
			virtual ~Field(void);

			FieldPtr& operator->();
			operator FieldPtr();
			void operator =(const Field& field);
			void operator =(const FieldPtr& field);
			_W(string) getName();

			operator variant_t();
			operator long();
			operator double();
			operator bool();
			operator _W(string)();
			operator time_t();

			variant_t getValue();
			_W(string) valAsString();
			long valAsInt();
			double valAsFloat();
			time_t valAsTime();

		private:
			FieldPtr field;
		};

	}
}
#endif