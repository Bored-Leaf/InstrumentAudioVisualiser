#include <iostream>
#include <memory>

#include "../include/WAVReader.h"

int main(int, char**){
    std::cout << "Hello, from InstrumentAudioVisualiser!\n";

    std::unique_ptr<WAVReader> WAVFile = std::make_unique<WAVReader>("WAVFiles/sample100hz.wav");
    std::cout << "WAV file sampleRate: " << WAVFile->getSampleRate() << '\n';
}
