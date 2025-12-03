#include <cstdint>

class IAudioSource {
public:
    virtual ~IAudioSource(){};  // Move to cpp file if body needs content
    
    virtual uint32_t getSampleRate() const = 0;
    virtual uint16_t getChannels() const = 0;
    virtual uint16_t getBitsPerSample() const = 0;
};