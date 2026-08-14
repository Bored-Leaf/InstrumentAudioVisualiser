#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <cassert>

#include "FFT.h"

namespace fft {
    namespace {
        struct HannWindow {
            std::vector<float> coefficients;
        };
        HannWindow hannWindow;
    };


    void generateHannCoefficients(const size_t windowSize) {
        hannWindow.coefficients.resize(windowSize);
        for(size_t n = 0;n < windowSize;n++) {
            hannWindow.coefficients[n] = 0.5 * (1.0F - std::cos((2.0F * M_PI) * n / (windowSize - 1.0F)));
        }
    }

    void applyHannWindow(std::vector<float> &fftInput) {
        assert(hannWindow.coefficients.size() != 0 && "coefficients must be initialised before use");
        assert(fftInput.size() == hannWindow.coefficients.size() && "fftInput.size() and hannWindow.size() must be identical");
        for (size_t i = 0;i < fftInput.size();i++) {
            fftInput[i] *= hannWindow.coefficients[i];
        }
    }

    std::vector<std::complex<float>> compute(std::vector<std::complex<float>> x) {
        int n = x.size();

        // Base case
        if (n == 1) {
            return x;
        }

        // Split into even and odd indicies
        std::vector<std::complex<float>> even{};
        std::vector<std::complex<float>> odd{};
        for (int i = 0;i < n;i += 2) {
            even.push_back(x[i]);
            odd.push_back(x[i + 1]);
        }

        // Recursively compute FFT of each half
        std::vector<std::complex<float>> fft_even = compute(even);
        std::vector<std::complex<float>> fft_odd = compute(odd);

        //merge results using twiddle factors
        std::vector<std::complex<float>> result(n);
        for (int k = 0;k < n/2;k++) {
            // Twiddle factor
            std::complex<float> t = std::polar<float>(1.0F, -2.0F * M_PI * k / n) * fft_odd[k];

            result[k] = fft_even[k] + t;
            result[k + n/2] = fft_even[k] - t;
        }

        return result;
    }

    float normaliseMagnitude(float magnitude) {
        float floor = -50;
        float ceiling = 20;

        // avoid log(0) with std::max()
        magnitude = std::max(magnitude, 1e-6F);
        magnitude = 20 * std::log10(magnitude);
        magnitude = (magnitude - floor) / (ceiling - floor);
        return std::clamp(magnitude, 0.0F, 1.0F);
    }

}