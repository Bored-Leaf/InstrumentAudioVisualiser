#include <iostream>
#include <memory>
#include <print>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "WAVReader.h"
#include <waveformUtils.h>
#include "shader.h"

GLFWwindow* setupGLFW();

void framebufferSize_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouseButton_callback(GLFWwindow* window,int button, int action, int mods);

const unsigned int SCR_WIDTH{800};
const unsigned int SCR_HEIGHT{600};
const char* windowName{"Instrument Audio Visualiser"};
const int waveformWindow{441 * 50};

int main() {
    GLFWwindow* window{setupGLFW()};
    if (window) {
        std::print("GLFW Window setup successful\n");
    } else {
        std::print("GLFW Window setup unsuccessful\n");
        return -1;
    }
    // Move eventually

    std::cout << "Hello, from InstrumentAudioVisualiser!\n";

    std::unique_ptr<WAVReader> WAVFile = std::make_unique<WAVReader>("WAVFiles/Ouch-2.wav");
    std::cout << "WAV file sampleRate: " << WAVFile->getSampleRate() << '\n';
    std::cout << "WAV file channels: " << WAVFile->getChannels() << '\n';
    std::cout << "WAV file bitsPerSample: " << WAVFile->getBitsPerSample() << '\n';

    // WaveformUtils::printWaveformTerminal(WAVFile);

    // Move eventually

    auto waveformShader = std::make_unique<Shader>("shaders/triangle.vert", "shaders/triangleFrag.frag");
    auto UIShader = std::make_unique<Shader>("shaders/UI.vert", "shaders/UIFrag.frag");

    std::vector<float> playButtonVerticies{
        0.9F, 0.9F, 0.0F,
        0.7F, 0.9F, 0.0F,
        0.9F, 0.7, 0.0F,

        0.7F, 0.9F, 0.0F,
        0.7F, 0.7F, 0.0F,
        0.9F, 0.7F, 0.0F
    };

    std::vector<float> waveformVertices{WaveformUtils::wavSamplesToVertices(WAVFile, waveformWindow, 0)};

    unsigned int waveformVAO{};
    unsigned int UIVAO{};
    unsigned int waveformVBO{};
    unsigned int playButtonVBO{};

    glGenVertexArrays(1, &waveformVAO);
    glGenVertexArrays(1, &UIVAO);
    glGenBuffers(1, &waveformVBO);
    glGenBuffers(1, &playButtonVBO);

    glBindVertexArray(waveformVAO);

    glBindBuffer(GL_ARRAY_BUFFER, waveformVBO);
    glBufferData(GL_ARRAY_BUFFER, waveformVertices.size() * sizeof(float), waveformVertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(UIVAO);
    glBindBuffer(GL_ARRAY_BUFFER, playButtonVBO);
    glBufferData(GL_ARRAY_BUFFER, playButtonVerticies.size() * sizeof(float), playButtonVerticies.data(), GL_STATIC_DRAW);

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

    bool isPlaying{false};
    // bool shouldLoop{false};
    int offset{};

    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        // Delta time
        currentFrame = static_cast<float>(glfwGetTime());
        dtTime = currentFrame - previousFrame;
        previousFrame = currentFrame;

        // UI functionality
        if (isPlaying) {
            // Update Vertecies by amount of time passed
            samplesToAdvance = sampleRate * dtTime;
            // Get Fractional part
            fractionalLoss += samplesToAdvance - static_cast<int>(samplesToAdvance);
            // Add missed Integral fractionalLoss if any
            offset = static_cast<int>(samplesToAdvance) + static_cast<int>(fractionalLoss);
            // Send vertex data to GPU
            WaveformUtils::updateWavVerticies(WAVFile, waveformVBO, offset, waveformWindow);    
            
            // The Integral has been added above so remove
            if (fractionalLoss > 1.0F) {
                fractionalLoss -= 1;
            }
        }

        

        glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        waveformShader->use();

        glLineWidth(2.0F);
        glBindVertexArray(waveformVAO);
        glDrawArrays(GL_LINE_STRIP, 0, waveformWindow);

        UIShader->use();

        glBindVertexArray(UIVAO);
        glDrawArrays(GL_TRIANGLES, 0, playButtonVerticies.size());

        glBindVertexArray(0);

        GLenum err{};
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::print(stderr , "OpenGL Error >> {}\n", err);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &waveformVAO);
    glDeleteBuffers(1, &waveformVBO);
    waveformShader->deleteShader();
    UIShader->deleteShader();

    glfwTerminate();

    return 0;
}

GLFWwindow* setupGLFW() {
    glfwSetErrorCallback([](int error, const char* description) {
        std::print(stderr, "GLFW Error ({}): {}\n", error, description);
    });

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, windowName, nullptr, nullptr);
    if (window == nullptr) {
        std::print(stderr, "Failed to create GLFW Window");
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSize_callback);
    glfwSetMouseButtonCallback(window, mouseButton_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::print(stderr, "Failed to initialise GLAD");
        glfwTerminate();
        return nullptr;
    }

    return window;
}

void framebufferSize_callback(GLFWwindow* /*window*/, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, 1);
}

void mouseButton_callback(GLFWwindow* window,int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos{};
        double ypos{};
        glfwGetCursorPos(window, &xpos, &ypos);
        std::print("x: {}, y: {}\n", xpos, ypos);
        // Find if clicked on play button then play
        // Use uniforms for when playing so its red and can't press again
        // Use uniforms for when mouse is hovering over the button. (Probably have to use a 
        // seperate callback for when mouse pos moves)
    }
}
