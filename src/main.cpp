#include <iostream>
#include <memory>
#include <print>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/gtc/matrix_transform.hpp"

#include "WAVReader.h"
#include <waveformUtils.h>
#include "shader.h"

GLFWwindow* setupGLFW();
void printWavFileInfo(const std::unique_ptr<WAVReader> &WAVFile);

void framebufferSize_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouseButton_callback(GLFWwindow* window,int button, int action, int mods);

const float SCR_WIDTH{800.0F};
const float SCR_HEIGHT{600.0F};
const char* windowName{"Instrument Audio Visualiser"};
const int waveformWindow{441 * 30};

bool isPlaying{false}; 
bool shouldLoop{false};

float playbuttonLeftX{};
float playbuttonRightX{};
float playbuttonBottomY{};
float playbuttonTopY{};

float loopbuttonLeftX{};
float loopbuttonRightX{};
float loopbuttonBottomY{};
float loopbuttonTopY{};

glm::mat4 projection = glm::mat4(1.0F);

std::unique_ptr<Shader> UIShader;

int main() {
    GLFWwindow* window{setupGLFW()};
    if (window) {
        std::print("GLFW Window setup successful\n");
    } else {
        std::print("GLFW Window setup unsuccessful\n");
        return -1;
    }

    std::cout << "Hello, from InstrumentAudioVisualiser!\n";

    std::unique_ptr<WAVReader> WAVFile = std::make_unique<WAVReader>("WAVFiles/Ouch-2.wav");

    auto waveformShader = std::make_unique<Shader>("shaders/triangle.vert", "shaders/triangleFrag.frag");
    // auto UIShader = std::make_unique<Shader>("shaders/UI.vert", "shaders/UIFrag.frag");
    UIShader = std::make_unique<Shader>("shaders/UI.vert", "shaders/UIFrag.frag");

    std::vector<float> uiButtonsVerticies{
        // Play Button
        760, 30, 0.0F,
        680, 30, 0.0F,
        760, 90, 0.0F,

        680, 30, 0.0F,
        680, 90, 0.0F,
        760, 90, 0.0F,

        // Loop Button
        760, 120, 0.0F,
        680, 120, 0.0F,
        760, 180, 0.0F,

        680, 120, 0.0F,
        680, 180, 0.0F,
        760, 180, 0.0F
    };
    // Play
    playbuttonLeftX   = 680;
    playbuttonRightX  = 760;
    playbuttonTopY    = 30;
    playbuttonBottomY = 90;

    // Loop
    loopbuttonLeftX   = 680;
    loopbuttonRightX  = 760;
    loopbuttonTopY    = 120;
    loopbuttonBottomY = 180;

    std::vector<float> waveformVertices{WaveformUtils::wavSamplesToVertices(WAVFile, waveformWindow, 0)};

    unsigned int waveformVAO{};
    unsigned int UIVAO{};
    unsigned int waveformVBO{};
    unsigned int uiButtonsVBO{};

    glGenVertexArrays(1, &waveformVAO);
    glGenVertexArrays(1, &UIVAO);
    glGenBuffers(1, &waveformVBO);
    glGenBuffers(1, &uiButtonsVBO);

    glBindVertexArray(waveformVAO);

    glBindBuffer(GL_ARRAY_BUFFER, waveformVBO);
    glBufferData(GL_ARRAY_BUFFER, waveformVertices.size() * sizeof(float), waveformVertices.data(), GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(UIVAO);
    glBindBuffer(GL_ARRAY_BUFFER, uiButtonsVBO);
    glBufferData(GL_ARRAY_BUFFER, uiButtonsVerticies.size() * sizeof(float), uiButtonsVerticies.data(), GL_STATIC_DRAW);

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

    bool currentButtonIsPlay{};

    int offset{};
    int totalOffset{};

    projection = glm::ortho(0.0F, SCR_WIDTH, SCR_HEIGHT, 0.0F);

    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        // Delta time
        currentFrame = static_cast<float>(glfwGetTime());
        dtTime = currentFrame - previousFrame;
        previousFrame = currentFrame;
        
        // UI functionality
        if (totalOffset > WAVFile->getTotalSampleCount()) {
            if (!shouldLoop) {
                isPlaying = false;
            }
            totalOffset = 0;
        }
        if (isPlaying) {
            // Update Vertecies by amount of time passed
            samplesToAdvance = sampleRate * dtTime;
            // Get Fractional part
            fractionalLoss += samplesToAdvance - static_cast<int>(samplesToAdvance);
            // Add missed Integral fractionalLoss if any
            offset = static_cast<int>(samplesToAdvance) + static_cast<int>(fractionalLoss);
            totalOffset += offset;
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
        UIShader->setMat4("projection", projection);

        glBindVertexArray(UIVAO);
        UIShader->setBool("playing", isPlaying);
        UIShader->setBool("currentButtonPlay", true);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        UIShader->setBool("playing", false);
        UIShader->setBool("looping", shouldLoop);
        UIShader->setBool("currentButtonPlay", false);
        glDrawArrays(GL_TRIANGLES, 6, 6);

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

void printWavFileInfo(const std::unique_ptr<WAVReader> &WAVFile) {
    std::cout << "WAV file sampleRate: " << WAVFile->getSampleRate() << '\n';
    std::cout << "WAV file channels: " << WAVFile->getChannels() << '\n';
    std::cout << "WAV file bitsPerSample: " << WAVFile->getBitsPerSample() << '\n';
    WaveformUtils::printWaveformTerminal(WAVFile);
}

void framebufferSize_callback(GLFWwindow* /*window*/, int width, int height) {
    glViewport(0, 0, width, height);

    projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, 1);
}

void mouseButton_callback(GLFWwindow* window,int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos{};
        double ypos{};
        glfwGetCursorPos(window, &xpos, &ypos);
        // playButton pressed
        if (xpos > playbuttonLeftX && xpos < playbuttonRightX &&
            ypos > playbuttonTopY && ypos < playbuttonBottomY) {
                isPlaying = true;
            }
        // loopButton pressed
        if (xpos > loopbuttonLeftX && xpos < loopbuttonRightX &&
            ypos > loopbuttonTopY && ypos < loopbuttonBottomY) {
                shouldLoop = !shouldLoop;
            }
        // Use uniforms for when playing so its red and can't press again
        // Use uniforms for when mouse is hovering over the button. (Probably have to use a 
        // seperate callback for when mouse pos moves)
    }
}
