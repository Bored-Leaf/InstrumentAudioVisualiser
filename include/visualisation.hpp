#pragma once

#include "glm/ext/matrix_float4x4.hpp"

class Visualisation {
public:
    virtual void draw(const glm::mat4 &projection) = 0;

    virtual void init() = 0;

    virtual ~Visualisation() = default;
};