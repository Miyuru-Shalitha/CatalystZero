#pragma once

#include "arena_allocator.hpp"

inline char* concat_string(Arena* arena, const char* string_a, const char* string_b)
{
	size_t length_a = 0;

	while (string_a[length_a] != '\0')
	{
		length_a++;
	}

	size_t length_b = 0;

	while (string_b[length_b] != '\0')
	{
		length_b++;
	}

	size_t total_length = length_a + length_b;
	char* string_space = (char*)arena_alloc(arena, total_length + 1);

	for (size_t i = 0; i < length_a; i++)
	{
		string_space[i] = string_a[i];
	}

	for (size_t i = 0; i < length_b; i++)
	{
		string_space[length_a + i] = string_b[i];
	}

	string_space[total_length] = '\0';

	return string_space;
}

// From Chat-GPT
inline const char* format_string(Arena* arena, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	// Allocate memory for the formatted string
	char* buffer = (char*)arena_alloc(arena, 256);
	if (buffer == nullptr)
	{
		va_end(args);
		return "Out of memory";
	}

	// Format the string
	vsnprintf(buffer, 256, fmt, args);
	va_end(args);

	return buffer;
}