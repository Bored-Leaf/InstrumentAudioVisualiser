#pragma once

#include <string>
#include <vector>

#include "IAudioSource.h"
#include "dr_wav.h"

class WAVReader : public IAudioSource {
public:
    WAVReader(const std::string& filePath);
    ~WAVReader(){}
    
    uint32_t getSampleRate() const override;
    uint16_t getChannels() const override;
    uint16_t getBitsPerSample() const override;

    std::vector<float> getSamples(int amount);
    std::vector<float> getSamples(int amount, int offset);
private:
    drwav m_wav;
    uint32_t m_sampleRate;
    uint16_t m_channels;
    uint16_t m_bitsPerSample;
    std::vector<float> audioData;

    void loadPcmData();
};