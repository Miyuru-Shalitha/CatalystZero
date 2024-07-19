#include "editor.hpp"

#include <glad/glad.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "log.hpp"
#include "string.hpp"

struct EditorData
{
    Arena transiant_storage;
    Arena persistance_storage;
    AppData* app_data;
    unsigned int selected_entity_id;
    char entity_count_char;
};

static EditorData editor_data = {
    .transiant_storage = arena_create(MB(50)),
    .persistance_storage = arena_create(MB(50))
};

static ImVec2 get_target_size(ImVec2 actual_size, ImVec2 available_size)
{
    // Calculate aspect ratios
    float texture_aspect_ration = (float)actual_size.x / (float)actual_size.y;
    float window_aspect_ratio = available_size.x / available_size.y;

    ImVec2 target_size;

    if (texture_aspect_ration < window_aspect_ratio)
    {
        // Texture is wider than window
        target_size.x = available_size.x;
        target_size.y = available_size.x / texture_aspect_ration;
    }
    else
    {
        // Texture is taller than window
        target_size.x = available_size.y * texture_aspect_ration;
        target_size.y = available_size.y;
    }

    // Calculate offset to center the texture
    ImVec2 offset(
        (available_size.x - target_size.x) * 0.5f,
        (available_size.y - target_size.y) * 0.5f
    );

    ImGui::SetCursorPos(
        ImVec2(
            ImGui::GetCursorPos().x + offset.x,
            ImGui::GetCursorPos().y + offset.y
        )
    );

    return target_size;
}

void initialize_editor(AppData* app_data)
{
    editor_data.app_data = app_data;
    editor_data.selected_entity_id = -1;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    io.FontDefault = io.Fonts->AddFontFromFileTTF("../../../Resources/fonts/Open_Sans/static/OpenSans-Regular.ttf", 18);

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(editor_data.app_data->window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init("#version 460");

    CZ_INFO("OpenGL Version: %s", glGetString(GL_VERSION));
}

void update_editor()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    ImGuiID dockspace_id = ImGui::GetID("Dockspace");
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    
    ImGui::DockSpaceOverViewport(dockspace_id, viewport, ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::ShowDemoWindow();

    if (ImGui::Begin("Scene Hierarchy"))
    {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
        {
            ImVec2 window_position = ImGui::GetWindowPos();
            ImVec2 window_size = ImGui::GetWindowSize();
            ImVec2 mouse_position = ImGui::GetMousePos();

            if (
                mouse_position.x >= window_position.x && mouse_position.x <= window_position.x + window_size.x &&
                mouse_position.y >= window_position.y && mouse_position.y <= window_position.y + window_size.y
                )
            {
                ImGui::OpenPopup("Menu");
            }
        }

        if (ImGui::BeginPopup("Menu"))
        {
            if (ImGui::MenuItem("Add Entity"))
            {
                Entity entity = create_entity();
                editor_data.selected_entity_id = entity.id;
                TransformComponent* transform = add_transform_component(entity.id);
                transform->scale = { 1.0f, 1.0f, 1.0f };
            }

            if (ImGui::BeginMenu("Add Lights"))
            {
                ImGui::MenuItem("Point Light");
                ImGui::MenuItem("Spot Light");
                ImGui::MenuItem("Sun Light");

                ImGui::EndMenu();
            }

            ImGui::EndPopup();
        }
        
        if (ImGui::TreeNodeEx("TreeNode", ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen))
        {
            for (size_t i = 0; i < editor_data.app_data->ecs_data.entity_size; i++)
            {
                unsigned int entity_handle = editor_data.app_data->ecs_data.entities[i].id;
                ImGuiTreeNodeFlags flags = (editor_data.selected_entity_id == entity_handle) ? 
                                            ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Selected : 
                                            ImGuiTreeNodeFlags_SpanAvailWidth;

                bool is_open = ImGui::TreeNodeEx("Entity", flags);

                if (ImGui::IsItemClicked())
                {
                    editor_data.selected_entity_id = entity_handle;
                }

                if (is_open)
                {
                    ImGui::TreePop();
                }
            }

            ImGui::TreePop();
        }

    }
    ImGui::End(); // Scene hierarchy end

    // Scene View begin
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    if (ImGui::Begin("Scene View", nullptr, ImGuiWindowFlags_NoScrollbar))
    {
        ImGui::Image((void*)editor_data.app_data->scene_texture_id, get_target_size(ImVec2(1920, 1080), ImGui::GetContentRegionAvail()), ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
    
    ImGui::PopStyleVar();
    // Scene view end

    if (ImGui::Begin("Console"))
    {
        ImGui::Text("Console");
    }
    ImGui::End(); // Console end

    if (ImGui::Begin("Inspector"))
    {
        if (editor_data.selected_entity_id != -1)
        {
            // Can optimize get_components to only get once when the entity is clicked in the hierarchy.
            TransformComponent* transform = get_transform_component(editor_data.selected_entity_id);
            StaticSpriteComponent* static_sprite = get_static_sprite_component(editor_data.selected_entity_id);

            if (transform != nullptr && ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (ImGui::BeginTable("Transform", 2))
                {
                    //ImGui::TableSetupColumn("Transform");
                    //ImGui::TableSetupColumn("x");
                    //ImGui::TableHeadersRow();

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Position");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::DragFloat3("##Position", &transform->position.x);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Rotation");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::DragFloat3("##Rotation", &transform->rotation.x);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Scale");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::DragFloat3("##Scale", &transform->scale.x);

                    ImGui::EndTable();
                }

                ImGui::TreePop();
            }

            if (static_sprite != nullptr && ImGui::TreeNodeEx("Static Sprite", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (ImGui::BeginTable("Static Sprite", 2))
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Sprite");
                    ImGui::TableSetColumnIndex(1);
                    // TODO(Miyuru): Display the texture. And make drag and drop.
                    ImGui::Text("Texture");
                    //ImGui::Image()

                    ImGui::EndTable();
                }

                ImGui::TreePop();
            }
        }

        if (ImGui::Button("Add Component"))
        {
            ImGui::OpenPopup("Components Popup");
        }

        if (ImGui::BeginPopup("Components Popup"))
        {
            if (ImGui::MenuItem("Static Sprite"))
            {
                if (editor_data.selected_entity_id != -1)
                {
                    add_static_sprite_component(editor_data.selected_entity_id);
                }
                else
                {
                    // TODO(Miyuru): Log a warning to editor console
                }
            }

            ImGui::EndPopup();
        }
    }
    ImGui::End(); // Inspector end

    ImGui::Render();
    int width, height;
    glfwGetFramebufferSize(editor_data.app_data->window, &width, &height);
    //glViewport(0, 0, width, height);
    //glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backupCurrentContext);
    }

    arena_reset(&editor_data.transiant_storage);
}

void shutdown_editor()
{
    arena_free(&editor_data.transiant_storage);
    arena_free(&editor_data.persistance_storage);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
