#pragma once

class GlfwSystem {
public:
    GlfwSystem();
    ~GlfwSystem(); 

    GlfwSystem(const GlfwSystem&) = delete;
    GlfwSystem& operator=(const GlfwSystem&) = delete;
    GlfwSystem(GlfwSystem&&) = delete;
    GlfwSystem& operator=(GlfwSystem&&) = delete;

    void poll_events();
};