#include <print>
#include <algorithm>
#include <random>

#include "renderer/fft_visualisation.hpp"
#include "constants.hpp"
#include "shader.h"

FFTVisualisation::FFTVisualisation(const char *vertPath, const char *fragPath)
    : m_shaders(Shader{vertPath, fragPath}) {}

void FFTVisualisation::init(const std::vector<float> &initBarVertexData) {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(initBarVertexData.size() * sizeof(float)), initBarVertexData.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
    glEnableVertexAttribArray(0);
}



void FFTVisualisation::draw(const glm::mat4 &projection) {
    m_shaders.use();
    m_shaders.setMat4("projection", projection);

    glLineWidth(2.0F);
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, m_numBins * 6);
}

void FFTVisualisation::cleanup() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    m_shaders.deleteShader();
}

unsigned int FFTVisualisation::getVBO() const {
    return m_VBO;
}

unsigned int FFTVisualisation::getVAO() const {
    return m_VAO;
}