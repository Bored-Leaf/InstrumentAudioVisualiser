#include <waveformUtils.h>

#include <glad/glad.h>

void WaveformUtils::printWaveformTerminal(const std::unique_ptr<WAVReader>& WAVFile) {
    // FEATURE: Add support for stereo and non 0DBFS.

    if (WAVFile->getChannels() > 1) {
        std::print("Terminal style waveform supports 1 channel\n");
        return;
    }

    std::vector<float> samples = WAVFile->getSamples(441);

    int dashedAmount{};
    int sampleAmount{};
    int width{30};

    for (auto sample : samples) {
        // PERF: Validate entire entire file or big chunks once before the loop
        if (abs(sample) > 1.0) {
            std::print("Terminal style waveform only support 0dDBFS");
            return;
        }

        sampleAmount = static_cast<int>(sample*static_cast<float>(width));
        dashedAmount = abs(sampleAmount);
        if (sample > 0) {
            std::print("{:>{}}", std::string(dashedAmount, '-'), width);
        } else {
            std::print("{:>{}}", " ", width);
        }
        std::print("|");
        if (sample < 0) {
            sampleAmount = abs(sampleAmount);
            std::print("{:<{}}", std::string(dashedAmount, '-'), width);
        }
        std::print("\n");
    }
}

std::vector<float> WaveformUtils::wavSamplesToVertices(const std::unique_ptr<WAVReader> &WAVFile, int amount, int offset) {
    // PERF: taking in a reference to a float to reduce allocations each frame
    std::vector<float> samples = WAVFile->getSamplesOffset(amount, offset);
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
    std::vector<float> waveformVertices = wavSamplesToVertices(WAVFile, amount, offSet);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, waveformVertices.size() * sizeof(float), waveformVertices.data(), GL_STATIC_DRAW);
}