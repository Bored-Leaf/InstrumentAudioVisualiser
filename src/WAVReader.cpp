#include <iostream>


#define DR_WAV_IMPLEMENTATION
#include "../include/WAVReader.h"

WAVReader::WAVReader(const std::string &filePath) : m_wav{} {
    if (!drwav_init_file(&m_wav, filePath.c_str(), NULL)) {
        std::string errorMsg{"Error opening WAV file at: "};
        throw std::invalid_argument(errorMsg + filePath);
    }

    m_sampleRate = m_wav.sampleRate;

    std::cout << "Read WAV file successfully\n";
}

uint32_t WAVReader::getSampleRate() {
    return m_sampleRate;
}