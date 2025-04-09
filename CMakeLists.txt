cmake_minimum_required(VERSION 3.29)

set(CMAKE_CXX_STANDARD 23)

project(CatalystZero)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp")

add_executable(CatalystZero)

target_sources(CatalystZero PRIVATE
    ${SOURCE_FILES}
    ThirdParty/glad/src/glad.c
)

target_include_directories(CatalystZero PRIVATE
    ThirdParty/glad/include
    ThirdParty/GL
)

target_link_libraries(CatalystZero PRIVATE
    user32.lib
    opengl32.lib
)