#ifndef LIBDUCKY_DB_ADOConnection_H_
#define LIBDUCKY_DB_ADOConnection_H_

#include "libducky_config.h"
#include "ADOObject.h"
#include <string>
#include <vector>
#include <map>
#include <set>

using std::vector;
using std::map;
using std::set;

namespace ducky
{
	namespace db {
		class ColumnInfo
		{
		public:
			ColumnInfo() : isPrimaryKey(false) {};

			_W(string) getTableCatalog();
			void setTableCatalog(_W(string) TableCatalog);
			_W(string) getTableSchema();
			void setTableSchema(_W(string) TableSchema);
			_W(string) getTableName();
			void setTableName(_W(string) TableName);
			_W(string) getColumnName();
			void setColumnName(_W(string) ColumnName);
			_W(string) getColumnGuid();
			void setColumnGuid(_W(string) ColumnGuid);
			_W(string) getColumnPropid();
			void setColumnPropid(_W(string) ColumnPropid);
			long getOrdinalPosition();
			void setOrdinalPosition(long OrdinalPosition);
			_W(string) getColumnHasdefault();
			void setColumnHasdefault(_W(string) ColumnHasdefault);
			_W(string) getColumnDefault();
			void setColumnDefault(_W(string) ColumnDefault);
			_W(string) getColumnFlags();
			void setColumnFlags(_W(string) ColumnFlags);
			bool getIsNullable();
			void setIsNullable(bool IsNullable);
			long getDataType();
			void setDataType(long DataType);
			_W(string) getTypeGuid();
			void setTypeGuid(_W(string) TypeGuid);
			long getCharacterMaximumLength();
			void setCharacterMaximumLength(long CharacterMaximumLength);
			long getCharacterOctetLength();
			void setCharacterOctetLength(long CharacterOctetLength);
			long getNumericPrecision();
			void setNumericPrecision(long NumericPrecision);
			long getNumericScale();
			void setNumericScale(long NumericScale);
			long getDatetimePrecision();
			void setDatetimePrecision(long DatetimePrecision);
			_W(string) getCharacterSetCatalog();
			void setCharacterSetCatalog(_W(string) CharacterSetCatalog);
			_W(string) getCharacterSetSchema();
			void setCharacterSetSchema(_W(string) CharacterSetSchema);
			_W(string) getCharacterSetName();
			void setCharacterSetName(_W(string) CharacterSetName);
			_W(string) getCollationCatalog();
			void setCollationCatalog(_W(string) CollationCatalog);
			_W(string) getCollationSchema();
			void setCollationSchema(_W(string) CollationSchema);
			_W(string) getCollationName();
			void setCollationName(_W(string) CollationName);
			_W(string) getDomainCatalog();
			void setDomainCatalog(_W(string) DomainCatalog);
			_W(string) getDomainSchema();
			void setDomainSchema(_W(string) DomainSchema);
			_W(string) getDomainName();
			void setDomainName(_W(string) DomainName);
			_W(string) getDescription();
			void setDescription(_W(string) Description);
			_W(string) getColumnLcid();
			void setColumnLcid(_W(string) ColumnLcid);
			_W(string) getColumnCompflags();
			void setColumnCompflags(_W(string) ColumnCompflags);
			_W(string) getColumnSortid();
			void setColumnSortid(_W(string) ColumnSortid);
			_W(string) getColumnTdscollation();
			void setColumnTdscollation(_W(string) ColumnTdscollation);
			bool getIsComputed();
			void setIsComputed(bool isComputed);

			static _W(string) getColumnDataTypeName(long iDataType);

		private:
			_W(string) TABLE_CATALOG;
			_W(string) TABLE_SCHEMA;
			_W(string) TABLE_NAME;
			_W(string) COLUMN_NAME;
			_W(string) COLUMN_GUID;
			_W(string) COLUMN_PROPID;
			long ORDINAL_POSITION;
			_W(string) COLUMN_HASDEFAULT;
			_W(string) COLUMN_DEFAULT;
			_W(string) COLUMN_FLAGS;
			bool IS_NULLABLE;
			long DATA_TYPE;
			_W(string) TYPE_GUID;
			long CHARACTER_MAXIMUM_LENGTH;
			long CHARACTER_OCTET_LENGTH;
			long NUMERIC_PRECISION;
			long NUMERIC_SCALE;
			long DATETIME_PRECISION;
			_W(string) CHARACTER_SET_CATALOG;
			_W(string) CHARACTER_SET_SCHEMA;
			_W(string) CHARACTER_SET_NAME;
			_W(string) COLLATION_CATALOG;
			_W(string) COLLATION_SCHEMA;
			_W(string) COLLATION_NAME;
			_W(string) DOMAIN_CATALOG;
			_W(string) DOMAIN_SCHEMA;
			_W(string) DOMAIN_NAME;
			_W(string) DESCRIPTION;
			_W(string) COLUMN_LCID;
			_W(string) COLUMN_COMPFLAGS;
			_W(string) COLUMN_SORTID;
			_W(string) COLUMN_TDSCOLLATION;
			bool IS_COMPUTED;
			bool isPrimaryKey;
		};

		class TableInfo
		{
		public:
			TableInfo() {};
			TableInfo(_W(string) tbName) : name(tbName) {}

			const _W(string)& getCatalog() const {
				return catalog;
			}

			void setCatalog(const _W(string)& catalog) {
				this->catalog = catalog;
			}

			const _W(string)& getCreatedDate() const {
				return createdDate;
			}

			void setCreatedDate(const _W(string)& createdDate) {
				this->createdDate = createdDate;
			}

			const _W(string)& getDescription() const {
				return description;
			}

			void setDescription(const _W(string)& description) {
				this->description = description;
			}

			const _W(string)& getGuid() const {
				return guid;
			}

			void setGuid(const _W(string)& guid) {
				this->guid = guid;
			}

			const _W(string)& getModifiedDate() const {
				return modifiedDate;
			}

			void setModifiedDate(const _W(string)& modifiedDate) {
				this->modifiedDate = modifiedDate;
			}

			const _W(string)& getName() const {
				return name;
			}

			void setName(const _W(string)& name) {
				this->name = name;
			}

			const _W(string)& getPropid() const {
				return propid;
			}

			void setPropid(const _W(string)& propid) {
				this->propid = propid;
			}

			const _W(string)& getSchema() const {
				return schema;
			}

			void setSchema(const _W(string)& schema) {
				this->schema = schema;
			}

			const _W(string)& getType() const {
				return type;
			}

			void setType(const _W(string)& type) {
				this->type = type;
			}

			map<_W(string), ColumnInfo>& getColumnInfos()
			{
				return this->columns;
			}
			void setColumnInfos(map<_W(string), ColumnInfo>& columns)
			{
				this->columns = columns;
			}

			set<_W(string)>& getPrimaryKeys()
			{
				return this->primaryKeys;
			}
			void setPrimaryKeys(set<_W(string)>& v)
			{
				this->primaryKeys = v;
			}

		private:
			_W(string) name;
			_W(string) catalog;
			_W(string) schema;
			_W(string) type;
			_W(string) guid;
			_W(string) description;
			_W(string) propid;
			_W(string) createdDate;
			_W(string) modifiedDate;
			set<_W(string)> primaryKeys;
			map<_W(string), ColumnInfo> columns;
		};

		class ADOConnection :
			public ADOObject
		{
		public:
			ADOConnection(void);
			ADOConnection(const _ConnectionPtr& conn);
			void operator =(const ADOConnection& conn);
			void operator =(const _ConnectionPtr& conn);
			operator bool();
			virtual ~ADOConnection(void);
			virtual void Open();
			virtual void Close();
			virtual bool IsConnected() const;
			operator _ConnectionPtr() const;
			operator _Connection&() const;
			operator _Connection*() const;
			_ConnectionPtr& operator->();

			void setAccessConnStr(_W(string) dbFilePath, _W(string) password = _T(""));
			void setODBCConnStr(_W(string) dataSourceName, _W(string) dbName, _W(string) userId = _T(""), _W(string) password = _T(""));
			void setSQLServerConnStr(_W(string) serverName,
				_W(string) dbName, _W(string) userId, _W(string) password);
			void setMYSQLConnStr(_W(string) serverName,
				_W(string) dbName, _W(string) userId, _W(string) password,
				_W(string) port = _T("3306"), _W(string) driverName = _T("MySQL ODBC 5.2 ANSI Driver"));
			void ADOConnection::setORACLEConnStr(_W(string) serverName,
				_W(string) serviceName, _W(string) userId, _W(string) password, _W(string) port = _T("1521"));

			void setConnectString(_W(string) connStr);
			_W(string) getConnectionString() const;
			void setUserId(_W(string) userId);
			_W(string) getUserId() const;
			void setPassword(_W(string) pass);
			_W(string) getPassword() const;
			void setOptions(long opt);
			long getOptions() const;
			void setConnectTimeout(long connTimeout);
			long getConnectTimeout() const;
			void setCursorLocation(CursorLocationEnum cl);
			CursorLocationEnum getCursorLocation() const;
			void setConnectMode(ConnectModeEnum mode);
			ConnectModeEnum getConnectMode() const;
			_W(string) getDatabaseName() const;
			map<_W(string), TableInfo> getTableInfos();
			TableInfo getTableInfo(_W(string) tableName);
			set<_W(string)> getTableNames();
			set<_W(string)> getPrimaryKeys(_W(string) tableName,
				_W(string) tableSchema = _T(""), _W(string) tableCatalog = _T(""));
			map<_W(string), _W(string)>& getProperties();
			_W(string) getProperty(_W(string) propName);

			long getState() const;

			void BeginTrans();
			void CommitTrans();
			void RollbackTrans();

			//Build Data Source Dialog
			static _W(string) promptDataSource(HWND hWndParent = NULL);
			static bool promptDataSource(
				ADOConnection& conn, HWND hWndParent = NULL);

		private:
			//bstr_t connStr;
			bstr_t userId;
			bstr_t password;
			map<_W(string), _W(string)> properties;
			void getProperties(map<_W(string), _W(string)>& properties);
			long options;
			long connTimeout;
			_ConnectionPtr conn;
		};

	}
}
#endif