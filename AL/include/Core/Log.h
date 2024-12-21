#ifndef LOG_H
#define LOG_H

#include "Core/Base.h"
#include "spdlog/spdlog.h"
#include <memory>

namespace ale
{
class AL_API Log
{
  public:
	static void init();

	inline static std::shared_ptr<spdlog::logger> &getCoreLogger();
	inline static std::shared_ptr<spdlog::logger> &getClientLogger();

  private:
	static std::shared_ptr<spdlog::logger> s_CoreLogger;
	static std::shared_ptr<spdlog::logger> s_ClientLogger;
};
} // namespace ale

// core log macros
#define AL_CORE_TRACE(...) ::ale::Log::getCoreLogger()->trace(__VA_ARGS__)
#define AL_CORE_INFO(...) ::ale::Log::getCoreLogger()->info(__VA_ARGS__)
#define AL_CORE_WARN(...) ::ale::Log::getCoreLogger()->warn(__VA_ARGS__)
#define AL_CORE_ERROR(...) ::ale::Log::getCoreLogger()->error(__VA_ARGS__)
#define AL_CORE_FATAL(...) ::ale::Log::getCoreLogger()->fatal(__VA_ARGS__)

// client log macros
#define AL_TRACE(...) ::ale::Log::getClientLogger()->trace(__VA_ARGS__)
#define AL_INFO(...) ::ale::Log::getClientLogger()->info(__VA_ARGS__)
#define AL_WARN(...) ::ale::Log::getClientLogger()->warn(__VA_ARGS__)
#define AL_ERROR(...) ::ale::Log::getClientLogger()->error(__VA_ARGS__)
#define AL_FATAL(...) ::ale::Log::getClientLogger()->fatal(__VA_ARGS__)

#endif