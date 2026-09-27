cmake_minimum_required(VERSION 4.2)
project(blender-toy LANGUAGES CXX)

add_executable(blender-toy
    src/main.cpp
)

target_compile_features(blender-toy PRIVATE cxx_std_20)

target_compile_options(blender-toy PRIVATE
    /W4
    /WX
    /permissive-
    $<$<CONFIG:Debug>:/fsanitize=address>
)

target_link_options(blender-toy PRIVATE
    $<$<CONFIG:Debug>:/INCREMENTAL:NO>
)

cmake_path(GET CMAKE_CXX_COMPILER PARENT_PATH msvc_bin_dir)
add_custom_command(TARGET blender-toy POST_BUILD
    COMMAND "$<$<CONFIG:Debug>:${CMAKE_COMMAND};-E;copy_if_different;${msvc_bin_dir}/clang_rt.asan_dynamic-x86_64.dll;$<TARGET_FILE_DIR:blender-toy>>"
    COMMAND_EXPAND_LISTS
)