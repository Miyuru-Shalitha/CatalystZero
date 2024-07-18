#define GLFW_INCLUDE_NONE
#include <glfw/glfw3.h>
#include <glad/glad.h>

#include "game.hpp"
#include "log.hpp"
#include "editor.hpp"
#include "arena_allocator.hpp"

static AppData app_data = {};

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_ESCAPE)
        {
            app_data.is_running = false;
        }
    }
}

static void window_close_callback(GLFWwindow* window)
{
    app_data.is_running = false;
}

int main()
{
    app_data.transiant_storage = arena_create(MB(50));
    app_data.persistance_storage = arena_create(MB(50));
    app_data.is_running = true;
    app_data.width = 1280;
    app_data.height = 720;

    if (!glfwInit())
    {
        CZ_ASSERT(false, "glfwInit() failed!");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    app_data.window = glfwCreateWindow(1280, 720, "CatalystZero", NULL, NULL);

    if (!app_data.window)
    {
        CZ_ASSERT(false, "glfwCreateWindow() failed!");
    }

    glfwMakeContextCurrent(app_data.window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwDestroyWindow(app_data.window);
        glfwTerminate();
        CZ_ASSERT(false, "gladLoadGLLoader() failed!");
    }

    glfwSetKeyCallback(app_data.window, key_callback);
    glfwSetWindowCloseCallback(app_data.window, window_close_callback);

    initialize_ecs(&app_data.persistance_storage, &app_data.ecs_data);
    initialize_editor(&app_data);
    initialize_game();

    while (app_data.is_running)
    {
        glViewport(0, 0, app_data.width, app_data.height);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();

        update_editor();
        update_game();
        
        glfwSwapBuffers(app_data.window);
    }

    shutdown_editor();

    glfwDestroyWindow(app_data.window);
    glfwTerminate();

    return 0;
}
