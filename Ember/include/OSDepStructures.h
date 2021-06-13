#ifndef OSDEP_H
#define OSDEP_H

#include "Window.h"
#include "EventHandler.h"

#if !defined(__cplusplus)
#error C++ compiler required.
#endif

#if defined(_WIN32) || defined(_WIN64)
#define PLATFORM_NAME "windows" // Windows
#include <Windows.h>
namespace Ember {
	HWND GetNativeWindowHandler(Window* window);

	UINT GetNativeMsg(EventHandler* handler);
	WPARAM GetWParam(EventHandler* handler);
	LPARAM GetLParam(EventHandler* handler);
}

#elif defined(__CYGWIN__) && !defined(_WIN32)
#define PLATFORM_NAME "windows" // Windows (Cygwin POSIX under Microsoft Window)
#error This platform is not supported.
#elif defined(__ANDROID__)
#define PLATFORM_NAME "android" // Android (implies Linux, so it must come first)
#error This platform is not supported.
#elif defined(__linux__)
#define PLATFORM_NAME "linux" // Debian, Ubuntu, Gentoo, Fedora, openSUSE, RedHat, Centos and other
#error This platform is not supported.
#elif defined(__unix__) || !defined(__APPLE__) && defined(__MACH__)
#include <sys/param.h>
#if defined(BSD)
#define PLATFORM_NAME "bsd" // FreeBSD, NetBSD, OpenBSD, DragonFly BSD
#error This platform is not supported.
#endif
#elif defined(__hpux)
#define PLATFORM_NAME "hp-ux" // HP-UX
#error This platform is not supported.
#elif defined(_AIX)
#define PLATFORM_NAME "aix" // IBM AIX
#error This platform is not supported.
#elif defined(__APPLE__) && defined(__MACH__) // Apple OSX and iOS (Darwin)
#include <TargetConditionals.h>
#error This platform is not supported.
#if TARGET_IPHONE_SIMULATOR == 1
#define PLATFORM_NAME "ios" // Apple iOS
#elif TARGET_OS_IPHONE == 1
#define PLATFORM_NAME "ios" // Apple iOS
#elif TARGET_OS_MAC == 1
#define PLATFORM_NAME "osx" // Apple OSX
#endif
#elif defined(__sun) && defined(__SVR4)
#define PLATFORM_NAME "solaris" // Oracle Solaris, Open Indiana
#error This platform is not supported.
#else
#define PLATFORM_NAME NULL
#endif

const char* GetPlatformName() {
    return (PLATFORM_NAME == NULL) ? "" : PLATFORM_NAME;
}

#endif // !OSDEP_H
