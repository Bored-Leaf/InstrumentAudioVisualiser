#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

class Shader {
public:
    unsigned int ID;

    Shader(const char *vertexPath, const char *fragmentPath) {
        // Retrieve the vertex/fragment source code from filepath
        std::string vertexCode{};
        std::string fragmentCode{};
        std::ifstream vShaderFile{};
        std::ifstream fShaderFile{};

        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);

            std::stringstream vShaderStream{};
            std::stringstream fShaderStream{};

            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            vShaderFile.close();
            fShaderFile.close();

            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        } catch (std::ifstream::failure e) {
            std::print(stderr, "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ >> {}", e.what());
        }
        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();

        // 2. Compile Shaders
        unsigned int vertex{};
        unsigned int fragment{};
        int success{};
        std::string infoLog{};

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, nullptr);
        glCompileShader(vertex);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex, 512, nullptr, infoLog.data());
            std::print(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED >> {}", infoLog);
        }

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, nullptr);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment, 512, nullptr, infoLog.data());
            std::print(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED >> {}", infoLog);
        }

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID, 512, nullptr, infoLog.data());
            std::print(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED >> {}", infoLog);
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use() const {
        glUseProgram(ID);
    }

    void deleteShader() const {
        glDeleteShader(ID);
    }

    void setBool(const std::string &name, bool value) {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
    }

    void setInt(const std::string &name, int value) {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setFloat(const std::string &name, float value) {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
};