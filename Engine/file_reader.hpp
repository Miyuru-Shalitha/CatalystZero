#pragma once

#include <Windows.h>

inline char* read_file(Arena* arena, const char* file_path)
{
	HANDLE file_handle = CreateFileA(file_path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);

	if (file_handle == INVALID_HANDLE_VALUE)
	{
		return nullptr;
	}

	LARGE_INTEGER file_size;

	if (!GetFileSizeEx(file_handle, &file_size))
	{
		CloseHandle(file_handle);
		return nullptr;
	}

	HANDLE file_mapping_object_handle = CreateFileMappingA(file_handle, NULL, PAGE_READONLY, 0, 0, NULL);

	if (!file_mapping_object_handle)
	{
		CloseHandle(file_handle);
		return nullptr;
	}

	char* data = (char*)MapViewOfFile(file_mapping_object_handle, FILE_MAP_READ, 0, 0, 0);

	if (!data)
	{
		CloseHandle(file_mapping_object_handle);
		CloseHandle(file_handle);

		return nullptr;
	}

	char* file_memory = (char*)arena_alloc(arena, (size_t)file_size.QuadPart + 1);

	if (!file_memory)
	{
		UnmapViewOfFile(data);
		CloseHandle(file_mapping_object_handle);
		CloseHandle(file_handle);

		return nullptr;
	}

	CopyMemory(file_memory, data, file_size.QuadPart);
	file_memory[file_size.QuadPart] = '\0';

	UnmapViewOfFile(data);
	CloseHandle(file_mapping_object_handle);
	CloseHandle(file_handle);

	return file_memory;
}