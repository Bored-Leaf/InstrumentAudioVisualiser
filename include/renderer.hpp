#pragma once

#include <memory>

#include "window_manager.hpp"
#include "visualisation.hpp"
#include "waveform_visualisation.hpp"

class Renderer {
public:
    Renderer(WaveformVisualisation waveformVis);
    ~Renderer();

    void init(const std::vector<float> &initVertexData);
    void update();

    void onResize(const int width, const int height);
    void onDrag(const double mouseY);

    [[nodiscard]] const Visualisation* getWaveformVis() const;
private:
    WindowManager m_windowManager{};
    std::unique_ptr<Visualisation> m_waveformVis;
    std::unique_ptr<Visualisation> m_fftVis;

    void cleanup();
};