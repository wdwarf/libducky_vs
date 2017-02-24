#ifndef LIBDUCKY_DB_ADOCommand_H_
#define LIBDUCKY_DB_ADOCommand_H_

#include "libducky_config.h"
#include "ADOObject.h"
#include "ADOConnection.h"
#include "RecordSet.h"
#include "Fields.h"
#include <string>

using std::_W(string);

/*
	//ʹ�÷���
	//
	//ע��windows 2000���°汾��ϵͳ���ܲ�֧�֡�
	//
	//��ʹ�ô�����������SQL��䣺
	//1��ʹ��?����ʱ�������Զ����±�����
	//2��ʹ����������ʱ��д��Ϊ ":ParamName"

	ADOCommand cmd;
	cmd.setConnection(conn);
	cmd.setSql("select * from TDMTRealtimeData where equeid=? or id=? or CreateDate=:CreateDate");
	cmd["0"].setTypeAsBSTR();	//��һ���ʺ�
	cmd["0"]->Value = "��AA-T01180";
	cmd["1"]->Value = "0ABDC216-37B7-4665-B187-0005B0FB36D8";	//�ڶ����ʺ�
	ADOParameter p = cmd["CreateDate"];		//����:CreateDate
	p->Value = "2014-05-01 01:00:00";
	RecordSet rs = cmd.execute();
	cout << rs.getFields()["equeid"].valAsString() << endl;
	rs.close();
*/

namespace ducky
{
	namespace db {
		class ADOParameter :
			public ADOObject
		{
		public:
			ADOParameter(void);
			ADOParameter(_ParameterPtr param);
			virtual ~ADOParameter(void);

			operator _ParameterPtr();
			_ParameterPtr operator->();
			ADOParameter& operator=(_ParameterPtr param);

			DataTypeEnum setTypeAsEmpty();
			DataTypeEnum setTypeAsTinyInt();
			DataTypeEnum setTypeAsSmallInt();
			DataTypeEnum setTypeAsInteger();
			DataTypeEnum setTypeAsBigInt();
			DataTypeEnum setTypeAsUnsignedTinyInt();
			DataTypeEnum setTypeAsUnsignedSmallInt();
			DataTypeEnum setTypeAsUnsignedInt();
			DataTypeEnum setTypeAsUnsignedBigInt();
			DataTypeEnum setTypeAsSingle();
			DataTypeEnum setTypeAsDouble();
			DataTypeEnum setTypeAsCurrency();
			DataTypeEnum setTypeAsDecimal();
			DataTypeEnum setTypeAsNumeric();
			DataTypeEnum setTypeAsBoolean();
			DataTypeEnum setTypeAsError();
			DataTypeEnum setTypeAsUserDefined();
			DataTypeEnum setTypeAsVariant();
			DataTypeEnum setTypeAsIDispatch();
			DataTypeEnum setTypeAsIUnknown();
			DataTypeEnum setTypeAsGUID();
			DataTypeEnum setTypeAsDate();
			DataTypeEnum setTypeAsDBDate();
			DataTypeEnum setTypeAsDBTime();
			DataTypeEnum setTypeAsDBTimeStamp();
			DataTypeEnum setTypeAsBSTR();
			DataTypeEnum setTypeAsChar();
			DataTypeEnum setTypeAsVarChar();
			DataTypeEnum setTypeAsLongVarChar();
			DataTypeEnum setTypeAsWChar();
			DataTypeEnum setTypeAsVarWChar();
			DataTypeEnum setTypeAsLongVarWChar();
			DataTypeEnum setTypeAsBinary();
			DataTypeEnum setTypeAsVarBinary();
			DataTypeEnum setTypeAsLongVarBinary();
			DataTypeEnum setTypeAsChapter();
			DataTypeEnum setTypeAsFileTime();
			DataTypeEnum setTypeAsPropVariant();
			DataTypeEnum setTypeAsVarNumeric();
			DataTypeEnum setTypeAsArray();

		private:
			_ParameterPtr param;
		};

		class ADOParameters :
			public ADOObject
		{
		public:
			ADOParameters(void);
			ADOParameters(ParametersPtr params);
			virtual ~ADOParameters(void);

			operator ParametersPtr();
			ParametersPtr operator->();
			ADOParameter getParameter(_W(string) paramName);

			template<class T>
			ADOParameter operator[](T  paramName)
			{
				WStringstream str;
				str << paramName;
				return this->params->Item[str.str().c_str()];
			}

		private:
			ParametersPtr params;
		};

		class ADOCommand :
			public ADOObject
		{
		public:
			ADOCommand(void);
			ADOCommand(_CommandPtr cmd);
			virtual ~ADOCommand(void);

			RecordSet Execute();
			void setConnection(const ADOConnection& conn);
			ADOConnection getConnection();

			void setPrepared(bool prepared);
			bool getPrepared();
			void setSql(_W(string) sql);
			_W(string) getSql();
			RecordSet execute(long* recordsAffected = NULL);

			operator _CommandPtr();
			_CommandPtr operator->();
			ADOCommand& operator=(_CommandPtr cmd);

			ADOParameters getParameters();

			template<class T>
			ADOParameter operator[](T  paramName)
			{
				_W(stringstream) str;
				str << paramName;
				return this->cmd->Parameters->Item[str.str().c_str()];
			}

		private:
			_CommandPtr cmd;
			_W(string) cmdText;
			vector<_W(string)> paranNames;
		};

	}
}

#endif