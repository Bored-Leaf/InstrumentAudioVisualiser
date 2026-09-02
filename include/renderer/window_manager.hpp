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

    [[nodiscard]] float             getHorizontalSplit() const; 
    [[nodiscard]] bool              getIsDraggingActive() const;
    [[nodiscard]] float             getWindowHeight() const;
    [[nodiscard]] float             getWindowWidth() const;

    void                            setIsDraggingActive(bool value);

private:
    glm::mat4 m_projection{glm::ortho(0.0F, constants::SCR_WIDTH, 0.0F, constants::SCR_HEIGHT)};

    Region m_waveformWindow;
    Region m_fftWindow;
    
    float m_windowHeight{constants::SCR_HEIGHT};
    float m_windowWidth{constants::SCR_WIDTH};
    float m_horizontalSplitRatio{0.5F};

    bool m_isDraggingActive{false};

    void updateLayout();
};