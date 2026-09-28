#include "gl_debug.hpp"
#include "glfw_system.hpp"
#include "window.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <exception>

namespace {

void process_events(GlfwSystem& system, Window& window) {
    system.poll_events();

    if (window.is_key_down(GLFW_KEY_ESCAPE)) {
        window.request_close();
    }
}

void draw(Window& window) {
    window.swap_buffers();
}

} // namespace

int main() {
    try {
        GlfwSystem system;
        Window window(system, 1200, 720, "blender-toy");

        install_gl_debug_callback();

        std::printf("OpenGL %s\n", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        std::printf("GPU %s\n", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));

        while (!window.should_close()) {
            process_events(system, window);
            draw(window);
        }
    } catch (const std::exception& e) {
        std::fprintf(stderr, "Fatal: %s\n", e.what());
        return 1;
    }
    return 0;
}
