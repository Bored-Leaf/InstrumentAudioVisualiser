#include "renderer.hpp"

Renderer::~Renderer() {
    cleanup();
}

void Renderer::init(const std::vector<float> &initVertexData) {
    waveformVis->init(initVertexData);
}

void Renderer::update() {
    //NEXT: Need way to get projection from regions
    //waveformVis->draw();
}

void Renderer::cleanup() {
    waveformVis->cleanup();
}