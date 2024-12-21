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

#endif