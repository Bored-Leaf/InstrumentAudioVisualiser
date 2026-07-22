#include <print>

#include "glm/ext/matrix_clip_space.hpp"
#include <glad/glad.h>

#include "renderer/region.hpp"

void Region::setBounds(float left, float right, float bottom, float top) {
    m_left = left;
    m_right = right;
    m_bottom = bottom;
    m_top = top;    
}

void Region::applyViewport() const {
    glViewport(m_left, m_bottom, m_right, m_top - m_bottom);
}