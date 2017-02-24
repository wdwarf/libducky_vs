#include "stdafx.h"
#include "libducky/db/ADOCommand.h"
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/xpressive/xpressive_dynamic.hpp>

using namespace std;

namespace ducky
{
	namespace db {
		ADOParameter::ADOParameter(void)
		{
			this->param.CreateInstance(__uuidof(Parameter));
		}

		ADOParameter::ADOParameter(_ParameterPtr param)
		{
			this->param = param;
		}

		ADOParameter::~ADOParameter(void)
		{
			if (this->param)
			{
				this->param.Release();
			}
		}

		ADOParameter::operator _ParameterPtr()
		{
			return this->param;
		}

		_ParameterPtr ADOParameter::operator->()
		{
			return this->param;
		}

		ADOParameter& ADOParameter::operator=(_ParameterPtr param)
		{
			this->param = param;
			return *this;
		}

		DataTypeEnum ADOParameter::setTypeAsEmpty()
		{
			this->param->Type = adEmpty;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsTinyInt()
		{
			this->param->Type = adTinyInt;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsSmallInt()
		{
			this->param->Type = adSmallInt;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsInteger()
		{
			this->param->Type = adInteger;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsBigInt()
		{
			this->param->Type = adBigInt;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsUnsignedTinyInt()
		{
			this->param->Type = adUnsignedTinyInt;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsUnsignedSmallInt()
		{
			this->param->Type = adUnsignedSmallInt;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsUnsignedInt()
		{
			this->param->Type = adUnsignedInt;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsUnsignedBigInt()
		{
			this->param->Type = adUnsignedBigInt;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsSingle()
		{
			this->param->Type = adSingle;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsDouble()
		{
			this->param->Type = adDouble;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsCurrency()
		{
			this->param->Type = adCurrency;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsDecimal()
		{
			this->param->Type = adDecimal;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsNumeric()
		{
			this->param->Type = adNumeric;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsBoolean()
		{
			this->param->Type = adBoolean;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsError()
		{
			this->param->Type = adError;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsUserDefined()
		{
			this->param->Type = adUserDefined;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsVariant()
		{
			this->param->Type = adVariant;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsIDispatch()
		{
			this->param->Type = adIDispatch;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsIUnknown()
		{
			this->param->Type = adIUnknown;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsGUID()
		{
			this->param->Type = adGUID;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsDate()
		{
			this->param->Type = adDate;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsDBDate()
		{
			this->param->Type = adDBDate;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsDBTime()
		{
			this->param->Type = adDBTime;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsDBTimeStamp()
		{
			this->param->Type = adDBTimeStamp;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsBSTR()
		{
			this->param->Type = adBSTR;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsChar()
		{
			this->param->Type = adChar;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsVarChar()
		{
			this->param->Type = adVarChar;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsLongVarChar()
		{
			this->param->Type = adLongVarChar;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsWChar()
		{
			this->param->Type = adWChar;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsVarWChar()
		{
			this->param->Type = adVarWChar;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsLongVarWChar()
		{
			this->param->Type = adLongVarWChar;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsBinary()
		{
			this->param->Type = adBinary;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsVarBinary()
		{
			this->param->Type = adVarBinary;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsLongVarBinary()
		{
			this->param->Type = adLongVarBinary;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsChapter()
		{
			this->param->Type = adChapter;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsFileTime()
		{
			this->param->Type = adFileTime;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsPropVariant()
		{
			this->param->Type = adPropVariant;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsVarNumeric()
		{
			this->param->Type = adVarNumeric;
			return this->param->Type;
		}

		DataTypeEnum ADOParameter::setTypeAsArray()
		{
			this->param->Type = adArray;
			return this->param->Type;
		}

		//------------------------------------------------------------------------------------------------

		ADOParameters::ADOParameters(void)
		{
			this->params.CreateInstance(__uuidof(Parameters));
		}

		ADOParameters::~ADOParameters(void)
		{
			if (this->params)
			{
				this->params.Release();
			}
		}

		ADOParameters::ADOParameters(ParametersPtr params)
		{
			this->params = params;
		}

		ADOParameters::operator ParametersPtr()
		{
			return this->params;
		}

		ParametersPtr ADOParameters::operator->()
		{
			return this->params;
		}

		ADOParameter ADOParameters::getParameter(_W(string) paramName)
		{
			return this->params->Item[bstr_t(paramName.c_str())];
		}

		//------------------------------------------------------------------------------------------------

		ADOCommand::ADOCommand(void)
		{
			this->cmd.CreateInstance(__uuidof(Command));
			this->cmd->Prepared = true;
		}

		ADOCommand::ADOCommand(_CommandPtr cmd)
		{
			this->cmd = cmd;
		}

		ADOCommand::~ADOCommand(void)
		{
			if (this->cmd)
			{
				this->cmd.Release();
			}
		}

		ADOCommand& ADOCommand::operator=(_CommandPtr cmd)
		{
			this->cmd = cmd;
			if (cmd)
			{
				this->cmdText = (const TCHAR*)cmd->CommandText;
			}
			this->paranNames.clear();
			return *this;
		}

		void ADOCommand::setConnection(const ADOConnection& conn)
		{
			this->cmd->ActiveConnection = conn;
		}

		ADOParameters ADOCommand::getParameters()
		{
			return this->cmd->Parameters;
		}

		void ADOCommand::setSql(_W(string) sql)
		{
			using namespace boost::xpressive;
			this->cmdText = sql;
			_CommandPtr newCmd;
			newCmd.CreateInstance(__uuidof(Command));
			newCmd->ActiveConnection = this->cmd->ActiveConnection;
			this->cmd.Release();
			this->cmd = newCmd;
			boost::replace_all(this->cmdText, _T("\\:"), _T("#COLON#"));
			_W(string) newCmdText = this->cmdText;
			_W(sregex) reg = _W(sregex)::compile(_T("(:[\\w_]+[\\w_\\d]*)|\\?"), icase);
			_W(smatch) what;
			vector<_W(string)> names;
			_W(string)::const_iterator start, end;
			start = newCmdText.begin();
			end = newCmdText.end();
			while (regex_search(start, end, what, reg))
			{
				names.push_back(what[0]);
				start = what[0].second;
			}

			newCmdText = regex_replace(newCmdText, reg, _T("?"));
			boost::replace_all(newCmdText, _T("#COLON#"), _T(":"));
			boost::replace_all(this->cmdText, _T("#COLON#"), _T(":"));

			this->cmd->CommandText = newCmdText.c_str();

			int n = 0;
			for (vector<_W(string)>::iterator it = names.begin(); it != names.end(); ++it)
			{
				_W(string) name = *it;
				boost::trim(name);
				boost::to_upper(name);
				boost::replace_all(name, _T(":"), _T(""));
				if (_T("?") == name)
				{
					_W(stringstream) s;
					s << n++;
					name = s.str();
				}
				_ParameterPtr param = this->cmd->CreateParameter(name.c_str(), adVarChar, adParamInput, 255);
				this->cmd->Parameters->Append(param);
				param.Release();
			}
		}

		_W(string) ADOCommand::getSql()
		{
			return this->cmdText;
		}

		RecordSet ADOCommand::execute(long* recordsAffected)
		{
			variant_t ra = 0;
			RecordSet rs = this->cmd->Execute(&ra, NULL, 0);
			if (recordsAffected)
			{
				*recordsAffected = ra;
			}
			return rs;
		}

		void ADOCommand::setPrepared(bool prepared)
		{
			this->cmd->Prepared = prepared;
		}

		bool ADOCommand::getPrepared()
		{
			return this->cmd->Prepared;
		}

		ADOConnection ADOCommand::getConnection()
		{
			return this->cmd->GetActiveConnection();
		}

		_CommandPtr ADOCommand::operator->()
		{
			return this->cmd;
		}

		ADOCommand::operator _CommandPtr()
		{
			return this->cmd;
		}

	}
}