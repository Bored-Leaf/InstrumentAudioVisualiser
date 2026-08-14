#include <vector>
#include <complex>

namespace fft {
    void generateHannCoefficients(size_t windowSize);
    void applyHannWindow(std::vector<float> &fftInput);
    std::vector<std::complex<float>> compute(std::vector<std::complex<float>> x);
    float normaliseMagnitude(float magnitude);
};