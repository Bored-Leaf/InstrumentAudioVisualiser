#include <memory>
#include <mutex>
#include <print>
#include <numbers>
#include <thread>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "constants.hpp"
#include "waveformUtils.h"
#include "fft_utils.hpp"
#include "FFT.h"
#include "AppState.h"

#include "renderer/renderer.hpp"
#include "renderer/waveform_visualisation.hpp"

GLFWwindow* setupGLFW();

void framebufferSize_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouseButton_callback(GLFWwindow* window,int button, int action, int mods);

void audioWorker(AppState& state);

// TODO: use glfwGetWindowUserPointer to pass it instead of global struct
AppState appState;

int main() {    
    GLFWwindow* window{setupGLFW()};
    if (window) {
        std::print("GLFW Window setup successful\n");
    } else {
        std::print("GLFW Window setup unsuccessful\n");
        return -1;
    }

    WaveformVisualisation waveformVis("shaders/waveformVert.vert", "shaders/waveformFrag.frag");
    FFTVisualisation fftVis("shaders/fftVert.vert", "shaders/fftFrag.frag");
    Renderer renderer{waveformVis, fftVis};

    RealAppState realAppState(renderer);

    // NEXT: Figure way to move this to setupGLFW(). Maybe make renderer.init() have the visualisations setup
    // their m_shaders in their init() instead of during object instantiation?
    glfwSetWindowUserPointer(window, &realAppState);

    appState.WAVFile = std::make_unique<WAVReader>("WAVFiles/Ouch-2.wav");

    // Move to UI implementation
    appState.UIShader = std::make_unique<Shader>("shaders/UI.vert", "shaders/UIFrag.frag");

    std::vector<float> uiButtonsVerticies{
        // Play Button
        760, 570, 0.0F,
        680, 570, 0.0F,
        760, 510, 0.0F,

        680, 570, 0.0F,
        680, 510, 0.0F,
        760, 510, 0.0F,

        // Loop Button
        760, 480, 0.0F,
        680, 480, 0.0F,
        760, 420, 0.0F,

        680, 480, 0.0F,
        680, 420, 0.0F,
        760, 420, 0.0F
    };

    // Play button
    appState.playButton = {.leftX=680, .rightX=760, .topY=30, .bottomY=90, .isactive=false};

    // Loop button
    appState.loopButton = {.leftX=680, .rightX=760, .topY=120, .bottomY=190, .isactive=false};

    std::vector<float> initWaveformVertexData{waveform_utils::wavSamplesToVertices(appState.WAVFile, constants::WAVEFORM_WINDOW, 0)};
    std::vector<float> initBarVertexData{};
    std::vector<std::complex<float>> initFFTZeroValues(constants::FFT_WINDOW);
    fft_utils::createBars(initBarVertexData, initFFTZeroValues);

    renderer.init(initWaveformVertexData, initBarVertexData);
    unsigned int waveformVBO{renderer.getWaveformVis()->getVBO()};
    unsigned int fftVBO{renderer.getFFTVis()->getVBO()};

    unsigned int UIVAO{};
    unsigned int uiButtonsVBO{};

    glGenVertexArrays(1, &UIVAO);
    glGenBuffers(1, &uiButtonsVBO);

    glBindVertexArray(UIVAO);
    glBindBuffer(GL_ARRAY_BUFFER, uiButtonsVBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(uiButtonsVerticies.size() * sizeof(float)), uiButtonsVerticies.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
    glEnableVertexAttribArray(0);

    appState.uiProjection = glm::ortho(0.0F, constants::SCR_WIDTH, 0.0F, constants::SCR_HEIGHT);

    std::thread audioThread(audioWorker, std::ref(appState));
    
    appState.running = true;
    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        // UI functionality
        // BUG: Only seems to work on Ouch-2.wav
        if (appState.isPlaying) {
            std::vector<float> waveformVerticies{};
            std::vector<std::complex<float>> fftOutput{};
            int sampleAmount{constants::WAVEFORM_WINDOW};

            {   // waveform
                std::lock_guard<std::mutex> lock(appState.mtx);
                bool success{appState.waveformBuffer.read(waveformVerticies, sampleAmount * 3)};
                if (success) {
                    // CLEANUP: move to waveformVis or something nice
                    waveform_utils::updateWavVerticies(waveformVBO, waveformVerticies);
                } else {
                    // std::print("Buffer is full, won't write\n");
                }
            }

            {   // fft
                std::lock_guard<std::mutex> lock(appState.mtx);
                bool success{appState.fftBuffer.read(fftOutput, constants::FFT_WINDOW)};
                if (success) {
                    // CLEANUP: move to fftVis or something nice
                    std::vector<float> barData{};
                    fft_utils::createBars(barData, fftOutput);
                    fft_utils::updateFFTVertices(fftVBO, barData);
                }
                
            }
        }

        renderer.update();

        // UI
        // obviously in the ui implementation
        glViewport(0, 0, 800, 600);
        appState.UIShader->use();
        appState.UIShader->setMat4("projection", appState.uiProjection);

        glBindVertexArray(UIVAO);
        appState.UIShader->setBool("playing", appState.isPlaying);
        appState.UIShader->setBool("currentButtonPlay", true);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        appState.UIShader->setBool("playing", false);
        appState.UIShader->setBool("looping", appState.shouldLoop);
        appState.UIShader->setBool("currentButtonPlay", false);
        glDrawArrays(GL_TRIANGLES, 6, 6);

        glBindVertexArray(0);

        GLenum err{};
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::print(stderr , "OpenGL Error >> {}\n", err);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    appState.running = false;
    audioThread.join();

    // TODO: Renderer::cleanup() is called on destructor
    appState.UIShader->deleteShader();

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

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(static_cast<int>(constants::SCR_WIDTH), static_cast<int>(constants::SCR_HEIGHT), constants::WINDOW_NAME, nullptr, nullptr);
    if (window == nullptr) {
        std::print(stderr, "Failed to create GLFW Window");
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSize_callback);
    glfwSetMouseButtonCallback(window, mouseButton_callback);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::print(stderr, "Failed to initialise GLAD");
        glfwTerminate();
        return nullptr;
    }

    glEnable(GL_DEPTH_TEST);

    return window;
}

void framebufferSize_callback(GLFWwindow* window, int width, int height) {
    auto *realAppState = static_cast<RealAppState*>(glfwGetWindowUserPointer(window));
    realAppState->renderer.onResize(width, height);

    appState.uiProjection = glm::ortho(0.0F, static_cast<float>(width), 0.0F, static_cast<float>(height));
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, 1);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void mouseButton_callback(GLFWwindow* window,int button, int action, int /*mods*/) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos{};
        double ypos{};
        glfwGetCursorPos(window, &xpos, &ypos);
        // playButton pressed
        if (xpos > appState.playButton.leftX && xpos < appState.playButton.rightX &&
            ypos > appState.playButton.topY && ypos < appState.playButton.bottomY) {
                appState.isPlaying = true;
            }
        // loopButton pressed
        if (xpos > appState.loopButton.leftX && xpos < appState.loopButton.rightX &&
            ypos > appState.loopButton.topY && ypos < appState.loopButton.bottomY) {
                appState.shouldLoop = !appState.shouldLoop;
            }

        // POLISH: Use uniforms for when mouse is hovering over the button.
        
    }
}

void audioWorker(AppState& state) {
    float currentFrame{};
    float previousFrame{};

    // waveform
    float       fractionalLoss{};
    uint        totalOffset{};
    uint32_t    sampleRate{state.WAVFile->getSampleRate()};

    // fft
    std::vector<std::complex<float>>    fftInput{};
    std::vector<float>                  newSamples{};

    // make call when fft_window size changes
    fft::generateHannCoefficients(constants::FFT_WINDOW);

    while(state.running) {
        currentFrame = static_cast<float>(glfwGetTime());
        float dtTime = currentFrame - previousFrame;
        previousFrame = currentFrame;
        
        // UI functionality
        if (totalOffset > state.WAVFile->getTotalSampleCount()) {
            if(!state.shouldLoop) {
                state.isPlaying = false;
            }
            totalOffset = 0;
        }
        if (state.isPlaying) {
            // Waveform sample pushing to GPU
            float samplesToAdvance = sampleRate * dtTime;
            //Get fractional part
            fractionalLoss = samplesToAdvance - static_cast<int>(samplesToAdvance);
            int offset = static_cast<int>(samplesToAdvance) + static_cast<int>(fractionalLoss);
            totalOffset += offset;

            // Generate and send vertex data to GPU
            int sampleAmount{constants::WAVEFORM_WINDOW};
            std::vector<float> waveformVerticies = waveform_utils::wavSamplesToVertices(state.WAVFile, sampleAmount, offset);
            
            if (fractionalLoss > 1.0F) {
                fractionalLoss -= 1;
            }

            { 
                std::lock_guard<std::mutex> lock(state.mtx);
                bool success{state.waveformBuffer.write(waveformVerticies)};
                if (!success) {
                    // std::print("Buffer is full, won't write!\n");
                }
            }

            // FFT computation and pushing
            std::vector<float> samplesToAdd{state.WAVFile->getSamples(sampleRate * dtTime)};
            newSamples.insert(newSamples.begin(), samplesToAdd.begin(), samplesToAdd.end());

            if (newSamples.size() >= constants::FFT_WINDOW) {
                std::vector<float> newSamplesToAdd{};
                // CLEANUP: What the hell, clean this up, a billion vectors for one thing jesus
                newSamplesToAdd.insert(newSamplesToAdd.begin(), newSamples.begin(), newSamples.begin() + constants::FFT_WINDOW);
                fft::applyHannWindow(newSamplesToAdd);
                fftInput.insert(fftInput.begin(), newSamplesToAdd.begin(), newSamplesToAdd.begin() + constants::FFT_WINDOW);
                {
                    std::lock_guard<std::mutex> lock(state.mtx);
                    bool success(state.fftBuffer.write(fft::compute(fftInput)));
                    if (!success) {
                        std::print("fft computation has too many elements in input. input should only have {} elements "
                                "not {}!!\n", constants::FFT_WINDOW, fftInput.size());
                    }
                }
                fftInput.erase(fftInput.begin(), fftInput.begin() + constants::FFT_WINDOW);
                newSamples.erase(newSamples.begin(), newSamples.begin() + constants::FFT_WINDOW);
            }
        }

        // BUG: Needed or else waveform will stop at weird place
        // Assume mismatch between main thread and this thread from
        // different read/write speeds
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}
