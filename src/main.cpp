#include <iostream>
#include <memory>
#include <print>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../include/WAVReader.h"

void printWaveformTerminal(const std::unique_ptr<WAVReader>& WAVFile);
void framebufferSize_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const unsigned int SCR_WIDTH{800};
    const unsigned int SCR_HEIGHT{600};
    const char* windowName{"Instrument Audio Visualiser"};

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

    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        std::print("Hello");

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    std::cout << "Hello, from InstrumentAudioVisualiser!\n";

    std::unique_ptr<WAVReader> WAVFile = std::make_unique<WAVReader>("WAVFiles/sample100hz.wav");
    std::cout << "WAV file sampleRate: " << WAVFile->getSampleRate() << '\n';
    std::cout << "WAV file channels: " << WAVFile->getChannels() << '\n';
    std::cout << "WAV file bitsPerSample: " << WAVFile->getBitsPerSample() << '\n';

    printWaveformTerminal(WAVFile);

    return 0;
}

void framebufferSize_callback(GLFWwindow* /*window*/, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, 1);
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
