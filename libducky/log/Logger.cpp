#include "stdafx.h"
#include <libducky/log/Logger.h>
#include "log4cpp/Category.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/PatternLayout.hh"
#include "log4cpp/Priority.hh"
#include "log4cpp/PropertyConfigurator.hh"
#include <ctime>
#include <boost/smart_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <comutil.h>

namespace fs = boost::filesystem;
using namespace std;

#ifdef _MSC_VER
#include <libducky/file/FileMgr.h>
#include <comdef.h>

using namespace ducky::file;
#ifdef _DEBUG

#if _MSC_VER == 1900
#pragma comment(lib, "log4cppLIBD_VC140.lib")
#elif _MSC_VER == 1800
#pragma comment(lib, "log4cppLIBD_VC120.lib")
#elif _MSC_VER == 1700
#pragma comment(lib, "log4cppLIBD_VC110.lib")
#elif _MSC_VER == 1600
#pragma comment(lib, "log4cppLIBD_VC100.lib")
#endif

#else

#if _MSC_VER == 1900
#pragma comment(lib, "log4cppLIB_VC140.lib")
#elif _MSC_VER == 1800
#pragma comment(lib, "log4cppLIB_VC120.lib")
#elif _MSC_VER == 1700
#pragma comment(lib, "log4cppLIB_VC110.lib")
#elif _MSC_VER == 1600
#pragma comment(lib, "log4cppLIB_VC100.lib")
#endif

#endif

#endif

namespace ducky {
	namespace log {


		class LoggerAppender : public log4cpp::FileAppender
		{
		public:
			LoggerAppender(const std::string& name, const std::string& fileName,
				bool append = true, mode_t mode = 00644)
				: FileAppender(name, fileName, append, mode)
			{
			}

			virtual void doAppend(const log4cpp::LoggingEvent& event)
			{
				string info;
				if (this->requiresLayout())
				{
					info = this->_getLayout().format(event);
				}
				else
				{
					info = event.message;
				}

			}
		};


		Logger::Logger(const _W(string)& instanceName) :
			root(LogCategory::getInstance((const char*)bstr_t(instanceName.c_str()))),
			currentPriority(log4cpp::Priority::INFO), reservedCount(-1) {
			this->logFileSuffix = _T("log");

			this->root.setPriority(log4cpp::Priority::INFO);
		}

		Logger::~Logger(void) {
			//this->root.shutdown();
		}

		void Logger::addOStreamTarget(ostream* stream) {
			log4cpp::OstreamAppender* osAppender = new log4cpp::OstreamAppender(
				"LoggerAppender", stream);
			log4cpp::PatternLayout* pLayout = new log4cpp::PatternLayout();
			pLayout->setConversionPattern("[%d{%Y-%m-%d %H:%M:%S} %p] %m\r\n");
			osAppender->setLayout(pLayout);
			this->root.addAppender(osAppender);
		}

		//---------------------------------------------------------------------------

		void Logger::addAppender(log4cpp::Appender* appender)
		{
			log4cpp::PatternLayout* pLayout = new log4cpp::PatternLayout();
			pLayout->setConversionPattern("[%d{%Y-%m-%d %H:%M:%S} %p] %m\r\n");
			appender->setLayout(pLayout);
			this->root.addAppender(appender);
		}

		//---------------------------------------------------------------------------

		_W(string) Logger::genLogFileName() {
			_W(string) fileName;

			time_t t = time(0);
			tm tTm;
#ifdef _MSC_VER
			localtime_s(&tTm, &t);
#else
			localtime_r(&t, &tTm);
#endif
			boost::scoped_array<char> buf(new char[50]);
			memset(&buf[0], 0, 50);
			strftime(&buf[0], 50, "%Y-%m-%d", &tTm);
			fileName = this->logFilePrefix + (const TCHAR*)bstr_t(&buf[0])
				+ _T(".") + logFileSuffix;
			return fileName;
		}

		//---------------------------------------------------------------------------

		void Logger::setPriority(log4cpp::Priority::PriorityLevel priority) {
			this->root.setPriority(priority);
		}

		//---------------------------------------------------------------------------

		void Logger::setLogFilePrefix(_W(string) logFilePrefix) {
			this->logFilePrefix = logFilePrefix;
			boost::trim(this->logFilePrefix);
		}

		//---------------------------------------------------------------------------

		void Logger::setLogFileSuffix(_W(string) logFileSuffix) {
			this->logFileSuffix = logFileSuffix;
			boost::trim(this->logFileSuffix);
			int pos = this->logFileSuffix.find(_T("."));
			if (0 == pos) {
				this->logFileSuffix = this->logFileSuffix.substr(1,
					this->logFileSuffix.length() - 1);
			}
		}

		//---------------------------------------------------------------------------

		bool Logger::createDir(_W(string) path) {
#ifdef _MSC_VER
			if (!path.empty())
			{
				int pos = -1;
				_W(string) tmpPathPart1;
				_W(string) tmpPathPart2 = path;
				while (((pos = tmpPathPart2.find_first_of(_T('\\'))) > 0) || !tmpPathPart2.empty())
				{
					_W(string) dirName = pos > 0 ? tmpPathPart2.substr(0, pos) : tmpPathPart2;
					tmpPathPart1 += tmpPathPart1.length() > 0 ? (_T("\\") + dirName) : dirName;
					tmpPathPart2 = (pos > 0) ?
						tmpPathPart2.substr(pos + 1, tmpPathPart2.length() - pos) : _W(string)(_T(""));

					int dPos = dirName.find(_T(':'));
					if (dPos <= 0 && dirName.length() > 0)
					{
						bstr_t p = tmpPathPart1.c_str();
						FileMgr::mkdir((const TCHAR*)p);
					}
					pos = -1;
				}
			}
#else
			if (!path.empty()) {
				int pos = -1;
				string tmpPathPart1;
				string tmpPathPart2 = path;
				while (((pos = tmpPathPart2.find_first_of('/')) > 0)
					|| !tmpPathPart2.empty()) {
					string dirName =
						pos > 0 ? tmpPathPart2.substr(0, pos) : tmpPathPart2;
					tmpPathPart1 +=
						tmpPathPart1.length() > 0 ? ("/" + dirName) : dirName;
					tmpPathPart2 =
						pos > 0 ?
						tmpPathPart2.substr(pos + 1,
							tmpPathPart2.length() - pos) :
						string("");

					int dPos = dirName.find(':');
					if (dPos <= 0 && dirName.length() > 0) {
						::mkdir(tmpPathPart1.c_str(), S_IRWXO | S_IRWXG | S_IRWXU);
					}
					pos = -1;
				}
			}
#endif

			return true;
		}

		//---------------------------------------------------------------------------

		void Logger::setLogDir(_W(string) logDir) {
			this->logDir = logDir;
			boost::trim(this->logDir);
			if (!this->logDir.empty()
				&& (_T('/') != this->logDir.at(this->logDir.length() - 1))) {
				this->logDir += _T("/");
			}
			if (!this->logDir.empty()) {
				this->createDir(this->logDir);
			}
		}

		//---------------------------------------------------------------------------

		void Logger::log(string info,
			log4cpp::Priority::PriorityLevel priority) {
			switch (priority) {
			case log4cpp::Priority::NOTSET:
				this->raw(info);
				break;
			case log4cpp::Priority::DEBUG:
				this->debug(info);
				break;
			case log4cpp::Priority::INFO:
				this->info(info);
				break;
			case log4cpp::Priority::NOTICE:
				this->notice(info);
				break;
			case log4cpp::Priority::WARN:
				this->warn(info);
				break;
			case log4cpp::Priority::CRIT:
				this->crit(info);
				break;
			case log4cpp::Priority::ALERT:
				this->alert(info);
				break;
			case log4cpp::Priority::ERROR:
				this->error(info);
				break;
			case log4cpp::Priority::FATAL:
				this->fatal(info);
				break;
			}
		}

		//---------------------------------------------------------------------------

		void Logger::log(wstring info,
			log4cpp::Priority::PriorityLevel priority)
		{
			this->log((const char*)bstr_t(info.c_str()), priority);
		}

		//---------------------------------------------------------------------------

		void Logger::fatal(string info) {
			boost::mutex::scoped_lock lock(this->mutex);
			this->checkLogFile();
			this->root.fatal(info);

		}

		//---------------------------------------------------------------------------

		void Logger::alert(string info) {
			boost::mutex::scoped_lock lock(this->mutex);
			this->checkLogFile();
			this->root.alert(info);

		}

		//---------------------------------------------------------------------------

		void Logger::crit(string info) {
			boost::mutex::scoped_lock lock(this->mutex);
			this->checkLogFile();
			this->root.crit(info);

		}

		//---------------------------------------------------------------------------

		void Logger::error(string info) {
			boost::mutex::scoped_lock lock(this->mutex);
			this->checkLogFile();
			this->root.error(info);

		}

		//---------------------------------------------------------------------------

		void Logger::warn(string info) {
			boost::mutex::scoped_lock lock(this->mutex);
			this->checkLogFile();
			this->root.warn(info);

		}

		//---------------------------------------------------------------------------

		void Logger::notice(string info) {
			boost::mutex::scoped_lock lock(this->mutex);
			this->checkLogFile();
			this->root.notice(info);

		}

		//---------------------------------------------------------------------------

		void Logger::info(string info) {
			boost::mutex::scoped_lock lock(this->mutex);
			this->checkLogFile();
			this->root.info(info);

		}

		//---------------------------------------------------------------------------

		void Logger::debug(string info) {
			boost::mutex::scoped_lock lock(this->mutex);
			this->checkLogFile();
			this->root.debug(info);

		}

		//---------------------------------------------------------------------------

		void Logger::raw(string info) {
			boost::mutex::scoped_lock lock(this->mutex);
			this->checkLogFile();
			this->root.log(log4cpp::Priority::NOTSET, info);

		}

		//---------------------------------------------------------------------------
#ifdef _MSC_VER
		void Logger::fatal(wstring info)
		{
			this->fatal((const char*)bstr_t(info.c_str()));
		}

		//---------------------------------------------------------------------------

		void Logger::alert(wstring info)
		{
			this->alert((const char*)bstr_t(info.c_str()));
		}

		//---------------------------------------------------------------------------

		void Logger::crit(wstring info)
		{
			this->crit((const char*)bstr_t(info.c_str()));
		}

		//---------------------------------------------------------------------------

		void Logger::error(wstring info)
		{
			this->error((const char*)bstr_t(info.c_str()));
		}

		//---------------------------------------------------------------------------

		void Logger::warn(wstring info)
		{
			this->warn((const char*)bstr_t(info.c_str()));
		}

		//---------------------------------------------------------------------------

		void Logger::notice(wstring info)
		{
			this->notice((const char*)bstr_t(info.c_str()));
		}

		//---------------------------------------------------------------------------

		void Logger::info(wstring info)
		{
			this->info((const char*)bstr_t(info.c_str()));
		}

		//---------------------------------------------------------------------------

		void Logger::debug(wstring info)
		{
			this->debug((const char*)bstr_t(info.c_str()));
		}

		//---------------------------------------------------------------------------

		void Logger::raw(wstring info)
		{
			this->raw((const char*)bstr_t(info.c_str()));
		}

		//---------------------------------------------------------------------------

		void Logger::fatal(const wchar_t* info)
		{
			this->fatal(wstring(info));
		}

		//---------------------------------------------------------------------------

		void Logger::alert(const wchar_t* info)
		{
			this->alert(wstring(info));
		}

		//---------------------------------------------------------------------------

		void Logger::crit(const wchar_t* info)
		{
			this->crit(wstring(info));
		}

		//---------------------------------------------------------------------------

		void Logger::error(const wchar_t* info)
		{
			this->error(wstring(info));
		}

		//---------------------------------------------------------------------------

		void Logger::warn(const wchar_t* info)
		{
			this->warn(wstring(info));
		}

		//---------------------------------------------------------------------------

		void Logger::notice(const wchar_t* info)
		{
			this->notice(wstring(info));
		}

		//---------------------------------------------------------------------------

		void Logger::info(const wchar_t* info)
		{
			this->info(wstring(info));
		}

		//---------------------------------------------------------------------------

		void Logger::debug(const wchar_t* info)
		{
			this->debug(wstring(info));
		}

		//---------------------------------------------------------------------------

		void Logger::raw(const wchar_t* info)
		{
			this->raw(wstring(info));
		}

#endif
		//---------------------------------------------------------------------------

		void Logger::fatal(const char* info) {
			this->fatal(string(info));
		}

		//---------------------------------------------------------------------------

		void Logger::alert(const char* info) {
			this->alert(string(info));
		}

		//---------------------------------------------------------------------------

		void Logger::crit(const char* info) {
			this->crit(string(info));
		}

		//---------------------------------------------------------------------------

		void Logger::error(const char* info) {
			this->error(string(info));
		}

		//---------------------------------------------------------------------------

		void Logger::warn(const char* info) {
			this->warn(string(info));
		}

		//---------------------------------------------------------------------------

		void Logger::notice(const char* info) {
			this->notice(string(info));
		}

		//---------------------------------------------------------------------------

		void Logger::info(const char* info) {
			this->info(string(info));
		}

		//---------------------------------------------------------------------------

		void Logger::debug(const char* info) {
			this->debug(string(info));
		}

		//---------------------------------------------------------------------------

		void Logger::raw(const char* info) {
			this->raw(string(info));
		}

		//---------------------------------------------------------------------------

		void Logger::checkLogFile() {
			if (!this->logDir.empty()) {
				if (!FileMgr::isDir(this->logDir))
				{
					this->createDir(this->logDir);
				}
			}
			_W(string) fileName = this->genLogFileName();
			if (fileName != this->logFileName) {
				this->logFileName = fileName;

				log4cpp::FileAppender* osAppender = (log4cpp::FileAppender*)this->root.getAppender("FileAppender");
				if (osAppender)
				{
					this->root.removeAppender(osAppender);
				}
				osAppender = new log4cpp::FileAppender(
					"FileAppender", (const char*)bstr_t((this->logDir + this->logFileName).c_str()));
				this->addAppender(osAppender);

				this->clearOldFiles();
			}
		}

		//---------------------------------------------------------------------------

		int Logger::getReservedLogFilesCount() {
			return this->reservedCount;
		}

		//---------------------------------------------------------------------------

		void Logger::setReservedLogFilesCount(int count) {
			this->reservedCount = count;
		}

		//---------------------------------------------------------------------------

		void Logger::clearOldFiles() {
			if (this->reservedCount > -1)
			{
				if (fs::exists(this->logDir)) {
					fs::directory_iterator item(this->logDir);
					fs::directory_iterator item_end;
					for (; item != item_end; item++) {
						_W(string) fileName = (const TCHAR*)bstr_t(item->path().filename().c_str());

						if (fileName.length() == this->logFileName.length()) {
							_W(string) prefix = fileName.substr(0,
								this->logFilePrefix.length());
							if (prefix != this->logFilePrefix) {
								continue;
							}
						}
						else {
							continue;
						}
						_W(string) dateStr = fileName;
						dateStr.erase(0, this->logFilePrefix.length());
						dateStr = dateStr.erase(
							dateStr.length() - this->logFileSuffix.length() - 1,
							this->logFileSuffix.length() + 1);
						_W(string) currentDateStr = this->logFileName;
						currentDateStr.erase(0, this->logFilePrefix.length());
						currentDateStr = currentDateStr.erase(
							currentDateStr.length() - this->logFileSuffix.length() - 1,
							this->logFileSuffix.length() + 1);

						vector<_W(string)> dateParts;
						vector<_W(string)> currentDateParts;
						boost::split(dateParts, dateStr, boost::is_any_of(_T("-")));
						boost::split(currentDateParts, currentDateStr,
							boost::is_any_of(_T("-")));

						int y1 = 0, y2 = 0, m1 = 0, m2 = 0, d1 = 0, d2 = 0;
						_W(stringstream) str;
						str << dateParts[0];
						str >> y1;
						str.str(_T(""));
						str.clear();
						str << dateParts[1];
						str >> m1;
						str.str(_T(""));
						str.clear();
						str << dateParts[2];
						str >> d1;

						str.str(_T(""));
						str.clear();
						str << currentDateParts[0];
						str >> y2;
						str.str(_T(""));
						str.clear();
						str << currentDateParts[1];
						str >> m2;
						str.str(_T(""));
						str.clear();
						str << currentDateParts[2];
						str >> d2;

						tm tm1, tm2;
						memset(&tm1, 0, sizeof(tm));
						memset(&tm2, 0, sizeof(tm));
						tm1.tm_year = y1 - 1900;
						tm1.tm_mon = m1 - 1;
						tm1.tm_mday = d1;

						tm2.tm_year = y2 - 1900;
						tm2.tm_mon = m2 - 1;
						tm2.tm_mday = d2;

						time_t t1 = 0, t2 = 0;
						t1 = mktime(&tm1);
						t2 = mktime(&tm2);

						time_t day = (t2 - t1) / 60 / 60 / 24;

						if (day > this->reservedCount || day < 0) {
#ifdef _MSC_VER
							bstr_t p = (this->logDir + fileName).c_str();
							FileMgr::rm((const TCHAR*)p);
#else
							remove((this->logDir + fileName).c_str());
#endif
						}
					}
				}
			}

		}

		//---------------------------------------------------------------------------
	}
}