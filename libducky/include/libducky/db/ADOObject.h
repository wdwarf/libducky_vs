#ifndef LIBDUCKY_DB_ADOObject_H_
#define LIBDUCKY_DB_ADOObject_H_

#include "libducky_config.h"
#include <libducky/Object.h>
#include <string>
using std::_W(string);

#define __USE_SYSTEM_ADO__

#ifdef __USE_SYSTEM_ADO__
#import "C:\Program Files\Common Files\system\ado\msadox.dll"
#import "C:\Program files\common files\system\ole db\oledb32.dll" //rename_namespace("wOLEDB")
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","wADOEOF")
#else
#import "msadox.dll"
#import "oledb32.dll" //rename_namespace("wOLEDB")
#import "msado15.dll" no_namespace rename("EOF","wADOEOF")
#endif
namespace ducky
{
	namespace db {

		class ADOObject : virtual public Object
		{
		public:
			ADOObject(void);
			virtual ~ADOObject(void);

			static bool CreateAccessFile(_W(string) dbFilePath, _W(string) password = _T(""));
			static _W(string) bstrToStr(bstr_t& str);

		};

	}
}
#endif