#include <iostream>
#include <memory>

#include "../include/WAVReader.h"

void printWaveformTerminal(const std::unique_ptr<WAVReader>& WAVFile);

//--stdlib=libc++
int main(int, char**){
    std::cout << "Hello, from InstrumentAudioVisualiser!\n";

    std::unique_ptr<WAVReader> WAVFile = std::make_unique<WAVReader>("WAVFiles/sample100hz.wav");
    std::cout << "WAV file sampleRate: " << WAVFile->getSampleRate() << '\n';
    std::cout << "WAV file channels: " << WAVFile->getChannels() << '\n';
    std::cout << "WAV file bitsPerSample: " << WAVFile->getBitsPerSample() << '\n';

    int amount{10};
    std::cout << "WAV files first " << amount << " samples: \n";
    std::vector<float> samples = WAVFile->getSamples(amount);
    for (auto sample : samples) {
        std::cout << sample << '\n';
    }

    printWaveformTerminal(WAVFile);
}

void printWaveformTerminal(const std::unique_ptr<WAVReader>& WAVFile) {
    
}
