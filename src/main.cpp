#include "glfw_system.hpp"
#include "window.hpp"

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
