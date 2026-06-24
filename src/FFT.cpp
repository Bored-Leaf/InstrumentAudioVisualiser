#include <cmath>

#include "FFT.h"

std::vector<std::complex<float>> fft(std::vector<std::complex<float>> x) {
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
    std::vector<std::complex<float>> fft_even = fft(even);
    std::vector<std::complex<float>> fft_odd = fft(odd);

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