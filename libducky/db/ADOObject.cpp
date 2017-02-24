#include "stdafx.h"
#include "libducky/db/ADOObject.h"
#include <iostream>

namespace ducky {
	namespace db {
		ADOObject::ADOObject(void)
		{
			::CoInitializeEx(NULL, COINIT_MULTITHREADED);
		}


		ADOObject::~ADOObject(void)
		{
		}

		bool ADOObject::CreateAccessFile(_W(string) dbFilePath, _W(string) password)
		{
			using namespace std;

			HRESULT hr = S_FALSE;
			_W(string) connStr(_T("Provider=Microsoft.JET.OLEDB.4.0;jet OleDB:Database Password=") + password + _T(";Data source = "));
			connStr += dbFilePath;

			try
			{
				ADOX::_CatalogPtr m_pCatalog = NULL;
				hr = m_pCatalog.CreateInstance(__uuidof (ADOX::Catalog));
				if (FAILED(hr))
				{
					_com_issue_error(hr);
				}
				else
				{
					hr = m_pCatalog->Create(_bstr_t(connStr.c_str()));
					m_pCatalog.Release();
				}
			}
			catch (...) {}
			if (FAILED(hr))
			{
				return false;
			}
			else
			{
				return true;
			}
		}

		_W(string) ADOObject::bstrToStr(bstr_t& str)
		{
			if (str.length() > 0)
			{
				return (const TCHAR*)str;
			}
			return _T("");
		}
	}
}