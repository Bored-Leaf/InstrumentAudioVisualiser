#pragma once

#include "glm/ext/matrix_float4x4.hpp"

class Region {
public:

    void setBounds(float top, float bottom, float left, float right);

    [[nodiscard]] glm::mat4 getProjection() const;

private:
    float m_top{};
    float m_bottom{};    
    float m_left{};
    float m_right{};

    glm::mat4 m_projection{};

    void recalculateProjection();
};