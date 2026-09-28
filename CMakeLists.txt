cmake_minimum_required(VERSION 4.2)
project(blender-toy LANGUAGES C CXX)

include(FetchContent)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.5.1
    GIT_SHALLOW    TRUE
    SYSTEM
)
FetchContent_MakeAvailable(glfw)

add_library(glad STATIC third_party/glad/src/gl.c)
target_include_directories(glad SYSTEM PUBLIC third_party/glad/include)

add_executable(blender-toy
    src/main.cpp
    src/glfw_system.cpp
    src/glfw_system.hpp
    src/window.cpp
    src/window.hpp
)

target_compile_features(blender-toy PRIVATE cxx_std_20)

target_compile_options(blender-toy PRIVATE
    /W4
    /WX
    /permissive-
    $<$<CONFIG:Debug>:/fsanitize=address>
)

target_link_libraries(blender-toy PRIVATE glfw glad)

target_compile_definitions(blender-toy PRIVATE GLFW_INCLUDE_NONE)

target_link_options(blender-toy PRIVATE
    $<$<CONFIG:Debug>:/INCREMENTAL:NO>
)

cmake_path(GET CMAKE_CXX_COMPILER PARENT_PATH msvc_bin_dir)
add_custom_command(TARGET blender-toy POST_BUILD
    COMMAND "$<$<CONFIG:Debug>:${CMAKE_COMMAND};-E;copy_if_different;${msvc_bin_dir}/clang_rt.asan_dynamic-x86_64.dll;$<TARGET_FILE_DIR:blender-toy>>"
    COMMAND_EXPAND_LISTS
)
