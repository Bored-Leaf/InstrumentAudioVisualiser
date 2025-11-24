#include <iostream>
#include <string>

#define DR_WAV_IMPLEMENTATION
#include "../include/WAVReader.h"

WAVReader::WAVReader(const char* filePath) : IAudioSource(), m_wav{} {
    if (!drwav_init_file(&m_wav, filePath, NULL)) {
        std::string errorMsg{"Error opening WAV file at: "};
        throw std::invalid_argument(errorMsg + filePath);
    }

    m_sampleRate = m_wav.sampleRate;

    std::cout << "Read WAV file successfully\n";
}

uint32_t WAVReader::getSampleRate() {
    return m_sampleRate;
}