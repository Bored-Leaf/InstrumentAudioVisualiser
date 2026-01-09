#include <iostream>
#include <memory>
#include <print>

#include "../include/WAVReader.h"

void printWaveformTerminal(const std::unique_ptr<WAVReader>& WAVFile);

int main(int, char**){
    std::cout << "Hello, from InstrumentAudioVisualiser!\n";

    std::unique_ptr<WAVReader> WAVFile = std::make_unique<WAVReader>("WAVFiles/sample100hz.wav");
    std::cout << "WAV file sampleRate: " << WAVFile->getSampleRate() << '\n';
    std::cout << "WAV file channels: " << WAVFile->getChannels() << '\n';
    std::cout << "WAV file bitsPerSample: " << WAVFile->getBitsPerSample() << '\n';

    printWaveformTerminal(WAVFile);
}

void printWaveformTerminal(const std::unique_ptr<WAVReader>& WAVFile) {

    if (WAVFile->getChannels() > 1) {
        std::print("Terminal style waveform supports 1 channel\n");
        return;
    }

    std::vector<float> samples = WAVFile->getSamples(441);

    int dashedAmount{};
    int sampleAmount{};
    int width{30};

    for (auto sample : samples) {

        if (abs(sample) > 1.0 || abs(sample) < -1.0) {
            std::print("Terminal style waveform only support 0dDBFS");
            return;
        }

        sampleAmount = static_cast<int>(sample*(float)width);
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
