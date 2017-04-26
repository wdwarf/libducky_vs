#pragma once
#include <libducky/Object.h>
#include <log4cpp/Category.hh>
#include <string>
#include <fstream>
#include <sstream>
#include <boost/thread.hpp>

using std::string;
using std::wstring;
using std::fstream;
using std::stringstream;
using std::wstringstream;
using std::ostream;

namespace ducky {

	namespace log
	{

		class Logger
			: public Object
		{
		public:
			Logger(const _W(string)& instanceName = _T("Logger"));
			virtual ~Logger(void);

			void fatal(const char* info);
			void alert(const char* info);
			void crit(const char* info);
			void error(const char* info);
			void warn(const char* info);
			void notice(const char* info);
			void info(const char* info);
			void debug(const char* info);
			void raw(const char* info);

			void fatal(string info);
			void alert(string info);
			void crit(string info);
			void error(string info);
			void warn(string info);
			void notice(string info);
			void info(string info);
			void debug(string info);
			void raw(string info);
#ifdef _MSC_VER

			void fatal(const wchar_t* info);
			void alert(const wchar_t* info);
			void crit(const wchar_t* info);
			void error(const wchar_t* info);
			void warn(const wchar_t* info);
			void notice(const wchar_t* info);
			void info(const wchar_t* info);
			void debug(const wchar_t* info);
			void raw(const wchar_t* info);

			void fatal(wstring info);
			void alert(wstring info);
			void crit(wstring info);
			void error(wstring info);
			void warn(wstring info);
			void notice(wstring info);
			void info(wstring info);
			void debug(wstring info);
			void raw(wstring info);
#endif
			void log(string info, log4cpp::Priority::PriorityLevel priority = log4cpp::Priority::INFO);
			void log(wstring info, log4cpp::Priority::PriorityLevel priority = log4cpp::Priority::INFO);

			void setLogFilePrefix(_W(string) logFilePrefix);
			void setLogFileSuffix(_W(string) logFileSuffix);
			void setPriority(log4cpp::Priority::PriorityLevel priority);
			void setLogDir(_W(string) logDir);
			int getReservedLogFilesCount();
			void setReservedLogFilesCount(int count);

			void addOStreamTarget(ostream* stream);
			void addAppender(log4cpp::Appender* appender);

			template<class T>
			Logger& operator<<(T t)
			{
				stringstream logStream;
				logStream << t;
				if (!logStream.str().empty())
				{
					this->log(logStream.str(), this->currentPriority);
				}
				return *this;
			}

			Logger& operator<<(log4cpp::Priority::PriorityLevel priority)
			{
				this->currentPriority = priority;
				return *this;
			}

		protected:
			_W(string) logFilePrefix;
			_W(string) logFileSuffix;
			_W(string) logFileName;
			_W(string) logDir;
			int reservedCount;
			void clearOldFiles();
			bool createDir(_W(string) path);
			void checkLogFile();
			_W(string) genLogFileName();
			typedef log4cpp::Category LogCategory;
			LogCategory& root;
			boost::mutex mutex;

			log4cpp::Priority::PriorityLevel currentPriority;
		};

	}
}