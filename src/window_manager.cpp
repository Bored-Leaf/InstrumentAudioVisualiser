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

void WindowManager::updateLayout() {
    float windowTop{m_windowHeight};
    float windowSplit{m_windowHeight * m_horizontalSplitRatio};
    float windowBottom{0};

    m_waveformWindow.setBounds(0, m_windowWidth, windowSplit, windowTop);
    m_fftWindow.setBounds(0, m_windowWidth, windowBottom, windowSplit);
}