#include "IAudioSource.h"
#include "dr_wav.h"

class WAVReader : public IAudioSource {
public:
    WAVReader(const char* filePath);
    virtual ~WAVReader(){}
    uint32_t getSampleRate() override;
private:
    drwav m_wav;
};