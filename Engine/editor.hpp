#pragma once

#define GLFW_INCLUDE_NONE
#include <glfw/glfw3.h>

#include "arena_allocator.hpp"
#include "common.hpp"

void initialize_editor(AppData* app_data);
void update_editor();
void shutdown_editor();
