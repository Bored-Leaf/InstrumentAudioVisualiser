#pragma once
#include <vector>

#include "renderer/visualisation.hpp"
#include "shader.h"
#include "constants.hpp"

class FFTVisualisation : public Visualisation {
public:
    FFTVisualisation(const char *vertPath, const char *fragPath);

    void init(const std::vector<float> &initVertexData) override;
    void draw(const glm::mat4 &projection) override;
    void cleanup() override;

    [[nodiscard]] unsigned int getVBO() const override;
    [[nodiscard]] unsigned int getVAO() const override;

private:
    unsigned int m_VBO{};
    unsigned int m_VAO{};

    Shader m_shaders;

    // TODO: Make number of bins able to change at runtime
    int m_numBins{(constants::FFT_WINDOW / 2) + 1};

    void createBars(std::vector<float> &barVertexData) const;
    [[nodiscard]] std::vector<float> createBar(float left, float right, float bottom, float top) const;
};