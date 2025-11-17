class IAudioSource {
public:
    virtual void getSampleRate() = 0;
    virtual void getSamples() = 0;
private:
    virtual bool isFinished() = 0;
    virtual bool isStero() = 0;
};