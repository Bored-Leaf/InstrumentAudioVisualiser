#pragma once

#include "glm/ext/matrix_clip_space.hpp"

#include "region.hpp"
#include "constants.hpp"

class WindowManager {
public:
    void onResize(int width, int height);
    void onDrag(double mouseY);

    [[nodiscard]] const Region&     getWaveformRegion() const;
    [[nodiscard]] const Region&     getFFTRegon() const;
    [[nodiscard]] const glm::mat4&  getProjection() const;

private:
    glm::mat4 m_projection{glm::ortho(0.0F, constants::SCR_WIDTH, 0.0F, constants::SCR_HEIGHT)};

    Region m_waveformWindow;
    Region m_fftWindow;
    
    float m_windowHeight{};
    float m_windowWidth{};
    float m_horizontalSplitRatio{0.5};

    void updateLayout();
};