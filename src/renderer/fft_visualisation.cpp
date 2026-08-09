#include <print>

#include "renderer/fft_visualisation.hpp"
#include "constants.hpp"
#include "shader.h"

FFTVisualisation::FFTVisualisation(const char *vertPath, const char *fragPath)
    : m_shaders(Shader{vertPath, fragPath}) {}

void FFTVisualisation::init(const std::vector<float> &initVertexData) {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);

    // NEXT: Work out how to structure the vertex data in a vector
    std::vector<float> initBarVertexData{};

    createBars(initBarVertexData);

    std::print("{}\n", initBarVertexData.size());

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(initBarVertexData.size() * sizeof(float)), initBarVertexData.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
    glEnableVertexAttribArray(0);
}

void FFTVisualisation::createBars(std::vector<float> &initBarVertexData) const {
    // Make all the bars visible
    float barWidth{constants::SCR_WIDTH / (m_numBins / 50)};
    std::print("{}\n", barWidth);
    float maxBarHeight{constants::SCR_HEIGHT * 0.8F};
    float barBetweenSpace{0};

    float TEMP_HEIGHT{maxBarHeight};
    initBarVertexData.insert(initBarVertexData.end(), {(constants::SCR_WIDTH / 2) - (barWidth / 2), 0, 0});
    initBarVertexData.insert(initBarVertexData.end(), {(constants::SCR_WIDTH / 2) + (barWidth / 2), TEMP_HEIGHT, 0});
    initBarVertexData.insert(initBarVertexData.end(), {(constants::SCR_WIDTH / 2) - (barWidth / 2), TEMP_HEIGHT, 0});

    initBarVertexData.insert(initBarVertexData.end(), {(constants::SCR_WIDTH / 2) - (barWidth / 2), 0, 0});
    initBarVertexData.insert(initBarVertexData.end(), {(constants::SCR_WIDTH / 2) + (barWidth / 2), 0, 0});
    initBarVertexData.insert(initBarVertexData.end(), {(constants::SCR_WIDTH / 2) + (barWidth / 2), TEMP_HEIGHT, 0});
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