#pragma once

#if defined _MSC_VER || defined __CYGWIN__
	#define TEMPEST_IMPORT __declspec(dllimport)
	#define TEMPEST_EXPORT __declspec(dllexport)
	#define TEMPEST_LOCAL
#elif defined __clang__ || __GNUC__ >= 4
	#define TEMPEST_IMPORT __attribute__((visibility("default")))
	#define TEMPEST_EXPORT __attribute__((visibility("default")))
	#define TEMPEST_LOCAL  __attribute__((visibility("hidden")))
#else
	#error "Unsupported Compiler"
#endif
