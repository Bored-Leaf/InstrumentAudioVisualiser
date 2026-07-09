#pragma once

#include "visualisation.hpp"
#include "shader.h"

class WaveformVisualisation : public Visualisation {
public:
    WaveformVisualisation(const char *vertPath, const char *fragPath);

    void init(const std::vector<float> &initVertexData) override;
    void draw(const glm::mat4 &projection) override;
    void cleanup() override;

    [[nodiscard]] unsigned int getVBO() const override;
    [[nodiscard]] unsigned int getVAO() const override;

private:
    unsigned int m_VBO{};
    unsigned int m_VAO{};

    Shader m_shaders;
};