#include <print>

#include "region.hpp"

#include "glm/ext/matrix_clip_space.hpp"

void Region::setBounds(float top, float bottom, float left, float right) {
    m_top = top;
    m_bottom = bottom;
    m_left = left;
    m_right = right;

    recalculateProjection();
}

glm::mat4 Region::getProjection() const {
    return m_projection;
}

void Region::recalculateProjection() {
    std::print("{}, {}, {}, {}\n", m_left, m_right, m_bottom, m_top);
    m_projection = glm::ortho(m_left, m_right, m_bottom, m_top);
}