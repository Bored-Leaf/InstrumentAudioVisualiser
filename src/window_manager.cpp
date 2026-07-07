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

void WindowManager::updateLayout() {
    float windowTop{m_windowHeight};
    float windowSplit{m_windowHeight * m_horizontalSplitRatio};
    float windowBottom{0};

    m_waveformWindow.setBounds(windowTop, windowSplit, 0, m_windowWidth);
    m_fftWindow.setBounds(windowSplit, windowBottom, 0, m_windowWidth);
}