#pragma once

#include <vector>
#include <complex>

namespace fft_utils {
    void createBars(std::vector<float> &barVertexData, const std::vector<std::complex<float>> &fftValues);
    [[nodiscard]] std::vector<float> createBar(float left, float right, float bottom, float top);
    void updateFFTVertices(unsigned int VBO, const std::vector<float> &fftBarVertices);
}
