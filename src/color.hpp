#pragma once

struct Color3 {
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;

    // For C APIs that take float[3] such as ImGui::ColorEdit3
    float* data() { return &r; }
    const float* data() const { return &r; }
};

static_assert(sizeof(Color3) == sizeof(float) * 3, "Color3 must be three packed floats");