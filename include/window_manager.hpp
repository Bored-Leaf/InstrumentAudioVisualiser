#pragma once
#include "region.hpp"

class WindowManager {
public:
    void onResize(int width, int height);
    void onDrag(double mouseY);

private:
    Region m_waveformWindow;
    Region m_fftWindow;
    
    float m_windowHeight{};
    float m_windowWidth{};
    float m_horizontalSplitRatio{};

    void updateLayout();
};