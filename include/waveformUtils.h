#pragma once

#include <vector>
#include <memory>
#include <WAVReader.h>
#include <print>

namespace WaveformUtils {
    void printWaveformTerminal(const std::unique_ptr<WAVReader>& WAVFile);
    std::vector<float> wavSamplesToVertices(const std::unique_ptr<WAVReader> &WAVFile, int amount, int offset);
    void fillwavVector(std::vector<float> &wavVectorToFill, std::vector<float> &samples, const size_t sampleSize);
    void updateWavVerticies(const std::unique_ptr<WAVReader> &WAVFile, unsigned int VBO, int samplesToAdvance, int amount);
}