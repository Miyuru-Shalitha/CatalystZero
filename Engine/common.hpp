#pragma once

#define GLFW_INCLUDE_NONE
#include <glfw/glfw3.h>

#include "input.hpp"
#include "arena_allocator.hpp"
#include "ecs.hpp"

struct AppData
{
    Arena transiant_storage;
    Arena persistance_storage;
    ECSData ecs_data;
    Input input;
    GLFWwindow* window;
    float dt;
    int width, height;
    unsigned int scene_texture_id;
    bool is_running;
};