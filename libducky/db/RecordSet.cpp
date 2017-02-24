#include "stdafx.h"
#include "libducky/db/RecordSet.h"

namespace ducky {
	namespace db {

		RecordSet::RecordSet(void)
		{
			this->rs.CreateInstance(__uuidof(Recordset));
			this->cursorType = adOpenStatic;
			this->lockType = adLockOptimistic;
			this->options = adCmdText;
		}

		RecordSet::RecordSet(_RecordsetPtr rs)
		{
			this->operator=(rs);
		}

		RecordSet::~RecordSet(void)
		{
			if (this->rs) this->rs.Release();
		}

		void RecordSet::first()
		{
			this->rs->MoveFirst();
		}

		void RecordSet::next()
		{
			this->rs->MoveNext();
		}

		void RecordSet::last()
		{
			this->rs->MoveLast();
		}

		void RecordSet::prev()
		{
			this->rs->MovePrevious();
		}

		Fields RecordSet::getFields()
		{
			return this->rs->GetFields();
		}

		bool RecordSet::bof()
		{
			return 0 != this->rs->GetBOF();
		}

		bool RecordSet::eof()
		{
			return 0 != this->rs->GetwADOEOF();
		}

		void RecordSet::del()
		{
			this->rs->Delete(adAffectCurrent);
			this->rs->UpdateBatch(adAffectCurrent);
		}

		long RecordSet::getRecordCount()
		{
			return this->rs->GetRecordCount();
		}

		void RecordSet::operator =(_RecordsetPtr rs)
		{
			this->rs = rs;
		}

		RecordSet::operator _RecordsetPtr()
		{
			return this->rs;
		}

		RecordSet::operator bool()
		{
			return !(this->eof() || this->bof());
		}

		void RecordSet::open()
		{
			this->rs->Open(sql, this->rs->GetActiveConnection(),
				this->cursorType, this->lockType, this->options);
		}

		void RecordSet::close()
		{
			this->rs->Close();
		}

		void RecordSet::setConnection(const ADOConnection& conn)
		{
			this->rs->PutActiveConnection(
				conn.operator _ConnectionPtr().GetInterfacePtr());
		}

		ADOConnection RecordSet::getConnection()
		{
			return (_ConnectionPtr)this->rs->GetActiveConnection();
		}

		void RecordSet::setSql(_W(string) sql)
		{
			this->sql = sql.c_str();
		}

		_W(string) RecordSet::getSql()
		{
			return (const TCHAR*)this->sql;
		}

		void RecordSet::setCursorType(CursorTypeEnum cursorType)
		{
			this->cursorType = cursorType;
		}

		void RecordSet::setLockType(LockTypeEnum lockType)
		{
			this->lockType = lockType;
		}

		void RecordSet::setOptions(long options)
		{
			this->options = options;
		}

		_RecordsetPtr& RecordSet::operator->()
		{
			return this->rs;
		}
	}
}