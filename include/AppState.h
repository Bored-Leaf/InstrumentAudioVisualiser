#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <complex>

#include "constants.hpp"
#include "shader.h"
#include "CircularBuffer.h"
#include "WAVReader.h"

#include "renderer/renderer.hpp"

struct Button {
    float leftX{};
    float rightX{};
    float topY{};
    float bottomY{};

    bool isactive{};
};

struct AppState {
    AppState() : waveformBuffer(131072, "waveformBuffer")
               , fftBuffer(1024, "fftBuffer") {}

    bool isPlaying{false};
    bool shouldLoop{false};

    Button playButton;
    Button loopButton;
    glm::mat4 uiProjection{1.0F};
    std::unique_ptr<Shader> UIShader;

    std::unique_ptr<WAVReader> WAVFile;

    CircularBuffer<float> waveformBuffer;
    CircularBuffer<std::complex<float>> fftBuffer;
    std::atomic<bool> running;
    std::mutex mtx;

    uint buttonsVBO;
    uint buttonsVAO;

    int windowWidth{static_cast<int>(constants::SCR_WIDTH)};
    int windowHeight{static_cast<int>(constants::SCR_HEIGHT)};
};

struct RealAppState {
    RealAppState(Renderer &inRenderer) : renderer(inRenderer) {}
    RealAppState(const RealAppState&) = delete;

    Renderer& renderer;
};