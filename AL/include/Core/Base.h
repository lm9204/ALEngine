#ifndef BASE_H
#define BASE_H

#ifdef AL_PLATFORM_WINDOWS
#ifdef AL_BUILD_DLL
#define AL_API __declspec(dllexport)
#else
#define AL_API __declspec(dllimport)
#endif
#else
#error AL only support Windows!
#endif

#define BIT(x) (1 << x)

#define AL_BIND_EVENT_FN(fn)                                                                                           \
	[this](auto &&...args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#endif