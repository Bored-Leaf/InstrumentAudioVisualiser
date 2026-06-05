#pragma once

#include <atomic>
#include <mutex>

#include "shader.h"
#include "CircularBuffer.h"

struct Button {
    float leftX{};
    float rightX{};
    float topY{};
    float bottomY{};

    bool isactive{};
};

struct AppState {
    bool isPlaying{false};
    bool shouldLoop{false};

    Button playButton;
    Button loopButton;
    glm::mat4 uiProjection{1.0F};
    std::unique_ptr<Shader> UIShader;

    CircularBuffer<float> buffer();
    std::atomic<bool> running;
    std::mutex mtx;
};