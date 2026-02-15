#pragma once

#include <string>

#include "glm/gtc/type_ptr.hpp"

#include <glad/glad.h>

class Shader {
public:
    unsigned int ID;

    Shader(const char *vertexPath, const char *fragmentPath);

    void use() const;

    void deleteShader() const;

    void setBool(const std::string &name, bool value);

    void setInt(const std::string &name, int value);

    void setFloat(const std::string &name, float value);

    void setMat4(const std::string &name, const glm::mat4 &value);
};