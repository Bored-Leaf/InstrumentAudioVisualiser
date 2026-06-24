#pragma once

#include <vector>
#include <memory>
#include <WAVReader.h>
#include <print>

namespace WaveformUtils {
    void printWaveformTerminal(const std::unique_ptr<WAVReader>& WAVFile);
    std::vector<float> wavSamplesToVertices(const std::unique_ptr<WAVReader> &WAVFile, int sampleAmount, int samplesToAdvance);
    void fillwavVector(std::vector<float> &wavVectorToFill, const std::vector<float> &samples, size_t amount);
    void updateWavVerticies(unsigned int VBO, const std::vector<float> &waveformVerticies);
}