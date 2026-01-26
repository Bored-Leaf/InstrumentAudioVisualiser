#include <iostream>
#include <memory>
#include <print>
#include <string>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "WAVReader.h"
#include <waveformUtils.h>
#include "shader.h"

void printWaveformTerminal(const std::unique_ptr<WAVReader>& WAVFile);

void framebufferSize_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH{800};
const unsigned int SCR_HEIGHT{600};
const char* windowName{"Instrument Audio Visualiser"};
const int waveformWindow{441 * 50};

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, windowName, nullptr, nullptr);
    if (window == nullptr) {
        std::print(stderr, "Failed to create GLFW Window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSize_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::print(stderr, "Failed to initialise GLAD");
        return -1;
    }

    // Move eventually

    std::cout << "Hello, from InstrumentAudioVisualiser!\n";

    std::unique_ptr<WAVReader> WAVFile = std::make_unique<WAVReader>("WAVFiles/Ouch-2.wav");
    std::cout << "WAV file sampleRate: " << WAVFile->getSampleRate() << '\n';
    std::cout << "WAV file channels: " << WAVFile->getChannels() << '\n';
    std::cout << "WAV file bitsPerSample: " << WAVFile->getBitsPerSample() << '\n';

    WaveformUtils::printWaveformTerminal(WAVFile);

    // Move eventually

    auto triangleShader = std::make_unique<Shader>("shaders/triangle.vert", "shaders/triangleFrag.frag");

    std::vector<float> vertices{
        // Positions                        // Colours
        -0.5F, -0.5F, 0.0F,   1.0F, 0.0F, 0.0F,
         0.5F, -0.5F, 0.0F,   0.0F, 1.0F, 0.0F,
        0.0F, 0.5F, 0.0F,   0.0F, 0.0F, 1.0F
    };

    std::vector<float> waveformVertices{WaveformUtils::wavSamplesToVertices(WAVFile, waveformWindow, 0)};

    unsigned int VAO{};
    unsigned int VBO{};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, waveformVertices.size() * sizeof(float), waveformVertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
    glEnableVertexAttribArray(0);

    int nrAttribes{};
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttribes);
    std::println("Number nr of vertex attributes supported: {}", nrAttribes);

    float samplesToAdvance{};
    float fractionalLoss{};
    uint32_t sampleRate{WAVFile->getSampleRate()};
    float dtTime{static_cast<float>(glfwGetTime())};
    float previousFrame{};
    float currentFrame{};
    while(!glfwWindowShouldClose(window)) {
        // Delta time
        currentFrame = static_cast<float>(glfwGetTime());
        dtTime = currentFrame - previousFrame;
        previousFrame = currentFrame;
        //std::print("delta: {}\n", dtTime);

        processInput(window);

        // Update Vertecies
        samplesToAdvance = sampleRate * dtTime;
        // Get Fractional part
        fractionalLoss += samplesToAdvance - static_cast<int>(samplesToAdvance);
        // Add Integer if over 1.0
        WaveformUtils::updateWavVerticies(WAVFile, VBO, static_cast<int>(samplesToAdvance) + static_cast<int>(fractionalLoss), waveformWindow);

        if (fractionalLoss > 1.0F) {
            fractionalLoss -= 1;
        }

        glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        triangleShader->use();

        glLineWidth(2.0F);
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINE_STRIP, 0, waveformWindow);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    triangleShader->deleteShader();

    glfwTerminate();

    return 0;
}

void framebufferSize_callback(GLFWwindow* /*window*/, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, 1);
}
