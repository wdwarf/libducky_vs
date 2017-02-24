#include "stdafx.h"
#include "libducky/db/Fields.h"

namespace ducky {
	namespace db {

		Fields::Fields(void)
		{
			this->fields.CreateInstance(__uuidof(::Fields));
		}

		Fields::~Fields(void)
		{
			if (this->fields) this->fields.Release();
		}

		Fields::Fields(const Fields& fields)
		{
			this->fields = fields.fields;
		}

		Fields::Fields(FieldsPtr& fields)
		{
			this->fields = fields;
		}

		void Fields::operator =(const Fields& fields)
		{
			this->fields = fields.fields;
		}

		void Fields::operator =(const FieldsPtr& fields)
		{
			this->fields = fields;
		}

		Fields::operator FieldsPtr()
		{
			return this->fields;
		}

		Field Fields::operator[](_W(string) fieldName)
		{
			return this->fields->GetItem(fieldName.c_str());
		}

		Field Fields::operator[](long fieldIndex)
		{
			return this->fields->GetItem(fieldIndex);
		}

		FieldsPtr& Fields::operator->()
		{
			return this->fields;
		}

		long Fields::getFieldCount()
		{
			return this->fields->GetCount();
		}

		vector<_W(string)> Fields::getFieldNames()
		{
			vector<_W(string)> re;
			long fieldCount = this->fields->GetCount();
			for (long i = 0; i < fieldCount; ++i)
			{
				bstr_t name = this->fields->GetItem(i)->GetName();
				if (name.length() > 0)
				{
					re.push_back((const TCHAR*)name);
				}
			}
			return re;
		}

	}
}