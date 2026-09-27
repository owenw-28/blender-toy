#include "window.hpp"

#include <GLFW/glfw3.h>

#include <stdexcept>

Window::Window(const GlfwSystem& /*system*/, int width, int height, const std::string& title) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);

    handle_.reset(glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr));
    if (!handle_) {
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(handle_.get());
    glfwSwapInterval(1); // Enable vsync
}

bool Window::should_close() const {
    return glfwWindowShouldClose(handle_.get()) == GLFW_TRUE;
}

void Window::request_close() {
    glfwSetWindowShouldClose(handle_.get(), GLFW_TRUE);
}

bool Window::is_key_down(int glfw_key) const {
    return glfwGetKey(handle_.get(), glfw_key) == GLFW_PRESS;
}

void Window::swap_buffers() {
    glfwSwapBuffers(handle_.get());
}

void Window::Deleter::operator()(GLFWwindow* window) const {
    glfwDestroyWindow(window);
}
