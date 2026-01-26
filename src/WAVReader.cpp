#include <iostream>


#define DR_WAV_IMPLEMENTATION
#include "../include/WAVReader.h"

// ========== Public ==========

WAVReader::WAVReader(const std::string& filePath) 
                : m_wav{}
                , m_sampleRate{}
                , m_channels{}
                , m_bitsPerSample{} {
    if (!drwav_init_file(&m_wav, filePath.c_str(), NULL)) {
        std::string errorMsg{"Error opening WAV file at: "};
        throw std::invalid_argument(errorMsg + filePath);
    }

    m_sampleRate = m_wav.sampleRate;
    m_channels = m_wav.channels;
    m_bitsPerSample = m_wav.bitsPerSample;

    loadPcmData();

    std::cout << "Read WAV file successfully\n";
}

uint32_t WAVReader::getSampleRate() const {
    return m_sampleRate;
}

uint16_t WAVReader::getChannels() const {
    return m_channels;
}

uint16_t WAVReader::getBitsPerSample() const {
    return m_bitsPerSample;
}

uint32_t WAVReader::getTotalSampleCount() const {
    return audioData.size();
}

std::vector<float> WAVReader::getSamples(int amount) {
    // TODO: Add boundary checking for end of file samples and pad with 0s
    std::vector<float> samples(amount);

    for (int i = 0; i < amount; ++i) {
        samples[i] = audioData[i];
    }
    
    return samples;
}

std::vector<float> WAVReader::getSamples(int amount, int offset) {
    std::vector<float> samples(amount);

    if (amount + offset > getTotalSampleCount()) {
        samples.resize(getTotalSampleCount() - offset);
    }

    for (int i = 0 + offset; i < amount + offset; ++i) {
        samples[i - offset] = audioData[i];
    }
    
    return samples;
}

// ========== Private ==========

void WAVReader::loadPcmData() {
    uint64_t totalFrames = m_wav.totalPCMFrameCount;
    audioData.resize(totalFrames * m_channels);

    drwav_read_pcm_frames_f32(&m_wav, totalFrames, audioData.data());
}

// drwav_read_pcm_frames_f32() to get pcm samples
/*Use something like:
unsigned long long totalFrames = wav.totalPCMFrameCount;
std::vector<float> audioData(totalFrames * wav.channels);

drwav_uint64 framesRead = drwav_read_pcm_frames_f32(&wav, totalFrames, audioData
to store data in std::vector instead of using float*
*/