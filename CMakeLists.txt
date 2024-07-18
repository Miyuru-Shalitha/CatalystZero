cmake_minimum_required(VERSION 3.26)

project(CatalystZero)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/Ox")
endif()

add_subdirectory(ThirdParty/glfw-3.4)

file(GLOB_RECURSE ENGINE_SOURCE CONFIGURE_DEPENDS Engine/*.cpp)
file(GLOB_RECURSE GAME_SOURCE CONFIGURE_DEPENDS Game/*.cpp)

##### ENGINE BEGIN #####
add_executable(
    Engine
    ${ENGINE_SOURCE}
    ThirdParty/glad/src/glad.c
    ThirdParty/imgui-docking/imgui.cpp
	ThirdParty/imgui-docking/imgui_demo.cpp
	ThirdParty/imgui-docking/imgui_draw.cpp
	ThirdParty/imgui-docking/imgui_tables.cpp
	ThirdParty/imgui-docking/imgui_widgets.cpp
	ThirdParty/imgui-docking/backends/imgui_impl_glfw.cpp
	ThirdParty/imgui-docking/backends/imgui_impl_opengl3.cpp
)

target_include_directories(
    Engine
    PUBLIC Engine
    PUBLIC Shared
    PUBLIC ThirdParty/glad/include
    PUBLIC ThirdParty/imgui-docking
	PUBLIC ThirdParty/imgui-docking/backends
)

target_link_libraries(
    Engine
    PRIVATE Game
    PRIVATE glfw
)
###### ENGINE END ######

##### GAME BEGIN #####
add_library(
    Game
    ${GAME_SOURCE}
)

target_include_directories(
    Game
    PUBLIC Game
)
###### GAME END ######
