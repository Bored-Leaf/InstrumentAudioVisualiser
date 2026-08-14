#pragma once

#include <memory>

#include "fft_visualisation.hpp"
#include "window_manager.hpp"
#include "visualisation.hpp"
#include "renderer/waveform_visualisation.hpp"
#include "renderer/fft_visualisation.hpp"

class Renderer {
public:
    Renderer(const WaveformVisualisation &waveformVis, const FFTVisualisation &fftVis);
    ~Renderer();

    void init(const std::vector<float> &initVertexData, std::vector<float> &initBarVertexData);
    void update();

    void onResize(int width, int height);
    void onDrag(double mouseY);

    [[nodiscard]] const Visualisation* getWaveformVis() const;
    [[nodiscard]] const Visualisation* getFFTVis() const;
private:
    WindowManager m_windowManager{};
    std::unique_ptr<Visualisation> m_waveformVis;
    std::unique_ptr<Visualisation> m_fftVis;

    void cleanup();
};