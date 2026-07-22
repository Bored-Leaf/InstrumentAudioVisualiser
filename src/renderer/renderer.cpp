#include "renderer/renderer.hpp"
#include "constants.hpp"
#include "renderer/visualisation.hpp"
#include "renderer/waveform_visualisation.hpp"
 
Renderer::Renderer(WaveformVisualisation waveformVis)
    : m_waveformVis(std::make_unique<WaveformVisualisation>(waveformVis)) { }

Renderer::~Renderer() {
    cleanup();
}

void Renderer::init(const std::vector<float> &initVertexData) {
    m_waveformVis->init(initVertexData);
    m_windowManager.onResize(constants::SCR_WIDTH, constants::SCR_HEIGHT);
}

void Renderer::update() {
    glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);

    m_windowManager.getWaveformRegion().applyViewport();
    m_waveformVis->draw(m_windowManager.getProjection());
}

void Renderer::onResize(const int width, const int height) {
    m_windowManager.onResize(width, height);
}

void Renderer::onDrag(const double mouseY) {
    m_windowManager.onDrag(mouseY);
}

const Visualisation* Renderer::getWaveformVis() const {
    assert(m_waveformVis != nullptr && "m_waveformVis is a nullptr");
    return m_waveformVis.get();
}

void Renderer::cleanup() {
    m_waveformVis->cleanup();
}