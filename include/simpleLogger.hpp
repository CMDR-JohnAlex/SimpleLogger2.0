#ifndef LOGGER_HPP
#define LOGGER_HPP

/*
If using C++20 then it may be a good idea to use std::format instead of fmt::format
To do so define `LOGGER_USE_STD_FORMAT` before including this file OR uncomment the line below
*/

//#define LOGGER_USE_STD_FORMAT

#ifdef LOGGER_USE_STD_FORMAT
#include <format>
#else
#define FMT_HEADER_ONLY
#include "fmt/core.h"
#include "fmt/chrono.h"
#endif

#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <fstream>
#include <chrono>
#include <string_view>

/* TODO: Make a logger

Logger features:
- [X] Logging levels (Unknown, Failure, Error, Warning, Important, Info, Debug, Verbose)
	- [X] Verbosity level, anything under gets filtered out
- [X] Log targets
	- [ ] Daily log files
	- [X] Console logging (optional colors)
	- [X] File logging
	- [X] Add ability to log to more than one target
- [ ] Custom log formats
- [X] Log time, thread id, file name, line number, etc.
- [ ] Async logging?


HOW TO:
======

How to get thread id:
--------------------
std::cout << "Thread ID: " << std::this_thread::get_id() << '\n';

Get file details:
----------------
void log(const std::string& message, const std::source_location& location = std::source_location::current()) {
std::cout << location.file_name() < '(' << location.line() << ':' << location.column() << ')' << '`' << location.function_name() << '`' << ": " << message << '\n';
}

How to format: (C++20 or greater)
-------------
#include <format>
std::format("Hello {1}!\n", "world", "dog") // Prints "Hello dog!"

How to get logs inputs with format:
----------------------------------
template<typename... Args>
std::string log(std::string_view format, Args&&... args)
{
	return std::vformat(format, std::make_format_args(args...));
}

std::string log(std::string_view format) // This is a backup, just in-case you don't pass a second parameter it will crash, so this prevents that.
{
	return static_cast<std::string>(format);
}



EXAMPLES:
========

Set verbose level:
-----------------
logger->setVerboseLevel(-2);
logger->setVerboseLevel(SimpleLogger::SeverityLevel::Verbose);

Add target:
----------
logger->addTarget(new SimpleLogger::ConsoleTarget());
logger->addTarget(new SimpleLogger::FileTarget());

Change console color on/off:
---------------------------
int consoleLogger = logger->addTarget(new SimpleLogger::ConsoleTarget());
logger->getTarget(consoleLogger)->enableColors(true);

Change log file path:
--------------------
int fileLogger = logger->addTarget(new SimpleLogger::FileTarget());
logger->getTarget(fileLogger)->changeFilePath("logs/log.txt");

Log to one target:
-----------------
int consoleLogger1 = logger->addTarget(new SimpleLogger::ConsoleTarget());
int consoleLogger2 = logger->addTarget(new SimpleLogger::ConsoleTarget());
logger->getTarget(consoleLogger1)->log(SimpleLogger::SeverityLevel::Verbose, "Only consoleLogger1!");

Remove target:
-------------
int consoleLogger1 = logger->addTarget(new SimpleLogger::ConsoleTarget());
logger->removeTarget(consoleLogger1);

Support for std::format tags:
----------------------------
logger->addTarget(new SimpleLogger::ConsoleTarget());
logger->log(SimpleLogger::SeverityLevel::Debug, "Hello {1}!", "World", "Dog");
logger->log(SimpleLogger::SeverityLevel::Debug, "Hello {0}, you are a {1}!", "World", "Dog");
logger->log(SimpleLogger::SeverityLevel::Debug, "Hello {}, you are a {}!", "World", "Dog");

*/

namespace SimpleLogger
{
	enum class SeverityLevel
	{
		Unknown = 5,
		Failure = 4,
		Error = 3,
		Warning = 2,
		Important = 1,
		Info = 0,
		Debug = -1,
		Verbose = -2
	};

	class Target
	{
	public:
		Target() = default;
		virtual ~Target() = default;

		virtual void log(const SeverityLevel& severityLevel, const std::string& message) = 0;

		void setPrefix(const std::string& newPrefix)
		{
			prefix = newPrefix;
		}

		void enableColors(bool shouldEnable = true, bool manyColors = true)
		{
			addColors = shouldEnable;
			this->manyColors = manyColors;
		}
		void disableColors()
		{
			addColors = false;
		}

		void changeFilePath(std::string path)
		{
			filePath = path;
		}
		void appendToLogFile(bool shouldAppend = true)
		{
			appendToFile = shouldAppend;
		}

	protected:
		std::string UTCTime()
		{
#ifdef LOGGER_USE_STD_FORMAT
			std::string UTCTime = std::format("{:%F %T}", floor<std::chrono::seconds>(std::chrono::system_clock::now()));
#else
			std::string UTCTime = fmt::format("{:%F %T}", floor<std::chrono::seconds>(std::chrono::system_clock::now()));
#endif

			// This is local time example
			//auto now = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
			//std::string localTime = std::format("{:%F %T}", std::chrono::floor<std::chrono::seconds>(now));

return UTCTime;
		}

		std::string severityLevelToText(const SeverityLevel& severityLevel)
		{
			switch (severityLevel)
			{
			case SeverityLevel::Failure:   return "[ FAILURE ]";
			case SeverityLevel::Error:     return "[  ERROR  ]";
			case SeverityLevel::Warning:   return "[ WARNING ]";
			case SeverityLevel::Important: return "[IMPORTANT]";
			case SeverityLevel::Info:      return "[  INFO   ]";
			case SeverityLevel::Debug:     return "[  DEBUG  ]";
			case SeverityLevel::Verbose:   return "[ VERBOSE ]";
			}
			return "[ UNKNOWN ]";
		}
		std::string severityLevelToColor(const SeverityLevel& severityLevel)
		{
			switch (severityLevel)
			{
			case SeverityLevel::Failure:   return "\x1b[31m";
			case SeverityLevel::Error:     return "\x1b[91m";
			case SeverityLevel::Warning:   return "\x1b[33m";
			case SeverityLevel::Important: return "\x1b[32m";
			case SeverityLevel::Info:      return "\x1b[34m";
			case SeverityLevel::Debug:     return "\x1b[35m";
			case SeverityLevel::Verbose:   return "\x1b[35m";
			}
			return "\x1b[90m"; // \x1b[0m
		}
		std::string severityLevelToColor()
		{
			return "\x1b[0m";
		}

	protected:
		std::string prefix = "";

		bool addColors = true;
		bool manyColors = true;

		std::string filePath = "logs/LogFile.log";
		bool appendToFile = false;

		bool addTime = true;
		bool addThreadID = true;
	};

	class FileTarget : public Target
	{
	public:
		FileTarget(std::string filePath = "logs/LogFile.log", bool appendToFile = false, bool addTime = true, bool addThreadID = true)
		{
			this->filePath = filePath;
			this->appendToFile = appendToFile;
			this->addTime = addTime;
			this->addThreadID = addThreadID;

			if (!appendToFile)
				std::fstream logFileRead(filePath, std::ios::out | std::ios::trunc); // std::ios::trunc will replace the file instead of appending to the old contents
		}

		virtual ~FileTarget()
		{
		}

		virtual void log(const SeverityLevel& severityLevel, const std::string& message)
		{
			std::fstream logFileRead(filePath, std::ios::out | std::ios::app); // std::ios::app will add to the file instead of deleting the old contents
			try
			{
				logFileRead.exceptions(std::ifstream::failbit | std::ifstream::badbit);

				if (prefix != "")
					logFileRead << prefix << ' ';
				if (addTime)
					logFileRead << UTCTime() << ' ';
				logFileRead << severityLevelToText(severityLevel) << ' ';
				if (addThreadID)
					logFileRead << "[" << std::this_thread::get_id() << "]" << ' ';
				logFileRead << message << '\n';
			}
			catch (std::ifstream::failure e)
			{
				std::cerr << "Something went wrong opening the file: \"" << filePath << "\". Please make sure any directories exist! Error: " << e.what() << std::endl;
			}
		}
	};

	class ConsoleTarget : public Target
	{
	public:
		ConsoleTarget(bool addColors = true, bool manyColors = true, bool addTime = true, bool addThreadID = true)
		{
			this->addColors = addColors;
			this->manyColors = manyColors;
			this->addTime = addTime;
			this->addThreadID = addThreadID;
		}

		virtual ~ConsoleTarget()
		{

		}

		virtual void log(const SeverityLevel& severityLevel, const std::string& message)
		{
			if (prefix != "")
				std::cout << prefix << ' ';
			if (addColors && manyColors)
				std::cout << severityLevelToColor(severityLevel);
			if (addTime)
				std::cout << UTCTime() << ' ';
			if (addColors && manyColors)
				std::cout << severityLevelToText(severityLevel) << ' ';
			else if (addColors && !manyColors)
				std::cout << severityLevelToColor(severityLevel) << severityLevelToText(severityLevel) << severityLevelToColor() << ' ';
			else
				std::cout << severityLevelToText(severityLevel) << ' ';
			if (addThreadID)
				std::cout << "[" << std::this_thread::get_id() << "]" << ' ';
			std::cout << message << '\n';
			if (addColors && manyColors)
				std::cout << severityLevelToColor();
		}
	};

	class Logger
	{
	public:
		Logger()
		{
			verboseLevel = static_cast<int>(SeverityLevel::Verbose);
		}

		virtual ~Logger()
		{
			for (Target* target : targets)
			{
				delete target;
			}
		}

		Target*& addTarget(Target* target)
		{
			targets.emplace(targets.begin() + insertIndex, target);
			insertIndex++;
			return targets[insertIndex - 1];
		}

		void removeTarget(Target* deleteTarget)
		{
			auto it = std::find(targets.begin(), targets.end(), deleteTarget);
			if (it != targets.end())
			{
				delete deleteTarget;
				targets.erase(it);
				insertIndex--;
			}
		}

		void setVerboseLevel(const int& verboseLevel)
		{
			this->verboseLevel = verboseLevel;
		}

		void setVerboseLevel(const SeverityLevel& severityLevel)
		{
			verboseLevel = static_cast<int>(severityLevel);
		}

		void setPrefix(const std::string& newPrefix)
		{
			for (Target* target : targets)
			{
				target->setPrefix(newPrefix);
			}
		}

		void log(const SeverityLevel& severityLevel, std::string_view message)
		{
			if (static_cast<int>(severityLevel) < verboseLevel) return;

			for (Target* target : targets)
			{
				target->log(severityLevel, static_cast<std::string>(message));
			}
		}
		
#ifdef LOGGER_USE_STD_FORMAT
		template<typename... Args>
		void log(const SeverityLevel& severityLevel, std::string_view format, Args&&... args)
		{
			log(severityLevel, std::vformat(format, std::make_format_args(args...)));
		}
#else
		template<typename... Args>
		void log(const SeverityLevel& severityLevel, std::string_view format, Args&&... args)
		{
			log(severityLevel, fmt::vformat(format, fmt::make_format_args(args...)));
		}
#endif

		void log(std::string_view message)
		{
			log(SeverityLevel::Unknown, message);
		}

	private:
		int verboseLevel;
		int insertIndex = 0;
		std::vector<Target*> targets;
		//std::map<int, Target*> targets;
		//std::unordered_map<int, Target*> targets;
	};
}

#endif // LOGGER_HPP