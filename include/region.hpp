#pragma once

#include "glm/ext/matrix_float4x4.hpp"

class Region {
public:

    void setBounds(float left, float right, float bottom, float top);
    void applyViewport() const;
private:
    float m_top{};
    float m_bottom{};    
    float m_left{};
    float m_right{};

    glm::mat4 m_projection{};
};