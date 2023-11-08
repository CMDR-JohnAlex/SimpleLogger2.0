#pragma once

#define FMT_HEADER_ONLY
#include "../include/simpleLogger.hpp"

namespace Engine
{
	class Log
	{
	public:
		static void init();
		static void shutdown();

		inline static SimpleLogger::Logger*& getCoreLogger() { return coreLogger; };
		inline static SimpleLogger::Logger*& getClientLogger() { return clientLogger; };

	private:
		static SimpleLogger::Logger* coreLogger;
		static SimpleLogger::Logger* clientLogger;
	};
}


#define ENGINE_CORE_UNKNOWN(...) ::Engine::Log::getCoreLogger()->log(SimpleLogger::SeverityLevel::Unknown, __VA_ARGS__)
#define ENGINE_CORE_FAILURE(...) ::Engine::Log::getCoreLogger()->log(SimpleLogger::SeverityLevel::Failure, __VA_ARGS__)
#define ENGINE_CORE_ERROR(...) ::Engine::Log::getCoreLogger()->log(SimpleLogger::SeverityLevel::Error, __VA_ARGS__)
#define ENGINE_CORE_WARNING(...) ::Engine::Log::getCoreLogger()->log(SimpleLogger::SeverityLevel::Warning, __VA_ARGS__)
#define ENGINE_CORE_IMPORTANT(...) ::Engine::Log::getCoreLogger()->log(SimpleLogger::SeverityLevel::Important, __VA_ARGS__)
#define ENGINE_CORE_INFO(...) ::Engine::Log::getCoreLogger()->log(SimpleLogger::SeverityLevel::Info, __VA_ARGS__)
#define ENGINE_CORE_DEBUG(...) ::Engine::Log::getCoreLogger()->log(SimpleLogger::SeverityLevel::Debug, __VA_ARGS__)
#define ENGINE_CORE_VERBOSE(...) ::Engine::Log::getCoreLogger()->log(SimpleLogger::SeverityLevel::Verbose, __VA_ARGS__)