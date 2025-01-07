#include <lowpasscoefs.h>
#include <cmath>
#include <vector>
#include <iostream>

#ifndef M_PI
constexpr float M_PI = 3.14159265358979323846f;
#endif


// Function to calculate low-pass filter coefficients
std::vector<float> calculateLowPassCoefficients(
    float cutoffFrequency,  // 0.0 to 0.5, where 0.5 = Nyquist (half the sample rate)
    size_t filterLength     // # of filter coefficients (should be odd for symmetry!!)
) {
    std::vector<float> coefficients(filterLength);
    size_t midPoint = filterLength / 2; // Midpoint for symmetry

    // Generate sinc function values
    //the ideal low-pass filter in the time domain is a sinc function, scaled to the cutoff frequency, 
    //however, the sinc function extends infinitely in both directions, so we need to truncate it to a finite length
    //we truncate it to N samples, where N is the filter length and center around 0 (symmetric)
    for (size_t n = 0; n < filterLength; ++n) {
        if (n == midPoint) {
            // special case: sinc(0) = 1
            coefficients[n] = 2 * cutoffFrequency;
        } else {
            float x = M_PI * (n - midPoint); // Offset from midpoint
            coefficients[n] = std::sin(2 * cutoffFrequency * x) / x;
        }
    }

    // Hamming window!
    // Truncating the sinc function introduces artifacts. To reduce these artifacts, we apply a window function to the sinc function, we choose Hamming!

    for (size_t n = 0; n < filterLength; ++n) {
        float window = 0.54 - 0.46 * std::cos(2 * M_PI * n / (filterLength - 1));
        coefficients[n] *= window;
    }

    // Normalize coefficients, so they sum to 1
    float sum = 0.0;
    for (float coeff : coefficients) {
        sum += coeff;
    }
    for (float& coeff : coefficients) {
        coeff /= sum;
    }

    return coefficients;
}
