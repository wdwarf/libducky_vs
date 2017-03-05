#ifndef _Registry_H_
#define _Registry_H_

#include "libducky_config.h"
#include <libducky/Object.h>
#include <Windows.h>
#include <string>
#include <list>
#include <boost/smart_ptr.hpp>

using std::_W(string);
using std::list;

/*
keys:
HKEY_CLASSES_ROOT
HKEY_CURRENT_CONFIG
HKEY_CURRENT_USER
HKEY_LOCAL_MACHINE
HKEY_USERS
*/
namespace ducky
{
	namespace registry
	{


		class RegistryValue
		{
		public:
			RegistryValue(_W(string) name, const TCHAR* data,
				DWORD dataLen, DWORD dataType);
			RegistryValue(const RegistryValue& rv);
			~RegistryValue();
			void operator=(const RegistryValue& rv);
			_W(string) getName();
			TCHAR* getData();
			DWORD getDataLen();
			DWORD getDataType();

			_W(string) asString();
			DWORD asLong();
			operator bool();
			bool operator<(RegistryValue& rv);

		private:
			_W(string) _name;
			boost::scoped_array<TCHAR> _data;
			DWORD _dataLen;
			DWORD _dataType;

			void Clear();
			void SetValue(_W(string) name, const TCHAR* data,
				DWORD dataLen, DWORD dataType);
		};

		class Registry
		{
		public:
			Registry(HKEY key = 0);
			~Registry(void);

			bool Open(
				HKEY hKey,
				_W(string) subKey = _T(""),
				REGSAM samDesired = KEY_ALL_ACCESS);
			bool IsOpen();
			bool Close();
			list<_W(string)> EnumKeys();
			list<RegistryValue> EnumValues();
			list<RegistryValue> EnumStringValues();
			bool GetValue(_W(string) subKey, _W(string) valueName,
				LPDWORD pType, void* pDataBuf, LPDWORD pBufLen,
				DWORD flags = RRF_RT_REG_SZ);

			bool SetKeyValue(_W(string) keyName, const TCHAR* data,
				DWORD dataLen, DWORD dataType = REG_SZ, _W(string) subKey = _T(""));

			bool SetValue(_W(string) keyName, const TCHAR* data,
				DWORD dataLen, DWORD dataType = REG_SZ);

			bool DeleteValue(_W(string) keyName, DWORD dataType = KEY_WOW64_32KEY);

			boost::shared_ptr<Registry> CreateKey(
				_W(string) subKey,
				DWORD reserved = 0,
				_W(string) strClass = _T(""),
				DWORD options = REG_OPTION_NON_VOLATILE,
				REGSAM samDesired = KEY_ALL_ACCESS,
				_In_opt_ CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL,
				_Out_opt_ LPDWORD lpdwDisposition = NULL);

		protected:
			HKEY key;
		};

	}
}
#endif