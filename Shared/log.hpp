#pragma once

#include <cstdio>

#define CZ_TRACE(message, ...) std::printf("\033[37m" message "\033[0m\n", ##__VA_ARGS__)
#define CZ_INFO(message, ...) std::printf("\033[32m" message "\033[0m\n", ##__VA_ARGS__)
#define CZ_WARN(message, ...) std::printf("\033[33m" message "\033[0m\n", ##__VA_ARGS__)
#define CZ_ERROR(message, ...) std::printf("\033[31m" message "\033[0m\n", ##__VA_ARGS__)
#define CZ_FATAL(message, ...) std::printf("\033[35m" message "\033[0m\n", ##__VA_ARGS__)

#define CZ_ASSERT(x, message, ...)        \
{                                         \
	if (!(x))                             \
	{                                     \
		CZ_ERROR(message, ##__VA_ARGS__); \
		__debugbreak();                   \
	}                                     \
}