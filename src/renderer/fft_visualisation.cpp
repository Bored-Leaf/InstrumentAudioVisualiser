#include <print>
#include <algorithm>
#include <random>

#include "renderer/fft_visualisation.hpp"
#include "constants.hpp"
#include "shader.h"

FFTVisualisation::FFTVisualisation(const char *vertPath, const char *fragPath)
    : m_shaders(Shader{vertPath, fragPath}) {}

void FFTVisualisation::init(const std::vector<float> &initVertexData) {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);

    std::vector<float> initBarVertexData{};

    createBars(initBarVertexData);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(initBarVertexData.size() * sizeof(float)), initBarVertexData.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
    glEnableVertexAttribArray(0);
}

void FFTVisualisation::createBars(std::vector<float> &barVertexData) const {
    float barWidth{constants::SCR_WIDTH / m_numBins};
    // TODO add bar space functionality
    // float spaceBetweenBars{0};

    // temp to get random heights
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distr(0.0F, constants::SCR_HEIGHT * 0.8F);

    for (int currentbar = 1;currentbar <= m_numBins;currentbar++) {
        float left{barWidth * (static_cast<float>(currentbar) - 1)};
        float right{barWidth * static_cast<float>(currentbar)};
        float bottom{0};
        // TODO make fft amplitude outputs
        float top{distr(gen)};

        std::vector<float> bar{createBar(left, right, bottom, top)};
        barVertexData.insert(barVertexData.end(), bar.begin(), bar.end());
    }
}

std::vector<float> FFTVisualisation::createBar(const float left, const float right, const float bottom, const float top) const {
    return {left, bottom, 0,
            right, top, 0,
            left, top, 0,
            left, bottom, 0,
            right, bottom, 0,
            right, top, 0};
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