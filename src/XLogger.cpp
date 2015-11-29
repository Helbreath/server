
#include "XLogger.h"
#include "common.h"

XLogger * XLogger::_instance = nullptr;

XLogger::XLogger(string filename, Message::Priority priority)
{
	pFCFile = pFCConsole = nullptr;
	if (filename.length() > 0)
	{
		pFCFile = new FormattingChannel(new PatternFormatter("%Y-%m-%d %H:%M:%S.%c | %s:%q:%t"));
		pFCFile->setChannel(new FileChannel(filename + ".log"));
		pFCFile->setProperty("rotation", "daily");
		pFCFile->setProperty("times", "local");
		pFCFile->open();
		fileLogger = &Poco::Logger::create("FileLogger", pFCFile, priority);
	}

	pFCConsole = new FormattingChannel(new PatternFormatter("%q:%I %t"));
	pFCConsole->setChannel(new ConsoleChannel);
	pFCConsole->open();

	consoleLogger = &Poco::Logger::create("ConsoleLogger", pFCConsole, priority);
}

XLogger::~XLogger()
{
	Poco::Logger::destroy("ConsoleLogger");
	pFCConsole->close();
	if (pFCFile)
	{
		Poco::Logger::destroy("FileLogger");
		pFCFile->close();
	}
}

void XLogger::information(string str, XLogging out)
{
	stLogEntry * entry = new stLogEntry;
	entry->str = str;
	entry->out = out;
	entry->severity = Poco::Message::Priority::PRIO_INFORMATION;
	{
		lock_guard<mutex> lock(mtx);
		entryQueue.push(entry);
	}
}

void XLogger::debug(string str, XLogging out)
{
	stLogEntry * entry = new stLogEntry;
	entry->str = str;
	entry->out = out;
	entry->severity = Poco::Message::Priority::PRIO_DEBUG;
	{
		lock_guard<mutex> lock(mtx);
		entryQueue.push(entry);
	}
}

void XLogger::error(string str, XLogging out)
{
	stLogEntry * entry = new stLogEntry;
	entry->str = str;
	entry->out = out;
	entry->severity = Poco::Message::Priority::PRIO_ERROR;
	{
		lock_guard<mutex> lock(mtx);
		entryQueue.push(entry);
	}
}

void XLogger::critical(string str, XLogging out)
{
	stLogEntry * entry = new stLogEntry;
	entry->str = str;
	entry->out = out;
	entry->severity = Poco::Message::Priority::PRIO_CRITICAL;
	{
		lock_guard<mutex> lock(mtx);
		entryQueue.push(entry);
	}
}

void XLogger::fatal(string str, XLogging out)
{
	stLogEntry * entry = new stLogEntry;
	entry->str = str;
	entry->out = out;
	entry->severity = Poco::Message::Priority::PRIO_FATAL;
	{
		lock_guard<mutex> lock(mtx);
		entryQueue.push(entry);
	}
}

void XLogger::run()
{
	while (1)
	{
		if (!this->running && entryQueue.size() == 0)
			return;
		if (entryQueue.size())
		{
			stLogEntry * entry;
			{
				lock_guard<mutex> lock(mtx);
				entry = entryQueue.front();
				entryQueue.pop();
			}

			switch (entry->severity)
			{
			case Poco::Message::Priority::PRIO_FATAL:
				if (entry->out == XL_CONSOLE)
					consoleLogger->fatal(entry->str);
				else if (entry->out == XL_FILE)
					fileLogger->fatal(entry->str);
				else if (entry->out == XL_BOTH)
				{
					consoleLogger->fatal(entry->str);
					fileLogger->fatal(entry->str);
				}
				break;
			case Poco::Message::Priority::PRIO_CRITICAL:
				if (entry->out == XL_CONSOLE)
					consoleLogger->critical(entry->str);
				else if (entry->out == XL_FILE)
					fileLogger->critical(entry->str);
				else if (entry->out == XL_BOTH)
				{
					consoleLogger->critical(entry->str);
					fileLogger->critical(entry->str);
				}
				break;
			case Poco::Message::Priority::PRIO_ERROR:
				if (entry->out == XL_CONSOLE)
					consoleLogger->error(entry->str);
				else if (entry->out == XL_FILE)
					fileLogger->error(entry->str);
				else if (entry->out == XL_BOTH)
				{
					consoleLogger->error(entry->str);
					fileLogger->error(entry->str);
				}
				break;
			case Poco::Message::Priority::PRIO_WARNING:
				if (entry->out == XL_CONSOLE)
					consoleLogger->warning(entry->str);
				else if (entry->out == XL_FILE)
					fileLogger->warning(entry->str);
				else if (entry->out == XL_BOTH)
				{
					consoleLogger->warning(entry->str);
					fileLogger->warning(entry->str);
				}
				break;
			case Poco::Message::Priority::PRIO_NOTICE:
				if (entry->out == XL_CONSOLE)
					consoleLogger->notice(entry->str);
				else if (entry->out == XL_FILE)
					fileLogger->notice(entry->str);
				else if (entry->out == XL_BOTH)
				{
					consoleLogger->notice(entry->str);
					fileLogger->notice(entry->str);
				}
				break;
			case Poco::Message::Priority::PRIO_INFORMATION:
				if (entry->out == XL_CONSOLE)
					consoleLogger->information(entry->str);
				else if (entry->out == XL_FILE)
					fileLogger->information(entry->str);
				else if (entry->out == XL_BOTH)
				{
					consoleLogger->information(entry->str);
					fileLogger->information(entry->str);
				}
				break;
			case Poco::Message::Priority::PRIO_DEBUG:
				if (entry->out == XL_CONSOLE)
					consoleLogger->debug(entry->str);
				else if (entry->out == XL_FILE)
					fileLogger->debug(entry->str);
				else if (entry->out == XL_BOTH)
				{
					consoleLogger->debug(entry->str);
					fileLogger->debug(entry->str);
				}
				break;
			case Poco::Message::Priority::PRIO_TRACE:
				if (entry->out == XL_CONSOLE)
					consoleLogger->trace(entry->str);
				else if (entry->out == XL_FILE)
					fileLogger->trace(entry->str);
				else if (entry->out == XL_BOTH)
				{
					consoleLogger->trace(entry->str);
					fileLogger->trace(entry->str);
				}
				break;
			}
			delete entry;
		}
	}
}
