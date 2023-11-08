
#define FMT_HEADER_ONLY
#include "main.hpp"

#include <thread>

namespace Engine
{
	SimpleLogger::Logger* Log::coreLogger;
	SimpleLogger::Logger* Log::clientLogger;

	void Log::init()
	{
		coreLogger = new SimpleLogger::Logger();
		clientLogger = new SimpleLogger::Logger();
	}
	void Log::shutdown()
	{
		delete coreLogger;
		delete clientLogger;
	}
}

int main()
{
	Engine::Log log;
	log.init();


	// Set the verbose level of the logger's targets to -2 or "Verbose" (two different ways)
	log.getCoreLogger()->setVerboseLevel(-2);
	log.getCoreLogger()->setVerboseLevel(SimpleLogger::SeverityLevel::Verbose);
	


	// Creating two console and one file target
	auto consoleTarget1 = log.getCoreLogger()->addTarget(new SimpleLogger::ConsoleTarget());
	auto consoleTarget2 = log.getCoreLogger()->addTarget(new SimpleLogger::ConsoleTarget());
	auto fileTarget1 = log.getCoreLogger()->addTarget(new SimpleLogger::FileTarget("LogFile.log"));
	// Set both console target's prefix
	consoleTarget1->setPrefix("[Target 1]");
	consoleTarget2->setPrefix("[Target 2]");
	// Log only to consoleTarget1
	consoleTarget1->log(SimpleLogger::SeverityLevel::Failure, "Only console target 1!");
	// Log to both console and the only file target
	log.getCoreLogger()->log(SimpleLogger::SeverityLevel::Failure, "All targets");
	// Example of the std::format or fmt::format usage in logs
	log.getCoreLogger()->log(SimpleLogger::SeverityLevel::Failure, "{1} and {0}", 1.5f, "test");
	log.getCoreLogger()->log(SimpleLogger::SeverityLevel::Debug, "Hello {1}!", "World", "Dog");
	log.getCoreLogger()->log(SimpleLogger::SeverityLevel::Info, "I would rather be {1} than {0}", "right", "happy");
	// Also works as macros
	ENGINE_CORE_ERROR("{1} and {0}", 1.5f, "test");
	// Remove targets
	log.getCoreLogger()->removeTarget(consoleTarget1);
	log.getCoreLogger()->removeTarget(consoleTarget2);
	log.getCoreLogger()->removeTarget(fileTarget1);
	


	// Create a console logging target and assign it to consoleTarget
	auto consoleTarget = log.getCoreLogger()->addTarget(new SimpleLogger::ConsoleTarget());
	// Create a file logging target that appends to the log file and assign it to fileTarget
	auto fileTarget = log.getCoreLogger()->addTarget(new SimpleLogger::FileTarget("LogFile.log", true));
	// Set the consoleTarget's prefix
	consoleTarget->setPrefix("[ENIGNE]");
	// Set the fileTarget's prefix
	fileTarget->setPrefix("[ENIGNE]");
	// Set all of the logger's target's prefixes
	log.getCoreLogger()->setPrefix("[ENIGNE]");
	// Logging examples
	log.getCoreLogger()->log("Example of an unknown log severity");
	log.getCoreLogger()->log(SimpleLogger::SeverityLevel::Failure, "Imminent program failure");
	log.getCoreLogger()->log(SimpleLogger::SeverityLevel::Error, "Error, but program can continue");
	log.getCoreLogger()->log(SimpleLogger::SeverityLevel::Warning, "Warning");
	log.getCoreLogger()->log(SimpleLogger::SeverityLevel::Important, "Important messages, more relevant than regular info messages");
	log.getCoreLogger()->log(SimpleLogger::SeverityLevel::Info, "Default level on release builds. Used for general messages");
	log.getCoreLogger()->log(SimpleLogger::SeverityLevel::Debug, "Default level on debug builds. Used for messages that are only relevant to the developer");
	log.getCoreLogger()->log(SimpleLogger::SeverityLevel::Verbose, "Verbose level on debug builds. Useful when developers need more information");

	return 0;
}

#include <iostream>

//// Core log macros
//#define ENGINE_CORE_SEPERATOR() std::cout << '\n';
//#define ENGINE_CORE_UNKNOWN(...) ::Engine::Log::getCoreLogger()->unknown(__VA_ARGS__)
//#define ENGINE_CORE_FAILURE(...) ::Engine::Log::getCoreLogger()->failure(__VA_ARGS__)
//#define ENGINE_CORE_ERROR(...) ::Engine::Log::getCoreLogger()->error(__VA_ARGS__)
//#define ENGINE_CORE_WARNING(...) ::Engine::Log::getCoreLogger()->warning(__VA_ARGS__)
//#define ENGINE_CORE_IMPORTANT(...) ::Engine::Log::getCoreLogger()->important(__VA_ARGS__)
//#define ENGINE_CORE_INFO(...) ::Engine::Log::getCoreLogger()->info(__VA_ARGS__)
//#define ENGINE_CORE_DEBUG(...) ::Engine::Log::getCoreLogger()->debug(__VA_ARGS__)
//#define ENGINE_CORE_VERBOSE(...) ::Engine::Log::getCoreLogger()->verbose(__VA_ARGS__)

//// Client log macros
//#define ENGINE_CORE_SEPERATOR() std::cout << '\n';
//#define ENGINE_UNKNOWN(...) ::Engine::Log::getClientLogger()->unknown(__VA_ARGS__)
//#define ENGINE_FAILURE(...) ::Engine::Log::getClientLogger()->failure(__VA_ARGS__)
//#define ENGINE_ERROR(...) ::Engine::Log::getClientLogger()->error(__VA_ARGS__)
//#define ENGINE_WARNING(...) ::Engine::Log::getClientLogger()->warning(__VA_ARGS__)
//#define ENGINE_IMPORTANT(...) ::Engine::Log::getClientLogger()->important(__VA_ARGS__)
//#define ENGINE_INFO(...) ::Engine::Log::getClientLogger()->info(__VA_ARGS__)
//#define ENGINE_DEBUG(...) ::Engine::Log::getClientLogger()->debug(__VA_ARGS__)
//#define ENGINE_VERBOSE(...) ::Engine::Log::getClientLogger()->verbose(__VA_ARGS__)