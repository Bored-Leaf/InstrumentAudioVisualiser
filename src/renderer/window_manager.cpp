#include "window_manager.hpp"

void WindowManager::onDrag(double mouseY) {
    m_horizontalSplitRatio = static_cast<float>(mouseY) / m_windowHeight;

    updateLayout();
}

void WindowManager::onResize(int width, int height) {
    m_windowHeight  = static_cast<float>(height);
    m_windowWidth   = static_cast<float>(width);

    updateLayout();
}

const Region& WindowManager::getWaveformRegion() const {
    return m_waveformWindow;
}

const Region& WindowManager::getFFTRegon() const {
    return m_fftWindow;
}

const glm::mat4& WindowManager::getProjection() const {
    return m_projection;
}

void WindowManager::updateLayout() {
    float windowLeft{0};
    float windowRight{m_windowWidth};
    float windowTop{m_windowHeight};
    float windowBottom{0};
    float windowSplit{m_windowHeight * m_horizontalSplitRatio};

    m_waveformWindow.setBounds(windowLeft, windowRight, windowSplit, windowTop);
    m_fftWindow.setBounds(windowLeft, windowRight, windowBottom, windowSplit);
}