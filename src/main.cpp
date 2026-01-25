#include <iostream>
#include <memory>
#include <print>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "WAVReader.h"
#include "shader.h"

void printWaveformTerminal(const std::unique_ptr<WAVReader>& WAVFile);
std::vector<float> wavSamplesToVertices(const std::unique_ptr<WAVReader> &WAVFile, int amount, int offset);
void framebufferSize_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH{800};
const unsigned int SCR_HEIGHT{600};
const char* windowName{"Instrument Audio Visualiser"};

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

    std::unique_ptr<WAVReader> WAVFile = std::make_unique<WAVReader>("WAVFiles/sample100hz.wav");
    std::cout << "WAV file sampleRate: " << WAVFile->getSampleRate() << '\n';
    std::cout << "WAV file channels: " << WAVFile->getChannels() << '\n';
    std::cout << "WAV file bitsPerSample: " << WAVFile->getBitsPerSample() << '\n';

    printWaveformTerminal(WAVFile);

    // Move eventually

    auto triangleShader = std::make_unique<Shader>("shaders/triangle.vert", "shaders/triangleFrag.frag");

    std::vector<float> vertices{
        // Positions                        // Colours
        -0.5F, -0.5F, 0.0F,   1.0F, 0.0F, 0.0F,
         0.5F, -0.5F, 0.0F,   0.0F, 1.0F, 0.0F,
        0.0F, 0.5F, 0.0F,   0.0F, 0.0F, 1.0F
    };

    std::vector<float> waveformVertices{wavSamplesToVertices(WAVFile, 441, 50)};

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

    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        triangleShader->use();

        glBindVertexArray(VAO);
        glDrawArrays(GL_LINE_STRIP, 0, 441);

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

std::vector<float> wavSamplesToVertices(const std::unique_ptr<WAVReader> &WAVFile, int amount, int offset) {
    // TODO: Make it a specific aspect ratio for when resizing window

    std::vector<float> samples = WAVFile->getSamples(amount);

    std::vector<float> wavVertices{};
    wavVertices.reserve(samples.size() * 3);

    float normalised_x{};
    size_t sampleSize{samples.size()};
    for (size_t i = 0;i < sampleSize; i += 1) {
        normalised_x = (sampleSize > 1)
            ? (2.0F * static_cast<float>(i) / (sampleSize - 1)) -1.0F
            : 0.0F;
        wavVertices.insert(wavVertices.end(), {normalised_x, samples[i], 0});
    }

    return wavVertices;
}

void printWaveformTerminal(const std::unique_ptr<WAVReader>& WAVFile) {
    // FEATURE: Add support for stereo and non 0DBFS.

    if (WAVFile->getChannels() > 1) {
        std::print("Terminal style waveform supports 1 channel\n");
        return;
    }

    std::vector<float> samples = WAVFile->getSamples(441);

    int dashedAmount{};
    int sampleAmount{};
    int width{30};

    for (auto sample : samples) {
        // PERF: Validate entire entire file or big chunks once before the loop
        if (abs(sample) > 1.0) {
            std::print("Terminal style waveform only support 0dDBFS");
            return;
        }

        sampleAmount = static_cast<int>(sample*static_cast<float>(width));
        dashedAmount = abs(sampleAmount);
        if (sample > 0) {
            std::print("{:>{}}", std::string(dashedAmount, '-'), width);
        } else {
            std::print("{:>{}}", " ", width);
        }
        std::print("|");
        if (sample < 0) {
            sampleAmount = abs(sampleAmount);
            std::print("{:<{}}", std::string(dashedAmount, '-'), width);
        }
        std::print("\n");
    }
}
