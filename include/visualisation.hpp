#pragma once

#include "glm/ext/matrix_float4x4.hpp"

class Visualisation {
public:
    virtual void draw(const glm::mat4 &projection) = 0;
    virtual void init(const std::vector<float> &initVertexData) = 0;
    virtual void cleanup() = 0;

    [[nodiscard]] virtual unsigned int getVBO() const = 0;
    [[nodiscard]] virtual unsigned int getVAO() const = 0;

    virtual ~Visualisation() = default;
};