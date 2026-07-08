#pragma once

#include <memory>

#include "visualisation.hpp"
#include "shader.h"

class WaveformVisualisation : public Visualisation {
public:
    void draw(const glm::mat4 &projection) override;

    void init() override;

private:
    unsigned int m_VBO{};
    unsigned int m_VAO{};

    Shader m_shaders;
};