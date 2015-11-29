
#pragma once

#include <string>
#include "Poco/Logger.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/Message.h"
#include <thread>
#include <queue>
#include <boost/thread/mutex.hpp>

using boost::mutex;
using std::string;

enum XLogging {
	XL_FILE,
	XL_CONSOLE,
	XL_BOTH
};

using Poco::PatternFormatter;
using Poco::FormattingChannel;
using Poco::ConsoleChannel;
using Poco::FileChannel;
using Poco::Message;

class XLogger
{
public:
	XLogger(string filename = "", Message::Priority priority = Message::PRIO_TRACE);
	~XLogger();

	static void CreateInstance(string filename = "", Message::Priority priority = Message::PRIO_TRACE)
	{
		_instance = new XLogger(filename, priority);
		_instance->running = true;
		_instance->processor = std::thread(std::bind(std::mem_fun(&XLogger::run), _instance));
	}
	static XLogger * GetSingleton() { return _instance; }
	static void DestroyInstance() { _instance->running = false; _instance->processor.join(); delete _instance; }

private:
	static XLogger * _instance;
public:

	void information(string str, XLogging out = XL_CONSOLE);
	void debug(string str, XLogging out = XL_CONSOLE);
	void error(string str, XLogging out = XL_CONSOLE);
	void critical(string str, XLogging out = XL_CONSOLE);
	void fatal(string str, XLogging out = XL_CONSOLE);

	void run();

	mutex mtx;

	struct stLogEntry {
		XLogging out;
		string str;
		Poco::Message::Priority severity;
	};

	bool running;

	std::queue<stLogEntry*> entryQueue;

	std::thread processor;

	// Console and File log formatting
	FormattingChannel* pFCConsole;
	FormattingChannel* pFCFile;

	// Console and File log handles
	Poco::Logger * consoleLogger;
	Poco::Logger * fileLogger;

};
