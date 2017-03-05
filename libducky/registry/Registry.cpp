#include "stdafx.h"
#include <libducky/registry/Registry.h>
#include <iostream>
#include <sstream>

using namespace std;

#pragma comment(lib, "Advapi32.lib")

namespace ducky
{
	namespace registry
	{

		//-------------------------------------------------------------

		RegistryValue::RegistryValue(_W(string) name, const TCHAR* data,
			DWORD dataLen, DWORD dataType)
		{
			this->SetValue(name, data, dataLen, dataType);
		}

		//-------------------------------------------------------------

		RegistryValue::~RegistryValue()
		{
			this->Clear();
		}

		//-------------------------------------------------------------

		RegistryValue::RegistryValue(const RegistryValue& rv)
		{
			this->_data.reset();
			this->SetValue(rv._name, &rv._data[0], rv._dataLen, rv._dataType);
		}

		//-------------------------------------------------------------

		void RegistryValue::operator=(const RegistryValue& rv)
		{
			this->SetValue(rv._name, &rv._data[0], rv._dataLen, rv._dataType);
		}

		//-------------------------------------------------------------

		bool RegistryValue::operator<(RegistryValue& rv)
		{
			return this->_name < rv._name;
		}

		//-------------------------------------------------------------

		void RegistryValue::Clear()
		{
			if (!this->_data.get())
			{
				this->_data.reset();
			}
			this->_name = _T("");
			this->_dataType = 0;
			this->_dataLen = 0;
		}

		//-------------------------------------------------------------

		void RegistryValue::SetValue(_W(string) name, const TCHAR* data,
			DWORD dataLen, DWORD dataType)
		{
			this->Clear();
			this->_name = name;
			this->_dataType = dataType;
			this->_dataLen = dataLen;
			this->_data.reset(new TCHAR[dataLen + 1]);
			memset(&this->_data[0], 0, (dataLen + 1) * sizeof(TCHAR));
			memcpy(&this->_data[0], data, dataLen * sizeof(TCHAR));
		}

		//-------------------------------------------------------------

		_W(string) RegistryValue::getName()
		{
			return this->_name;
		}

		//-------------------------------------------------------------

		TCHAR* RegistryValue::getData()
		{
			return this->_data.get();
		}

		//-------------------------------------------------------------

		DWORD RegistryValue::getDataLen()
		{
			return this->_dataLen;
		}

		//-------------------------------------------------------------

		DWORD RegistryValue::getDataType()
		{
			return this->_dataType;
		}

		//-------------------------------------------------------------

		RegistryValue::operator bool()
		{
			return 0 != this->_data.get();
		}

		//-------------------------------------------------------------

		_W(string) RegistryValue::asString()
		{
			_W(string) re;
			if (this->_data.get())
			{
				switch (this->_dataType)
				{
				case REG_SZ:
				case REG_EXPAND_SZ:
				{
					re = &this->_data[0];
				}
				break;
				case REG_DWORD:
				case REG_DWORD_BIG_ENDIAN:
				{
					DWORD dRe = 0;
					if (sizeof(DWORD) >= this->_dataLen)
					{
						memcpy(&dRe, &this->_data[0], this->_dataLen * sizeof(TCHAR));
					}
					_W(stringstream) str;
					str << dRe;
					re = str.str();
				}
				break;
				case REG_BINARY:
				{
					_W(stringstream) str;
					for (int i = 0; i < this->_dataLen; ++i)
					{
						unsigned short n = 0;
						memcpy(&n, &this->_data[0] + i, 1);
						str.width(2);
						str.fill(_T('0'));
						str << hex;
						str << n << _T(" ");
					}
					re = str.str();
				}
				break;
				}
			}
			return re;
		}

		//-------------------------------------------------------------

		DWORD RegistryValue::asLong()
		{
			DWORD re = 0;
			if (this->_data)
			{
				switch (this->_dataType)
				{
				case REG_SZ:
				case REG_EXPAND_SZ:
				{
					_W(stringstream) str;
					str << &this->_data[0];
					str >> re;
				}
				break;
				case REG_DWORD:
				case REG_DWORD_BIG_ENDIAN:
				{
					if (sizeof(DWORD) >= this->_dataLen)
					{
						memcpy(&re, &this->_data[0], this->_dataLen * sizeof(TCHAR));
					}
				}
				break;
				case REG_BINARY:
				{
					if (this->_dataLen <= sizeof(DWORD))
					{
						memcpy(&re, &this->_data[0], this->_dataLen * sizeof(TCHAR));
					}
				}
				break;
				}
			}
			return re;
		}


		//-------------------------------------------------------------


		Registry::Registry(HKEY key)
		{
			this->key = key;
		}

		//-------------------------------------------------------------

		Registry::~Registry(void)
		{
			this->Close();
		}

		//-------------------------------------------------------------

		bool Registry::Open(
			HKEY hKey,
			_W(string) subKey,
			REGSAM samDesired)
		{
			this->Close();
			return (ERROR_SUCCESS == ::RegOpenKeyEx(hKey, subKey.c_str(),
				0, samDesired, &this->key));
		}

		//-------------------------------------------------------------

		bool Registry::IsOpen()
		{
			return (0 != this->key);
		}

		//-------------------------------------------------------------

		bool Registry::Close()
		{
			if (ERROR_SUCCESS == ::RegCloseKey(this->key))
			{
				this->key = 0;
				return true;
			}
			return false;
		}

		//-------------------------------------------------------------

		bool Registry::GetValue(_W(string) subKey, _W(string) valueName,
			LPDWORD pType, void* pDataBuf, LPDWORD pBufLen,
			DWORD flags)
		{
			return (ERROR_SUCCESS == RegGetValue(
				this->key, subKey.c_str(), valueName.c_str(), flags, pType,
				pDataBuf, pBufLen));
		}

		//-------------------------------------------------------------

		list<_W(string)> Registry::EnumKeys()
		{
			list<_W(string)> re;
			if (this->IsOpen())
			{
				DWORD index = 0;
				DWORD bufLen = 320;
				boost::scoped_array<TCHAR> bufName(new TCHAR[bufLen]);
				memset(&bufName[0], 0, bufLen * sizeof(TCHAR));
				while (ERROR_SUCCESS == ::RegEnumKey(this->key, index++, &bufName[0], bufLen))
				{
					re.push_back(&bufName[0]);
					memset(&bufName[0], 0, bufLen * sizeof(TCHAR));
				}
			}
			re.sort();
			return re;
		}

		//-------------------------------------------------------------

		list<RegistryValue> Registry::EnumValues()
		{
			list<RegistryValue> result;
			if (this->IsOpen())
			{
				DWORD index = 0;
				DWORD bufLen = 320;
				DWORD dataBufLen = 1024;
				boost::scoped_array<TCHAR> bufName(new TCHAR[bufLen]);
				boost::scoped_array<BYTE> bufData(new BYTE[dataBufLen]);
				DWORD nameLen = bufLen;
				DWORD dataLen = dataBufLen;
				DWORD type = 0;
				bool isHasData = true;
				while (isHasData)
				{
					nameLen = bufLen;
					dataLen = dataBufLen;
					memset(&bufName[0], 0, bufLen * sizeof(TCHAR));
					memset(&bufData[0], 0, dataBufLen);

					DWORD re = ::RegEnumValue(this->key, index,
						&bufName[0], &nameLen, 0, &type, &bufData[0], &dataLen);

					switch (re)
					{
					case ERROR_SUCCESS:
					{
						RegistryValue rv(&bufName[0], (TCHAR*)&bufData[0], dataLen, type);
						result.push_back(rv);
						++index;
					}
					break;
					case ERROR_MORE_DATA:
					{
						dataBufLen = dataLen + 1;
						bufData.reset(new BYTE[dataBufLen]);
					}
					break;
					case ERROR_NO_DATA:
					case ERROR_NO_MORE_ITEMS:
					{
						isHasData = false;
					}
					break;
					default:
						++index;
						break;
					}
				}
			}
			result.sort();
			return result;
		}

		//-------------------------------------------------------------

		list<RegistryValue> Registry::EnumStringValues()
		{
			list<RegistryValue> result;
			list<RegistryValue> valueList = this->EnumValues();
			for (list<RegistryValue>::iterator it = valueList.begin(); it != valueList.end(); ++it)
			{
				if (REG_SZ == it->getDataType() || REG_EXPAND_SZ == it->getDataType())
				{
					result.push_back(*it);
				}
			}
			return result;
		}

		//-------------------------------------------------------------

		bool Registry::SetKeyValue(_W(string) keyName, const TCHAR* data,
			DWORD dataLen, DWORD dataType, _W(string) subKey)
		{
			return (ERROR_SUCCESS == RegSetKeyValue(this->key, subKey.c_str(), keyName.c_str(),
				dataType, data, dataLen));
		}

		bool Registry::SetValue(_W(string) keyName, const TCHAR* data,
			DWORD dataLen, DWORD dataType)
		{
			return (ERROR_SUCCESS == RegSetValueEx(this->key, keyName.c_str(), 0, dataType, (BYTE*)data, dataLen));
			//return RegSetValue(this->key, keyName.c_str(), dataType, data, dataLen);
		}

		bool Registry::DeleteValue(_W(string) keyName, DWORD dataType)
		{
			return (ERROR_SUCCESS == RegDeleteValue(this->key, keyName.c_str()));
			//return RegDeleteKeyEx(this->key, keyName.c_str(), dataType, 0);
		}

		boost::shared_ptr<Registry> Registry::CreateKey(
			_W(string) subKey,
			DWORD reserved,
			_W(string) strClass,
			DWORD options,
			REGSAM samDesired,
			_In_opt_ CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			_Out_opt_ LPDWORD lpdwDisposition)
		{
			boost::shared_ptr<Registry> re = boost::make_shared<Registry>((HKEY)NULL);
			HKEY newKey = 0;
			DWORD disposition = 0;
			long ec = RegCreateKeyEx(this->key, subKey.c_str(),
				reserved, (LPTSTR)strClass.c_str(), options, samDesired,
				lpSecurityAttributes, &newKey, &disposition);
			if (ERROR_SUCCESS == ec)
			{
				re = boost::make_shared<Registry>(newKey);
			}
			return re;
		}

	}
}