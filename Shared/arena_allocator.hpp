#pragma once

#include <Windows.h>

#include "log.hpp"

#define KB(kb) (kb) * 1024
#define MB(mb) (mb) * 1024 * 1024
#define GB(gb) (gb) * 1024 * 1024 * 1024

struct Arena
{
    size_t capacity;
    size_t size;
    char* memory;
};

inline Arena arena_create(size_t capacity)
{
    char* memory = (char*)VirtualAlloc(NULL, capacity, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    if (!memory)
    {
        CZ_ASSERT(false, "Failed to allocate memory");
    }

    return Arena {
        .capacity = capacity,
        .size = 0,
        .memory = memory
    };
}

inline void* arena_alloc(Arena* arena, size_t size)
{
    if (arena->size == arena->capacity)
    {
        CZ_ASSERT(false, "Not enough capacity");
        return nullptr;
    }

    void* memory = arena->memory + arena->size;
    arena->size += size;

    return memory;
}

inline void arena_reset(Arena* arena)
{
    arena->size = 0;
}

inline void arena_free(Arena* arena)
{
    VirtualFree(arena->memory, 0, MEM_RELEASE);
}