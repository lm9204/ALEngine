#ifndef INSTRUMENTOR_H
#define INSTRUMENTOR_H

#include "Core/Base.h"
#include "Core/Log.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

namespace ale
{

using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

struct ProfileResult
{
	std::string Name;

	FloatingPointMicroseconds Start;
	std::chrono::microseconds ElapsedTime;
	std::thread::id ThreadID;
};

struct InstrumentationSession
{
	std::string Name;
};

class AL_API Instrumentor
{
  public:
	Instrumentor(const Instrumentor &) = delete;
	Instrumentor(Instrumentor &&) = delete;

	void beginSession(const std::string &name, const std::string &filepath = "results.json")
	{
		std::lock_guard lock(m_Mutex);
		if (m_CurrentSession)
		{
			// If there is already a current session, then close it before beginning new one.
			// Subsequent profiling output meant for the original session will end up in the
			// newly opened session instead.  That's better than having badly formatted
			// profiling output.
			// 현재 활성화된 세션이 있을 때, 새로운 세션을 시작하기 전에 기존 세션을 종료함으로써 데이터 무결성을
			// 유지하고 혼란을 방지
			if (Log::getCoreLogger()) // Edge case: beginSession() might be before Log::Init()
			{
				AL_CORE_ERROR("Instrumentor::beginSession('{0}') when session '{1}' already open.", name,
							  m_CurrentSession->Name);
			}
			internalEndSession();
		}
		m_OutputStream.open(filepath);

		if (m_OutputStream.is_open())
		{
			m_CurrentSession = new InstrumentationSession({name});
			writeHeader();
		}
		else
		{
			if (Log::getCoreLogger()) // Edge case: beginSession() might be before Log::Init()
			{
				AL_CORE_ERROR("Instrumentor could not open results file '{0}'.", filepath);
			}
		}
	}

	void EndSession()
	{
		std::lock_guard lock(m_Mutex);
		internalEndSession();
	}

	void writeProfile(const ProfileResult &result)
	{
		std::stringstream json;

		json << std::setprecision(3) << std::fixed;
		json << ",{";
		json << "\"cat\":\"function\",";
		json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
		json << "\"name\":\"" << result.Name << "\",";
		json << "\"ph\":\"X\",";
		json << "\"pid\":0,";
		json << "\"tid\":" << result.ThreadID << ",";
		json << "\"ts\":" << result.Start.count();
		json << "}";

		std::lock_guard lock(m_Mutex);
		if (m_CurrentSession)
		{
			m_OutputStream << json.str();
			m_OutputStream.flush();
		}
	}

	static Instrumentor &get()
	{
		static Instrumentor instance;
		return instance;
	}

  private:
	Instrumentor() : m_CurrentSession(nullptr)
	{
	}

	~Instrumentor()
	{
		EndSession();
	}

	void writeHeader()
	{
		m_OutputStream << "{\"otherData\": {},\"traceEvents\":[{}";
		m_OutputStream.flush();
	}

	void writeFooter()
	{
		m_OutputStream << "]}";
		m_OutputStream.flush();
	}

	// Note: you must already own lock on m_Mutex before
	// calling internalEndSession()
	void internalEndSession()
	{
		if (m_CurrentSession)
		{
			writeFooter();
			m_OutputStream.close();
			delete m_CurrentSession;
			m_CurrentSession = nullptr;
		}
	}

  private:
	std::mutex m_Mutex;
	InstrumentationSession *m_CurrentSession;
	std::ofstream m_OutputStream;
};

class AL_API InstrumentationTimer
{
  public:
	InstrumentationTimer(const char *name) : m_Name(name), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::steady_clock::now();
	}

	~InstrumentationTimer()
	{
		if (!m_Stopped)
			stop();
	}

	void stop()
	{
		auto endTimepoint = std::chrono::steady_clock::now();
		auto highResStart = FloatingPointMicroseconds{m_StartTimepoint.time_since_epoch()};
		auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() -
						   std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

		Instrumentor::get().writeProfile({m_Name, highResStart, elapsedTime, std::this_thread::get_id()});

		m_Stopped = true;
	}

  private:
	const char *m_Name;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped;
};

namespace InstrumentorUtils
{

template <size_t N> struct ChangeResult
{
	char Data[N];
};

template <size_t N, size_t K> constexpr auto cleanupOutputString(const char (&expr)[N], const char (&remove)[K])
{
	ChangeResult<N> result = {};

	size_t srcIndex = 0;
	size_t dstIndex = 0;
	while (srcIndex < N)
	{
		size_t matchIndex = 0;
		while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
			matchIndex++;
		if (matchIndex == K - 1)
			srcIndex += matchIndex;
		result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
		srcIndex++;
	}
	return result;
}
} // namespace InstrumentorUtils
} // namespace ale

#if AL_PROFILE
#define AL_PROFILE_BEGIN_SESSION(name, filepath) ::ale::Instrumentor().get().beginSession(name, filepath)
#define AL_PROFILE_END_SESSION() ::ale::Instrumentor().get().endSession()
#define AL_PROFILE_SCOPE_LINE2(name, line)                                                                             \
	constexpr auto fixedName##line = ::ale::InstrumentorUtils::cleanupOutputString(name, "__cdecl ");                  \
	::ale::InstrumentationTimer timer##line(fixedName##line.Data)
#define AL_PROFILE_SCOPE_LINE(name, line) AL_PROFILE_SCOPE_LINE2(name, line)
#define AL_PROFILE_SCOPE(name) AL_PROFILE_SCOPE_LINE(name, __LINE__)
#define AL_PROFILE_FUNCTION() AL_PROFILE_SCOPE(HZ_FUNC_SIG)
#else
#define AL_PROFILE_BEGIN_SESSION(name, filepath)
#define AL_PROFILE_END_SESSION()
#define AL_PROFILE_SCOPE_LINE2()
#define AL_PROFILE_SCOPE_LINE()
#define AL_PROFILE_SCOPE()
#define AL_PROFILE_FUNCTION()

#endif
#endif