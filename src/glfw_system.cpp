#include "glfw_system.hpp"

#include <GLFW/glfw3.h>

#include <cstdio>
#include <stdexcept>

namespace {

void on_glfw_error(int code, const char* description) {
    std::fprintf(stderr, "GLFW error %d: %s\n", code, description);
}

} // namespace

GlfwSystem::GlfwSystem() {
    glfwSetErrorCallback(on_glfw_error);

    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
}

GlfwSystem::~GlfwSystem() {
    glfwTerminate();
}

void GlfwSystem::poll_events() {
    glfwPollEvents();
}