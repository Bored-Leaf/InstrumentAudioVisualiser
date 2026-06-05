#include <waveformUtils.h>

#include <glad/glad.h>

// Used for verifying opengl rendering waveform correctly
void WaveformUtils::printWaveformTerminal(const std::unique_ptr<WAVReader>& WAVFile) {
    if (WAVFile->getChannels() > 1) {
        std::print("Terminal style waveform supports 1 channel\n");
        return;
    }

    std::vector<float> samples = WAVFile->getSamples(441);

    int dashedAmount{};
    int sampleAmount{};
    int width{30};

    for (auto sample : samples) {
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

std::vector<float> WaveformUtils::wavSamplesToVertices(const std::unique_ptr<WAVReader> &WAVFile, const int sampleAmount, const int samplesToAdvance) {
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

void WaveformUtils::fillwavVector(std::vector<float> &wavVectorToFill, const std::vector<float> &samplesToUse, size_t amount) {
    float normalised_x{};
    for (size_t i = 0;i < amount;i++) {
        normalised_x = (amount > 1)
            ? (2.0F * static_cast<float>(i) / (amount - 1)) -1.0F
            : 0.0F;
        
        if (i < samplesToUse.size()) {
            wavVectorToFill.insert(wavVectorToFill.end(), {normalised_x, samplesToUse[i], 0});
        } else {
            wavVectorToFill.insert(wavVectorToFill.end(), {normalised_x, 0.0F, 0});
        }
        }
}

void WaveformUtils::updateWavVerticies(const int VBO, const std::vector<float> &waveformVerticies) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, waveformVerticies.size() * sizeof(float), waveformVerticies.data(), GL_STATIC_DRAW);
}