#include <waveformUtils.h>

#include <glad/glad.h>

std::vector<float> WaveformUtils::wavSamplesToVertices(const std::unique_ptr<WAVReader> &WAVFile, int amount, int offset) {
    // PERF: taking in a reference to a float to reduce allocations each frame
    // TODO: Add boundary checking for end of file samples and pad with 0 when end of sample file
    std::vector<float> samples = WAVFile->getSamples(amount, offset);
    std::vector<float> wavVertices{};

    if (WAVFile->getTotalSampleCount() < amount) {
        wavVertices.reserve(amount * 3);
        fillwavVector(wavVertices, samples, amount);
    } else {
        wavVertices.reserve(samples.size() * 3);
        fillwavVector(wavVertices, samples, amount);
    }

    return wavVertices;
}

void WaveformUtils::fillwavVector(std::vector<float> &wavVectorToFill, std::vector<float> &samples, const size_t totalAmount) {
    float normalised_x{};
    for (size_t i = 0;i < totalAmount;i++) {
        normalised_x = (totalAmount > 1)
            ? (2.0F * static_cast<float>(i) / (totalAmount - 1)) -1.0F
            : 0.0F;
        
        if (i < samples.size()) {
            wavVectorToFill.insert(wavVectorToFill.end(), {normalised_x, samples[i], 0});
        } else {
            wavVectorToFill.insert(wavVectorToFill.end(), {normalised_x, 0.0F, 0});
        }
        }
}

void WaveformUtils::updateWavVerticies(const std::unique_ptr<WAVReader> &WAVFile, unsigned int VBO, int samplesToAdvance, int amount) {
    static int offSet{0};

    offSet+=samplesToAdvance;
    offSet = offSet % WAVFile->getTotalSampleCount();
    offSet = 0;
    std::vector<float> waveformVertices = wavSamplesToVertices(WAVFile, amount, offSet);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, waveformVertices.size() * sizeof(float), waveformVertices.data(), GL_STATIC_DRAW);
}