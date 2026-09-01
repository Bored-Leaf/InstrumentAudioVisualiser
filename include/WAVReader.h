#pragma once

#include <string>
#include <vector>

#include "IAudioSource.h"
#include "dr_wav.h"

class WAVReader : public IAudioSource {
public:
    WAVReader(const std::string& filePath);
    ~WAVReader(){}
    
    [[nodiscard]] uint32_t getSampleRate() const override;
    [[nodiscard]] uint16_t getChannels() const override;
    [[nodiscard]] uint16_t getBitsPerSample() const override;
    [[nodiscard]] uint32_t getTotalSampleCount() const;

    [[nodiscard]] std::vector<float> getSamples(int amount);
    [[nodiscard]] std::vector<float> getSamplesOffset(int amount, int offset);
private:
    drwav m_wav;
    uint32_t m_sampleRate;
    uint16_t m_channels;
    uint16_t m_bitsPerSample;
    std::vector<float> audioData;

    void loadPcmData();
};