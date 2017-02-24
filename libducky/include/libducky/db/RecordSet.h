#ifndef LIBDUCKY_DB_RecordSet_H_
#define LIBDUCKY_DB_RecordSet_H_

#include "libducky_config.h"
#include "AdoObject.h"
#include "Fields.h"
#include "ADOConnection.h"

namespace ducky
{
	namespace db {
		class RecordSet :
			public ADOObject
		{
		public:
			RecordSet(void);
			RecordSet(_RecordsetPtr rs);
			virtual ~RecordSet(void);

			operator _RecordsetPtr();
			_RecordsetPtr& operator->();
			void operator =(_RecordsetPtr rs);
			operator bool();

			void prev();
			void next();
			void first();
			void last();
			bool bof();
			bool eof();
			void del();
			long getRecordCount();

			void open();
			void close();

			void setSql(_W(string) sql);
			_W(string) getSql();
			void setCursorType(CursorTypeEnum cursorType);
			void setLockType(LockTypeEnum lockType);
			void setOptions(long options);

			Fields getFields();
			void setConnection(const ADOConnection& conn);
			ADOConnection getConnection();

		private:
			bstr_t sql;
			long options;
			LockTypeEnum lockType;
			CursorTypeEnum cursorType;
			_RecordsetPtr rs;
		};
	}
}
#endif