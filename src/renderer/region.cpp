#include <glad/glad.h>

#include "renderer/region.hpp"

void Region::setBounds(float left, float right, float bottom, float top) {
    m_left = left;
    m_right = right;
    m_bottom = bottom;
    m_top = top;    
}

void Region::applyViewport() const {
    // BUG: no good with bigger sizes like 1600x1200
    glViewport(m_left, m_bottom, m_right, m_top - m_bottom);
}