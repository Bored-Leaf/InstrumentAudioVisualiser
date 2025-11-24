#include <cstdint>

class IAudioSource {
public:
    virtual ~IAudioSource(){};  // Move to cpp file if body needs content
    virtual uint32_t getSampleRate() = 0;

protected:
    IAudioSource() : m_sampleRate{} {}

    uint32_t m_sampleRate;
};