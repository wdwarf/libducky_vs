#include "stdafx.h"
#include "libducky/db/Field.h"
#include <ctime>

namespace ducky {
	namespace db {

		Field::Field(void)
		{
			this->field.CreateInstance(__uuidof(::Field));
		}

		Field::Field(const Field& field)
		{
			this->operator =(field.field);
		}

		Field::~Field(void)
		{
			if (this->field) this->field.Release();
		}

		Field::Field(FieldPtr& field)
		{
			this->field = field;
		}

		void Field::operator =(const Field& field)
		{
			this->field = field.field;
		}

		void Field::operator =(const FieldPtr& field)
		{
			this->field = field;
		}

		Field::operator FieldPtr()
		{
			return this->field;
		}

		variant_t Field::getValue()
		{
			return this->field->GetValue();
		}

		_W(string) Field::valAsString()
		{
			variant_t val = this->field->GetValue();
			if (VT_NULL != val.vt)
			{
				return (const TCHAR*)bstr_t(val);
			}
			return _T("");
		}

		long Field::valAsInt()
		{
			variant_t val = this->field->GetValue();
			if (VT_NULL != val.vt)
			{
				return val;
			}
			return 0;
		}

		double Field::valAsFloat()
		{
			variant_t val = this->field->GetValue();
			if (VT_NULL != val.vt)
			{
				return val;
			}
			return 0.f;
		}

		time_t Field::valAsTime()
		{
			variant_t val = this->field->GetValue();
			if (VT_NULL != val.vt && VT_DATE == val.vt)
			{
				double date = val;
				long tz = 0;
				_get_timezone(&tz);
				//»ò
				//tz = timezone;
				return (time_t)((date - 25569) * 86400 + tz + 0.5);
			}
			return 0;
		}

		FieldPtr& Field::operator->()
		{
			return this->field;
		}

		_W(string) Field::getName()
		{
			return (const TCHAR*)this->field->GetName();
		}

		Field::operator long()
		{
			return this->valAsInt();
		}

		Field::operator double()
		{
			return this->valAsFloat();
		}

		Field::operator bool()
		{
			return bool(this->valAsInt());
		}

		Field::operator _W(string)()
		{
			return this->valAsString();
		}

		Field::operator variant_t()
		{
			return this->getValue();
		}

		Field::operator time_t()
		{
			return this->valAsTime();
		}

	}
}