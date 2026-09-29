#include "color.hpp"
#include "gl_debug.hpp"
#include "glfw_system.hpp"
#include "imgui_layer.hpp"
#include "window.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include <cstdio>
#include <exception>

namespace {

void process_events(GlfwSystem& system, Window& window) {
    system.poll_events();

    if (!ImGui::GetIO().WantCaptureKeyboard && window.is_key_down(GLFW_KEY_ESCAPE)) {
        window.request_close();
    }
}

void draw_ui(Color3& clear_color) {
    ImGui::Begin("Viewport");
    ImGui::ColorEdit3("Background", clear_color.data());

    const float fps = ImGui::GetIO().Framerate;
    ImGui::Text("%.1f FPS (%.2f ms/frame)", fps, 1000.0f / fps);
    ImGui::End();
}

void draw(Window& window, ImGuiLayer& imgui, const Color3& clear_color) {
    glClearColor(clear_color.r, clear_color.g, clear_color.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    imgui.end_frame(); 
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

        ImGuiLayer imgui(window);

        Color3 clear_color{0.23f, 0.23f, 0.23f};

        while (!window.should_close()) {
            process_events(system, window);

            imgui.begin_frame();
            draw_ui(clear_color);

            draw(window, imgui, clear_color);
        }
    } catch (const std::exception& e) {
        std::fprintf(stderr, "Fatal: %s\n", e.what());
        return 1;
    }
    return 0;
}
