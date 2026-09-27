#pragma once

#include <memory>
#include <string>

struct GLFWwindow;
class GlfwSystem;

class Window {
public:
    Window(const GlfwSystem& system, int width, int height, const std::string& title);

    bool should_close() const;
    void request_close();
    bool is_key_down(int glfw_key) const;
    void swap_buffers();

    GLFWwindow* native_handle() const { return handle_.get(); }

private:
    struct Deleter {
        void operator()(GLFWwindow* window) const;
    };
    std::unique_ptr<GLFWwindow, Deleter> handle_;
};