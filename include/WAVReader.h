#include <string>

#include "IAudioSource.h"
#include "dr_wav.h"

class WAVReader : public IAudioSource {
public:
    WAVReader(const std::string &filePath);
    virtual ~WAVReader(){}
    uint32_t getSampleRate() override;
private:
    drwav m_wav;
    uint32_t m_sampleRate;
};