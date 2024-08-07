#define GLFW_INCLUDE_NONE
#include <glfw/glfw3.h>
#include <glad/glad.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "game.hpp"
#include "log.hpp"
#include "editor.hpp"
#include "arena_allocator.hpp"
#include "file_reader.hpp"

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

static void window_resize_callback(GLFWwindow* window, int width, int height)
{
    app_data.width = width;
    app_data.height = height;
}

static GLuint create_shader(const char* vertex_shader_source, const char* fragment_shader_source)
{
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    GLint success;
    GLchar infoLog[512] = {};

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        CZ_ASSERT(false, "%s", infoLog);
    }

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        CZ_ASSERT(false, "%s", infoLog);
    }

    GLuint static_shader_program = glCreateProgram();
    glAttachShader(static_shader_program, vertex_shader);
    glAttachShader(static_shader_program, fragment_shader);
    glLinkProgram(static_shader_program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glValidateProgram(static_shader_program);

    return static_shader_program;
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
    glfwSetWindowSizeCallback(app_data.window, window_resize_callback);

    /////////////////////////////////////////////////
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    static float vertices[20] = {
        // Positions          
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // Top-left
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // Bottom-left
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Bottom-right
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f  // Top-right
    };

    static unsigned int indices[6] = {
        0, 3, 2, 2, 1, 0
    };

    GLuint vertex_array;
    glCreateVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    GLuint vertex_buffer;
    glCreateBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 20, vertices, GL_STATIC_DRAW);

    GLuint index_buffer;
    glCreateBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    /////////////////////////////////////////////////

    ///// Framebuffer /////
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &app_data.scene_texture_id);
    glBindTexture(GL_TEXTURE_2D, app_data.scene_texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1920, 1080, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, app_data.scene_texture_id, 0);

    GLuint renderBufferObject;
    glGenRenderbuffers(1, &renderBufferObject);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1920, 1080);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        CZ_ASSERT(false, "Framebuffer is not complete!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    char* default_shader_vertex_source = read_file(&app_data.transiant_storage, "../../../Resources/shaders/default.vs.glsl");
    char* default_shader_fragment_source = read_file(&app_data.transiant_storage, "../../../Resources/shaders/default.fs.glsl");
    GLuint default_shader_program = create_shader(default_shader_vertex_source, default_shader_fragment_source);
    ///////////////////////

    initialize_ecs(&app_data.persistance_storage, &app_data.ecs_data);
    initialize_editor(&app_data);
    initialize_game();

    while (app_data.is_running)
    {
        glfwPollEvents();

        glViewport(0, 0, app_data.width, app_data.height);
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glBindTexture(GL_TEXTURE_2D, app_data.scene_texture_id);

        glViewport(0, 0, 1920, 1080);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(default_shader_program);

        /////////////////////////////////
        glm::mat4 view(1.0);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -0.3f));
        glm::mat4 projection = glm::ortho(-1.6f, 1.6f, -0.9f, 0.9f, 0.1f, 100.0f);
        /////////////////////////////////

        for (size_t i = 0; i < app_data.ecs_data.transform_size; i++)
        {
            TransformComponent* transform = &app_data.ecs_data.transforms[i];

            glm::mat4 model(1.0);
            model = glm::translate(model, glm::vec3(transform->position.x, transform->position.y, transform->position.z));
            glm::quat quaternion = glm::quat(glm::vec3(glm::vec3(transform->rotation.x, transform->rotation.y, transform->rotation.z)));
            glm::mat4 rotation_matrix = glm::toMat4(quaternion);
            model = model * rotation_matrix;
            model = glm::scale(model, glm::vec3(transform->scale.x, transform->scale.y, transform->scale.z));

            unsigned int model_location = glGetUniformLocation(default_shader_program, "model");
            glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));

            unsigned int view_location = glGetUniformLocation(default_shader_program, "view");
            glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

            unsigned int projection_location = glGetUniformLocation(default_shader_program, "projection");
            glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

            glBindVertexArray(vertex_array);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        update_editor();
        update_game();
        
        glfwSwapBuffers(app_data.window);

        arena_reset(&app_data.transiant_storage);
    }

    shutdown_editor();
    arena_free(&app_data.transiant_storage);
    arena_free(&app_data.persistance_storage);

    glfwDestroyWindow(app_data.window);
    glfwTerminate();

    return 0;
}
