#include "renderer/renderer.hpp"
#include "constants.hpp"
#include "renderer/visualisation.hpp"
#include "shader.h"

#include <memory>
 
Renderer::Renderer(const WaveformVisualisation &waveformVis, const FFTVisualisation &fftVis)
    : m_waveformVis(std::make_unique<WaveformVisualisation>(waveformVis))
    , m_fftVis(std::make_unique<FFTVisualisation>(fftVis)) { }

Renderer::~Renderer() {
    cleanup();
}

void Renderer::init(const std::vector<float> &initVertexData, std::vector<float> &initBarVertexData) {
    m_waveformVis->init(initVertexData);
    m_fftVis->init(initBarVertexData);

    // Initial window bounds settings
    m_windowManager.onResize(constants::SCR_WIDTH, constants::SCR_HEIGHT);
}

void Renderer::update() {
    glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_windowManager.getWaveformRegion().applyViewport();
    m_waveformVis->draw(m_windowManager.getProjection());
    m_windowManager.getFFTRegon().applyViewport();
    m_fftVis->draw(m_windowManager.getProjection());
}

void Renderer::onResize(const int width, const int height) {
    m_windowManager.onResize(width, height);
}

void Renderer::onDrag(const double mouseY) {
    m_windowManager.onDrag(mouseY);
}

const Visualisation* Renderer::getWaveformVis() const {
    assert(m_waveformVis != nullptr && "m_waveformVis is nullptr");
    return m_waveformVis.get();
}

const Visualisation* Renderer::getFFTVis() const {
    assert(m_fftVis != nullptr && "m_FFTVis is nullptr");
    return m_fftVis.get();
}

void Renderer::cleanup() {
    m_waveformVis->cleanup();
    m_fftVis->cleanup();
}