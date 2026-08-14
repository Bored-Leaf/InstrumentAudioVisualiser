#include <print>

#include "constants.hpp"
#include <waveformUtils.h>

#include <glad/glad.h>

namespace waveform_utils {
    std::vector<float> wavSamplesToVertices(const std::unique_ptr<WAVReader> &WAVFile, const int sampleAmount, const int samplesToAdvance) {
        // CRIT: only call from one thread else offset would get messy
        static int offSet{0};

        offSet+=samplesToAdvance;
        // Prevent accumulating offSet when stopping
        if (offSet > WAVFile->getTotalSampleCount()) {
            offSet = 0;
        }

        // TODO: Use CirulcarBuffer to write data to
        std::vector<float> samples = WAVFile->getSamplesOffset(sampleAmount, offSet);
        std::vector<float> wavVertices{};

        if (WAVFile->getTotalSampleCount() < sampleAmount) {
            wavVertices.reserve(sampleAmount * 3);
            fillwavVector(wavVertices, samples, sampleAmount);
        } else {
            wavVertices.reserve(samples.size() * 3);
            fillwavVector(wavVertices, samples, sampleAmount);
        }

        return wavVertices;
    }

    void fillwavVector(std::vector<float> &wavVectorToFill, const std::vector<float> &samplesToUse, size_t amount) {
        // BUG: weird line
        constexpr float amplitude{2.0F};

        for (size_t i = 0;i < amount;i++) {
            float x{(amount > 1)
                ? (static_cast<float>(constants::SCR_WIDTH) * static_cast<float>(i) / (amount - 1))
                : 0.0F};
            if (i < samplesToUse.size()) {
                float y{(samplesToUse[i] > -1)
                ? ((samplesToUse[i] * amplitude) + 1.0F) * (constants::SCR_HEIGHT / 2.0F)
                : 0.0F};
                float z{0};
                wavVectorToFill.insert(wavVectorToFill.end(), {x, y, z});
            } else {
                wavVectorToFill.insert(wavVectorToFill.end(), {x, 0.0F, 0.0F});
            }
        }
    }

    void updateWavVerticies(const unsigned int VBO, const std::vector<float> &waveformVerticies) {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, waveformVerticies.size() * sizeof(float), waveformVerticies.data(), GL_DYNAMIC_DRAW);
    }
}