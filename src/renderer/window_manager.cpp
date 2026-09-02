#include <algorithm>

#include "renderer/window_manager.hpp"

void WindowManager::onDrag(double mouseY) {
    //convert GLFW mouse pos to opengl space
    float convertedY{m_windowHeight - static_cast<float>(mouseY)};

    convertedY = std::clamp(convertedY,m_windowHeight * 0.1F, m_windowHeight * 0.9F);
    m_horizontalSplitRatio = convertedY / m_windowHeight;

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

float WindowManager::getHorizontalSplit() const {
    return m_horizontalSplitRatio;
}

bool WindowManager::getIsDraggingActive() const {
    return m_isDraggingActive;
}

float WindowManager::getWindowHeight() const {
    return m_windowHeight;
}

float WindowManager::getWindowWidth() const {
    return m_windowWidth;
}

void WindowManager::setIsDraggingActive(bool value) {
    m_isDraggingActive = value;
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