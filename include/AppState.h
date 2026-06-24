#pragma once

#include <atomic>
#include <memory>
#include <mutex>

#include "shader.h"
#include "CircularBuffer.h"
#include "WAVReader.h"

struct Button {
    float leftX{};
    float rightX{};
    float topY{};
    float bottomY{};

    bool isactive{};
};

struct AppState {
    AppState() : buffer(131072){}

    bool isPlaying{false};
    bool shouldLoop{false};

    Button playButton;
    Button loopButton;
    glm::mat4 uiProjection{1.0F};
    std::unique_ptr<Shader> UIShader;

    std::unique_ptr<WAVReader> WAVFile;

    CircularBuffer<float> buffer;
    std::atomic<bool> running;
    std::mutex mtx;
};