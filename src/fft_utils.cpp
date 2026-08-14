#include "fft_utils.hpp"
#include "constants.hpp"
#include <FFT.h>

#include <glad/glad.h>

#include <print>

namespace fft_utils {
    void createBars(std::vector<float> &barVertexData, const std::vector<std::complex<float>> &fftValues) {
        // TODO: Remove when moved into fft_visualisation.cpp
        int m_numBins{(constants::FFT_WINDOW / 2) + 1};

        float barWidth{constants::SCR_WIDTH / m_numBins};
        // TODO add bar space functionality
        // float spaceBetweenBars{0};

        for (int currentbar = 1;currentbar <= m_numBins;currentbar++) {
            float left{barWidth * (static_cast<float>(currentbar) - 1)};
            float right{barWidth * static_cast<float>(currentbar)};
            float bottom{0};
            float magnitude{std::abs(fftValues[currentbar])};
            float top{fft::normaliseMagnitude(magnitude) * (constants::SCR_HEIGHT * 0.95F)};

            std::vector<float> bar{fft_utils::createBar(left, right, bottom, top)};
            barVertexData.insert(barVertexData.end(), bar.begin(), bar.end());
        }
    }

    std::vector<float> createBar(const float left, const float right, const float bottom, const float top) {
        return {left, bottom, 0,
                right, top, 0,
                left, top, 0,
                left, bottom, 0,
                right, bottom, 0,
                right, top, 0};
    }

    void updateFFTVertices(const unsigned int VBO, const std::vector<float> &fftBarVertices) {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, fftBarVertices.size() * sizeof(float), fftBarVertices.data(), GL_DYNAMIC_DRAW);
    }
}