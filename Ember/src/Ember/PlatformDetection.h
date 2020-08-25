#ifndef PLATFORM_DETECTION_H
#define PLATFORM_DETECTION_H

#ifdef _WIN32
	#ifdef _WIN64
		#error "Ember does not support x64."
	#else
		#define EMBER_WINDOW_PLATFORM
	#endif

#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define HZ_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define HZ_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
#endif

#elif defined(__ANDROID__)
		#define HZ_PLATFORM_ANDROID
		#error "Android is not supported!"
#elif defined(__linux__)
		#define HZ_PLATFORM_LINUX
		#error "Linux is not supported!"
	#else
		#error "Unknown platform!"
#endif

#endif // !PLATFORM_DETECTION_H
