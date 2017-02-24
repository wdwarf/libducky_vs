#include "stdafx.h"
#include <msdasc.h>
#include <boost/algorithm/string.hpp>
#include "libducky/db/ADOConnection.h"
#include "libducky/db/RecordSet.h"
#include "libducky/db/Fields.h"
#include "libducky/db/Field.h"
#include "libducky/exception/Exception.h"
#include "libducky/db/ADOCatalog.h"
#include <sstream>

using namespace std;
namespace ducky {
	namespace db {

#define NONTHRO(stat) try{stat}catch(...){}
		//#define NONTHRO(stat) stat

		ADOConnection::ADOConnection(void)
		{
			HRESULT re = conn.CreateInstance(__uuidof(Connection));
			this->options = adConnectUnspecified;
			this->conn->CursorLocation = adUseClient;
		}

		ADOConnection::ADOConnection(const _ConnectionPtr& conn)
		{
			this->conn = conn;
			this->options = adConnectUnspecified;
			this->conn->CursorLocation = conn->GetCursorLocation();
		}

		void ADOConnection::operator =(const ADOConnection& conn)
		{
			this->conn = conn;
			this->options = adConnectUnspecified;
			this->conn->CursorLocation = conn.conn->GetCursorLocation();
		}

		void ADOConnection::operator =(const _ConnectionPtr& conn)
		{
			this->conn = conn;
			this->options = adConnectUnspecified;
			this->conn->CursorLocation = conn->GetCursorLocation();
		}

		ADOConnection::operator bool()
		{
			return this->conn;
		}

		ADOConnection::~ADOConnection(void)
		{
			conn.Release();
		}

		void ADOConnection::Open()
		{
			this->conn->Open("", userId, password, options);
			this->properties.clear();
			//this->getProperties(this->properties);
		}

		void ADOConnection::Close()
		{
			this->conn->Close();
		}

		bool ADOConnection::IsConnected() const
		{
			return adStateOpen == (adStateOpen & this->conn->GetState());
		}

		ADOConnection::operator _ConnectionPtr() const
		{
			return this->conn;
		}

		ADOConnection::operator _Connection&() const
		{
			return this->conn;
		}

		ADOConnection::operator _Connection*() const
		{
			return this->conn;
		}

		void ADOConnection::setConnectString(_W(string) connStr)
		{
			this->conn->ConnectionString = connStr.c_str();
		}

		_W(string) ADOConnection::getConnectionString() const
		{
			if (this->conn && this->conn->ConnectionString.length() > 0)
			{
				return (const TCHAR*)this->conn->ConnectionString;
			}
			return _T("");
		}

		void ADOConnection::setUserId(_W(string) userId)
		{
			this->userId = userId.c_str();
		}

		_W(string) ADOConnection::getUserId() const
		{
			return (const TCHAR*)this->userId;
		}

		void ADOConnection::setPassword(_W(string) pass)
		{
			this->password = pass.c_str();
		}

		_W(string) ADOConnection::getPassword() const
		{
			return (const TCHAR*)this->password;
		}

		void ADOConnection::setOptions(long opt)
		{
			this->options = opt;
		}

		long ADOConnection::getOptions() const
		{
			return this->options;
		}

		long ADOConnection::getState() const
		{
			return this->conn->GetState();
		}

		void ADOConnection::setCursorLocation(CursorLocationEnum cl)
		{
			this->conn->CursorLocation = cl;
		}

		CursorLocationEnum ADOConnection::getCursorLocation() const
		{
			return this->conn->CursorLocation;
		}

		void ADOConnection::setConnectMode(ConnectModeEnum mode)
		{
			this->conn->PutMode(mode);
		}

		ConnectModeEnum ADOConnection::getConnectMode() const
		{
			return this->conn->GetMode();
		}

		void ADOConnection::BeginTrans()
		{
			this->conn->BeginTrans();
		}

		void ADOConnection::CommitTrans()
		{
			this->conn->CommitTrans();
		}

		void ADOConnection::RollbackTrans()
		{
			this->conn->RollbackTrans();
		}

		_ConnectionPtr& ADOConnection::operator->()
		{
			return this->conn;
		}

		_W(string) ColumnInfo::getColumnDataTypeName(long iDataType)
		{
			_W(string) typeName;
			switch (iDataType)
			{
			case 2:
				typeName = _T("SMALLMONEY");
				break;
			case 3:
				typeName = _T("INT");
				break;
			case 4:
				typeName = _T("REAL");
				break;
			case 5:
				typeName = _T("FLOAT");
				break;
			case 6:
				typeName = _T("MONEY");
				break;
			case 7:
				typeName = _T("DATETIME");
				break;
			case 11:
				typeName = _T("BIT");
				break;
			case 13:
				typeName = _T("TIMESTAMP");
				break;
			case 17:
				typeName = _T("UNIQUEIDENTIFIER");
				break;
			case 20:
				typeName = _T("BIGINT");
				break;
			case 72:
				typeName = _T("UUID");
				break;
			case 128:
				typeName = _T("VARBINARY");
				break;
			case 129:
				typeName = _T("VARCHAR");
				break;
			case 130:
				typeName = _T("NVARCHAR");
				break;
			case 131:
				typeName = _T("DECIMAL");
				break;
			case 135:
				typeName = _T("TIMESTAMP");
				break;
			case 139:
				typeName = _T("NUMBER");
				break;
			case 200:
				typeName = _T("");
				break;
			case 201:
				typeName = _T("TIMESTAMP");
				break;
			case 202:
				typeName = _T("NVARCHAR");
				break;
			case 203:
				typeName = _T("NTEXT");
				break;
			case 204:
				typeName = _T("VARCHAR");
				break;
			case 205:
				typeName = _T("IMAGE");
				break;
			}
			return typeName;
		}

		set<_W(string)> ADOConnection::getPrimaryKeys(_W(string) tableName,
			_W(string) tableSchema, _W(string) tableCatalog)
		{
			set<_W(string)> re;
			RecordSet rs =
				this->conn->OpenSchema(adSchemaPrimaryKeys);
			_W(stringstream) filterStr;
			filterStr << _T("TABLE_NAME='") << tableName << _T("'");
			if (!tableSchema.empty())
			{
				filterStr << _T(" and TABLE_SCHEMA='") << tableSchema << _T("'");
			}
			if (!tableCatalog.empty())
			{
				filterStr << _T(" and TABLE_CATALOG='") << tableCatalog << _T("'");
			}
			rs->Filter = filterStr.str().c_str();
			while (!rs.eof())
			{
				Fields fs = rs.getFields();
				re.insert(fs[_T("COLUMN_NAME")]);
				rs.next();
			}
			return re;
		}

		_W(string) ADOConnection::getDatabaseName() const
		{
			bstr_t name = this->conn->GetDefaultDatabase();
			if (name.length() > 0)
			{
				return (const TCHAR*)name;
			}
			else
			{
				return _T("");
			}
		}

		map<_W(string), TableInfo> ADOConnection::getTableInfos()
		{
			map<_W(string), TableInfo> re;

			_W(string) userName = this->getProperty(_T("User Name"));
			_W(string) schemaUsage = this->getProperty(_T("Schema Usage"));
			_W(string) dbmsName = this->getProperty(_T("DBMS Name"));
			boost::to_lower(dbmsName);
			long iSchemaUsage = 0;
			_W(stringstream) str;
			str << schemaUsage;
			str >> iSchemaUsage;

			variant_t vtTableSchema(bstr_t(userName.c_str()));

			variant_t var;
			var.vt = VT_EMPTY;
			variant_t vtCriteria;
			long ix = 0;
			SAFEARRAY* pSa = SafeArrayCreateVector(VT_VARIANT, 0, 4);
			ix = 0;
			SafeArrayPutElement(pSa, &ix, &var);
			ix = 1;
			if ((iSchemaUsage > 0) && (_T("oracle") == dbmsName))
				SafeArrayPutElement(pSa, &ix, &vtTableSchema);
			else
				SafeArrayPutElement(pSa, &ix, &var);
			ix = 2;
			SafeArrayPutElement(pSa, &ix, &var);
			ix = 3;
			SafeArrayPutElement(pSa, &ix, &var);
			vtCriteria.vt = VT_ARRAY | VT_VARIANT;
			vtCriteria.parray = pSa;

			RecordSet rs =
				this->conn->OpenSchema(adSchemaTables, vtCriteria);
			while (!rs.eof())
			{
				Fields fields = rs->GetFields();
				_W(string) tbType = fields[_T("TABLE_TYPE")].valAsString();
				if (_T("TABLE") == tbType)
				{
					TableInfo info;
					NONTHRO(info.setCatalog(fields[_T("TABLE_CATALOG")].valAsString()););
					NONTHRO(info.setSchema(fields[_T("TABLE_SCHEMA")].valAsString()););
					NONTHRO(info.setName(fields[_T("TABLE_NAME")].valAsString()););
					NONTHRO(info.setType(fields[_T("TABLE_TYPE")].valAsString()););
					NONTHRO(info.setGuid(fields[_T("TABLE_GUID")].valAsString()););
					NONTHRO(info.setDescription(fields[_T("DESCRIPTION")].valAsString()););
					NONTHRO(info.setPropid(fields[_T("TABLE_PROPID")].valAsString()););
					NONTHRO(info.setCreatedDate(fields[_T("DATE_CREATED")].valAsString()););
					NONTHRO(info.setModifiedDate(fields[_T("DATE_MODIFIED")].valAsString()););

					info.setPrimaryKeys(
						this->getPrimaryKeys(info.getName(), info.getSchema(), info.getCatalog()));

					re.insert(make_pair(info.getName(), info));
				}
				rs->MoveNext();
			}

			for (map<_W(string), TableInfo>::iterator it = re.begin(); it != re.end(); ++it)
			{
				_W(string) tableName = it->second.getName();
				variant_t vtTbName(bstr_t(tableName.c_str()));
				variant_t vtTableSchema(bstr_t(it->second.getSchema().c_str()));
				variant_t vtTableCatalog(bstr_t(it->second.getCatalog().c_str()));

				variant_t var;
				var.vt = VT_EMPTY;
				variant_t vtCriteria;
				long ix = 0;
				SAFEARRAY* pSa = SafeArrayCreateVector(VT_VARIANT, 0, 4);
				ix = 0;
				if (iSchemaUsage > 0)
					SafeArrayPutElement(pSa, &ix, &vtTableCatalog);
				else
					SafeArrayPutElement(pSa, &ix, &var);
				ix = 1;
				if (iSchemaUsage > 0)
					SafeArrayPutElement(pSa, &ix, &vtTableSchema);
				else
					SafeArrayPutElement(pSa, &ix, &var);
				ix = 2;
				SafeArrayPutElement(pSa, &ix, &vtTbName);
				ix = 3;
				SafeArrayPutElement(pSa, &ix, &var);
				vtCriteria.vt = VT_ARRAY | VT_VARIANT;
				vtCriteria.parray = pSa;

				rs = this->conn->OpenSchema(adSchemaColumns, vtCriteria);

				while (!rs.eof())
				{
					Fields fields = rs->GetFields();

					ColumnInfo columnInfo;
					NONTHRO(columnInfo.setTableCatalog(fields[_T("TABLE_CATALOG")]););
					NONTHRO(columnInfo.setTableSchema(fields[_T("TABLE_SCHEMA")]););
					NONTHRO(columnInfo.setTableName(fields[_T("TABLE_NAME")]););
					NONTHRO(columnInfo.setColumnName(fields[_T("COLUMN_NAME")]););
					NONTHRO(columnInfo.setColumnGuid(fields[_T("COLUMN_GUID")]););
					NONTHRO(columnInfo.setColumnPropid(fields[_T("COLUMN_PROPID")]););
					NONTHRO(columnInfo.setOrdinalPosition(fields[_T("ORDINAL_POSITION")]););
					NONTHRO(columnInfo.setColumnHasdefault(fields[_T("COLUMN_HASDEFAULT")]););
					NONTHRO(columnInfo.setColumnDefault(fields[_T("COLUMN_DEFAULT")]););
					NONTHRO(columnInfo.setColumnFlags(fields[_T("COLUMN_FLAGS")]););
					NONTHRO(columnInfo.setIsNullable(fields[_T("IS_NULLABLE")]););
					NONTHRO(columnInfo.setDataType(fields[_T("DATA_TYPE")]););
					NONTHRO(columnInfo.setTypeGuid(fields[_T("TYPE_GUID")]););
					NONTHRO(columnInfo.setCharacterMaximumLength(fields[_T("CHARACTER_MAXIMUM_LENGTH")]););
					NONTHRO(columnInfo.setCharacterOctetLength(fields[_T("CHARACTER_OCTET_LENGTH")]););
					NONTHRO(columnInfo.setNumericPrecision(fields[_T("NUMERIC_PRECISION")]););
					NONTHRO(columnInfo.setNumericScale(fields[_T("NUMERIC_SCALE")]););
					NONTHRO(columnInfo.setDatetimePrecision(fields[_T("DATETIME_PRECISION")]););
					NONTHRO(columnInfo.setCharacterSetCatalog(fields[_T("CHARACTER_SET_CATALOG")]););
					NONTHRO(columnInfo.setCharacterSetSchema(fields[_T("CHARACTER_SET_SCHEMA")]););
					NONTHRO(columnInfo.setCharacterSetName(fields[_T("CHARACTER_SET_NAME")]););
					NONTHRO(columnInfo.setCollationCatalog(fields[_T("COLLATION_CATALOG")]););
					NONTHRO(columnInfo.setCollationSchema(fields[_T("COLLATION_SCHEMA")]););
					NONTHRO(columnInfo.setCollationName(fields[_T("COLLATION_NAME")]););
					NONTHRO(columnInfo.setDomainCatalog(fields[_T("DOMAIN_CATALOG")]););
					NONTHRO(columnInfo.setDomainSchema(fields[_T("DOMAIN_SCHEMA")]););
					NONTHRO(columnInfo.setDomainName(fields[_T("DOMAIN_NAME")]););
					NONTHRO(columnInfo.setDescription(fields[_T("DESCRIPTION")]););
					NONTHRO(columnInfo.setColumnLcid(fields[_T("COLUMN_LCID")]););
					NONTHRO(columnInfo.setColumnCompflags(fields[_T("COLUMN_COMPFLAGS")]););
					NONTHRO(columnInfo.setColumnSortid(fields[_T("COLUMN_SORTID")]););
					NONTHRO(columnInfo.setColumnTdscollation(fields[_T("COLUMN_TDSCOLLATION")]););
					NONTHRO(columnInfo.setIsComputed(fields[_T("IS_COMPUTED")]););

					rs->MoveNext();
				}
			}
			return re;
		}

		TableInfo ADOConnection::getTableInfo(_W(string) tableName)
		{
			TableInfo re;
			_W(string) srcTableName = tableName;
			boost::trim(tableName);
			boost::to_upper(tableName);

			_W(string) userName = this->getProperty(_T("User Name"));
			_W(string) schemaUsage = this->getProperty(_T("Schema Usage"));
			long iSchemaUsage = 0;
			_W(stringstream) str;
			str << schemaUsage;
			str >> iSchemaUsage;
			_W(string) dbmsName = this->getProperty(_T("DBMS Name"));
			boost::to_lower(dbmsName);

			variant_t var;
			var.vt = VT_EMPTY;
			variant_t vtCriteria;
			long ix = 0;
			variant_t vtTbName(bstr_t(tableName.c_str()));
			variant_t vtTableSchema(bstr_t(userName.c_str()));
			SAFEARRAY* pSa = SafeArrayCreateVector(VT_VARIANT, 0, 4);
			ix = 0;
			SafeArrayPutElement(pSa, &ix, &var);
			ix = 1;
			if ((iSchemaUsage > 0) && (_T("oracle") == dbmsName))
				SafeArrayPutElement(pSa, &ix, &vtTableSchema);
			else
				SafeArrayPutElement(pSa, &ix, &var);
			ix = 2;
			SafeArrayPutElement(pSa, &ix, &vtTbName);
			ix = 3;
			SafeArrayPutElement(pSa, &ix, &var);
			vtCriteria.vt = VT_ARRAY | VT_VARIANT;
			vtCriteria.parray = pSa;

			RecordSet rs =
				this->conn->OpenSchema(adSchemaTables, vtCriteria);

			while (!rs.eof())
			{
				Fields fields = rs->GetFields();

				_W(string) tbType = fields[_T("TABLE_TYPE")].valAsString();
				if (_T("TABLE") == tbType)
				{
					_W(string) tbName = fields[_T("TABLE_NAME")].valAsString();
					boost::trim(tbName);
					boost::to_upper(tbName);
					if (tbName == tableName)
					{
						NONTHRO(re.setCatalog(fields[_T("TABLE_CATALOG")].valAsString()););
						NONTHRO(re.setSchema(fields[_T("TABLE_SCHEMA")].valAsString()););
						NONTHRO(re.setName(fields[_T("TABLE_NAME")].valAsString()););
						NONTHRO(re.setType(fields[_T("TABLE_TYPE")].valAsString()););
						NONTHRO(re.setGuid(fields[_T("TABLE_GUID")].valAsString()););
						NONTHRO(re.setDescription(fields[_T("DESCRIPTION")].valAsString()););
						NONTHRO(re.setPropid(fields[_T("TABLE_PROPID")].valAsString()););
						NONTHRO(re.setCreatedDate(fields[_T("DATE_CREATED")].valAsString()););
						NONTHRO(re.setModifiedDate(fields[_T("DATE_MODIFIED")].valAsString()););

						re.setPrimaryKeys(
							this->getPrimaryKeys(re.getName(), re.getSchema(), re.getCatalog()));

						break;
					}
				}
				rs->MoveNext();
			}

			if (re.getName().empty())
			{
				return re;
			}

			_W(string) tbSchema = re.getSchema();
			boost::trim(tbSchema);
			boost::to_upper(tbSchema);

			rs = this->conn->OpenSchema(adSchemaColumns, vtCriteria);
			while (!rs.eof())
			{
				Fields fields = rs->GetFields();
				_W(string) colTbName = fields[_T("TABLE_NAME")].valAsString();
				_W(string) colTbSchema = fields[_T("TABLE_SCHEMA")];
				boost::trim(colTbName);
				boost::to_upper(colTbName);
				boost::trim(colTbSchema);
				boost::to_upper(colTbSchema);

				if ((colTbName == tableName) && (tbSchema == colTbSchema))
				{
					ColumnInfo columnInfo;
					NONTHRO(columnInfo.setTableCatalog(fields[_T("TABLE_CATALOG")]););
					NONTHRO(columnInfo.setTableSchema(fields[_T("TABLE_SCHEMA")]););
					NONTHRO(columnInfo.setTableName(fields[_T("TABLE_NAME")]););
					NONTHRO(columnInfo.setColumnName(fields[_T("COLUMN_NAME")]););
					NONTHRO(columnInfo.setColumnGuid(fields[_T("COLUMN_GUID")]););
					NONTHRO(columnInfo.setColumnPropid(fields[_T("COLUMN_PROPID")]););
					NONTHRO(columnInfo.setOrdinalPosition(fields[_T("ORDINAL_POSITION")]););
					NONTHRO(columnInfo.setColumnHasdefault(fields[_T("COLUMN_HASDEFAULT")]););
					NONTHRO(columnInfo.setColumnDefault(fields[_T("COLUMN_DEFAULT")]););
					NONTHRO(columnInfo.setColumnFlags(fields[_T("COLUMN_FLAGS")]););
					NONTHRO(columnInfo.setIsNullable(fields[_T("IS_NULLABLE")]););
					NONTHRO(columnInfo.setDataType(fields[_T("DATA_TYPE")]););
					NONTHRO(columnInfo.setTypeGuid(fields[_T("TYPE_GUID")]););
					NONTHRO(columnInfo.setCharacterMaximumLength(fields[_T("CHARACTER_MAXIMUM_LENGTH")]););
					NONTHRO(columnInfo.setCharacterOctetLength(fields[_T("CHARACTER_OCTET_LENGTH")]););
					NONTHRO(columnInfo.setNumericPrecision(fields[_T("NUMERIC_PRECISION")]););
					NONTHRO(columnInfo.setNumericScale(fields[_T("NUMERIC_SCALE")]););
					NONTHRO(columnInfo.setDatetimePrecision(fields[_T("DATETIME_PRECISION")]););
					NONTHRO(columnInfo.setCharacterSetCatalog(fields[_T("CHARACTER_SET_CATALOG")]););
					NONTHRO(columnInfo.setCharacterSetSchema(fields[_T("CHARACTER_SET_SCHEMA")]););
					NONTHRO(columnInfo.setCharacterSetName(fields[_T("CHARACTER_SET_NAME")]););
					NONTHRO(columnInfo.setCollationCatalog(fields[_T("COLLATION_CATALOG")]););
					NONTHRO(columnInfo.setCollationSchema(fields[_T("COLLATION_SCHEMA")]););
					NONTHRO(columnInfo.setCollationName(fields[_T("COLLATION_NAME")]););
					NONTHRO(columnInfo.setDomainCatalog(fields[_T("DOMAIN_CATALOG")]););
					NONTHRO(columnInfo.setDomainSchema(fields[_T("DOMAIN_SCHEMA")]););
					NONTHRO(columnInfo.setDomainName(fields[_T("DOMAIN_NAME")]););
					NONTHRO(columnInfo.setDescription(fields[_T("DESCRIPTION")]););
					NONTHRO(columnInfo.setColumnLcid(fields[_T("COLUMN_LCID")]););
					NONTHRO(columnInfo.setColumnCompflags(fields[_T("COLUMN_COMPFLAGS")]););
					NONTHRO(columnInfo.setColumnSortid(fields[_T("COLUMN_SORTID")]););
					NONTHRO(columnInfo.setColumnTdscollation(fields[_T("COLUMN_TDSCOLLATION")]););
					NONTHRO(columnInfo.setIsComputed(fields[_T("IS_COMPUTED")]););

					re.getColumnInfos().insert(make_pair(columnInfo.getColumnName(), columnInfo));
				}

				rs->MoveNext();
			}

			return re;
		}

		/*
		TableInfo ADOConnection::getTableInfo(_W(string) tableName)
		{
			TableInfo re;
			_W(string) srcTableName = tableName;
			boost::trim(tableName);
			boost::to_upper(tableName);

			try
			{
				ADOX::_CatalogPtr m_pCatalog = NULL;
				HRESULT hr = m_pCatalog.CreateInstance(__uuidof (ADOX::Catalog));
				if (FAILED(hr))
				{
					_com_issue_error(hr);
				}
				else
				{
					m_pCatalog->PutActiveConnection(this->conn.GetInterfacePtr());
					ADOX::TablesPtr tbs = m_pCatalog->Tables;
					long count = tbs->GetCount();
					for(long i = 0; i < count; ++i)
					{
						ADOX::_TablePtr tb = tbs->GetItem(i);
						if(_W(string)("TABLE") == tb->GetType().operator LPCSTR())
						{
							bstr_t name = tb->GetName();
							_W(string) tbName = name.operator LPCSTR();
							boost::trim(tbName);
							boost::to_upper(tbName);
							if(tbName == tableName)
							{
								fstream f;
								f.open("d:\\prop.txt", ios::out | ios::trunc);

								auto keys = tb->GetKeys();
								for(long i = 0; i < keys->Count; ++i)
								{
									auto key = keys->Item[i];
									f << "key: " << key->GetName().operator LPCSTR() << "\r\n";

									auto cols = key->Columns;

									for(long i = 0; i < cols->Count; ++i)
									{
										//f << cols->Item[i]->GetName().operator LPCSTR() << ", ";
									}
									f << "-----------------------------\r\n";
								}

								re.setName(tbName);
								ADOX::ColumnsPtr colsPtr = tb->Columns;
								long colCount = colsPtr->GetCount();

								for(long i = 0; i < colCount; ++i)
								{
									ADOX::_ColumnPtr colPtr = colsPtr->GetItem(i);
									_W(string) colName = colPtr->GetName().operator LPCSTR();

									ColumnInfo info;
									info.setColumnName(colName);
									info.setDataType(colPtr->GetType());

									re.getColumnInfos().push_back(info);
								}
								f.close();

								break;
							}
						}
					}
					m_pCatalog.Release();
				}
			}
			catch(_com_error& e)
			{
				bstr_t s = e.Description();
				s += e.ErrorMessage();
				if(s.length() > 0) MessageBox(0, s.operator LPCSTR(), 0, 0);
			}
			catch(...){}

			return re;
		}
		*/

		set<_W(string)> ADOConnection::getTableNames()
		{
			set<_W(string)> re;

			_W(string) userName = this->getProperty(_T("User Name"));
			_W(string) schemaUsage = this->getProperty(_T("Schema Usage"));
			long iSchemaUsage = 0;
			_W(stringstream) str;
			str << schemaUsage;
			str >> iSchemaUsage;
			_W(string) dbmsName = this->getProperty(_T("DBMS Name"));
			boost::to_lower(dbmsName);

			variant_t var;
			var.vt = VT_EMPTY;
			variant_t vtCriteria;
			long ix = 0;
			variant_t vtUserName(bstr_t(userName.c_str()));
			SAFEARRAY* pSa = SafeArrayCreateVector(VT_VARIANT, 0, 4);
			ix = 0;
			SafeArrayPutElement(pSa, &ix, &var);
			ix = 1;
			if ((iSchemaUsage > 0) && (_T("oracle") == dbmsName))
				SafeArrayPutElement(pSa, &ix, &vtUserName);
			else
				SafeArrayPutElement(pSa, &ix, &var);
			ix = 2;
			SafeArrayPutElement(pSa, &ix, &var);
			ix = 3;
			SafeArrayPutElement(pSa, &ix, &var);
			vtCriteria.vt = VT_ARRAY | VT_VARIANT;
			vtCriteria.parray = pSa;

			RecordSet rs =
				this->conn->OpenSchema(adSchemaTables, vtCriteria);
			while (!rs.eof())
			{
				Fields fields = rs->GetFields();
				_W(string) tbType = fields[_T("TABLE_TYPE")].valAsString();
				if (_T("TABLE") == tbType)
				{
					_W(string) tbName = fields[_T("TABLE_NAME")].valAsString();
					re.insert(tbName);
				}
				rs->MoveNext();
			}

			/*
			try
			{
				ADOX::_CatalogPtr m_pCatalog = NULL;
				HRESULT hr = m_pCatalog.CreateInstance(__uuidof (ADOX::Catalog));
				if (FAILED(hr))
				{
					_com_issue_error(hr);
				}
				else
				{
					m_pCatalog->PutActiveConnection(this->conn.GetInterfacePtr());
					ADOX::TablesPtr tbs = m_pCatalog->Tables;
					long count = tbs->GetCount();
					for(long i = 0; i < count; ++i)
					{
						ADOX::_TablePtr tb = tbs->GetItem(i);
						if(_W(string)("TABLE") == tb->GetType().operator LPCSTR())
						{
							bstr_t name = tb->GetName();
							re.push_back(name.operator LPCSTR());
						}
					}
					m_pCatalog.Release();
				}
			}catch(...){}
			*/
			return re;
		}

		map<_W(string), _W(string)>& ADOConnection::getProperties()
		{
			if (this->properties.empty())
			{
				this->getProperties(this->properties);
			}
			return this->properties;
		}

		_W(string) ADOConnection::getProperty(_W(string) propName)
		{
			_W(string) re;

			if (this->properties.empty())
			{
				this->getProperties(this->properties);
			}

			map<_W(string), _W(string)>::iterator it = this->properties.find(propName);
			if (it != this->properties.end())
			{
				return it->second;
			}
			return re;
		}

		void ADOConnection::getProperties(map<_W(string), _W(string)>& properties)
		{
			::PropertiesPtr props = this->conn->Properties;
			for (long i = 0; i < props->Count; ++i)
			{
				bstr_t name = props->Item[i]->Name;
				bstr_t val = props->Item[i]->Value;
				properties.insert(make_pair(ADOObject::bstrToStr(name), ADOObject::bstrToStr(val)));
			}
		}

		void ADOConnection::setAccessConnStr(_W(string) dbFilePath, _W(string) password)
		{
			_W(string) connStr = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=\"")
				+ dbFilePath + _T("\";Persist Security Info=False;Jet OLEDB:Database Password=") + password;
			this->conn->ConnectionString = connStr.c_str();
		}

		void ADOConnection::setODBCConnStr(_W(string) dataSourceName, _W(string) dbName, _W(string) userId, _W(string) password)
		{
			_W(string) connStr = _T("Provider=MSDASQL.1;Password=") + password
				+ _T(";Persist Security Info=True;User ID=") + userId
				+ _T(";Data Source=") + dataSourceName + _T(";Initial Catalog=") + dbName;
			this->conn->ConnectionString = connStr.c_str();
		}

		void ADOConnection::setSQLServerConnStr(_W(string) serverName,
			_W(string) dbName, _W(string) userId, _W(string) password)
		{
			_W(string) connStr = _T("Provider=SQLOLEDB.1;Password=") + password
				+ _T(";Persist Security Info=True;User ID=") + userId
				+ _T(";Initial Catalog=") + dbName + _T(";Data Source=") + serverName;
			this->conn->ConnectionString = connStr.c_str();
		}

		void ADOConnection::setMYSQLConnStr(_W(string) serverName,
			_W(string) dbName, _W(string) userId, _W(string) password,
			_W(string) port, _W(string) driverName)
		{
			_W(string) connStr = _T("Provider=MSDASQL.1;Persist Security Info=True;Extended Properties=\"DRIVER={")
				+ driverName + _T("};UID=") + userId + _T(";PD=") + password + _T(";SERVER=") + serverName
				+ _T(";DATABASE=") + dbName + _T(";PORT=") + port + _T(";COLUMN_SIZE_S32=1;\"");
			this->conn->ConnectionString = connStr.c_str();
		}

		void ADOConnection::setORACLEConnStr(_W(string) serverName,
			_W(string) serviceName, _W(string) userId, _W(string) password, _W(string) port)
		{
			_W(string) connStr = _T("Provider=OraOLEDB.Oracle.1;Password=") + password + _T(";User ID=") + userId
				+ _T(";Data Source=\"(DESCRIPTION =(ADDRESS_LIST =(ADDRESS = (PROTOCOL = TCP)(HOST = (") + serverName
				+ _T(")(PORT = ") + port + _T(")))(CONNECT_DATA =(SERVICE_NAME = ") + serviceName + _T(")))\"");
			this->conn->ConnectionString = connStr.c_str();
		}

		_W(string) ADOConnection::promptDataSource(HWND hndParent)
		{
			_W(string) connStr;
			MSDASC::IDataSourceLocatorPtr dlPrompt;
			HRESULT re = dlPrompt.CreateInstance(__uuidof(DataLinks));
			_ConnectionPtr p_conn;
			if (SUCCEEDED(re))
			{
				dlPrompt->hWnd = (long)hndParent;
				p_conn = dlPrompt->PromptNew();
				if (p_conn)
				{
					bstr_t str = p_conn->ConnectionString;
					if (str.length() > 0)
					{
						connStr = (const TCHAR*)str;
					}
					p_conn.Release();
				}
				dlPrompt.Release();
			}
			return connStr;
		}

		bool ADOConnection::promptDataSource(ADOConnection& conn, HWND hndParent)
		{
			MSDASC::IDataSourceLocatorPtr dlPrompt;
			HRESULT re = dlPrompt.CreateInstance(__uuidof(DataLinks));
			_ConnectionPtr p_conn = conn;
			if (SUCCEEDED(re))
			{
				dlPrompt->hWnd = (long)hndParent;
				_ConnectionPtr::Interface* ip = p_conn.GetInterfacePtr();
				if (ip)
				{
					return dlPrompt->PromptEdit((IDispatch**)&ip);
				}
				dlPrompt.Release();
			}
			return false;
		}

		//===========================================================

		_W(string) ColumnInfo::getTableCatalog()
		{
			return this->TABLE_CATALOG;
		}

		void ColumnInfo::setTableCatalog(_W(string) TableCatalog)
		{
			this->TABLE_CATALOG = TableCatalog;
		}

		_W(string) ColumnInfo::getTableSchema()
		{
			return this->TABLE_SCHEMA;
		}

		void ColumnInfo::setTableSchema(_W(string) TableSchema)
		{
			this->TABLE_SCHEMA = TableSchema;
		}

		_W(string) ColumnInfo::getTableName()
		{
			return this->TABLE_NAME;
		}

		void ColumnInfo::setTableName(_W(string) TableName)
		{
			this->TABLE_NAME = TableName;
		}

		_W(string) ColumnInfo::getColumnName()
		{
			return this->COLUMN_NAME;
		}

		void ColumnInfo::setColumnName(_W(string) ColumnName)
		{
			this->COLUMN_NAME = ColumnName;
		}

		_W(string) ColumnInfo::getColumnGuid()
		{
			return this->COLUMN_GUID;
		}

		void ColumnInfo::setColumnGuid(_W(string) ColumnGuid)
		{
			this->COLUMN_GUID = ColumnGuid;
		}

		_W(string) ColumnInfo::getColumnPropid()
		{
			return this->COLUMN_PROPID;
		}

		void ColumnInfo::setColumnPropid(_W(string) ColumnPropid)
		{
			this->COLUMN_PROPID = ColumnPropid;
		}

		long ColumnInfo::getOrdinalPosition()
		{
			return this->ORDINAL_POSITION;
		}

		void ColumnInfo::setOrdinalPosition(long OrdinalPosition)
		{
			this->ORDINAL_POSITION = OrdinalPosition;
		}

		_W(string) ColumnInfo::getColumnHasdefault()
		{
			return this->COLUMN_HASDEFAULT;
		}

		void ColumnInfo::setColumnHasdefault(_W(string) ColumnHasdefault)
		{
			this->COLUMN_HASDEFAULT = ColumnHasdefault;
		}

		_W(string) ColumnInfo::getColumnDefault()
		{
			return this->COLUMN_DEFAULT;
		}

		void ColumnInfo::setColumnDefault(_W(string) ColumnDefault)
		{
			this->COLUMN_DEFAULT = ColumnDefault;
		}

		_W(string) ColumnInfo::getColumnFlags()
		{
			return this->COLUMN_FLAGS;
		}

		void ColumnInfo::setColumnFlags(_W(string) ColumnFlags)
		{
			this->COLUMN_FLAGS = ColumnFlags;
		}

		bool ColumnInfo::getIsNullable()
		{
			return this->IS_NULLABLE;
		}

		void ColumnInfo::setIsNullable(bool IsNullable)
		{
			this->IS_NULLABLE = IsNullable;
		}

		long ColumnInfo::getDataType()
		{
			return this->DATA_TYPE;
		}

		void ColumnInfo::setDataType(long DataType)
		{
			this->DATA_TYPE = DataType;
		}

		_W(string) ColumnInfo::getTypeGuid()
		{
			return this->TYPE_GUID;
		}

		void ColumnInfo::setTypeGuid(_W(string) TypeGuid)
		{
			this->TYPE_GUID = TypeGuid;
		}

		long ColumnInfo::getCharacterMaximumLength()
		{
			return this->CHARACTER_MAXIMUM_LENGTH;
		}

		void ColumnInfo::setCharacterMaximumLength(long CharacterMaximumLength)
		{
			this->CHARACTER_MAXIMUM_LENGTH = CharacterMaximumLength;
		}

		long ColumnInfo::getCharacterOctetLength()
		{
			return this->CHARACTER_OCTET_LENGTH;
		}

		void ColumnInfo::setCharacterOctetLength(long CharacterOctetLength)
		{
			this->CHARACTER_OCTET_LENGTH = CharacterOctetLength;
		}

		long ColumnInfo::getNumericPrecision()
		{
			return this->NUMERIC_PRECISION;
		}

		void ColumnInfo::setNumericPrecision(long NumericPrecision)
		{
			this->NUMERIC_PRECISION = NumericPrecision;
		}

		long ColumnInfo::getNumericScale()
		{
			return this->NUMERIC_SCALE;
		}

		void ColumnInfo::setNumericScale(long NumericScale)
		{
			this->NUMERIC_SCALE = NumericScale;
		}

		long ColumnInfo::getDatetimePrecision()
		{
			return this->DATETIME_PRECISION;
		}

		void ColumnInfo::setDatetimePrecision(long DatetimePrecision)
		{
			this->DATETIME_PRECISION = DatetimePrecision;
		}

		_W(string) ColumnInfo::getCharacterSetCatalog()
		{
			return this->CHARACTER_SET_CATALOG;
		}

		void ColumnInfo::setCharacterSetCatalog(_W(string) CharacterSetCatalog)
		{
			this->CHARACTER_SET_CATALOG = CharacterSetCatalog;
		}

		_W(string) ColumnInfo::getCharacterSetSchema()
		{
			return this->CHARACTER_SET_SCHEMA;
		}

		void ColumnInfo::setCharacterSetSchema(_W(string) CharacterSetSchema)
		{
			this->CHARACTER_SET_SCHEMA = CharacterSetSchema;
		}

		_W(string) ColumnInfo::getCharacterSetName()
		{
			return this->CHARACTER_SET_NAME;
		}

		void ColumnInfo::setCharacterSetName(_W(string) CharacterSetName)
		{
			this->CHARACTER_SET_NAME = CharacterSetName;
		}

		_W(string) ColumnInfo::getCollationCatalog()
		{
			return this->COLLATION_CATALOG;
		}

		void ColumnInfo::setCollationCatalog(_W(string) CollationCatalog)
		{
			this->COLLATION_CATALOG = CollationCatalog;
		}

		_W(string) ColumnInfo::getCollationSchema()
		{
			return this->COLLATION_SCHEMA;
		}

		void ColumnInfo::setCollationSchema(_W(string) CollationSchema)
		{
			this->COLLATION_SCHEMA = CollationSchema;
		}

		_W(string) ColumnInfo::getCollationName()
		{
			return this->COLLATION_NAME;
		}

		void ColumnInfo::setCollationName(_W(string) CollationName)
		{
			this->COLLATION_NAME = CollationName;
		}

		_W(string) ColumnInfo::getDomainCatalog()
		{
			return this->DOMAIN_CATALOG;
		}

		void ColumnInfo::setDomainCatalog(_W(string) DomainCatalog)
		{
			this->DOMAIN_CATALOG = DomainCatalog;
		}

		_W(string) ColumnInfo::getDomainSchema()
		{
			return this->DOMAIN_SCHEMA;
		}

		void ColumnInfo::setDomainSchema(_W(string) DomainSchema)
		{
			this->DOMAIN_SCHEMA = DomainSchema;
		}

		_W(string) ColumnInfo::getDomainName()
		{
			return this->DOMAIN_NAME;
		}

		void ColumnInfo::setDomainName(_W(string) DomainName)
		{
			this->DOMAIN_NAME = DomainName;
		}

		_W(string) ColumnInfo::getDescription()
		{
			return this->DESCRIPTION;
		}

		void ColumnInfo::setDescription(_W(string) Description)
		{
			this->DESCRIPTION = Description;
		}

		_W(string) ColumnInfo::getColumnLcid()
		{
			return this->COLUMN_LCID;
		}

		void ColumnInfo::setColumnLcid(_W(string) ColumnLcid)
		{
			this->COLUMN_LCID = ColumnLcid;
		}

		_W(string) ColumnInfo::getColumnCompflags()
		{
			return this->COLUMN_COMPFLAGS;
		}

		void ColumnInfo::setColumnCompflags(_W(string) ColumnCompflags)
		{
			this->COLUMN_COMPFLAGS = ColumnCompflags;
		}

		_W(string) ColumnInfo::getColumnSortid()
		{
			return this->COLUMN_SORTID;
		}

		void ColumnInfo::setColumnSortid(_W(string) ColumnSortid)
		{
			this->COLUMN_SORTID = ColumnSortid;
		}

		_W(string) ColumnInfo::getColumnTdscollation()
		{
			return this->COLUMN_TDSCOLLATION;
		}

		void ColumnInfo::setColumnTdscollation(_W(string) ColumnTdscollation)
		{
			this->COLUMN_TDSCOLLATION = ColumnTdscollation;
		}

		bool ColumnInfo::getIsComputed()
		{
			return this->IS_COMPUTED;
		}

		void ColumnInfo::setIsComputed(bool IsComputed)
		{
			this->IS_COMPUTED = IsComputed;
		}

	}
}