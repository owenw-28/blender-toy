cmake_minimum_required(VERSION 4.2)
project(blender-toy LANGUAGES C CXX)

include(FetchContent)

function(blender_toy_apply_defaults target)
    target_compile_features(${target} PRIVATE cxx_std_20)
    target_compile_options(${target} PRIVATE
        /W4
        /WX
        /permissive-
        $<$<CONFIG:Debug>:/fsanitize=address>
    )
    target_link_options(${target} PRIVATE
        $<$<CONFIG:Debug>:/INCREMENTAL:NO>
    )
    cmake_path(GET CMAKE_CXX_COMPILER PARENT_PATH msvc_bin_dir)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND "$<$<CONFIG:Debug>:${CMAKE_COMMAND};-E;copy_if_different;${msvc_bin_dir}/clang_rt.asan_dynamic-x86_64.dll;$<TARGET_FILE_DIR:${target}>>"
        COMMAND_EXPAND_LISTS
    )
endfunction()

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

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.92.9b
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui SYSTEM PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(imgui PUBLIC glfw)

set(gtest_force_shared_crt ON  CACHE BOOL "" FORCE)
set(BUILD_GMOCK            OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST          OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.18.0
    GIT_SHALLOW    TRUE
    SYSTEM
)
FetchContent_MakeAvailable(googletest)

foreach(gtest_target gtest gtest_main)
    target_compile_options(${gtest_target} PRIVATE $<$<CONFIG:Debug>:/fsanitize=address>)
endforeach()

add_library(bt_math INTERFACE)
target_include_directories(bt_math INTERFACE src)
target_compile_features(bt_math INTERFACE cxx_std_20)

add_executable(blender-toy
    src/main.cpp
    src/glfw_system.cpp
    src/glfw_system.hpp
    src/window.cpp
    src/window.hpp
    src/gl_debug.cpp
    src/gl_debug.hpp
    src/color.hpp
    src/imgui_layer.cpp
    src/imgui_layer.hpp
    src/math/vec.hpp
    src/math/mat.hpp
)
blender_toy_apply_defaults(blender-toy)
target_link_libraries(blender-toy PRIVATE glfw glad imgui bt_math)
target_compile_definitions(blender-toy PRIVATE GLFW_INCLUDE_NONE)

enable_testing()

add_executable(bt_math_tests
    tests/math/vec_test.cpp
    tests/math/mat_test.cpp
)
blender_toy_apply_defaults(bt_math_tests)
target_link_libraries(bt_math_tests PRIVATE bt_math GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(bt_math_tests)
