#include "waveform_visualisation.hpp"
#include "constants.hpp"

void WaveformVisualisation::init(const std::vector<float> &initVertexData) {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(initVertexData.size() * sizeof(float)), initVertexData.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
    glEnableVertexAttribArray(0);
}

void WaveformVisualisation::draw(const glm::mat4 &projection) {
    m_shaders.use();
    m_shaders.setMat4("projection", projection);

    glLineWidth(2.0F);
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_LINE_STRIP, 0, constants::WAVEFORM_WINDOW);
}

void WaveformVisualisation::cleanup() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    m_shaders.deleteShader();
}

unsigned int WaveformVisualisation::getVBO() const {
    return m_VBO;
}

unsigned int WaveformVisualisation::getVAO() const {
    return m_VAO;
}