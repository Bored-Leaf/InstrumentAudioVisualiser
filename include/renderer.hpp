#pragma once

#include <memory>

#include "window_manager.hpp"
#include "visualisation.hpp"

class Renderer {
public:
    Renderer() = default;
    ~Renderer();

    void init(const std::vector<float> &initVertexData);
    void update();
private:
    WindowManager windowManager{};
    std::unique_ptr<Visualisation> waveformVis;
    std::unique_ptr<Visualisation> fftVis;

    void cleanup();
};